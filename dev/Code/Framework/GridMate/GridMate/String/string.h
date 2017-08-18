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
#ifndef GM_CONTAINERS_STRING_H
#define GM_CONTAINERS_STRING_H

#include <GridMate/Memory.h>
#include <AzCore/std/string/string.h>


namespace GridMate
{
    /**
     * All libs should use specialized allocators
     */
    typedef AZStd::basic_string<char, AZStd::char_traits<char>, SysContAlloc > string;
    typedef AZStd::basic_string<wchar_t, AZStd::char_traits<wchar_t>, SysContAlloc > wstring;

    typedef AZStd::basic_string<char, AZStd::char_traits<char>, GridMateStdAlloc > gridmate_string;
    typedef AZStd::basic_string<wchar_t, AZStd::char_traits<wchar_t>, GridMateStdAlloc > gridmate_wstring;
}
#endif // GM_CONTAINERS_STRING_H
