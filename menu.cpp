#include "menu.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Render.h"


namespace Settings {
	bool bShowMenu		    = false;
	bool bESP			    = false;
	bool bDistance		    = false;
	bool bNames			    = false;
	bool bBones			    = false;
	bool bHealth		    = false;
	bool bEndCheat		    = false;
	bool bFriendlies	    = false;
    bool bCrossHair         = false;
    bool bLegit             = false;
    bool bUAV               = false;
    bool bDebug             = false;
    bool bVisuals           = false;
    bool bSnapLines         = false;
	int Distance		    = 500;
    int ScreenshotsTaken    = 0;
    int TypeOfESP           = 0;
    float CrossHairSize     = 6.0f;
    float PaddingSize       = 1.0f;
    float AspectRatio       = 0.2f;
    ImVec4 BoxColor		    = { 255, 0, 0, 255 };
    ImVec4 NameColor		= { 1.0f, 1.0f, 1.0f, 1.0f };
    ImVec4 DistanceColor	= { 1.0f, 1.0f, 1.0f, 1.0f };
    ImVec4 BonesColor		= { 1.0f,1.0f,1.0f,1.0f };
    ImVec4 FriendlyColor	= { 0.0f, 1.0f, 0.0f, 1.0f };
    ImVec4 CrossHairColor   = { 1.0f, 0.0f, 0.0f, 1.0f };
    ImVec4 CloseLineColor   = {1.0f, 0.0f, 0.0f, 1.0f};
    ImVec4 FarLineColor     = { 0.0f, 1.0f, 0.0f, 1.0f };

}




namespace Menu {
    WORD SelectedButton = 0;




    

	void SetupTheme() {
        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowTitleAlign = ImVec2(0.5, 0.5);
        style.Colors[ImGuiCol_TitleBg] = ImColor(255, 10, 10);
        style.Colors[ImGuiCol_TitleBgActive] = ImColor(255, 10, 10);
        style.Colors[ImGuiCol_WindowBg] = ImColor(16, 16, 16); //16

        style.Colors[ImGuiCol_ChildBg] = ImColor(19, 19, 19);

        style.WindowMinSize = ImVec2(600, 700);

        style.FramePadding = ImVec2(10, 5);
        style.FrameRounding = 1.0f;
        style.CellPadding = ImVec2(0, 0);


        style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
        style.Colors[ImGuiCol_TextDisabled] = ImColor(255, 0, 0);
        style.Colors[ImGuiCol_SliderGrab] = ImColor(30, 30, 30);
        style.Colors[ImGuiCol_SliderGrabActive] = ImColor(28, 28, 28);

        style.Colors[ImGuiCol_CheckMark] = ImColor(211, 211, 211);

        style.Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
        style.Colors[ImGuiCol_ButtonActive] = ImColor(60, 60, 60, 255);
        style.Colors[ImGuiCol_ButtonHovered] = ImColor(41, 40, 41, 255);

        style.Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 211);

        style.Colors[ImGuiCol_FrameBg] = ImColor(100, 100, 100);
        style.Colors[ImGuiCol_FrameBgHovered] = ImColor(130, 130, 130);
        style.Colors[ImGuiCol_FrameBgActive] = ImColor(211, 211, 211);

        style.Colors[ImGuiCol_HeaderHovered] = ImColor(50, 50, 50);
        style.Colors[ImGuiCol_HeaderActive] = ImColor(50, 50, 50);
        style.Colors[ImGuiCol_Header] = ImColor(50, 50, 50);

	}

	void RenderMenu() {
        SetupTheme();
        ImGui::Begin(("Sirius"), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::PushStyleColor(ImGuiCol_Border, ImColor(150, 150, 150, 150).Value);
        ImVec4 Active = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
        ImVec4 Inactive = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        ImVec4 ActiveBorder = ImGui::GetStyleColorVec4(ImGuiCol_TitleBg);
        ImVec4 InactiveBorder = ImGui::GetStyleColorVec4(ImGuiCol_Text);

        

        ImGui::BeginChild("A", ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
            ImGui::PushStyleColor(ImGuiCol_Border, SelectedButton == 0 ? ActiveBorder : InactiveBorder);
            ImGui::PushStyleColor(ImGuiCol_Button, SelectedButton == 0 ? Active : Inactive);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
            if (ImGui::Button(skCrypt("Visuals"), ImVec2(ImGui::GetContentRegionAvail().x, 40))) {
                SelectedButton = 0;
            }

            ImGui::NewLine();
            ImGui::PushStyleColor(ImGuiCol_Border, SelectedButton == 1 ? ActiveBorder : InactiveBorder);
            ImGui::PushStyleColor(ImGuiCol_Button, SelectedButton == 1 ? Active : Inactive);
            if (ImGui::Button(skCrypt("Legit"), ImVec2(ImGui::GetContentRegionAvail().x, 40))) {
                SelectedButton = 1;
            }
            ImGui::NewLine();
            ImGui::PushStyleColor(ImGuiCol_Border, SelectedButton == 2 ? ActiveBorder : InactiveBorder);
            ImGui::PushStyleColor(ImGuiCol_Button, SelectedButton == 2 ? Active : Inactive);
            if (ImGui::Button(skCrypt("Misc"), ImVec2(ImGui::GetContentRegionAvail().x, 40))) {
                SelectedButton = 2;
            }
            ImGui::NewLine();
            ImGui::PushStyleColor(ImGuiCol_Border, SelectedButton == 3 ? ActiveBorder : InactiveBorder);
            ImGui::PushStyleColor(ImGuiCol_Button, SelectedButton == 3 ? Active : Inactive);
            if (ImGui::Button(skCrypt("Colors / Settings"), ImVec2(ImGui::GetContentRegionAvail().x, 40))) {
                SelectedButton = 3;
            }
            ImGui::NewLine();
            ImGui::PushStyleColor(ImGuiCol_Border, SelectedButton == 4 ? ActiveBorder : InactiveBorder);
            ImGui::PushStyleColor(ImGuiCol_Button, SelectedButton == 4 ? Active : Inactive);
            if (ImGui::Button("TODO......", ImVec2(ImGui::GetContentRegionAvail().x, 40))) {
                SelectedButton = 4;
            }
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(10);
        }
        ImGui::EndChild();

        {
            ImGui::SameLine();
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
            ImGui::SameLine();
        }



        ImGui::BeginChild("B", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        {
            switch (SelectedButton) {
            case 0: // Visuals
            {
                Visuals();
            }break;

            case 1: // Legit Settings
            {
                Legit();
            }break;

            case 2: // Misc
            {
                Misc();
            }break;

            case 3: // Colors / Settings
            {
                ColorSettings();
            }break;

            case 4: // Debug Memory Viewer
            {
                DebugMode();
            }break;

            }
        }
        ImGui::EndChild();

        ImGui::PopStyleColor(1);

        ImGui::End();
	}

    void Visuals()
    {
        ToggleButton("g", &Settings::bVisuals); ImGui::SameLine(); ImGui::Text(skCrypt("Visuals"));

        ImGui::NewLine();


        ToggleButton("a", &Settings::bESP); ImGui::SameLine(); ImGui::Text(skCrypt("Box"));

        ImGui::NewLine();

        ToggleButton("b", &Settings::bNames); ImGui::SameLine(); ImGui::Text(skCrypt("Names"));

        ImGui::NewLine();

        ToggleButton("c", &Settings::bFriendlies); ImGui::SameLine(); ImGui::Text(skCrypt("TeamMates"));


        ImGui::NewLine();

        ToggleButton("d", &Settings::bDistance); ImGui::SameLine(); ImGui::Text(skCrypt("Distance"));

        ImGui::NewLine();

        ToggleButton("e", &Settings::bBones); ImGui::SameLine(); ImGui::Text(skCrypt("Bones"));

        ImGui::NewLine();

        ToggleButton("f", &Settings::bCrossHair); ImGui::SameLine(); ImGui::Text(skCrypt("Crosshair"));

        ImGui::NewLine();

        ToggleButton("l", &Settings::bSnapLines); ImGui::SameLine(); ImGui::Text(skCrypt("SnapLines"));
       
    }

    void ColorSettings()
    {
        // Box Color
        if (ImGui::ColorButton(skCrypt("##BC"), ImVec4(Settings::BoxColor), ImGuiColorEditFlags_NoAlpha))
        {
            ImGui::OpenPopup(skCrypt("BoxColorPicker"));
        }
        ImGui::SameLine();
        ImGui::Text(skCrypt("Box Color"));
        ImGui::NewLine();

        if (ImGui::BeginPopupContextItem(skCrypt("BoxColorPicker")))
        {
            ImGui::ColorPicker4("", &Settings::BoxColor.x, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview);
            ImGui::EndPopup();
        }

        // Name Color
        if (ImGui::ColorButton(skCrypt("##N"), ImVec4(Settings::NameColor), ImGuiColorEditFlags_NoAlpha))
        {
            ImGui::OpenPopup(skCrypt("NameColorPicker"));
        }
        ImGui::SameLine();
        ImGui::Text(skCrypt("Name Color"));
        ImGui::NewLine();

        if (ImGui::BeginPopupContextItem(skCrypt("NameColorPicker")))
        {
            ImGui::ColorPicker4("", &Settings::NameColor.x, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview);
            ImGui::EndPopup();
        }

        // TeamMates Color
        if (ImGui::ColorButton(skCrypt("##F"), ImVec4(Settings::FriendlyColor), ImGuiColorEditFlags_NoAlpha))
        {
            ImGui::OpenPopup(skCrypt("FriendlyColorPicker"));
        }
        ImGui::SameLine();
        ImGui::Text(skCrypt("TeamMates Color"));
        ImGui::NewLine();

        if (ImGui::BeginPopupContextItem(skCrypt("FriendlyColorPicker")))
        {
            ImGui::ColorPicker4("", &Settings::FriendlyColor.x, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview);
            ImGui::EndPopup();
        }

        // Distance Color
        if (ImGui::ColorButton(skCrypt("##D"), ImVec4(Settings::DistanceColor), ImGuiColorEditFlags_NoAlpha))
        {
            ImGui::OpenPopup(skCrypt("DistanceColorPicker"));
        }
        ImGui::SameLine();
        ImGui::Text(skCrypt("Distance Color"));
        ImGui::NewLine();

        if (ImGui::BeginPopupContextItem(skCrypt("DistanceColorPicker")))
        {
            ImGui::ColorPicker4("", &Settings::DistanceColor.x, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoSidePreview);
            ImGui::EndPopup();
        }

        // Bones Color
        if (ImGui::ColorButton(skCrypt("##C"), ImVec4(Settings::BonesColor), ImGuiColorEditFlags_NoAlpha))
        {
            ImGui::OpenPopup(skCrypt("BonesColorPicker"));
        }
        ImGui::SameLine();
        ImGui::Text(skCrypt("Bones Color"));
        ImGui::NewLine();

        if (ImGui::BeginPopupContextItem(skCrypt("BonesColorPicker")))
        {
            ImGui::ColorPicker4("", &Settings::BonesColor.x, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview);
            ImGui::EndPopup();
        }

        // CrossHair Color
        if (ImGui::ColorButton(skCrypt("##A"), ImVec4(Settings::CrossHairColor), ImGuiColorEditFlags_NoAlpha))
        {
            ImGui::OpenPopup(skCrypt("CrossHairColorPicker"));
        }
        ImGui::SameLine();
        ImGui::Text(skCrypt("CrossHair Color"));
        ImGui::NewLine();

        if (ImGui::BeginPopupContextItem(skCrypt("CrossHairColorPicker")))
        {
            ImGui::ColorPicker4("", &Settings::CrossHairColor.x, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview);
            ImGui::EndPopup();
        }

        // SnapLine Close Color
        if (ImGui::ColorButton(skCrypt("##SL"), ImVec4(Settings::CloseLineColor), ImGuiColorEditFlags_NoAlpha))
        {
            ImGui::OpenPopup(skCrypt("CloseLineColorPicker"));
        }
        ImGui::SameLine();
        ImGui::Text(skCrypt("SnapLine Close Color"));
        ImGui::NewLine();

        if (ImGui::BeginPopupContextItem(skCrypt("CloseLineColorPicker")))
        {
            ImGui::ColorPicker4("", &Settings::CloseLineColor.x, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview);
            ImGui::EndPopup();
        }

        // SnapLine Far Color
        if (ImGui::ColorButton(skCrypt("##SL2"), ImVec4(Settings::FarLineColor), ImGuiColorEditFlags_NoAlpha))
        {
            ImGui::OpenPopup(skCrypt("FarLineColorPicker"));
        }
        ImGui::SameLine();
        ImGui::Text(skCrypt("SnapLine Far Color"));
        ImGui::NewLine();

        if (ImGui::BeginPopupContextItem(skCrypt("FarLineColorPicker")))
        {
            ImGui::ColorPicker4("", &Settings::FarLineColor.x, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview);
            ImGui::EndPopup();
        }



        ImGui::Text(skCrypt("Distance"));
        ImGui::Spacing();
        ImGui::SetNextItemWidth(210.0f);
        ImGui::SliderInt(skCrypt("##E"), &Settings::Distance, 100, 400, "%d", 0);
        ImGui::NewLine();
        ImGui::Text(skCrypt("CrossHair Size"));
        ImGui::Spacing();
        ImGui::SetNextItemWidth(210.0f);
        ImGui::SliderFloat(skCrypt("##S"), &Settings::CrossHairSize, 6.0f, 20.0f, skCrypt("%0.1f"), 0);

        ImGui::NewLine();
        ImGui::Text(skCrypt("Box Type"));
        const char* items[] = { "Box", "Box Filled", "Cornered Box" };
        ImGui::Spacing();
        ImGui::SetNextItemWidth(210.0f);
        ImGui::Combo(" ", &Settings::TypeOfESP, items, IM_ARRAYSIZE(items));



    }

    void Legit() {
        ImGui::Checkbox(skCrypt(" Active"), &Settings::bLegit);
        ImGui::Spacing();
        //ImGui::TextWrapped(//"Only draw visible players");
    }

    void Misc() {
        ImGui::Checkbox(skCrypt(" Infinite UAV"), &Settings::bUAV);
       // ImGui::Text("(Gives infinite UAV)");
        ImGui::NewLine();
    }
    
    void DebugMode() {
        ImGui::Checkbox(skCrypt(" Memory viewer"), &Settings::bDebug);
        ImGui::NewLine();
        ImGui::Text("Screenshots taken: %d xD", Settings::ScreenshotsTaken);
    }

    void ToggleButton(const char* str_id, bool* v)
    {
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        float height = (ImGui::GetFrameHeight()) - 3;
        float width = height * 1.55f;
        float radius = height * 0.50f;

        if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))
            *v = !*v;
        ImU32 col_bg;
        if (ImGui::IsItemHovered())
            col_bg = *v ? IM_COL32(240, 90, 90, 255) : IM_COL32(160, 160, 160, 255);
        else
            col_bg = *v ? IM_COL32(255, 0, 0, 255) : IM_COL32(100, 100, 100, 255);

        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
        draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(150, 150, 150, 255));
    }



}




