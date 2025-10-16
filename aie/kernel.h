#ifndef __kernel__
#define __kernel__
#include <adf.h>

void widget_distributer(input_stream<cfloat> *in, output_stream<cfloat> *out0,
                        output_stream<cfloat> *out1);
void widget_collector(input_stream<cfloat> *in0, input_stream<cfloat> *in1,
                      output_stream<cfloat> *out);

#endif