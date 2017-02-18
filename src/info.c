
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

__PUB_API__ const char *rs232_device_name(const rs232_t *rs232)
{
    struct rs232_s *s = rs232_ref((rs232_t *)rs232);
    char *ptr = NULL;

    errno_clear();

    if (NULL == rs232) {
        errno_set(RS232_NULL_ARG);
        return NULL;
    }

    ptr = s->device;
    rs232_unref(s);

    return ptr;
}

__PUB_API__ int rs232_fd(const rs232_t *rs232)
{
    struct rs232_s *s = rs232_ref((rs232_t *)rs232);
    int fd;

    errno_clear();

    if (NULL == rs232) {
        errno_set(RS232_NULL_ARG);
        return -1;
    }

    fd = s->fd;
    rs232_unref(s);

    return fd;
}

__PUB_API__ enum rs232_speed rs232_speed(const rs232_t *rs232)
{
    struct rs232_s *s = rs232_ref((rs232_t *)rs232);
    enum rs232_speed speed = RS232_SPD_UNKNOWN;

    errno_clear();

    if (NULL == rs232) {
        errno_set(RS232_NULL_ARG);
        return -1;
    }

    speed = s->speed;
    rs232_unref(s);

    return speed;
}

