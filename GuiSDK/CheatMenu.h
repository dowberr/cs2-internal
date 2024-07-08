#pragma once
#include "MenuConfig.hpp"
#include "../util/xor.hpp"
#include "Windows.h"

VOID MenuESP()
{
    ImGui::PushItemWidth(220.0f);

    ImGui::Checkbox(xorstr_("Enable Box ESP"), &MenuConfig::ShowBoxESP);
    ImGui::SameLine(190);
    ImGui::ColorEdit4(xorstr_("##BoxColor"), reinterpret_cast<float*>(&MenuConfig::BoxColor), ImGuiColorEditFlags_NoInputs);
    ImGui::SameLine(240);
    ImGui::Text(xorstr_("Thickness:"));
    ImGui::SameLine(330);
    ImGui::SliderFloat(xorstr_("##BoxThickness"), &MenuConfig::BoxThickness, 0.5f, 2.0f, "%.2f");

    ImGui::Spacing();
    ImGui::Checkbox(xorstr_("Enable Bone ESP"), &MenuConfig::ShowBoneESP);
    ImGui::SameLine(190);
    ImGui::ColorEdit4(xorstr_("##BoneColor"), reinterpret_cast<float*>(&MenuConfig::BoneColor), ImGuiColorEditFlags_NoInputs);
    ImGui::SameLine(240);
    ImGui::Text(xorstr_("Thickness:"));
    ImGui::SameLine(330);
    ImGui::SliderFloat(xorstr_("##BoneThickness"), &MenuConfig::BoneThickness, 0.5f, 2.0f, "%.2f");

    ImGui::Spacing();
    ImGui::Checkbox(xorstr_("Team Check"), &MenuConfig::TeamCheck);
    ImGui::Checkbox(xorstr_("Health ESP"), &MenuConfig::ShowHealthBar);
    ImGui::Checkbox(xorstr_("Distance ESP"), &MenuConfig::ShowDistance);
    ImGui::SameLine(190);
    ImGui::ColorEdit4(xorstr_("##DistanceColor"), reinterpret_cast<float*>(&MenuConfig::DistanceColor), ImGuiColorEditFlags_NoInputs); // to edit
    ImGui::Checkbox(xorstr_("Weapon ESP"), &MenuConfig::ShowWeaponESP);
    ImGui::SameLine(190);
    ImGui::ColorEdit4(xorstr_("##WeaponColor"), reinterpret_cast<float*>(&MenuConfig::WeaponColor), ImGuiColorEditFlags_NoInputs); // to edit

    ImGui::Checkbox(xorstr_("Name ESP"), &MenuConfig::ShowPlayerName);
    ImGui::SameLine(190);
    ImGui::ColorEdit4(xorstr_("##NameColor"), reinterpret_cast<float*>(&MenuConfig::NameColor), ImGuiColorEditFlags_NoInputs); // to edit

    ImGui::Checkbox(xorstr_("Item ESP"), &MenuConfig::ItemESP);
    ImGui::SameLine(190);
    ImGui::ColorEdit4(xorstr_("##ItemColor"), reinterpret_cast<float*>(&MenuConfig::ItemColor), ImGuiColorEditFlags_NoInputs);
    ImGui::PopItemWidth();


}


VOID MenuAimBot()
{
    ImGui::Checkbox(xorstr_("AimBot"), &MenuConfig::EnableAimbot);
    ImGui::SliderFloat(xorstr_("Smoothness"), &MenuConfig::AimSmoothness, 1.0f, 10.0f);
    ImGui::SliderFloat(xorstr_("FOV"), &MenuConfig::FOV, 1.0f, 1000.0f);
    ImGui::Checkbox(xorstr_("Draw FOV"), &MenuConfig::DrawFOV);
    ImGui::Checkbox(xorstr_("Aim Closest Bone"), &MenuConfig::ClosestBone);



    if (ImGui::Combo(xorstr_("Bone Selection"), &MenuConfig::CurrentBoneIndex, Bones, IM_ARRAYSIZE(Bones)))
    {
        BONEINDEX SelectedBoneIndex = BoneIndexMap[MenuConfig::CurrentBoneIndex];
        MenuConfig::SelectedBoneIndex = SelectedBoneIndex;
    }

    if (ImGui::Combo(xorstr_("AimKey"), &CurrentHotkey, HotKeys, IM_ARRAYSIZE(HotKeys)))
        MenuConfig::SelectedHotkey = CurrentHotkey;

}


VOID TriggerBotMenu()
{
    ImGui::Checkbox(xorstr_("Enable TriggerBot"), &MenuConfig::TriggerBot);
    ImGui::SliderFloat(xorstr_("TriggerBot Delay/ms"), &MenuConfig::TriggerBotDelay, 1.0f, 1000.0f);
    ImGui::Checkbox(xorstr_("bh"), &MenuConfig::TriggerBot);
}


VOID RenderUI()
{
    ImVec4 backgroundColor = ImVec4(0.05f, 0.1f, 0.15f, 1.0f);
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.3f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.3f, 0.2f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, backgroundColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, backgroundColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, backgroundColor);

    ImGui::SetNextWindowSize(ImVec2(650, 470));
    ImGui::SetNextWindowBgAlpha(0.90f);

    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoResize;

    if (MenuConfig::ShowMenu)
    {
        ImGui::Begin(xorstr_("dowber internal"), nullptr, WindowFlags);

        if (ImGui::Button(xorstr_("ESP"), ImVec2(185, 0))) MenuConfig::MenuState = 1;
        ImGui::SameLine();
        if (ImGui::Button(xorstr_("Aimbot"), ImVec2(185, 0))) MenuConfig::MenuState = 2;
        ImGui::SameLine();
        if (ImGui::Button(xorstr_("TriggerBot"), ImVec2(185, 0))) MenuConfig::MenuState = 3;

        ImVec2 menuSize = ImVec2(650, 470);
        switch (MenuConfig::MenuState)
        {
        case 1:
            ImGui::SetWindowSize(menuSize);
            MenuESP();
            break;
        case 2:
            ImGui::SetWindowSize(menuSize);
            MenuAimBot();
            break;
        case 3:
            ImGui::SetWindowSize(menuSize);
            TriggerBotMenu();
            break;
        default:
            break;
        }

        ImGui::End();
    }

    ImGui::PopStyleColor(3);
}