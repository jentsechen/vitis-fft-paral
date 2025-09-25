#include "./cnpy/cnpy.cpp"
#include "./cnpy/cnpy.h"
#include "xrt/xrt_aie.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_kernel.h"
#include <chrono>
#include <complex>

const size_t n_iter = 1024;
const size_t n_byte_per_sample = 8;
const size_t n_sample_per_iter = 1024;
const size_t block_size_in_byte =
    n_iter * n_sample_per_iter * n_byte_per_sample;

int main(int argc, char **argv) {
  cnpy::NpyArray arr = cnpy::npy_load(argv[2]);
  assert(arr.shape[0] > 0 && arr.shape[1] > 0);
  //   std::cout << "number of values: " << arr.shape[1] << std::endl;
  const std::complex<float> *data_in = arr.data<std::complex<float>>();

  char *xclbinFilename = argv[1];
  auto device = xrt::device(0);
  auto uuid = device.load_xclbin(xclbinFilename);

  auto fft_no_paral_graph_rhdl = xrt::graph(device, uuid, "fft_no_paral_graph");
  auto in_buf =
      xrt::aie::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  auto *in_arr = in_buf.map<__uint64_t *>();
  memset(in_arr, 0, block_size_in_byte);
  memcpy(in_arr, data_in,
         std::min(arr.shape[1] * n_byte_per_sample, block_size_in_byte));
  auto out_buf =
      xrt::aie::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  auto *out_arr = out_buf.map<__uint64_t *>();

  auto start = std::chrono::high_resolution_clock::now();
  int offset = 0;
  for (int iter = 0; iter < n_iter; ++iter) {
    auto in_buf_run = in_buf.async(
        "fft_no_paral_graph.fft_no_paral_in", XCL_BO_SYNC_BO_GMIO_TO_AIE,
        n_sample_per_iter * n_byte_per_sample, offset);
    fft_no_paral_graph_rhdl.run(1);
    fft_no_paral_graph_rhdl.wait();
    auto out_buf_run = out_buf.async(
        "fft_no_paral_graph.fft_no_paral_out", XCL_BO_SYNC_BO_AIE_TO_GMIO,
        n_sample_per_iter * n_byte_per_sample, offset);
    out_buf_run.wait();
    offset += (n_sample_per_iter * n_byte_per_sample);
  }
  fft_no_paral_graph_rhdl.end();
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "time requirement: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     start)
                   .count()
            << " us" << std::endl;

  std::vector<std::complex<float>> output(
      reinterpret_cast<std::complex<float> *>(out_arr),
      reinterpret_cast<std::complex<float> *>(out_arr) +
          n_iter * n_sample_per_iter);
  cnpy::npy_save("output.npy", output.data(), {n_iter * n_sample_per_iter},
                 "w");

  std::cout << "Done!" << std::endl;

  return 0;
};