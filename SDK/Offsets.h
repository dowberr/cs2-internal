#pragma once
#include <windows.h>
#include <string>
#include <Psapi.h> 
#include "../Util/xor.hpp"
#pragma comment(lib, "Psapi.lib")

inline std::string EntityList = xorstr_("48 8B ?? ?? ?? ?? ?? 48 89 7C 24 30 8B FA C1 EB 0E 81 E7 FF 3F");
inline std::string ViewMatrix = xorstr_("48 8D ?? ?? ?? ?? ?? 48 C1 E0 06 48 03 C1 C3 CC CC");
inline std::string LocalPlayerController = xorstr_("48 8D ?? ?? ?? ?? ?? 48 8B 04 C1 48 83 C4 28 C3 CC CC CC");
inline std::string LocalPlayerPawn = xorstr_("48 89 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 33 C0 89 0D");
inline std::string GameRules = xorstr_("48 8B ?? ?? ?? ?? ?? 48 85 C0 ?? ?? 45 85 C0");


inline std::string pClippingWeapon = xorstr_("48 8B 87 ?? ?? ?? ?? 48 85 C0 ?? ?? ?? ?? ?? ?? 48 3B C3");
inline std::string vOldOrigin = xorstr_("F2 0F 11 ?? ?? ?? ?? ?? 8B 40 ?? 89 83 ?? ?? 00 00 8B 83 ?? ?? 00 00");
inline std::string iTeamNum = xorstr_("0F B6 86 ?? ?? ?? ?? 48 8B CE 89 47 ?? 48 8B 06");
inline std::string pArmorValue = xorstr_("8B 83 ?? ?? ?? ?? 89 83 ?? ?? ?? ?? 48 83 C4 20 5B C3 CC CC CC CC CC CC");
inline std::string pGameSceneNode = xorstr_("48 8B 81 ?? ?? ?? ?? 48 8B 48 ?? 48 85 C9");
inline std::string pBoneArray = xorstr_("48 8B ?? ?? ?? ?? ?? 4C 8D 43 ?? 4D 8B CE 48 8B CE E8");
inline std::string bPawnIsAlive = xorstr_("0F B6 81 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC B0 01 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 48 89");
inline std::string iHealth = xorstr_("8B 83 ?? ?? ?? ?? 89 83 ?? ?? ?? ?? 48 8b ?? ?? ?? 48 83 C4 20 ?? C3");
inline std::string EntityIndex = xorstr_("89 8F ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? 48 8B 02 48 8B ??");


class ModuleManager {
public:
    ModuleManager() : OverlayDLL(0), ClientDLL(0), EngineDLL(0), OverlaySize(0), ClientSize(0), EngineSize(0) {
        InitializeModules();
    }

    void InitializeModules() {
        OverlayDLL = reinterpret_cast<DWORD64>(GetModuleHandle(xorstr_("gameoverlayrenderer64.dll")));
        ClientDLL = reinterpret_cast<DWORD64>(GetModuleHandle(xorstr_("client.dll")));
        EngineDLL = reinterpret_cast<DWORD64>(GetModuleHandle(xorstr_("engine2.dll")));
        DXGI = reinterpret_cast<DWORD64>(GetModuleHandle(xorstr_("dxgi.dll")));


        OverlaySize = ModuleSize(OverlayDLL);
        ClientSize = ModuleSize(ClientDLL);
        EngineSize = ModuleSize(EngineDLL);
        DXGISize = ModuleSize(DXGI);

    }

    DWORD64 GetOverlayDLL() const {
        return OverlayDLL;
    }

    DWORD64 GetClientDLL() const {
        return ClientDLL;
    }

    DWORD64 GetEngineDLL() const {
        return EngineDLL;
    }

    DWORD64 GetDXGI() const {
        return DXGI;
    }

    static DWORD ModuleSize(DWORD64 Base) {
        if (Base == 0)
            return 0;

        auto DosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(Base);
        auto NTHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(Base + DosHeader->e_lfanew);

        return NTHeaders->OptionalHeader.SizeOfImage;
    }

    DWORD GetOverlaySize() const {
        return OverlaySize;
    }

    DWORD GetClientSize() const {
        return ClientSize;
    }

    DWORD GetEngineSize() const {
        return EngineSize;
    }

    DWORD GetDXGISize() const {
        return DXGISize;
    }

public:
    DWORD64 OverlayDLL;
    DWORD64 ClientDLL;
    DWORD64 EngineDLL;
    DWORD64 DXGI;

    DWORD OverlaySize;
    DWORD ClientSize;
    DWORD EngineSize;
    DWORD DXGISize;

};

class GameData 
{
public:
    DWORD64 dwEntityList;
    DWORD64 dwViewMatrix;
    DWORD64 dwLocalPlayerController;
    DWORD64 dwLocalPlayerPawn;
    DWORD64 dwGameRules;

    struct CBasePlayerController
    {
        DWORD m_iTeamNum;
        DWORD m_bPawnIsAlive;
        DWORD m_iszPlayerName;
        DWORD m_hPlayerPawn;
        DWORD m_iHealth;
        DWORD m_pGameSceneNode;
        DWORD m_pBoneArray;
    }PlayerEntity;

    struct C_BasePlayerPawn
    {
        DWORD m_vOldOrigin;
        DWORD m_pClippingWeapon;
        DWORD m_ArmorValue;
    }PlayerPawn;

    struct dwGameEntitySystem
    {
        DWORD GetHighestEntityIndex;
    }EntitySystem;
};

inline ModuleManager* Manager = new ModuleManager;
inline GameData* Offsets = new GameData;
VOID GameModules();
