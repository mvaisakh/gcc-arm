/* Copyright (C) 2001, 2009, 2012, 2013 Free Software Foundation, Inc.
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

#ifndef _SCM_INSTRUCTIONS_H_
#define _SCM_INSTRUCTIONS_H_

#include <libguile.h>
#include <libguile/vm-operations.h>

#ifdef BUILDING_LIBGUILE

enum scm_opcode
  {
#define ENUM(opcode, tag, name, meta) scm_op_##tag = opcode,
    FOR_EACH_VM_OPERATION(ENUM)
#undef ENUM
  };

#define SCM_PACK_OP_24(op,arg)      (scm_op_##op | (arg) << 8)
#define SCM_PACK_OP_8_8_8(op,a,b,c) SCM_PACK_OP_24 (op, (a) | ((b) << 8) | ((c) << 16))
#define SCM_PACK_OP_8_16(op,a,b)    SCM_PACK_OP_24 (op, (a) | (b) << 8)
#define SCM_PACK_OP_16_8(op,a,b)    SCM_PACK_OP_24 (op, (a) | (b) << 16)
#define SCM_PACK_OP_12_12(op,a,b)   SCM_PACK_OP_24 (op, (a) | (b) << 12)
#define SCM_PACK_OP_ARG_8_24(a,b)   ((a) | ((b) << 8))

#define SCM_VM_NUM_INSTRUCTIONS (1<<8)
#define SCM_VM_INSTRUCTION_MASK (SCM_VM_NUM_INSTRUCTIONS-1)

#endif /* BUILDING_LIBGUILE  */

SCM_INTERNAL SCM scm_instruction_list (void);

SCM_INTERNAL void scm_bootstrap_instructions (void);
SCM_INTERNAL void scm_init_instructions (void);

#endif /* _SCM_INSTRUCTIONS_H_ */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
