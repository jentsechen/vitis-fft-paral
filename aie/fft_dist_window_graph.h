#ifndef __fft_dist_window_graph__
#define __fft_dist_window_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include "kernel.h"
#include <adf.h>

class FftDistWindowGraph : public adf::graph {
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
  adf::kernel distributer_kernel_0, distributer_kernel_1_0,
      distributer_kernel_1_1;
  adf::kernel collector_kernel_0, collector_kernel_1_0, collector_kernel_1_1;

public:
  //   adf::input_plio fft_in;
  //   adf::output_plio fft_out;
  adf::port<adf::input> fft_in;
  adf::port<adf::output> fft_out;
  FftDistWindowGraph() {
    // fft_in = input_plio::create("fft_in", adf::plio_64_bits);
    // fft_out = output_plio::create("fft_out", adf::plio_64_bits);

    distributer_kernel_0 = adf::kernel::create(widget_distributer_level0);
    source(distributer_kernel_0) = "./widget_distributer.cpp";
    runtime<ratio>(distributer_kernel_0) = 0.8;
    distributer_kernel_1_0 = adf::kernel::create(widget_distributer);
    source(distributer_kernel_1_0) = "./widget_distributer.cpp";
    runtime<ratio>(distributer_kernel_1_0) = 0.8;
    distributer_kernel_1_1 = adf::kernel::create(widget_distributer);
    source(distributer_kernel_1_1) = "./widget_distributer.cpp";
    runtime<ratio>(distributer_kernel_1_1) = 0.8;

    collector_kernel_0 = adf::kernel::create(widget_collector_level0);
    source(collector_kernel_0) = "./widget_collector.cpp";
    runtime<ratio>(collector_kernel_0) = 0.8;
    collector_kernel_1_0 = adf::kernel::create(widget_collector);
    source(collector_kernel_1_0) = "./widget_collector.cpp";
    runtime<ratio>(collector_kernel_1_0) = 0.8;
    collector_kernel_1_1 = adf::kernel::create(widget_collector);
    source(collector_kernel_1_1) = "./widget_collector.cpp";
    runtime<ratio>(collector_kernel_1_1) = 0.8;

    // adf::connect<>(fft_in.out[0], distributer_kernel_0.in[0]);
    // adf::connect<>(distributer_kernel_0.out[0], distributer_kernel_1_0.in[0]);
    // adf::connect<>(distributer_kernel_0.out[1], distributer_kernel_1_1.in[0]);
    // for (int i = 0; i < 2; i++) {
    //   adf::connect<>(distributer_kernel_1_0.out[i], fft_kernel[i].in[0]);
    //   adf::connect<>(fft_kernel[i].out[0], collector_kernel_1_0.in[i]);
    //   adf::connect<>(distributer_kernel_1_1.out[i], fft_kernel[i + 2].in[0]);
    //   adf::connect<>(fft_kernel[i + 2].out[0], collector_kernel_1_1.in[i]);
    // }
    // adf::connect<>(collector_kernel_1_0.out[0], collector_kernel_0.in[0]);
    // adf::connect<>(collector_kernel_1_1.out[0], collector_kernel_0.in[1]);
    // adf::connect<>(collector_kernel_0.out[0], fft_out.in[0]);
    adf::connect<>(fft_in, distributer_kernel_0.in[0]);
    adf::connect<>(distributer_kernel_0.out[0], distributer_kernel_1_0.in[0]);
    adf::connect<>(distributer_kernel_0.out[1], distributer_kernel_1_1.in[0]);
    for (int i = 0; i < 2; i++) {
      adf::connect<>(distributer_kernel_1_0.out[i], fft_kernel[i].in[0]);
      adf::connect<>(fft_kernel[i].out[0], collector_kernel_1_0.in[i]);
      adf::connect<>(distributer_kernel_1_1.out[i], fft_kernel[i + 2].in[0]);
      adf::connect<>(fft_kernel[i + 2].out[0], collector_kernel_1_1.in[i]);
    }
    adf::connect<>(collector_kernel_1_0.out[0], collector_kernel_0.in[0]);
    adf::connect<>(collector_kernel_1_1.out[0], collector_kernel_0.in[1]);
    adf::connect<>(collector_kernel_0.out[0], fft_out);
  }
};

#endif