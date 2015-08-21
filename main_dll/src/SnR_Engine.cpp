#include "SnR_Engine.h"
#include <forward_list>

namespace SnR_Engine {
	SnR_Engine::SnR_Engine()
	{
		init_module(GetModuleHandle(nullptr));
	}


	SnR_Engine::~SnR_Engine()
	{
		// Do nothing :3
	}

	uint SnR_Engine::doSearchAndReplace(ubyte rule[], ubyte replacement[])
	{
#ifdef _DEBUG
#ifdef _WIN64
		printf("Search range[size: %d]: %016llx ~ %016llx\n", dataSize, baseAddr, baseAddr + dataSize);
#else
		printf("Search range[size: %d]: %08x ~ %08x\n", dataSize, baseAddr, baseAddr + dataSize);
#endif
#endif
		uint count = 0;
		ubyte *src = reinterpret_cast<ubyte *>(baseAddr);
		ubyte *eof = src + dataSize;
		while (src < eof)
		{
			if (checkRule(src, rule))
			{
				doReplace(src, replacement);
				count++;
			}

			src++;
		}

		return count;
	}

	void SnR_Engine::doReplace(ubyte *src, ubyte replacement[])
	{
#ifdef _DEBUG
#ifdef _WIN64
		printf("Apply Patch at %016llx\n", reinterpret_cast<uAddr>(src));
#else
		printf("Apply Patch at %08x\n", reinterpret_cast<uAddr>(src));
#endif
#endif

		ubyte *lpReplc = replacement;
		uint len;
		DWORD oldProtect;

		while (true)
		{
			switch (*(lpReplc++))
			{
			case SearchMode_Skip:
				src += *(lpReplc++);
				break;

			case SearchMode_Replace:
				len = *(lpReplc++);
				VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);
				memcpy(src, lpReplc, len);
				VirtualProtect(src, len, oldProtect, &oldProtect);
				src += len;
				lpReplc += len;
				break;

			case SearchMode_EOF:
				return;

			default:
#ifdef _DEBUG
				printf("Unexpected char: %c", *(lpReplc - 1));
#endif
				return;
			}
		}
	}

	bool SnR_Engine::checkRule(ubyte *src, ubyte rule[])
	{
		uint len;
		ubyte *lpRule = rule;
		while (true)
		{
			switch (*(lpRule++))
			{
			case SearchMode_Skip:
				src += *(lpRule++);
				break;

			case SearchMode_Search:
				// ??????
				len = *(lpRule++);
				if (memcmp(src, lpRule, len) != 0)
					return false;

				lpRule += len;
				src += len;

				break;

			case SearchMode_EOF:
				return true;

			default:
#ifdef _DEBUG
				printf("Unexpected char: %c", *(lpRule - 1));
#endif
				return false;
			}
		}
	}

	SnR_Engine::SnR_Engine(HMODULE hModule)
	{
		init_module(hModule);
	}

	SnR_Engine::SnR_Engine(uAddr baseAddr, uint bufSize)
	{
		this->baseAddr = baseAddr;
		this->dataSize = bufSize;
	}

	void SnR_Engine::init_module(HMODULE hModule)
	{
		this->baseAddr = reinterpret_cast<uAddr>(hModule);
		
		MODULEINFO modInfo;
		GetModuleInformation(reinterpret_cast<HANDLE>(-1), hModule, &modInfo, sizeof MODULEINFO);
		this->dataSize = modInfo.SizeOfImage;
	}

}