#include "./cnpy/cnpy.cpp"
#include "./cnpy/cnpy.h"
#include "xrt/xrt_aie.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_kernel.h"
#include <chrono>
#include <complex>
#define N_PARAL 1

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

  auto fft_graph_rhdl = xrt::graph(device, uuid, "fft_graph");
#if N_PARAL == 1
  //   auto in_buf =
  //       xrt::aie::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  auto in_buf = xrt::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  auto *in_arr = in_buf.map<std::complex<float> *>();
  memset(in_arr, 0, block_size_in_byte);
  memcpy(in_arr, data_in,
         std::min(arr.shape[1] * n_byte_per_sample, block_size_in_byte));
  //   auto out_buf =
  //       xrt::aie::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  auto out_buf = xrt::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  auto *out_arr = out_buf.map<std::complex<float> *>();
  auto mm2s = xrt::kernel(device, uuid, "mm2s:{mm2s_0}");
  auto s2mm = xrt::kernel(device, uuid, "s2mm:{s2mm_0}");
#elif N_PARAL == 2
  auto in_buf_0 =
      xrt::bo(device, block_size_in_byte / 2, xrt::bo::flags::normal, 0);
  auto in_buf_1 =
      xrt::bo(device, block_size_in_byte / 2, xrt::bo::flags::normal, 0);
  auto *in_arr_0 = in_buf_0.map<std::complex<float> *>();
  auto *in_arr_1 = in_buf_1.map<std::complex<float> *>();
  memset(in_arr_0, 0, block_size_in_byte / 2);
  memset(in_arr_1, 0, block_size_in_byte / 2);
  int in_idx_0 = 0, in_idx_1 = 0;
  for (int i = 0; i < arr.shape[1]; i++) {
    if (i % 2 == 0) {
      in_arr_0[in_idx_0] = data_in[i];
      in_idx_0++;
    } else {
      in_arr_1[in_idx_1] = data_in[i];
      in_idx_1++;
    }
  }
  auto out_buf_0 =
      xrt::bo(device, block_size_in_byte / 2, xrt::bo::flags::normal, 0);
  auto out_buf_1 =
      xrt::bo(device, block_size_in_byte / 2, xrt::bo::flags::normal, 0);
  auto *out_arr_0 = out_buf_0.map<std::complex<float> *>();
  auto *out_arr_1 = out_buf_1.map<std::complex<float> *>();
  auto mm2s_0 = xrt::kernel(device, uuid, "mm2s:{mm2s_0}");
  auto mm2s_1 = xrt::kernel(device, uuid, "mm2s:{mm2s_1}");
  auto s2mm_0 = xrt::kernel(device, uuid, "s2mm:{s2mm_0}");
  auto s2mm_1 = xrt::kernel(device, uuid, "s2mm:{s2mm_1}");
#endif

  auto start = std::chrono::high_resolution_clock::now();
#if N_PARAL == 1
  //   int offset = 0;
  //   for (int iter = 0; iter < n_iter; ++iter) {
  //     std::cout << "iter: " << iter << std::endl;
  //     auto in_buf_run =
  //         in_buf.async("fft_graph.fft_in", XCL_BO_SYNC_BO_GMIO_TO_AIE,
  //                      n_sample_per_iter * n_byte_per_sample, offset);
  //     fft_graph_rhdl.run(1);
  //     fft_graph_rhdl.wait();
  //     auto out_buf_run =
  //         out_buf.async("fft_graph.fft_out", XCL_BO_SYNC_BO_AIE_TO_GMIO,
  //                       n_sample_per_iter * n_byte_per_sample, offset);
  //     out_buf_run.wait();
  //     offset += (n_sample_per_iter * n_byte_per_sample);
  //   }
  in_buf.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  for (int iter = 0; iter < n_iter / 8; ++iter) {
    auto mm2s_rhdl = mm2s(in_buf, iter);
    auto s2mm_rhdl = s2mm(out_buf, iter);
    fft_graph_rhdl.run(1);
    mm2s_rhdl.wait();
    s2mm_rhdl.wait();
    fft_graph_rhdl.wait();
  }
  out_buf.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
#elif N_PARAL == 2
  in_buf_0.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  in_buf_1.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  for (int iter = 0; iter < n_iter / 4; ++iter) {
    auto mm2s_0_rhdl = mm2s_0(in_buf_0, iter);
    auto mm2s_1_rhdl = mm2s_1(in_buf_1, iter);
    auto s2mm_0_rhdl = s2mm_0(out_buf_0, iter);
    auto s2mm_1_rhdl = s2mm_1(out_buf_1, iter);
    fft_graph_rhdl.run(1);
    mm2s_0_rhdl.wait();
    mm2s_1_rhdl.wait();
    s2mm_0_rhdl.wait();
    s2mm_1_rhdl.wait();
    fft_graph_rhdl.wait();
  }
  out_buf_0.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  out_buf_1.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
#endif
  fft_graph_rhdl.end();
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "time requirement: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     start)
                   .count()
            << " us" << std::endl;

#if N_PARAL == 1
  std::vector<std::complex<float>> output(
      reinterpret_cast<std::complex<float> *>(out_arr),
      reinterpret_cast<std::complex<float> *>(out_arr) +
          n_iter * n_sample_per_iter);
#elif N_PARAL == 2
  std::vector<std::complex<float>> output(n_iter * n_sample_per_iter);
  for (int i = 0; i < n_iter * n_sample_per_iter / 2; i++) {
    output[2 * i] = out_arr_0[i];
    output[2 * i + 1] = out_arr_1[i];
  }
#endif
  cnpy::npy_save("output.npy", output.data(), {n_iter * n_sample_per_iter},
                 "w");

  std::cout << "Done!" << std::endl;

  return 0;
};