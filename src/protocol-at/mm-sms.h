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
 * Author: Aleksander Morgado <aleksander@lanedo.com>
 *
 * Copyright (C) 2012 Google, Inc.
 */

#ifndef MM_SMS_H
#define MM_SMS_H

#include <glib.h>
#include <glib-object.h>

#define _LIBMM_INSIDE_MM
#include <libmm-glib.h>

#include "mm-base-sms.h"
#include "mm-base-modem.h"

#define MM_TYPE_SMS            (mm_sms_get_type ())
#define MM_SMS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), MM_TYPE_SMS, MMSms))
#define MM_SMS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  MM_TYPE_SMS, MMSmsClass))
#define MM_IS_SMS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MM_TYPE_SMS))
#define MM_IS_SMS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  MM_TYPE_SMS))
#define MM_SMS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  MM_TYPE_SMS, MMSmsClass))

typedef struct _MMSms MMSms;
typedef struct _MMSmsClass MMSmsClass;

struct _MMSms {
    MMBaseSms parent;
};

struct _MMSmsClass {
    MMBaseSmsClass parent;
};

GType mm_sms_get_type (void);

MMBaseSms *mm_sms_new (MMBaseModem *modem);

#endif /* MM_SMS_H */
