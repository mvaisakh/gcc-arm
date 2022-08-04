/* Copyright (C) 2013 Free Software Foundation, Inc.
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

#ifndef _SCM_VM_BUILTINS_H_
#define _SCM_VM_BUILTINS_H_

#ifdef BUILDING_LIBGUILE

#define FOR_EACH_VM_BUILTIN(M) \
  M(apply, APPLY, 2, 0, 1) \
  M(values, VALUES, 0, 0, 1) \
  M(abort_to_prompt, ABORT_TO_PROMPT, 1, 0, 1) \
  M(call_with_values, CALL_WITH_VALUES, 2, 0, 0) \
  M(call_with_current_continuation, CALL_WITH_CURRENT_CONTINUATION, 1, 0, 0)

/* These enumerated values are embedded in VM code, and as such are
   part of Guile's ABI.  */
enum scm_vm_builtins
{
#define ENUM(builtin, BUILTIN, req, opt, rest) SCM_VM_BUILTIN_##BUILTIN,
  FOR_EACH_VM_BUILTIN(ENUM)
#undef ENUM
  SCM_VM_BUILTIN_COUNT
};

SCM_INTERNAL SCM scm_vm_builtin_name_to_index (SCM name);
SCM_INTERNAL SCM scm_vm_builtin_index_to_name (SCM idx);
SCM_INTERNAL void scm_init_vm_builtin_properties (void);

#endif /* BUILDING_LIBGUILE */

#endif /* _SCM_VM_BUILTINS_H_ */
