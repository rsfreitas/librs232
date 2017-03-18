
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

#include "librs232.h"

static const char *__description[] = {
    cl_tr_noop("Ok"),
    cl_tr_noop("Internal memory allocation error"),
    cl_tr_noop("Invalid argument"),
    cl_tr_noop("The device is not a valid RS232 device"),
    cl_tr_noop("Unable to open the device"),
    cl_tr_noop("Unable to lock the device"),
    cl_tr_noop("The device was not properly initialized"),
    cl_tr_noop("Error while writing into the device"),
    cl_tr_noop("Unsupported speed"),
    cl_tr_noop("Error reading from the device"),
    cl_tr_noop("Timeout while reading from the device")
};

static const char *__unknown_error = cl_tr_noop("Unknown error");
#define __errno         (*cl_errno_storage())

void errno_clear(void)
{
    __errno = RS232_NO_ERROR;
}

void errno_set(enum rs232_error_code code)
{
    __errno = code;
}

enum rs232_error_code rs232_get_last_error(void)
{
    return __errno;
}

const char *rs232_strerror(enum rs232_error_code code)
{
    if (code >= RS232_MAX_ERROR_CODE)
        return __unknown_error;

    return __description[code];
}

