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

#ifndef CRYINCLUDE_CRYMOVIE_SCRIPTVARNODE_H
#define CRYINCLUDE_CRYMOVIE_SCRIPTVARNODE_H
#pragma once

#include "AnimNode.h"

class CAnimScriptVarNode
    : public CAnimNode
{
public:
    CAnimScriptVarNode(const int id);

    virtual EAnimNodeType GetType() const { return eAnimNodeType_ScriptVar; }

    //////////////////////////////////////////////////////////////////////////
    // Overrides from CAnimNode
    //////////////////////////////////////////////////////////////////////////
    void Animate(SAnimContext& ec);
    void CreateDefaultTracks();
    void OnReset();
    void OnResume();

    //////////////////////////////////////////////////////////////////////////
    virtual unsigned int GetParamCount() const;
    virtual CAnimParamType GetParamType(unsigned int nIndex) const;
    virtual bool GetParamInfoFromType(const CAnimParamType& paramId, SParamInfo& info) const;

    void GetMemoryUsage(ICrySizer* pSizer) const
    {
        pSizer->AddObject(this, sizeof(*this));
        CAnimNode::GetMemoryUsage(pSizer);
    }
private:
    void SetScriptValue();
    float m_value;
};

#endif // CRYINCLUDE_CRYMOVIE_SCRIPTVARNODE_H
