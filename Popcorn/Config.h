#pragma once

#include <Windows.h>


#define _USE_MATH_DEFINES
#include <math.h>
// --------------------------------------------------------------------------------------------------------------------------------------
class AColor
{
public:
	AColor(unsigned char r, unsigned char g, unsigned char b);

	unsigned char R, G, B;
};
// --------------------------------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:

	static void Setup_Colors();
	static void Create_Pen_Brush(const AColor& color, HPEN& pen, HBRUSH& brush);
	static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
	static int Rand(int range);

	static bool Level_Has_Floor;
	static int Current_Timer_Tick;
	static const AColor BG_Color, Red_Brick_Color, Blue_Brick_Color;
	static HPEN BG_Pen;
	static HBRUSH BG_Brush;
	static HWND Hwnd;

	static const double Moving_Step_Size;

	static const int Global_Scale = 3; // scale of the game
	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;
	static const int Level_Width = 12; // width of the level
	static const int Level_Height = 14; // height of the level
	static const int Level_X_Offset = 8; // offset of the level
	static const int Level_Y_Offset = 6; // offset of the level
	static const int Cell_Width = 16; // width of the cell
	static const int Cell_Height = 8; // height of the cell
	static const int Brick_Width = 15; // width of the brick
	static const int Brick_Height = 7; // height of the brick
	static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width; // max X position of the ball
	static const int Max_Y_Pos = 199; // max Y position of the ball
	static const int Platform_Y_Pos = 185; // Y position of the platform
	static const int FPS = 20;
};
// --------------------------------------------------------------------------------------------------------------------------------------
