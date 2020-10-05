#ifndef SAMPLEOVERLAYER_H_
#define SAMPLEOVERLAYER_H_

#include "textparseoperations.h"
#include "wavhandler.h"

#define SUCCESS 0
#define ERROR   1

// Create the sample data that will be placed in the output file
int create_data( text_info_t* input, wave_info_t wave_input, wave_info_t* wave_output, int replace_sampling );

#endif // SAMPLEOVERLAYER_H_
