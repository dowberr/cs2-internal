#include "Aimbot.h"
#include "../GuiSDK/MenuConfig.hpp"
#include "../SDK/W2S.h"

VOID CalculateAimbot(CEntity& PlayerEntity)
{
    if (MenuConfig::EnableAimbot)
    {
        MousePos = ImGui::GetMousePos();

        if (MenuConfig::ClosestBone)
        {
            float MinDistanceToMouse = FLT_MAX;
            Vec3 ClosestBoneWorldPos;
            Vec2 ClosestBoneScreenPos;

            for (int i = 0; i < PlayerEntity.BoneData.BonePosList.size(); ++i)
            {
                Vec3 BoneWorldPos = PlayerEntity.BoneData.BonePosList[i].Pos;
                Vec2 BoneScreenPos;
                if (View.WorldToScreen(BoneWorldPos, BoneScreenPos))
                {
                    float Distance = MousePos.DistanceTo(BoneScreenPos);
                    if (Distance < MinDistanceToMouse)
                    {
                        MinDistanceToMouse = Distance;
                        ClosestBoneWorldPos = BoneWorldPos;
                        ClosestBoneScreenPos = BoneScreenPos;
                    }
                }
            }

            float DistanceToLocal = LocalController.vOldOrigin.DistanceTo(PlayerEntity.Pawn.vOldOrigin);
            float Distance2_Meters = DistanceToLocal * 0.0254;

            if (Distance2_Meters < ClosestDistance && MinDistanceToMouse <= MenuConfig::FOV)
            {
                ClosestDistance = Distance2_Meters;
                AimAtPosition = ClosestBoneScreenPos;
            }
        }

        else
        {
            Vec3 EnemySelectedBoneWorldPos = PlayerEntity.BoneData.BonePosList[MenuConfig::SelectedBoneIndex].Pos;
            Vec2 EnemyHeadScreenPos;

            if (View.WorldToScreen(EnemySelectedBoneWorldPos, EnemyHeadScreenPos))
            {
                float DistanceToLocal = LocalController.vOldOrigin.DistanceTo(PlayerEntity.Pawn.vOldOrigin);

                float Distance = MousePos.DistanceTo(EnemyHeadScreenPos);

                if (DistanceToLocal < ClosestDistance && Distance <= MenuConfig::FOV)
                {
                    ClosestDistance = DistanceToLocal;
                    AimAtPosition = EnemyHeadScreenPos;
                }
            }
        }
    }
}

VOID Aimbot()
{
    if (MenuConfig::EnableAimbot)
    {

        if (ClosestDistance != 1000000.0f && GetAsyncKeyState(HotKeyCodes[MenuConfig::SelectedHotkey]) & 0x8000)
        {
            float SmoothFactor = 1.0f / MenuConfig::AimSmoothness;
            float DeltaX = (AimAtPosition.x - MousePos.x) * SmoothFactor;
            float DeltaY = (AimAtPosition.y - MousePos.y) * SmoothFactor;

            INPUT input = { 0 };
            input.type = INPUT_MOUSE;
            input.mi.dx = DeltaX;
            input.mi.dy = DeltaY;
            input.mi.dwFlags = MOUSEEVENTF_MOVE;

            SendInput(1, &input, sizeof(INPUT));
        }

        ClosestDistance = 1000000.0f;
        AimAtPosition = Vec2(0.0f, 0.0f);
    }
}