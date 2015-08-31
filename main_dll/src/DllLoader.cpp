#include "DllLoader.h"
#include <forward_list>

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
#ifdef _DEBUG
	// Re-enable printf functions.
	FILE *stream;
	if (!AttachConsole(-1))
		AllocConsole();
	freopen_s(&stream, "CONOUT$", "w+", stdout);

	dprintf("Fixing library import..\n");
#endif
	wchar_t * szTargetDll = new wchar_t[MAX_PATH];
	wchar_t * szThisDllPath = new wchar_t[MAX_PATH];

	GetSystemDirectory(szTargetDll, MAX_PATH);
	GetModuleFileName(hInstance, szThisDllPath, MAX_PATH);
	wchar_t * szDllName = wcsrchr(szThisDllPath, L'\\');
	wcscat_s(szTargetDll, MAX_PATH, szDllName++);

	hDll = LoadLibrary(szTargetDll);

	// If error, exit.
	if (!hDll) {
		wchar_t * szErrorMsg = new wchar_t[MAX_PATH];
		wsprintf(szErrorMsg, L"Failed to load dll:\n%s", szTargetDll);
		dwprintf(L"%s\n==============================================\n", szErrorMsg);
#ifndef _DEBUG
		MessageBox(nullptr, szErrorMsg, L"DLL ERROR", MB_ICONERROR);
#endif
		delete(szErrorMsg);
		delete(szThisDllPath);
		delete(szTargetDll);
		ExitProcess(1);
		return;
	}
	
	int i = 0;
	while (szDllName[i])
	{
		szDllName[i] = tolower(szDllName[i]);
		i++;
	}

	dwprintf(L"Fix import for %s..\n", szDllName);

#pragma region Restore API
#ifdef __EXP_VERSION
	if (wcscmp(szDllName, L"version.dll") == 0)
	{
		fixAPI("GetFileVersionInfoA", uAddr(API_EXPORT::GetFileVersionInfoA));
		fixAPI("GetFileVersionInfoByHandle", uAddr(API_EXPORT::GetFileVersionInfoByHandle));
		fixAPI("GetFileVersionInfoExW", uAddr(API_EXPORT::GetFileVersionInfoExW));
		fixAPI("GetFileVersionInfoSizeA", uAddr(API_EXPORT::GetFileVersionInfoSizeA));
		fixAPI("GetFileVersionInfoSizeExW", uAddr(API_EXPORT::GetFileVersionInfoSizeExW));
		fixAPI("GetFileVersionInfoSizeW", uAddr(API_EXPORT::GetFileVersionInfoSizeW));
		fixAPI("GetFileVersionInfoW", uAddr(API_EXPORT::GetFileVersionInfoW));
		fixAPI("VerFindFileA", uAddr(API_EXPORT::VerFindFileA));
		fixAPI("VerFindFileW", uAddr(API_EXPORT::VerFindFileW));
		fixAPI("VerInstallFileA", uAddr(API_EXPORT::VerInstallFileA));
		fixAPI("VerInstallFileW", uAddr(API_EXPORT::VerInstallFileW));
		fixAPI("VerLanguageNameA", uAddr(API_EXPORT::VerLanguageNameA));
		fixAPI("VerLanguageNameW", uAddr(API_EXPORT::VerLanguageNameW));
		fixAPI("VerQueryValueA", uAddr(API_EXPORT::VerQueryValueA));
		fixAPI("VerQueryValueW", uAddr(API_EXPORT::VerQueryValueW));
	}
#endif

#ifdef __EXP_LPK
	if (wcscmp(szDllName, L"lpk.dll") == 0)
	{
		fixAPI("LpkDllInitialize", uAddr(API_EXPORT::LpkDllInitialize));
		fixAPI("LpkDrawTextEx", uAddr(API_EXPORT::LpkDrawTextEx));

		memcpy(API_EXPORT::LpkEditControl, 
			GetProcAddress(hDll, "LpkEditControl"),
			sizeof(API_EXPORT::LpkEditControl)
		);

		fixAPI("LpkExtTextOut", uAddr(API_EXPORT::LpkExtTextOut));
		fixAPI("LpkGetCharacterPlacement", uAddr(API_EXPORT::LpkGetCharacterPlacement));
		fixAPI("LpkGetTextExtentExPoint", uAddr(API_EXPORT::LpkGetTextExtentExPoint));
		fixAPI("LpkInitialize", uAddr(API_EXPORT::LpkInitialize));
		fixAPI("LpkPSMTextOut", uAddr(API_EXPORT::LpkPSMTextOut));
		fixAPI("LpkTabbedTextOut", uAddr(API_EXPORT::LpkTabbedTextOut));
		fixAPI("LpkUseGDIWidthCache", uAddr(API_EXPORT::LpkUseGDIWidthCache));
		fixAPI("ftsWordBreak", uAddr(API_EXPORT::ftsWordBreak));
	}
#endif

#ifdef __EXP_MSIMG32
	fixAPI("AlphaBlend", uAddr(API_EXPORT::AlphaBlend));
	fixAPI("DllInitialize", uAddr(API_EXPORT::DllInitialize));
	fixAPI("GradientFill", uAddr(API_EXPORT::GradientFill));
	fixAPI("TransparentBlt", uAddr(API_EXPORT::TransparentBlt));
	fixAPI("vSetDdrawflag", uAddr(API_EXPORT::vSetDdrawflag));
#endif

#ifdef __EXP_WINMM
	if (wcscmp(szDllName, L"winmm.dll") == 0)
	{
		#ifdef PlaySound
			#ifndef __BAK__PlaySound
				#define __BAK__PlaySound PlaySound
			#endif
			#undef PlaySound
		#endif
		fixAPI("mciExecute", uAddr(API_EXPORT::mciExecute));
		fixAPI("CloseDriver", uAddr(API_EXPORT::CloseDriver));
		fixAPI("DefDriverProc", uAddr(API_EXPORT::DefDriverProc));
		fixAPI("DriverCallback", uAddr(API_EXPORT::DriverCallback));
		fixAPI("DrvGetModuleHandle", uAddr(API_EXPORT::DrvGetModuleHandle));
		fixAPI("GetDriverModuleHandle", uAddr(API_EXPORT::GetDriverModuleHandle));
		fixAPI("OpenDriver", uAddr(API_EXPORT::OpenDriver));
		fixAPI("PlaySound", uAddr(API_EXPORT::PlaySound));
		fixAPI("PlaySoundA", uAddr(API_EXPORT::PlaySoundA));
		fixAPI("PlaySoundW", uAddr(API_EXPORT::PlaySoundW));
		fixAPI("SendDriverMessage", uAddr(API_EXPORT::SendDriverMessage));
		fixAPI("WOWAppExit", uAddr(API_EXPORT::WOWAppExit));
		fixAPI("auxGetDevCapsA", uAddr(API_EXPORT::auxGetDevCapsA));
		fixAPI("auxGetDevCapsW", uAddr(API_EXPORT::auxGetDevCapsW));
		fixAPI("auxGetNumDevs", uAddr(API_EXPORT::auxGetNumDevs));
		fixAPI("auxGetVolume", uAddr(API_EXPORT::auxGetVolume));
		fixAPI("auxOutMessage", uAddr(API_EXPORT::auxOutMessage));
		fixAPI("auxSetVolume", uAddr(API_EXPORT::auxSetVolume));
		fixAPI("joyConfigChanged", uAddr(API_EXPORT::joyConfigChanged));
		fixAPI("joyGetDevCapsA", uAddr(API_EXPORT::joyGetDevCapsA));
		fixAPI("joyGetDevCapsW", uAddr(API_EXPORT::joyGetDevCapsW));
		fixAPI("joyGetNumDevs", uAddr(API_EXPORT::joyGetNumDevs));
		fixAPI("joyGetPos", uAddr(API_EXPORT::joyGetPos));
		fixAPI("joyGetPosEx", uAddr(API_EXPORT::joyGetPosEx));
		fixAPI("joyGetThreshold", uAddr(API_EXPORT::joyGetThreshold));
		fixAPI("joyReleaseCapture", uAddr(API_EXPORT::joyReleaseCapture));
		fixAPI("joySetCapture", uAddr(API_EXPORT::joySetCapture));
		fixAPI("joySetThreshold", uAddr(API_EXPORT::joySetThreshold));
		fixAPI("mciDriverNotify", uAddr(API_EXPORT::mciDriverNotify));
		fixAPI("mciDriverYield", uAddr(API_EXPORT::mciDriverYield));
		fixAPI("mciFreeCommandResource", uAddr(API_EXPORT::mciFreeCommandResource));
		fixAPI("mciGetCreatorTask", uAddr(API_EXPORT::mciGetCreatorTask));
		fixAPI("mciGetDeviceIDA", uAddr(API_EXPORT::mciGetDeviceIDA));
		fixAPI("mciGetDeviceIDFromElementIDA", uAddr(API_EXPORT::mciGetDeviceIDFromElementIDA));
		fixAPI("mciGetDeviceIDFromElementIDW", uAddr(API_EXPORT::mciGetDeviceIDFromElementIDW));
		fixAPI("mciGetDeviceIDW", uAddr(API_EXPORT::mciGetDeviceIDW));
		fixAPI("mciGetDriverData", uAddr(API_EXPORT::mciGetDriverData));
		fixAPI("mciGetErrorStringA", uAddr(API_EXPORT::mciGetErrorStringA));
		fixAPI("mciGetErrorStringW", uAddr(API_EXPORT::mciGetErrorStringW));
		fixAPI("mciGetYieldProc", uAddr(API_EXPORT::mciGetYieldProc));
		fixAPI("mciLoadCommandResource", uAddr(API_EXPORT::mciLoadCommandResource));
		fixAPI("mciSendCommandA", uAddr(API_EXPORT::mciSendCommandA));
		fixAPI("mciSendCommandW", uAddr(API_EXPORT::mciSendCommandW));
		fixAPI("mciSendStringA", uAddr(API_EXPORT::mciSendStringA));
		fixAPI("mciSendStringW", uAddr(API_EXPORT::mciSendStringW));
		fixAPI("mciSetDriverData", uAddr(API_EXPORT::mciSetDriverData));
		fixAPI("mciSetYieldProc", uAddr(API_EXPORT::mciSetYieldProc));
		fixAPI("midiConnect", uAddr(API_EXPORT::midiConnect));
		fixAPI("midiDisconnect", uAddr(API_EXPORT::midiDisconnect));
		fixAPI("midiInAddBuffer", uAddr(API_EXPORT::midiInAddBuffer));
		fixAPI("midiInClose", uAddr(API_EXPORT::midiInClose));
		fixAPI("midiInGetDevCapsA", uAddr(API_EXPORT::midiInGetDevCapsA));
		fixAPI("midiInGetDevCapsW", uAddr(API_EXPORT::midiInGetDevCapsW));
		fixAPI("midiInGetErrorTextA", uAddr(API_EXPORT::midiInGetErrorTextA));
		fixAPI("midiInGetErrorTextW", uAddr(API_EXPORT::midiInGetErrorTextW));
		fixAPI("midiInGetID", uAddr(API_EXPORT::midiInGetID));
		fixAPI("midiInGetNumDevs", uAddr(API_EXPORT::midiInGetNumDevs));
		fixAPI("midiInMessage", uAddr(API_EXPORT::midiInMessage));
		fixAPI("midiInOpen", uAddr(API_EXPORT::midiInOpen));
		fixAPI("midiInPrepareHeader", uAddr(API_EXPORT::midiInPrepareHeader));
		fixAPI("midiInReset", uAddr(API_EXPORT::midiInReset));
		fixAPI("midiInStart", uAddr(API_EXPORT::midiInStart));
		fixAPI("midiInStop", uAddr(API_EXPORT::midiInStop));
		fixAPI("midiInUnprepareHeader", uAddr(API_EXPORT::midiInUnprepareHeader));
		fixAPI("midiOutCacheDrumPatches", uAddr(API_EXPORT::midiOutCacheDrumPatches));
		fixAPI("midiOutCachePatches", uAddr(API_EXPORT::midiOutCachePatches));
		fixAPI("midiOutClose", uAddr(API_EXPORT::midiOutClose));
		fixAPI("midiOutGetDevCapsA", uAddr(API_EXPORT::midiOutGetDevCapsA));
		fixAPI("midiOutGetDevCapsW", uAddr(API_EXPORT::midiOutGetDevCapsW));
		fixAPI("midiOutGetErrorTextA", uAddr(API_EXPORT::midiOutGetErrorTextA));
		fixAPI("midiOutGetErrorTextW", uAddr(API_EXPORT::midiOutGetErrorTextW));
		fixAPI("midiOutGetID", uAddr(API_EXPORT::midiOutGetID));
		fixAPI("midiOutGetNumDevs", uAddr(API_EXPORT::midiOutGetNumDevs));
		fixAPI("midiOutGetVolume", uAddr(API_EXPORT::midiOutGetVolume));
		fixAPI("midiOutLongMsg", uAddr(API_EXPORT::midiOutLongMsg));
		fixAPI("midiOutMessage", uAddr(API_EXPORT::midiOutMessage));
		fixAPI("midiOutOpen", uAddr(API_EXPORT::midiOutOpen));
		fixAPI("midiOutPrepareHeader", uAddr(API_EXPORT::midiOutPrepareHeader));
		fixAPI("midiOutReset", uAddr(API_EXPORT::midiOutReset));
		fixAPI("midiOutSetVolume", uAddr(API_EXPORT::midiOutSetVolume));
		fixAPI("midiOutShortMsg", uAddr(API_EXPORT::midiOutShortMsg));
		fixAPI("midiOutUnprepareHeader", uAddr(API_EXPORT::midiOutUnprepareHeader));
		fixAPI("midiStreamClose", uAddr(API_EXPORT::midiStreamClose));
		fixAPI("midiStreamOpen", uAddr(API_EXPORT::midiStreamOpen));
		fixAPI("midiStreamOut", uAddr(API_EXPORT::midiStreamOut));
		fixAPI("midiStreamPause", uAddr(API_EXPORT::midiStreamPause));
		fixAPI("midiStreamPosition", uAddr(API_EXPORT::midiStreamPosition));
		fixAPI("midiStreamProperty", uAddr(API_EXPORT::midiStreamProperty));
		fixAPI("midiStreamRestart", uAddr(API_EXPORT::midiStreamRestart));
		fixAPI("midiStreamStop", uAddr(API_EXPORT::midiStreamStop));
		fixAPI("mixerClose", uAddr(API_EXPORT::mixerClose));
		fixAPI("mixerGetControlDetailsA", uAddr(API_EXPORT::mixerGetControlDetailsA));
		fixAPI("mixerGetControlDetailsW", uAddr(API_EXPORT::mixerGetControlDetailsW));
		fixAPI("mixerGetDevCapsA", uAddr(API_EXPORT::mixerGetDevCapsA));
		fixAPI("mixerGetDevCapsW", uAddr(API_EXPORT::mixerGetDevCapsW));
		fixAPI("mixerGetID", uAddr(API_EXPORT::mixerGetID));
		fixAPI("mixerGetLineControlsA", uAddr(API_EXPORT::mixerGetLineControlsA));
		fixAPI("mixerGetLineControlsW", uAddr(API_EXPORT::mixerGetLineControlsW));
		fixAPI("mixerGetLineInfoA", uAddr(API_EXPORT::mixerGetLineInfoA));
		fixAPI("mixerGetLineInfoW", uAddr(API_EXPORT::mixerGetLineInfoW));
		fixAPI("mixerGetNumDevs", uAddr(API_EXPORT::mixerGetNumDevs));
		fixAPI("mixerMessage", uAddr(API_EXPORT::mixerMessage));
		fixAPI("mixerOpen", uAddr(API_EXPORT::mixerOpen));
		fixAPI("mixerSetControlDetails", uAddr(API_EXPORT::mixerSetControlDetails));
		fixAPI("mmDrvInstall", uAddr(API_EXPORT::mmDrvInstall));
		fixAPI("mmGetCurrentTask", uAddr(API_EXPORT::mmGetCurrentTask));
		fixAPI("mmTaskBlock", uAddr(API_EXPORT::mmTaskBlock));
		fixAPI("mmTaskCreate", uAddr(API_EXPORT::mmTaskCreate));
		fixAPI("mmTaskSignal", uAddr(API_EXPORT::mmTaskSignal));
		fixAPI("mmTaskYield", uAddr(API_EXPORT::mmTaskYield));
		fixAPI("mmioAdvance", uAddr(API_EXPORT::mmioAdvance));
		fixAPI("mmioAscend", uAddr(API_EXPORT::mmioAscend));
		fixAPI("mmioClose", uAddr(API_EXPORT::mmioClose));
		fixAPI("mmioCreateChunk", uAddr(API_EXPORT::mmioCreateChunk));
		fixAPI("mmioDescend", uAddr(API_EXPORT::mmioDescend));
		fixAPI("mmioFlush", uAddr(API_EXPORT::mmioFlush));
		fixAPI("mmioGetInfo", uAddr(API_EXPORT::mmioGetInfo));
		fixAPI("mmioInstallIOProcA", uAddr(API_EXPORT::mmioInstallIOProcA));
		fixAPI("mmioInstallIOProcW", uAddr(API_EXPORT::mmioInstallIOProcW));
		fixAPI("mmioOpenA", uAddr(API_EXPORT::mmioOpenA));
		fixAPI("mmioOpenW", uAddr(API_EXPORT::mmioOpenW));
		fixAPI("mmioRead", uAddr(API_EXPORT::mmioRead));
		fixAPI("mmioRenameA", uAddr(API_EXPORT::mmioRenameA));
		fixAPI("mmioRenameW", uAddr(API_EXPORT::mmioRenameW));
		fixAPI("mmioSeek", uAddr(API_EXPORT::mmioSeek));
		fixAPI("mmioSendMessage", uAddr(API_EXPORT::mmioSendMessage));
		fixAPI("mmioSetBuffer", uAddr(API_EXPORT::mmioSetBuffer));
		fixAPI("mmioSetInfo", uAddr(API_EXPORT::mmioSetInfo));
		fixAPI("mmioStringToFOURCCA", uAddr(API_EXPORT::mmioStringToFOURCCA));
		fixAPI("mmioStringToFOURCCW", uAddr(API_EXPORT::mmioStringToFOURCCW));
		fixAPI("mmioWrite", uAddr(API_EXPORT::mmioWrite));
		fixAPI("mmsystemGetVersion", uAddr(API_EXPORT::mmsystemGetVersion));
		fixAPI("sndPlaySoundA", uAddr(API_EXPORT::sndPlaySoundA));
		fixAPI("sndPlaySoundW", uAddr(API_EXPORT::sndPlaySoundW));
		fixAPI("timeBeginPeriod", uAddr(API_EXPORT::timeBeginPeriod));
		fixAPI("timeEndPeriod", uAddr(API_EXPORT::timeEndPeriod));
		fixAPI("timeGetDevCaps", uAddr(API_EXPORT::timeGetDevCaps));
		fixAPI("timeGetSystemTime", uAddr(API_EXPORT::timeGetSystemTime));
		fixAPI("timeGetTime", uAddr(API_EXPORT::timeGetTime));
		fixAPI("timeKillEvent", uAddr(API_EXPORT::timeKillEvent));
		fixAPI("timeSetEvent", uAddr(API_EXPORT::timeSetEvent));
		fixAPI("waveInAddBuffer", uAddr(API_EXPORT::waveInAddBuffer));
		fixAPI("waveInClose", uAddr(API_EXPORT::waveInClose));
		fixAPI("waveInGetDevCapsA", uAddr(API_EXPORT::waveInGetDevCapsA));
		fixAPI("waveInGetDevCapsW", uAddr(API_EXPORT::waveInGetDevCapsW));
		fixAPI("waveInGetErrorTextA", uAddr(API_EXPORT::waveInGetErrorTextA));
		fixAPI("waveInGetErrorTextW", uAddr(API_EXPORT::waveInGetErrorTextW));
		fixAPI("waveInGetID", uAddr(API_EXPORT::waveInGetID));
		fixAPI("waveInGetNumDevs", uAddr(API_EXPORT::waveInGetNumDevs));
		fixAPI("waveInGetPosition", uAddr(API_EXPORT::waveInGetPosition));
		fixAPI("waveInMessage", uAddr(API_EXPORT::waveInMessage));
		fixAPI("waveInOpen", uAddr(API_EXPORT::waveInOpen));
		fixAPI("waveInPrepareHeader", uAddr(API_EXPORT::waveInPrepareHeader));
		fixAPI("waveInReset", uAddr(API_EXPORT::waveInReset));
		fixAPI("waveInStart", uAddr(API_EXPORT::waveInStart));
		fixAPI("waveInStop", uAddr(API_EXPORT::waveInStop));
		fixAPI("waveInUnprepareHeader", uAddr(API_EXPORT::waveInUnprepareHeader));
		fixAPI("waveOutBreakLoop", uAddr(API_EXPORT::waveOutBreakLoop));
		fixAPI("waveOutClose", uAddr(API_EXPORT::waveOutClose));
		fixAPI("waveOutGetDevCapsA", uAddr(API_EXPORT::waveOutGetDevCapsA));
		fixAPI("waveOutGetDevCapsW", uAddr(API_EXPORT::waveOutGetDevCapsW));
		fixAPI("waveOutGetErrorTextA", uAddr(API_EXPORT::waveOutGetErrorTextA));
		fixAPI("waveOutGetErrorTextW", uAddr(API_EXPORT::waveOutGetErrorTextW));
		fixAPI("waveOutGetID", uAddr(API_EXPORT::waveOutGetID));
		fixAPI("waveOutGetNumDevs", uAddr(API_EXPORT::waveOutGetNumDevs));
		fixAPI("waveOutGetPitch", uAddr(API_EXPORT::waveOutGetPitch));
		fixAPI("waveOutGetPlaybackRate", uAddr(API_EXPORT::waveOutGetPlaybackRate));
		fixAPI("waveOutGetPosition", uAddr(API_EXPORT::waveOutGetPosition));
		fixAPI("waveOutGetVolume", uAddr(API_EXPORT::waveOutGetVolume));
		fixAPI("waveOutMessage", uAddr(API_EXPORT::waveOutMessage));
		fixAPI("waveOutOpen", uAddr(API_EXPORT::waveOutOpen));
		fixAPI("waveOutPause", uAddr(API_EXPORT::waveOutPause));
		fixAPI("waveOutPrepareHeader", uAddr(API_EXPORT::waveOutPrepareHeader));
		fixAPI("waveOutReset", uAddr(API_EXPORT::waveOutReset));
		fixAPI("waveOutRestart", uAddr(API_EXPORT::waveOutRestart));
		fixAPI("waveOutSetPitch", uAddr(API_EXPORT::waveOutSetPitch));
		fixAPI("waveOutSetPlaybackRate", uAddr(API_EXPORT::waveOutSetPlaybackRate));
		fixAPI("waveOutSetVolume", uAddr(API_EXPORT::waveOutSetVolume));
		fixAPI("waveOutUnprepareHeader", uAddr(API_EXPORT::waveOutUnprepareHeader));
		fixAPI("waveOutWrite", uAddr(API_EXPORT::waveOutWrite));
		#ifdef __BAK__PlaySound
			#define PlaySound __BAK__PlaySound
			#undef __BAK__PlaySound
		#endif
	}
#endif
#pragma endregion

	delete(szThisDllPath);
	delete(szTargetDll);

	dwprintf(L"Call to do_patch ..\n");
	
#ifdef __XP_PATCH_IN_THREAD
	HMODULE hBase = GetModuleHandle(nullptr);
	if (IsWindowsVistaOrGreater())
	{
		do_patch(hBase);
	} else
	{
		CreateThread(nullptr, 0,
			LPTHREAD_START_ROUTINE(do_patch), hBase, 0, nullptr);
	}
#else
	do_patch(GetModuleHandle(nullptr));
#endif

	dwprintf(L"All done.\n==============================================\n");
}

void exitLibrary() {
	FreeLibrary(hDll);
}


void fixAPI(LPCSTR lpProcName, uAddr fnEntry)
{
	// In x64 bit system, 
	// When compile as DEBUG, it will generate code like jmp xxxx
	// Which is not long enough to hold a 64-bit address.

	// Where as in RELEASE mode, it will just pass the *real* address
	// to this function.
#if defined(_WIN64) && defined(_DEBUG)
	fnEntry = ReadRelativeAddress(fnEntry + 1);
#endif

	FARPROC fnDest = GetProcAddress(hDll, lpProcName);
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
		
		WriteUShortRaw(fnEntry, 0xB848);
		*reinterpret_cast<FARPROC *>(fnEntry + 2) = fnDest;

		// jmp rax
		WriteUShortRaw(fnEntry + 10, 0xE0FF);
#else
		// 773E3D66 | E9 0D 19 F6 9A | jmp 12345678 |
		WriteByteRaw(fnEntry, 0xE9);
		WriteRelativeAddress(fnEntry + 1, uAddr(fnDest));
#endif
		VirtualProtect(reinterpret_cast<void *>(fnEntry), SHELL_LENGTH, oldProtect, &oldProtect);
	}
	else
	{
		dprintf("Couldn't find export function %s: fnDest is null.\n", lpProcName);
	}
}

#pragma region API Export
namespace API_EXPORT {
#ifdef __EXP_VERSION
	// Create some padding for the function.
	dll_export GetFileVersionInfoA(void) { Sleep(1); }
	dll_export GetFileVersionInfoByHandle(void) { Sleep(2); }
	dll_export GetFileVersionInfoExW(void) { Sleep(3); }
	dll_export GetFileVersionInfoSizeA(void) { Sleep(4); }
	dll_export GetFileVersionInfoSizeExW(void) { Sleep(5); }
	dll_export GetFileVersionInfoSizeW(void) { Sleep(6); }
	dll_export GetFileVersionInfoW(void) { Sleep(7); }
	dll_export VerFindFileA(void) { Sleep(8); }
	dll_export VerFindFileW(void) { Sleep(9); }
	dll_export VerInstallFileA(void) { Sleep(10); }
	dll_export VerInstallFileW(void) { Sleep(11); }
	dll_export VerLanguageNameA(void) { Sleep(12); }
	dll_export VerLanguageNameW(void) { Sleep(13); }
	dll_export VerQueryValueA(void) { Sleep(14); }
	dll_export VerQueryValueW(void) { Sleep(15); }
#endif

#ifdef __EXP_LPK
	dll_export LpkDllInitialize(void) { Sleep(19); };
	dll_export LpkDrawTextEx(void) { Sleep(20); }
	var_export VOID(*LpkEditControl[14])() = { 0 };
	dll_export LpkExtTextOut(void) { Sleep(22); }
	dll_export LpkGetCharacterPlacement(void) { Sleep(23); }
	dll_export LpkGetTextExtentExPoint(void) { Sleep(24); }
	dll_export LpkInitialize(void) { Sleep(25); }
	dll_export LpkPSMTextOut(void) { Sleep(26); }
	dll_export LpkTabbedTextOut(void) { Sleep(27); }
	dll_export LpkUseGDIWidthCache(void) { Sleep(28); }
	dll_export ftsWordBreak(void) { Sleep(29); }
#endif

#ifdef __EXP_MSIMG32
	dll_export AlphaBlend(void) { Sleep(300); };
	dll_export DllInitialize(void) { Sleep(301); };
	dll_export GradientFill(void) { Sleep(302); };
	dll_export TransparentBlt(void) { Sleep(303); };
	dll_export vSetDdrawflag(void) { Sleep(304); };
#endif

#ifdef __EXP_WINMM
	#ifdef PlaySound
		#ifndef __BAK__PlaySound
			#define __BAK__PlaySound PlaySound
		#endif
		#undef PlaySound
	#endif

	dll_export mciExecute(void) { Sleep(30); }
	dll_export CloseDriver(void) { Sleep(31); }
	dll_export DefDriverProc(void) { Sleep(32); }
	dll_export DriverCallback(void) { Sleep(33); }
	dll_export DrvGetModuleHandle(void) { Sleep(34); }
	dll_export GetDriverModuleHandle(void) { Sleep(35); }
	dll_export OpenDriver(void) { Sleep(36); }
	dll_export PlaySound(void) { Sleep(37); }
	dll_export PlaySoundA(void) { Sleep(38); }
	dll_export PlaySoundW(void) { Sleep(39); }
	dll_export SendDriverMessage(void) { Sleep(40); }
	dll_export WOWAppExit(void) { Sleep(41); }
	dll_export auxGetDevCapsA(void) { Sleep(42); }
	dll_export auxGetDevCapsW(void) { Sleep(43); }
	dll_export auxGetNumDevs(void) { Sleep(44); }
	dll_export auxGetVolume(void) { Sleep(45); }
	dll_export auxOutMessage(void) { Sleep(46); }
	dll_export auxSetVolume(void) { Sleep(47); }
	dll_export joyConfigChanged(void) { Sleep(48); }
	dll_export joyGetDevCapsA(void) { Sleep(49); }
	dll_export joyGetDevCapsW(void) { Sleep(50); }
	dll_export joyGetNumDevs(void) { Sleep(51); }
	dll_export joyGetPos(void) { Sleep(52); }
	dll_export joyGetPosEx(void) { Sleep(53); }
	dll_export joyGetThreshold(void) { Sleep(54); }
	dll_export joyReleaseCapture(void) { Sleep(55); }
	dll_export joySetCapture(void) { Sleep(56); }
	dll_export joySetThreshold(void) { Sleep(57); }
	dll_export mciDriverNotify(void) { Sleep(58); }
	dll_export mciDriverYield(void) { Sleep(59); }
	dll_export mciFreeCommandResource(void) { Sleep(60); }
	dll_export mciGetCreatorTask(void) { Sleep(61); }
	dll_export mciGetDeviceIDA(void) { Sleep(62); }
	dll_export mciGetDeviceIDFromElementIDA(void) { Sleep(63); }
	dll_export mciGetDeviceIDFromElementIDW(void) { Sleep(64); }
	dll_export mciGetDeviceIDW(void) { Sleep(65); }
	dll_export mciGetDriverData(void) { Sleep(66); }
	dll_export mciGetErrorStringA(void) { Sleep(67); }
	dll_export mciGetErrorStringW(void) { Sleep(68); }
	dll_export mciGetYieldProc(void) { Sleep(69); }
	dll_export mciLoadCommandResource(void) { Sleep(70); }
	dll_export mciSendCommandA(void) { Sleep(71); }
	dll_export mciSendCommandW(void) { Sleep(72); }
	dll_export mciSendStringA(void) { Sleep(73); }
	dll_export mciSendStringW(void) { Sleep(74); }
	dll_export mciSetDriverData(void) { Sleep(75); }
	dll_export mciSetYieldProc(void) { Sleep(76); }
	dll_export midiConnect(void) { Sleep(77); }
	dll_export midiDisconnect(void) { Sleep(78); }
	dll_export midiInAddBuffer(void) { Sleep(79); }
	dll_export midiInClose(void) { Sleep(80); }
	dll_export midiInGetDevCapsA(void) { Sleep(81); }
	dll_export midiInGetDevCapsW(void) { Sleep(82); }
	dll_export midiInGetErrorTextA(void) { Sleep(83); }
	dll_export midiInGetErrorTextW(void) { Sleep(84); }
	dll_export midiInGetID(void) { Sleep(85); }
	dll_export midiInGetNumDevs(void) { Sleep(86); }
	dll_export midiInMessage(void) { Sleep(87); }
	dll_export midiInOpen(void) { Sleep(88); }
	dll_export midiInPrepareHeader(void) { Sleep(89); }
	dll_export midiInReset(void) { Sleep(90); }
	dll_export midiInStart(void) { Sleep(91); }
	dll_export midiInStop(void) { Sleep(92); }
	dll_export midiInUnprepareHeader(void) { Sleep(93); }
	dll_export midiOutCacheDrumPatches(void) { Sleep(94); }
	dll_export midiOutCachePatches(void) { Sleep(95); }
	dll_export midiOutClose(void) { Sleep(96); }
	dll_export midiOutGetDevCapsA(void) { Sleep(97); }
	dll_export midiOutGetDevCapsW(void) { Sleep(98); }
	dll_export midiOutGetErrorTextA(void) { Sleep(99); }
	dll_export midiOutGetErrorTextW(void) { Sleep(100); }
	dll_export midiOutGetID(void) { Sleep(101); }
	dll_export midiOutGetNumDevs(void) { Sleep(102); }
	dll_export midiOutGetVolume(void) { Sleep(103); }
	dll_export midiOutLongMsg(void) { Sleep(104); }
	dll_export midiOutMessage(void) { Sleep(105); }
	dll_export midiOutOpen(void) { Sleep(106); }
	dll_export midiOutPrepareHeader(void) { Sleep(107); }
	dll_export midiOutReset(void) { Sleep(108); }
	dll_export midiOutSetVolume(void) { Sleep(109); }
	dll_export midiOutShortMsg(void) { Sleep(110); }
	dll_export midiOutUnprepareHeader(void) { Sleep(111); }
	dll_export midiStreamClose(void) { Sleep(112); }
	dll_export midiStreamOpen(void) { Sleep(113); }
	dll_export midiStreamOut(void) { Sleep(114); }
	dll_export midiStreamPause(void) { Sleep(115); }
	dll_export midiStreamPosition(void) { Sleep(116); }
	dll_export midiStreamProperty(void) { Sleep(117); }
	dll_export midiStreamRestart(void) { Sleep(118); }
	dll_export midiStreamStop(void) { Sleep(119); }
	dll_export mixerClose(void) { Sleep(120); }
	dll_export mixerGetControlDetailsA(void) { Sleep(121); }
	dll_export mixerGetControlDetailsW(void) { Sleep(122); }
	dll_export mixerGetDevCapsA(void) { Sleep(123); }
	dll_export mixerGetDevCapsW(void) { Sleep(124); }
	dll_export mixerGetID(void) { Sleep(125); }
	dll_export mixerGetLineControlsA(void) { Sleep(126); }
	dll_export mixerGetLineControlsW(void) { Sleep(127); }
	dll_export mixerGetLineInfoA(void) { Sleep(128); }
	dll_export mixerGetLineInfoW(void) { Sleep(129); }
	dll_export mixerGetNumDevs(void) { Sleep(130); }
	dll_export mixerMessage(void) { Sleep(131); }
	dll_export mixerOpen(void) { Sleep(132); }
	dll_export mixerSetControlDetails(void) { Sleep(133); }
	dll_export mmDrvInstall(void) { Sleep(134); }
	dll_export mmGetCurrentTask(void) { Sleep(135); }
	dll_export mmTaskBlock(void) { Sleep(136); }
	dll_export mmTaskCreate(void) { Sleep(137); }
	dll_export mmTaskSignal(void) { Sleep(138); }
	dll_export mmTaskYield(void) { Sleep(139); }
	dll_export mmioAdvance(void) { Sleep(140); }
	dll_export mmioAscend(void) { Sleep(141); }
	dll_export mmioClose(void) { Sleep(142); }
	dll_export mmioCreateChunk(void) { Sleep(143); }
	dll_export mmioDescend(void) { Sleep(144); }
	dll_export mmioFlush(void) { Sleep(145); }
	dll_export mmioGetInfo(void) { Sleep(146); }
	dll_export mmioInstallIOProcA(void) { Sleep(147); }
	dll_export mmioInstallIOProcW(void) { Sleep(148); }
	dll_export mmioOpenA(void) { Sleep(149); }
	dll_export mmioOpenW(void) { Sleep(150); }
	dll_export mmioRead(void) { Sleep(151); }
	dll_export mmioRenameA(void) { Sleep(152); }
	dll_export mmioRenameW(void) { Sleep(153); }
	dll_export mmioSeek(void) { Sleep(154); }
	dll_export mmioSendMessage(void) { Sleep(155); }
	dll_export mmioSetBuffer(void) { Sleep(156); }
	dll_export mmioSetInfo(void) { Sleep(157); }
	dll_export mmioStringToFOURCCA(void) { Sleep(158); }
	dll_export mmioStringToFOURCCW(void) { Sleep(159); }
	dll_export mmioWrite(void) { Sleep(160); }
	dll_export mmsystemGetVersion(void) { Sleep(161); }
	dll_export sndPlaySoundA(void) { Sleep(162); }
	dll_export sndPlaySoundW(void) { Sleep(163); }
	dll_export timeBeginPeriod(void) { Sleep(164); }
	dll_export timeEndPeriod(void) { Sleep(165); }
	dll_export timeGetDevCaps(void) { Sleep(166); }
	dll_export timeGetSystemTime(void) { Sleep(167); }
	dll_export timeGetTime(void) { Sleep(168); }
	dll_export timeKillEvent(void) { Sleep(169); }
	dll_export timeSetEvent(void) { Sleep(170); }
	dll_export waveInAddBuffer(void) { Sleep(171); }
	dll_export waveInClose(void) { Sleep(172); }
	dll_export waveInGetDevCapsA(void) { Sleep(173); }
	dll_export waveInGetDevCapsW(void) { Sleep(174); }
	dll_export waveInGetErrorTextA(void) { Sleep(175); }
	dll_export waveInGetErrorTextW(void) { Sleep(176); }
	dll_export waveInGetID(void) { Sleep(177); }
	dll_export waveInGetNumDevs(void) { Sleep(178); }
	dll_export waveInGetPosition(void) { Sleep(179); }
	dll_export waveInMessage(void) { Sleep(180); }
	dll_export waveInOpen(void) { Sleep(181); }
	dll_export waveInPrepareHeader(void) { Sleep(182); }
	dll_export waveInReset(void) { Sleep(183); }
	dll_export waveInStart(void) { Sleep(184); }
	dll_export waveInStop(void) { Sleep(185); }
	dll_export waveInUnprepareHeader(void) { Sleep(186); }
	dll_export waveOutBreakLoop(void) { Sleep(187); }
	dll_export waveOutClose(void) { Sleep(188); }
	dll_export waveOutGetDevCapsA(void) { Sleep(189); }
	dll_export waveOutGetDevCapsW(void) { Sleep(190); }
	dll_export waveOutGetErrorTextA(void) { Sleep(191); }
	dll_export waveOutGetErrorTextW(void) { Sleep(192); }
	dll_export waveOutGetID(void) { Sleep(193); }
	dll_export waveOutGetNumDevs(void) { Sleep(194); }
	dll_export waveOutGetPitch(void) { Sleep(195); }
	dll_export waveOutGetPlaybackRate(void) { Sleep(196); }
	dll_export waveOutGetPosition(void) { Sleep(197); }
	dll_export waveOutGetVolume(void) { Sleep(198); }
	dll_export waveOutMessage(void) { Sleep(199); }
	dll_export waveOutOpen(void) { Sleep(200); }
	dll_export waveOutPause(void) { Sleep(201); }
	dll_export waveOutPrepareHeader(void) { Sleep(202); }
	dll_export waveOutReset(void) { Sleep(203); }
	dll_export waveOutRestart(void) { Sleep(204); }
	dll_export waveOutSetPitch(void) { Sleep(205); }
	dll_export waveOutSetPlaybackRate(void) { Sleep(206); }
	dll_export waveOutSetVolume(void) { Sleep(207); }
	dll_export waveOutUnprepareHeader(void) { Sleep(208); }
	dll_export waveOutWrite(void) { Sleep(209); }

	#ifdef __BAK__PlaySound
		#define PlaySound __BAK__PlaySound
		#undef __BAK__PlaySound
	#endif
#endif

}
#pragma endregion