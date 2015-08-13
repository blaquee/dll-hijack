#pragma once
#include "CommonUnits.h"

void WriteRelativeAddress(uint address, int content);

template<typename T>
void WriteMemory(uint, T);

void WriteInt(uint address, int value);
void WriteByte(uint address, ubyte value);