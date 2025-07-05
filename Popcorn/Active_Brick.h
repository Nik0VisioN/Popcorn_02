#pragma once

#include "Config.h"

// --------------------------------------------------------------------------------------------------------------------------------------
enum EBrick_Type
{
	EBT_None,
	EBT_Red,
	EBT_Blue,
};
// --------------------------------------------------------------------------------------------------------------------------------------
class AActive_Brick
{
public:
	AActive_Brick(EBrick_Type brick_type);

	void Act();
	void Draw(HDC hdc, RECT& paint_area);
	
	static void Setup_Colors();

private:

	RECT Brick_Rect;
	EBrick_Type Brick_Type;
	int Fade_Step;

	static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step);
	static void Get_Fading_Color(const AColor& color, int step, HPEN& pen, HBRUSH& brush);

	static const int Max_Fade_Step = 80;

	static HPEN Fading_Red_Brick_Pens[Max_Fade_Step];
	static HBRUSH Fading_Red_Brick_Brushes[Max_Fade_Step];
	static HPEN Fading_Blue_Brick_Pens[Max_Fade_Step];
	static HBRUSH Fading_Blue_Brick_Brushes[Max_Fade_Step];
};
// --------------------------------------------------------------------------------------------------------------------------------------
