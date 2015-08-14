#ifndef UNICODE
#define UNICODE
#endif

#pragma comment(lib,"version")
#include <stdio.h>
#include <Windows.h>

#define byte unsigned char

int main()
{
	wchar_t *szPath = new wchar_t[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	int size = GetFileVersionInfoSize(szPath, NULL);
	byte *sBuffer = new byte[size];
	GetFileVersionInfo(szPath, NULL, size, sBuffer);

	// Code below taken from:
	// http://www.tech-archive.net/Archive/VC/microsoft.public.vc.language/2006-01/msg00151.html
	VS_FIXEDFILEINFO *lpFfi;
	UINT uLen;
	VerQueryValue(sBuffer, L"\\", reinterpret_cast<LPVOID *>(&lpFfi), &uLen);
	DWORD dwFileVersionMS = lpFfi->dwFileVersionMS;
	DWORD dwFileVersionLS = lpFfi->dwFileVersionLS;

	DWORD dwLeftMost = HIWORD(dwFileVersionMS);
	DWORD dwSecondLeft = LOWORD(dwFileVersionMS);
	DWORD dwSecondRight = HIWORD(dwFileVersionLS);
	DWORD dwRightMost = LOWORD(dwFileVersionLS);
	wprintf(L"File: %s\nVersion: %d.%d.%d.%d\n", szPath, dwLeftMost, dwSecondLeft, dwSecondRight, dwRightMost);
	free(szPath);
	free(sBuffer);

	system("pause");
	return 0;
}