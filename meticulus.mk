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

# Use my custom build ninja that prints to screen sequentially
# so you can see whats going on.
# Resolve depenancy issue: sudo apt-get install libc++-dev
ifeq ($(USER), meticulus)
$(shell echo "Using Meticulus's Ninja" >&2)
$(shell cp -f $(LOCAL_PATH)/prebuilt/ninja prebuilts/build-tools/linux-x86/bin/ninja)

PRODUCT_PACKAGES += \
    CodinalteParts
else
$(shell cd prebuilts/build-tools && git checkout linux-x86/bin/ninja)
endif
