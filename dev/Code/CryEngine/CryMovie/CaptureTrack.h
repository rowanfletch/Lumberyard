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

#ifndef CRYINCLUDE_CRYMOVIE_CAPTURETRACK_H
#define CRYINCLUDE_CRYMOVIE_CAPTURETRACK_H

#pragma once

#include "IMovieSystem.h"
#include "AnimTrack.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/** A track for capturing a movie from the engine rendering.
*/
class CCaptureTrack
    : public TAnimTrack<ICaptureKey>
{
public:
    void SerializeKey(ICaptureKey& key, XmlNodeRef& keyNode, bool bLoading);
    void GetKeyInfo(int key, const char*& description, float& duration);

    virtual void GetMemoryUsage(ICrySizer* pSizer) const
    {
        pSizer->AddObject(this, sizeof(*this));
    }
};

#endif // CRYINCLUDE_CRYMOVIE_CAPTURETRACK_H
