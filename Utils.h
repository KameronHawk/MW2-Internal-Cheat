#pragma once
#include <vector>
#include <iostream>
#include "undocumented.h"

#pragma warning(disable : 4996)


uintptr_t pattern_scan(uintptr_t module_base, const char* signature);

PVOID GetModuleBase(const wchar_t* ModuleName);


