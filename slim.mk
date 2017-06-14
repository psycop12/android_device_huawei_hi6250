#
# Copyright (C) 2016 Jonathan Jason Dennis (theonejohnnyd@gmail.com)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# AOSP overlay
DEVICE_PACKAGE_OVERLAYS += $(LOCAL_PATH)/overlay/aosp

# Inherit Slim common Phone stuff.
$(call inherit-product-if-exists, vendor/slim/config/common_full_phone.mk)

# Inherit cm
$(call inherit-product, device/huawei/hi6250/cm.mk)

PRODUCT_NAME := slim_hi6250

PRODUCT_PACKAGES += \
    messaging

ifeq ($(AUTOPATCH),true)
    $(shell cd device/huawei/hi6250/patches 2>&1 > /dev/null && ./patch.sh cm >&2)
else
    $(shell echo ---------------------------------------------------------------- >&2)
    $(shell echo AUTOPATCH not enabled... >&2)
    $(shell echo run manually: \"cd device/huawei/hi6250/patches \&\& ./patch.sh slim\">&2)
    $(shell echo OR: >&2)
    $(shell echo enable autopatch: \"export AUTOPATCH=true \&\& lunch $(PRODUCT_NAME)-userdebug\">&2)
    $(shell echo ---------------------------------------------------------------- >&2)
endif

