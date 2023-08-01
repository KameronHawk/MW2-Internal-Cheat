#pragma once
#include <Windows.h>
#include <dxgi1_4.h>
#include "Utils.h"
#include "game.h"
#include "features.h"
BOOL __fastcall hkIsWindowedInternal(IDXGISwapChain3* pSwapChain);





class Hooks {
private:
	static uintptr_t OriginalFunc;
	static uintptr_t HookedFunc;

public:
	static LONG WINAPI ExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo);

	BOOL InitHooks();

	BOOL VEHHook(uintptr_t FuncAddressToHook, uintptr_t MyHookedFunc);



};










