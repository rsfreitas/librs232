
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

#include <termios.h>
#include <sys/stat.h>

#include "librs232.h"

bool is_supported_speed(enum rs232_speed speed)
{
    switch (speed) {
        case RS232_SPD_4800:
        case RS232_SPD_9600:
        case RS232_SPD_19200:
        case RS232_SPD_38400:
        case RS232_SPD_115200:
            return true;

        default:
            break;
    }

    return false;
}

unsigned int rs232_speed_to_termios_speed(enum rs232_speed speed)
{
    switch (speed) {
        case RS232_SPD_4800:
            return B4800;

        case RS232_SPD_9600:
            return B9600;

        case RS232_SPD_19200:
            return B19200;

        case RS232_SPD_38400:
            return B38400;

        case RS232_SPD_115200:
            return B115200;

        default:
            break;
    }

    return 0;
}

bool is_tty_device(const char *device)
{
    struct stat st;

    if (stat(device, &st) < 0)
        return false;

    if (S_ISCHR(st.st_mode) == 0)
        return true;

    return false;
}

