#include "fft_one_paral_graph.h"
// #include "fft_two_paral_graph.h"
#include <adf.h>

using namespace adf;

FftOneParalGraph fft_one_paral_graph;
// FftTwoParalGraph fft_two_paral_graph;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv) {
  fft_one_paral_graph.init();
  fft_one_paral_graph.run(1);
  fft_one_paral_graph.end();
  //   fft_two_paral_graph.init();
  //   fft_two_paral_graph.run(1);
  //   fft_two_paral_graph.end();
  return 0;
};
#endif