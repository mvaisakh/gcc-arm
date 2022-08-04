/* classes: h_files */

#ifndef SCM_WEAK_SET_H
#define SCM_WEAK_SET_H

/* Copyright (C) 2011 Free Software Foundation, Inc.
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



/* The weak set API is currently only used internally.  We could make it
   public later, after some API review.  */

/* Function that returns nonzero if the given object is the one we are
   looking for.  */
typedef int (*scm_t_set_predicate_fn) (SCM obj, void *closure);

/* Function to fold over the elements of a set.  */
typedef SCM (*scm_t_set_fold_fn) (void *closure, SCM key, SCM result);

SCM_INTERNAL SCM scm_c_make_weak_set (unsigned long k);
SCM_INTERNAL SCM scm_weak_set_p (SCM h);
SCM_INTERNAL SCM scm_c_weak_set_lookup (SCM set, unsigned long raw_hash,
                                        scm_t_set_predicate_fn pred,
                                        void *closure, SCM dflt);
SCM_INTERNAL SCM scm_c_weak_set_add_x (SCM set, unsigned long raw_hash,
                                       scm_t_set_predicate_fn pred,
                                       void *closure, SCM obj);
SCM_INTERNAL void scm_c_weak_set_remove_x (SCM set, unsigned long raw_hash,
                                           scm_t_set_predicate_fn pred,
                                           void *closure);
SCM_INTERNAL SCM scm_weak_set_add_x (SCM set, SCM obj);
SCM_INTERNAL SCM scm_weak_set_remove_x (SCM set, SCM obj);
SCM_INTERNAL SCM scm_weak_set_clear_x (SCM set);
SCM_INTERNAL SCM scm_c_weak_set_fold (scm_t_set_fold_fn proc, void *closure,
                                      SCM init, SCM set);
SCM_INTERNAL SCM scm_weak_set_fold (SCM proc, SCM init, SCM set);
SCM_INTERNAL SCM scm_weak_set_for_each (SCM proc, SCM set);
SCM_INTERNAL SCM scm_weak_set_map_to_list (SCM proc, SCM set);

SCM_INTERNAL void scm_i_weak_set_print (SCM exp, SCM port, scm_print_state *pstate);
SCM_INTERNAL void scm_init_weak_set (void);

#endif  /* SCM_WEAK_SET_H */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
