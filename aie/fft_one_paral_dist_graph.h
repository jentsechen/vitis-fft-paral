#ifndef __fft_one_paral_graph__
#define __fft_one_paral_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include "kernel.h"
#include <adf.h>

class FftOneParalDistGraph : public adf::graph {
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
      fft_kernel[2];
  adf::kernel distributer_kernel, collector_kernel;

public:
  adf::input_plio fft_in;
  adf::output_plio fft_out;
  FftOneParalDistGraph() {
    fft_in = input_plio::create("fft_in", adf::plio_64_bits);
    fft_out = output_plio::create("fft_out", adf::plio_64_bits);

    distributer_kernel = adf::kernel::create(widget_distributer);
    source(distributer_kernel) = "./widget_distributer.cpp";
    runtime<ratio>(distributer_kernel) = 0.8;
    collector_kernel = adf::kernel::create(widget_collector);
    source(collector_kernel) = "./widget_collector.cpp";
    runtime<ratio>(collector_kernel) = 0.8;

    adf::connect<>(fft_in.out[0], distributer_kernel.in[0]);
    for (int i = 0; i < 2; i++) {
      adf::connect<>(distributer_kernel.out[i], fft_kernel[i].in[0]);
      adf::connect<>(fft_kernel[i].out[0], collector_kernel.in[i]);
    }
    adf::connect<>(collector_kernel.out[0], fft_out.in[0]);
  }
};

#endif