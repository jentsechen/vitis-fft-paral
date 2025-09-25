#ifndef __fft_one_paral_graph__
#define __fft_one_paral_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include <adf.h>

class FftOneParalGraph : public adf::graph {
private:
  xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<cfloat, // TT_DATA
                                                     cfloat, // TT_TWIDDLE
                                                     1024,   // TP_POINT_SIZE
                                                     1,      // TP_FFT_NIFFT
                                                     0,      // TP_SHIFT
                                                     1,      // TP_CASC_LEN
                                                     0,      // TP_DYN_PT_SIZE
                                                     1024,   // TP_WINDOW_VSIZE
                                                     0,      // TP_API
                                                     0, // TP_PARALLEL_POWER
                                                     0  // TP_USE_WIDGETS
                                                     >
      fft_kernel;

public:
  adf::input_gmio fft_one_paral_in;
  adf::output_gmio fft_one_paral_out;
  FftOneParalGraph() {
    fft_one_paral_in = adf::input_gmio::create("fft_one_paral_in", 64, 1000);
    fft_one_paral_out = adf::output_gmio::create("fft_one_paral_out", 64, 1000);
    adf::connect<>(fft_one_paral_in.out[0], fft_kernel.in[0]);
    adf::connect<>(fft_kernel.out[0], fft_one_paral_out.in[0]);
  }
};

#endif