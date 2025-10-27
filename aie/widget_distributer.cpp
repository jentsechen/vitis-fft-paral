#include <adf.h>
#include <aie_api/aie.hpp>

void widget_distributer(input_stream<cfloat> *in, output_stream<cfloat> *out0,
                        output_stream<cfloat> *out1) {
  for (int i = 0; i < 2048; i++) {
    writeincr(out0, readincr(in));
  }
  for (int i = 0; i < 2048; i++) {
    writeincr(out1, readincr(in));
  }
}

void widget_distributer_level0(input_stream<cfloat> *in,
                               output_stream<cfloat> *out0,
                               output_stream<cfloat> *out1) {
  for (int i = 0; i < 4096; i++) {
    writeincr(out0, readincr(in));
  }
  for (int i = 0; i < 4096; i++) {
    writeincr(out1, readincr(in));
  }
}

// void widget_distributer_window(
//     input_stream<cfloat> *__restrict in,
//     adf::output_buffer<cfloat, adf::extents<2048>> &__restrict out0,
//     adf::output_buffer<cfloat, adf::extents<2048>> &__restrict out1,
//     adf::output_buffer<cfloat, adf::extents<2048>> &__restrict out2,
//     adf::output_buffer<cfloat, adf::extents<2048>> &__restrict out3) {
//   cfloat *ptr0 = (cfloat *)aie::begin(out0);
//   cfloat *ptr1 = (cfloat *)aie::begin(out1);
//   cfloat *ptr2 = (cfloat *)aie::begin(out2);
//   cfloat *ptr3 = (cfloat *)aie::begin(out3);
//   for (int i = 0; i < 2048; i++) {
//     ptr0[i] = readincr(in);
//   }
//   for (int i = 0; i < 2048; i++) {
//     ptr1[i] = readincr(in);
//   }
//   for (int i = 0; i < 2048; i++) {
//     ptr2[i] = readincr(in);
//   }
//   for (int i = 0; i < 2048; i++) {
//     ptr3[i] = readincr(in);
//   }
// }

// void wgt_dist_lv1(input_stream<cfloat> *mv_in0, input_stream<cfloat> *mv_in1,
//                   output_cascade<cfloat> *data_out) {
//   for (int i = 0; i < 1024; i++) {
//     writeincr(data_out, readincr(mv_in0));
//     writeincr(data_out, readincr(mv_in1));
//   }
// }

// void wgt_dist_lv0(input_stream<cfloat> *mv_in0, input_stream<cfloat> *mv_in1,
//                   output_stream<cfloat> *mv_out0,
//                   output_stream<cfloat> *mv_out1,
//                   output_cascade<cfloat> *data_out) {
//   for (int i = 0; i < 1024; i++) {
//     writeincr(data_out, readincr(mv_in0));
//     writeincr(data_out, readincr(mv_in1));
//   }
//   for (int i = 0; i < 1024; i++) {
//     writeincr(mv_out0, readincr(mv_in0));
//     writeincr(mv_out1, readincr(mv_in1));
//   }
// }

void wdt_dist_io_strm_lv2(input_stream<cfloat> *mv_in,
                          output_stream<cfloat> *d_out0,
                          output_stream<cfloat> *d_out1) {
  for (int i = 0; i < 2048; i++) {
    writeincr(d_out0, readincr(mv_in));
  }
  for (int i = 0; i < 2048; i++) {
    writeincr(d_out1, readincr(mv_in));
  }
}
void wdt_dist_io_strm_lv1(input_stream<cfloat> *mv_in,
                          output_stream<cfloat> *mv_out,
                          output_stream<cfloat> *d_out) {
  for (int i = 0; i < 2048; i++) {
    writeincr(d_out, readincr(mv_in));
  }
  for (int i = 0; i < 4096; i++) {
    writeincr(mv_out, readincr(mv_in));
  }
}
void wdt_dist_io_strm_lv0(input_stream<cfloat> *mv_in,
                          output_stream<cfloat> *mv_out,
                          output_stream<cfloat> *d_out) {
  for (int i = 0; i < 2048; i++) {
    writeincr(d_out, readincr(mv_in));
  }
  for (int i = 0; i < 6144; i++) {
    writeincr(mv_out, readincr(mv_in));
  }
}