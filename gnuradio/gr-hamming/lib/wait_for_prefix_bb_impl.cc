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

#include <iostream>
#include <queue>
#include <gnuradio/io_signature.h>
#include "wait_for_prefix_bb_impl.h"

using namespace std;

namespace gr {
  namespace hamming {

    wait_for_prefix_bb::sptr
    wait_for_prefix_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new wait_for_prefix_bb_impl());
    }

    /*
     * The private constructor
     */
    wait_for_prefix_bb_impl::wait_for_prefix_bb_impl()
      : gr::block("wait_for_prefix_bb",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
        _prefix_seen = false;
        _length_seen = false;
        _count = 0;
        _length = 0;
        _history = queue<char>();
        // 1100110011001100
        _prefix = queue<char>();
        for (int i = 0; i < 16; i++) {
            _prefix.push((i / 2 + 1) % 2);
        }
    }

    /*
     * Our virtual destructor.
     */
    wait_for_prefix_bb_impl::~wait_for_prefix_bb_impl()
    {
    }

    void
    wait_for_prefix_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
    }

    int
    wait_for_prefix_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const char *in = (const char *) input_items[0];
        char *out = (char *) output_items[0];

        size_t j = 0;
        for (size_t i = 0; i < noutput_items; ++i) {
            if (!_prefix_seen) {
                _history.push(in[i]);
                if (_history.size() > _prefix.size()) {
                    _history.pop();
                }
                if (_history == _prefix) {
                    _prefix_seen = true;
                    _history = queue<char>();
                }
            } else if (!_length_seen) {
                    _length = (_length << 1) + in[i];
                if (++_count == 8) {
                    _length_seen = true;
                    _count = 0;
                }
            } else {
                if (_count++ < _length * 16) {
                    out[j++] = in[i];
                } else {
                    _prefix_seen = false;
                    _length_seen = false;
                    _count = 0;
                    _length = 0;
                }
            }
        }

        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return j;
    }

  } /* namespace hamming */
} /* namespace gr */

