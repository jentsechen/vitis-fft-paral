#ifndef __kernel__
#define __kernel__
#include <adf.h>

void widget_distributer(input_stream<cfloat> *in, output_stream<cfloat> *out0,
                        output_stream<cfloat> *out1);
void widget_collector(input_stream<cfloat> *in0, input_stream<cfloat> *in1,
                      output_stream<cfloat> *out);

void widget_distributer_level0(input_stream<cfloat> *in,
                               output_stream<cfloat> *out0,
                               output_stream<cfloat> *out1);
void widget_collector_level0(input_stream<cfloat> *in0,
                             input_stream<cfloat> *in1,
                             output_stream<cfloat> *out);

// void widget_distributer_window(
//     input_stream<cfloat> *__restrict in,
//     adf::output_buffer<cfloat, adf::extents<2048>> &__restrict out0,
//     adf::output_buffer<cfloat, adf::extents<2048>> &__restrict out1,
//     adf::output_buffer<cfloat, adf::extents<2048>> &__restrict out2,
//     adf::output_buffer<cfloat, adf::extents<2048>> &__restrict out3);
// void widget_collector_window(
//     adf::input_buffer<cfloat, adf::extents<2048>> &__restrict in0,
//     adf::input_buffer<cfloat, adf::extents<2048>> &__restrict in1,
//     adf::input_buffer<cfloat, adf::extents<2048>> &__restrict in2,
//     adf::input_buffer<cfloat, adf::extents<2048>> &__restrict in3,
//     output_stream<cfloat> *__restrict out);

#endif