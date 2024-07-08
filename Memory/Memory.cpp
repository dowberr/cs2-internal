#pragma once

#include "Memory.h"



BOOL ConvertHexToByteArray(const std::string& HexString, std::vector<uint8_t>& ByteArray) 
{
    ByteArray.clear();
    size_t len = HexString.length();
    for (size_t i = 0; i < len; i += 3) {
        std::string ByteString = HexString.substr(i, 2);
        if (ByteString == "??" || ByteString == "?? ")
            ByteArray.push_back(0);

        else 
        {
            char* End;
            long byte = std::strtol(ByteString.c_str(), &End, 16);

            if (*End != 0)
                return FALSE; 

            ByteArray.push_back(static_cast<uint8_t>(byte));
        }
    }

    return TRUE;
}

uintptr_t PatternScan(uintptr_t Start, uintptr_t End, const std::string& Pattern, bool ExtractAddress, bool ExtractOffset, int OffsetPosition) 
{
    std::vector<uint8_t> PatternBytes;
    if (!ConvertHexToByteArray(Pattern, PatternBytes))
        return 0;

    uintptr_t rangeEnd = End - PatternBytes.size();
    for (uintptr_t Address = Start; Address <= rangeEnd; ++Address) {
        bool Match = true;
        for (size_t i = 0; i < PatternBytes.size(); ++i) {
            if (PatternBytes[i] == 0x00) continue;
            if (*(uint8_t*)(Address + i) != PatternBytes[i]) {
                Match = false;
                break;
            }
        }

        if (Match) {
            if (ExtractAddress) {
                auto relativeOffset = *reinterpret_cast<int32_t*>(Address + OffsetPosition);
                return Address + 7 + relativeOffset; 
            }

            if (ExtractOffset)
                return *reinterpret_cast<int32_t*>(Address + OffsetPosition); 


            return Address; 
        }
    }

    return 0; 
}

void** CopyVirtualTable(void** Original) 
{
    constexpr int Entries = 1024 / sizeof(void*);

    void** NewVTable = new void* [Entries];

    for (int i = 0; i < Entries; ++i)
        NewVTable[i] = Original[i];


    return NewVTable;
}

PVOID* ChangeVirtualTable(PVOID ClassInstance, PVOID* Table)
{
    PVOID* VirtualTable = *(PVOID**)(ClassInstance);

    if (!VirtualTable)
        return NULL;

    *(PVOID**)(ClassInstance) = Table;

    return VirtualTable;
}