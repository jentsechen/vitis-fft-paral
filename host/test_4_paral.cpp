
#include "test.h"
#include <string>

std::vector<std::complex<float>> test(xrt::device &device,
                                      const xrt::uuid &uuid,
                                      const std::complex<float> *input_data,
                                      size_t input_n_sample) {
  // AIE kernels
  auto fft_graph_rhdl = xrt::graph(device, uuid, "fft_graph");
  const std::vector<std::string> graph_in_port{
      "fft_graph.fft_in_0", "fft_graph.fft_in_1", "fft_graph.fft_in_2",
      "fft_graph.fft_in_3"};
  const std::vector<std::string> graph_out_port{
      "fft_graph.fft_out_0", "fft_graph.fft_out_1", "fft_graph.fft_out_2",
      "fft_graph.fft_out_3"};

  // buffers
  std::vector<xrt::aie::bo> in_buf(n_paral), out_buf(n_paral);
  for (int i_paral = 0; i_paral < n_paral; i_paral++) {
    in_buf[i_paral] =
        xrt::aie::bo(device, n_sample_per_paral * n_byte_per_sample,
                     xrt::bo::flags::normal, 0);
    out_buf[i_paral] =
        xrt::aie::bo(device, n_sample_per_paral * n_byte_per_sample,
                     xrt::bo::flags::normal, 0);
  }

  // init. input buffer
  for (int i_paral = 0; i_paral < n_paral; i_paral++) {
    auto *in_arr = in_buf[i_paral].map<std::complex<float> *>();
    memset(in_arr, 0, n_sample_per_paral * n_byte_per_sample);
    if (i_paral * n_sample_per_paral < input_n_sample) {
      memcpy(in_arr,
             (const void *)((const char *)input_data +
                            i_paral * n_sample_per_paral * n_byte_per_sample),
             std::min((input_n_sample - i_paral * n_sample_per_paral) *
                          n_byte_per_sample,
                      n_sample_per_paral * n_byte_per_sample));
    }
  }
  std::vector<std::complex<float> *> out_arr(n_paral);
  for (int i_paral = 0; i_paral < n_paral; i_paral++) {
    out_arr[i_paral] = out_buf[i_paral].map<std::complex<float> *>();
  }

  auto start = std::chrono::high_resolution_clock::now();
  for (int iter = 0; iter < n_iter / (n_paral * n_batch_fft); ++iter) {
    for (int i_paral = 0; i_paral < n_paral; i_paral++) {
      in_buf[i_paral].async(graph_in_port[i_paral], XCL_BO_SYNC_BO_GMIO_TO_AIE,
                            n_sample_per_iter * n_byte_per_sample * n_batch_fft,
                            iter * n_sample_per_iter * n_byte_per_sample *
                                n_batch_fft);
    }
    fft_graph_rhdl.run(1);
    fft_graph_rhdl.wait();
    // for (int i_paral = 0; i_paral < n_paral; i_paral++) {
    //   out_buf[i_paral].async("fft_graph.fft_out_" + std::to_string(i_paral),
    //                          XCL_BO_SYNC_BO_AIE_TO_GMIO,
    //                          n_sample_per_iter * n_byte_per_sample,
    //                          iter * n_sample_per_iter * n_byte_per_sample);
    // }
    auto out_buf_run_0 = out_buf[0].async(
        graph_out_port[0], XCL_BO_SYNC_BO_AIE_TO_GMIO,
        n_sample_per_iter * n_byte_per_sample * n_batch_fft,
        iter * n_sample_per_iter * n_byte_per_sample * n_batch_fft);
    auto out_buf_run_1 = out_buf[1].async(
        graph_out_port[1], XCL_BO_SYNC_BO_AIE_TO_GMIO,
        n_sample_per_iter * n_byte_per_sample * n_batch_fft,
        iter * n_sample_per_iter * n_byte_per_sample * n_batch_fft);
    auto out_buf_run_2 = out_buf[2].async(
        graph_out_port[2], XCL_BO_SYNC_BO_AIE_TO_GMIO,
        n_sample_per_iter * n_byte_per_sample * n_batch_fft,
        iter * n_sample_per_iter * n_byte_per_sample * n_batch_fft);
    auto out_buf_run_3 = out_buf[3].async(
        graph_out_port[3], XCL_BO_SYNC_BO_AIE_TO_GMIO,
        n_sample_per_iter * n_byte_per_sample * n_batch_fft,
        iter * n_sample_per_iter * n_byte_per_sample * n_batch_fft);
    out_buf_run_0.wait();
    out_buf_run_1.wait();
    out_buf_run_2.wait();
    out_buf_run_3.wait();
  }
  fft_graph_rhdl.end();
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "time requirement: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     start)
                   .count()
            << " us" << std::endl;

  std::vector<std::complex<float>> output;
  output.reserve(n_iter * n_sample_per_iter * n_byte_per_sample);
  for (int i_paral = 0; i_paral < n_paral; i_paral++) {
    memcpy(output.data() + i_paral * n_sample_per_paral,
           reinterpret_cast<std::complex<float> *>(out_arr[i_paral]),
           n_sample_per_paral * n_byte_per_sample);
  }
  return output;
}
