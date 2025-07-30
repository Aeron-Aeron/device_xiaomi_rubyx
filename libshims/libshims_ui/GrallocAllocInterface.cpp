/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "GrallocOptimizer.h"
#include <cutils/properties.h>
#include <dlfcn.h>
#include <cstring>
#include <utils/Log.h>

#undef LOG_TAG
#define LOG_TAG "GrallocAllocInterface"
#define USE_DMA_BUF_HEAPS_PROP "ro.vendor.dma_buf_heaps"
#define GRALLOC_OPTIMIZATION_PROP "persist.vendor.gralloc.optimize"


namespace {
    static void* g_gralloc_instance = nullptr;
    static void* g_gralloc_handle = nullptr;
    static void* g_dma_manager_instance = nullptr;
    static void* g_dma_legacy_manager_instance = nullptr;
    
    typedef void* (*GetInstanceFunc)();
    typedef void* (*DmaManagerGetInstanceFunc)();
    typedef void* (*DmaLegacyManagerGetInstanceFunc)();
    
    void* getGrallocInstance() {
        char property[PROPERTY_VALUE_MAX];
        if (property_get(GRALLOC_OPTIMIZATION_PROP, property, "1") > 0 && 
            strcmp(property, "0") == 0) {
            return nullptr;
        }
        
        if (g_gralloc_instance) {
            return g_gralloc_instance;
        }
        
        if (property_get(USE_DMA_BUF_HEAPS_PROP, property, NULL) > 0) {
            if (g_dma_manager_instance) {
                g_gralloc_instance = g_dma_manager_instance;
                return g_gralloc_instance;
            }
            
            if (!g_gralloc_handle) {
                g_gralloc_handle = dlopen("libgralloctypes_mtk.so", RTLD_NOW);
            }
            
            if (g_gralloc_handle) {
                DmaManagerGetInstanceFunc dmaGetInstance = 
                    (DmaManagerGetInstanceFunc)dlsym(g_gralloc_handle, "_ZN10DmaManager11GetInstanceEv");
                if (dmaGetInstance) {
                    g_dma_manager_instance = dmaGetInstance();
                    g_gralloc_instance = g_dma_manager_instance;
                    ALOGD("Using DMA manager instance: %p", g_gralloc_instance);
                    return g_gralloc_instance;
                }
            }
        } else {
            if (g_dma_legacy_manager_instance) {
                g_gralloc_instance = g_dma_legacy_manager_instance;
                return g_gralloc_instance;
            }
            
            if (!g_gralloc_handle) {
                g_gralloc_handle = dlopen("libgralloctypes_mtk.so", RTLD_NOW);
            }
            
            if (g_gralloc_handle) {
                DmaLegacyManagerGetInstanceFunc dmaLegacyGetInstance = 
                    (DmaLegacyManagerGetInstanceFunc)dlsym(g_gralloc_handle, "_ZN16DmaLegacyManager11GetInstanceEv");
                if (dmaLegacyGetInstance) {
                    g_dma_legacy_manager_instance = dmaLegacyGetInstance();
                    g_gralloc_instance = g_dma_legacy_manager_instance;
                    ALOGD("Using DMA legacy manager instance: %p", g_gralloc_instance);
                    return g_gralloc_instance;
                }
            }
        }
        
        if (!g_gralloc_handle) {
            g_gralloc_handle = dlopen("libgralloctypes_mtk.so", RTLD_NOW);
            if (!g_gralloc_handle) {
                ALOGE("Failed to load libgralloctypes_mtk.so: %s", dlerror());
                return nullptr;
            }
        }
        
        GetInstanceFunc originalGetInstance = (GetInstanceFunc)dlsym(g_gralloc_handle, "_ZN7gralloc13AllocInterface11GetInstanceEv");
        if (!originalGetInstance) {
            originalGetInstance = (GetInstanceFunc)dlsym(g_gralloc_handle, "GetInstance");
            if (!originalGetInstance) {
                ALOGE("Failed to find GetInstance symbol in gralloc library");
                return nullptr;
            }
        }
        
        g_gralloc_instance = originalGetInstance();
        ALOGD("Cached gralloc instance: %p", g_gralloc_instance);
        
        return g_gralloc_instance;
    }
}

extern "C" {
    void* _ZN7gralloc13AllocInterface11GetInstanceEv() {
        return getGrallocInstance();
    }
    
    void* GetInstance() {
        return getGrallocInstance();
    }
    
    void* _ZN3mtk7gralloc13AllocInterface11GetInstanceEv() {
        return getGrallocInstance();
    }
}