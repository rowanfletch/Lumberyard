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
#ifndef AZ_UNITY_BUILD

#include <AzCore/Jobs/JobManagerComponent.h>
#include <AzCore/Math/Crc.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>

#include <AzCore/Jobs/JobManager.h>
#include <AzCore/Jobs/JobContext.h>

#include <AzCore/std/parallel/thread.h>
#include <AzCore/Math/MathUtils.h>

namespace AZ
{
    //=========================================================================
    // JobManagerComponent
    // [5/29/2012]
    //=========================================================================
    JobManagerComponent::JobManagerComponent()
        : m_jobManager(nullptr)
        , m_jobGlobalContext(nullptr)
        , m_numberOfWorkerThreads(0)
        , m_firstThreadCPU(-1)
    {
    }

    //=========================================================================
    // Activate
    // [5/29/2012]
    //=========================================================================
    void JobManagerComponent::Activate()
    {
        AZ_Assert(m_jobManager == nullptr, "Job manager should not exists at this time!");

        JobManagerBus::Handler::BusConnect();

        JobManagerDesc desc;
        JobManagerThreadDesc threadDesc;

        int numberOfWorkerThreads = m_numberOfWorkerThreads;
        if (numberOfWorkerThreads <= 0)
        {
            numberOfWorkerThreads = AZ::GetMin(static_cast<unsigned int>(desc.m_workerThreads.capacity()), AZStd::thread::hardware_concurrency());
        }

        threadDesc.m_cpuId = m_firstThreadCPU;
        for (int i = 0; i < numberOfWorkerThreads; ++i)
        {
            desc.m_workerThreads.push_back(threadDesc);
            if (threadDesc.m_cpuId > -1)
            {
                threadDesc.m_cpuId++;
            }
        }

        m_jobManager = aznew JobManager(desc);
        m_jobGlobalContext = aznew JobContext(*m_jobManager);

        JobContext::SetGlobalContext(m_jobGlobalContext);
    }

    //=========================================================================
    // Deactivate
    // [5/29/2012]
    //=========================================================================
    void JobManagerComponent::Deactivate()
    {
        JobManagerBus::Handler::BusDisconnect();

        JobContext::SetGlobalContext(nullptr);

        delete m_jobGlobalContext;
        m_jobGlobalContext = nullptr;
        delete m_jobManager;
        m_jobManager = nullptr;
    }

    //=========================================================================
    // GetProvidedServices
    //=========================================================================
    void JobManagerComponent::GetProvidedServices(ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("JobsService", 0xd5ab5a50));
    }

    //=========================================================================
    // GetIncompatibleServices
    //=========================================================================
    void JobManagerComponent::GetIncompatibleServices(ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("JobsService", 0xd5ab5a50));
    }

    //=========================================================================
    // GetDependentServices
    //=========================================================================
    void JobManagerComponent::GetDependentServices(ComponentDescriptor::DependencyArrayType& dependent)
    {
        dependent.push_back(AZ_CRC("MemoryService", 0x5c4d473c));
        dependent.push_back(AZ_CRC("ProfilerService"));
    }

    //=========================================================================
    // Reflect
    //=========================================================================
    void JobManagerComponent::Reflect(ReflectContext* context)
    {
        if (SerializeContext* serializeContext = azrtti_cast<SerializeContext*>(context))
        {
            serializeContext->Class<JobManagerComponent, AZ::Component>()
                ->Version(1)
                ->Field("NumberOfWorkerThreads", &JobManagerComponent::m_numberOfWorkerThreads)
                ->Field("FirstThreadCPUID", &JobManagerComponent::m_firstThreadCPU)
                ;

            if (EditContext* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<JobManagerComponent>(
                    "Job Manager", "Provides fine grained job system and worker threads")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::Category, "Engine")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System", 0xc94d118b))
                    ->DataElement(AZ::Edit::UIHandlers::SpinBox, &JobManagerComponent::m_numberOfWorkerThreads, "Worker threads", "Number of worked threads for this job manager.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0)
                        ->Attribute(AZ::Edit::Attributes::Max, 16)
                    ->DataElement(AZ::Edit::UIHandlers::SpinBox, &JobManagerComponent::m_firstThreadCPU, "CPU ID", "First CPU ID for a worker thread, each consecutive thread will use the next CPU ID. -1 Will not assign CPU Ids")
                        ->Attribute(AZ::Edit::Attributes::Min, -1)
                        ->Attribute(AZ::Edit::Attributes::Max, 16)
                    ;
            }
        }
    }
} // namespace AZ

#endif // #ifndef AZ_UNITY_BUILD
