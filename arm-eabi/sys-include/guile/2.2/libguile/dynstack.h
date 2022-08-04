/* classes: h_files */

#ifndef SCM_DYNSTACK_H
#define SCM_DYNSTACK_H

/* Copyright (C) 2012, 2013 Free Software Foundation, Inc.
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



typedef struct
{
  scm_t_bits *base;
  scm_t_bits *top;
  scm_t_bits *limit;
} scm_t_dynstack;



/* Items on the dynstack are preceded by two-word headers, giving the
   offset of the preceding item (or 0 if there is none) and the type,
   flags, and length of the following dynstack entry, in words.  In
   addition, there is a "null header" at the top of the stack,
   indicating the length of the previous item, but with a tag of zero.

   For example, consider an empty dynstack, with a capacity of 6 words:

   +----------+----------+                                           +
   |prev=0    |tag=0     |                                           |
   +----------+----------+                                           +
   ^base                 ^top                                   limit^

   Now we evaluate (dynamic-wind enter thunk leave).  That will result
   in a dynstack of:

                         / the len=2 words     \
   +----------+----------+----------+----------+----------+----------+
   |prev=0    |tag:len=2 |enter     |leave     |prev=4    |tag=0     |
   +----------+----------+----------+----------+----------+----------+
   ^base                                                    top,limit^

   The tag is a combination of the type of the dynstack item, some flags
   associated with the item, and the length of the item.  See
   SCM_MAKE_DYNSTACK_TAG below for the details.

   This arrangement makes it possible to have variable-length dynstack
   items, and yet be able to traverse them forwards or backwards.  */

#define SCM_DYNSTACK_HEADER_LEN 2

#define SCM_DYNSTACK_PREV_OFFSET(top) ((top)[-2])
#define SCM_DYNSTACK_SET_PREV_OFFSET(top, offset) (top)[-2] = (offset)

#define SCM_DYNSTACK_TAG(top) ((top)[-1])
#define SCM_DYNSTACK_SET_TAG(top, tag) (top)[-1] = (tag)

typedef enum {
  SCM_DYNSTACK_TYPE_NONE = 0,
  SCM_DYNSTACK_TYPE_FRAME,
  SCM_DYNSTACK_TYPE_UNWINDER,
  SCM_DYNSTACK_TYPE_REWINDER,
  SCM_DYNSTACK_TYPE_WITH_FLUID,
  SCM_DYNSTACK_TYPE_PROMPT,
  SCM_DYNSTACK_TYPE_DYNWIND,
  SCM_DYNSTACK_TYPE_DYNAMIC_STATE,
} scm_t_dynstack_item_type;

#define SCM_DYNSTACK_TAG_TYPE_MASK 0xf
#define SCM_DYNSTACK_TAG_FLAGS_MASK 0xf0
#define SCM_DYNSTACK_TAG_FLAGS_SHIFT 4
#define SCM_DYNSTACK_TAG_LEN_SHIFT 8

#define SCM_MAKE_DYNSTACK_TAG(type, flags, len)           \
  ((type) | (flags) | ((len) << SCM_DYNSTACK_TAG_LEN_SHIFT))

#define SCM_DYNSTACK_TAG_TYPE(tag)                     \
  ((tag) & SCM_DYNSTACK_TAG_TYPE_MASK)
#define SCM_DYNSTACK_TAG_FLAGS(tag)                    \
  ((tag) & SCM_DYNSTACK_TAG_FLAGS_MASK)
#define SCM_DYNSTACK_TAG_LEN(tag)                      \
  ((tag) >> SCM_DYNSTACK_TAG_LEN_SHIFT)

#define SCM_DYNSTACK_PREV(top)                                          \
  (SCM_DYNSTACK_PREV_OFFSET (top)                                       \
   ? ((top) - SCM_DYNSTACK_PREV_OFFSET (top)) : NULL)
#define SCM_DYNSTACK_NEXT(top)                                          \
  (SCM_DYNSTACK_TAG (top)                                               \
   ? ((top) + SCM_DYNSTACK_TAG_LEN (SCM_DYNSTACK_TAG (top))             \
      + SCM_DYNSTACK_HEADER_LEN)                                        \
   : NULL)

#define SCM_DYNSTACK_FIRST(dynstack) \
  ((dynstack)->base + SCM_DYNSTACK_HEADER_LEN)

#define SCM_DYNSTACK_CAPACITY(dynstack) \
  ((dynstack)->limit - (dynstack)->base)
#define SCM_DYNSTACK_SPACE(dynstack) \
  ((dynstack)->limit - (dynstack)->top)
#define SCM_DYNSTACK_HEIGHT(dynstack) \
  ((dynstack)->top - (dynstack)->base)

#define SCM_DYNSTACK_HAS_SPACE(dynstack, n) \
  (SCM_DYNSTACK_SPACE (dynstack) >= n + SCM_DYNSTACK_HEADER_LEN)

typedef enum {
  SCM_F_DYNSTACK_FRAME_REWINDABLE = (1 << SCM_DYNSTACK_TAG_FLAGS_SHIFT)
} scm_t_dynstack_frame_flags;

typedef enum {
  SCM_F_DYNSTACK_WINDER_EXPLICIT = (1 << SCM_DYNSTACK_TAG_FLAGS_SHIFT)
} scm_t_dynstack_winder_flags;

typedef enum {
  SCM_F_DYNSTACK_PROMPT_ESCAPE_ONLY = (1 << SCM_DYNSTACK_TAG_FLAGS_SHIFT)
} scm_t_dynstack_prompt_flags;

typedef void (*scm_t_guard) (void *);




/* Pushing and popping entries on the dynamic stack.  */

SCM_INTERNAL void scm_dynstack_push_frame (scm_t_dynstack *,
                                           scm_t_dynstack_frame_flags);
SCM_INTERNAL void scm_dynstack_push_rewinder (scm_t_dynstack *,
                                              scm_t_dynstack_winder_flags,
                                              scm_t_guard, void *);
SCM_INTERNAL void scm_dynstack_push_unwinder (scm_t_dynstack *,
                                              scm_t_dynstack_winder_flags,
                                              scm_t_guard, void *);
SCM_INTERNAL void scm_dynstack_push_fluid (
  scm_t_dynstack *, SCM fluid, SCM value,
  scm_t_dynamic_state *dynamic_state);
SCM_INTERNAL void scm_dynstack_push_dynamic_state (scm_t_dynstack *, SCM,
                                                   scm_t_dynamic_state *);
SCM_INTERNAL void scm_dynstack_push_prompt (scm_t_dynstack *,
                                            scm_t_dynstack_prompt_flags,
                                            SCM key,
                                            scm_t_ptrdiff fp_offset,
                                            scm_t_ptrdiff sp_offset,
                                            scm_t_uint32 *ip,
                                            scm_i_jmp_buf *registers);
SCM_INTERNAL void scm_dynstack_push_dynwind (scm_t_dynstack *,
                                             SCM enter, SCM leave);

SCM_INTERNAL void scm_dynstack_pop (scm_t_dynstack *);




/* Capturing, winding, and unwinding.  */

SCM_INTERNAL scm_t_dynstack* scm_dynstack_capture_all (scm_t_dynstack *dynstack);
SCM_INTERNAL scm_t_dynstack* scm_dynstack_capture (scm_t_dynstack *dynstack,
                                                   scm_t_bits *item);

SCM_INTERNAL void scm_dynstack_wind_1 (scm_t_dynstack *, scm_t_bits *);
SCM_INTERNAL scm_t_bits scm_dynstack_unwind_1 (scm_t_dynstack *);

SCM_INTERNAL void scm_dynstack_wind (scm_t_dynstack *, scm_t_bits *);
SCM_INTERNAL void scm_dynstack_unwind (scm_t_dynstack *, scm_t_bits *);




/* Miscellany.  */

SCM_INTERNAL scm_t_bits* scm_dynstack_unwind_fork (scm_t_dynstack *,
                                                   scm_t_dynstack *);

SCM_INTERNAL void scm_dynstack_unwind_frame (scm_t_dynstack *);
SCM_INTERNAL void scm_dynstack_unwind_fluid
  (scm_t_dynstack *dynstack, scm_t_dynamic_state *dynamic_state);
SCM_INTERNAL void scm_dynstack_unwind_dynamic_state
  (scm_t_dynstack *dynstack, scm_t_dynamic_state *dynamic_state);

SCM_INTERNAL scm_t_bits* scm_dynstack_find_prompt (scm_t_dynstack *, SCM,
                                                   scm_t_dynstack_prompt_flags *,
                                                   scm_t_ptrdiff *,
                                                   scm_t_ptrdiff *,
                                                   scm_t_uint32 **,
                                                   scm_i_jmp_buf **);

SCM_INTERNAL SCM scm_dynstack_find_old_fluid_value (scm_t_dynstack *,
                                                    SCM, size_t, SCM);

SCM_INTERNAL void scm_dynstack_relocate_prompts (scm_t_dynstack *,
                                                 scm_t_ptrdiff);

SCM_INTERNAL void scm_dynstack_wind_prompt (scm_t_dynstack *, scm_t_bits *,
                                            scm_t_ptrdiff, scm_i_jmp_buf *);


#endif  /* SCM_DYNSTACK_H */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
