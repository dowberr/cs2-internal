#pragma once
#include "../imgui/imgui.h"
#include "../SDK/Bone.h"
#include "../util/xor.hpp"
namespace MenuConfig
{
	inline bool ShowBoneESP = true;
	inline bool ShowBoxESP = true;
	inline bool ShowHealthBar = true;
	inline int SelectedHotkey;
	inline bool ShowWeaponESP = true;
	inline int SelectedBoneIndex;
	inline bool TriggerBot = false;
	inline bool EnableAimbot = false;
	inline bool ShowDistance = true;
	inline bool ItemESP = true;
	inline bool ShowPlayerName = true;
	inline bool VisibleCheck = true;
	inline bool DrawFOV = false;
	inline ImColor BoneColor = ImColor(255, 255, 255, 255);
	inline ImColor BoxColor = ImColor(255, 0, 0, 255);
	inline ImColor ItemColor = ImColor(240, 115, 200, 215);
	inline ImColor NameColor = ImColor(235, 125, 10, 255);
	inline ImColor WeaponColor = ImColor(155, 165, 90, 200);
	inline ImColor DistanceColor = ImColor(135, 150, 240, 220);

	inline float BoxThickness = 1.0f;
	inline float BoneThickness = 1.0f;
	inline float AimSmoothness = 1.0f;
	inline bool ClosestBone = false;
	inline float TriggerBotDelay = 0.050;
	inline float FOV = 100.0f;;
	inline bool ShowMenu = true;
	inline bool TeamCheck = true;
	inline int  BoxType = 0;
	inline int MenuState = 1;
	inline bool AntiFlashbang = true;
	inline bool ShowArmor = false;
	inline bool ShowTimer = true;
	inline int CurrentBoneIndex = 6;

}

inline int CurrentHotkey;


inline const char* HotKeys[] =
{
		"Shift",   "Alt" ,
		"Left Mouse Button" ,   "Right Mouse Button" ,   "Middle Mouse Button" ,
		"Mouse Button 4" ,   "Mouse Button 5"
};
inline int HotKeyCodes[] =
{
	VK_SHIFT, // "Shift"
	VK_MENU,  // "Alt"
	VK_LBUTTON, // "Left Mouse Button"
	VK_RBUTTON, // "Right Mouse Button"
	VK_MBUTTON, // "Middle Mouse Button"
	VK_XBUTTON1, // "Mouse Button 4"
	VK_XBUTTON2, // "Mouse Button 5"
};


inline const char* Bones[] =
{
	"Head", // head = 6
	"Neck", // neck_0 = 5
	"Spine 1", // spine_1 = 4
	"Spine 2", // spine_2 = 2
	"Pelvis", // pelvis = 0
	"Left Upper Arm", // arm_upper_L = 8
	"Left Lower Arm", // arm_lower_L = 9
	"Left Hand", // hand_L = 10
	"Right Upper Arm", // arm_upper_R = 13
	"Right Lower Arm", // arm_lower_R = 14
	"Right Hand", // hand_R = 15
	"Left Upper Leg", // leg_upper_L = 22
	"Left Lower Leg", // leg_lower_L = 23
	"Left Ankle", // ankle_L = 24
	"Right Upper Leg", // leg_upper_R = 25
	"Right Lower Leg", // leg_lower_R = 26
	"Right Ankle" // ankle_R = 27
};

inline BONEINDEX BoneIndexMap[] =
{
	BONEINDEX::head,        // 0 "Head" maps to BONEINDEX::head (6)
	BONEINDEX::neck_0,      // 1 "Neck" maps to BONEINDEX::neck_0 (5)
	BONEINDEX::spine_1,     // 2 "Spine 1" maps to BONEINDEX::spine_1 (4)
	BONEINDEX::spine_2,     // 3 "Spine 2" maps to BONEINDEX::spine_2 (2)
	BONEINDEX::pelvis,      // 4 "Pelvis" maps to BONEINDEX::pelvis (0)
	BONEINDEX::arm_upper_L, // 5 "Left Upper Arm" maps to BONEINDEX::arm_upper_L (8)
	BONEINDEX::arm_lower_L, // 6 "Left Lower Arm" maps to BONEINDEX::arm_lower_L (9)
	BONEINDEX::hand_L,      // 7 "Left Hand" maps to BONEINDEX::hand_L (10)
	BONEINDEX::arm_upper_R, // 8 "Right Upper Arm" maps to BONEINDEX::arm_upper_R (13)
	BONEINDEX::arm_lower_R, // 9 "Right Lower Arm" maps to BONEINDEX::arm_lower_R (14)
	BONEINDEX::hand_R,      // 10 "Right Hand" maps to BONEINDEX::hand_R (15)
	BONEINDEX::leg_upper_L, // 11 "Left Upper Leg" maps to BONEINDEX::leg_upper_L (22)
	BONEINDEX::leg_lower_L, // 12 "Left Lower Leg" maps to BONEINDEX::leg_lower_L (23)
	BONEINDEX::ankle_L,     // 13 "Left Ankle" maps to BONEINDEX::ankle_L (24)
	BONEINDEX::leg_upper_R, // 14 "Right Upper Leg" maps to BONEINDEX::leg_upper_R (25)
	BONEINDEX::leg_lower_R, // 15 "Right Lower Leg" maps to BONEINDEX::leg_lower_R (26)
	BONEINDEX::ankle_R      // 16 "Right Ankle" maps to BONEINDEX::ankle_R (27)
};