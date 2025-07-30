/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace gralloc_optimizer {

class GrallocOptimizer {
public:
    static GrallocOptimizer* getInstance();
    
    void optimizeForTransitions();
    
    void* getCachedAllocInterface();
    
    void enableMtkOptimizations();
    
private:
    GrallocOptimizer() = default;
    ~GrallocOptimizer() = default;
    
    static GrallocOptimizer* instance_;
    void* cached_alloc_interface_;
    bool mtk_optimizations_enabled_;
    
    GrallocOptimizer(const GrallocOptimizer&) = delete;
    GrallocOptimizer& operator=(const GrallocOptimizer&) = delete;
};

} 