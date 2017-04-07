
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

#ifndef _LIBRS232_R_TTY_H
#define _LIBRS232_R_TTY_H          1

#ifndef LIBRS232_COMPILE
# ifndef _LIBRS232_H
#  error "Never use <r_tty.h> directly; include <librs232.h> instead."
# endif
#endif

/**
 * @name rs232_ref
 * @brief Increases the reference count of a rs232_t object.
 *
 * @param [in] rs232: The rs232_t object.
 *
 * @return On success returns the object itself with its reference count
 *         increased or NULL otherwise.
 */
rs232_t *rs232_ref(rs232_t *rs232);

/**
 * @name rs232_unref
 * @brief Decreases the reference count for a rs232_t object.
 *
 * When its reference count drops to 0, the item is finalized ans its memory is
 * released.
 *
 * @param [in] rs232: The rs232_t object.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int rs232_unref(rs232_t *rs232);

/**
 * @name rs232_open
 * @brief Opens a RS232 device to communication.
 *
 * @param [in] device: The device pathname.
 * @param [in] speed: The speed used in communication.
 *
 * @return On success returns a rs232_t object ready to communicate or NULL
 *         otherwise.
 */
rs232_t *rs232_open(const char *device, enum rs232_speed speed);

/**
 * @name rs232_close
 * @brief Closes a RS232 device.
 *
 * @param [in] rs232: The rs232_t object.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int rs232_close(rs232_t *rs232);

/**
 * @name rs232_getc
 * @brief Reads a byte from the RS232 device.
 *
 * @param [in] rs232: The rs232_t object.
 * @param [in] block: A boolean flag to indicate if the function will block until
 *                    some data is received or not.
 *
 * @return On success returns the read byte from the device or -1 otherwise.
 */
int rs232_getc(rs232_t *rs232, bool block);

/**
 * @name rs232_putc
 * @brief Writes a byte into the RS232 device.
 *
 * This function is not thread-safe.
 *
 * @param [in] rs232: The rs232_t object.
 * @param [in] byte: The data which will be written.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int rs232_putc(rs232_t *rs232, unsigned char byte);

/**
 * @name rs232_puts
 * @brief Writes a string into the RS232 device.
 *
 * This function is not thread-safe.
 *
 * @param [in] rs232: The rs232_t object.
 * @param [in] data: The data which will be written.
 * @param [in] data_size: The size of the data.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int rs232_puts(rs232_t *rs232, const char *data, unsigned int data_size);

/**
 * @name rs232_flush
 * @brief Clears a communication channel with a RS232 device.
 *
 * @param [in] rs232: The rs232_t object.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int rs232_flush(rs232_t *rs232);

#endif

