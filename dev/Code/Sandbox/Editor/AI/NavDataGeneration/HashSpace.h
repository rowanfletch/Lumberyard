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

#ifndef CRYINCLUDE_EDITOR_AI_NAVDATAGENERATION_HASHSPACE_H
#define CRYINCLUDE_EDITOR_AI_NAVDATAGENERATION_HASHSPACE_H
#pragma once


#include "CryFile.h"
#include "CommonDefs.h"

#include <vector>

template<typename T>
class CHashSpaceTraits
{
public:
    const Vec3& operator() (const T& item) const {return item.GetPos(); }
};

/// Contains any type that can be represented with a single position, obtained
/// through T::GetPos()
template<typename T, typename TraitsT = CHashSpaceTraits<T> >
class CHashSpace
    : public TraitsT
{
public:
    typedef TraitsT Traits;

    //simple iterator encapsulating the lookup from processobjecstwithinrange
    //to reduce code duplication
    class CRadialIterator
    {
    public:


        CRadialIterator             (const Vec3& pos, float radius, const CHashSpace<T, TraitsT>& hashSpace)
            : m_curIndex(0)
            , m_radiusSQ(square(radius))
            , m_Pos(pos)
            , m_object(NULL)
            , m_hashSpace(hashSpace)
        {
            int32 starti;
            hashSpace.GetIJKFromPosition(pos - Vec3(radius, radius, radius), starti, m_startJ, m_startK);
            hashSpace.GetIJKFromPosition(pos + Vec3(radius, radius, radius), m_endI, m_endJ, m_endK);

            m_curI = starti;
            m_curJ = m_startJ;
            m_curK = m_startK;

            operator++();
        }

        ~CRadialIterator                        () {}

        operator T*                                 ()
        {
            return const_cast<T*>(m_object);
        }

        operator const T*                       () const
        {
            return m_object;
        }

        float           GetCurDistSQ            () const
        {
            return m_curDistSQ;
        }

        CRadialIterator operator++  (int)
        {
            CRadialIterator temp =  *this;

            operator++();

            return temp;
        }

        void    operator++                      ()
        {
            m_object = NULL;

            for (; m_curI <= m_endI; ++m_curI)
            {
                for (; m_curJ <= m_endJ; ++m_curJ)
                {
                    for (; m_curK <= m_endK; ++m_curK)
                    {
                        unsigned index = m_hashSpace.GetHashBucketIndex(m_curI, m_curJ, m_curK);
                        unsigned mask = m_hashSpace.GetLocationMask(m_curI, m_curJ, m_curK);
                        const Objects& objects = m_hashSpace.m_cells[index].objects;
                        const std::vector<unsigned>& masks = m_hashSpace.m_cells[index].masks;

                        uint32  count = objects.size();
                        for (; m_curIndex < count; ++m_curIndex)
                        {
                            if (mask != masks[m_curIndex])
                            {
                                continue;
                            }
                            const T& object = objects[m_curIndex];
                            m_curDistSQ = Distance::Point_PointSq(m_Pos, m_hashSpace.GetPos(object));
                            if (m_curDistSQ < m_radiusSQ)
                            {
                                m_object = &object;
                                ++m_curIndex;
                                return;
                            }
                        }

                        m_curIndex = 0;
                    }

                    m_curK = m_startK;
                }

                m_curJ = m_startJ;
            }

            m_curDistSQ = -1.0f;
        }

    private:

        int32                                                   m_startJ;
        int32                                                   m_startK;

        int32                                                   m_endI;
        int32                                                   m_endJ;
        int32                                                   m_endK;

        int32                                                   m_curI;
        int32                                                   m_curJ;
        int32                                                   m_curK;

        uint32                                              m_curIndex;
        float                                                   m_radiusSQ;

        Vec3                                                    m_Pos;

        const T* m_object;
        float                                                   m_curDistSQ;

        const CHashSpace<T, TraitsT>& m_hashSpace;
    };

    // Note: bucket count must be power of 2!
    CHashSpace(const Vec3& cellSize, int buckets);
    CHashSpace(const Vec3& cellSize, int buckets, const Traits& traits);
    ~CHashSpace();

    /// Adds a copy of the object
    void AddObject(const T& object);
    /// Removes all objects indicated by operator==
    void RemoveObject(const T& object);
    /// Removes all objects
    void Clear(bool freeMemory);

    /// the functor gets called and passed every object (and the distance-squared) that is within radius of
    /// pos. Returns the number of objects within range
    template<typename Functor>
    void ProcessObjectsWithinRadius (const Vec3& pos, float radius, Functor& functor);

    template<typename Functor>
    void ProcessObjectsWithinRadius (const Vec3& pos, float radius, Functor& functor) const;

    //
    template <typename Functor>
    bool GetObjectWithinRadius          (const Vec3& pos, float radius, Functor& functor) const;

    /// returns the total number of objects
    inline unsigned GetNumObjects() const { return m_totalNumObjects; }
    // Returns number of buckets in the hash space.
    inline unsigned GetBucketCount() const { return m_cells.size(); }
    // Returns number of objects in the specified bucket.
    inline unsigned GetObjectCountInBucket(unsigned bucket) const { return m_cells[bucket].objects.size(); }
    // Returns the indexed object in the specified bucket.
    inline const T& GetObjectInBucket(unsigned obj, unsigned bucket) const { return m_cells[bucket].objects[obj]; }

    /// Gets the AABB associated with an i, j, k
    void GetAABBFromIJK(int i, int j, int k, AABB& aabb) const;
    /// return the individual i, j, k for 3D array lookup
    void GetIJKFromPosition(const Vec3& pos, int& i, int& j, int& k) const;
    /// returns bucket index from given i,j,k
    unsigned GetHashBucketIndex(int i, int j, int k) const;

    /// Returns true if successful, false otherwise
    bool WriteToFile(CCryFile& file);
    /// Returns true if successful, false otherwise
    bool ReadFromFile(CCryFile& file);

    /// Returns the memory usage in bytes
    size_t MemStats() const;

    inline unsigned GetLocationMask(int i, int j, int k) const
    {
        const unsigned mi = (1 << 12) - 1;
        const unsigned mj = (1 << 12) - 1;
        const unsigned mk = (1 << 8) - 1;
        unsigned x = (unsigned)(i & mi);
        unsigned y = (unsigned)(j & mj);
        unsigned z = (unsigned)(k & mk);
        return x | (y << 12) | (z << 24);
    }

private:
    const Vec3& GetPos(const T& item) const;

    typedef std::vector<T> Objects;
    struct Cell
    {
        std::vector<unsigned> masks;
        Objects objects;
    };
    typedef std::vector< Cell > Cells;

    /// returns index into m_cells
    unsigned GetCellIndexFromPosition(const Vec3& pos) const;

    Cells m_cells;
    Vec3 m_cellSize;
    Vec3 m_cellScale;
    int m_buckets;

    unsigned m_totalNumObjects;
};

//====================================================================
// Inline implementation
//====================================================================

//====================================================================
// MemStats
//====================================================================
template<typename T, typename TraitsT>
size_t CHashSpace<T, TraitsT>::MemStats() const
{
    size_t result = 0;
    for (unsigned i = 0; i < m_cells.size(); ++i)
    {
        result += sizeof(Cell) + m_cells[i].objects.capacity() * sizeof(T) + m_cells[i].masks.capacity() * sizeof(unsigned);
    }
    result += sizeof(*this);
    return result;
}

//====================================================================
// CHashSpace
//====================================================================
template<typename T, typename TraitsT>
inline CHashSpace<T, TraitsT>::CHashSpace(const Vec3& cellSize, int buckets, const Traits& traits)
    : Traits(traits)
    , m_cellSize(cellSize)
    , m_cellScale(1.0f / cellSize.x, 1.0f / cellSize.y, 1.0f / cellSize.z)
    , m_buckets(buckets)
    , m_totalNumObjects(0)
{
    m_cells.resize(m_buckets);
}

template<typename T, typename TraitsT>
inline CHashSpace<T, TraitsT>::CHashSpace(const Vec3& cellSize, int buckets)
    : m_cellSize(cellSize)
    , m_cellScale(1.0f / cellSize.x, 1.0f / cellSize.y, 1.0f / cellSize.z)
    , m_buckets(buckets)
    , m_totalNumObjects(0)
{
    m_cells.resize(m_buckets);
}

//====================================================================
// ~CHashSpace
//====================================================================
template<typename T, typename TraitsT>
inline CHashSpace<T, TraitsT>::~CHashSpace()
{
}

//====================================================================
// Clear
//====================================================================
template<typename T, typename TraitsT>
inline void CHashSpace<T, TraitsT>::Clear(bool clearMemory)
{
    for (unsigned i = 0; i < m_cells.size(); ++i)
    {
        if (clearMemory)
        {
            ClearVectorMemory(m_cells[i].objects);
            ClearVectorMemory(m_cells[i].masks);
        }
        else
        {
            m_cells[i].objects.resize(0);
            m_cells[i].masks.resize(0);
        }
    }
    m_totalNumObjects = 0;
}

//====================================================================
// GetIJKFromPosition
//====================================================================
template<typename T, typename TraitsT>
inline void CHashSpace<T, TraitsT>::GetIJKFromPosition(const Vec3& pos, int& i, int& j, int& k) const
{
    i = (int)floorf(pos.x * m_cellScale.x);
    j = (int)floorf(pos.y * m_cellScale.y);
    k = (int)floorf(pos.z * m_cellScale.z);
}

//====================================================================
// GetHashBucketIndex
//====================================================================
template<typename T, typename TraitsT>
inline unsigned CHashSpace<T, TraitsT>::GetHashBucketIndex(int i, int j, int k) const
{
    const int h1 = 0x8da6b343;
    const int h2 = 0xd8163841;
    const int h3 = 0xcb1ab31f;
    int n = h1 * i + h2 * j + h3 * k;
    return (unsigned)(n & (m_buckets - 1));

    /*  const int h1 = 73856093;
        const int h2 = 19349663;
        const int h3 = 83492791;
        int n = (h1*i) ^ (h2*j) ^ (h3*k);
        return (unsigned)(n & (m_buckets-1));*/

    /*  const int shift = 5;
        const int mask = (1 << shift)-1;
        int n = (i & mask) | ((j&mask) << shift) | ((k&mask) << (shift*2));
        return (unsigned)(n & (m_buckets-1));*/
}


//====================================================================
// GetCellIndexFromPosition
//====================================================================
template<typename T, typename TraitsT>
inline unsigned CHashSpace<T, TraitsT>::GetCellIndexFromPosition(const Vec3& pos) const
{
    int i, j, k;
    GetIJKFromPosition(pos, i, j, k);
    return GetHashBucketIndex(i, j, k);
}

//====================================================================
// GetAABBFromIJK
//====================================================================
template<typename T, typename TraitsT>
inline void CHashSpace<T, TraitsT>::GetAABBFromIJK(int i, int j, int k, AABB& aabb) const
{
    aabb.min.Set(i * m_cellSize.x, j * m_cellSize.y, k * m_cellSize.z);
    aabb.max = aabb.min + m_cellSize;
}


//====================================================================
// AddObject
//====================================================================
template<typename T, typename TraitsT>
inline void CHashSpace<T, TraitsT>::AddObject(const T& object)
{
    int i, j, k;
    GetIJKFromPosition(GetPos(object), i, j, k);
    unsigned index = GetHashBucketIndex(i, j, k);
    unsigned mask = GetLocationMask(i, j, k);

    m_cells[index].objects.push_back(object);
    m_cells[index].masks.push_back(mask);

    ++m_totalNumObjects;
}

//====================================================================
// RemoveObject
//====================================================================
template<typename T, typename TraitsT>
inline void CHashSpace<T, TraitsT>::RemoveObject(const T& object)
{
    unsigned index = GetCellIndexFromPosition(GetPos(object));
    Cell& cell = m_cells[index];
    for (unsigned i = 0, ni = cell.objects.size(); i < ni; )
    {
        T& obj = cell.objects[i];
        if (obj == object)
        {
            cell.objects[i] = cell.objects.back();
            cell.objects.pop_back();
            cell.masks[i] = cell.masks.back();
            cell.masks.pop_back();
            --ni;
            --m_totalNumObjects;
        }
        else
        {
            ++i;
        }
    }
}

//====================================================================
// ProcessObjectsWithinRadius
//====================================================================
template<typename T, typename TraitsT>
template <typename Functor>
void CHashSpace<T, TraitsT>::ProcessObjectsWithinRadius(const Vec3& pos, float radius, Functor& functor)
{
    T* obj = NULL;
    for (typename CHashSpace<T, TraitsT>::CRadialIterator it(pos, radius, *this); obj = it; ++it)
    {
        functor(*obj, it.GetCurDistSQ());
    }
}

//====================================================================
// ProcessObjectsWithinRadius
//====================================================================
template<typename T, typename TraitsT>
template <typename Functor>
void CHashSpace<T, TraitsT>::ProcessObjectsWithinRadius(const Vec3& pos, float radius, Functor& functor) const
{
    T* obj = NULL;
    for (typename CHashSpace<T, TraitsT>::CRadialIterator it(pos, radius, *this); obj = it; ++it)
    {
        functor(*obj, it.GetCurDistSQ());
    }
}

//====================================================================
// GetObjectWithinRadius
//====================================================================
template<typename T, typename TraitsT>
template <typename Functor>
bool CHashSpace<T, TraitsT>::GetObjectWithinRadius(const Vec3& pos, float radius, Functor& functor) const
{
    bool ret = false;

    const T* obj = NULL;
    for (typename CHashSpace<T, TraitsT>::CRadialIterator it(pos, radius, *this); obj = it; ++it)
    {
        if (functor(*obj, it.GetCurDistSQ()))
        {
            ret = true;
            break;
        }
    }

    return ret;
}


//====================================================================
// WriteToFile
//====================================================================
template<typename T, typename TraitsT>
bool CHashSpace<T, TraitsT>::WriteToFile(CCryFile& file)
{
    int version = 1;
    file.Write(&version, sizeof(version));
    file.Write(&m_buckets, sizeof(m_buckets));
    file.Write(&m_cellSize, sizeof(m_cellSize));
    unsigned num = m_cells.size();
    file.Write(&num, sizeof(num));
    for (unsigned i = 0; i < num; ++i)
    {
        Cell& cell = m_cells[i];
        unsigned numObjects = cell.objects.size();
        file.Write(&numObjects, sizeof(numObjects));
        if (numObjects > 0)
        {
            file.Write(&cell.objects[0], numObjects * sizeof(cell.objects[0]));
            file.Write(&cell.masks[0], numObjects * sizeof(cell.masks[0]));
        }
    }
    return true;
}

//====================================================================
// ReadFromFile
//====================================================================
template<typename T, typename TraitsT>
bool CHashSpace<T, TraitsT>::ReadFromFile(CCryFile& file)
{
    Clear(false);
    int version = 1;
    file.Write(&version, sizeof(version));
    file.Write(&m_buckets, sizeof(m_buckets));
    file.ReadType(&m_cellSize);
    m_cellScale.x = 1.0f / m_cellSize.x;
    m_cellScale.y = 1.0f / m_cellSize.y;
    m_cellScale.z = 1.0f / m_cellSize.z;
    unsigned num;
    file.ReadType(&num);
    m_cells.resize(num);
    for (unsigned i = 0; i < num; ++i)
    {
        Cell& cell = m_cells[i];
        unsigned numObjects;
        file.ReadType(&numObjects);
        cell.objects.resize(numObjects);
        cell.masks.resize(numObjects);
        if (numObjects > 0)
        {
            file.ReadType(&cell.objects[0], numObjects);
            file.ReadType(&cell.masks[0], numObjects);
        }
        m_totalNumObjects += numObjects;
    }
    return true;
}

//====================================================================
// GetPos
//====================================================================
template<typename T, typename TraitsT>
const Vec3& CHashSpace<T, TraitsT>::GetPos(const T& item) const
{
    return (*static_cast<const Traits*>(this))(item);
}

#endif // CRYINCLUDE_EDITOR_AI_NAVDATAGENERATION_HASHSPACE_H
