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
import cw_binary_swig as cw_binary

class qa_sample_bit_bb (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        src_data = (0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0)
        expected_result = (0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0)

        src = blocks.vector_source_b(src_data)
        sample = cw_binary.sample_bit_bb(4)
        dst = blocks.vector_sink_b()
        self.tb.connect(src, sample)
        self.tb.connect(sample, dst)
        self.tb.run()
        result_data = dst.data()

        self.tb.run ()

        self.assertTupleEqual(expected_result, result_data)

if __name__ == '__main__':
    gr_unittest.run(qa_sample_bit_bb, "qa_sample_bit_bb.xml")
