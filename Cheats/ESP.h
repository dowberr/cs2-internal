#pragma once
#include "../SDK/Entity.h"
#include "../SDK/W2S.h"
#include "../GuiSDK/MenuConfig.hpp"

inline ImVec4 Rect;
ImVec4 Get2DBox(CEntity& PlayerEntity);
BOOL RenderESP(CEntity& Entity);
VOID GetItemText(WeaponEntity& Weapon);
VOID DrawFov();
