/* classes: h_files */

#ifndef SCM_PORTS_H
#define SCM_PORTS_H

/* Copyright (C) 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2003, 2004,
 *   2006, 2008, 2009, 2010, 2011, 2012, 2013, 2014 Free Software Foundation, Inc.
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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "libguile/bytevectors.h"
#include "libguile/gc.h"
#include "libguile/tags.h"
#include "libguile/error.h"
#include "libguile/print.h"
#include "libguile/struct.h"
#include "libguile/threads.h"
#include "libguile/strings.h"
#include "libguile/vectors.h"



SCM_INTERNAL SCM scm_i_port_weak_set;




#define SCM_EOF_OBJECT_P(x) (scm_is_eq ((x), SCM_EOF_VAL))

/* A port's first word contains its tag, which is a tc7 value.  Above
   there is a flag indicating whether the port is open or not, and then
   some "mode bits": flags indicating whether the port is an input
   and/or an output port and how Guile should buffer the port.  */
#define SCM_OPN		(1U<<8) /* Is the port open? */
#define SCM_RDNG	(1U<<9) /* Is it a readable port? */
#define SCM_WRTNG	(1U<<10) /* Is it writable? */
#define SCM_BUF0	(1U<<11) /* Is it unbuffered? */
#define SCM_BUFLINE     (1U<<12) /* Is it line-buffered? */
#ifdef BUILDING_LIBGUILE
#define SCM_F_PORT_FINALIZING (1U<<13) /* Port is being closed via GC. */
#endif

#define SCM_PORTP(x) (SCM_HAS_TYP7 (x, scm_tc7_port))
#define SCM_OPPORTP(x) (SCM_PORTP (x) && (SCM_CELL_WORD_0 (x) & SCM_OPN))
#define SCM_INPUT_PORT_P(x) (SCM_PORTP (x) && (SCM_CELL_WORD_0 (x) & SCM_RDNG))
#define SCM_OUTPUT_PORT_P(x) (SCM_PORTP (x) && (SCM_CELL_WORD_0 (x) & SCM_WRTNG))
#define SCM_OPINPORTP(x) (SCM_OPPORTP (x) && SCM_INPUT_PORT_P (x))
#define SCM_OPOUTPORTP(x) (SCM_OPPORTP (x) && SCM_OUTPUT_PORT_P (x))
#define SCM_OPENP(x) (SCM_OPPORTP (x))
#define SCM_CLOSEDP(x) (!SCM_OPENP (x))
#define SCM_CLR_PORT_OPEN_FLAG(p) \
  SCM_SET_CELL_WORD_0 ((p), SCM_CELL_WORD_0 (p) & ~SCM_OPN)
#ifdef BUILDING_LIBGUILE
#define SCM_PORT_FINALIZING_P(x) \
  (SCM_CELL_WORD_0 (x) & SCM_F_PORT_FINALIZING)
#define SCM_SET_PORT_FINALIZING(p) \
  SCM_SET_CELL_WORD_0 ((p), SCM_CELL_WORD_0 (p) | SCM_F_PORT_FINALIZING)
#endif

typedef struct scm_t_port_type scm_t_port_type;
typedef struct scm_t_port scm_t_port;

#define SCM_STREAM(port) (SCM_CELL_WORD_1 (port))
#define SCM_SETSTREAM(port, stream) (SCM_SET_CELL_WORD_1 (port, stream))
#define SCM_PORT(x)         ((scm_t_port *) SCM_CELL_WORD_2 (x))
#define SCM_PORT_TYPE(port) ((scm_t_port_type *) SCM_CELL_WORD_3 (port))




/* Port types, and their vtables.  */
SCM_API scm_t_port_type *scm_make_port_type
	(char *name,
         size_t (*read) (SCM port, SCM dst, size_t start, size_t count),
         size_t (*write) (SCM port, SCM src, size_t start, size_t count));
SCM_API void scm_set_port_scm_read (scm_t_port_type *ptob, SCM read);
SCM_API void scm_set_port_scm_write (scm_t_port_type *ptob, SCM write);
SCM_API void scm_set_port_read_wait_fd (scm_t_port_type *ptob,
                                        int (*wait_fd) (SCM port));
SCM_API void scm_set_port_write_wait_fd (scm_t_port_type *ptob,
                                         int (*wait_fd) (SCM port));
SCM_API void scm_set_port_print (scm_t_port_type *ptob,
				 int (*print) (SCM exp,
					       SCM port,
					       scm_print_state *pstate));
SCM_API void scm_set_port_close (scm_t_port_type *ptob, void (*close) (SCM));
SCM_API void scm_set_port_needs_close_on_gc (scm_t_port_type *ptob,
                                             int needs_close_p);
SCM_API void scm_set_port_seek (scm_t_port_type *ptob,
				scm_t_off (*seek) (SCM port,
						   scm_t_off OFFSET,
						   int WHENCE));
SCM_API void scm_set_port_truncate (scm_t_port_type *ptob,
				    void (*truncate) (SCM port,
						      scm_t_off length));
SCM_API void scm_set_port_input_waiting (scm_t_port_type *ptob,
                                         int (*input_waiting) (SCM));
SCM_API void scm_set_port_get_natural_buffer_sizes
  (scm_t_port_type *ptob,
   void (*get_natural_buffer_sizes) (SCM, size_t *, size_t *));
SCM_API void scm_set_port_random_access_p (scm_t_port_type *ptob,
                                           int (*random_access_p) (SCM port));

/* The input, output, error, and load ports.  */
SCM_API SCM scm_current_input_port (void);
SCM_API SCM scm_current_output_port (void);
SCM_API SCM scm_current_error_port (void);
SCM_API SCM scm_current_warning_port (void);
SCM_API SCM scm_current_load_port (void);
SCM_API SCM scm_set_current_input_port (SCM port);
SCM_API SCM scm_set_current_output_port (SCM port);
SCM_API SCM scm_set_current_error_port (SCM port);
SCM_API SCM scm_set_current_warning_port (SCM port);
SCM_API void scm_dynwind_current_input_port (SCM port);
SCM_API void scm_dynwind_current_output_port (SCM port);
SCM_API void scm_dynwind_current_error_port (SCM port);
SCM_INTERNAL void scm_i_dynwind_current_load_port (SCM port);

/* Mode bits.  */
SCM_INTERNAL long scm_i_mode_bits (SCM modes);
SCM_API long scm_mode_bits (char *modes);
SCM_API SCM scm_port_mode (SCM port);

/* Low-level constructors.  */
SCM_API SCM scm_c_make_port_with_encoding (scm_t_port_type *ptob,
                                           unsigned long mode_bits,
                                           SCM encoding,
                                           SCM conversion_strategy,
                                           scm_t_bits stream);
SCM_API SCM scm_c_make_port (scm_t_port_type *ptob, unsigned long mode_bits,
                             scm_t_bits stream);

/* Predicates.  */
SCM_API SCM scm_port_p (SCM x);
SCM_API SCM scm_input_port_p (SCM x);
SCM_API SCM scm_output_port_p (SCM x);
SCM_API SCM scm_port_closed_p (SCM port);
SCM_API SCM scm_eof_object_p (SCM x);

/* Closing ports.  */
SCM_API SCM scm_close_port (SCM port);
SCM_API SCM scm_close_input_port (SCM port);
SCM_API SCM scm_close_output_port (SCM port);

/* Encoding characters to byte streams, and decoding byte streams to
   characters.  */
SCM_INTERNAL scm_t_string_failed_conversion_handler
scm_i_string_failed_conversion_handler (SCM conversion_strategy);
SCM_INTERNAL SCM scm_i_default_port_encoding (void);
SCM_INTERNAL void scm_i_set_default_port_encoding (const char *encoding);
SCM_INTERNAL SCM scm_i_default_port_conversion_strategy (void);
SCM_INTERNAL void scm_i_set_default_port_conversion_strategy (SCM strategy);
SCM_INTERNAL void scm_i_set_port_encoding_x (SCM port, const char *str);
SCM_INTERNAL SCM scm_sys_port_encoding (SCM port);
SCM_INTERNAL SCM scm_sys_set_port_encoding_x (SCM port, SCM encoding);
SCM_API SCM scm_port_encoding (SCM port);
SCM_API SCM scm_set_port_encoding_x (SCM port, SCM encoding);
SCM_API SCM scm_port_conversion_strategy (SCM port);
SCM_API SCM scm_set_port_conversion_strategy_x (SCM port, SCM behavior);

/* Input.  */
SCM_INTERNAL SCM scm_port_maybe_consume_initial_byte_order_mark (SCM, SCM, SCM);
SCM_API int scm_get_byte_or_eof (SCM port);
SCM_API int scm_peek_byte_or_eof (SCM port);
SCM_API size_t scm_c_read (SCM port, void *buffer, size_t size);
SCM_API size_t scm_c_read_bytes (SCM port, SCM dst, size_t start, size_t count);
SCM_API scm_t_wchar scm_getc (SCM port);
SCM_API SCM scm_read_char (SCM port);

/* Pushback.  */
SCM_API void scm_unget_bytes (const unsigned char *buf, size_t len, SCM port);
SCM_API void scm_unget_byte (int c, SCM port);
SCM_API void scm_ungetc (scm_t_wchar c, SCM port);
SCM_API void scm_ungets (const char *s, int n, SCM port);
SCM_API SCM scm_peek_char (SCM port);
SCM_API SCM scm_unread_char (SCM cobj, SCM port);
SCM_API SCM scm_unread_string (SCM str, SCM port);

/* Manipulating the buffers.  */
SCM_API SCM scm_setvbuf (SCM port, SCM mode, SCM size);
SCM_INTERNAL SCM scm_fill_input (SCM port, size_t minimum_size,
                                 size_t *cur_out, size_t *avail_out);
SCM_INTERNAL size_t scm_take_from_input_buffers (SCM port, char *dest, size_t read_len);
SCM_API SCM scm_drain_input (SCM port);
SCM_API void scm_end_input (SCM port);
SCM_API SCM scm_force_output (SCM port);
SCM_API void scm_flush (SCM port);

SCM_INTERNAL SCM scm_port_random_access_p (SCM port);
SCM_INTERNAL SCM scm_port_read_buffering (SCM port);
SCM_INTERNAL SCM scm_expand_port_read_buffer_x (SCM port, SCM size,
                                                SCM putback_p);
SCM_INTERNAL SCM scm_port_read (SCM port);
SCM_INTERNAL SCM scm_port_write (SCM port);
SCM_INTERNAL SCM scm_port_read_buffer (SCM port);
SCM_INTERNAL SCM scm_port_write_buffer (SCM port);
SCM_INTERNAL SCM scm_port_auxiliary_write_buffer (SCM port);

/* Output.  */
SCM_API void scm_c_write (SCM port, const void *buffer, size_t size);
SCM_API void scm_c_write_bytes (SCM port, SCM src, size_t start, size_t count);
SCM_API void scm_c_put_latin1_chars (SCM port, const scm_t_uint8 *buf,
                                     size_t len);
SCM_API void scm_c_put_utf32_chars (SCM port, const scm_t_uint32 *buf,
                                    size_t len);
SCM_API void scm_c_put_string (SCM port, SCM str, size_t start, size_t count);
SCM_API SCM scm_put_string (SCM port, SCM str, SCM start, SCM count);
SCM_API void scm_c_put_char (SCM port, scm_t_wchar ch);
SCM_API SCM scm_put_char (SCM port, SCM ch);
SCM_INTERNAL void scm_c_put_escaped_char (SCM port, scm_t_wchar ch);
SCM_INTERNAL int scm_c_can_put_char (SCM port, scm_t_wchar ch);
SCM_API void scm_putc (char c, SCM port);
SCM_API void scm_puts (const char *str_data, SCM port);
SCM_API void scm_lfwrite (const char *ptr, size_t size, SCM port);
SCM_INTERNAL void scm_lfwrite_substr (SCM str, size_t start, size_t end,
				      SCM port);

/* Querying and setting positions, and character availability.  */
SCM_API SCM scm_char_ready_p (SCM port);
SCM_API SCM scm_seek (SCM object, SCM offset, SCM whence);
SCM_API SCM scm_truncate_file (SCM object, SCM length);
SCM_API SCM scm_port_line (SCM port);
SCM_API SCM scm_set_port_line_x (SCM port, SCM line);
SCM_API SCM scm_port_column (SCM port);
SCM_API SCM scm_set_port_column_x (SCM port, SCM line);
SCM_API SCM scm_port_filename (SCM port);
SCM_API SCM scm_set_port_filename_x (SCM port, SCM filename);

/* Port properties.  */
SCM_INTERNAL SCM scm_i_port_property (SCM port, SCM key);
SCM_INTERNAL SCM scm_i_set_port_property_x (SCM port, SCM key, SCM value);

/* Implementation helpers for port printing functions.  */
SCM_API int scm_port_print (SCM exp, SCM port, scm_print_state *);
SCM_API void scm_print_port_mode (SCM exp, SCM port);

/* Iterating over all ports.  */
SCM_API SCM scm_port_for_each (SCM proc);
SCM_API void scm_c_port_for_each (void (*proc)(void *data, SCM p), void *data);
SCM_API SCM scm_flush_all_ports (void);

/* Void ports.  */
SCM_API SCM scm_void_port (char * mode_str);
SCM_API SCM scm_sys_make_void_port (SCM mode);

/* Initialization.  */
SCM_INTERNAL void scm_init_ports (void);


#endif  /* SCM_PORTS_H */

/*
  Local Variables:
  c-file-style: "gnu"
  End:
*/
