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
#include "sample_on_ctrl_ff_impl.h"

namespace gr {
  namespace cw_binary {

    sample_on_ctrl_ff::sptr
    sample_on_ctrl_ff::make()
    {
      return gnuradio::get_initial_sptr
        (new sample_on_ctrl_ff_impl());
    }

    /*
     * The private constructor
     */
    sample_on_ctrl_ff_impl::sample_on_ctrl_ff_impl()
      : gr::block("sample_on_ctrl_ff",
              gr::io_signature::make2(2, 2, sizeof(float), sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {}

    /*
     * Our virtual destructor.
     */
    sample_on_ctrl_ff_impl::~sample_on_ctrl_ff_impl()
    {
    }

    void
    sample_on_ctrl_ff_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
        ninput_items_required[1] = noutput_items;
    }

    int
    sample_on_ctrl_ff_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        const char *ctrl = (const char *) input_items[1];
        float *out = (float *) output_items[0];

        // Do <+signal processing+>
        int j = 0;
        for (int i = 0; i < noutput_items; i++) {
            if (ctrl[i]) {
                out[j++] = in[i];
            }
        }

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return j;
    }

  } /* namespace cw_binary */
} /* namespace gr */

