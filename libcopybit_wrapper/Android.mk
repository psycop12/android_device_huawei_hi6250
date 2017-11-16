#
# Meticulus hi6250 libcopybit_wrapper
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

LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)

#LOCAL_SHARED_LIBRARIES := libcopybit_wrapper liblog libcutils libc
#LOCAL_SRC_FILES := main.cpp
#LOCAL_MODULE := copybittest
#LOCAL_CFLAGS:= -DLOG_TAG=\"copybit_wrapper\"
#include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_SHARED_LIBRARIES := liblog libcutils
LOCAL_SRC_FILES := copybit.cpp copybit_interface.cpp dump.cpp
LOCAL_MODULE := libcopybit_wrapper
LOCAL_CFLAGS:= -DLOG_TAG='"Meticulus Copybit Wrapper"'

ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
LOCAL_CFLAGS += -DDEBUG
endif

# Install the symlinks.
LOCAL_POST_INSTALL_CMD := mkdir -p $(TARGET_OUT)/lib64/hw; ln -sf ../libcopybit_wrapper.so $(TARGET_OUT)/lib64/hw/copybit.hi6250.so;

include $(BUILD_SHARED_LIBRARY)
