#ifndef SCM_ATOMIC_H
#define SCM_ATOMIC_H

/* Copyright (C) 2016 Free Software Foundation, Inc.
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
#include "libguile/gc.h"
#include "libguile/tags.h"



static inline int
scm_is_atomic_box (SCM obj)
{
  return SCM_HAS_TYP7 (obj, scm_tc7_atomic_box);
}

static inline SCM*
scm_atomic_box_loc (SCM obj)
{
  return SCM_CELL_OBJECT_LOC (obj, 1);
}



#ifdef BUILDING_LIBGUILE
SCM_INTERNAL SCM scm_make_atomic_box (SCM init);
SCM_INTERNAL SCM scm_atomic_box_p (SCM obj);
SCM_INTERNAL SCM scm_atomic_box_ref (SCM box);
SCM_INTERNAL SCM scm_atomic_box_set_x (SCM box, SCM val);
SCM_INTERNAL SCM scm_atomic_box_swap_x (SCM box, SCM val);
SCM_INTERNAL SCM scm_atomic_box_compare_and_swap_x (SCM box, SCM expected, SCM desired);
SCM_INTERNAL void scm_i_atomic_box_print (SCM box, SCM port, scm_print_state *pstate);

SCM_INTERNAL void scm_register_atomic (void);
#endif  /* BUILDING_LIBGUILE */

#endif  /* SCM_ATOMIC_H */
