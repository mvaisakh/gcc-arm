/* classes: h_files */

#ifndef SCM_FLUIDS_H
#define SCM_FLUIDS_H

/* Copyright (C) 1996,2000,2001, 2006, 2008, 2009, 2010, 2011, 2012, 2013 Free Software Foundation, Inc.
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
#include "libguile/vectors.h"


/* Fluids.

   Fluids are objects of a certain type that can hold one SCM value per
   dynamic state. That is, modifications to this value are only visible
   to code that executes with the same dynamic state as the modifying
   code. When a new dynamic state is constructed, it inherits the
   values from its parent. Because each thread executes with its own
   dynamic state, you can use fluids for thread local storage.
 */

#define SCM_FLUID_P(x)          (SCM_HAS_TYP7 (x, scm_tc7_fluid))

#ifdef BUILDING_LIBGUILE
# include <libguile/cache-internal.h>

struct scm_dynamic_state
{
  SCM thread_local_values;
  SCM values;
  uint8_t has_aliased_values;
  struct scm_cache cache;
};
#endif

SCM_API SCM scm_make_fluid (void);
SCM_API SCM scm_make_fluid_with_default (SCM dflt);
SCM_API SCM scm_make_unbound_fluid (void);
SCM_API SCM scm_make_thread_local_fluid (SCM dflt);
SCM_API int scm_is_fluid (SCM obj);
SCM_API SCM scm_fluid_p (SCM fl);
SCM_API SCM scm_fluid_thread_local_p (SCM fluid);
SCM_API SCM scm_fluid_ref (SCM fluid);
SCM_API SCM scm_fluid_ref_star (SCM fluid, SCM depth);
SCM_API SCM scm_fluid_set_x (SCM fluid, SCM value);
SCM_API SCM scm_fluid_unset_x (SCM fluid);
SCM_API SCM scm_fluid_bound_p (SCM fluid);

SCM_INTERNAL void scm_swap_fluid (SCM fluid, SCM value_box,
                                  scm_t_dynamic_state *dynamic_state);

SCM_API SCM scm_c_with_fluids (SCM fluids, SCM vals,
			       SCM (*cproc)(void *), void *cdata);
SCM_API SCM scm_c_with_fluid (SCM fluid, SCM val,
			      SCM (*cproc)(void *), void *cdata);
SCM_API SCM scm_with_fluids (SCM fluids, SCM vals, SCM thunk);
SCM_API SCM scm_with_fluid (SCM fluid, SCM val, SCM thunk);

SCM_API void scm_dynwind_fluid (SCM fluid, SCM value);

SCM_API SCM scm_dynamic_state_p (SCM obj);
SCM_API int scm_is_dynamic_state (SCM obj);
SCM_API SCM scm_current_dynamic_state (void);
SCM_API SCM scm_set_current_dynamic_state (SCM state);
SCM_API void scm_dynwind_current_dynamic_state (SCM state);
SCM_API void *scm_c_with_dynamic_state (SCM state, 
					void *(*func)(void *), void *data);
SCM_API SCM scm_with_dynamic_state (SCM state, SCM proc);
SCM_INTERNAL SCM scm_dynamic_state_ref (SCM state, SCM fluid, SCM dflt);

SCM_INTERNAL SCM scm_i_make_initial_dynamic_state (void);

SCM_INTERNAL void scm_i_fluid_print (SCM exp, SCM port, scm_print_state *pstate);
SCM_INTERNAL void scm_i_dynamic_state_print (SCM exp, SCM port, scm_print_state *pstate);
SCM_INTERNAL void scm_init_fluids (void);

#endif  /* SCM_FLUIDS_H */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
