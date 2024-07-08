#pragma once
#include "ESP.h"



ImVec4 Get2DBox(CEntity& PlayerEntity)
{
	const float FixedPlayerHeight = 75.0f;
	Vec3 TopPos3D;

	TopPos3D = PlayerEntity.Pawn.vOldOrigin;
	TopPos3D.z += FixedPlayerHeight;

	Vec2 GroundPos2D, TopPos2D;
	if (!View.WorldToScreen(PlayerEntity.Pawn.vOldOrigin, GroundPos2D) || !View.WorldToScreen(TopPos3D, TopPos2D))
		return ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };

	if (TopPos2D.y < 0.0f || GroundPos2D.y > ImGui::GetIO().DisplaySize.y)
		return ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };


	Vec2 Size, Pos;
	Size.y = std::abs(TopPos2D.y - GroundPos2D.y);
	Size.x = Size.y * 0.60;

	Pos.x = GroundPos2D.x - Size.x / 2;
	Pos.y = TopPos2D.y;

	return ImVec4{ Pos.x, Pos.y, Size.x, Size.y };
}

VOID GetItemText(WeaponEntity& Weapon)
{
	if (!View.WorldToScreen(Weapon.ItemPos3D, Weapon.ItemPos2D))
		return;

	if (Weapon.ItemPos2D.x < 0.0f || Weapon.ItemPos2D.x > ImGui::GetIO().DisplaySize.x ||
		Weapon.ItemPos2D.y < 0.0f || Weapon.ItemPos2D.y > ImGui::GetIO().DisplaySize.y)
		return;

	if (Weapon.ItemPos3D.x == 0 || Weapon.ItemPos3D.y == 0 || Weapon.ItemPos3D.z == 0)
		return;

	if (MenuConfig::ItemESP)
	{
		if (!Weapon.pClippingWeapon.empty())
			Weapon.pClippingWeapon[0] = toupper(Weapon.pClippingWeapon[0]);

		ImGui::GetBackgroundDrawList()->AddText(ImVec2(Weapon.ItemPos2D.x, Weapon.ItemPos2D.y), MenuConfig::ItemColor, Weapon.pClippingWeapon.c_str());
	}

}

void DrawBone(CEntity& Entity, ImColor Color, float Thickness)
{
	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	Vec3 EntityPos = Entity.Pawn.vOldOrigin;
	Vec3 BonePos27 = Entity.BoneData.BonePosList[27].Pos;

	float MaxDistance = 45.0f;
	float DistanceToBone27 = sqrt(
		pow(BonePos27.x - EntityPos.x, 2) +
		pow(BonePos27.y - EntityPos.y, 2) +
		pow(BonePos27.z - EntityPos.z, 2)
	);

	if (DistanceToBone27 > MaxDistance)
		return;

	for (const auto& boneList : BoneJointList::List)
	{
		BoneJointPos Previous;
		bool isFirstBone = true;
		for (const auto& Index : boneList)
		{
			BoneJointPos Current = Entity.BoneData.BonePosList[Index];
			if (!isFirstBone && Previous.IsVisible && Current.IsVisible)
			{
				draw_list->AddLine(
					ImVec2(Previous.ScreenPos.x, Previous.ScreenPos.y),
					ImVec2(Current.ScreenPos.x, Current.ScreenPos.y),
					Color, Thickness
				);
			}
	

			Vec3 HeadPos = Entity.BoneData.BonePosList[BoneJointList::List.front().front()].Pos;
			float SquaredDistanceToHead = pow(HeadPos.x - LocalController.vOldOrigin.x, 2) +
				pow(HeadPos.y - LocalController.vOldOrigin.y, 2) +
				pow(HeadPos.z - LocalController.vOldOrigin.z, 2);

			float DistanceToHead = sqrt(SquaredDistanceToHead) * 0.0254f;

			if (DistanceToHead < 35.0f)
			{

				if (Index == BONEINDEX::head && Current.IsVisible)
				{
					BoneJointPos Head = Entity.BoneData.BonePosList[BoneJointList::List.front().front()];
					draw_list->AddCircle(
						ImVec2(Head.ScreenPos.x, Head.ScreenPos.y - 6.5f),
						9.0f, Color, 12, Thickness
					);
				}

			}

			isFirstBone = false;
			Previous = Current;
		}
	}
}


void DrawCustomBox(const ImVec2& rect_min, const ImVec2& rect_max, const ImColor& boxColor, float borderThickness) {
	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

	float cornerLength = 10.0f; 

	draw_list->AddLine(ImVec2(rect_min.x, rect_min.y + cornerLength), rect_min, boxColor, borderThickness);
	draw_list->AddLine(ImVec2(rect_min.x + cornerLength, rect_min.y), rect_min, boxColor, borderThickness);

	draw_list->AddLine(ImVec2(rect_max.x, rect_min.y + cornerLength), ImVec2(rect_max.x, rect_min.y), boxColor, borderThickness);
	draw_list->AddLine(ImVec2(rect_max.x - cornerLength, rect_min.y), ImVec2(rect_max.x, rect_min.y), boxColor, borderThickness);

	draw_list->AddLine(ImVec2(rect_max.x, rect_max.y - cornerLength), ImVec2(rect_max.x, rect_max.y), boxColor, borderThickness);
	draw_list->AddLine(ImVec2(rect_max.x - cornerLength, rect_max.y), ImVec2(rect_max.x, rect_max.y), boxColor, borderThickness);

	draw_list->AddLine(ImVec2(rect_min.x, rect_max.y - cornerLength), ImVec2(rect_min.x, rect_max.y), boxColor, borderThickness);
	draw_list->AddLine(ImVec2(rect_min.x + cornerLength, rect_max.y), ImVec2(rect_min.x, rect_max.y), boxColor, borderThickness);
}


void DrawHealthBar(DWORD EntityAddress, float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size, bool Horizontal = true)
{
	ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

	float Proportion = CurrentHealth / MaxHealth;

	ImColor FirstStageColor = ImColor(96, 246, 113, 220);
	ImColor SecondStageColor = ImColor(247, 214, 103, 220);
	ImColor ThirdStageColor = ImColor(255, 95, 95, 220);
	ImColor BackGroundColor = ImColor(90, 90, 90, 220);
	ImColor Color;
	if (Proportion > 0.5)
		Color = FirstStageColor;
	else if (Proportion > 0.25)
		Color = SecondStageColor;
	else
		Color = ThirdStageColor;

	DrawList->AddRectFilled(Pos, { Pos.x + Size.x, Pos.y + Size.y }, BackGroundColor);

	if (Horizontal)
	{
		DrawList->AddRectFilled(Pos, { Pos.x + Size.x * Proportion, Pos.y + Size.y }, Color);
	}
	else
	{
		float healthHeight = Size.y * Proportion;
		DrawList->AddRectFilled({ Pos.x, Pos.y + Size.y - healthHeight }, { Pos.x + Size.x, Pos.y + Size.y }, Color);
	}

	ImColor BorderColor = ImColor(45, 45, 45, 220);
	DrawList->AddRect(Pos, { Pos.x + Size.x, Pos.y + Size.y }, BorderColor);
}

VOID DrawFov()
{
	if (!MenuConfig::DrawFOV)
		return;

	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	float screenWidth = ImGui::GetIO().DisplaySize.x;
	float screenHeight = ImGui::GetIO().DisplaySize.y;
	ImVec2 center(screenWidth / 2.0f, screenHeight / 2.0f);
	draw_list->AddCircle(center, MenuConfig::FOV, IM_COL32(255, 255, 255, 255), 64, 2.0f);
}



BOOL RenderESP(CEntity& PlayerEntity)
{

	if (MenuConfig::ShowBoneESP)
	{
		DrawBone(PlayerEntity, MenuConfig::BoneColor, MenuConfig::BoneThickness);
	}

	if (MenuConfig::ShowBoxESP)
	{
		ImVec2 Rect_Min = ImVec2(Rect.x, Rect.y);
		ImVec2 Rect_Max = ImVec2(Rect.x + Rect.z, Rect.y + Rect.w);

		DrawCustomBox(Rect_Min, Rect_Max, MenuConfig::BoxColor, MenuConfig::BoxThickness);
	}

	if (MenuConfig::ShowHealthBar)
	{
		ImVec2 HealthBarPos, HealthBarSize;

		HealthBarPos = { Rect.x - 7.0f, Rect.y };
		HealthBarSize = { 7.0f, Rect.w };
		DrawHealthBar(PlayerEntity.EntityAddress, 100, PlayerEntity.Pawn.iHealth, HealthBarPos, HealthBarSize, false);

	}

	if (MenuConfig::ShowPlayerName)
	{
		
		if (!PlayerEntity.Entity.iszPlayerName.empty())
			PlayerEntity.Entity.iszPlayerName[0] = toupper(PlayerEntity.Entity.iszPlayerName[0]);


		ImVec2 TextSize = ImGui::CalcTextSize(PlayerEntity.Entity.iszPlayerName.c_str());
		ImVec2 TextPos = { Rect.x + Rect.z / 2 - TextSize.x / 2, Rect.y - 16 };
		ImGui::GetForegroundDrawList()->AddText(TextPos, MenuConfig::NameColor, PlayerEntity.Entity.iszPlayerName.c_str());
		
	}

	if (MenuConfig::ShowWeaponESP)
	{
		if (!PlayerEntity.Pawn.pClippingWeapon.empty())
			PlayerEntity.Pawn.pClippingWeapon[0] = toupper(PlayerEntity.Pawn.pClippingWeapon[0]);

		ImVec2 textSize = ImGui::CalcTextSize(PlayerEntity.Pawn.pClippingWeapon.c_str());
		ImVec2 textPosition;
		textPosition.x = Rect.x + (Rect.z - textSize.x) * 0.5f;
		textPosition.y = Rect.y + Rect.w;

		ImGui::GetForegroundDrawList()->AddText(textPosition, MenuConfig::WeaponColor, PlayerEntity.Pawn.pClippingWeapon.c_str());
	}

	if (MenuConfig::ShowDistance)
	{
		float DistanceToLocal = LocalController.vOldOrigin.DistanceTo(PlayerEntity.Pawn.vOldOrigin);
		float Distance2_Meters = DistanceToLocal * 0.0254;
	
		int RoundedDistance = static_cast<int>(Distance2_Meters);

		std::string distanceText = std::to_string(RoundedDistance) + "m";

		ImVec2 distanceTextSize = ImGui::CalcTextSize(distanceText.c_str());

		ImVec2 distanceTextPos = { Rect.x + Rect.z / 2 - distanceTextSize.x / 2, Rect.y - 12 - distanceTextSize.y - 3 };

		ImGui::GetForegroundDrawList()->AddText(distanceTextPos, MenuConfig::DistanceColor, distanceText.c_str());
	}

	return TRUE;
}