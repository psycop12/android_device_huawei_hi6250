/*
 * Meticulus hi6250 libsensors
 * Copyright (c) 2017 Jonathan Dennis [Meticulus]
 *                               theonejohnnyd@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#define LOG_TAG "libshim"
#include <libexif/exif-entry.h>


extern "C" {

/* libaudio_route */
extern int pop_seq_init(){return 0;};
extern int pop_seq_set(){return 0;};
extern int force_flush_set() { return 0;};

/* libexif */
extern void exif_entry_gps_initialize(ExifEntry * e, ExifTag tag)
{
    exif_entry_initialize(e,tag);
}


}
