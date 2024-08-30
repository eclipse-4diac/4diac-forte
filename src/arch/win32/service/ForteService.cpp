/****************************** Module Header ******************************\
* Module Name:  SampleService.cpp
* Project:      CppWindowsService
* Copyright (c) Microsoft Corporation.
*
* 2024, Ketut Kumajaya - modified for FORTE to run as a Windows service
*
* Provides a sample service class that derives from the service base class -
* CServiceBase. The sample service logs the service start and stop
* information to the Application event log, and shows how to run the main
* function of the service in a thread pool worker thread.
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
#include "ForteService.h"
#include "ThreadPool.h"
#pragma endregion


#include <sstream>
#include "../../devlog.h"
#include "../../forte_stringFunctions.h"

void endForte(int paSig);
int _main(int argc, char *arg[]);


CForteService::CForteService(PWSTR pszServiceName,
                               int argc,
                               char *argv[],
                               BOOL fCanStop,
                               BOOL fCanShutdown,
                               BOOL fCanPauseContinue)
: CServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
{
    m_fStopping = FALSE;
    m_argc = argc;
    m_argv = argv;

    // Create a manual-reset event that is not signaled at first to indicate
    // the stopped signal of the service.
    m_hStoppedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_hStoppedEvent == NULL)
    {
        throw GetLastError();
    }
}


CForteService::~CForteService(void)
{
    if (m_hStoppedEvent)
    {
        CloseHandle(m_hStoppedEvent);
        m_hStoppedEvent = NULL;
    }
}


//
//   FUNCTION: CForteService::OnStart(DWORD, LPWSTR *)
//
//   PURPOSE: The function is executed when a Start command is sent to the
//   service by the SCM or when the operating system starts (for a service
//   that starts automatically). It specifies actions to take when the
//   service starts. In this code sample, OnStart logs a service-start
//   message to the Application log, and queues the main service function for
//   execution in a thread pool worker thread.
//
//   PARAMETERS:
//   * dwArgc   - number of command line arguments
//   * lpszArgv - array of command line arguments
//
//   NOTE: A service application is designed to be long running. Therefore,
//   it usually polls or monitors something in the system. The monitoring is
//   set up in the OnStart method. However, OnStart does not actually do the
//   monitoring. The OnStart method must return to the operating system after
//   the service's operation has begun. It must not loop forever or block. To
//   set up a simple monitoring mechanism, one general solution is to create
//   a timer in OnStart. The timer would then raise events in your code
//   periodically, at which time your service could do its monitoring. The
//   other solution is to spawn a new thread to perform the main service
//   functions, which is demonstrated in this code sample.
//
void CForteService::OnStart(DWORD, LPWSTR*)
{
    // Log command line arguments
    std::ostringstream stream;
    for (int i = 0; i < m_argc; ++i)
    {
        stream << " " << m_argv[i];
    }

    // Log a service start message to the Application log.
    WriteEventLogEntry((PWSTR)(forte_stringToWstring(("FORTE service started:" + stream.str())).c_str()),
        EVENTLOG_INFORMATION_TYPE);

    // Queue the main service function for execution in a worker thread.
    CThreadPool::QueueUserWorkItem(&CForteService::ServiceWorkerThread, this);
}


//
//   FUNCTION: CForteService::ServiceWorkerThread(void)
//
//   PURPOSE: The method performs the main function of the service. It runs
//   on a thread pool worker thread.
//
void CForteService::ServiceWorkerThread(void)
{
    // Periodically check if the service is stopping.
    while (!m_fStopping)
    {
        // Log a prosess start message
        DEVLOG_INFO("FORTE process start\n");

        // Perform main service function here...
        _main(m_argc, m_argv);

        // Log a prosess stop message
        DEVLOG_INFO("FORTE process end\n");

        ::Sleep(2000);  // Simulate some lengthy operations.
    }

    // Signal the stopped event.
    SetEvent(m_hStoppedEvent);
}


//
//   FUNCTION: CForteService::OnStop(void)
//
//   PURPOSE: The function is executed when a Stop command is sent to the
//   service by SCM. It specifies actions to take when a service stops
//   running. In this code sample, OnStop logs a service-stop message to the
//   Application log, and waits for the finish of the main service function.
//
//   COMMENTS:
//   Be sure to periodically call ReportServiceStatus() with
//   SERVICE_STOP_PENDING if the procedure is going to take long time.
//
void CForteService::OnStop()
{
    // Log a service stop message to the Application log.
    WriteEventLogEntry((PWSTR)L"FORTE service stopped",
        EVENTLOG_INFORMATION_TYPE);

    // Indicate that the service is stopping and wait for the finish of the
    // main service function (ServiceWorkerThread).
    m_fStopping = TRUE;

    // Kill FORTE process
    endForte(0);

    if (WaitForSingleObject(m_hStoppedEvent, INFINITE) != WAIT_OBJECT_0)
    {
        throw GetLastError();
    }
}