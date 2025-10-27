#include "./cnpy/cnpy.cpp"
#include "./cnpy/cnpy.h"
#include "test.h"
#include "xrt/xrt_aie.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_kernel.h"
#include <chrono>
#include <complex>

int main(int argc, char **argv) {
  cnpy::NpyArray arr = cnpy::npy_load(argv[2]);
  assert(arr.shape[0] > 0 && arr.shape[1] > 0);
  //   std::cout << "number of values: " << arr.shape[1] << std::endl;
  const std::complex<float> *data_in = arr.data<std::complex<float>>();

  char *xclbinFilename = argv[1];
  auto device = xrt::device(0);
  auto uuid = device.load_xclbin(xclbinFilename);

  auto output = test(device, uuid, data_in, arr.shape[1]);
  cnpy::npy_save("output.npy", output.data(), {n_iter * n_sample_per_iter},
                 "w");

  std::cout << "Done!" << std::endl;

  return 0;
};