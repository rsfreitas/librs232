
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

static void usage(void)
{
    printf("Usage: rs232 [OPTIONS]\n");
    printf("An example showing how to use the librs232 API.\n\n");
    printf("Options:\n");
    printf(" -h\t\tShows this help screen.\n");
    printf(" -d [device]\tIndicates the device to be manipulated.\n");
    printf(" -s [speed]\tIndicate the port Baud Rate.\n");
    printf("\n");
    printf("Supported baud rates:\n");
    printf("Speed - Baud Rate\n%-5d - 4800\n%-5d - 9600\n%-5d - 19200\n"
           "%-5d - 38400\n%-5d - 115200\n", 1, 2, 3, 4, 5);
    printf("\n");
}

static void trap_signal(int sign)
{
    switch (sign) {
        case SIGINT:
            __finish = true;
            break;
    }
}

static void sends_some_data(rs232_t *dev)
{
    char c = '!';
    cl_string_t *s = NULL;

    /* Sends every printable character */
    for (; c < '~'; c++)
        rs232_putc(dev, c);

    rs232_putc(dev, '\n');

    /* Sends some strings */
    for (c = 2; c < 127; c++) {
        s = cl_string_create_random((unsigned int)c);
        rs232_puts(dev, cl_string_valueof(s), cl_string_length(s));
        rs232_putc(dev, '\n');
        cl_string_unref(s);
    }
}

int main(int argc, char **argv)
{
    rs232_t *dev = NULL;
    const char *opt = "hd:s:\0";
    int option, c;
    char *device = NULL;
    enum rs232_speed speed = RS232_SPD_115200;

    do {
        option = getopt(argc, argv, opt);

        switch (option) {
            case 'h':
                usage();
                return 1;

            case 's':
                speed = atoi(optarg);

                if ((speed < 1) || (speed > 5)) {
                    printf("Unsupported RS232 speed.\n");
                    return -1;
                }

                break;

            case 'd':
                device = strdup(optarg);
                break;

            case '?':
                return -1;
        }
    } while (option != -1);

    cl_init(NULL);
    dev = rs232_open(device, speed);

    if (NULL == dev) {
        printf("(1): %s\n", rs232_strerror(rs232_get_last_error()));
        return -1;
    }

    cl_trap(SIGINT, trap_signal);

    /* Sends a few bytes and words to the serial device */
    sends_some_data(dev);

    while (1) {
        if (__finish)
            break;

        cl_msleep(1);
        c = rs232_getc(dev, false);

        if (c != -1)
            printf("%02x\n", c);
    }

    rs232_close(dev);
    cl_uninit();

    if (device != NULL)
        free(device);

    return 0;
}

