#include "VersionLoader.h"
#include "DoPatch.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		initLibrary();
		break;

	case DLL_PROCESS_DETACH:
		exitLibrary();
		break;
	}
	return TRUE;
}

void initLibrary() {
	wchar_t * szDll = new wchar_t[MAX_PATH];

	GetSystemDirectory(szDll, MAX_PATH);
	wcscat_s(szDll, MAX_PATH, L"\\version.dll");
	hVersion = LoadLibrary(szDll);

	free(szDll);

	// If error, exit.
	if (!hVersion) ExitProcess(1);
	
#pragma region Restore API
	fn_GetFileVersionInfoA = GetProcAddress(hVersion, "GetFileVersionInfoA");
	fn_GetFileVersionInfoByHandle = GetProcAddress(hVersion, "GetFileVersionInfoByHandle");
	fn_GetFileVersionInfoExW = GetProcAddress(hVersion, "GetFileVersionInfoExW");
	fn_GetFileVersionInfoSizeA = GetProcAddress(hVersion, "GetFileVersionInfoSizeA");
	fn_GetFileVersionInfoSizeExW = GetProcAddress(hVersion, "GetFileVersionInfoSizeExW");
	fn_GetFileVersionInfoSizeW = GetProcAddress(hVersion, "GetFileVersionInfoSizeW");
	fn_GetFileVersionInfoW = GetProcAddress(hVersion, "GetFileVersionInfoW");
	fn_VerFindFileA = GetProcAddress(hVersion, "VerFindFileA");
	fn_VerFindFileW = GetProcAddress(hVersion, "VerFindFileW");
	fn_VerInstallFileA = GetProcAddress(hVersion, "VerInstallFileA");
	fn_VerInstallFileW = GetProcAddress(hVersion, "VerInstallFileW");
	fn_VerLanguageNameA = GetProcAddress(hVersion, "VerLanguageNameA");
	fn_VerLanguageNameW = GetProcAddress(hVersion, "VerLanguageNameW");
	fn_VerQueryValueA = GetProcAddress(hVersion, "VerQueryValueA");
	fn_VerQueryValueW = GetProcAddress(hVersion, "VerQueryValueW");
#pragma endregion

	do_patch(GetModuleHandle(NULL));
}

void exitLibrary() {
	FreeLibrary(hVersion);
}

#pragma region API Export
namespace {
	dll_export GetFileVersionInfoA(void) { __asm jmp fn_GetFileVersionInfoA }
	dll_export GetFileVersionInfoByHandle(void) { __asm jmp fn_GetFileVersionInfoByHandle }
	dll_export GetFileVersionInfoExW(void) { __asm jmp fn_GetFileVersionInfoExW }
	dll_export GetFileVersionInfoSizeA(void) { __asm jmp fn_GetFileVersionInfoSizeA }
	dll_export GetFileVersionInfoSizeExW(void) { __asm jmp fn_GetFileVersionInfoSizeExW }
	dll_export GetFileVersionInfoSizeW(void) { __asm jmp fn_GetFileVersionInfoSizeW }
	dll_export GetFileVersionInfoW(void) { __asm jmp fn_GetFileVersionInfoW }
	dll_export VerFindFileA(void) { __asm jmp fn_VerFindFileA }
	dll_export VerFindFileW(void) { __asm jmp fn_VerFindFileW }
	dll_export VerInstallFileA(void) { __asm jmp fn_VerInstallFileA }
	dll_export VerInstallFileW(void) { __asm jmp fn_VerInstallFileW }
	dll_export VerLanguageNameA(void) { __asm jmp fn_VerLanguageNameA }
	dll_export VerLanguageNameW(void) { __asm jmp fn_VerLanguageNameW }
	dll_export VerQueryValueA(void) { __asm jmp fn_VerQueryValueA }
	dll_export VerQueryValueW(void) { __asm jmp fn_VerQueryValueW }
}
#pragma endregion