//
//  ModifiedWardDurBRDF.cpp
//
//  Created by 渡部 心 on 2015/09/05.
//  Copyright (c) 2015年 渡部 心. All rights reserved.
//

#include "ModifiedWardDurBRDF.h"

Spectrum ModifiedWardDurBRDF::sample(const BSDFQuery &query, const BSDFSample &smp, BSDFQueryResult *result) const {
    if (!matches(query.flags, result))
        return Spectrum::Zero;
    
    float quad = 2 * M_PI * smp.uDir[1];
    float phi_h = std::atan2(m_anisoY * std::sin(quad), m_anisoX * std::cos(quad));
    float cosphi_ax = std::cos(phi_h) / m_anisoX;
    float sinphi_ay = std::sin(phi_h) / m_anisoY;
    float theta_h = std::atan(std::sqrt(-std::log(1 - smp.uDir[0]) / (cosphi_ax * cosphi_ax + sinphi_ay * sinphi_ay)));
    //    if (query.dirLocal.z < 0)
    //        theta_h = M_PI - theta_h;
    Vector3D halfv = Vector3D(std::sin(theta_h) * std::cos(phi_h), std::sin(theta_h) * std::sin(phi_h), std::cos(theta_h));
    result->dir_sn = 2 * dot(query.dir_sn, halfv) * halfv - query.dir_sn;
    if (result->dir_sn.z /** query.dirLocal.z*/ <= 0) {
        result->dirPDF = 0.0f;
        return Spectrum::Zero;
    }
    
    float hx_ax = halfv.x / m_anisoX;
    float hy_ay = halfv.y / m_anisoY;
    float dotHN = std::fabs(halfv.z);
    float dotHI = dot(halfv, result->dir_sn);
    float numerator = std::exp(-(hx_ax * hx_ax + hy_ay * hy_ay) / (dotHN * dotHN));
    float commonDenom = 4 * M_PI * m_anisoX * m_anisoY * dotHI * dotHN * dotHN * dotHN;
    
    result->dirPDF = numerator / commonDenom;
    result->dirType = m_type;
    Spectrum ret = m_R * (numerator / (commonDenom * dotHI * dotHN));
    BSDF_SAMPLE_ASSERT;
    return ret;
}

Spectrum ModifiedWardDurBRDF::evaluateInternal(const BSDFQuery &query, const Vector3D &dir) const {
    if (!query.flags.matches(m_type))
        return Spectrum::Zero;
    
    Vector3D halfv = normalize(query.dir_sn + dir);
    float hx_ax = halfv.x / m_anisoX;
    float hy_ay = halfv.y / m_anisoY;
    float dotHN = std::fabs(halfv.z);
    float dotHI = dot(halfv, dir);
    float numerator = std::exp(-(hx_ax * hx_ax + hy_ay * hy_ay) / (dotHN * dotHN));
    float denominator = 4 * M_PI * m_anisoX * m_anisoY * dotHI * dotHI * dotHN * dotHN * dotHN * dotHN;
    Spectrum ret = m_R * numerator / denominator;
    BSDF_EVALUATE_ASSERT;
    return ret;
}

float ModifiedWardDurBRDF::evaluatePDF(const BSDFQuery &query, const Vector3D &dir) const {
    if (!query.flags.matches(m_type))
        return 0;
    
    Vector3D halfv = normalize(query.dir_sn + dir);
    float hx_ax = halfv.x / m_anisoX;
    float hy_ay = halfv.y / m_anisoY;
    float dotHN = std::fabs(halfv.z);
    float dotHI = dot(halfv, dir);
    float numerator = std::exp(-(hx_ax * hx_ax + hy_ay * hy_ay) / (dotHN * dotHN));
    float denominator = 4 * M_PI * m_anisoX * m_anisoY * dotHI * dotHN * dotHN * dotHN;
    float ret = numerator / denominator;
    BSDF_EVALUATE_PDF_ASSERT;
    return ret;
}

float ModifiedWardDurBRDF::weight(const BSDFQuery &query) const {
    if (!query.flags.matches(m_type))
        return 0;
    return m_R.maxValue();
}

Spectrum ModifiedWardDurBRDF::getBaseColor(DirectionType flags) const {
    if (!flags.matches(m_type))
        return Spectrum::Zero;
    return m_R;
}