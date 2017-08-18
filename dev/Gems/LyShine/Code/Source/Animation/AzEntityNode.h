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

#pragma once

#include <set>
#include "AnimNode.h"
//#include "SoundTrack.h"
#include "StlUtils.h"
#include <AzCore/Component/Entity.h>
#include <LyShine/Bus/UiAnimationBus.h>

// remove comment to enable the check.
//#define CHECK_FOR_TOO_MANY_ONPROPERTY_SCRIPT_CALLS

class CUiAnimAzEntityNode
    : public CUiAnimNode
    , public UiAnimNodeBus::Handler
{
    struct SScriptPropertyParamInfo;
    struct SAnimState;

public:
    AZ_CLASS_ALLOCATOR(CUiAnimAzEntityNode, AZ::SystemAllocator, 0)
    AZ_RTTI(CUiAnimAzEntityNode, "{1C6FAEE1-92E4-42ED-8EEB-3483C36A0B77}", CUiAnimNode);

    CUiAnimAzEntityNode(const int id);
    CUiAnimAzEntityNode();
    ~CUiAnimAzEntityNode();
    static void Initialize();

    void EnableEntityPhysics(bool bEnable);

    virtual EUiAnimNodeType GetType() const { return eUiAnimNodeType_AzEntity; }

    virtual void AddTrack(IUiAnimTrack* pTrack);

    //////////////////////////////////////////////////////////////////////////
    // Overrides from CUiAnimNode
    //////////////////////////////////////////////////////////////////////////

    // UiAnimNodeInterface
    virtual AZ::EntityId GetAzEntityId() override { return m_entityId; };
    virtual void SetAzEntity(AZ::Entity* entity) override { m_entityId = entity->GetId(); }
    // ~UiAnimNodeInterface


    virtual void StillUpdate();
    virtual void Animate(SUiAnimContext& ec);

    virtual void CreateDefaultTracks();

    bool SetParamValueAz(float time, const UiAnimParamData& param, float value) override;
    bool SetParamValueAz(float time, const UiAnimParamData& param, bool value) override;
    bool SetParamValueAz(float time, const UiAnimParamData& param, int value) override;
    bool SetParamValueAz(float time, const UiAnimParamData& param, unsigned int value) override;
    bool SetParamValueAz(float time, const UiAnimParamData& param, const AZ::Vector2& value) override;
    bool SetParamValueAz(float time, const UiAnimParamData& param, const AZ::Vector3& value) override;
    bool SetParamValueAz(float time, const UiAnimParamData& param, const AZ::Vector4& value) override;
    bool SetParamValueAz(float time, const UiAnimParamData& param, const AZ::Color& value) override;

    bool GetParamValueAz(float time, const UiAnimParamData& param, float& value) override;

    virtual void PrecacheStatic(float startTime) override;
    virtual void PrecacheDynamic(float time) override;

    Vec3 GetPos() { return m_pos; };
    Quat GetRotate() { return m_rotate; };
    Vec3 GetScale() { return m_scale; };

    virtual void Activate(bool bActivate);

    IUiAnimTrack* GetTrackForAzField(const UiAnimParamData& param) const override;
    IUiAnimTrack* CreateTrackForAzField(const UiAnimParamData& param) override;

    //////////////////////////////////////////////////////////////////////////
    void Serialize(XmlNodeRef& xmlNode, bool bLoading, bool bLoadEmptyTracks);
    virtual void InitPostLoad(IUiAnimSequence* pSequence, bool remapIds, LyShine::EntityIdMap* entityIdMap);
    void OnReset();
    void OnResetHard();
    void OnStart();
    void OnPause();
    void OnStop();

    //////////////////////////////////////////////////////////////////////////
    virtual unsigned int GetParamCount() const;
    virtual CUiAnimParamType GetParamType(unsigned int nIndex) const;
    virtual const char* GetParamName(const CUiAnimParamType& param) const;
    const char* GetParamNameForTrack(const CUiAnimParamType& param, const IUiAnimTrack* track) const override;

    static int GetParamCountStatic();
    static bool GetParamInfoStatic(int nIndex, SParamInfo& info);

    static void Reflect(AZ::SerializeContext* serializeContext);

protected:
    virtual bool GetParamInfoFromType(const CUiAnimParamType& paramId, SParamInfo& info) const;

    //! Given the class data definition and a track for a field within it,
    //! compute the offset for the field and set it in the track
    const AZ::SerializeContext::ClassElement* ComputeOffsetFromElementName(
        const AZ::SerializeContext::ClassData* classData,
        IUiAnimTrack* pTrack,
        size_t baseOffset);

    //! This is called on load to compute the offset into the component for each track
    void ComputeOffsetsFromElementNames();

    void ReleaseSounds();

    // functions involved in the process to parse and store lua animated properties
    virtual void UpdateDynamicParams();
    virtual void UpdateDynamicParams_Editor();
    virtual void UpdateDynamicParams_PureGame();

    void FindDynamicPropertiesRec(IScriptTable* pScriptTable, const string& currentPath, unsigned int depth);
    void AddPropertyToParamInfoMap(const char* pKey, const string& currentPath, SScriptPropertyParamInfo& paramInfo);
    bool ObtainPropertyTypeInfo(const char* pKey, IScriptTable* pScriptTable, SScriptPropertyParamInfo& paramInfo);
    void FindScriptTableForParameterRec(IScriptTable* pScriptTable, const string& path, string& propertyName, SmartScriptTable& propertyScriptTable);

    enum EUpdateEntityFlags
    {
        eUpdateEntity_Position = 1 << 0,
        eUpdateEntity_Rotation = 1 << 1,
        eUpdateEntity_Animation = 1 << 2,
    };

protected:
    Vec3 m_pos;
    Quat m_rotate;
    Vec3 m_scale;

private:
    IUiAnimTrack* CreateVectorTrack(const UiAnimParamData& param, EUiAnimValue valueType, int numElements);

    //! Reference to AZ entity
    AZ::EntityId m_entityId;

    //! Pointer to target animation node.
    AZStd::intrusive_ptr<IUiAnimNode> m_target;

    // Cached parameters of node at given time.
    float m_time;
    Vec3 m_velocity;
    Vec3 m_angVelocity;

    //! Last animated key in Entity track.
    int m_lastEntityKey;

    bool m_bWasTransRot;
    bool m_visible;
    bool m_bInitialPhysicsStatus;

    // Pos/rot noise parameters
    struct Noise
    {
        float m_amp;
        float m_freq;

        Vec3 Get(float time) const;

        Noise()
            : m_amp(0.0f)
            , m_freq(0.0f) {}
    };

    Noise m_posNoise;
    Noise m_rotNoise;

    struct SScriptPropertyParamInfo
    {
        string variableName;
        string displayName;
        SmartScriptTable scriptTable;
        bool isVectorTable;
        SParamInfo animNodeParamInfo;
    };

    std::vector< SScriptPropertyParamInfo > m_entityScriptPropertiesParamInfos;
    typedef AZStd::unordered_map< string, size_t, stl::hash_string_caseless<string>, stl::equality_string_caseless<string> > TScriptPropertyParamInfoMap;
    TScriptPropertyParamInfoMap m_nameToScriptPropertyParamInfo;
    #ifdef CHECK_FOR_TOO_MANY_ONPROPERTY_SCRIPT_CALLS
    uint32 m_OnPropertyCalls;
    #endif
};
