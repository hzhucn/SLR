//
//  basic_EDFs.h
//
//  Created by 渡部 心 on 2015/09/05.
//  Copyright (c) 2015年 渡部 心. All rights reserved.
//

#ifndef __SLR__basic_EDFs__
#define __SLR__basic_EDFs__

#include "../defines.h"
#include "../references.h"
#include "../Core/directional_distribution_functions.h"

class DiffuseEDF : public EDF {
public:
    DiffuseEDF() : EDF(DirectionType::Reflection | DirectionType::LowFreq) { };
    
    Spectrum sample(const EDFQuery &query, const EDFSample &smp, EDFQueryResult* result) const override;
    Spectrum evaluate(const EDFQuery &query, const Vector3D &dir) const override;
    float evaluatePDF(const EDFQuery &query, const Vector3D &dir) const override;
    float weight(const EDFQuery &query) const { return 1.0f; };
};

#endif /* defined(__SLR__basic_EDFs__) */