#include "Entity.h"
#include "Memory.h"
#include "Offsets.h"
#include "../Cheats/ESP.h"
#include "../Cheats/Aimbot.h"
#include "Offsets.h"
#include "../Cheats/TriggerBot.h"


VOID LocalEntity::LocalPlayerController()
{
    ReadMemoryInternal(Manager->ClientDLL + Offsets->dwLocalPlayerController, this->EntityAddress, sizeof(this->EntityAddress));
    ReadMemoryInternal(this->EntityAddress + Offsets->PlayerEntity.m_iTeamNum, this->iTeamNum, sizeof(this->iTeamNum));
}

BOOL CEntity::IsAlive()
{
	return this->Pawn.iHealth > 0 && this->Entity.bPawnIsAlive == 1;
}

BOOL CEntity::TeamCheck()
{
    return this->Entity.iTeamNum == LocalController.iTeamNum;
}

BOOL CEntity::isLocal()
{
    return this->EntityAddress == LocalController.EntityAddress;
}

VOID CEntity::PlayerWeapon()
{
    DWORD64 WeaponAddress;
    ReadMemoryInternal(this->EntityPawnAddress + Offsets->PlayerPawn.m_pClippingWeapon, WeaponAddress, sizeof(WeaponAddress));

    ReadMemoryInternal(WeaponAddress + 0x10, WeaponAddress, sizeof(WeaponAddress));
    ReadMemoryInternal(WeaponAddress + 0x20, WeaponAddress, sizeof(WeaponAddress));

    char WeaponBuffer[32];
    ReadMemoryInternal(WeaponAddress, WeaponBuffer, sizeof(WeaponBuffer) - 1);
    WeaponBuffer[sizeof(WeaponBuffer) - 1] = '\0'; 

    std::string WeaponName(WeaponBuffer);
    std::size_t UnderScoreIndex = WeaponName.find('_');
    if (UnderScoreIndex != std::string::npos)
        WeaponName = WeaponName.substr(UnderScoreIndex + 1);

    this->Pawn.pClippingWeapon = WeaponName; 
}

VOID CEntity::EntityName()
{
    char NameBuffer[21];
    ReadMemoryInternal<char[21]>(this->EntityAddress + Offsets->PlayerEntity.m_iszPlayerName, NameBuffer, 21);
    NameBuffer[20] = '\0';
    this->Entity.iszPlayerName = std::string(NameBuffer);


}

VOID CEntity::UpdatePlayerBoneData()
{

    ReadMemoryInternal<DWORD64>(this->EntityPawnAddress + Offsets->PlayerEntity.m_pGameSceneNode, this->Pawn.GameSceneNode, sizeof(this->Pawn.GameSceneNode));
    ReadMemoryInternal<DWORD64>(this->Pawn.GameSceneNode + Offsets->PlayerEntity.m_pBoneArray, this->Pawn.BoneArray, sizeof(this->Pawn.BoneArray));
    BoneJointData BoneArray[30]{};
    ReadMemoryInternal(this->Pawn.BoneArray, BoneArray, sizeof(BoneArray));

    this->BoneData.BonePosList.clear();

    for (int i = 0; i < 30; ++i)
    {
        Vec2 ScreenPos;
        bool IsVisible = View.WorldToScreen(BoneArray[i].Pos, ScreenPos);
        this->BoneData.BonePosList.push_back({ BoneArray[i].Pos, ScreenPos, IsVisible });
    }

}




VOID GetEntityWeapon()
{

    Entity->EntityWeaponVector.clear();
    ReadMemoryInternal(Entity->EntityList + Offsets->EntitySystem.GetHighestEntityIndex, Weapon->Index, sizeof(Weapon->Index));

    if (Weapon->Index > 1024)
        return;

    for (int i = 64; i < Weapon->Index; i++)
    {
        DWORD64 EntityAddress;

        ReadMemoryInternal<DWORD64>(Entity->EntityList + (i < 512 ? 0x10 : 0x18), EntityAddress, sizeof(EntityAddress));
        ReadMemoryInternal<DWORD64>(EntityAddress + 0x78 * (i & 0x1FF), EntityAddress, sizeof(EntityAddress));

        if (EntityAddress > 0x7FFFFF && EntityAddress != 0 && EntityAddress < 0x7FFD5DC0700000)
            Entity->EntityWeaponVector.push_back(EntityAddress);

    }
}

VOID PlayerEntities()
{
    Entity->EntityPlayerVector.clear();
    Entity->PawnVector.clear();

    for (int i = 1; i < 64; i++)
    {

        DWORD64 EntityPawnAddress;
        ReadMemoryInternal<DWORD64>(Entity->EntityListEntry + i * 0x78, Entity->EntityAddress, sizeof(Entity->EntityAddress));
        ReadMemoryInternal<DWORD>(Entity->EntityAddress + Offsets->PlayerEntity.m_hPlayerPawn, Entity->Entity.PawnIndex, sizeof(Entity->Entity.PawnIndex));
        DWORD64 EntityPawnListEntry;
        ReadMemoryInternal<DWORD64>(Manager->ClientDLL + Offsets->dwEntityList, EntityPawnListEntry, sizeof(EntityPawnListEntry));
        EntityPawnAddress = EntityPawnListEntry + 0x10 + 8 * ((Entity->Entity.PawnIndex & 0x7FFF) >> 9);
        ReadMemoryInternal<DWORD64>(EntityPawnAddress, EntityPawnAddress, sizeof(EntityPawnAddress));
        EntityPawnAddress += 0x78 * (Entity->Entity.PawnIndex & 0x1FF);
        ReadMemoryInternal<DWORD64>(EntityPawnAddress, Entity->EntityPawnAddress, sizeof(Entity->EntityPawnAddress));
        if (Entity->EntityAddress > 0x7FFFFF && Entity->EntityAddress != 0 && Entity->EntityAddress < 0x7FFD5DC07000)
        {
            Entity->EntityPlayerVector.push_back(Entity->EntityAddress);
            Entity->PawnVector.push_back(Entity->EntityPawnAddress);
        }

    }
}

VOID UpdatePlayersESP(CEntity& PlayerEntity)
{
    ReadMemoryInternal<Vec3>(PlayerEntity.EntityPawnAddress + Offsets->PlayerPawn.m_vOldOrigin, PlayerEntity.Pawn.vOldOrigin, sizeof(PlayerEntity.Pawn.vOldOrigin));
    ReadMemoryInternal<int>(PlayerEntity.EntityPawnAddress + Offsets->PlayerEntity.m_iHealth, PlayerEntity.Pawn.iHealth, sizeof(PlayerEntity.Pawn.iHealth));
    ReadMemoryInternal<int>(PlayerEntity.EntityAddress + Offsets->PlayerEntity.m_bPawnIsAlive, PlayerEntity.Entity.bPawnIsAlive, sizeof(PlayerEntity.Entity.bPawnIsAlive));
    ReadMemoryInternal<int>(PlayerEntity.EntityAddress + Offsets->PlayerEntity.m_iTeamNum, PlayerEntity.Entity.iTeamNum, sizeof(PlayerEntity.Entity.iTeamNum));

}

VOID UpdateWeaponESP(WeaponEntity& DroppedWeapon)
{
    char WeaponBuffer[64];
    ReadMemoryInternal<DWORD64>(DroppedWeapon.EntityAddress + 0x10, DroppedWeapon.WeaponAddress, sizeof(DroppedWeapon.WeaponAddress));
    ReadMemoryInternal<DWORD64>(DroppedWeapon.WeaponAddress + 0x20, DroppedWeapon.WeaponAddress, sizeof(DroppedWeapon.WeaponAddress));

    if (ReadMemoryInternal(DroppedWeapon.WeaponAddress, WeaponBuffer, sizeof(WeaponBuffer) - 1))
    {
        WeaponBuffer[sizeof(WeaponBuffer) - 1] = '\0';
        DroppedWeapon.pClippingWeapon = std::string(WeaponBuffer);
        std::size_t UnderScoreIndex = DroppedWeapon.pClippingWeapon.find('_');
        std::size_t ProjectileIndex = DroppedWeapon.pClippingWeapon.find("_projectile");

        if (strncmp(WeaponBuffer, "weapon_", 7) == 0 || strstr(WeaponBuffer, "baseanimgraph") != nullptr || ProjectileIndex != std::string::npos)
        {

            if (UnderScoreIndex != std::string::npos && strncmp(WeaponBuffer, "weapon_", 7) == 0)
                DroppedWeapon.pClippingWeapon = DroppedWeapon.pClippingWeapon.substr(UnderScoreIndex + 1);

            if (strcmp(WeaponBuffer, "baseanimgraph") == 0)
                DroppedWeapon.pClippingWeapon = "Defuse Kit";

            ReadMemoryInternal<DWORD64>(DroppedWeapon.EntityAddress + Offsets->PlayerEntity.m_pGameSceneNode, DroppedWeapon.GameSceneNode, sizeof(DroppedWeapon.GameSceneNode));
            ReadMemoryInternal<Vec3>(DroppedWeapon.GameSceneNode + 0x80, DroppedWeapon.ItemPos3D, sizeof(DroppedWeapon.ItemPos3D));

        }
    }
}


VOID EnableCheats()
{
    PlayerEntities();
    GetEntityWeapon();

    LocalController.LocalPlayerController();

    for (int i = 0; i < Entity->PawnVector.size(); i++)
    {
        CEntity PlayerEntity;
        PlayerEntity.EntityPawnAddress = Entity->PawnVector[i];
        PlayerEntity.EntityAddress = Entity->EntityPlayerVector[i];
        UpdatePlayersESP(PlayerEntity);
        PlayerEntity.EntityName();
        PlayerEntity.PlayerWeapon();
        PlayerEntity.UpdatePlayerBoneData();

        if (PlayerEntity.isLocal())
            ReadMemoryInternal<Vec3>(PlayerEntity.EntityPawnAddress + Offsets->PlayerPawn.m_vOldOrigin, LocalController.vOldOrigin, sizeof(LocalController.vOldOrigin));

        if (PlayerEntity.isLocal())
            continue;

        if (MenuConfig::TeamCheck && PlayerEntity.TeamCheck())
            continue;

        if (!PlayerEntity.IsAlive())
            continue;

        if (PlayerEntity.BoneData.BonePosList.size() == 0)
            continue;


        CalculateAimbot(PlayerEntity);
        Rect = Get2DBox(PlayerEntity);
        RenderESP(PlayerEntity);
    }


   
    for (int i = 0; i < Entity->EntityWeaponVector.size(); i++)
    {
        WeaponEntity WeaponEntity;
        WeaponEntity.EntityAddress = Entity->EntityWeaponVector[i];
        UpdateWeaponESP(WeaponEntity);
        GetItemText(WeaponEntity);
    }
    
    TriggerBot();
    Aimbot();;

}