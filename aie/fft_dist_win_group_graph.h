#ifndef __fft_dist_win_group_graph__
#define __fft_dist_win_group_graph__

#include "adf.h"
#include "fft_dist_window_graph.h"

class FftDistWinGroupGraph : public adf::graph {
private:
  FftDistWindowGraph fft_channel[4];

public:
  adf::input_plio fft_in0, fft_in1, fft_in2, fft_in3;
  adf::output_plio fft_out0, fft_out1, fft_out2, fft_out3;
  FftDistWinGroupGraph() {
    fft_in0 = adf::input_plio::create("fft_in0", adf::plio_64_bits);
    fft_in1 = adf::input_plio::create("fft_in1", adf::plio_64_bits);
    fft_in2 = adf::input_plio::create("fft_in2", adf::plio_64_bits);
    fft_in3 = adf::input_plio::create("fft_in3", adf::plio_64_bits);
    fft_out0 = adf::output_plio::create("fft_out0", adf::plio_64_bits);
    fft_out1 = adf::output_plio::create("fft_out1", adf::plio_64_bits);
    fft_out2 = adf::output_plio::create("fft_out2", adf::plio_64_bits);
    fft_out3 = adf::output_plio::create("fft_out3", adf::plio_64_bits);

    adf::connect<>(fft_in0.out[0], fft_channel[0].fft_in);
    adf::connect<>(fft_in1.out[0], fft_channel[1].fft_in);
    adf::connect<>(fft_in2.out[0], fft_channel[2].fft_in);
    adf::connect<>(fft_in3.out[0], fft_channel[3].fft_in);
    adf::connect<>(fft_channel[0].fft_out, fft_out0.in[0]);
    adf::connect<>(fft_channel[1].fft_out, fft_out1.in[0]);
    adf::connect<>(fft_channel[2].fft_out, fft_out2.in[0]);
    adf::connect<>(fft_channel[3].fft_out, fft_out3.in[0]);

    // adf::location<adf::graph>(fft_channel[0]) = adf::tile(4, 0);
    // adf::location<adf::graph>(fft_channel[1]) = adf::tile(8, 0);
    // adf::location<adf::graph>(fft_channel[2]) = adf::tile(12, 0);
    // adf::location<adf::graph>(fft_channel[3]) = adf::tile(16, 0);
  }
};

#endif