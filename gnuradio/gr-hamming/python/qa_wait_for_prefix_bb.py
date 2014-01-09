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

class qa_wait_for_prefix_bb (gr_unittest.TestCase):


    def setUp (self):
        self.tb = gr.top_block ()
        self.prefix = (1, 1, 0, 0) * 4

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        msg_len = 3
        msg = (1, 0, 1, 0, 1, 0, 1, 0) * msg_len * 2
        msg_len_bin = (0, 0, 0, 0, 0, 0, 1, 1)
        src_data = self.prefix + msg_len_bin + msg + (0, 0, 0, 0, 0, 0, 0, 0)
        expected_result = msg

        src = blocks.vector_source_b(src_data)
        wait = hamming.wait_for_prefix_bb()
        dst = blocks.vector_sink_b()

        self.tb.connect(src, wait)
        self.tb.connect(wait, dst)
        self.tb.run ()

        self.assertTupleEqual(expected_result, dst.data())

    def test_001_t (self):
        msg_len = 3
        msg = (1, 0, 1, 0, 1, 0, 1, 0) * msg_len * 2
        msg_len_bin = (0, 0, 0, 0, 0, 0, 1, 1)
        src_data = (0, 0, 0, 0) + self.prefix + msg_len_bin + msg + (0, 0, 0, 0, 0, 0, 0, 0)
        expected_result = msg * 2

        src = blocks.vector_source_b(src_data * 2)
        wait = hamming.wait_for_prefix_bb()
        dst = blocks.vector_sink_b()

        self.tb.connect(src, wait)
        self.tb.connect(wait, dst)
        self.tb.run ()

        self.assertTupleEqual(expected_result, dst.data())


if __name__ == '__main__':
    gr_unittest.run(qa_wait_for_prefix_bb, "qa_wait_for_prefix_bb.xml")
