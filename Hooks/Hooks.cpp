#include "Hooks.h"
#include "../Memory/Memory.h"
#include "../D3D/d3d.h"
#include "..//SDK/Offsets.h"
#include "..//SDK/Entity.h"
#include "../util/detour.h"

BOOL InitializeOffsets()
{
    uintptr_t StartAddress = Manager->ClientDLL;
    uintptr_t EndAddress = StartAddress + Manager->ClientSize;


    uintptr_t dwEntityList = PatternScan(StartAddress, EndAddress, EntityList, true, false, 3);
    uintptr_t dwViewMatrix = PatternScan(StartAddress, EndAddress, ViewMatrix, true, false, 3);
    uintptr_t dwLocalPlayerController = PatternScan(StartAddress, EndAddress, LocalPlayerController, true, false, 3);
    uintptr_t dwLocalPlayerPawn = PatternScan(StartAddress, EndAddress, LocalPlayerPawn, true, false, 3);
    uintptr_t dwGameRules = PatternScan(StartAddress, EndAddress, GameRules, true, false, 3);



    uintptr_t m_pClippingWeapon = PatternScan(StartAddress, EndAddress, pClippingWeapon, false, true, 3);
    uintptr_t m_vOldOrigin = PatternScan(StartAddress, EndAddress, vOldOrigin, false, true, 4);
    uintptr_t m_iTeamNum = PatternScan(StartAddress, EndAddress, iTeamNum, false, true, 3);
    uintptr_t m_ArmorValue = PatternScan(StartAddress, EndAddress, pArmorValue, false, true, 2);
    uintptr_t m_pGameSceneNode = PatternScan(StartAddress, EndAddress, pGameSceneNode, false, true, 3);
    uintptr_t m_pBoneArray = PatternScan(StartAddress, EndAddress, pBoneArray, false, true, 3);
    uintptr_t m_bPawnIsAlive = PatternScan(StartAddress, EndAddress, bPawnIsAlive, false, true, 3);
    uintptr_t m_iHealth = PatternScan(StartAddress, EndAddress, iHealth, false, true, 2);
    uintptr_t m_HighestEntityIndex = PatternScan(StartAddress, EndAddress, EntityIndex, false, true, 2);




    dwEntityList = dwEntityList - Manager->ClientDLL;
    dwViewMatrix = dwViewMatrix - Manager->ClientDLL;
    dwLocalPlayerController = dwLocalPlayerController - Manager->ClientDLL;
    dwLocalPlayerPawn = (dwLocalPlayerPawn + 0x48) - Manager->ClientDLL;
    dwGameRules = dwGameRules - Manager->ClientDLL;

    if (dwEntityList != 0)
    {
        Offsets->dwEntityList = dwEntityList;
        Offsets->dwViewMatrix = dwViewMatrix;
        Offsets->dwLocalPlayerController = dwLocalPlayerController;
        Offsets->dwLocalPlayerPawn = dwLocalPlayerPawn;
        Offsets->dwGameRules = dwGameRules;

        Offsets->EntitySystem.GetHighestEntityIndex = m_HighestEntityIndex;

        Offsets->PlayerEntity.m_iTeamNum = m_iTeamNum;
        Offsets->PlayerEntity.m_bPawnIsAlive = m_bPawnIsAlive;
        Offsets->PlayerEntity.m_iszPlayerName = m_bPawnIsAlive - 0x1B4;
        Offsets->PlayerEntity.m_hPlayerPawn = m_bPawnIsAlive - 0x8;
        Offsets->PlayerEntity.m_iHealth = m_iHealth;
        Offsets->PlayerEntity.m_pGameSceneNode = m_pGameSceneNode;
        Offsets->PlayerEntity.m_pBoneArray = m_pBoneArray;


        Offsets->PlayerPawn.m_vOldOrigin = m_vOldOrigin;
        Offsets->PlayerPawn.m_pClippingWeapon = m_pClippingWeapon;
        Offsets->PlayerPawn.m_ArmorValue = m_ArmorValue;

        return TRUE;
    }

    return FALSE;
}


BOOL InitializeRender() 
{
    uintptr_t OverlayBegin = Manager->GetOverlayDLL();
    uintptr_t OverlayEnd = OverlayBegin + Manager->GetOverlaySize();
    std::string PresentPattern = xorstr_("48 8B ?? ?? ?? ?? ?? 48 89 ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 89 ?? ?? ?? 89 74");
    uintptr_t AOBPresent = PatternScan(OverlayBegin, OverlayEnd, PresentPattern, true, false, 3);

    if (AOBPresent == 0)
        return FALSE; 

    uintptr_t pSwapChain = AOBPresent + 0x20;

    uintptr_t SwapChain = *reinterpret_cast<uintptr_t*>(pSwapChain);

    void** VirtualTableEntry = *reinterpret_cast<void***>(SwapChain);

    oPresent = reinterpret_cast<Present>(VirtualTableEntry[8]);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)oPresent, hkPresent);
    DetourTransactionCommit();
    
    return TRUE;
}


