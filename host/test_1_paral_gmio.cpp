#include "test.h"

std::vector<std::complex<float>> test(xrt::device &device,
                                      const xrt::uuid &uuid,
                                      const std::complex<float> *input_data,
                                      size_t input_n_sample) {
  // AIE kernels
  auto fft_graph_rhdl = xrt::graph(device, uuid, "fft_graph");

  // buffers
  auto in_buf =
      xrt::aie::bo(device, n_iter * n_sample_per_iter * n_byte_per_sample,
                   xrt::bo::flags::normal, 0);
  auto out_buf =
      xrt::aie::bo(device, n_iter * n_sample_per_iter * n_byte_per_sample,
                   xrt::bo::flags::normal, 0);

  // init. input buffer
  auto *in_arr = in_buf.map<std::complex<float> *>();
  memset(in_arr, 0, block_size_in_byte);
  memcpy(in_arr, input_data,
         std::min(input_n_sample * n_byte_per_sample, block_size_in_byte));
  auto *out_arr = out_buf.map<std::complex<float> *>();

  auto start = std::chrono::high_resolution_clock::now();
  for (int iter = 0; iter < n_iter / (n_stack * n_batch_fft); ++iter) {
    in_buf.async("fft_graph.fft_in", XCL_BO_SYNC_BO_GMIO_TO_AIE,
                 n_sample_per_iter * n_byte_per_sample * n_stack * n_batch_fft,
                 iter * n_sample_per_iter * n_byte_per_sample * n_stack *
                     n_batch_fft);
    fft_graph_rhdl.run(1);
    fft_graph_rhdl.wait();
    auto out_buf_run = out_buf.async(
        "fft_graph.fft_out", XCL_BO_SYNC_BO_AIE_TO_GMIO,
        n_sample_per_iter * n_byte_per_sample * n_stack * n_batch_fft,
        iter * n_sample_per_iter * n_byte_per_sample * n_stack * n_batch_fft);
    out_buf_run.wait();
  }
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
