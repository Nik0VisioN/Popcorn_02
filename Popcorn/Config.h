#pragma once

#include <Windows.h>


#define _USE_MATH_DEFINES
#include <math.h>


// --------------------------------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

	static const int Global_Scale = 3; // scale of the game
	static const int Border_X_Offset = 6;
	static const int Level_Width = 12; // width of the level
	static const int Level_Height = 14; // height of the level
	static const int Level_X_Offset = 8; // offset of the level
	static const int Level_Y_Offset = 6; // offset of the level
	static const int Y_Pos = 185; // Y position of the platform
	static const int Cell_Width = 16; // width of the cell
	static const int Cell_Height = 8; // height of the cell
	static const int Ball_Size = 4; // size of the ball
	static const int Border_Y_Offset = 4;
	static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width; // max X position of the ball
	static const int Max_Y_Pos = 199 - Ball_Size; // max Y position of the ball
};
// --------------------------------------------------------------------------------------------------------------------------------------
