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

// Description : Shape containers.


#ifndef CRYINCLUDE_EDITOR_AI_NAVDATAGENERATION_SHAPECONTAINER_H
#define CRYINCLUDE_EDITOR_AI_NAVDATAGENERATION_SHAPECONTAINER_H
#pragma once


#include "Shape.h"

template<typename ShapeClass>
struct SQuadTreeAIShapeElement
{
    /// no-arg constructor is just to allow std::vector::resize(0)
    SQuadTreeAIShapeElement(ShapeClass* pShape = 0)
        : m_pShape(pShape)
        , m_counter(-1) {}

    inline bool DoesAABBIntersectAABB(const AABB& aabb) const { return Overlap::AABB_AABB2D(aabb, m_pShape->GetAABB()); }
    inline bool DoesAABBIntersectPoint(const Vec3& point) const { return Overlap::Point_AABB2D(point, m_pShape->GetAABB()); }
    inline bool DoesIntersectAABB(const AABB& aabb) const { return m_pShape->OverlapAABB(aabb); }
    inline const AABB& GetAABB() const { return m_pShape->GetAABB(); }

    ShapeClass* m_pShape;
    mutable int m_counter;
};

template<typename SAIQuadTreeElement>
class CAIQuadTree;

// Simple helper class to deal with shapes.
class CAIShapeContainer
{
public:

    typedef std::map<string, CAIShape*> NameToShapeMap;
    typedef std::vector<CAIShape*> ShapeVector;
    typedef std::vector<const SQuadTreeAIShapeElement<CAIShape>*> QuadTreeAIShapeVector;

    // Constructor
    CAIShapeContainer();
    // Destructor
    ~CAIShapeContainer();
    // Clear all shapes and supporting structures.
    void Clear();
    // Replace the container with content from specified container.
    void Copy(const CAIShapeContainer& cont);
    // Insert the contents of the specified container.
    void Insert(const CAIShapeContainer& cont);
    // Insert new shape, the container will delete the shape when the container cleared.
    void InsertShape(CAIShape* pShape);
    // Deletes shape from container and removes any references to it.
    void DeleteShape(const string& name);
    // Deletes shape from container and removes any references to it.
    void DeleteShape(CAIShape* pShape);
    // Detaches specified shape from the container.
    void DetachShape(CAIShape* pShape);
    // Finds shape by name.
    CAIShape* FindShape(const string& name);
    // Builds quadtree structure to speed up spatial queries.
    void BuildQuadTree(int maxElementsPerCell, float minCellSize);
    // Builds bin sorting structures for the shapes to speed up spatial queries.
    void BuildBins();
    // Clears the quadtree
    void ClearQuadTree();
    // Returns the shape vector.
    inline const ShapeVector& GetShapes() const { return m_shapes; }
    // Returns the shape vector.
    inline ShapeVector& GetShapes() { return m_shapes; }
    // Returns all shapes that overlap the specified AABB.
    void GetShapesOverlappingAABB(ShapeVector& elements, const AABB& aabb);
    // Returns true if specified point is inside any of the shapes in the container.
    bool IsPointInside(const Vec3& pt, const CAIShape** outShape = 0) const;
    // Counts and returns number of shapes that contain the specified point.
    unsigned IsPointInsideCount(const Vec3& pt, const CAIShape** outShape = 0) const;
    // Returns true if the specified point lies on any edges within specified tolerance of any of the shapes in the container.
    bool IsPointOnEdge(const Vec3& pt, float tol, const CAIShape** outShape = 0) const;
    // Returns true if the linesegment intersects with any of the shapes in hte containers.
    // Finds nearest hit.
    bool IntersectLineSeg(const Vec3& start, const Vec3& end, Vec3& outClosestPoint,
        Vec3* outNormal = 0, bool bForceNormalOutwards = false, const string* nameToSkip = 0) const;

    // Returns memory used by the container including the shapes.
    size_t MemStats() const;

private:

    typedef CAIQuadTree<SQuadTreeAIShapeElement<CAIShape> > ShapeQuadTree;

    ShapeQuadTree* m_quadTree;
    NameToShapeMap m_nameToShape;
    ShapeVector m_shapes;
};

/*
// Simple helper class to deal with shapes.
class CAISpecialAreaContainer
{
public:

    typedef std::map<string, CAISpecialArea*> NameToShapeMap;
    typedef std::vector<CAISpecialArea*> ShapeVector;
    typedef std::vector<const SQuadTreeAIShapeElement<CAISpecialArea>*> QuadTreeAISpecialAreaVector;

    // Constructor
    CAISpecialAreaContainer();
    // Destructor
    ~CAISpecialAreaContainer();
    // Clear all shapes and supporting structures.
    void Clear();
    // Insert new shape, the container will delete the shape when the container cleared.
    void InsertShape(CAISpecialArea* pShape);
    // Deletes shape from container and removes any references to it.
    void DeleteShape(const string& name);
    // Deletes shape from container and removes any references to it.
    void DeleteShape(CAISpecialArea* pShape);
    // Finds shape by name.
    CAISpecialArea* FindShape(const string& name);
    // Quick lookup of shape by ID.
    inline CAISpecialArea* CAISpecialAreaContainer::GetShapeById(int id)
    {
        AIAssert(id < 0 || id >= m_idToShape.size());
        return m_idToShape[id];
    }
    // Builds quad tree structure to speed up spatial queries.
    void BuildQuadTree(int maxElementsPerCell, float minCellSize);
    // Builds bin sorting structures for the shapes to speed up spatial queries.
    void BuildBins();
    // Returns the shape vector.
    inline const ShapeVector& GetShapes() const { return m_shapes; }
    // Returns the shape vector.
    inline ShapeVector& GetShapes() { return m_shapes; }
    // Returns all shapes that overlap the specified AABB.
    //  bool GetShapesOverlappingAABB(QuadTreeAIShapeVector& elements, const AABB& aabb);
    // Returns true if specified point is inside any of the shapes in the container.
    bool IsPointInside(const Vec3& pt, bool checkHeight, const CAISpecialArea** outShape = 0) const;

    // Returns memory used by the container including the shapes.
    size_t MemStats() const;

private:

    typedef CAIQuadTree<SQuadTreeAIShapeElement<CAISpecialArea> > ShapeQuadTree;

    ShapeQuadTree* m_quadTree;
    NameToShapeMap m_nameToShape;
    ShapeVector m_idToShape;
    ShapeVector m_shapes;
};
*/

#endif // CRYINCLUDE_EDITOR_AI_NAVDATAGENERATION_SHAPECONTAINER_H
