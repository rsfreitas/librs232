
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Fri Feb 17 09:50:15 2017
 * Project: librs232
 *
 * Copyright (C) 2017 Rodrigo Freitas
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#ifndef _LIBRS232_R_ERROR_H
#define _LIBRS232_R_ERROR_H          1

#ifndef LIBRS232_COMPILE
# ifndef _LIBRS232_H
#  error "Never use <r_error.h> directly; include <librs232.h> instead."
# endif
#endif

enum rs232_error_code {
    RS232_NO_ERROR,

    RS232_MALLOC_ERROR,
    RS232_NULL_ARG,
    RS232_NO_TTY_DEVICE,
    RS232_UNABLE_TO_OPEN,
    RS232_UNABLE_TO_LOCK_TTY,
    RS232_TTY_NOT_READY,
    RS232_WRITE_ERROR,
    RS232_UNSUPPORTED_SPEED,
    RS232_READ_ERROR,
    RS232_READ_TIMEOUT,

    RS232_MAX_ERROR_CODE
};

#ifdef LIBRS232_COMPILE
void errno_clear(void);
void errno_set(enum rs232_error_code code);
#endif

enum rs232_error_code rs232_get_last_error(void);
const char *rs232_strerror(enum rs232_error_code code);

#endif

