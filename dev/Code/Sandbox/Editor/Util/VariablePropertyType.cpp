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
#include "StdAfx.h"
#include "VariablePropertyType.h"

#include "Variable.h"
#include "UIEnumsDatabase.h"
#include "IEditor.h"

namespace Prop
{
    struct
    {
        int dataType;
        const char* name;
        PropertyType type;
        int image;
    } static s_propertyTypeNames[] =
    {
        { IVariable::DT_SIMPLE, "Bool", ePropertyBool, 2 },
        { IVariable::DT_SIMPLE, "Int", ePropertyInt, 0 },
        { IVariable::DT_SIMPLE, "Float", ePropertyFloat, 0 },
        { IVariable::DT_SIMPLE, "Vector", ePropertyVector2, 10 },
        { IVariable::DT_SIMPLE, "Vector", ePropertyVector, 10 },
        { IVariable::DT_SIMPLE, "Vector", ePropertyVector4, 10 },
        { IVariable::DT_SIMPLE, "String", ePropertyString, 3 },
        { IVariable::DT_PERCENT, "Float", ePropertyInt, 13 },
        { IVariable::DT_BOOLEAN, "Boolean", ePropertyBool, 2 },
        { IVariable::DT_COLOR, "Color", ePropertyColor, 1 },
        { IVariable::DT_COLORA, "ColorA", ePropertyColor, 1 },
        { IVariable::DT_CURVE | IVariable::DT_PERCENT, "FloatCurve", ePropertyFloatCurve, 13 },
        { IVariable::DT_CURVE | IVariable::DT_COLOR, "ColorCurve", ePropertyColorCurve, 1 },
        { IVariable::DT_ANGLE, "Angle", ePropertyAngle, 0 },
        { IVariable::DT_FILE, "File", ePropertyFile, 7 },
        { IVariable::DT_TEXTURE, "Texture", ePropertyTexture, 4 },
        { IVariable::DT_ANIMATION, "Animation", ePropertyAnimation, -1 },
        { IVariable::DT_OBJECT, "Model", ePropertyModel, 5 },
        { IVariable::DT_SIMPLE, "Selection", ePropertySelection, -1 },
        { IVariable::DT_SIMPLE, "List", ePropertyList, -1 },
        { IVariable::DT_SHADER, "Shader", ePropertyShader, 9 },
        { IVariable::DT_MATERIAL, "Material", ePropertyMaterial, 14 },
        { IVariable::DT_AI_BEHAVIOR, "AIBehavior", ePropertyAiBehavior, 8 },
        { IVariable::DT_AI_ANCHOR, "AIAnchor", ePropertyAiAnchor, 8 },
#ifdef USE_DEPRECATED_AI_CHARACTER_SYSTEM
        { IVariable::DT_AI_CHARACTER, "AICharacter", ePropertyAiCharacter, 8 },
#endif // USE_DEPRECATED_AI_CHARACTER_SYSTEM
        { IVariable::DT_AI_PFPROPERTIESLIST, "AgentTypeList", ePropertyAiPFPropertiesList, 8 },
        { IVariable::DT_AIENTITYCLASSES, "AI Entity Classes", ePropertyAiEntityClasses, 8 },
        { IVariable::DT_EQUIP, "Equip", ePropertyEquip, 11 },
        { IVariable::DT_REVERBPRESET, "ReverbPreset", ePropertyReverbPreset, 11 },
        { IVariable::DT_LOCAL_STRING, "LocalString", ePropertyLocalString, 3 },
        { IVariable::DT_SOCLASS, "Smart Object Class", ePropertySOClass, 8 },
        { IVariable::DT_SOCLASSES, "Smart Object Classes", ePropertySOClasses, 8 },
        { IVariable::DT_SOSTATE, "Smart Object State", ePropertySOState, 8 },
        { IVariable::DT_SOSTATES, "Smart Object States", ePropertySOStates, 8 },
        { IVariable::DT_SOSTATEPATTERN, "Smart Object State Pattern", ePropertySOStatePattern, 8 },
        { IVariable::DT_SOACTION, "AI Action", ePropertySOAction, 8 },
        { IVariable::DT_SOHELPER, "Smart Object Helper", ePropertySOHelper, 8 },
        { IVariable::DT_SONAVHELPER, "Smart Object Navigation Helper", ePropertySONavHelper, 8 },
        { IVariable::DT_SOANIMHELPER, "Smart Object Animation Helper", ePropertySOAnimHelper, 8 },
        { IVariable::DT_SOEVENT, "Smart Object Event", ePropertySOEvent, 8 },
        { IVariable::DT_SOTEMPLATE, "Smart Object Template", ePropertySOTemplate, 8 },
        { IVariable::DT_CUSTOMACTION, "Custom Action", ePropertyCustomAction, 7 },
        { IVariable::DT_VEEDHELPER, "Vehicle Helper", ePropertySelection, -1 },
        { IVariable::DT_VEEDPART, "Vehicle Part", ePropertySelection, -1 },
        { IVariable::DT_VEEDCOMP, "Vehicle Component", ePropertySelection, -1 },
        { IVariable::DT_GAMETOKEN, "Game Token", ePropertyGameToken, -1 },
        { IVariable::DT_SEQUENCE, "Sequence", ePropertySequence, -1 },
        { IVariable::DT_MISSIONOBJ, "Mission Objective", ePropertyMissionObj, -1 },
        { IVariable::DT_USERITEMCB, "User", ePropertyUser, -1 },
        { IVariable::DT_AITERRITORY, "AITerritory", ePropertyAiTerritory, 8 },
        { IVariable::DT_AIWAVE, "AIWave", ePropertyAiWave, 8 },
        { IVariable::DT_SEQUENCE_ID, "SequenceId", ePropertySequenceId, -1 },
        { IVariable::DT_LIGHT_ANIMATION, "LightAnimation", ePropertyLightAnimation, -1 },
        { IVariable::DT_FLARE, "Flare", ePropertyFlare, 7 },
        { IVariable::DT_PARTICLE_EFFECT, "ParticleEffect", ePropertyParticleName, 3 },
        { IVariable::DT_GEOM_CACHE, "Geometry Cache", ePropertyGeomCache, 5 },
        { IVariable::DT_AUDIO_TRIGGER, "Audio Trigger", ePropertyAudioTrigger, 6 },
        { IVariable::DT_AUDIO_SWITCH, "Audio Switch", ePropertyAudioSwitch, 6 },
        { IVariable::DT_AUDIO_SWITCH_STATE, "Audio Switch", ePropertyAudioSwitchState, 6 },
        { IVariable::DT_AUDIO_RTPC, "Audio Realtime Parameter Control", ePropertyAudioRTPC, 6 },
        { IVariable::DT_AUDIO_ENVIRONMENT, "Audio Environment", ePropertyAudioEnvironment, 6 },
        { IVariable::DT_AUDIO_PRELOAD_REQUEST, "Audio Preload Request", ePropertyAudioPreloadRequest, 6 },
        { IVariable::DT_SIMPLE, "Custom", ePropertyFlowCustomData, -1 },
        { IVariable::DT_UI_ELEMENT, "UiElement", ePropertyUiElement, -1 }
    };

    static const int NumPropertyTypes = sizeof(s_propertyTypeNames) / sizeof(s_propertyTypeNames[0]);

    Description::Description()
        : m_type(ePropertyInvalid)
        , m_numImages(-1)
        , m_enumList(NULL)
        , m_rangeMin(0)
        , m_rangeMax(100)
        , m_step(0)
        , m_bHardMin(false)
        , m_bHardMax(false)
        , m_valueMultiplier(1)
        , m_pEnumDBItem(NULL)
    {
    }

    Description::Description(IVariable* pVar)
        : m_type(ePropertyInvalid)
        , m_numImages(-1)
        , m_enumList(NULL)
        , m_rangeMin(0)
        , m_rangeMax(100)
        , m_step(0)
        , m_bHardMin(false)
        , m_bHardMax(false)
        , m_valueMultiplier(1)
        , m_pEnumDBItem(NULL)
    {
        if (!pVar)
        {
            return;
        }

        const int type = (int)pVar->GetDataType();

        if (type != IVariable::DT_SIMPLE)
        {
            m_type = GetType(type);
            m_numImages = GetNumImages(pVar);
        }

        m_name = pVar->GetHumanName();
        m_enumList = pVar->GetEnumList();

        if (m_enumList != NULL)
        {
            m_type = ePropertySelection;
        }

        if (m_type == ePropertyInvalid)
        {
            switch (pVar->GetType())
            {
            case IVariable::INT:
                m_type = ePropertyInt;
                break;
            case IVariable::BOOL:
                m_type = ePropertyBool;
                break;
            case IVariable::FLOAT:
                m_type = ePropertyFloat;
                break;
            case IVariable::VECTOR2:
                m_type = ePropertyVector2;
                break;
            case IVariable::VECTOR4:
                m_type = ePropertyVector4;
                break;
            case IVariable::VECTOR:
                m_type = ePropertyVector;
                break;
            case IVariable::STRING:
                m_type = ePropertyString;
                break;
            case IVariable::FLOW_CUSTOM_DATA:
                m_type = ePropertyFlowCustomData;
                break;
            default:
                break;
            }

            m_numImages = Prop::GetNumImages(m_type);
        }

        // Get variable limits.
        pVar->GetLimits(m_rangeMin, m_rangeMax, m_step, m_bHardMin, m_bHardMax);

        // Check if value is percents.
        if (type == IVariable::DT_PERCENT)
        {
            // Scale all values by 100.
            m_valueMultiplier = 100;
        }
        else if (type == IVariable::DT_ANGLE)
        {
            // Scale radians to degrees.
            m_valueMultiplier = RAD2DEG(1);
            m_rangeMin = max(-360.0f, m_rangeMin);
            m_rangeMax = min(360.0f, m_rangeMax);
        }
        else if (type == IVariable::DT_UIENUM)
        {
            m_pEnumDBItem = GetIEditor()->GetUIEnumsDatabase()->FindEnum(m_name);
        }


        const bool useExplicitStep = (pVar->GetFlags() & IVariable::UI_EXPLICIT_STEP);
        if (!useExplicitStep)
        {
            // Limit step size to 1000.
            int nPrec = max(3 - int(log(m_rangeMax - m_rangeMin) / log(10.f)), 0);
            m_step = max(m_step, powf(10.f, -nPrec));
        }
    }

    const char* GetName(int dataType)
    {
        for (int i = 0; i < NumPropertyTypes; i++)
        {
            if (dataType == s_propertyTypeNames[i].type)
            {
                return s_propertyTypeNames[i].name;
            }
        }

        return "";
    }

    PropertyType GetType(int dataType)
    {
        for (int i = 0; i < NumPropertyTypes; i++)
        {
            if (dataType == s_propertyTypeNames[i].dataType)
            {
                return s_propertyTypeNames[i].type;
            }
        }

        return ePropertyInvalid;
    }

    PropertyType GetType(const IVariable* var)
    {
        assert(var);
        if (!var)
        {
            return ePropertyInvalid;
        }

        const int dataType = (int)var->GetDataType();

        for (int i = 0; i < NumPropertyTypes; i++)
        {
            if (dataType == s_propertyTypeNames[i].dataType)
            {
                return s_propertyTypeNames[i].type;
            }
        }

        return ePropertyInvalid;
    }

    PropertyType GetType(const char* type)
    {
        assert(type);
        if (!type)
        {
            return ePropertyInvalid;
        }

        for (int i = 0; i < NumPropertyTypes; i++)
        {
            if (stricmp(type, s_propertyTypeNames[i].name) == 0)
            {
                return s_propertyTypeNames[i].type;
            }
        }

        return ePropertyInvalid;
    }

    // look up image by property type
    int GetNumImages(int propertyType)
    {
        for (int i = 0; i < NumPropertyTypes; i++)
        {
            if (propertyType == s_propertyTypeNames[i].type)
            {
                return s_propertyTypeNames[i].image;
            }
        }

        return -1;
    }

    // look up image by data type
    int GetNumImages(const IVariable* var)
    {
        assert(var);
        if (!var)
        {
            return -1;
        }

        const int dataType = (int)var->GetDataType();

        for (int i = 0; i < NumPropertyTypes; i++)
        {
            if (dataType == s_propertyTypeNames[i].dataType)
            {
                return s_propertyTypeNames[i].image;
            }
        }

        return -1;
    }

    int GetNumImages(const char* type)
    {
        assert(type);
        if (!type)
        {
            return -1;
        }

        for (int i = 0; i < NumPropertyTypes; i++)
        {
            if (stricmp(type, s_propertyTypeNames[i].name) == 0)
            {
                return s_propertyTypeNames[i].image;
            }
        }

        return -1;
    }

    const char* GetPropertyTypeToResourceType(PropertyType type)
    {
        // The strings below are names used together with
        // REGISTER_RESOURCE_SELECTOR. See IResourceSelector.h.
        switch (type)
        {
        case ePropertyModel:
            return "Model";
        case ePropertyGeomCache:
            return "GeomCache";
        case ePropertyAudioTrigger:
            return "AudioTrigger";
        case ePropertyAudioSwitch:
            return "AudioSwitch";
        case ePropertyAudioSwitchState:
            return "AudioSwitchState";
        case ePropertyAudioRTPC:
            return "AudioRTPC";
        case ePropertyAudioEnvironment:
            return "AudioEnvironment";
        case ePropertyAudioPreloadRequest:
            return "AudioPreloadRequest";
        default:
            return 0;
        }
    }
}