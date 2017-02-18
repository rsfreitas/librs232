
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

#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "librs232.h"

static void tty_close(struct rs232_s *rs232);

static void destroy_rs232_s(const struct cref_s *ref)
{
    struct rs232_s *s = cl_container_of(ref, struct rs232_s, ref);

    if (NULL == s)
        return;

    if (s->active == true)
        tty_close(s);

    if (s->device != NULL)
        free(s->device);

    free(s);
}

static struct rs232_s *new_rs232_s(void)
{
    struct rs232_s *s = NULL;

    s = calloc(1, sizeof(struct rs232_s));

    if (NULL == s) {
        errno_set(RS232_MALLOC_ERROR);
        return NULL;
    }

    s->active = false;
    s->speed = RS232_SPD_UNKNOWN;
    s->minor = -1;
    s->bytes_read = 0;
    s->returned_byte = 0;
    memset(s->read_buffer, 0, sizeof(s->read_buffer));

    /* Initialize reference count */
    s->ref.count = 1;
    s->ref.free = destroy_rs232_s;

    return s;
}

static int tty_open(struct rs232_s *rs232, const char *device,
    enum rs232_speed speed)
{
    struct termios tio;

    if (is_tty_device(device) == false) {
        errno_set(RS232_NO_TTY_DEVICE);
        return -1;
    }

    rs232->fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);

    if (rs232->fd < 0) {
        errno_set(RS232_UNABLE_TO_OPEN);
        return -1;
    }

    if (lockf(rs232->fd, F_TLOCK, 0)) {
        errno_set(RS232_UNABLE_TO_LOCK_TTY);
        return -1;
    }

    fcntl(rs232->fd, F_SETFL, 0);
    tcgetattr(rs232->fd, &tio);

    tio.c_iflag &= ~BRKINT;
    tio.c_iflag &= ~ICRNL;
    tio.c_iflag &= ~INLCR;
    tio.c_iflag &= ~INPCK;
    tio.c_iflag &= ~PARMRK;
    tio.c_iflag &= ~ISTRIP;
    tio.c_iflag &= ~IXON;
    tio.c_iflag &= ~IGNCR;
    tio.c_iflag &= ~IGNPAR;
    tio.c_iflag |= IGNBRK;
    tio.c_iflag |= IXOFF;

    tio.c_oflag &= ~ONLCR;
    tio.c_oflag &= ~OCRNL;
    tio.c_oflag &= ~ONLRET;
    tio.c_oflag &= ~OPOST;
    tio.c_oflag &= ~ONOCR;

    tio.c_cflag &= ~HUPCL;
    tio.c_cflag &= ~PARENB;
    tio.c_cflag &= ~PARODD;
    tio.c_cflag &= ~CRTSCTS;
    tio.c_cflag &= ~CSTOPB;
    tio.c_cflag |= CLOCAL;
    tio.c_cflag |= CREAD;
    tio.c_cflag |= CS8;

    tio.c_lflag = 0;

    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 0;

    cfsetispeed(&tio, rs232_speed_to_termios_speed(speed));
    cfsetospeed(&tio, rs232_speed_to_termios_speed(speed));

    tcsetattr(rs232->fd, TCSANOW, &tio);
    tcflush(rs232->fd, TCIOFLUSH);

    rs232->device = strdup(device);
    rs232->speed = speed;
    rs232->active = true;

    return 0;
}

static void tty_close(struct rs232_s *rs232)
{
    if ((rs232->fd == -1) || (rs232->active == false))
        return;

    lockf(rs232->fd, F_ULOCK, 0);
    close(rs232->fd);

    rs232->active = false;
}

static void tty_flush(struct rs232_s *rs232)
{
    if ((rs232->fd == -1) || (rs232->active == false))
        return;

    tcflush(rs232->fd, TCIFLUSH);
}

static int __putc(struct rs232_s *rs232, unsigned char byte)
{
    if (write(rs232->fd, &byte, 1) < 0)
        return -1;

    return 0;
}

static int __puts(struct rs232_s *rs232, const char *data,
    unsigned int data_size)
{
    if (write(rs232->fd, data, data_size) < 0)
        return -1;

    return 0;
}

static int tty_put(struct rs232_s *rs232, unsigned char byte, const char *data,
    unsigned int data_size)
{
    int ret;

    if ((rs232->fd == -1) || (rs232->active == false)) {
        errno_set(RS232_TTY_NOT_READY);
        return -1;
    }

    tcflush(rs232->fd, TCOFLUSH);

    if (data != NULL)
        ret = __puts(rs232, data, data_size);
    else
        ret = __putc(rs232, byte);

    if (ret < 0) {
        errno_set(RS232_WRITE_ERROR);
        return -1;
    }

    /* XXX: Because of this, this function is not thread-safe */
    tcdrain(rs232->fd);

    return 0;
}

static int tty_getc(struct rs232_s *rs232, bool block)
{
    struct timeval tv;
    fd_set fds;
    int ret = -1, c = -1;

    if ((rs232->fd == -1) || (rs232->active == false)) {
        errno_set(RS232_TTY_NOT_READY);
        return -1;
    }

    /* Did we have any byte to return? */
    if (rs232->bytes_read > 0) {
        rs232->bytes_read--;
        rs232->returned_byte++;
        return rs232->read_buffer[rs232->returned_byte - 1];
    }

    /* Are we going to wait the data? */
    if (block == false) {
        FD_ZERO(&fds);
        FD_SET(rs232->fd, &fds);

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        ret = select(rs232->fd + 1, &fds, NULL, NULL, &tv);
    } else
        ret = 1;

    if (ret == 1) {
        rs232->returned_byte = 0;
        rs232->bytes_read = read(rs232->fd, rs232->read_buffer,
                                 sizeof(rs232->read_buffer));

        if (rs232->bytes_read == 0)
            errno_set(RS232_READ_ERROR);
        else
            return tty_getc(rs232, block);
    } else
        errno_set(RS232_READ_TIMEOUT);

    return c;
}

/*
 *
 * API
 *
 */

__PUB_API__ rs232_t *rs232_ref(rs232_t *rs232)
{
    struct rs232_s *s = (struct rs232_s *)rs232;

    errno_clear();

    if (NULL == rs232) {
        errno_set(RS232_NULL_ARG);
        return NULL;
    }

    cref_inc(&s->ref);

    return rs232;
}

__PUB_API__ int rs232_unref(rs232_t *rs232)
{
    struct rs232_s *s = (struct rs232_s *)rs232;

    errno_clear();

    if (NULL == rs232) {
        errno_set(RS232_NULL_ARG);
        return -1;
    }

    cref_dec(&s->ref);

    return 0;
}

__PUB_API__ rs232_t *rs232_open(const char *device, enum rs232_speed speed)
{
    struct rs232_s *s = NULL;

    errno_clear();

    if (NULL == device) {
        errno_set(RS232_NULL_ARG);
        return NULL;
    }

    if (is_supported_speed(speed) == false) {
        errno_set(RS232_UNSUPPORTED_SPEED);
        return NULL;
    }

    s = new_rs232_s();

    if (NULL == s)
        return NULL;

    if (tty_open(s, device, speed) < 0)
        goto end_block;

    return s;

end_block:
    rs232_unref(s);
    return NULL;
}

__PUB_API__ int rs232_close(rs232_t *rs232)
{
    return rs232_unref(rs232);
}

__PUB_API__ int rs232_getc(rs232_t *rs232, bool block)
{
    struct rs232_s *s = rs232_ref(rs232);
    int ret;

    errno_clear();

    if (NULL == rs232) {
        errno_set(RS232_NULL_ARG);
        return -1;
    }

    ret = tty_getc(s, block);
    rs232_unref(s);

    return ret;
}

__PUB_API__ int rs232_putc(rs232_t *rs232, unsigned char byte)
{
    struct rs232_s *s = rs232_ref(rs232);
    int ret;

    errno_clear();

    if (NULL == rs232) {
        errno_set(RS232_NULL_ARG);
        return -1;
    }

    ret = tty_put(s, byte, NULL, 0);
    rs232_unref(s);

    return ret;
}

__PUB_API__ int rs232_puts(rs232_t *rs232, const char *data,
    unsigned int data_size)
{
    struct rs232_s *s = rs232_ref(rs232);
    int ret;

    errno_clear();

    if (NULL == rs232) {
        errno_set(RS232_NULL_ARG);
        return -1;
    }

    ret = tty_put(s, 0, data, data_size);
    rs232_unref(s);

    return ret;
}

__PUB_API__ int rs232_flush(rs232_t *rs232)
{
    struct rs232_s *s = rs232_ref(rs232);

    errno_clear();

    if (NULL == rs232) {
        errno_set(RS232_NULL_ARG);
        return -1;
    }

    tty_flush(s);
    rs232_unref(s);

    return 0;
}

