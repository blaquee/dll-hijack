#pragma once

#include "Common.h"
#include "PatchUtil.h"
#include "DoPatch.h"

#define dll_export extern "C" __declspec(dllexport) void __cdecl

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

void initLibrary();
void exitLibrary();

HINSTANCE hInstance;
HMODULE hVersion;

namespace API_EXPORT
{
	dll_export GetFileVersionInfoA(void);
	dll_export GetFileVersionInfoByHandle(void);
	dll_export GetFileVersionInfoExW(void);
	dll_export GetFileVersionInfoSizeA(void);
	dll_export GetFileVersionInfoSizeExW(void);
	dll_export GetFileVersionInfoSizeW(void);
	dll_export GetFileVersionInfoW(void);
	dll_export VerFindFileA(void);
	dll_export VerFindFileW(void);
	dll_export VerInstallFileA(void);
	dll_export VerInstallFileW(void);
	dll_export VerLanguageNameA(void);
	dll_export VerLanguageNameW(void);
	dll_export VerQueryValueA(void);
	dll_export VerQueryValueW(void);
}

void fixAPI(LPCSTR lpProcName, uAddr a);