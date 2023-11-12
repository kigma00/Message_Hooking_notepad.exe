#include "pch.h"
#include <stdio.h>
#include <windows.h>

#define DEF_PROCESS_NAME L"notepad.exe"

HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
HWND g_hWnd = NULL;

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hInstance = hinstDLL;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    WCHAR szPath[MAX_PATH] = { 0 };

    if (nCode >= 0)
    {
        if (!(lParam & 0x80000000))
        {
            GetModuleFileNameW(NULL, szPath, MAX_PATH);
            WCHAR* p = wcsrchr(szPath, L'\\');
            if (!_wcsicmp(p + 1, DEF_PROCESS_NAME))
                return 1;
        }
    }
    // 다른 프로세스에서 발생한 키 입력에 대해서는 아무 조치도 취하지 않음.
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllexport) void HookStart()
{
    g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hInstance, 0);
}
__declspec(dllexport) void HookStop()
{
    if (g_hHook)
    {
        UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
    }
}
#ifdef __cplusplus
}
#endif
