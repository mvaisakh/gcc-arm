/* This file contains definitions for deprecated features.  When you
   deprecate something, move it here when that is feasible.
*/

#ifndef SCM_DEPRECATED_H
#define SCM_DEPRECATED_H

/* Copyright (C) 2003,2004, 2005, 2006, 2007, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2017 Free Software Foundation, Inc.
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
#include "libguile/strings.h"
#include "libguile/eval.h"
#include "libguile/throw.h"
#include "libguile/iselect.h"

#if (SCM_ENABLE_DEPRECATED == 1)

/* Deprecated 13-05-2011 because it's better just to scm_dynwind_begin.
   That also avoids the temptation to stuff pointers in an SCM.  */

typedef SCM (*scm_t_inner) (void *);
SCM_DEPRECATED SCM scm_internal_dynamic_wind (scm_t_guard before,
                                              scm_t_inner inner,
                                              scm_t_guard after,
                                              void *inner_data,
                                              void *guard_data);


/* Deprecated 15-05-2011 because it's better to be explicit with the
   `return'.  Code is more readable that way.  */
#define SCM_WTA_DISPATCH_0(gf, subr)			        \
  return scm_wta_dispatch_0 ((gf), (subr))
#define SCM_WTA_DISPATCH_1(gf, a1, pos, subr)			\
  return scm_wta_dispatch_1 ((gf), (a1), (pos), (subr))
#define SCM_WTA_DISPATCH_2(gf, a1, a2, pos, subr)          \
  return scm_wta_dispatch_2 ((gf), (a1), (a2), (pos), (subr))
#define SCM_WTA_DISPATCH_N(gf, args, pos, subr)			\
  return scm_wta_dispatch_n ((gf), (args), (pos), (subr))

/* Deprecated 15-05-2011 because this idiom is not very readable.  */
#define SCM_GASSERT0(cond, gf, subr)            \
  if (SCM_UNLIKELY (!(cond)))                   \
    return scm_wta_dispatch_0 ((gf), (subr))
#define SCM_GASSERT1(cond, gf, a1, pos, subr)           \
  if (SCM_UNLIKELY (!(cond)))                           \
    return scm_wta_dispatch_1 ((gf), (a1), (pos), (subr))
#define SCM_GASSERT2(cond, gf, a1, a2, pos, subr)	\
  if (SCM_UNLIKELY (!(cond)))                           \
    return scm_wta_dispatch_2 ((gf), (a1), (a2), (pos), (subr))
#define SCM_GASSERTn(cond, gf, args, pos, subr)         \
  if (SCM_UNLIKELY (!(cond)))                           \
    return scm_wta_dispatch_n ((gf), (args), (pos), (subr))

/* Deprecated 15-05-2011 because this is a one-off macro that does
   strange things.  */
#define SCM_WTA_DISPATCH_1_SUBR(subr, a1, pos)				\
  return (SCM_UNPACK ((*SCM_SUBR_GENERIC (subr)))			\
	  ? scm_call_1 ((*SCM_SUBR_GENERIC (subr)), (a1))               \
	  : (scm_i_wrong_type_arg_symbol (SCM_SUBR_NAME (subr), (pos), (a1)), SCM_UNSPECIFIED))

#define SCM_LIST0 SCM_EOL
#define SCM_LIST1(e0) scm_cons ((e0), SCM_EOL)
#define SCM_LIST2(e0, e1) scm_cons2 ((e0), (e1), SCM_EOL)
#define SCM_LIST3(e0, e1, e2) scm_cons ((e0), SCM_LIST2 ((e1), (e2)))
#define SCM_LIST4(e0, e1, e2, e3)\
     scm_cons2 ((e0), (e1), SCM_LIST2 ((e2), (e3)))
#define SCM_LIST5(e0, e1, e2, e3, e4)\
     scm_cons ((e0), SCM_LIST4 ((e1), (e2), (e3), (e4)))
#define SCM_LIST6(e0, e1, e2, e3, e4, e5)\
     scm_cons2 ((e0), (e1), SCM_LIST4 ((e2), (e3), (e4), (e5)))
#define SCM_LIST7(e0, e1, e2, e3, e4, e5, e6)\
     scm_cons ((e0), SCM_LIST6 ((e1), (e2), (e3), (e4), (e5), (e6)))
#define SCM_LIST8(e0, e1, e2, e3, e4, e5, e6, e7)\
     scm_cons2 ((e0), (e1), SCM_LIST6 ((e2), (e3), (e4), (e5), (e6), (e7)))
#define SCM_LIST9(e0, e1, e2, e3, e4, e5, e6, e7, e8)\
     scm_cons ((e0),\
	       SCM_LIST8 ((e1), (e2), (e3), (e4), (e5), (e6), (e7), (e8)))

#define SCM_CHAR_CODE_LIMIT SCM_CHAR_CODE_LIMIT__GONE__REPLACE_WITH__256L
#define SCM_OPDIRP SCM_OPDIRP__GONE__REPLACE_WITH__SCM_DIRP_and_SCM_DIR_OPEN_P
#define SCM_PROCEDURE SCM_PROCEDURE__GONE__REPLACE_WITH__scm_procedure
#define SCM_PROCEDURE_WITH_SETTER_P SCM_PROCEDURE_WITH_SETTER_P__GONE__REPLACE_WITH__scm_is_true__scm_procedure_with_setter_p
#define SCM_SETTER SCM_SETTER__GONE__REPLACE_WITH__scm_setter
#define SCM_THREAD_SWITCHING_CODE SCM_THREAD_SWITCHING_CODE__GONE__REMOVE_FROM_YOUR_CODE
#define SCM_VALIDATE_NUMBER_COPY SCM_VALIDATE_NUMBER_COPY__GONE__REPLACE_WITH__SCM_VALIDATE_DOUBLE_COPY
#define SCM_VALIDATE_NUMBER_DEF_COPY SCM_VALIDATE_NUMBER_DEF_COPY__GONE__REPLACE_WITH__SCM_UNBNDP_and_SCM_VALIDATE_DOUBLE_COPY
#define SCM_VALIDATE_OPDIR SCM_VALIDATE_OPDIR__GONE
#define SCM_VALIDATE_STRING_COPY SCM_VALIDATE_STRING_COPY__GONE
#define SCM_VALIDATE_SUBSTRING_SPEC_COPY SCM_VALIDATE_SUBSTRING_SPEC_COPY__GONE
#define scm_array scm_array__GONE__REPLACE_WITH__scm_t_array
#define scm_array_dim scm_array_dim__GONE__REPLACE_WITH__scm_t_array_dim
#define scm_async_click scm_async_click__GONE__REPLACE_WITH__scm_async_tick
#define scm_call_generic_0 scm_call_generic_0__GONE__REPLACE_WITH__scm_call_0
#define scm_call_generic_1 scm_call_generic_1__GONE__REPLACE_WITH__scm_call_1
#define scm_call_generic_2 scm_call_generic_2__GONE__REPLACE_WITH__scm_call_2
#define scm_call_generic_3 scm_call_generic_3__GONE__REPLACE_WITH__scm_call_3
#define scm_apply_generic scm_apply_generic__GONE__REPLACE_WITH__scm_apply_0
#define scm_fport scm_fport__GONE__REPLACE_WITH__scm_t_fport
#define scm_listify scm_listify__GONE__REPLACE_WITH__scm_list_n
#define scm_option scm_option__GONE__REPLACE_WITH__scm_t_option
#define scm_port scm_port__GONE__REPLACE_WITH__scm_t_port
#define scm_ptob_descriptor scm_ptob_descriptor__GONE__REPLACE_WITH__scm_t_port_type
#define scm_rng scm_rng__GONE__REPLACE_WITH__scm_t_rng
#define scm_rstate scm_rstate__GONE__REPLACE_WITH__scm_t_rstate
#define scm_sizet scm_sizet__GONE__REPLACE_WITH__size_t
#define scm_srcprops scm_srcprops__GONE__REPLACE_WITH__scm_t_srcprops
#define scm_srcprops_chunk scm_srcprops_chunk__GONE__REPLACE_WITH__scm_t_srcprops_chunk
#define scm_struct_i_flags scm_struct_i_flags__GONE__REPLACE_WITH__scm_vtable_index_flags
#define scm_struct_i_free scm_struct_i_free__GONE__REPLACE_WITH__scm_vtable_index_instance_finalize
#define scm_subr_entry scm_subr_entry__GONE__REPLACE_WITH__scm_t_subr_entry
#define scm_substring_move_left_x scm_substring_move_left_x__GONE__REPLACE_WITH__scm_substring_move_x
#define scm_substring_move_right_x scm_substring_move_right_x__GONE__REPLACE_WITH__scm_substring_move_x
#define scm_vtable_index_printer scm_vtable_index_printer__GONE__REPLACE_WITH__scm_vtable_index_instance_printer
#define scm_vtable_index_vtable scm_vtable_index_vtable__GONE__REPLACE_WITH__scm_vtable_index_self

#ifndef BUILDING_LIBGUILE
#define SCM_ASYNC_TICK  SCM_ASYNC_TICK__GONE__REPLACE_WITH__scm_async_tick
#endif




/* Deprecated 26-05-2011, as the GC_STUBBORN API doesn't do anything any
   more.  */
SCM_DEPRECATED SCM scm_immutable_cell (scm_t_bits car, scm_t_bits cdr);
SCM_DEPRECATED SCM scm_immutable_double_cell (scm_t_bits car, scm_t_bits cbr,
                                              scm_t_bits ccr, scm_t_bits cdr);



SCM_DEPRECATED SCM scm_memory_alloc_key;
SCM_DEPRECATED void scm_memory_error (const char *subr) SCM_NORETURN;



SCM_DEPRECATED SCM scm_no_applicable_method;

SCM_DEPRECATED SCM scm_class_boolean;
SCM_DEPRECATED SCM scm_class_char;
SCM_DEPRECATED SCM scm_class_pair;
SCM_DEPRECATED SCM scm_class_procedure;
SCM_DEPRECATED SCM scm_class_string;
SCM_DEPRECATED SCM scm_class_symbol;
SCM_DEPRECATED SCM scm_class_primitive_generic;
SCM_DEPRECATED SCM scm_class_vector;
SCM_DEPRECATED SCM scm_class_null;
SCM_DEPRECATED SCM scm_class_real;
SCM_DEPRECATED SCM scm_class_complex;
SCM_DEPRECATED SCM scm_class_integer;
SCM_DEPRECATED SCM scm_class_fraction;
SCM_DEPRECATED SCM scm_class_unknown;
SCM_DEPRECATED SCM scm_class_top;
SCM_DEPRECATED SCM scm_class_object;
SCM_DEPRECATED SCM scm_class_class;
SCM_DEPRECATED SCM scm_class_applicable;
SCM_DEPRECATED SCM scm_class_applicable_struct;
SCM_DEPRECATED SCM scm_class_applicable_struct_with_setter;
SCM_DEPRECATED SCM scm_class_generic;
SCM_DEPRECATED SCM scm_class_generic_with_setter;
SCM_DEPRECATED SCM scm_class_accessor;
SCM_DEPRECATED SCM scm_class_extended_generic;
SCM_DEPRECATED SCM scm_class_extended_generic_with_setter;
SCM_DEPRECATED SCM scm_class_extended_accessor;
SCM_DEPRECATED SCM scm_class_method;
SCM_DEPRECATED SCM scm_class_accessor_method;
SCM_DEPRECATED SCM scm_class_procedure_class;
SCM_DEPRECATED SCM scm_class_applicable_struct_class;
SCM_DEPRECATED SCM scm_class_number;
SCM_DEPRECATED SCM scm_class_list;
SCM_DEPRECATED SCM scm_class_keyword;
SCM_DEPRECATED SCM scm_class_port;
SCM_DEPRECATED SCM scm_class_input_output_port;
SCM_DEPRECATED SCM scm_class_input_port;
SCM_DEPRECATED SCM scm_class_output_port;
SCM_DEPRECATED SCM scm_class_foreign_slot;
SCM_DEPRECATED SCM scm_class_self;
SCM_DEPRECATED SCM scm_class_protected;
SCM_DEPRECATED SCM scm_class_hidden;
SCM_DEPRECATED SCM scm_class_opaque;
SCM_DEPRECATED SCM scm_class_read_only;
SCM_DEPRECATED SCM scm_class_protected_hidden;
SCM_DEPRECATED SCM scm_class_protected_opaque;
SCM_DEPRECATED SCM scm_class_protected_read_only;
SCM_DEPRECATED SCM scm_class_scm;
SCM_DEPRECATED SCM scm_class_int;
SCM_DEPRECATED SCM scm_class_float;
SCM_DEPRECATED SCM scm_class_double;

SCM_DEPRECATED SCM *scm_smob_class;

SCM_INTERNAL void scm_init_deprecated_goops (void);

SCM_DEPRECATED SCM scm_compute_applicable_methods (SCM gf, SCM args, long len, int scm_find_method);
SCM_DEPRECATED SCM scm_find_method (SCM l);
SCM_DEPRECATED SCM scm_basic_make_class (SCM c, SCM name, SCM dsupers, SCM dslots);
SCM_DEPRECATED SCM scm_get_keyword (SCM kw, SCM initargs, SCM default_value);
SCM_DEPRECATED SCM scm_slot_ref_using_class (SCM cls, SCM obj, SCM slot_name);
SCM_DEPRECATED SCM scm_slot_set_using_class_x (SCM cls, SCM obj, SCM slot_name, SCM value);
SCM_DEPRECATED SCM scm_slot_bound_using_class_p (SCM cls, SCM obj, SCM slot_name);
SCM_DEPRECATED SCM scm_slot_exists_using_class_p (SCM cls, SCM obj, SCM slot_name);



SCM_DEPRECATED SCM scm_make_arbiter (SCM name);
SCM_DEPRECATED SCM scm_try_arbiter (SCM arb);
SCM_DEPRECATED SCM scm_release_arbiter (SCM arb);



SCM_DEPRECATED SCM scm_async (SCM thunk);
SCM_DEPRECATED SCM scm_async_mark (SCM a);
SCM_DEPRECATED SCM scm_run_asyncs (SCM list_of_a);



SCM_DEPRECATED void scm_critical_section_start (void);
SCM_DEPRECATED void scm_critical_section_end (void);
SCM_DEPRECATED void scm_dynwind_critical_section (SCM mutex);

#define SCM_CRITICAL_SECTION_START  scm_critical_section_start ()
#define SCM_CRITICAL_SECTION_END    scm_critical_section_end ()



SCM_DEPRECATED SCM scm_make_mutex_with_flags (SCM flags);
SCM_DEPRECATED SCM scm_unlock_mutex_timed (SCM mx, SCM cond, SCM timeout);
SCM_DEPRECATED SCM scm_lock_mutex_timed (SCM m, SCM timeout, SCM owner);



SCM_DEPRECATED SCM scm_internal_cwdr (scm_t_catch_body body,
                                      void *body_data,
                                      scm_t_catch_handler handler,
                                      void *handler_data,
                                      SCM_STACKITEM *stack_start);
SCM_DEPRECATED SCM scm_call_with_dynamic_root (SCM thunk, SCM handler);
SCM_DEPRECATED SCM scm_dynamic_root (void);
SCM_DEPRECATED SCM scm_apply_with_dynamic_root (SCM proc, SCM a1,
                                                SCM args, SCM handler);



SCM_DEPRECATED SCM scm_make_dynamic_state (SCM parent);



/* Deprecated 2016-11-18. Never documented. Unnecessary, since
   array-copy! already unrolls and does it in more general cases. */
/* With this also remove SCM_I_ARRAY_FLAG_CONTIGUOUS,
   SCM_SET_ARRAY_CONTIGUOUS_FLAG, SCM_CLR_ARRAY_CONTIGUOUS_FLAG,
   scm_i_ra_set_contp, and uses thereof. */
SCM_DEPRECATED SCM scm_from_contiguous_array (SCM bounds, const SCM *elts,
                                              size_t len);



SCM_DEPRECATED int SCM_FDES_RANDOM_P (int fdes);



SCM_DEPRECATED SCM scm_make_struct (SCM vtable, SCM tail_array_size, SCM init);



void scm_i_init_deprecated (void);

#endif

#endif /* SCM_DEPRECATED_H */
