
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

#ifndef _LIBRS232_R_UTILS_H
#define _LIBRS232_R_UTILS_H          1

#ifndef LIBRS232_COMPILE
# ifdef _LIBRS232_H
#  error "Never use <r_utils.h> directly; include <librs232.h> instead."
# endif
#endif

bool is_supported_speed(enum rs232_speed speed);
bool is_tty_device(const char *device);
unsigned int rs232_speed_to_termios_speed(enum rs232_speed speed);

#endif

