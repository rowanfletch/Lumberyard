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
#pragma once

#include <LyShine/Bus/UiLayoutBus.h>
#include <LyShine/Bus/UiLayoutRowBus.h>
#include <LyShine/Bus/UiLayoutCellDefaultBus.h>
#include <LyShine/UiComponentTypes.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <UiLayoutHelpers.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//! This component overrides the transforms of immediate children to organize them into a vertical column
class UiLayoutRowComponent
    : public AZ::Component
    , public UiLayoutBus::Handler
    , public UiLayoutRowBus::Handler
    , public UiLayoutCellDefaultBus::Handler
    , public UiTransformChangeNotificationBus::Handler
{
public: // member functions

    AZ_COMPONENT(UiLayoutRowComponent, LyShine::UiLayoutRowComponentUuid, AZ::Component);

    UiLayoutRowComponent();
    ~UiLayoutRowComponent() override;

    // UiLayoutInterface
    virtual void ApplyLayoutWidth() override;
    virtual void ApplyLayoutHeight() override;
    virtual bool IsUsingLayoutCellsToCalculateLayout() override;
    virtual bool GetIgnoreDefaultLayoutCells() override;
    virtual void SetIgnoreDefaultLayoutCells(bool ignoreDefaultLayoutCells) override;
    virtual IDraw2d::HAlign GetHorizontalChildAlignment() override;
    virtual void SetHorizontalChildAlignment(IDraw2d::HAlign alignment) override;
    virtual IDraw2d::VAlign GetVerticalChildAlignment() override;
    virtual void SetVerticalChildAlignment(IDraw2d::VAlign alignment) override;
    virtual bool IsControllingChild(AZ::EntityId childId) override;
    virtual AZ::Vector2 GetSizeToFitChildElements(const AZ::Vector2& childElementSize, int numChildElements) override;
    // ~UiLayoutInterface

    // UiLayoutRowInterface
    virtual UiLayoutInterface::Padding GetPadding() override;
    virtual void SetPadding(UiLayoutInterface::Padding padding) override;
    virtual float GetSpacing() override;
    virtual void SetSpacing(float spacing) override;
    virtual UiLayoutInterface::HorizontalOrder GetOrder() override;
    virtual void SetOrder(UiLayoutInterface::HorizontalOrder order) override;
    // ~UiLayoutRowInterface

    // UiLayoutCellDefaultInterface
    float GetMinWidth() override;
    float GetMinHeight() override;
    float GetTargetWidth() override;
    float GetTargetHeight() override;
    float GetExtraWidthRatio() override;
    float GetExtraHeightRatio() override;
    // ~UiLayoutCellDefaultInterface

    // UiTransformChangeNotificationBus
    void OnCanvasSpaceRectChanged(AZ::EntityId entityId, const UiTransformInterface::Rect& oldRect, const UiTransformInterface::Rect& newRect) override;
    // ~UiTransformChangeNotificationBus

public:  // static member functions

    static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("UiLayoutService", 0xac613bbc));
    }

    static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("UiLayoutService", 0xac613bbc));
    }

    static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC("UiElementService", 0x3dca7ad4));
        required.push_back(AZ_CRC("UiTransformService", 0x3a838e34));
    }

    static void Reflect(AZ::ReflectContext* context);

protected: // member functions

    // AZ::Component
    void Activate() override;
    void Deactivate() override;
    // ~AZ::Component

    AZ_DISABLE_COPY_MOVE(UiLayoutRowComponent);

    //! Called on a property change that has caused this element's layout to be invalid
    void InvalidateLayout();

    //! Called when a property that is used to calculate default layout cell values has changed
    void InvalidateParentLayout();

    //! Helper functions to set the child elements' transform properties.
    //! Element widths are calculated first since layout cell height
    //! properties can depend on element widths
    void ApplyLayoutWidth(float availableWidth);
    void ApplyLayoutHeight(float availableHeight);

protected: // data

    //! the padding (in pixels) inside the edges of this element
    UiLayoutInterface::Padding m_padding;

    //! the spacing (in pixels) between child elements
    float m_spacing;

    //! the order that the child elements are placed in
    UiLayoutInterface::HorizontalOrder m_order;

    //! Child alignment
    IDraw2d::HAlign m_childHAlignment;
    IDraw2d::VAlign m_childVAlignment;

    //! Whether the layout is to ignore the children's default layout cell values
    bool m_ignoreDefaultLayoutCells;

private: // static member functions

    static bool VersionConverter(AZ::SerializeContext& context,
        AZ::SerializeContext::DataElementNode& classElement);
};
