#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#define SAMPLE_BIT_WIDTH 64
#define FFT_SIZE 1024 * 4
#define N_PARAL 1

extern "C" {
void s2mm(ap_int<SAMPLE_BIT_WIDTH> *mem, int index,
          hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s) {
#pragma HLS INTERFACE m_axi port = mem offset = slave bundle = gmem
#pragma HLS INTERFACE axis port = s
#pragma HLS INTERFACE s_axilite port = mem bundle = control
#pragma HLS INTERFACE s_axilite port = index bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

  for (int i = 0; i < FFT_SIZE / N_PARAL; i++) {
#pragma HLS LOOP_TRIPCOUNT min = FFT_SIZE max = FFT_SIZE
#pragma HLS PIPELINE II = 1
    ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x = s.read();
    mem[index * FFT_SIZE / N_PARAL + i] = x.data;
  }
}
}
