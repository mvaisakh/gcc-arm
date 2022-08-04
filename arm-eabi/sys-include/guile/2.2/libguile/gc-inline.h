/* classes: h_files */

#ifndef SCM_GC_INLINE_H
#define SCM_GC_INLINE_H

/* Copyright (C) 1995, 1996, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2006,
 *   2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014 Free Software Foundation, Inc.
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

/* Much of this file was copied from gc_inline.h, from the BDW
 * collector.  Its copyright notice is:
 *
 * Copyright 1988, 1989 Hans-J. Boehm, Alan J. Demers
 * Copyright (c) 1991-1995 by Xerox Corporation.  All rights reserved.
 * Copyright (c) 2005 Hewlett-Packard Development Company, L.P.
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose,  provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 */



#include "libguile/__scm.h"

#include "libguile/gc.h"
#include "libguile/bdw-gc.h"
#include "libguile/threads.h"

#include <gc/gc_inline.h> /* GC_generic_malloc_many */



#define SCM_INLINE_GC_GRANULE_WORDS 2
#define SCM_INLINE_GC_GRANULE_BYTES \
  (sizeof(void *) * SCM_INLINE_GC_GRANULE_WORDS)

/* A freelist set contains SCM_INLINE_GC_FREELIST_COUNT pointers to
   singly linked lists of objects of different sizes, the ith one
   containing objects i + 1 granules in size.  This setting of
   SCM_INLINE_GC_FREELIST_COUNT will hold freelists for allocations of
   up to 256 bytes.  */
#define SCM_INLINE_GC_FREELIST_COUNT (256U / SCM_INLINE_GC_GRANULE_BYTES)

static inline size_t
scm_inline_gc_bytes_to_freelist_index (size_t bytes)
{
  return (bytes - 1U) / SCM_INLINE_GC_GRANULE_BYTES;
}

static inline size_t
scm_inline_gc_freelist_object_size (size_t idx)
{
  return (idx + 1U) * SCM_INLINE_GC_GRANULE_BYTES;
}

/* The values of these must match the internal POINTERLESS and NORMAL
   definitions in libgc, for which unfortunately there are no external
   definitions.  Alack.  */
typedef enum scm_inline_gc_kind
  {
    SCM_INLINE_GC_KIND_POINTERLESS,
    SCM_INLINE_GC_KIND_NORMAL
  } scm_inline_gc_kind;

static inline void *
scm_inline_gc_alloc (void **freelist, size_t idx, scm_inline_gc_kind kind)
{
  void *head = *freelist;

  if (SCM_UNLIKELY (!head))
    {
      size_t bytes = scm_inline_gc_freelist_object_size (idx);
      GC_generic_malloc_many (bytes, kind, freelist);
      head = *freelist;
      if (SCM_UNLIKELY (!head))
        return (*GC_get_oom_fn ()) (bytes);
    }

  *freelist = *(void **)(head);

  return head;
}

static inline void *
scm_inline_gc_malloc_pointerless (scm_i_thread *thread, size_t bytes)
{
  size_t idx = scm_inline_gc_bytes_to_freelist_index (bytes);

  if (SCM_UNLIKELY (idx >= SCM_INLINE_GC_FREELIST_COUNT))
    return GC_malloc_atomic (bytes);

  return scm_inline_gc_alloc
    (&thread->pointerless_freelists[idx], idx, SCM_INLINE_GC_KIND_POINTERLESS);
}

static inline void *
scm_inline_gc_malloc (scm_i_thread *thread, size_t bytes)
{
  size_t idx = scm_inline_gc_bytes_to_freelist_index (bytes);

  if (SCM_UNLIKELY (idx >= SCM_INLINE_GC_FREELIST_COUNT))
    return GC_malloc (bytes);

  return scm_inline_gc_alloc
    (&thread->freelists[idx], idx, SCM_INLINE_GC_KIND_NORMAL);
}

static inline void *
scm_inline_gc_malloc_words (scm_i_thread *thread, size_t words)
{
  return scm_inline_gc_malloc (thread, words * sizeof (void *));
}

static inline SCM
scm_inline_cell (scm_i_thread *thread, scm_t_bits car, scm_t_bits cdr)
{
  SCM cell = SCM_PACK_POINTER (scm_inline_gc_malloc_words (thread, 2));
  
  SCM_GC_SET_CELL_WORD (cell, 0, car);
  SCM_GC_SET_CELL_WORD (cell, 1, cdr);

  return cell;
}

static inline SCM
scm_inline_double_cell (scm_i_thread *thread, scm_t_bits car, scm_t_bits cbr,
                           scm_t_bits ccr, scm_t_bits cdr)
{
  SCM cell = SCM_PACK_POINTER (scm_inline_gc_malloc_words (thread, 4));
  
  SCM_GC_SET_CELL_WORD (cell, 0, car);
  SCM_GC_SET_CELL_WORD (cell, 1, cbr);
  SCM_GC_SET_CELL_WORD (cell, 2, ccr);
  SCM_GC_SET_CELL_WORD (cell, 3, cdr);

  return cell;
}

static inline SCM
scm_inline_words (scm_i_thread *thread, scm_t_bits car, scm_t_uint32 n_words)
{
  SCM obj = SCM_PACK_POINTER (scm_inline_gc_malloc_words (thread, n_words));
  
  SCM_GC_SET_CELL_WORD (obj, 0, car);

  return obj;
}

static inline SCM
scm_inline_cons (scm_i_thread *thread, SCM x, SCM y)
{
  return scm_inline_cell (thread, SCM_UNPACK (x), SCM_UNPACK (y));
}


#endif  /* SCM_GC_INLINE_H */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
