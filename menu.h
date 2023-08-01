#pragma once
#include "imgui.h"
#include "crypt.h"

namespace Settings {

    extern bool bShowMenu;
    extern bool bESP;
    extern bool bDistance;
    extern bool bNames;
    extern bool bBones;
    extern bool bHealth;
    extern bool bEndCheat;
    extern bool bFriendlies;
    extern bool bCrossHair;
    extern bool bLegit;
    extern bool bUAV;
    extern bool bDebug;
    extern bool bVisuals;
    extern bool bSnapLines;
    extern int Distance;
    extern int ScreenshotsTaken;
    extern int TypeOfESP;
    extern float CrossHairSize;
    extern float PaddingSize;
    extern float AspectRatio;
    extern ImVec4 BoxColor;
    extern ImVec4 NameColor;
    extern ImVec4 DistanceColor;
    extern ImVec4 BonesColor;
    extern ImVec4 FriendlyColor;
    extern ImVec4 CrossHairColor;
    extern ImVec4 CloseLineColor;
    extern ImVec4 FarLineColor;
}










namespace Menu {

	void SetupTheme();

	void RenderMenu();

    void Visuals();

    void ColorSettings();

    void Legit();

    void Misc();

    void DebugMode();

    void ToggleButton(const char* str_id, bool* v);
}

