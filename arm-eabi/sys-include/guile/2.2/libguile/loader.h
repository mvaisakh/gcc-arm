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

#ifndef _SCM_LOADER_H_
#define _SCM_LOADER_H_

#include <libguile.h>

SCM_API SCM scm_load_thunk_from_file (SCM filename);
SCM_API SCM scm_load_thunk_from_memory (SCM bv);

SCM_INTERNAL const scm_t_uint8 *
scm_find_slot_map_unlocked (const scm_t_uint32 *ip);

SCM_INTERNAL void scm_bootstrap_loader (void);
SCM_INTERNAL void scm_init_loader (void);

#endif /* _SCM_LOADER_H_ */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
