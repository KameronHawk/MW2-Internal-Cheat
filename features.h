#pragma once
#include "game.h"


namespace Features {
	void Box(vec2_t TopLeft, vec2_t BottomRight, int TypeOfBox, ImVec4 Color, float Distance);

	void DrawCorneredBox(vec2_t& TopLeft, vec2_t& BottomRight, ImVec4 Color, float distance);

	void DrawFilledBox(vec2_t TopLeft, vec2_t BottomRight, ImVec4 Color);

	void Name(std::string Name, vec2_t ScreenPos);

	void Distance(vec2_t HeadPos2D, char* buff);

	void Team(vec2_t TopLeft, vec2_t BottomRight);

	void Bones(vec3_t From, vec3_t To);

	void DrawLines(vec2_t LineTo, ImVec4 LineColor);

	



}
















