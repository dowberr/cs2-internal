#include "TriggerBot.h"

VOID TriggerBot()
{
    if (!MenuConfig::TriggerBot)
        return;

    int CurrentHealth;
    bool TriggerShoot = false;
    int TeamID;

    ReadMemoryInternal(Manager->ClientDLL + Offsets->dwLocalPlayerPawn, LocalController.PawnAddress, sizeof(LocalController.PawnAddress));
    ReadMemoryInternal(LocalController.PawnAddress + 0x13B0, LocalController.uHandle, sizeof(LocalController.uHandle));

    if (LocalController.uHandle == -1)
        return;

    DWORD64 EntityPawnAddress;
    EntityPawnAddress = Entity->EntityList + 0x10 + 8 * ((LocalController.uHandle & 0x7FFF) >> 9);
    ReadMemoryInternal<DWORD64>(EntityPawnAddress, EntityPawnAddress, sizeof(EntityPawnAddress));
    EntityPawnAddress += 0x78 * (LocalController.uHandle & 0x1FF);
    ReadMemoryInternal<DWORD64>(EntityPawnAddress, EntityPawnAddress, sizeof(EntityPawnAddress));
    ReadMemoryInternal<int>(EntityPawnAddress + Offsets->PlayerEntity.m_iHealth, CurrentHealth, sizeof(CurrentHealth));
    ReadMemoryInternal<int>(EntityPawnAddress + Offsets->PlayerEntity.m_iTeamNum, TeamID, sizeof(TeamID));

    if (MenuConfig::TeamCheck)
        TriggerShoot = LocalController.iTeamNum != TeamID && CurrentHealth > 0;
    else

        TriggerShoot = CurrentHealth > 0;

    if (!TriggerShoot)
        return;

    static std::chrono::time_point<std::chrono::steady_clock> LastTimePoint = std::chrono::steady_clock::now();
    auto CurTimePoint = std::chrono::steady_clock::now();

    if (CurTimePoint - LastTimePoint >= std::chrono::milliseconds(static_cast<long long>(MenuConfig::TriggerBotDelay)))
    {
        const bool isAlreadyShooting = GetAsyncKeyState(VK_LBUTTON) < 0;

        if (!isAlreadyShooting)
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }

        LastTimePoint = CurTimePoint;
    }
}
