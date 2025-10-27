#ifndef __fft_one_paral_dist_acc_graph__
#define __fft_one_paral_dist_acc_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include "kernel.h"
#include "par.h"
#include <adf.h>

class FftOneParalDistAccGraph : public adf::graph {
private:
  xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<
      cfloat,                 // TT_DATA
      cfloat,                 // TT_TWIDDLE
      FFT_SIZE,               // TP_POINT_SIZE
      1,                      // TP_FFT_NIFFT
      0,                      // TP_SHIFT
      1,                      // TP_CASC_LEN
      0,                      // TP_DYN_PT_SIZE
      FFT_SIZE * N_BATCH_FFT, // TP_WINDOW_VSIZE
      0,                      // TP_API
      0,                      // TP_PARALLEL_POWER
      0                       // TP_USE_WIDGETS
      >
      fft_kernel[N_STACK];
  adf::kernel wdt_dist_io_strm_kernel[N_STACK];
  adf::kernel wdt_coll_io_strm_kernel[N_STACK];

public:
  adf::input_plio fft_in;
  adf::output_plio fft_out;
  FftOneParalDistAccGraph() {
    fft_in = input_plio::create("fft_in", adf::plio_64_bits);
    fft_out = output_plio::create("fft_out", adf::plio_64_bits);

    // can not be parameterized
    // wdt_dist_io_strm_kernel[0] = adf::kernel::create(wdt_dist_io_strm<3>);
    // wdt_dist_io_strm_kernel[1] = adf::kernel::create(wdt_dist_io_strm<2>);
    // wdt_dist_io_strm_kernel[2] = adf::kernel::create(wdt_dist_io_strm<1>);
    // wdt_coll_io_strm_kernel[0] = adf::kernel::create(wdt_coll_io_strm<3>);
    // wdt_coll_io_strm_kernel[1] = adf::kernel::create(wdt_coll_io_strm<2>);
    // wdt_coll_io_strm_kernel[2] = adf::kernel::create(wdt_coll_io_strm<1>);

    wdt_dist_io_strm_kernel[0] = adf::kernel::create(wdt_dist_io_strm<7>);
    wdt_dist_io_strm_kernel[1] = adf::kernel::create(wdt_dist_io_strm<6>);
    wdt_dist_io_strm_kernel[2] = adf::kernel::create(wdt_dist_io_strm<5>);
    wdt_dist_io_strm_kernel[3] = adf::kernel::create(wdt_dist_io_strm<4>);
    wdt_dist_io_strm_kernel[4] = adf::kernel::create(wdt_dist_io_strm<3>);
    wdt_dist_io_strm_kernel[5] = adf::kernel::create(wdt_dist_io_strm<2>);
    wdt_dist_io_strm_kernel[6] = adf::kernel::create(wdt_dist_io_strm<1>);

    wdt_coll_io_strm_kernel[0] = adf::kernel::create(wdt_coll_io_strm<7>);
    wdt_coll_io_strm_kernel[1] = adf::kernel::create(wdt_coll_io_strm<6>);
    wdt_coll_io_strm_kernel[2] = adf::kernel::create(wdt_coll_io_strm<5>);
    wdt_coll_io_strm_kernel[3] = adf::kernel::create(wdt_coll_io_strm<4>);
    wdt_coll_io_strm_kernel[4] = adf::kernel::create(wdt_coll_io_strm<3>);
    wdt_coll_io_strm_kernel[5] = adf::kernel::create(wdt_coll_io_strm<2>);
    wdt_coll_io_strm_kernel[6] = adf::kernel::create(wdt_coll_io_strm<1>);

    for (unsigned int i = 0; i < N_STACK - 1; i++) {
      source(wdt_dist_io_strm_kernel[i]) = "./widget_distributer.cpp";
      runtime<ratio>(wdt_dist_io_strm_kernel[i]) = 0.8;
    }
    for (unsigned int i = 0; i < N_STACK - 1; i++) {
      source(wdt_coll_io_strm_kernel[i]) = "./widget_collector.cpp";
      runtime<ratio>(wdt_coll_io_strm_kernel[i]) = 0.8;
    }

    adf::connect<>(fft_in.out[0], wdt_dist_io_strm_kernel[0].in[0]);
    for (int i = 0; i < N_STACK - 2; i++) {
      adf::connect<>(wdt_dist_io_strm_kernel[i].out[0],
                     wdt_dist_io_strm_kernel[i + 1].in[0]);
    }
    for (int i = 0; i < N_STACK - 2; i++) {
      adf::connect<>(wdt_dist_io_strm_kernel[i].out[1], fft_kernel[i].in[0]);
    }
    adf::connect<>(wdt_dist_io_strm_kernel[N_STACK - 2].out[0],
                   fft_kernel[N_STACK - 1].in[0]);
    adf::connect<>(wdt_dist_io_strm_kernel[N_STACK - 2].out[1],
                   fft_kernel[N_STACK - 2].in[0]);

    adf::connect<>(fft_kernel[N_STACK - 1].out[0],
                   wdt_coll_io_strm_kernel[N_STACK - 2].in[0]);
    adf::connect<>(fft_kernel[N_STACK - 2].out[0],
                   wdt_coll_io_strm_kernel[N_STACK - 2].in[1]);
    for (int i = 0; i < N_STACK - 2; i++) {
      adf::connect<>(wdt_coll_io_strm_kernel[i + 1].out[0],
                     wdt_coll_io_strm_kernel[i].in[0]);
    }
    for (int i = 0; i < N_STACK - 2; i++) {
      adf::connect<>(fft_kernel[i].out[0], wdt_coll_io_strm_kernel[i].in[1]);
    }
    adf::connect<>(wdt_coll_io_strm_kernel[0].out[0], fft_out.in[0]);
  }
};

#endif