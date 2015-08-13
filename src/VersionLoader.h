#pragma once

#ifndef UNICODE
	#define UNICODE
#endif

#include <Windows.h>
#define dll_export extern "C" __declspec(dllexport) __declspec(naked) void __cdecl

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

HINSTANCE hInstance;
HMODULE hVersion;

void initLibrary();
void exitLibrary();

FARPROC fn_GetFileVersionInfoA;
FARPROC fn_GetFileVersionInfoByHandle;
FARPROC fn_GetFileVersionInfoExW;
FARPROC fn_GetFileVersionInfoSizeA;
FARPROC fn_GetFileVersionInfoSizeExW;
FARPROC fn_GetFileVersionInfoSizeW;
FARPROC fn_GetFileVersionInfoW;
FARPROC fn_VerFindFileA;
FARPROC fn_VerFindFileW;
FARPROC fn_VerInstallFileA;
FARPROC fn_VerInstallFileW;
FARPROC fn_VerLanguageNameA;
FARPROC fn_VerLanguageNameW;
FARPROC fn_VerQueryValueA;
FARPROC fn_VerQueryValueW;

