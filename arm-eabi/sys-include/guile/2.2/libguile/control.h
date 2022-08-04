/* Copyright (C) 2010, 2011, 2012, 2013  Free Software Foundation, Inc.
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

#ifndef SCM_CONTROL_H
#define SCM_CONTROL_H

#include "libguile/vm.h"


SCM_INTERNAL SCM scm_i_prompt_pop_abort_args_x (struct scm_vm *vp,
                                                scm_t_ptrdiff saved_stack_depth);

SCM_INTERNAL void scm_c_abort (struct scm_vm *vp, SCM tag, size_t n, SCM *argv,
                               scm_i_jmp_buf *registers) SCM_NORETURN;
SCM_INTERNAL SCM scm_abort_to_prompt_star (SCM tag, SCM args) SCM_NORETURN;


SCM_INTERNAL void scm_init_control (void);


#endif /* SCM_CONTROL_H */
