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

#ifndef CRYINCLUDE_CRYENGINE_RENDERDLL_COMMON_RESFILE_INFO_H
#define CRYINCLUDE_CRYENGINE_RENDERDLL_COMMON_RESFILE_INFO_H
#pragma once

#include "ResFile.h"

STRUCT_INFO_BEGIN(SFileResHeader)
STRUCT_VAR_INFO(hid, TYPE_INFO(uint32))
STRUCT_VAR_INFO(ver, TYPE_INFO(int))
STRUCT_VAR_INFO(num_files, TYPE_INFO(int))
//STRUCT_VAR_INFO(num_files_ref, TYPE_INFO(int))
STRUCT_VAR_INFO(ofs_dir, TYPE_INFO(int))
STRUCT_VAR_INFO(num_files_ref, TYPE_INFO(uint32))
STRUCT_INFO_END(SFileResHeader)

STRUCT_INFO_BEGIN(SResFileLookupDataDisk)
STRUCT_VAR_INFO(m_NumOfFilesUnique, TYPE_INFO(int))
STRUCT_VAR_INFO(m_NumOfFilesRef, TYPE_INFO(int))
STRUCT_VAR_INFO(m_OffsetDir, TYPE_INFO(uint32))
STRUCT_VAR_INFO(m_CRC32, TYPE_INFO(uint32))
STRUCT_VAR_INFO(m_CacheMajorVer, TYPE_INFO(uint16))
STRUCT_VAR_INFO(m_CacheMinorVer, TYPE_INFO(uint16))
STRUCT_INFO_END(SResFileLookupDataDisk)

STRUCT_INFO_BEGIN(SCFXLookupData)
STRUCT_VAR_INFO(m_CRC32, TYPE_INFO(uint32))
STRUCT_INFO_END(SCFXLookupData)

STRUCT_INFO_BEGIN(CCryNameTSCRC)
STRUCT_VAR_INFO(m_nID, TYPE_INFO(int))
STRUCT_INFO_END(CCryNameTSCRC)

STRUCT_INFO_BEGIN(SDirEntry)
STRUCT_VAR_INFO(Name, TYPE_INFO(CCryNameTSCRC))
STRUCT_BITFIELD_INFO(size, TYPE_INFO(uint32), 24)
STRUCT_BITFIELD_INFO(flags, TYPE_INFO(uint32), 8)
STRUCT_VAR_INFO(offset, TYPE_INFO(int32))
STRUCT_INFO_END(SDirEntry)

STRUCT_INFO_BEGIN(SDirEntryRef)
STRUCT_VAR_INFO(Name, TYPE_INFO(CCryNameTSCRC))
STRUCT_VAR_INFO(ref, TYPE_INFO(uint32))
STRUCT_INFO_END(SDirEntryRef)

STRUCT_INFO_BEGIN(SShaderBinHeader)
STRUCT_VAR_INFO(m_Magic, TYPE_INFO(FOURCC))
STRUCT_VAR_INFO(m_CRC32, TYPE_INFO(uint32))
STRUCT_VAR_INFO(m_VersionLow, TYPE_INFO(uint16))
STRUCT_VAR_INFO(m_VersionHigh, TYPE_INFO(uint16))
STRUCT_VAR_INFO(m_nOffsetStringTable, TYPE_INFO(uint32))
STRUCT_VAR_INFO(m_nOffsetParamsLocal, TYPE_INFO(uint32))
STRUCT_VAR_INFO(m_nTokens, TYPE_INFO(uint32))
STRUCT_VAR_INFO(m_nSourceCRC32, TYPE_INFO(uint32))
STRUCT_INFO_END(SShaderBinHeader)

STRUCT_INFO_BEGIN(SShaderBinParamsHeader)
STRUCT_VAR_INFO(nMask, TYPE_INFO(uint32))
STRUCT_VAR_INFO(nName, TYPE_INFO(uint32))
STRUCT_VAR_INFO(nParams, TYPE_INFO(int32))
STRUCT_VAR_INFO(nFuncs, TYPE_INFO(int32))
STRUCT_INFO_END(SShaderBinParamsHeader)

STRUCT_INFO_BEGIN(SVersionInfo)
STRUCT_VAR_INFO(m_ResVersion, TYPE_INFO(int))
STRUCT_VAR_INFO(m_szCacheVer, TYPE_ARRAY(16, TYPE_INFO(char)))
STRUCT_INFO_END(SVersionInfo)

#if defined(SHADERS_SERIALIZING)

STRUCT_INFO_BEGIN(SSShaderCacheHeader)
STRUCT_VAR_INFO(m_SizeOf, TYPE_INFO(int))
STRUCT_VAR_INFO(m_szVer, TYPE_ARRAY(16, TYPE_INFO(char)))
STRUCT_VAR_INFO(m_MajorVer, TYPE_INFO(uint16))
STRUCT_VAR_INFO(m_MinorVer, TYPE_INFO(uint16))
STRUCT_VAR_INFO(m_CRC32, TYPE_INFO(uint32))
STRUCT_VAR_INFO(m_SourceCRC32, TYPE_INFO(uint32))
STRUCT_INFO_END(SSShaderCacheHeader)

#endif

STRUCT_INFO_BEGIN(SShaderCacheHeaderItem)
STRUCT_VAR_INFO(m_nVertexFormat, TYPE_INFO(byte))
STRUCT_VAR_INFO(m_Class, TYPE_INFO(byte))
STRUCT_VAR_INFO(m_nInstBinds, TYPE_INFO(byte))
STRUCT_VAR_INFO(m_StreamMask_Stream, TYPE_INFO(byte))
STRUCT_VAR_INFO(m_StreamMask_Decl, TYPE_INFO(uint16))
STRUCT_VAR_INFO(m_nInstructions, TYPE_INFO(short))
STRUCT_VAR_INFO(m_CRC32, TYPE_INFO(uint32))
//STRUCT_VAR_INFO(m_DeviceObjectID, TYPE_INFO(int))
STRUCT_INFO_END(SShaderCacheHeaderItem)

#endif // CRYINCLUDE_CRYENGINE_RENDERDLL_COMMON_RESFILE_INFO_H
