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

// Description : Time TRange class.


#ifndef CRYINCLUDE_CRYCOMMON_RANGE_H
#define CRYINCLUDE_CRYCOMMON_RANGE_H

#pragma once

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/*!
        Class TRange, can represent anything that is range between two values, mostly used for time ranges.
 */
template <class T>
class TRange
{
public:
    T start;
    T end;

    TRange()    { start = T(0); end = T(0); };
    TRange(const TRange& r) { start = r.start; end = r.end; };
    TRange(T s, T e) { start = s; end = e; };

    void Set(T s, T e) { start = s; end = e; };
    void Clear() { start = 0; end = 0; };

    //! Get length of range.
    T   Length() const { return end - start; };
    //! Check if range is empty.
    bool IsEmpty()  const { return (start == 0 && end == 0); }

    //! Check if value is inside range.
    bool IsInside(T val) { return val >= start && val <= end; };

    void ClipValue(T& val) const
    {
        if (val < start)
        {
            val = start;
        }
        if (val > end)
        {
            val = end;
        }
    }

    //! Compare two ranges.
    bool    operator == (const TRange& r) const
    {
        return start == r.start && end == r.end;
    }

    bool  operator != (const TRange& r) const
    {
        return !(*this == r);
    }
    //! Assign operator.
    TRange& operator =(const TRange& r)
    {
        start = r.start;
        end = r.end;
        return *this;
    }
    //! Interect two ranges.
    TRange  operator & (const TRange& r) const
    {
        return TRange(MAX(start, r.start), MIN(end, r.end));
    }
    TRange& operator &= (const TRange& r)
    {
        return (*this = (*this & r));
    }
    //! Concatent two ranges.
    TRange  operator | (const TRange& r) const
    {
        return TRange(MIN(start, r.start), MAX(end, r.end));
    }
    TRange& operator |= (const TRange& r)
    {
        return (*this = (*this | r));
    }
    //! Add new value to range.
    TRange  operator + (T v) const
    {
        T s = start, e = end;
        if (v < start)
        {
            s = v;
        }
        if (v > end)
        {
            e = v;
        }
        return TRange(s, e);
    }
    //! Add new value to range.
    TRange& operator += (T v) const
    {
        if (v < start)
        {
            start = v;
        }
        if (v > end)
        {
            end = v;
        }
        return *this;
    }
};

//! CRange if just TRange for floats..
typedef TRange<float> Range;

#endif // CRYINCLUDE_CRYCOMMON_RANGE_H
