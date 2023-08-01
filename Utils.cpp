#include "Utils.h"

#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))


uintptr_t pattern_scan(uintptr_t module_base, const char* signature) {
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector < int >{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);
		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dosHeader = (PIMAGE_DOS_HEADER)module_base;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((uint8_t*)module_base + dosHeader->e_lfanew);

	auto size_of_image = ntHeaders->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte(signature);
	auto module_bytes = reinterpret_cast<uint8_t*>(module_base);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();
	auto CurrentSig = 0;
	int TimesFound = 0;
	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (module_bytes[i + j] != d[j] && d[j] != -1) {
				found = false; // if not found keep scanning
				break;
			}
		}
		if (found) {
			
			
			return (uintptr_t)&module_bytes[i];
		}
	}
	return NULL;
}

PVOID GetModuleBase(const wchar_t* ModuleName) {
	PPEB64 Peb = (PPEB64)__readgsqword(0x60);
	PEB_LDR_DATA* ldr = (PEB_LDR_DATA*)Peb->Ldr;
	LIST_ENTRY* list = &ldr->InMemoryOrderModuleList;
	LIST_ENTRY* entry = list->Flink;


	while (entry != list)
	{
		LDR_DATA_TABLE_ENTRY* Module = CONTAINING_RECORD(entry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);


		if (wcsstr(Module->FullDllName.Buffer, ModuleName)) {
			return Module->DllBase;
		}


		entry = entry->Flink;
	}
	return NULL;
}




