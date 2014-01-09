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

#include <vector>
#include <gnuradio/io_signature.h>
#include "decode_hamming_bb_impl.h"

namespace gr {
  namespace hamming {

    decode_hamming_bb::sptr
    decode_hamming_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new decode_hamming_bb_impl());
    }

    /*
     * The private constructor
     */
    decode_hamming_bb_impl::decode_hamming_bb_impl()
      : gr::sync_block("decode_hamming_bb",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
        H = std::vector<char>(3);
        H[0] = 0b01010101;
        H[1] = 0b00110011;
        H[2] = 0b00001111;

        R = std::vector<char>(4);
        R[0] = 0b00010000;
        R[1] = 0b00000100;
        R[2] = 0b00000010;
        R[3] = 0b00000001;
    }

    /*
     * Our virtual destructor.
     */
    decode_hamming_bb_impl::~decode_hamming_bb_impl()
    {
    }

    int
    decode_hamming_bb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const char *in = (const char *) input_items[0];
        char *out = (char *) output_items[0];

        for (size_t i = 0; i < noutput_items; i++) {
            // get rid of parity bit
            char hamming_coded = (in[i] >> 1) & 0b01111111;

            char error_bit = 0;
            for (int j = 0; j < 3; j++) {
                char a = hamming_coded & H[j];
                int count = 0;
                while (a) {
                    a &= a - 1;
                    count++;
                }
                if (count % 2 != 0) {
                    error_bit |= 1 << j;
                }
            }
            if (error_bit) {
                hamming_coded = (in[i] ^ (1 << (8 - error_bit))) >> 1;
                hamming_coded &= 0b01111111;
            }

            char data = 0;
            for (int j = 0; j < 4; j++) {
                if (hamming_coded & R[j]) {
                    data += 1 << (3 - j);
                }
            }
            out[i] = data;
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace hamming */
} /* namespace gr */

