#include "features.h"

void Features::Box(vec2_t TopLeft, vec2_t BottomRight, int TypeOfBox, ImVec4 Color, float Distance)
{
	ImU32 FillColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
	float alpha = 0.5f;
	ImVec4 realColor = ImGui::ColorConvertU32ToFloat4(FillColor);
	realColor.w = alpha;
	switch (TypeOfBox) {
	case 0:
		
		Draw::DrawRectFilled(TopLeft, BottomRight, realColor, 0.0f);
		Draw::DrawRect(TopLeft, BottomRight, Color, 0.0f, 1.5f);
		break;


	case 1:
		DrawFilledBox(TopLeft, BottomRight, Color);
		break;

	case 2:
		DrawCorneredBox(TopLeft, BottomRight, Color, Distance);
		break;

	default:
		break;
	}



	

}

void Features::Name(std::string Name, vec2_t ScreenPos) {

	Draw::vDrawText(Name, { ScreenPos.x, ScreenPos.y + 1 }, 12.0f, Settings::NameColor, true);
}

void Features::Distance(vec2_t HeadPos2D, char* buff)
{
	Draw::vDrawText(buff, { HeadPos2D.x, HeadPos2D.y - 15 }, 12.0f, Settings::DistanceColor, true);
}

void Features::Team(vec2_t TopLeft, vec2_t BottomRight)
{
	Draw::DrawRect(TopLeft, BottomRight, Settings::FriendlyColor, 0.0f, 1.5f);
}

void Features::DrawCorneredBox(vec2_t& TopLeft, vec2_t& BottomRight, ImVec4 Color, float distance)
{
	Draw::DrawCornerBox(TopLeft, BottomRight, Color, distance);
}

void Features::DrawFilledBox(vec2_t TopLeft, vec2_t BottomRight, ImVec4 Color)
{
	Draw::DrawRectFilled(TopLeft, BottomRight, Color, 1.5f);
}
