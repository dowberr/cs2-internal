#include <Windows.h>
#include "d3d.h"
#include "Hooks/Hooks.h"
#include "SDK/Offsets.h"
#include "SDK/Entity.h"
#include "DbgHelp.h"
#include <intrin.h> // For _mm_unpacklo_ps
#include <random>
#include "Util/detour.h"

void AllocateConsole()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
}


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        AllocateConsole();
        GameModules();
        if(InitializeRender())
            InitializeOffsets();

        Entity->UpdateEntity();

    }

    return TRUE;
}