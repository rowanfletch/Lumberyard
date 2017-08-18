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
#ifndef CRYINCLUDE_CRYCOMMON_ICONSOLEMOCK_H
#define CRYINCLUDE_CRYCOMMON_ICONSOLEMOCK_H
#pragma once

#include <IRemoteCommand.h>
#include <IConsole.h>
#include <AzTest/AzTest.h>

#pragma warning( push )
#pragma warning( disable: 4373 )      // virtual function overrides differ only by const/volatile qualifiers, mock issue


// Implements the Console interface
class ConsoleMock
    : public IConsole
{
public:
    MOCK_METHOD0(Release, void());
    MOCK_METHOD1(Init, void(ISystem * pSystem));
    MOCK_METHOD5(RegisterString, ICVar * (const char* sName, const char* sValue, int nFlags, const char* help, ConsoleVarFunc pChangeFunc));
    MOCK_METHOD5(RegisterInt, ICVar * (const char* sName, int iValue, int nFlags, const char* help, ConsoleVarFunc pChangeFunc));
    MOCK_METHOD5(RegisterInt64, ICVar * (const char* sName, int64 iValue, int nFlags, const char* help, ConsoleVarFunc pChangeFunc));
    MOCK_METHOD5(RegisterFloat, ICVar * (const char* sName, float fValue, int nFlags, const char* help, ConsoleVarFunc pChangeFunc));
    MOCK_METHOD7(Register, ICVar * (const char* name, float* src, float defaultvalue, int nFlags, const char* help, ConsoleVarFunc pChangeFunc, bool allowModify));
    MOCK_METHOD7(Register, ICVar * (const char* name, int* src, int defaultvalue, int nFlags, const char* help, ConsoleVarFunc pChangeFunc, bool allowModify));
    MOCK_METHOD7(Register, ICVar * (const char* name, const char** src, const char* defaultvalue, int nFlags, const char* help, ConsoleVarFunc pChangeFunc, bool allowModify));
    MOCK_METHOD1(Register, ICVar * (ICVar * pVar));
    MOCK_METHOD2(UnregisterVariable, void(const char* sVarName, bool bDelete ));
    MOCK_METHOD1(SetScrollMax, void(int value));
    MOCK_METHOD1(AddOutputPrintSink, void(IOutputPrintSink * inpSink));
    MOCK_METHOD1(RemoveOutputPrintSink, void(IOutputPrintSink * inpSink));
    MOCK_METHOD2(ShowConsole, void(bool show, const int iRequestScrollMax));
    MOCK_METHOD2(DumpCVars, void(ICVarDumpSink * pCallback, unsigned int nFlagsFilter));
    MOCK_METHOD2(CreateKeyBind, void(const char* sCmd, const char* sRes));
    MOCK_METHOD2(SetImage, void (ITexture * pImage, bool bDeleteCurrent));
    MOCK_METHOD0(GetImage, ITexture * ());
    MOCK_METHOD1(StaticBackground, void (bool bStatic));
    MOCK_METHOD1(SetLoadingImage, void(const char* szFilename));
    MOCK_CONST_METHOD3(GetLineNo, bool(const int indwLineNo, char* outszBuffer, const int indwBufferSize));
    MOCK_CONST_METHOD0(GetLineCount, int ());
    MOCK_METHOD1(GetCVar, ICVar * (const char* name));
    MOCK_METHOD3(GetVariable, char*(const char* szVarName, const char* szFileName, const char* def_val));
    MOCK_METHOD3(GetVariable, float (const char* szVarName, const char* szFileName, float def_val));
    MOCK_METHOD1(PrintLine, void (const char* s));
    MOCK_METHOD1(PrintLinePlus, void (const char* s));
    MOCK_METHOD0(GetStatus, bool());
    MOCK_METHOD0(Clear, void());
    MOCK_METHOD0(Update, void());
    MOCK_METHOD0(Draw, void());
    MOCK_METHOD4(AddCommand, bool (const char* sCommand, ConsoleCommandFunc func, int nFlags, const char* sHelp));
    MOCK_METHOD4(AddCommand, bool (const char* sName, const char* sScriptFunc, int nFlags, const char* sHelp));
    MOCK_METHOD1(RemoveCommand, void (const char* sName));
    MOCK_METHOD3(ExecuteString, void (const char* command, const bool bSilentMode, const bool bDeferExecution ));
    MOCK_METHOD0(IsOpened, bool ());
    MOCK_METHOD0(GetNumVars, int());
    MOCK_METHOD0(GetNumVisibleVars, int());
    MOCK_METHOD3(GetSortedVars, size_t (const char** pszArray, size_t numItems, const char* szPrefix));
    MOCK_METHOD1(AutoComplete, const char*(const char* substr));
    MOCK_METHOD1(AutoCompletePrev, const char*(const char* substr));
    MOCK_METHOD1(ProcessCompletion, const char*(const char* szInputBuffer));
    MOCK_METHOD2(RegisterAutoComplete, void (const char* sVarOrCommand, IConsoleArgumentAutoComplete * pArgAutoComplete));
    MOCK_METHOD1(UnRegisterAutoComplete, void (const char* sVarOrCommand));
    MOCK_METHOD0(ResetAutoCompletion, void ());
    MOCK_CONST_METHOD1(GetMemoryUsage, void (ICrySizer * pSizer));
    MOCK_METHOD1(ResetProgressBar, void (int nProgressRange));
    MOCK_METHOD0(TickProgressBar, void ());
    MOCK_METHOD1(SetInputLine, void (const char* szLine));
    MOCK_METHOD1(DumpKeyBinds, void (IKeyBindDumpSink * pCallback));
    MOCK_CONST_METHOD1(FindKeyBind, const char*(const char* sCmd));
    MOCK_METHOD0(GetNumCheatVars, int ());
    MOCK_METHOD2(SetCheatVarHashRange, void (size_t firstVar, size_t lastVar));
    MOCK_METHOD0(CalcCheatVarHash, void ());
    MOCK_METHOD0(IsHashCalculated, bool ());
    MOCK_METHOD0(GetCheatVarHash, uint64 ());
    MOCK_METHOD1(PrintCheatVars, void (bool bUseLastHashRange));
    MOCK_METHOD1(GetCheatVarAt, char* (uint32 nOffset));
    MOCK_METHOD1(AddConsoleVarSink, void (IConsoleVarSink * pSink));
    MOCK_METHOD1(RemoveConsoleVarSink, void (IConsoleVarSink * pSink));
    MOCK_METHOD1(GetHistoryElement, const char*(const bool bUpOrDown));
    MOCK_METHOD1(AddCommandToHistory, void (const char* szCommand));
    MOCK_METHOD2(LoadConfigVar, void (const char* sVariable, const char* sValue));
    MOCK_METHOD1(EnableActivationKey, void (bool bEnable));
#if defined(DEDICATED_SERVER)
    MOCK_METHOD2(SetClientDataProbeString, void (const char* pName, const char* pValue));
#endif

    // can't mock variadic methods, so just override here.
    void Exit(const char* command, ...) override {};
};

#pragma warning( pop )
#endif // CRYINCLUDE_CRYCOMMON_ICONSOLEMOCK_H
