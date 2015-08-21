#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>
#include "Common.h"
#include <TlHelp32.h>
#include <forward_list>

namespace SnR_Engine {
	class SnR_Engine; // just tell the compiler to expect a class def

	ubyte const SearchMode_Search  = '\x00';
	ubyte const SearchMode_Skip    = '\x01';
	ubyte const SearchMode_Replace = '\x02';
	ubyte const SearchMode_EOF     = '\xFF';

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

		uint SnR_Engine::calcRuleSize(ubyte rule[]);
		uint doSearchAndReplace(ubyte rule[], ubyte replacement[]);
		void doReplace(ubyte *src, ubyte replacement[]);
		bool checkRule(ubyte *src, ubyte rule[]);

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
