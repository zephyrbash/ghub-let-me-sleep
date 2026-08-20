#include <windows.h>
#include <tlhelp32.h>
#include <powrprof.h>
#include <stdio.h>
#include <string.h>

static int kill_process() {
    // Get a snapshot of all processes
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    // Create an entry of all processes
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    // If there are no processes, just return
    if (!Process32First(snapshot, &processEntry)) {
        CloseHandle(snapshot);
        return 0;
    }

    int killed = 0;

    do {
        // const char *process_name = "WindowsTerminal.exe"; // <- easy to test in the VM
        const char *process_name = "lghub_agent.exe";
        if (_stricmp(processEntry.szExeFile, process_name) == 0) {
            HANDLE process = OpenProcess(
                PROCESS_TERMINATE,
                FALSE,
                processEntry.th32ProcessID
                );

            if (process) {
                if (TerminateProcess(process, 0)) {
                    printf("Killed %s (PID %lu)\n",
                        processEntry.szExeFile,
                        processEntry.th32ProcessID);

                    killed = 1;
                }

                CloseHandle(process);
            }
        }
    } while (Process32Next(snapshot, &processEntry));

    CloseHandle(snapshot);
    return killed;
}

int main(void) {
    kill_process();

    Sleep(1000);

    // Put the system to sleep
    if (!SetSuspendState(FALSE, FALSE, FALSE)) {
        fprintf(stderr, "Failed to put the system to sleep\n");
        return 1;
    }

    return 0;
}
