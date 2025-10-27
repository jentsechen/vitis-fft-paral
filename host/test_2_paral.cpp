
#include "test.h"

std::vector<std::complex<float>> test(xrt::device &device,
                                      const xrt::uuid &uuid,
                                      const std::complex<float> *input_data,
                                      size_t input_n_sample) {
  // AIE kernels
  auto fft_graph_rhdl = xrt::graph(device, uuid, "fft_graph");

  // PL kernels
  auto mm2s_0 = xrt::kernel(device, uuid, "mm2s:{mm2s_0}");
  auto mm2s_1 = xrt::kernel(device, uuid, "mm2s:{mm2s_1}");
  auto s2mm_0 = xrt::kernel(device, uuid, "s2mm:{s2mm_0}");
  auto s2mm_1 = xrt::kernel(device, uuid, "s2mm:{s2mm_1}");

  // buffers
  auto in_buf_0 =
      xrt::bo(device, block_size_in_byte / 2, xrt::bo::flags::normal, 0);
  auto in_buf_1 =
      xrt::bo(device, block_size_in_byte / 2, xrt::bo::flags::normal, 0);
  auto out_buf_0 =
      xrt::bo(device, block_size_in_byte / 2, xrt::bo::flags::normal, 0);
  auto out_buf_1 =
      xrt::bo(device, block_size_in_byte / 2, xrt::bo::flags::normal, 0);

  // init. input buffer
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
  auto *out_arr_0 = out_buf_0.map<std::complex<float> *>();
  auto *out_arr_1 = out_buf_1.map<std::complex<float> *>();

  auto start = std::chrono::high_resolution_clock::now();
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
  fft_graph_rhdl.end();
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "time requirement: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     start)
                   .count()
            << " us" << std::endl;

  std::vector<std::complex<float>> output(n_iter * n_sample_per_iter);
  for (int i = 0; i < n_iter * n_sample_per_iter / 2; i++) {
    output[2 * i] = out_arr_0[i];
    output[2 * i + 1] = out_arr_1[i];
  }
  return output;
}
