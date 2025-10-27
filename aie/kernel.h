#ifndef __kernel__
#define __kernel__
#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>

#define FFT_SIZE 1024
#define N_BATCH_FFT 2

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

// void wgt_dist_lv1(input_stream<cfloat> *mv_in0, input_stream<cfloat> *mv_in1,
//                   output_cascade<cfloat> *data_out);
// void wgt_dist_lv0(input_stream<cfloat> *mv_in0, input_stream<cfloat> *mv_in1,
//                   output_stream<cfloat> *mv_out0,
//                   output_stream<cfloat> *mv_out1,
//                   output_cascade<cfloat> *data_out);

// void wgt_coll_lv1(input_cascade<cfloat> *data_in,
//                   output_stream<cfloat> *mv_out0,
//                   output_stream<cfloat> *mv_out1);
// void wgt_coll_lv0(input_stream<cfloat> *mv_in0, input_stream<cfloat> *mv_in1,
//                   input_cascade<cfloat> *data_in,
//                   output_stream<cfloat> *mv_out0,
//                   output_stream<cfloat> *mv_out1);

template <unsigned int N_BATCH_MOVE>
void wdt_dist_io_strm(input_stream<cfloat> *in, output_stream<cfloat> *out0,
                      output_stream<cfloat> *out1);

template <unsigned int N_BATCH_MOVE>
void wdt_coll_io_strm(input_stream<cfloat> *in0, input_stream<cfloat> *in1,
                      output_stream<cfloat> *out);

#endif