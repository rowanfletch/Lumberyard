/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#ifndef CRYINCLUDE_CRYMOVIE_MATERIALNODE_H
#define CRYINCLUDE_CRYMOVIE_MATERIALNODE_H
#pragma once

#include "AnimNode.h"

class CAnimMaterialNode
    : public CAnimNode
{
public:
    CAnimMaterialNode(const int id);
    static void Initialize();

    virtual void SetName(const char* name);

    virtual EAnimNodeType GetType() const { return eAnimNodeType_Material; }

    //////////////////////////////////////////////////////////////////////////
    // Overrides from CAnimNode
    //////////////////////////////////////////////////////////////////////////
    void Animate(SAnimContext& ec);

    //////////////////////////////////////////////////////////////////////////
    // Supported tracks description.
    //////////////////////////////////////////////////////////////////////////
    virtual unsigned int GetParamCount() const;
    virtual CAnimParamType GetParamType(unsigned int nIndex) const;
    virtual const char* GetParamName(const CAnimParamType& paramType) const;

    void GetMemoryUsage(ICrySizer* pSizer) const
    {
        pSizer->AddObject(this, sizeof(*this));
        CAnimNode::GetMemoryUsage(pSizer);
    }

    virtual void GetKeyValueRange(float& fMin, float& fMax) const { fMin = m_fMinKeyValue; fMax = m_fMaxKeyValue; };
    virtual void SetKeyValueRange(float fMin, float fMax){ m_fMinKeyValue = fMin; m_fMaxKeyValue = fMax; };

    virtual void InitializeTrack(IAnimTrack* pTrack, const CAnimParamType& paramType);
    
protected:
    virtual bool GetParamInfoFromType(const CAnimParamType& paramId, SParamInfo& info) const;

    void UpdateDynamicParamsInternal() override;
private:
    void SetScriptValue();
    void AnimateNamedParameter(SAnimContext& ec, IRenderShaderResources* pShaderResources, const char* name, IAnimTrack* pTrack);
    _smart_ptr<IMaterial> GetMaterialByName(const char* pName);

    float m_fMinKeyValue;
    float m_fMaxKeyValue;

    std::vector< CAnimNode::SParamInfo > m_dynamicShaderParamInfos;
    typedef AZStd::unordered_map< string, size_t, stl::hash_string_caseless<string>, stl::equality_string_caseless<string> > TDynamicShaderParamsMap;
    TDynamicShaderParamsMap m_nameToDynamicShaderParam;
};

#endif // CRYINCLUDE_CRYMOVIE_MATERIALNODE_H
