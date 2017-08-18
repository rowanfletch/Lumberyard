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

#ifndef CRYINCLUDE_CRYCOMMON_IGAME_H
#define CRYINCLUDE_CRYCOMMON_IGAME_H
#pragma once

#define LY_SAVEGAME_FILENAME "LUMBERYARD"
#define LY_SAVEGAME_FILE_EXT ".sav"

#include <ICmdLine.h>
#include <INetwork.h>
#include <ITestSystem.h> // <> required for Interfuscator

struct IAIActorProxy;
struct IGameFramework;
struct IGameStateRecorder;
struct IGameAudio;
struct IGameWarningsListener;
struct IAntiCheatManager;
//struct ITestManager;
struct SGameStartParams;
struct SRenderingPassInfo;
struct IGamePhysicsSettings;
struct IGameToEditorInterface;

// Summary
//      Main interface used for the game central object
// Description
//      The IGame interface should be implemented in the GameDLL. Game systems
//      residing in the GameDLL can be initialized and updated inside the Game
//      object.
// See Also
//      IEditorGame
struct IGame
{
    struct ExportFilesInfo
    {
        ExportFilesInfo(const char* _baseFileName, const uint32 _fileCount)
            : m_pBaseFileName(_baseFileName)
            , m_fileCount(_fileCount)
        {
        }

        ILINE uint32 GetFileCount() const { return m_fileCount; }
        ILINE const char*  GetBaseFileName() const { return m_pBaseFileName; }

        static void GetNameForFile(const char* baseFileName, const uint32 fileIdx, char* outputName, const int outputSize)
        {
            assert(baseFileName != NULL);
            sprintf_s(outputName, outputSize, "%s_%d", baseFileName, fileIdx);
        }

    private:
        const char*     m_pBaseFileName;
        const uint32    m_fileCount;
    };

    // Summary
    //   Entry function used to initialize a new IGame of a GameMod
    // Parameters
    //   pGameFramework - a pointer to the game framework, from which a pointer of ISystem can be retrieved
    // Returns
    //   A pointer to the newly created IGame implementation
    typedef IGame*(* TEntryFunction)(IGameFramework* pGameFramework);

    // Summary
    //   Type to represent saved game names, keeping the string on the stack if possible.
    typedef CryStackStringT<char, 256> TSaveGameName;

    // <interfuscator:shuffle>
    virtual ~IGame(){}

    // Description:
    //      Initialize the MOD.
    //      The shutdown method, must be called independent of this method's return value.
    // Arguments:
    //      pCmdLine - Pointer to the command line interface.
    //      pFramework - Pointer to the IGameFramework interface.
    // Return Value:
    //      0 if something went wrong with initialization, non-zero otherwise.
    virtual bool Init(IGameFramework* pFramework) = 0;

    // Init editor related things
    virtual void InitEditor(IGameToEditorInterface* pGameToEditor) { }

    virtual void GetMemoryStatistics(ICrySizer* s) { }

    // Description:
    //      Finish initializing the MOD.
    //      Called after the game framework has finished its CompleteInit.
    //      This is the point at which to register game flow nodes etc.
    virtual bool CompleteInit() {return true; };

    // Description:
    //      Shuts down the MOD and delete itself.
    virtual void Shutdown() = 0;

    // Description:
    //      Notify game of pre-physics update.
    virtual void PrePhysicsUpdate() {}

    // Description:
    //      Updates the MOD.
    // Arguments:
    //      haveFocus - Boolean describing if the game has the input focus or not.
    // Return Value:
    //      0 to terminate the game (i.e. when quitting), non-zero to continue
    virtual int Update(bool haveFocus, unsigned int updateFlags) = 0;

    // Description:
    //      Called on the game when entering/exiting game mode in editor
    // Arguments:
    //      bStart - Boolean describing if we enter or exit game mode
    virtual void EditorResetGame(bool bStart) { }

    // Description:
    //      Called on the game when the local player id is set.
    // Arguments:
    //      playerId - The entity Id of the local player.
    virtual void PlayerIdSet(EntityId playerId) = 0;

    // Description:
    //      Returns the name of the mode. (i.e.: "Capture The Flag")
    // Return Value:
    //      The name of the mode. (i.e.: "Capture The Flag")
    virtual const char* GetLongName() = 0;

    // Description:
    //      Returns a short description of the mode. (i.e.: dc)
    // Return Value:
    //      A short description of the mode. (i.e.: dc)
    virtual const char* GetName() = 0;

    // Description:
    //      Loads a specified action map, used mainly for loading the default action map
    // Return Value:
    //      Void
    virtual void LoadActionMaps(const char* filename) = 0;

    // Description:
    //      Called when playerID is reset in GameChannel
    // Return Value:
    //      Void
    virtual void OnClearPlayerIds() { }

    // Description:
    //      Retrieves the local player entity Id.
    // Return Value:
    //      Void
    virtual EntityId GetClientActorId() const = 0;

    // Description:
    //      Auto-Savegame name creation
    // Return Value:
    //      c_str or NULL
    virtual IGame::TSaveGameName CreateSaveGameName() { return NULL; }

    // Description:
    //      Returns a pointer to the game framework being used.
    // Return Value:
    //      Pointer to the game framework being used.
    virtual IGameFramework* GetIGameFramework() = 0;

    // Description:
    //      Mapping level filename to "official" name.
    // Return Value:
    //      c_str or NULL
    virtual const char* GetMappedLevelName(const char* levelName) const { return nullptr; }

    // Description:
    //    Returns the anticheat system interface
    virtual IAntiCheatManager* GetAntiCheatManager() { return nullptr; }

    // Description:
    //      Query whether an initial levelstart savegame should be created.
    // Return Value:
    //      true or false
    virtual const bool DoInitialSavegame() const { return true; }

    // Description:
    //      Add a game warning that is shown to the player
    // Return Value:
    //    A unique handle to the warning or 0 for any error.
    virtual uint32 AddGameWarning(const char* stringId, const char* paramMessage, IGameWarningsListener* pListener = NULL) { return 0; }

    // Description:
    //      Called from 3DEngine in RenderScene, so polygons and meshes can be added to the scene from game
    virtual void OnRenderScene(const SRenderingPassInfo& passInfo) { }

    // Description
    //      Render Game Warnings
    virtual void RenderGameWarnings() { }

    // Description:
    //      Remove a game warning
    virtual void RemoveGameWarning(const char* stringId) { }

    // Description:
    //      callback to game for game specific actions on level end
    // Return Value:
    //      false, if the level end should continue
    //      true, if the game handles the end level action and calls ScheduleEndLevel directly
    virtual bool GameEndLevel(const char* stringId) { return false; }

    virtual void SetUserProfileChanged(bool yesNo) { }

    //    creates a GameStateRecorder instance in GameDll and passes its ownership to the caller (CryAction/GamePlayRecorder)
    virtual IGameStateRecorder* CreateGameStateRecorder(IGameplayListener* pL) { return nullptr; }

    virtual void FullSerialize(TSerialize ser) { }
    virtual void PostSerialize() { }

    // Description
    // Editor export interface hook, to allow the game to export its own data into the level paks
    // Return: Exported file information
    virtual IGame::ExportFilesInfo ExportLevelData(const char* levelName, const char* missionName) const { return ExportFilesInfo("NoName", 0); }

    // Description
    // Interface hook to load all game exported data when the level is loaded
    virtual void LoadExportedLevelData(const char* levelName, const char* missionName) { }

    // Description:
    //      called by FlowSystem to register all game specific flow nodes
    virtual void RegisterGameFlowNodes() { }

    // Description:
    //      Access to game interface
    virtual void* GetGameInterface() { return nullptr; }
};

#endif // CRYINCLUDE_CRYCOMMON_IGAME_H
