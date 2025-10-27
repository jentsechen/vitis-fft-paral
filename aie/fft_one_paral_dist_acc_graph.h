#ifndef __fft_one_paral_dist_acc_graph__
#define __fft_one_paral_dist_acc_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include "kernel.h"
#include <adf.h>

class FftOneParalDistAccGraph : public adf::graph {
private:
  xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<cfloat, // TT_DATA
                                                     cfloat, // TT_TWIDDLE
                                                     1024,   // TP_POINT_SIZE
                                                     1,      // TP_FFT_NIFFT
                                                     0,      // TP_SHIFT
                                                     1,      // TP_CASC_LEN
                                                     0,      // TP_DYN_PT_SIZE
                                                     1024 *
                                                         2, // TP_WINDOW_VSIZE
                                                     0,     // TP_API
                                                     0,     // TP_PARALLEL_POWER
                                                     0      // TP_USE_WIDGETS
                                                     >
      fft_kernel[4];
  adf::kernel wdt_dist_io_strm_lv0_kernel, wdt_dist_io_strm_lv1_kernel,
      wdt_dist_io_strm_lv2_kernel;
  adf::kernel wdt_coll_io_strm_lv0_kernel, wdt_coll_io_strm_lv1_kernel,
      wdt_coll_io_strm_lv2_kernel;

public:
  adf::input_plio fft_in;
  adf::output_plio fft_out;
  FftOneParalDistAccGraph() {
    fft_in = input_plio::create("fft_in", adf::plio_64_bits);
    fft_out = output_plio::create("fft_out", adf::plio_64_bits);

    wdt_dist_io_strm_lv0_kernel = adf::kernel::create(wdt_dist_io_strm_lv0);
    source(wdt_dist_io_strm_lv0_kernel) = "./widget_distributer.cpp";
    runtime<ratio>(wdt_dist_io_strm_lv0_kernel) = 0.8;
    wdt_dist_io_strm_lv1_kernel = adf::kernel::create(wdt_dist_io_strm_lv1);
    source(wdt_dist_io_strm_lv1_kernel) = "./widget_distributer.cpp";
    runtime<ratio>(wdt_dist_io_strm_lv1_kernel) = 0.8;
    wdt_dist_io_strm_lv2_kernel = adf::kernel::create(wdt_dist_io_strm_lv2);
    source(wdt_dist_io_strm_lv2_kernel) = "./widget_distributer.cpp";
    runtime<ratio>(wdt_dist_io_strm_lv2_kernel) = 0.8;
    wdt_coll_io_strm_lv0_kernel = adf::kernel::create(wdt_coll_io_strm_lv0);
    source(wdt_coll_io_strm_lv0_kernel) = "./widget_collector.cpp";
    runtime<ratio>(wdt_coll_io_strm_lv0_kernel) = 0.8;
    wdt_coll_io_strm_lv1_kernel = adf::kernel::create(wdt_coll_io_strm_lv1);
    source(wdt_coll_io_strm_lv1_kernel) = "./widget_collector.cpp";
    runtime<ratio>(wdt_coll_io_strm_lv1_kernel) = 0.8;
    wdt_coll_io_strm_lv2_kernel = adf::kernel::create(wdt_coll_io_strm_lv2);
    source(wdt_coll_io_strm_lv2_kernel) = "./widget_collector.cpp";
    runtime<ratio>(wdt_coll_io_strm_lv2_kernel) = 0.8;

    adf::connect<>(fft_in.out[0], wdt_dist_io_strm_lv0_kernel.in[0]);
    adf::connect<>(wdt_dist_io_strm_lv0_kernel.out[0],
                   wdt_dist_io_strm_lv1_kernel.in[0]);
    adf::connect<>(wdt_dist_io_strm_lv0_kernel.out[1], fft_kernel[0].in[0]);
    adf::connect<>(wdt_dist_io_strm_lv1_kernel.out[0],
                   wdt_dist_io_strm_lv2_kernel.in[0]);
    adf::connect<>(wdt_dist_io_strm_lv1_kernel.out[1], fft_kernel[1].in[0]);
    adf::connect<>(wdt_dist_io_strm_lv2_kernel.out[0], fft_kernel[2].in[0]);
    adf::connect<>(wdt_dist_io_strm_lv2_kernel.out[1], fft_kernel[3].in[0]);

    adf::connect<>(fft_kernel[2].out[0], wdt_coll_io_strm_lv2_kernel.in[0]);
    adf::connect<>(fft_kernel[3].out[0], wdt_coll_io_strm_lv2_kernel.in[1]);
    adf::connect<>(wdt_coll_io_strm_lv2_kernel.out[0],
                   wdt_coll_io_strm_lv1_kernel.in[0]);
    adf::connect<>(fft_kernel[1].out[0], wdt_coll_io_strm_lv1_kernel.in[1]);
    adf::connect<>(wdt_coll_io_strm_lv1_kernel.out[0],
                   wdt_coll_io_strm_lv0_kernel.in[0]);
    adf::connect<>(fft_kernel[0].out[0], wdt_coll_io_strm_lv0_kernel.in[1]);
    adf::connect<>(wdt_coll_io_strm_lv0_kernel.out[0], fft_out.in[0]);
  }
};

#endif