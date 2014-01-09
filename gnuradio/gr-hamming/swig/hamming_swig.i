/* -*- c++ -*- */

#define HAMMING_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "hamming_swig_doc.i"

%{
#include "hamming/decode_hamming_bb.h"
#include "hamming/wait_for_prefix_bb.h"
%}


%include "hamming/decode_hamming_bb.h"
GR_SWIG_BLOCK_MAGIC2(hamming, decode_hamming_bb);


%include "hamming/wait_for_prefix_bb.h"
GR_SWIG_BLOCK_MAGIC2(hamming, wait_for_prefix_bb);
