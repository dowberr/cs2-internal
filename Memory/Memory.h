#pragma once
#include <Windows.h>
#include "Memory.h"
#include "string"
#include "vector"

BOOL ConvertHexToByteArray(const std::string& HexString, std::vector<uint8_t>& ByteArray);
uintptr_t PatternScan(uintptr_t Start, uintptr_t End, const std::string& Pattern, bool ExtractAddress, bool ExtractOffset, int OffsetPosition);
void** CopyVirtualTable(void** Original);
PVOID* ChangeVirtualTable(PVOID ClassInstance, PVOID* Table);


template <typename ReadType>
BOOL ReadMemoryInternal(DWORD64 Address, ReadType& Value, size_t Size) 
{
    if (Address == 0 || Address < 0xAA70AA8 || Address > 0x7FFD5DC070000)
        return false;

    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(reinterpret_cast<LPCVOID>(Address), &mbi, sizeof(mbi)) == 0)
        return false;

    if (mbi.State != MEM_COMMIT)
        return false;

    if ((mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) || !(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)))
        return false;

    if (Size > (static_cast<size_t>(mbi.RegionSize) - (Address - reinterpret_cast<DWORD64>(mbi.BaseAddress))))
        return false;

    memcpy(&Value, reinterpret_cast<void*>(Address), Size);

    return true;
}