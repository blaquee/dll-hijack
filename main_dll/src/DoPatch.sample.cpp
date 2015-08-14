#include "Common.h"
#include "DoPatch.h"
#include "PatchUtil.h"

uAddr hBase;

void do_patch(HMODULE base) {
	hBase = reinterpret_cast<uAddr>(base);
	
	// Your code here ..
}