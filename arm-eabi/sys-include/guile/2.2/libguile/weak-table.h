/* classes: h_files */

#ifndef SCM_WEAK_TABLE_H
#define SCM_WEAK_TABLE_H

/* Copyright (C) 2011, 2012 Free Software Foundation, Inc.
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



/* The weak table API is currently only used internally.  We could make it
   public later, after some API review.  */

typedef enum {
  SCM_WEAK_TABLE_KIND_KEY,
  SCM_WEAK_TABLE_KIND_VALUE,
  SCM_WEAK_TABLE_KIND_BOTH,
} scm_t_weak_table_kind;

/* Function that returns nonzero if the given mapping is the one we are
   looking for.  */
typedef int (*scm_t_table_predicate_fn) (SCM k, SCM v, void *closure);

/* Function to fold over the elements of a set.  */
typedef SCM (*scm_t_table_fold_fn) (void *closure, SCM k, SCM v, SCM result);

SCM_INTERNAL SCM scm_c_make_weak_table (unsigned long k,
                                        scm_t_weak_table_kind kind);
SCM_INTERNAL SCM scm_weak_table_p (SCM h);

SCM_INTERNAL SCM scm_c_weak_table_ref (SCM table, unsigned long raw_hash,
                                       scm_t_table_predicate_fn pred,
                                       void *closure, SCM dflt);
SCM_INTERNAL void scm_c_weak_table_put_x (SCM table, unsigned long raw_hash,
                                          scm_t_table_predicate_fn pred,
                                          void *closure, SCM key, SCM value);
SCM_INTERNAL void scm_c_weak_table_remove_x (SCM table, unsigned long raw_hash,
                                             scm_t_table_predicate_fn pred,
                                             void *closure);

SCM_INTERNAL SCM scm_weak_table_refq (SCM table, SCM key, SCM dflt);
SCM_INTERNAL void scm_weak_table_putq_x (SCM table, SCM key, SCM value);
SCM_INTERNAL void scm_weak_table_remq_x (SCM table, SCM key);

SCM_INTERNAL void scm_weak_table_clear_x (SCM table);

SCM_INTERNAL SCM scm_c_weak_table_fold (scm_t_table_fold_fn proc, void *closure,
                                      SCM init, SCM table);
SCM_INTERNAL SCM scm_weak_table_fold (SCM proc, SCM init, SCM table);
SCM_INTERNAL void scm_weak_table_for_each (SCM proc, SCM table);
SCM_INTERNAL SCM scm_weak_table_map_to_list (SCM proc, SCM table);



/* Legacy interface.  */
SCM_API SCM scm_make_weak_key_hash_table (SCM k);
SCM_API SCM scm_make_weak_value_hash_table (SCM k);
SCM_API SCM scm_make_doubly_weak_hash_table (SCM k);
SCM_API SCM scm_weak_key_hash_table_p (SCM h);
SCM_API SCM scm_weak_value_hash_table_p (SCM h);
SCM_API SCM scm_doubly_weak_hash_table_p (SCM h);



SCM_INTERNAL void scm_i_weak_table_print (SCM exp, SCM port, scm_print_state *pstate);
SCM_INTERNAL void scm_weak_table_prehistory (void);
SCM_INTERNAL void scm_init_weak_table (void);

#endif  /* SCM_WEAK_TABLE_H */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
