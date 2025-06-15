#
# SPDX-FileCopyrightText: 2023-2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit_only.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit from device makefile.
$(call inherit-product, device/xiaomi/rubyx/device.mk)

# Inherit some common LineageOS stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# The Pixel Project Stuff
CUSTOM_BUILD_TYPE := OFFICIAL
WITH_GMS := true
TARGET_SUPPORTS_QUICK_TAP := true
TARGET_SUPPORTS_GOOGLE_RECORDER := true
TARGET_INCLUDE_STOCK_ARCORE := true
TARGET_INCLUDE_STOCK_AICORE := true
TARGET_INCLUDE_LIVE_WALLPAPERS := true
CUSTOM_MAINTAINER := Aeron
TARGET_ENABLE_BLUR := true

PRODUCT_NAME := aosp_rubyx
PRODUCT_DEVICE := rubyx
PRODUCT_MANUFACTURER := Xiaomi
PRODUCT_BRAND := Redmi
PRODUCT_MODEL := ruby

PRODUCT_GMS_CLIENTID_BASE := android-xiaomi

PRODUCT_BUILD_PROP_OVERRIDES += \
    BuildFingerprint=Redmi/ruby_global/ruby:14/UP1A.230620.001/OS2.0.2.0.UMOMIXM:user/release-keys \
    SystemName=ruby_global \
    SystemDevice=ruby
