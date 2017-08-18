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
#include "UiDynamicLayoutComponent.h"

#include "UiElementComponent.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Component/ComponentApplicationBus.h>

#include <LyShine/Bus/UiCanvasBus.h>
#include <LyShine/Bus/UiLayoutBus.h>
#include <LyShine/Bus/UiTransform2dBus.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC MEMBER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
UiDynamicLayoutComponent::UiDynamicLayoutComponent()
    : m_numChildElementsToClone(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
UiDynamicLayoutComponent::~UiDynamicLayoutComponent()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiDynamicLayoutComponent::SetNumChildElements(int numChildren)
{
    AZ::Entity* prototypeEntity = nullptr;
    EBUS_EVENT_RESULT(prototypeEntity, AZ::ComponentApplicationBus, FindEntity, m_prototypeElement);
    if (prototypeEntity)
    {
        int curNumChildren = 0;
        EBUS_EVENT_ID_RESULT(curNumChildren, GetEntityId(), UiElementBus, GetNumChildElements);

        if (curNumChildren != numChildren)
        {
            if (curNumChildren < numChildren)
            {
                AZ::EntityId canvasEntityId;
                EBUS_EVENT_ID_RESULT(canvasEntityId, GetEntityId(), UiElementBus, GetCanvasEntityId);

                for (int i = 0; i < numChildren - curNumChildren; i++)
                {
                    // Clone the prototype element and add it as a child
                    AZ::Entity* clonedElement = nullptr;
                    EBUS_EVENT_ID_RESULT(clonedElement, canvasEntityId, UiCanvasBus, CloneElement, prototypeEntity, GetEntity());
                }
            }
            else // curNumChildren > numChildren
            {
                UiElementComponent* elementComponent = GetEntity()->FindComponent<UiElementComponent>();
                AZ_Assert(elementComponent, "entity has no UiElementComponent");

                for (int i = curNumChildren - 1; i >= numChildren; i--)
                {
                    // Remove the cloned child element
                    AZ::Entity* element = nullptr;
                    EBUS_EVENT_ID_RESULT(element, GetEntityId(), UiElementBus, GetChildElement, i);
                    if (element)
                    {
                        elementComponent->RemoveChild(element);
                        EBUS_EVENT_ID(element->GetId(), UiElementBus, DestroyElement);
                    }
                }
            }

            // Resize the element
            ResizeToFitChildElements();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiDynamicLayoutComponent::InGamePostActivate()
{
    // Find the prototype element
    int numChildren = 0;
    EBUS_EVENT_ID_RESULT(numChildren, GetEntityId(), UiElementBus, GetNumChildElements);

    if (numChildren > 0)
    {
        AZ::Entity* prototypeEntity = nullptr;
        EBUS_EVENT_ID_RESULT(prototypeEntity, GetEntityId(), UiElementBus, GetChildElement, 0);

        if (prototypeEntity)
        {
            // Store the prototype element for future cloning
            m_prototypeElement = prototypeEntity->GetId();

            // Store the size of the prototype element for future layout element size calculations
            EBUS_EVENT_ID_RESULT(m_prototypeElementSize, m_prototypeElement, UiTransformBus, GetCanvasSpaceSizeNoScaleRotate);

            UiElementComponent* elementComponent = GetEntity()->FindComponent<UiElementComponent>();
            AZ_Assert(elementComponent, "entity has no UiElementComponent");

            // Remove any extra elements
            if (numChildren > 1)
            {
                for (int i = numChildren - 1; i > 0; i--)
                {
                    // Remove the child element
                    AZ::Entity* element = nullptr;
                    EBUS_EVENT_ID_RESULT(element, GetEntityId(), UiElementBus, GetChildElement, i);
                    if (element)
                    {
                        elementComponent->RemoveChild(element);
                        EBUS_EVENT_ID(element->GetId(), UiElementBus, DestroyElement);
                    }
                }
            }

            // Remove the prototype element
            elementComponent->RemoveChild(prototypeEntity);

            // Listen for canvas space rect changes
            UiTransformChangeNotificationBus::Handler::BusConnect(GetEntityId());
        }
    }

    // Initialize the number of child elements
    SetNumChildElements(m_numChildElementsToClone);

    if (m_numChildElementsToClone == 0)
    {
        // Resize the element
        ResizeToFitChildElements();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiDynamicLayoutComponent::OnCanvasSpaceRectChanged(AZ::EntityId entityId, const UiTransformInterface::Rect& oldRect, const UiTransformInterface::Rect& newRect)
{
    // If old rect equals new rect, size changed due to initialization
    bool sizeChanged = (oldRect == newRect) || (!oldRect.GetSize().IsClose(newRect.GetSize(), 0.05f));

    if (sizeChanged)
    {
        // Resize the element
        ResizeToFitChildElements();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiDynamicLayoutComponent::OnUiElementBeingDestroyed()
{
    if (m_prototypeElement.IsValid())
    {
        EBUS_EVENT_ID(m_prototypeElement, UiElementBus, DestroyElement);
        m_prototypeElement.SetInvalid();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC STATIC MEMBER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////

void UiDynamicLayoutComponent::Reflect(AZ::ReflectContext* context)
{
    AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);

    if (serializeContext)
    {
        serializeContext->Class<UiDynamicLayoutComponent, AZ::Component>()
            ->Version(1)
            ->Field("NumChildElements", &UiDynamicLayoutComponent::m_numChildElementsToClone);

        AZ::EditContext* ec = serializeContext->GetEditContext();
        if (ec)
        {
            auto editInfo = ec->Class<UiDynamicLayoutComponent>("DynamicLayout",
                    "A component that clones the prototype element and resizes the layout. The first child element acts as the prototype element.");

            editInfo->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                ->Attribute(AZ::Edit::Attributes::Icon, "Editor/Icons/Components/UiDynamicLayout.png")
                ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Editor/Icons/Components/Viewport/UiDynamicLayout.png")
                ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("UI", 0x27ff46b0))
                ->Attribute(AZ::Edit::Attributes::AutoExpand, true);

            editInfo->DataElement(AZ::Edit::UIHandlers::SpinBox, &UiDynamicLayoutComponent::m_numChildElementsToClone, "Num Cloned Elements",
                "The number of child elements to initialize the layout with.")
                ->Attribute(AZ::Edit::Attributes::Min, 0);
        }
    }

    AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context);
    if (behaviorContext)
    {
        behaviorContext->EBus<UiDynamicLayoutBus>("UiDynamicLayoutBus")
            ->Event("SetNumChildElements", &UiDynamicLayoutBus::Events::SetNumChildElements);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PROTECTED MEMBER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiDynamicLayoutComponent::Activate()
{
    UiDynamicLayoutBus::Handler::BusConnect(GetEntityId());
    UiInitializationBus::Handler::BusConnect(GetEntityId());
    UiElementNotificationBus::Handler::BusConnect(GetEntityId());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiDynamicLayoutComponent::Deactivate()
{
    UiDynamicLayoutBus::Handler::BusDisconnect();
    UiInitializationBus::Handler::BusDisconnect();
    if (UiTransformChangeNotificationBus::Handler::BusIsConnected())
    {
        UiTransformChangeNotificationBus::Handler::BusDisconnect();
    }
    UiElementNotificationBus::Handler::BusDisconnect();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiDynamicLayoutComponent::ResizeToFitChildElements()
{
    if (!m_prototypeElement.IsValid())
    {
        return;
    }

    int numChildren = 0;
    EBUS_EVENT_ID_RESULT(numChildren, GetEntityId(), UiElementBus, GetNumChildElements);

    AZ::Vector2 curSize(0.0f, 0.0f);
    EBUS_EVENT_ID_RESULT(curSize, GetEntityId(), UiTransformBus, GetCanvasSpaceSizeNoScaleRotate);

    AZ::Vector2 newSize(0.0f, 0.0f);
    EBUS_EVENT_ID_RESULT(newSize, GetEntityId(), UiLayoutBus, GetSizeToFitChildElements, m_prototypeElementSize, numChildren);

    if (!curSize.IsClose(newSize, 0.05f))
    {
        UiTransform2dInterface::Offsets offsets;
        EBUS_EVENT_ID_RESULT(offsets, GetEntityId(), UiTransform2dBus, GetOffsets);

        AZ::Vector2 pivot;
        EBUS_EVENT_ID_RESULT(pivot, GetEntityId(), UiTransformBus, GetPivot);

        AZ::Vector2 sizeDiff = newSize - curSize;

        bool offsetsChanged = false;
        if (sizeDiff.GetX() != 0.0f)
        {
            offsets.m_left -= sizeDiff.GetX() * pivot.GetX();
            offsets.m_right += sizeDiff.GetX() * (1.0f - pivot.GetX());
            offsetsChanged = true;
        }
        if (sizeDiff.GetY() != 0.0f)
        {
            offsets.m_top -= sizeDiff.GetY() * pivot.GetY();
            offsets.m_bottom += sizeDiff.GetY() * (1.0f - pivot.GetY());
            offsetsChanged = true;
        }

        if (offsetsChanged)
        {
            EBUS_EVENT_ID(GetEntityId(), UiTransform2dBus, SetOffsets, offsets);
        }
    }
}
