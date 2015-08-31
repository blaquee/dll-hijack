#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>
#include "Common.h"
#include <TlHelp32.h>

namespace SnR_Engine {
	class SnR_Engine;

	ubyte const SearchMode_Search = 0x00;
	ubyte const SearchMode_Skip = 0x01;
	ubyte const SearchMode_Replace = 0x02;
	ubyte const SearchMode_EOF = 0xFF;

	class SnR_Engine
	{
	private:
		uAddr baseAddr;
		uint dataSize;
		void init_module(HMODULE hModule);

	public:
		SnR_Engine();
		SnR_Engine(HMODULE hModule);
		SnR_Engine(uAddr baseAddr, uint bufSize);
		~SnR_Engine();

		uint calcRuleSize(ubyte rule[]);
		uint doSearchAndReplace(ubyte rule[], ubyte replacement[]);
		bool doSearchAndReplaceOnce(ubyte rule[], ubyte replacement[]);
		void doReplace(ubyte *src, ubyte replacement[]);
		bool checkRule(ubyte *src, ubyte rule[]);
		uAddr findNext(uAddr offset, ubyte rule[]);
		uAddr findPrev(uAddr offset, ubyte rule[]);

		void setBufferSize(uint bufferSize)
		{
			dataSize = bufferSize;
		};
		uAddr getBaseAddress()
		{
			return baseAddr;
		};

		uint getBufferSize()
		{
			return dataSize;
		}
	};
}
