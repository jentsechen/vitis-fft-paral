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