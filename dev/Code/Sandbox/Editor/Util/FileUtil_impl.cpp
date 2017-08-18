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
#include "FileUtil_impl.h"
#include "FileUtil.h"

bool CFileUtil_impl::ScanDirectory(const QString& path, const QString& fileSpec, FileArray& files, bool recursive, bool addDirAlso, ScanDirectoryUpdateCallBack updateCB, bool bSkipPaks)
{
    return CFileUtil::ScanDirectory(path, fileSpec, files, recursive, addDirAlso, updateCB, bSkipPaks);
}

void CFileUtil_impl::ShowInExplorer(const QString& path)
{
    CFileUtil::ShowInExplorer(path);
}

bool CFileUtil_impl::CompileLuaFile(const char* luaFilename)
{
    return CFileUtil::CompileLuaFile(luaFilename);
}

bool CFileUtil_impl::ExtractFile(QString& file, bool bMsgBoxAskForExtraction, const char* pDestinationFilename)
{
    return CFileUtil::ExtractFile(file, bMsgBoxAskForExtraction, pDestinationFilename);
}

void CFileUtil_impl::EditTextFile(const char* txtFile, int line, ETextFileType fileType)
{
    CFileUtil::EditTextFile(txtFile, line, fileType);
}

void CFileUtil_impl::EditTextureFile(const char* txtureFile, bool bUseGameFolder)
{
    CFileUtil::EditTextureFile(txtureFile, bUseGameFolder);
}

bool CFileUtil_impl::CalculateDccFilename(const QString& assetFilename, QString& dccFilename)
{
    return CFileUtil::CalculateDccFilename(assetFilename, dccFilename);
}

void CFileUtil_impl::FormatFilterString(QString& filter)
{
    CFileUtil::FormatFilterString(filter);
}

bool CFileUtil_impl::SelectSaveFile(const QString& fileFilter, const QString& defaulExtension, const QString& startFolder, QString& fileName)
{
    return CFileUtil::SelectSaveFile(fileFilter, defaulExtension, startFolder, fileName);
}

bool CFileUtil_impl::OverwriteFile(const QString& filename)
{
    return CFileUtil::OverwriteFile(filename);
}

bool CFileUtil_impl::CheckoutFile(const char* filename, QWidget* parentWindow)
{
    return CFileUtil::CheckoutFile(filename, parentWindow);
}

bool CFileUtil_impl::RevertFile(const char* filename, QWidget* parentWindow)
{
    return CFileUtil::RevertFile(filename, parentWindow);
}

bool CFileUtil_impl::DeleteFromSourceControl(const char* filename, QWidget* parentWindow)
{
    return CFileUtil::DeleteFromSourceControl(filename, parentWindow);
}

void CFileUtil_impl::CreateDirectory(const char* dir)
{
    CFileUtil::CreateDirectory(dir);
}

void CFileUtil_impl::BackupFile(const char* filename)
{
    CFileUtil::BackupFile(filename);
}

void CFileUtil_impl::BackupFileDated(const char* filename, bool bUseBackupSubDirectory)
{
    CFileUtil::BackupFileDated(filename, bUseBackupSubDirectory);
}

bool CFileUtil_impl::Deltree(const char* szFolder, bool bRecurse)
{
    return CFileUtil::Deltree(szFolder, bRecurse);
}

bool CFileUtil_impl::Exists(const QString& strPath, bool boDirectory, FileDesc* pDesc)
{
    return CFileUtil::Exists(strPath, boDirectory, pDesc);
}

bool CFileUtil_impl::FileExists(const QString& strFilePath, FileDesc* pDesc)
{
    return CFileUtil::FileExists(strFilePath, pDesc);
}

bool CFileUtil_impl::PathExists(const QString& strPath)
{
    return CFileUtil::PathExists(strPath);
}

bool CFileUtil_impl::GetDiskFileSize(const char* pFilePath, uint64& rOutSize)
{
    return CFileUtil::GetDiskFileSize(pFilePath, rOutSize);
}

bool CFileUtil_impl::IsFileExclusivelyAccessable(const QString& strFilePath)
{
    return CFileUtil::IsFileExclusivelyAccessable(strFilePath);
}

bool CFileUtil_impl::CreatePath(const QString& strPath)
{
    return CFileUtil::CreatePath(strPath);
}

bool CFileUtil_impl::DeleteFile(const QString& strPath)
{
    return CFileUtil::DeleteFile(strPath);
}

bool CFileUtil_impl::RemoveDirectory(const QString& strPath)
{
    return CFileUtil::RemoveDirectory(strPath);
}

IFileUtil::ECopyTreeResult CFileUtil_impl::CopyTree(const QString& strSourceDirectory, const QString& strTargetDirectory, bool boRecurse, bool boConfirmOverwrite)
{
    return CFileUtil::CopyTree(strSourceDirectory, strTargetDirectory, boRecurse, boConfirmOverwrite);
}

IFileUtil::ECopyTreeResult CFileUtil_impl::CopyFile(const QString& strSourceFile, const QString& strTargetFile, bool boConfirmOverwrite, void* pfnProgress, bool* pbCancel)
{
    return CFileUtil::CopyFile(strSourceFile, strTargetFile, boConfirmOverwrite, pfnProgress, pbCancel);
}

IFileUtil::ECopyTreeResult CFileUtil_impl::MoveTree(const QString& strSourceDirectory, const QString& strTargetDirectory, bool boRecurse, bool boConfirmOverwrite)
{
    return CFileUtil::MoveTree(strSourceDirectory, strTargetDirectory, boRecurse, boConfirmOverwrite);
}

IFileUtil::ECopyTreeResult CFileUtil_impl::MoveFile(const QString& strSourceFile, const QString& strTargetFile, bool boConfirmOverwrite)
{
    return CFileUtil::MoveFile(strSourceFile, strTargetFile, boConfirmOverwrite);
}

void CFileUtil_impl::GatherAssetFilenamesFromLevel(std::set<QString>& rOutFilenames, bool bMakeLowerCase, bool bMakeUnixPath)
{
    CFileUtil::GatherAssetFilenamesFromLevel(rOutFilenames, bMakeLowerCase, bMakeUnixPath);
}

uint32 CFileUtil_impl::GetAttributes(const char* filename, bool bUseSourceControl)
{
    return CFileUtil::GetAttributes(filename, bUseSourceControl);
}

bool CFileUtil_impl::CompareFiles(const QString& strFilePath1, const QString& strFilePath2)
{
    return CFileUtil::CompareFiles(strFilePath1, strFilePath2);
}

QString CFileUtil_impl::GetPath(const QString& path)
{
    return Path::GetPath(path);
}


