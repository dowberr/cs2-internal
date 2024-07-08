#pragma once
#include "Windows.h"
#include "../SDK/Entity.h"


inline  Vec2 MousePos;
inline  float ClosestDistance = 1000000.0f;
inline  Vec2 AimAtPosition = { 0, 0 };
VOID Aimbot();
VOID CalculateAimbot(CEntity& Entity);
