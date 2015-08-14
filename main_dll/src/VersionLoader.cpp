#include "VersionLoader.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		hInstance = hinstDLL;
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
	fixAPI("GetFileVersionInfoA", reinterpret_cast<uAddr>(API_EXPORT::GetFileVersionInfoA));
	fixAPI("GetFileVersionInfoByHandle", reinterpret_cast<uAddr>(API_EXPORT::GetFileVersionInfoByHandle));
	fixAPI("GetFileVersionInfoExW", reinterpret_cast<uAddr>(API_EXPORT::GetFileVersionInfoExW));
	fixAPI("GetFileVersionInfoSizeA", reinterpret_cast<uAddr>(API_EXPORT::GetFileVersionInfoSizeA));
	fixAPI("GetFileVersionInfoSizeExW", reinterpret_cast<uAddr>(API_EXPORT::GetFileVersionInfoSizeExW));
	fixAPI("GetFileVersionInfoSizeW", reinterpret_cast<uAddr>(API_EXPORT::GetFileVersionInfoSizeW));
	fixAPI("GetFileVersionInfoW", reinterpret_cast<uAddr>(API_EXPORT::GetFileVersionInfoW));
	fixAPI("VerFindFileA", reinterpret_cast<uAddr>(API_EXPORT::VerFindFileA));
	fixAPI("VerFindFileW", reinterpret_cast<uAddr>(API_EXPORT::VerFindFileW));
	fixAPI("VerInstallFileA", reinterpret_cast<uAddr>(API_EXPORT::VerInstallFileA));
	fixAPI("VerInstallFileW", reinterpret_cast<uAddr>(API_EXPORT::VerInstallFileW));
	fixAPI("VerLanguageNameA", reinterpret_cast<uAddr>(API_EXPORT::VerLanguageNameA));
	fixAPI("VerLanguageNameW", reinterpret_cast<uAddr>(API_EXPORT::VerLanguageNameW));
	fixAPI("VerQueryValueA", reinterpret_cast<uAddr>(API_EXPORT::VerQueryValueA));
	fixAPI("VerQueryValueW", reinterpret_cast<uAddr>(API_EXPORT::VerQueryValueW));
#pragma endregion

	do_patch(GetModuleHandle(NULL));
}

void exitLibrary() {
	FreeLibrary(hVersion);
}


void fixAPI(LPCSTR lpProcName, uAddr fnEntry)
{
	// In x64 bit system, 
	// When compile as DEBUG, it will generate code like jmp xxxx
	// Which is not long enough to hold a 64-bit address.

	// Where as in RELEASE mode, it will just pass the *real* address
	// to this function.
#if defined(_WIN64) && defined(_DEBUG)
	fnEntry = ReadRelativeAddress_x86_jmp(fnEntry);
#endif

	FARPROC fnDest = GetProcAddress(hVersion, lpProcName);
	if (fnEntry && fnDest)
	{
		DWORD oldProtect;
#ifdef _WIN64
#define SHELL_LENGTH 12
#else
#define SHELL_LENGTH 5
#endif
		VirtualProtect(reinterpret_cast<void *>(fnEntry), SHELL_LENGTH, PAGE_EXECUTE_READWRITE, &oldProtect);

#ifdef _WIN64
		// 00007FFD7709E251 | 48 B8 56 34 12 90 78 56 34 12 | mov rax, 1234567890123456 |
		// 00007FFD7709E25B | FF E0 | jmp rax |
		
		WriteUInt16(fnEntry, 0xB848);
		*reinterpret_cast<FARPROC *>(fnEntry + 2) = fnDest;

		// jmp rax
		WriteUInt16(fnEntry + 10, 0xE0FF);
#else
		// 773E3D66 | E9 0D 19 F6 9A | jmp 12345678 |
		WriteByte(fnEntry, 0xE9);
		WriteRelativeAddress(fnEntry + 1, reinterpret_cast<uAddr>(fnDest));
#endif
		VirtualProtect(reinterpret_cast<void *>(fnEntry), SHELL_LENGTH, oldProtect, &oldProtect);
	}
	else
	{
		int size_a = sizeof(lpProcName) + 1;
		int size_w = MultiByteToWideChar(CP_ACP, 0, lpProcName, size_a, NULL, 0);
		wchar_t *wProcName = new wchar_t[size_w];
		MultiByteToWideChar(CP_ACP, 0, lpProcName, size_a, wProcName, size_w);

		wchar_t *wMsgError = new wchar_t[MAX_PATH];
		wsprintf(wMsgError, L"Can not find export function: %s.", wProcName);

		MessageBox(NULL, wMsgError, L"API ERROR", MB_ICONERROR);
		free(wProcName);
		free(wMsgError);
		ExitProcess(-1);
	}
}

#pragma region API Export
namespace API_EXPORT {
	LPCWSTR MSG_TITLE   = L"ERROR";
	LPCWSTR MSG_CONTENT = L"Something happened";

	// Create some padding for the function.
	dll_export GetFileVersionInfoA(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 0); }
	dll_export GetFileVersionInfoByHandle(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 1); }
	dll_export GetFileVersionInfoExW(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 2); }
	dll_export GetFileVersionInfoSizeA(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 3); }
	dll_export GetFileVersionInfoSizeExW(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 4); }
	dll_export GetFileVersionInfoSizeW(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 5); }
	dll_export GetFileVersionInfoW(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 6); }
	dll_export VerFindFileA(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 7); }
	dll_export VerFindFileW(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 8); }
	dll_export VerInstallFileA(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 9); }
	dll_export VerInstallFileW(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 10); }
	dll_export VerLanguageNameA(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 11); }
	dll_export VerLanguageNameW(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 12); }
	dll_export VerQueryValueA(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 13); }
	dll_export VerQueryValueW(void) { MessageBox(NULL, MSG_CONTENT, MSG_TITLE, 14); }
}
#pragma endregion