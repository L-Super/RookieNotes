借助 Windows API 实现：
```cpp
#include <Windows.h> 
#include <TlHelp32.h>
  
// 判断进程是否存在/运行
bool isProcessRunning(const char* processName) {  
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
    if (snapshot != INVALID_HANDLE_VALUE) {  
        PROCESSENTRY32 processEntry;  
        processEntry.dwSize = sizeof(PROCESSENTRY32);  
        if (Process32First(snapshot, &processEntry)) {  
            do {  
                if (strcmp(processEntry.szExeFile, processName) == 0) {  
                    CloseHandle(snapshot);  
                    return true;  
                }  
            } while (Process32Next(snapshot, &processEntry));  
        }  
        CloseHandle(snapshot);  
    }  
    return false;  
}
```

注意，这里的 `#include <TlHelp32.h>` 头文件必须在 `#include <Windows.h>` 之后