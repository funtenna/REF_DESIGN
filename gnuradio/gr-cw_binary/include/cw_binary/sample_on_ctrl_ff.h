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


#ifndef INCLUDED_CW_BINARY_SAMPLE_ON_CTRL_FF_H
#define INCLUDED_CW_BINARY_SAMPLE_ON_CTRL_FF_H

#include <cw_binary/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace cw_binary {

    /*!
     * \brief <+description of block+>
     * \ingroup cw_binary
     *
     */
    class CW_BINARY_API sample_on_ctrl_ff : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<sample_on_ctrl_ff> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of cw_binary::sample_on_ctrl_ff.
       *
       * To avoid accidental use of raw pointers, cw_binary::sample_on_ctrl_ff's
       * constructor is in a private implementation
       * class. cw_binary::sample_on_ctrl_ff::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace cw_binary
} // namespace gr

#endif /* INCLUDED_CW_BINARY_SAMPLE_ON_CTRL_FF_H */

