#pragma once
#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>


// --------------------------------------------------------------------------------------------------------------------------------------
class AColor
{
public:
	AColor();
	AColor(unsigned char r, unsigned char g, unsigned char b);
	AColor(const AColor &color, int pen_size);
	AColor(unsigned char r, unsigned char g, unsigned char b, int pen_size);
	AColor(const AColor &pen_color, const AColor &brush_color, int pen_size);

	int Get_RGB() const;
	void Select(HDC hdc) const;
	void Select_Pen(HDC hdc) const;
	HBRUSH Get_Brush() const;

	unsigned char R, G, B;

private:
	HPEN Pen;
	HBRUSH Brush;
};
// --------------------------------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static int Rand(int range);
	static void Round_Rect(HDC hdc, RECT &rect, int corner_radius = 2);
	static void Throw();

	static bool Level_Has_Floor;
	static int Current_Timer_Tick;
	static const AColor BG_Color, Purple_Color, Blue_Color, White_Color, Letter_Color;
	static const AColor Unbreakable_Blue_Highlight, Unbreakable_Purple_Highlight, Teleport_Portal_Color;
	static const AColor Advertisement_Purple, Advertisement_Blue;
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
	static const int Max_Active_Bricks_Count = 10; // max count of active bricks
	static const int Hits_Per_Letter = 10; // chanse of a letter to fall from a brick
	static const int Max_Falling_Letters_Count = 10; // max count of falling letters
};
// --------------------------------------------------------------------------------------------------------------------------------------
