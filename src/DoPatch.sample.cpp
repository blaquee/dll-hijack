#include "DoPatch.h"
#include "PatchUtil.h"

uint hBase;

void do_patch(HMODULE base) {
	hBase = reinterpret_cast<uint>(base);
	
	// Your code here ..
}