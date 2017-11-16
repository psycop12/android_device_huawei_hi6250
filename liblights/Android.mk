#
# Meticulus hi6250 liblights
# Copyright (c) 2017 Jonathan Dennis [Meticulus]
#                               theonejohnnyd@gmail.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

LOCAL_PATH:= $(call my-dir)

# HAL module implemenation stored in
# hw/<POWERS_HARDWARE_MODULE_ID>.<ro.hardware>.so
include $(CLEAR_VARS)

LOCAL_SRC_FILES := lights.c stock_lights_interface.c
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES := liblog libcutils
LOCAL_MODULE := lights.hi6250
LOCAL_CFLAGS := -DLOG_TAG='"Meticulus Lights"'

ifneq ($(TARGET_PRODUCT),omni_hi6250)
ifneq ($(TARGET_PRODUCT),aosp_hi6250)
ifneq ($(TARGET_PRODUCT),pa_hi6250)
LOCAL_CFLAGS += -DLEDSMODE
endif
endif
else
# Debug for omni
LOCAL_CFLAGS += -DDEBUG
endif
include $(BUILD_SHARED_LIBRARY)
