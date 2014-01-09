/* -*- c++ -*- */

#define CW_BINARY_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "cw_binary_swig_doc.i"

%{
#include "cw_binary/change_fb.h"
#include "cw_binary/sample_bit_bb.h"
#include "cw_binary/sample_on_ctrl_ff.h"
%}


%include "cw_binary/change_fb.h"
GR_SWIG_BLOCK_MAGIC2(cw_binary, change_fb);
%include "cw_binary/sample_bit_bb.h"
GR_SWIG_BLOCK_MAGIC2(cw_binary, sample_bit_bb);
%include "cw_binary/sample_on_ctrl_ff.h"
GR_SWIG_BLOCK_MAGIC2(cw_binary, sample_on_ctrl_ff);
