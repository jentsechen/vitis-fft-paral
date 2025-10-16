// #include "fft_one_paral_graph.h"
// #include "fft_one_paral_dist_graph.h"
#include "fft_dist_window_graph.h"
// #include "fft_one_paral_stream_graph.h"
// #include "fft_two_paral_graph.h"
#include <adf.h>

using namespace adf;

// FftOneParalGraph fft_graph;
// FftOneParalDistGraph fft_graph;
FftDistWindowGraph fft_graph;
// FftOneParalStreamGraph fft_graph;
// FftTwoParalGraph fft_graph;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv) {
  fft_graph.init();
  fft_graph.run(1);
  fft_graph.end();
  return 0;
};
#endif