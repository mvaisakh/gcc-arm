/* classes: h_files */

#ifndef SCM_UNIFORM_H
#define SCM_UNIFORM_H

/* Copyright (C) 1995,1996,1997,1999,2000,2001, 2004, 2006, 2008, 2009,
 * 2013, 2014 Free Software Foundation, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */



#include "libguile/__scm.h"



/*
 * Uniform vectors contain unboxed values. They are not necessarily contiguous.
 */

SCM_INTERNAL const size_t scm_i_array_element_type_sizes[];
#define SCM_ARRAY_ELEMENT_TYPE_IS_UNBOXED(t)    \
  (scm_i_array_element_type_sizes[(t)] != 0)

/* type size in bytes */
SCM_API size_t scm_array_handle_uniform_element_size (scm_t_array_handle *h);
/* type size in bits */
SCM_API size_t scm_array_handle_uniform_element_bit_size (scm_t_array_handle *h);

SCM_API const void *scm_array_handle_uniform_elements (scm_t_array_handle *h);
SCM_API void *scm_array_handle_uniform_writable_elements (scm_t_array_handle *h);

SCM_INTERNAL void scm_init_uniform (void);


#endif  /* SCM_UNIFORM_H */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
