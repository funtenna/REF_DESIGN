#!/usr/bin/env python2
# -*- coding: utf-8 -*-
# 
# Copyright 2013 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest, blocks
import hamming_swig as hamming

class qa_decode_hamming_bb (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # test perfectly hamming coded
        hamming_vals = [
            (0b0000, 0b00000000),
            (0b1000, 0b11100001),
            (0b0100, 0b10011001),
            (0b1100, 0b01111000),
            (0b0010, 0b01010101),
            (0b1010, 0b10110100),
            (0b0110, 0b11001100),
            (0b1110, 0b00101101),
            (0b0001, 0b11010010),
            (0b1001, 0b00110011),
            (0b0101, 0b01001011),
            (0b1101, 0b10101010),
            (0b0011, 0b10000111),
            (0b1011, 0b01100110),
            (0b0111, 0b00011110),
            (0b1111, 0b11111111)
        ]
        expected_results, src_data = zip(*hamming_vals)

        src = blocks.vector_source_b(src_data)
        hamming_decode = hamming.decode_hamming_bb()
        dst = blocks.vector_sink_b()
        self.tb.connect(src, hamming_decode)
        self.tb.connect(hamming_decode, dst)
        self.tb.run ()
        # check data
        result_data = dst.data()
        self.assertTupleEqual(expected_results, result_data)

    def test_002_t (self):
        # test 1 error hamming coded
        hamming_vals = [
            (0b0000, 0b00000000 ^ 0b10000000),
            (0b1000, 0b11100001 ^ 0b01000000),
            (0b0100, 0b10011001 ^ 0b00100000),
            (0b1100, 0b01111000 ^ 0b00010000),
            (0b0010, 0b01010101 ^ 0b00001000),
            (0b1010, 0b10110100 ^ 0b00000100),
            (0b0110, 0b11001100 ^ 0b00000010),
            (0b1110, 0b00101101 ^ 0b00000001),
            (0b0001, 0b11010010 ^ 0b10000000),
            (0b1001, 0b00110011 ^ 0b01000000),
            (0b0101, 0b01001011 ^ 0b00100000),
            (0b1101, 0b10101010 ^ 0b00010000),
            (0b0011, 0b10000111 ^ 0b00001000),
            (0b1011, 0b01100110 ^ 0b00000100),
            (0b0111, 0b00011110 ^ 0b00000010),
            (0b1111, 0b11111111 ^ 0b00000001)
        ]
        expected_results, src_data = zip(*hamming_vals)

        src = blocks.vector_source_b(src_data)
        hamming_decode = hamming.decode_hamming_bb()
        dst = blocks.vector_sink_b()
        self.tb.connect(src, hamming_decode)
        self.tb.connect(hamming_decode, dst)
        self.tb.run ()
        # check data
        result_data = dst.data()
        self.assertTupleEqual(expected_results, result_data)


if __name__ == '__main__':
    gr_unittest.run(qa_decode_hamming_bb, "qa_decode_hamming_bb.xml")
