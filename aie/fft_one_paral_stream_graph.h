#ifndef __fft_one_paral_stream_graph__
#define __fft_one_paral_stream_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include <adf.h>

class FftOneParalStreamGraph : public adf::graph {
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
                                                     1,     // TP_API
                                                     0,     // TP_PARALLEL_POWER
                                                     1      // TP_USE_WIDGETS
                                                     >
      fft_kernel;

public:
  adf::input_plio fft_in_0, fft_in_1;
  adf::output_plio fft_out_0, fft_out_1;
  FftOneParalStreamGraph() {
    fft_in_0 = input_plio::create("fft_in_0", adf::plio_64_bits);
    fft_in_1 = input_plio::create("fft_in_1", adf::plio_64_bits);
    fft_out_0 = output_plio::create("fft_out_0", adf::plio_64_bits);
    fft_out_1 = output_plio::create("fft_out_1", adf::plio_64_bits);

    adf::connect<>(fft_in_0.out[0], fft_kernel.in[0]);
    adf::connect<>(fft_in_1.out[0], fft_kernel.in[1]);
    adf::connect<>(fft_kernel.out[0], fft_out_0.in[0]);
    adf::connect<>(fft_kernel.out[1], fft_out_1.in[0]);
  }
};

#endif