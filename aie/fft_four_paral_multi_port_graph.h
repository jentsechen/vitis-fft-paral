#ifndef __fft_four_paral_multi_port_graph__
#define __fft_four_paral_multi_port_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include "kernel.h"
#include "par.h"
#include <adf.h>

class FftFourParalMultiPortGraph : public adf::graph {
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
      fft_kernel[N_PARAL];

public:
  //   adf::input_gmio fft_in[N_PARAL];
  //   adf::output_gmio fft_out[N_PARAL];
  adf::input_gmio fft_in_0, fft_in_1, fft_in_2, fft_in_3;
  adf::output_gmio fft_out_0, fft_out_1, fft_out_2, fft_out_3;
  FftFourParalMultiPortGraph() {
    // fft_in[0] = adf::input_gmio::create("fft_in_0", 64, 1000);
    // fft_in[1] = adf::input_gmio::create("fft_in_1", 64, 1000);
    // fft_in[2] = adf::input_gmio::create("fft_in_2", 64, 1000);
    // fft_in[3] = adf::input_gmio::create("fft_in_3", 64, 1000);
    // fft_out[0] = adf::output_gmio::create("fft_out_0", 64, 1000);
    // fft_out[1] = adf::output_gmio::create("fft_out_1", 64, 1000);
    // fft_out[2] = adf::output_gmio::create("fft_out_2", 64, 1000);
    // fft_out[3] = adf::output_gmio::create("fft_out_3", 64, 1000);
    // for (int i = 0; i < N_PARAL; i++) {
    //   adf::connect<>(fft_in[i].out[0], fft_kernel[i].in[0]);
    //   adf::connect<>(fft_kernel[i].out[0], fft_out[i].in[0]);
    // }
    fft_in_0 = adf::input_gmio::create("fft_in_0", 256, 1000);
    fft_in_1 = adf::input_gmio::create("fft_in_1", 256, 1000);
    fft_in_2 = adf::input_gmio::create("fft_in_2", 256, 1000);
    fft_in_3 = adf::input_gmio::create("fft_in_3", 256, 1000);
    fft_out_0 = adf::output_gmio::create("fft_out_0", 256, 1000);
    fft_out_1 = adf::output_gmio::create("fft_out_1", 256, 1000);
    fft_out_2 = adf::output_gmio::create("fft_out_2", 256, 1000);
    fft_out_3 = adf::output_gmio::create("fft_out_3", 256, 1000);
    adf::connect<>(fft_in_0.out[0], fft_kernel[0].in[0]);
    adf::connect<>(fft_in_1.out[0], fft_kernel[1].in[0]);
    adf::connect<>(fft_in_2.out[0], fft_kernel[2].in[0]);
    adf::connect<>(fft_in_3.out[0], fft_kernel[3].in[0]);
    adf::connect<>(fft_kernel[0].out[0], fft_out_0.in[0]);
    adf::connect<>(fft_kernel[1].out[0], fft_out_1.in[0]);
    adf::connect<>(fft_kernel[2].out[0], fft_out_2.in[0]);
    adf::connect<>(fft_kernel[3].out[0], fft_out_3.in[0]);
  }
};

#endif