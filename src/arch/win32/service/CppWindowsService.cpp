/****************************** Module Header ******************************\
* Module Name:  CppWindowsService.cpp
* Project:      CppWindowsService
* Copyright (c) Microsoft Corporation.
*
* 2024, Ketut Kumajaya - modified for FORTE to run as a Windows service
*
* The file defines the entry point of the application. According to the
* arguments in the command line, the function installs or uninstalls or
* starts the service by calling into different routines.
*
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/resources/licenses.aspx#MPL.
* All other rights reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma region Includes
#include <cstdio>
#include <windows.h>
#include "ServiceInstaller.h"
#include "ServiceBase.h"
#include "SampleService.h"
#pragma endregion


#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include "../../devlog.h"
#include "../../forte_stringFunctions.h"

int _main(int argc, char *arg[]);


//
// Settings of the service
//

// Internal name of the service
#define SERVICE_NAME             (PWSTR)L"4diac-forte"

// Displayed name of the service
#define SERVICE_DISPLAY_NAME     (PWSTR)L"4diac FORTE Runtime for Windows"

// Service start options.
#define SERVICE_START_TYPE       SERVICE_DEMAND_START

// List of service dependencies - "dep1\0dep2\0\0"
#define SERVICE_DEPENDENCIES     (PWSTR)L""

// The name of the account under which the service should run
#define SERVICE_ACCOUNT          (PWSTR)L"LocalSystem"

// The password to the service account name
#define SERVICE_PASSWORD         NULL


//
//  FUNCTION: wmain(int, wchar_t *[])
//
//  PURPOSE: entrypoint for the application.
//
//  PARAMETERS:
//    argc - number of command line arguments
//    argv - array of command line arguments
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//    wmain() either performs the command line task, or run the service.
//
int wmain(int argc, wchar_t *argv[])
{
    int ret = -1;

    // User can edit forte command line arguments in
    // HKLM\SYSTEM\CurrentControlSet\Services\4diac-forte\ImagePath

    // Reconstruct command line arguments from wide to narrow string
    std::vector<char*> l_argv;
    for (int i = 0; i < argc; ++i)
    {
        std::string str = forte_wstringToString(argv[i]);
        char* buf = new char[str.length() + 1];
        std::strcpy(buf, str.c_str());
        l_argv.push_back(buf);
    }
    l_argv.push_back(nullptr); // argv[argc] is this null pointer

    if ((argc > 1) && ((*argv[1] == L'-' || (*argv[1] == L'/'))))
    {
        if (_wcsicmp(L"install", argv[1] + 1) == 0)
        {
            // Install the service when the command is
            // "-install" or "/install".
            InstallService(
                SERVICE_NAME,               // Name of service
                SERVICE_DISPLAY_NAME,       // Name to display
                SERVICE_START_TYPE,         // Service start type
                SERVICE_DEPENDENCIES,       // Dependencies
                SERVICE_ACCOUNT,            // Service running account
                SERVICE_PASSWORD            // Password of the account
                );
        }
        else if (_wcsicmp(L"remove", argv[1] + 1) == 0)
        {
            // Uninstall the service when the command is
            // "-remove" or "/remove".
            UninstallService(SERVICE_NAME);
        }
        else if (_wcsicmp(L"service", argv[1] + 1) == 0 || _wcsicmp(L"s", argv[1] + 1) == 0)
        {
            // Start setup stdout/stderr redirection
            wchar_t szPath[MAX_PATH];
            if (GetModuleFileNameW(NULL, szPath, ARRAYSIZE(szPath)) == 0) {
                DEVLOG_ERROR("GetModuleFileNameW failed w/err 0x%08lx\n", GetLastError());
            }

            std::ofstream coutfile;
            std::filesystem::path fullpath(forte_wstringToString(szPath));
            // Disable logging with "-service nolog" argument
            if (std::filesystem::exists(fullpath) && _wcsicmp(L"nolog", argv[2]) != 0) {
                coutfile.open(fullpath.replace_extension("log").string());
                std::cout.rdbuf(coutfile.rdbuf());
                std::cerr.rdbuf(coutfile.rdbuf());
            }
            else
            {
                // Disable logs completely
                std::cout.rdbuf(nullptr);
                std::cerr.rdbuf(nullptr);
            }

            SampleService service(SERVICE_DISPLAY_NAME, static_cast<int>(l_argv.size() - 1), l_argv.data());
            if (!CServiceBase::Run(service))
            {
                DEVLOG_ERROR("Service failed to run w/err 0x%08lx\n", GetLastError());
            }
        }
        else
        {
            ret = _main(static_cast<int>(l_argv.size() - 1), l_argv.data());
        }
    }
    else
    {
        ret = _main(static_cast<int>(l_argv.size() - 1), l_argv.data());
    }

    // Clear the command line arguments
    for(auto &buf : l_argv)
    {
        if (buf != nullptr)
        {
            delete[] buf;
        }
    }
    l_argv.clear();

    return ret;
}