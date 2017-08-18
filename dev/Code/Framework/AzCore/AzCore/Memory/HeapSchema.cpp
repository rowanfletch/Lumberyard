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

#include <AzCore/Memory/HeapSchema.h>
#include <AzCore/Memory/SystemAllocator.h>

#include <AzCore/PlatformIncl.h>
#include <AzCore/std/parallel/mutex.h>
#include <AzCore/std/parallel/thread.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// We use dlmalloc as a default system allocator or version of it (nedmalloc, ptmalloc3).
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define MSPACES 1
#define MALLOC_FAILURE_ACTION       // DO nothing we assert in our code with more information

#define ONLY_MSPACES 1
#define HAVE_MORECORE 0
#if !defined(AZ_PLATFORM_WINDOWS)
#   define HAVE_MMAP 0
#endif // !AZ_PLATFORM_WINDOWS
#define LACKS_SYS_TYPES_H
#define LACKS_UNISTD_H
#define LACKS_SYS_PARAM_H
#define LACKS_SYS_MMAN_H
#define LACKS_STRINGS_H
#define LACKS_SYS_TYPES_H
#define LACKS_ERRNO_H

#ifdef AZ_PLATFORM_WII
#   define MALLOC_ALIGNMENT ((size_t)32U)

#   include <stdlib.h> // for size_t, ptrdiff_t
using namespace std;
#endif //

// Need to enable footers so frees lock the right mspace (we can remove in release if we use only one memory space)
#define FOOTERS 1

#if !defined(_DEBUG)
// Skip any checks
#   define INSECURE 1
#endif

// We can enable this in special BUILDS (not even debug) to check memory integrity!
#undef DEBUG
#define USE_DL_PREFIX

///* The default of 64Kb means we spend too much time kernel-side */
//#ifndef DEFAULT_GRANULARITY
//#define DEFAULT_GRANULARITY (1*1024*1024)
//#endif

void ConstuctMutex(void* address)
{
    AZStd::mutex* mx = new(address) AZStd::mutex();
    (void)mx;
#   if defined(AZ_PLATFORM_WINDOWS) || defined(AZ_PLATFORM_X360)
    // In memory allocation case (usually tools) we might have high contention,
    // using spin lock will improve performance.
    SetCriticalSectionSpinCount(mx->native_handle(), 4000);
#   endif
}

#define USE_LOCKS 2         ///< Use our lock

#define MLOCK_T AZStd::mutex

// this is tricky since we initialize the mutex in the ctor, but the code uses C memset and POD struct.
#define INITIAL_LOCK(m)  ConstuctMutex(m)
// we have added destroy lock to make sure each mutex is free properly.
#define DESTROY_LOCK(m)  (m)->~mutex()
#define ACQUIRE_LOCK(m)  ((m)->lock(), 0)
#define RELEASE_LOCK(m)  (m)->unlock()
#define TRY_LOCK(m)      (m)->try_lock()

// The global lock is used only when we initialize global mparams (which will happen on the first mspace_create)
// and creating allocators is NOT thread safe by definition. Second it's used to get continuous memory
// which we don't enable. So we can skip it all together.
#define ACQUIRE_MALLOC_GLOBAL_LOCK()
#define RELEASE_MALLOC_GLOBAL_LOCK()

#if defined(AZ_PLATFORM_ANDROID)
#define NO_MALLINFO 1
#endif //AZ_PLATFORM_ANDROID

#ifdef AZ_COMPILER_MSVC
#   pragma warning(push)
#   pragma warning(disable: 4702) // unreachable code (dlmalloc.inl has that with the current settings I rather not modify the code)
#   pragma warning(disable: 4267) // conversion from size_t to bindex_t in dlmalloc.h
#endif

#if defined(AZ_COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable: 4702) // unreachable code
#endif

#include "dlmalloc.inl"

#if defined(AZ_COMPILER_MSVC)
#pragma warning(pop)
#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

using namespace AZ;

//=========================================================================
// HeapSchema
// [9/2/2009]
//=========================================================================
HeapSchema::HeapSchema(const Descriptor& desc)
{
    m_capacity = 0;
    m_used = 0;

    m_desc = desc;
    m_subAllocator = 0;

    for (int i = 0; i < Descriptor::m_maxNumBlocks; ++i)
    {
        m_memSpaces[i] = 0;
        m_ownMemoryBlock[i] = false;
    }

    for (int i = 0; i < m_desc.m_numMemoryBlocks; ++i)
    {
        if (m_desc.m_memoryBlocks[i] == 0)  // Allocate memory block if requested!
        {
            AZ_Assert(AllocatorInstance<SystemAllocator>::IsReady(), "You requested to allocate memory using the system allocator, but it's not created yet!");
            m_subAllocator = &AllocatorInstance<SystemAllocator>::Get();
            m_desc.m_memoryBlocks[i] = azmalloc(m_desc.m_memoryBlocksByteSize[i], 16, SystemAllocator, "Heap schema");
            m_ownMemoryBlock[i] = true;
        }

        m_memSpaces[i] = AZDLMalloc::create_mspace_with_base(m_desc.m_memoryBlocks[i], m_desc.m_memoryBlocksByteSize[i], m_desc.m_isMultithreadAlloc);
        AZ_Assert(m_memSpaces[i], "MSpace with base creation failed!");
        AZDLMalloc::mspace_az_set_expandable(m_memSpaces[i], false);

        m_capacity += m_desc.m_memoryBlocksByteSize[i];
    }

    if (m_desc.m_numMemoryBlocks == 0)
    {
        // Create default memory space if we can to serve for default allocations
        m_memSpaces[0] = AZDLMalloc::create_mspace(0, m_desc.m_isMultithreadAlloc);
        if (m_memSpaces[0])
        {
            AZDLMalloc::mspace_az_set_expandable(m_memSpaces[0], true);
#if defined(AZ_PLATFORM_WINDOWS)
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            m_capacity = (char*)si.lpMaximumApplicationAddress - (char*)si.lpMinimumApplicationAddress;
#endif
        }
    }
}

//=========================================================================
// ~HeapSchema
// [9/2/2009]
//=========================================================================
HeapSchema::~HeapSchema()
{
    // Verify all memory is free.
    for (int i = 0; i < Descriptor::m_maxNumBlocks; ++i)
    {
        if (m_memSpaces[i])
        {
            AZDLMalloc::destroy_mspace(m_memSpaces[i]);
            m_memSpaces[i] = 0;

            if (m_ownMemoryBlock[i])
            {
                azfree(m_desc.m_memoryBlocks[i], SystemAllocator);
            }
        }
    }

    m_capacity = 0;
}

//=========================================================================
// Allocate
// [9/02/2009]
//=========================================================================
HeapSchema::pointer_type
HeapSchema::Allocate(size_type byteSize, size_type alignment, int flags, const char* name, const char* fileName, int lineNum, unsigned int suppressStackRecord)
{
    (void)name;
    (void)fileName;
    (void)lineNum;
    (void)suppressStackRecord;
    int blockId = flags;
    AZ_Assert(m_memSpaces[blockId]!=0, "Invalid block id!");
    HeapSchema::pointer_type address = AZDLMalloc::mspace_memalign(m_memSpaces[blockId], alignment, byteSize);
    if (address)
    {
        m_used += ChunckSize(address);
    }
    return address;
}

//=========================================================================
// DeAllocate
// [9/2/2009]
//=========================================================================
void
HeapSchema::DeAllocate(pointer_type ptr, size_type byteSize, size_type alignment)
{
    (void)byteSize;
    (void)alignment;
    if (ptr==0)
    {
        return;
    }

    // if we use m_spaces just count the chunk sizes.
    m_used -= ChunckSize(ptr);
#   ifdef FOOTERS
    AZDLMalloc::mspace_free(0, ptr);                        ///< We use footers so we know which memspace the pointer belongs to.
#   else
    int i = 0;
    for (; i < m_desc.m_numMemoryBlocks; ++i)
    {
        void* start = m_desc.m_memoryBlocks[i];
        void* end = (char*)start + m_desc.m_memoryBlocksByteSize[i];
        if (ptr >= start && ptr <= end)
        {
            break;
        }
    }
    //AZ_Assert(i<m_desc.m_numMemoryBlocks,"Memory address 0x%08x is not in any of memory blocks!",ptr);
    AZDLMalloc::mspace_free(m_memSpaces[i], ptr);
#   endif
}

//=========================================================================
// AllocationSize
// [11/22/2010]
//=========================================================================
HeapSchema::size_type
HeapSchema::AllocationSize(pointer_type ptr)
{
    return AZDLMalloc::dlmalloc_usable_size(ptr);
}

//=========================================================================
// GetMaxAllocationSize
// [12/2/2010]
//=========================================================================
HeapSchema::size_type
HeapSchema::GetMaxAllocationSize() const
{
    size_type maxChunk = 0;
    //if(memoryBlock<0)
    {
        for (int i = 0; i < Descriptor::m_maxNumBlocks; ++i)
        {
            if (m_memSpaces[i])
            {
                size_type curMaxChunk = AZDLMalloc::mspace_az_get_max_free_chuck(m_memSpaces[i]);
                if (curMaxChunk>maxChunk)
                {
                    maxChunk = curMaxChunk;
                }
            }
        }
    }
    /*else if(memoryBlock < Descriptor::m_maxNumBlocks )
    {
        if( m_memSpaces[memoryBlock] )
            maxChunk = AZDLMalloc::mspace_az_get_max_free_chuck(m_memSpaces[memoryBlock]);
    }*/

    return maxChunk;
}

//=========================================================================
// ChunckSize
// [11/24/2010]
//=========================================================================
AZ_FORCE_INLINE HeapSchema::size_type
HeapSchema::ChunckSize(pointer_type ptr)
{
    // based on azmalloc_usable_size + the overhead
    if (ptr != 0)
    {
        mchunkptr p = mem2chunk(ptr);
        //if (is_inuse(p))  // we can even skip this check since we track for double free and so on anyway
        return chunksize(p);
    }
    return 0;
}

// Clear all DLMALLOC defines
#undef USE_LOCKS
#undef MLOCK_T
#undef INITIAL_LOCK
#undef ACQUIRE_LOCK
#undef RELEASE_LOCK
#undef TRY_LOCK

#undef ONLY_MSPACES
#undef MSPACES
#undef HAVE_MMAP
#undef FOOTERS
#undef DEBUG
#undef USE_DL_PREFIX

#undef CURRENT_THREAD
#undef LACKS_SYS_TYPES_H

#endif // #ifndef AZ_UNITY_BUILD
