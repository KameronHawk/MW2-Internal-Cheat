#include "game.h"



namespace Game {
   
};










HWND hwnd = 0;

HWND process::GetProcessWindow()
{
    if (hwnd)
        return hwnd;

    EnumWindows(EnumWindowCallBack, GetCurrentProcessId());

    if (hwnd == NULL)
        return 0;

    return hwnd;
}

BOOL process::EnumWindowCallBack(HWND hWnd, LPARAM lParam)
{
    DWORD dwPid = 0;
    GetWindowThreadProcessId(hWnd, &dwPid);
    if (dwPid == lParam)
    {
        hwnd = hWnd;
        return FALSE;
    }
    return TRUE;
}