#include <adf.h>
#include <aie_api/aie.hpp>

void widget_collector(input_stream<cfloat> *in0, input_stream<cfloat> *in1,
                      output_stream<cfloat> *out) {
  for (int i = 0; i < 2048; i++) {
    writeincr(out, readincr(in0));
  }
  for (int i = 0; i < 2048; i++) {
    writeincr(out, readincr(in1));
  }
}

void widget_collector_level0(input_stream<cfloat> *in0,
                             input_stream<cfloat> *in1,
                             output_stream<cfloat> *out) {
  for (int i = 0; i < 4096; i++) {
    writeincr(out, readincr(in0));
  }
  for (int i = 0; i < 4096; i++) {
    writeincr(out, readincr(in1));
  }
}

// void widget_collector_window(
//     adf::input_buffer<cfloat, adf::extents<2048>> &__restrict in0,
//     adf::input_buffer<cfloat, adf::extents<2048>> &__restrict in1,
//     adf::input_buffer<cfloat, adf::extents<2048>> &__restrict in2,
//     adf::input_buffer<cfloat, adf::extents<2048>> &__restrict in3,
//     output_stream<cfloat> *__restrict out) {
//   cfloat *ptr0 = (cfloat *)aie::begin(in0);
//   cfloat *ptr1 = (cfloat *)aie::begin(in1);
//   cfloat *ptr2 = (cfloat *)aie::begin(in2);
//   cfloat *ptr3 = (cfloat *)aie::begin(in3);
//   for (int i = 0; i < 2048; i++) {
//     writeincr(out, ptr0[i]);
//   }
//   for (int i = 0; i < 2048; i++) {
//     writeincr(out, ptr1[i]);
//   }
//   for (int i = 0; i < 2048; i++) {
//     writeincr(out, ptr2[i]);
//   }
//   for (int i = 0; i < 2048; i++) {
//     writeincr(out, ptr3[i]);
//   }
// }

// void wgt_coll_lv1(input_cascade<cfloat> *data_in,
//                   output_stream<cfloat> *mv_out0,
//                   output_stream<cfloat> *mv_out1) {
//   for (int i = 0; i < 1024; i++) {
//     writeincr(mv_out0, readincr(data_in));
//     writeincr(mv_out1, readincr(data_in));
//   }
// }

// void wgt_coll_lv0(input_cascade<cfloat> *data_in, input_stream<cfloat> *mv_in0,
//                   input_stream<cfloat> *mv_in1, output_stream<cfloat> *mv_out0,
//                   output_stream<cfloat> *mv_out1) {
//   for (int i = 0; i < 1024; i++) {
//     writeincr(mv_out0, readincr(data_in));
//     writeincr(mv_out1, readincr(data_in));
//   }
//   for (int i = 0; i < 1024; i++) {
//     writeincr(mv_out0, readincr(mv_in0));
//     writeincr(mv_out1, readincr(mv_in1));
//   }
// }

void wdt_coll_io_strm_lv2(input_stream<cfloat> *d_in0,
                          input_stream<cfloat> *d_in1,
                          output_stream<cfloat> *mv_out) {
  for (int i = 0; i < 2048; i++) {
    writeincr(mv_out, readincr(d_in0));
  }
  for (int i = 0; i < 2048; i++) {
    writeincr(mv_out, readincr(d_in1));
  }
}
void wdt_coll_io_strm_lv1(input_stream<cfloat> *mv_in,
                          input_stream<cfloat> *d_in,
                          output_stream<cfloat> *mv_out) {
  for (int i = 0; i < 2048; i++) {
    writeincr(mv_out, readincr(d_in));
  }
  for (int i = 0; i < 4096; i++) {
    writeincr(mv_out, readincr(mv_in));
  }
}
void wdt_coll_io_strm_lv0(input_stream<cfloat> *mv_in,
                          input_stream<cfloat> *d_in,
                          output_stream<cfloat> *mv_out) {
  for (int i = 0; i < 2048; i++) {
    writeincr(mv_out, readincr(d_in));
  }
  for (int i = 0; i < 6144; i++) {
    writeincr(mv_out, readincr(mv_in));
  }
}
