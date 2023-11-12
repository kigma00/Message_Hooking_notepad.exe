#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define DEF_DLL_NAME L"KeyHook.dll"
#define DEF_HOOKSTART "HookStart"
#define DEF_HOOKSTOP "HookStop"

typedef void (*PFN_HOOKSTART)();
typedef void (*PFN_HOOKSTOP)();

int main(void) {
    HINSTANCE hDll = NULL;
    PFN_HOOKSTART HookStart = NULL;
    PFN_HOOKSTOP HookStop = NULL;
    char ch = 0;

    // KeyHook.dll 로딩
    hDll = LoadLibrary(DEF_DLL_NAME);

    if (hDll == NULL) {
        printf("Failed to load DLL.\n");
        return 1;
    }

    // Export Function Address
    HookStart = (PFN_HOOKSTART)GetProcAddress(hDll, DEF_HOOKSTART);
    HookStop = (PFN_HOOKSTOP)GetProcAddress(hDll, DEF_HOOKSTOP);

    if (HookStart == NULL || HookStop == NULL) {
        printf("Failed to get function addresses.\n");
        FreeLibrary(hDll);
        return 1;
    }

    // Hooking Start 함수 호출
    HookStart();

    printf("Press 'q' to quit.\n");

    // Wait until User input 'q'
    while (_getch() != 'q');

    // Hooking Stop 함수 호출
    HookStop();

    // KeyHook.dll 언로딩
    FreeLibrary(hDll);
}
