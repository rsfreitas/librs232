
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Apr  1 22:20:49 2017
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Change this to the libcollections */
#include <signal.h>

#include <collections.h>

#include "librs232.h"

static bool __finish = false;

static void trap_signal(int sign)
{
    switch (sign) {
        case SIGINT:
            __finish = true;
            break;
    }
}

int main(int argc, char **argv)
{
    rs232_t *dev = NULL;
    const char *opt = "d:\0";
    int option, c;
    char *device = NULL;

    do {
        option = getopt(argc, argv, opt);

        switch (option) {
            case 'd':
                device = strdup(optarg);
                break;

            case '?':
                return -1;
        }
    } while (option != -1);

    cl_init(NULL);
    dev = rs232_open(device, RS232_SPD_115200);

    if (NULL == dev) {
        printf("(1): %s\n", rs232_strerror(rs232_get_last_error()));
        return -1;
    }

    cl_trap(SIGINT, trap_signal);

    while (1) {
        if (__finish)
            break;

        cl_msleep(1);
        c = rs232_getc(dev, false);

        if (c != -1)
            printf("leu: %02x\n", c);
    }

    rs232_close(dev);
    cl_uninit();

    if (device != NULL)
        free(device);

    return 0;
}

