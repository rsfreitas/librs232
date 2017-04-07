
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

#ifndef _LIBRS232_R_INFO_H
#define _LIBRS232_R_INFO_H          1

#ifndef LIBRS232_COMPILE
# ifndef _LIBRS232_H
#  error "Never use <r_info.h> directly; include <librs232.h> instead."
# endif
#endif

/**
 * @name rs232_device_name
 * @brief Gets the device name of a rs232_t object.
 *
 * @param [in] rs232: The rs232_t object.
 *
 * @return On success returns the device name or NULL otherwise.
 */
const char *rs232_device_name(const rs232_t *rs232);

/**
 * @name rs232_fd
 * @brief Gets the file descriptor of a openned rs232_t object.
 *
 * @param [in] rs232: The rs232_t object.
 *
 * @return On success returns the file descriptor or -1 otherwise.
 */
int rs232_fd(const rs232_t *rs232);

/**
 * @name rs232_speed
 * @brief Gets the current speed of a rs232_t object.
 *
 * @param [in] rs232: The rs232_t object.
 *
 * @return On success returns the current speed or -1 otherwise.
 */
enum rs232_speed rs232_speed(const rs232_t *rs232);

#endif

