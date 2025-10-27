
#include "test.h"

std::vector<std::complex<float>> test(xrt::device &device,
                                      const xrt::uuid &uuid,
                                      const std::complex<float> *input_data,
                                      size_t input_n_sample) {
  // AIE kernels
  auto fft_graph_rhdl = xrt::graph(device, uuid, "fft_graph");

  // PL kernels
  auto mm2s = xrt::kernel(device, uuid, "mm2s:{mm2s_0}");
  auto s2mm = xrt::kernel(device, uuid, "s2mm:{s2mm_0}");

  // buffers
  auto in_buf = xrt::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  auto out_buf = xrt::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);

  // init. input buffer
  auto *in_arr = in_buf.map<std::complex<float> *>();
  memset(in_arr, 0, block_size_in_byte);
  memcpy(in_arr, input_data,
         std::min(input_n_sample * n_byte_per_sample, block_size_in_byte));
  auto *out_arr = out_buf.map<std::complex<float> *>();

  auto start = std::chrono::high_resolution_clock::now();
  in_buf.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  for (int iter = 0; iter < n_iter / 16; ++iter) {
    auto mm2s_rhdl = mm2s(in_buf, iter);
    auto s2mm_rhdl = s2mm(out_buf, iter);
    fft_graph_rhdl.run(1);
    mm2s_rhdl.wait();
    s2mm_rhdl.wait();
    fft_graph_rhdl.wait();
  }
  out_buf.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  fft_graph_rhdl.end();
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
  return output;
}
