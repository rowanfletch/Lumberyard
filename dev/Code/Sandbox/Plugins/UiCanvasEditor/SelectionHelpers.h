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

namespace SelectionHelpers
{
    //-------------------------------------------------------------------------------

    void UnmarkAllAndAllTheirChildren(QTreeWidgetItem* baseRootItem);

    void MarkOnlyDirectChildrenOf(const QTreeWidgetItemRawPtrQList& parentItems);

    bool A_IsParentOf_B(QTreeWidgetItem* A,
        QTreeWidgetItem* B);

    bool IsMarkedOrParentIsMarked(HierarchyItem* item);

    void FindUnmarked(HierarchyItemRawPtrList& results,
        const QTreeWidgetItemRawPtrQList& parentItems);

    //-------------------------------------------------------------------------------

    AZ::Entity* GetTopLevelParentOfElement(const LyShine::EntityArray& elements, AZ::Entity* elementToFind);

    void RemoveEntityFromArray(LyShine::EntityArray& listToTrim, const AZ::Entity* entityToRemove);

    //-------------------------------------------------------------------------------

    void GetListOfTopLevelSelectedItems(const HierarchyWidget* widget,
        const QTreeWidgetItemRawPtrQList& selectedItems,
        QTreeWidgetItemRawPtrQList& results);

    void GetListOfTopLevelSelectedItems(const HierarchyWidget* widget,
        const QTreeWidgetItemRawPtrQList& selectedItems,
        QTreeWidgetItem* invisibleRootItem,
        HierarchyItemRawPtrList& results);

    //-------------------------------------------------------------------------------

    HierarchyItemRawPtrList GetSelectedHierarchyItems(const HierarchyWidget* widget,
        const QTreeWidgetItemRawPtrQList& selectedItems);
    LyShine::EntityArray GetSelectedElements(const HierarchyWidget* widget,
        const QTreeWidgetItemRawPtrQList& selectedItems);
    EntityHelpers::EntityIdList GetSelectedElementIds(const HierarchyWidget* widget,
        const QTreeWidgetItemRawPtrQList& selectedItems,
        bool addInvalidIdIfEmpty);
    LyShine::EntityArray GetTopLevelSelectedElements(const HierarchyWidget* widget,
        const QTreeWidgetItemRawPtrQList& selectedItems);
    LyShine::EntityArray GetTopLevelSelectedElementsNotControlledByParent(const HierarchyWidget* widget,
        const QTreeWidgetItemRawPtrQList& selectedItems);

    //-------------------------------------------------------------------------------
}   // namespace SelectionHelpers
