/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "sample_bit_bb_impl.h"

namespace gr {
  namespace cw_binary {

    sample_bit_bb::sptr
    sample_bit_bb::make(int bit_length)
    {
      return gnuradio::get_initial_sptr
        (new sample_bit_bb_impl(bit_length));
    }

    /*
     * The private constructor
     */
    sample_bit_bb_impl::sample_bit_bb_impl(int bit_length)
      : gr::sync_block("sample_bit_bb",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
        _started = false;
        _bit_length = bit_length;
    }

    /*
     * Our virtual destructor.
     */
    sample_bit_bb_impl::~sample_bit_bb_impl()
    {
    }

    int
    sample_bit_bb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const char *in = (const char *) input_items[0];
        char *out = (char *) output_items[0];

        for(int i = 0; i < noutput_items; i++) {
            out[i] = 0;
            if (in[i]) {
                _started = true;
                _count = _bit_length / 2;
            } else if (_started && _count++ == _bit_length) {
                out[i] = 1;
                _count = 0;
            }
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace cw_binary */
} /* namespace gr */

