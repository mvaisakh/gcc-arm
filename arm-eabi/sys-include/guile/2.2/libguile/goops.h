/* classes: h_files */

#ifndef SCM_GOOPS_H
#define SCM_GOOPS_H

/* Copyright (C) 1998,1999,2000,2001,2002,2003, 2006, 2008, 2009, 2011, 2015 Free Software Foundation, Inc.
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



/* This software is a derivative work of other copyrighted softwares; the
 * copyright notices of these softwares are placed in the file COPYRIGHTS
 *
 * This file is based upon stklos.h from the STk distribution by
 * Erick Gallesio <eg@unice.fr>.
 */

#include "libguile/__scm.h"

#include "libguile/validate.h"

/* {Class flags}
 *
 * These are used for efficient identification of instances of a
 * certain class or its subclasses when traversal of the inheritance
 * graph would be too costly.
 */
#define SCM_VTABLE_FLAG_GOOPS_CLASS SCM_VTABLE_FLAG_GOOPS_0
#define SCM_VTABLE_FLAG_GOOPS_VALID SCM_VTABLE_FLAG_GOOPS_1
#define SCM_VTABLE_FLAG_GOOPS_SLOT SCM_VTABLE_FLAG_GOOPS_2
#define SCM_VTABLE_FLAG_GOOPS_STATIC SCM_VTABLE_FLAG_GOOPS_3

#define SCM_CLASS_OF(x)         SCM_STRUCT_VTABLE (x)
#define SCM_CLASS_FLAGS(class) (SCM_VTABLE_FLAGS (class))
#define SCM_OBJ_CLASS_FLAGS(obj) (SCM_STRUCT_VTABLE_FLAGS (obj))
#define SCM_SET_CLASS_FLAGS(c, f) (SCM_SET_VTABLE_FLAGS (c, f))
#define SCM_CLEAR_CLASS_FLAGS(c, f) (SCM_CLEAR_VTABLE_FLAGS (c, f))

#define SCM_CLASSF_METACLASS     (SCM_VTABLE_FLAG_GOOPS_CLASS|SCM_VTABLE_FLAG_VTABLE)
#define SCM_CLASSF_GOOPS_VALID   SCM_VTABLE_FLAG_GOOPS_VALID
#define SCM_CLASSF_GOOPS         SCM_VTABLE_FLAG_GOOPS_CLASS
#define SCM_CLASSF_GOOPS_OR_VALID (SCM_CLASSF_GOOPS | SCM_CLASSF_GOOPS_VALID)

#define SCM_CLASS_OF(x)        SCM_STRUCT_VTABLE (x)

#define SCM_CLASSP(x) \
  (SCM_STRUCTP (x) && SCM_STRUCT_VTABLE_FLAGS (x) & SCM_CLASSF_METACLASS)
#define SCM_VALIDATE_CLASS(pos, x) SCM_MAKE_VALIDATE_MSG (pos, x, CLASSP, "class")

#define SCM_INSTANCEP(x) \
  (SCM_STRUCTP (x) && (SCM_STRUCT_VTABLE_FLAGS (x) & SCM_CLASSF_GOOPS))
#define SCM_VALIDATE_INSTANCE(pos, x) SCM_MAKE_VALIDATE_MSG (pos, x, INSTANCEP, "instance")

#define SCM_SLOT(x, i)         (SCM_STRUCT_SLOT_REF (x, i))
#define SCM_SET_SLOT(x, i, v)  (SCM_STRUCT_SLOT_SET (x, i, v))

#define SCM_SUBCLASSP(c1, c2) \
  (scm_is_true (scm_c_memq (c2, scm_class_precedence_list (c1))))
#define SCM_IS_A_P(x, c) \
  (SCM_INSTANCEP (x) && SCM_SUBCLASSP (SCM_CLASS_OF (x), c))

#define SCM_GENERICP(x) (scm_is_generic (x))
#define SCM_VALIDATE_GENERIC(pos, x) SCM_MAKE_VALIDATE_MSG (pos, x, GENERICP, "generic function")

#define SCM_METHODP(x) (scm_is_method (x))
#define SCM_VALIDATE_METHOD(pos, x) SCM_MAKE_VALIDATE_MSG (pos, x, METHODP, "method")

#define SCM_SET_CLASS_DESTRUCTOR(c, d) SCM_SET_VTABLE_DESTRUCTOR (c, d)

SCM_INTERNAL SCM scm_i_smob_class[];

SCM_API SCM scm_module_goops;

SCM_API SCM scm_goops_version (void);
SCM_API void scm_load_goops (void);
SCM_API SCM scm_make_extended_class (char const *type_name, int applicablep);
SCM_INTERNAL void scm_make_port_classes (scm_t_port_type *ptob);
SCM_API SCM scm_ensure_accessor (SCM name);
SCM_API SCM scm_class_of (SCM obj);

/* Low level functions exported */
SCM_INTERNAL SCM scm_make_standard_class (SCM meta, SCM name, SCM dsupers,
                                          SCM dslots);

/* Primitives exported */
SCM_API SCM scm_slot_ref (SCM obj, SCM slot_name);
SCM_API SCM scm_slot_set_x (SCM obj, SCM slot_name, SCM value);

SCM_INTERNAL void scm_i_inherit_applicable (SCM c);
SCM_API SCM scm_instance_p (SCM obj);
SCM_API int scm_is_generic (SCM x);
SCM_API int scm_is_method (SCM x);
SCM_API SCM scm_class_name (SCM obj);
SCM_API SCM scm_class_direct_supers (SCM obj);
SCM_API SCM scm_class_direct_slots (SCM obj);
SCM_API SCM scm_class_direct_subclasses (SCM obj);
SCM_API SCM scm_class_direct_methods (SCM obj);
SCM_API SCM scm_class_precedence_list (SCM obj);
SCM_API SCM scm_class_slots (SCM obj);
SCM_API SCM scm_generic_function_name (SCM obj);
SCM_API SCM scm_generic_function_methods (SCM obj);
SCM_API SCM scm_method_generic_function (SCM obj);
SCM_API SCM scm_method_specializers (SCM obj);
SCM_API SCM scm_method_procedure (SCM obj);
SCM_API SCM scm_slot_bound_p (SCM obj, SCM slot_name);
SCM_API SCM scm_slot_exists_p (SCM obj, SCM slot_name);
SCM_API SCM scm_sys_modify_instance (SCM old, SCM newinst);
SCM_API SCM scm_sys_modify_class (SCM old, SCM newcls);
SCM_API SCM scm_generic_capability_p (SCM proc);
SCM_API SCM scm_enable_primitive_generic_x (SCM subrs);
SCM_INTERNAL SCM scm_set_primitive_generic_x (SCM subr, SCM generic);
SCM_API SCM scm_primitive_generic_generic (SCM subr);
SCM_API SCM scm_make (SCM args);
SCM_API void scm_change_object_class (SCM, SCM, SCM);

/* These procedures are for dispatching to a generic when a primitive
   fails to apply.  They raise a wrong-type-arg error if the primitive's
   generic has not been initialized yet.  */
SCM_API SCM scm_wta_dispatch_0 (SCM gf, const char *subr);
SCM_API SCM scm_wta_dispatch_1 (SCM gf, SCM a1, int pos, const char *subr);
SCM_API SCM scm_wta_dispatch_2 (SCM gf, SCM a1, SCM a2, int pos, const char *subr);
SCM_API SCM scm_wta_dispatch_n (SCM gf, SCM args, int pos, const char *subr);

SCM_INTERNAL SCM scm_i_define_class_for_vtable (SCM vtable);


SCM_INTERNAL void scm_init_goops (void);

#endif  /* SCM_GOOPS_H */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
