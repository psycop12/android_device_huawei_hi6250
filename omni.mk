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

# Inherit cm
$(call inherit-product, device/huawei/hi6250/cm.mk)

# Inherit omni APNs list
$(call inherit-product, vendor/omni/config/gsm.mk)

# Inherit omni custom product configuration
$(call inherit-product, vendor/omni/config/common.mk)

PRODUCT_NAME := omni_hi6250

PRODUCT_PACKAGES += \
    messaging \
    charger_res_images

ifeq ($(AUTOPATCH),true)
    $(shell cd device/huawei/hi6250/patches 2>&1 > /dev/null && ./patch.sh omni >&2)
else
    $(shell echo ---------------------------------------------------------------- >&2)
    $(shell echo AUTOPATCH not enabled... >&2)
    $(shell echo run manually: \"cd device/huawei/hi6250/patches \&\& ./patch.sh omni\">&2)
    $(shell echo OR: >&2)
    $(shell echo enable autopatch: \"export AUTOPATCH=true \&\& lunch $(PRODUCT_NAME)-userdebug\">&2)
    $(shell echo ---------------------------------------------------------------- >&2)
endif

$(call inherit-product, device/huawei/hi6250/twrp.mk)

