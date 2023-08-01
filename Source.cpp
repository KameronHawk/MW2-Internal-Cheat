#include "game.h"
#include "Utils.h"
#include "crypt.h"
#include "Hook.h"




BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: { 
        Hooks Hook;

        if (Hook.InitHooks())
            return 129;

        
        
    }
        
    }
    return 129;
}