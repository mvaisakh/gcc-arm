/* classes: h_files */

#ifndef SCM_VECTORS_H
#define SCM_VECTORS_H

/* Copyright (C) 1995,1996,1998,2000,2001,2002,2004,2005, 2006, 2008, 2009, 2011, 2014 Free Software Foundation, Inc.
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



SCM_API SCM scm_vector_p (SCM x);
SCM_API SCM scm_vector_length (SCM v);
SCM_API SCM scm_vector (SCM l);
SCM_API SCM scm_vector_ref (SCM v, SCM k);
SCM_API SCM scm_vector_set_x (SCM v, SCM k, SCM obj);
SCM_API SCM scm_make_vector (SCM k, SCM fill);
SCM_API SCM scm_vector_to_list (SCM v);
SCM_API SCM scm_vector_fill_x (SCM v, SCM fill_x);
SCM_API SCM scm_vector_move_left_x (SCM vec1, SCM start1, SCM end1,
				    SCM vec2, SCM start2);
SCM_API SCM scm_vector_move_right_x (SCM vec1, SCM start1, SCM end1, 
				     SCM vec2, SCM start2);
SCM_API SCM scm_vector_copy (SCM vec);

SCM_API int scm_is_vector (SCM obj);
SCM_API int scm_is_simple_vector (SCM obj);
SCM_API SCM scm_c_make_vector (size_t len, SCM fill);
SCM_API size_t scm_c_vector_length (SCM vec);
SCM_API SCM scm_c_vector_ref (SCM vec, size_t k);
SCM_API void scm_c_vector_set_x (SCM vec, size_t k, SCM obj);
SCM_API const SCM *scm_vector_elements (SCM vec,
					scm_t_array_handle *h,
					size_t *lenp, ssize_t *incp);
SCM_API SCM *scm_vector_writable_elements (SCM vec,
					   scm_t_array_handle *h,
					   size_t *lenp, ssize_t *incp);

/* Fast, non-checking accessors for simple vectors.
 */
#define SCM_SIMPLE_VECTOR_LENGTH(x)      SCM_I_VECTOR_LENGTH(x)
#define SCM_SIMPLE_VECTOR_REF(x,idx)     ((SCM_I_VECTOR_ELTS(x))[idx])
#define SCM_SIMPLE_VECTOR_SET(x,idx,val) ((SCM_I_VECTOR_WELTS(x))[idx]=(val))


/* Internals */

/* Vectors residualized into compiled objects have scm_tc7_vector in the
   low 7 bits, but also an additional bit set to indicate
   immutability.  */
#define SCM_F_VECTOR_IMMUTABLE 0x80UL
#define SCM_I_IS_MUTABLE_VECTOR(x)                              \
  (SCM_NIMP (x) &&                                              \
   ((SCM_CELL_TYPE (x) & (0x7f | SCM_F_VECTOR_IMMUTABLE))       \
    == scm_tc7_vector))
#define SCM_I_IS_VECTOR(x)     (SCM_HAS_TYP7 (x, scm_tc7_vector))
#define SCM_I_VECTOR_ELTS(x)   ((const SCM *) SCM_I_VECTOR_WELTS (x))
#define SCM_I_VECTOR_WELTS(x)  (SCM_CELL_OBJECT_LOC (x, 1))
#define SCM_I_VECTOR_LENGTH(x) (((size_t) SCM_CELL_WORD_0 (x)) >> 8)

SCM_INTERNAL SCM  scm_i_vector_equal_p (SCM x, SCM y);


SCM_INTERNAL void scm_init_vectors (void);

#endif  /* SCM_VECTORS_H */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
