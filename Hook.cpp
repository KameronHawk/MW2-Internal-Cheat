#include "Hook.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                       FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef BOOL (__fastcall* IsWindowedInternal)(IDXGISwapChain3* pSwapChain);
IsWindowedInternal g_OriginalIsMaximized = nullptr;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BYTE OriginalBytes[12];
BYTE Shellcode[12] = { 0x48, 0xB8, 0x00, 0x00, 0xBE, 0xBA, 0xEF, 0xBE, 0xAD, 0xDE, 0xFF, 0xE0 };

uintptr_t Hooks::HookedFunc = 0;
uintptr_t Hooks::OriginalFunc = 0;
bool IsInitialized = false;

BOOL __fastcall hkIsWindowedInternal(IDXGISwapChain3* pSwapChain) {

    if (!IsInitialized) {
        Globals::gWindow = Draw::MyGetWindow();//"Call of Duty® HQ" "gfx_test"

        if (!Globals::gWindow) {
            return 0;
        }


        if (!SUCCEEDED((Globals::pSwapChain = pSwapChain)->GetDevice(__uuidof(ID3D12Device), (void**)&Globals::pD3DDevice))) {
            return 0;
        }
        if (!Draw::InitD3d12()) {
            return 0;
        }
        Draw::OriginalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW((HWND)Globals::gWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Draw::MyWndProc)));
        
        IsInitialized = true;
    }

    


    Draw::Begin();
    Draw::vDrawText("test", { 0, 0 }, 18.0f, { 255, 255, 255, 255 }, false);
    

    Draw::End();


    return FALSE;
}



BOOL Hooks::InitHooks() {
    // hook present scene
    auto sModName = skCrypt(L"dxgi.dll");
    uintptr_t DXGIBase = (uintptr_t)GetModuleBase(sModName);
    sModName.clear();

    if (!DXGIBase) {
        return FALSE;
    }

    //*reinterpret_cast<PVOID*>(&Shellcode[2]) = &hkPresentImplCore;


    void* IsMaximizedAddr = (void*)pattern_scan(DXGIBase, skCrypt("40 53 48 83 EC 20 33 DB 39"));

    


    if (VEHHook((uintptr_t)IsMaximizedAddr, (uintptr_t)hkIsWindowedInternal)) {
        return true;
    }

    





    // hook screenshot functions here



    return TRUE;
}


LONG WINAPI Hooks::ExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo) {

    if (ExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION) {

        if (ExceptionInfo->ContextRecord->Rip == (uintptr_t)Hooks::OriginalFunc) {
            ExceptionInfo->ContextRecord->Rip = (uintptr_t)Hooks::HookedFunc;
        }
        ExceptionInfo->ContextRecord->EFlags |= 0x100;

        return EXCEPTION_CONTINUE_EXECUTION;

    }

    if (ExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP) {
        DWORD old;
        VirtualProtect((PVOID)OriginalFunc, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &old);


        return EXCEPTION_CONTINUE_EXECUTION;
    }

    return EXCEPTION_CONTINUE_SEARCH;
}



BOOL Hooks::VEHHook(uintptr_t FuncAddressToHook, uintptr_t MyFuncHook) {

    Hooks::HookedFunc = MyFuncHook;
    Hooks::OriginalFunc = FuncAddressToHook;

    PVOID VEHHandle = AddVectoredExceptionHandler(2, (PVECTORED_EXCEPTION_HANDLER)ExceptionHandler);

    // setup page to READONLY so anytime the func executes we snag the exception and point to our func!
    DWORD OldProtection = 0;
    if (!VEHHandle || !VirtualProtect((PVOID)FuncAddressToHook, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &OldProtection)) {
        
        return FALSE;
    }




    return TRUE;
}
