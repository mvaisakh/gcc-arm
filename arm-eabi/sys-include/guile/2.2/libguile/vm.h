/* Copyright (C) 2001, 2009, 2010, 2011, 2012, 2013, 2014, 2015 Free Software Foundation, Inc.
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

#ifndef _SCM_VM_H_
#define _SCM_VM_H_

#include <libguile.h>
#include <libguile/programs.h>

enum {
  SCM_VM_APPLY_HOOK,
  SCM_VM_PUSH_CONTINUATION_HOOK,
  SCM_VM_POP_CONTINUATION_HOOK,
  SCM_VM_NEXT_HOOK,
  SCM_VM_ABORT_CONTINUATION_HOOK,
  SCM_VM_NUM_HOOKS,
};

#define SCM_VM_REGULAR_ENGINE 0
#define SCM_VM_DEBUG_ENGINE 1
#define SCM_VM_NUM_ENGINES 2

struct scm_vm {
  scm_t_uint32 *ip;		/* instruction pointer */
  union scm_vm_stack_element *sp; /* stack pointer */
  union scm_vm_stack_element *fp; /* frame pointer */
  union scm_vm_stack_element *stack_limit; /* stack limit address */
  int trace_level;              /* traces enabled if trace_level > 0 */
  union scm_vm_stack_element *sp_min_since_gc; /* deepest sp since last gc */
  size_t stack_size;		/* stack size */
  union scm_vm_stack_element *stack_bottom; /* lowest address in allocated stack */
  union scm_vm_stack_element *stack_top; /* highest address in allocated stack */
  SCM overflow_handler_stack;   /* alist of max-stack-size -> thunk */
  SCM hooks[SCM_VM_NUM_HOOKS];	/* hooks */
  const void *resumable_prompt_cookie; /* opaque cookie */
  int engine;                   /* which vm engine we're using */
};

SCM_INTERNAL struct scm_vm *scm_the_vm (void);
SCM_API SCM scm_call_with_vm (SCM proc, SCM args);

SCM_API SCM scm_call_with_stack_overflow_handler (SCM limit, SCM thunk,
                                                  SCM handler);

SCM_API SCM scm_vm_apply_hook (void);
SCM_API SCM scm_vm_push_continuation_hook (void);
SCM_API SCM scm_vm_pop_continuation_hook (void);
SCM_API SCM scm_vm_abort_continuation_hook (void);
SCM_API SCM scm_vm_next_hook (void);
SCM_API SCM scm_vm_trace_level (void);
SCM_API SCM scm_set_vm_trace_level_x (SCM level);
SCM_API SCM scm_vm_engine (void);
SCM_API SCM scm_set_vm_engine_x (SCM engine);
SCM_API SCM scm_set_default_vm_engine_x (SCM engine);
SCM_API void scm_c_set_vm_engine_x (int engine);
SCM_API void scm_c_set_default_vm_engine_x (int engine);

struct GC_ms_entry;
SCM_INTERNAL struct GC_ms_entry * scm_i_vm_mark_stack (struct scm_vm *,
                                                       struct GC_ms_entry *,
                                                       struct GC_ms_entry *);
SCM_INTERNAL void scm_i_vm_free_stack (struct scm_vm *vp);

#define SCM_F_VM_CONT_PARTIAL 0x1
#define SCM_F_VM_CONT_REWINDABLE 0x2

struct scm_vm_cont {
  /* IP of newest frame.  */
  scm_t_uint32 *ra;
  /* Offset of FP of newest frame, relative to stack top.  */
  scm_t_ptrdiff fp_offset;
  /* Besides being the stack size, this is also the offset of the SP of
     the newest frame.  */
  scm_t_ptrdiff stack_size;
  /* Stack bottom, which also keeps saved stack alive for GC.  */
  union scm_vm_stack_element *stack_bottom;
  /* Saved dynamic stack, with prompts relocated to record saved SP/FP
     offsets from the stack top of this scm_vm_cont.  */
  scm_t_dynstack *dynstack;
  /* See the continuation is partial and/or rewindable.  */
  scm_t_uint32 flags;
};

#define SCM_VM_CONT_P(OBJ)	(SCM_HAS_TYP7 (OBJ, scm_tc7_vm_cont))
#define SCM_VM_CONT_DATA(CONT)	((struct scm_vm_cont *) SCM_CELL_WORD_1 (CONT))
#define SCM_VM_CONT_PARTIAL_P(CONT) (SCM_VM_CONT_DATA (CONT)->flags & SCM_F_VM_CONT_PARTIAL)
#define SCM_VM_CONT_REWINDABLE_P(CONT) (SCM_VM_CONT_DATA (CONT)->flags & SCM_F_VM_CONT_REWINDABLE)

SCM_API SCM scm_load_compiled_with_vm (SCM file);

SCM_INTERNAL SCM scm_i_call_with_current_continuation (SCM proc);
SCM_INTERNAL SCM scm_i_capture_current_stack (void);
SCM_INTERNAL SCM scm_i_vm_capture_stack (union scm_vm_stack_element *stack_top,
                                         union scm_vm_stack_element *fp,
                                         union scm_vm_stack_element *sp,
                                         scm_t_uint32 *ra,
                                         scm_t_dynstack *dynstack,
                                         scm_t_uint32 flags);
SCM_INTERNAL int scm_i_vm_cont_to_frame (SCM cont, struct scm_frame *frame);
SCM_INTERNAL void scm_i_vm_cont_print (SCM x, SCM port,
                                       scm_print_state *pstate);
SCM_INTERNAL int scm_i_vm_is_boot_continuation_code (scm_t_uint32 *ip);
SCM_INTERNAL void scm_bootstrap_vm (void);
SCM_INTERNAL void scm_init_vm (void);

#endif /* _SCM_VM_H_ */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
