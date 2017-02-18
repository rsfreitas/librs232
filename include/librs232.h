
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

#ifndef _LIBRS232_H
#define _LIBRS232_H         1

/** Port speed */
enum rs232_speed {
    RS232_SPD_UNKNOWN,
    RS232_SPD_4800,
    RS232_SPD_9600,
    RS232_SPD_19200,
    RS232_SPD_38400,
    RS232_SPD_115200
};

#ifdef LIBRS232_COMPILE
# define MAJOR_VERSION      0
# define MINOR_VERSION      1
# define RELEASE            1

# include "rs232_lib/r_internal.h"
#endif

/** RS232 port object */
typedef void                rs232_t;

#include "rs232_lib/r_error.h"
#include "rs232_lib/r_utils.h"
#include "rs232_lib/r_info.h"
#include "rs232_lib/r_tty.h"

#endif

