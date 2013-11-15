/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details:
 *
 * Copyright (C) 2008 - 2009 Novell, Inc.
 * Copyright (C) 2009 - 2010 Red Hat, Inc.
 */

#ifndef MM_PORT_SERIAL_H
#define MM_PORT_SERIAL_H

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include "mm-port.h"

#define MM_TYPE_PORT_SERIAL            (mm_port_serial_get_type ())
#define MM_PORT_SERIAL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), MM_TYPE_PORT_SERIAL, MMPortSerial))
#define MM_PORT_SERIAL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  MM_TYPE_PORT_SERIAL, MMPortSerialClass))
#define MM_IS_PORT_SERIAL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MM_TYPE_PORT_SERIAL))
#define MM_IS_PORT_SERIAL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  MM_TYPE_PORT_SERIAL))
#define MM_PORT_SERIAL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  MM_TYPE_PORT_SERIAL, MMPortSerialClass))

#define MM_PORT_SERIAL_BAUD         "baud"
#define MM_PORT_SERIAL_BITS         "bits"
#define MM_PORT_SERIAL_PARITY       "parity"
#define MM_PORT_SERIAL_STOPBITS     "stopbits"
#define MM_PORT_SERIAL_SEND_DELAY   "send-delay"
#define MM_PORT_SERIAL_RTS_CTS      "rts-cts"
#define MM_PORT_SERIAL_FD           "fd" /* Construct-only */
#define MM_PORT_SERIAL_SPEW_CONTROL "spew-control" /* Construct-only */
#define MM_PORT_SERIAL_FLASH_OK     "flash-ok" /* Construct-only */

typedef struct _MMPortSerial MMPortSerial;
typedef struct _MMPortSerialClass MMPortSerialClass;

typedef void (*MMSerialReopenFn)       (MMPortSerial *port,
                                        GError *error,
                                        gpointer user_data);

typedef void (*MMSerialFlashFn)        (MMPortSerial *port,
                                        GError *error,
                                        gpointer user_data);

typedef void (*MMSerialResponseFn)     (MMPortSerial *port,
                                        GByteArray *response,
                                        GError *error,
                                        gpointer user_data);


struct _MMPortSerial {
    MMPort parent;
};

struct _MMPortSerialClass {
    MMPortClass parent;

    /* Called for subclasses to parse unsolicited responses.  If any recognized
     * unsolicited response is found, it should be removed from the 'response'
     * byte array before returning.
     */
    void     (*parse_unsolicited) (MMPortSerial *self, GByteArray *response);

    /* Called to parse the device's response to a command or determine if the
     * response was an error response.  If the response indicates an error, an
     * appropriate error should be returned in the 'error' argument.  The
     * function should return FALSE if there is not enough data yet to determine
     * the device's reply (whether success *or* error), and should return TRUE
     * when the device's response has been recognized and parsed.
     */
    gboolean (*parse_response)    (MMPortSerial *self,
                                   GByteArray *response,
                                   GError **error);

    /* Called after parsing to allow the command response to be delivered to
     * it's callback to be handled.  Returns the # of bytes of the response
     * consumed.
     */
    gsize     (*handle_response)  (MMPortSerial *self,
                                   GByteArray *response,
                                   GError *error,
                                   GCallback callback,
                                   gpointer callback_data);

    /* Called to configure the serial port fd after it's opened.  On error, should
     * return FALSE and set 'error' as appropriate.
     */
    gboolean (*config_fd)         (MMPortSerial *self, int fd, GError **error);

    /* Called to configure the serial port after it's opened. Errors, if any,
     * should get ignored. */
    void     (*config)            (MMPortSerial *self);

    void (*debug_log)             (MMPortSerial *self,
                                   const char *prefix,
                                   const char *buf,
                                   gsize len);

    /* Signals */
    void (*buffer_full)           (MMPortSerial *port, const GByteArray *buffer);
    void (*timed_out)             (MMPortSerial *port, guint n_consecutive_replies);
    void (*forced_close)          (MMPortSerial *port);
};

GType mm_port_serial_get_type (void);

MMPortSerial *mm_port_serial_new (const char *name, MMPortType ptype);

/* Keep in mind that port open/close is refcounted, so ensure that
 * open/close calls are properly balanced.
 */

gboolean mm_port_serial_is_open           (MMPortSerial *self);

gboolean mm_port_serial_open              (MMPortSerial *self,
                                           GError  **error);

void     mm_port_serial_close             (MMPortSerial *self);

gboolean mm_port_serial_reopen            (MMPortSerial *self,
                                           guint32 reopen_time,
                                           MMSerialReopenFn callback,
                                           gpointer user_data);

gboolean mm_port_serial_flash             (MMPortSerial *self,
                                           guint32 flash_time,
                                           gboolean ignore_errors,
                                           MMSerialFlashFn callback,
                                           gpointer user_data);

void     mm_port_serial_flash_cancel      (MMPortSerial *self);

gboolean mm_port_serial_get_flash_ok      (MMPortSerial *self);

void     mm_port_serial_queue_command     (MMPortSerial *self,
                                           GByteArray *command,
                                           gboolean take_command,
                                           guint32 timeout_seconds,
                                           GCancellable *cancellable,
                                           MMSerialResponseFn callback,
                                           gpointer user_data);

void     mm_port_serial_queue_command_cached (MMPortSerial *self,
                                              GByteArray *command,
                                              gboolean take_command,
                                              guint32 timeout_seconds,
                                              GCancellable *cancellable,
                                              MMSerialResponseFn callback,
                                              gpointer user_data);

#endif /* MM_PORT_SERIAL_H */