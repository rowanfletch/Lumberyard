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

#include "stdafx.h"

#include "GenericLogPanel.h"

#include <AzCore/IO/FileIO.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzFramework/IO/LocalFileIO.h>

#include <QDateTime>
#include <QTimer>

#include <UI/Logging/GenericLogPanel.moc>

#include <AzToolsFramework/UI/Logging/LogLine.h>
#include <AzFramework/Logging/LogFile.h>

namespace AzToolsFramework
{
    namespace LogPanel
    {
        GenericLogPanel::GenericLogPanel(QWidget* pParent /* = nullptr*/)
            : BaseLogPanel(pParent)
        {
        }

        QWidget* GenericLogPanel::CreateTab(const TabSettings& settings)
        {
            GenericLogTab* newTab = new GenericLogTab(this, settings);
            newTab->SetDataSource(&m_dataModel);
            return newTab;
        }

        //! Fill From File - append log data from a file
        //! You can call this repeatedly if you'd like.
        void GenericLogPanel::FillFromFile(const AZStd::string& fileName)
        {
            using namespace AZ::IO;

            FileIOBase* mainInstance = FileIOBase::GetInstance();

            // delete localInstance if it goes out of scope for any reason:
            AZStd::unique_ptr<FileIOBase> localInstance;
            if (!mainInstance)
            {
                // create one locally
                localInstance.reset(new LocalFileIO());
                mainInstance = localInstance.get();
            }

            HandleType openedFile = InvalidHandle;
            if (!mainInstance->Open(fileName.c_str(), OpenMode::ModeRead, openedFile))
            {
                Logging::LogLine line(AZStd::string::format("unable to open the log file at %s", fileName.c_str()).c_str(), "LOGGING", Logging::LogLine::TYPE_WARNING, QDateTime::currentMSecsSinceEpoch());
                AddLogLine(line);
                CommitAddedLines();
                return;
            }

            AZ::u64 logSize = 0;
            if (!mainInstance->Size(openedFile, logSize))
            {
                Logging::LogLine line(AZStd::string::format("unable to read the size of the log file at %s", fileName.c_str()).c_str(), "LOGGING", Logging::LogLine::TYPE_WARNING, QDateTime::currentMSecsSinceEpoch());
                AddLogLine(line);
                mainInstance->Close(openedFile);
                CommitAddedLines();
                return;
            }

            AZStd::vector<char> tempStorage;
            tempStorage.resize(logSize + 1);
            if (!mainInstance->Read(openedFile, tempStorage.data(), logSize, true))
            {
                Logging::LogLine line(AZStd::string::format("Unable to read %llu bytes from log file %s", logSize, fileName.c_str()).c_str(), "LOGGING", Logging::LogLine::TYPE_WARNING, QDateTime::currentMSecsSinceEpoch());
                AddLogLine(line);
                mainInstance->Close(openedFile);
                CommitAddedLines();
                return;
            }

            tempStorage[logSize] = 0;
            ParseData(tempStorage.data(), logSize);

            mainInstance->Close(openedFile);
        }

        void GenericLogPanel::ParseData(const char* entireLog, AZ::u64 logLength)
        {
            Logging::LogLine::ParseLog(entireLog, logLength, 
                AZStd::bind(&GenericLogPanel::AddLogLine, this, AZStd::placeholders::_1));

            CommitAddedLines();
        }

        //! calling AddLogLine consumes the given line (move operation) and updates all tabs
        void GenericLogPanel::AddLogLine(Logging::LogLine& target)
        {
            m_dataModel.AppendLine(target);
            if (!m_alreadyQueuedCommit)
            {
                m_alreadyQueuedCommit = true;
                QMetaObject::invokeMethod(this, "CommitAddedLines", Qt::QueuedConnection);
            }
        }

        void GenericLogPanel::CommitAddedLines()
        {
            m_alreadyQueuedCommit = false;
            m_dataModel.CommitAdd();
        }

        GenericLogTab::GenericLogTab(QWidget* pParent, const TabSettings& in_settings)
            : BaseLogView(pParent)
        {
            m_filteredModel.SetTabSettings(in_settings);
        }

        void GenericLogTab::SetDataSource(QAbstractItemModel* source)
        {
            m_filteredModel.setSourceModel(source);
            ConnectModelToView(&m_filteredModel);
        }
    } // namespace LogPanel
} // namespace AzToolsFramework

