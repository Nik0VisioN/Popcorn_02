#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>
// --------------------------------------------------------------------------------------------------------------------------------------
enum Eletter_Type
{
	ELT_None,

	ELT_0
};

// --------------------------------------------------------------------------------------------------------------------------------------

enum Ebrick_Type
{
	EBT_None,
	EBT_Red,
	EBT_Blue,
};

// --------------------------------------------------------------------------------------------------------------------------------------

enum Ekey_Type
{
	EKT_None,
	EKT_Left,
	EKT_Right,
	EKT_Space,
};

// --------------------------------------------------------------------------------------------------------------------------------------

const int Timer_ID = WM_USER + 1; // ID of the timer

// --------------------------------------------------------------------------------------------------------------------------------------

class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND Hwnd);
	void Draw_Frame(HDC hdc, RECT& paint_area);
	int On_Key_Down(Ekey_Type key_type);
	int On_Timer();

	static const int Level_Width = 12; // width of the level
	static const int Level_Height = 14; // height of the level

private:
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
void Redraw_Platform();
void Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type);
void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
void Draw_Brick_Letter(HDC hdc, int x, int y, Ebrick_Type brick_type, Eletter_Type letter_type, int rotation_step);
void Draw_Level(HDC hdc);
void Draw_Platform(HDC hdc, int x, int y);
void Draw_Ball(HDC hdc, RECT& paint_area);
void Draw_Border(HDC hdc, int x, int y, bool top_border);
void Draw_Bounds(HDC hdc, RECT& paint_area);
void Chech_Level_Brick_Hit(int& next_y_pos);
void Move_Ball();


	HWND Hwnd;
	HPEN Highlight_Pen, BG_Pen, Letter_Pen, Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Ball_Pen, Border_Blue_Pen, Border_White_Pen;
	HBRUSH Brick_Red_Brush, BG_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush, Ball_Brush, Border_Blue_Brush, Border_White_Brush;

	int Inner_Width = 21;
	int Platform_X_Pos = Border_X_Offset;
	int Platform_X_Step = Global_Scale * 2;
	int Platform_Width = 28;


	int Ball_X_Pos = 20, Ball_Y_Pos = 170;
	double Ball_Speed = 3.0, Ball_Direction = M_PI - M_PI_4;



	RECT Platform_Rect, Prev_Platform_Rect;
	RECT Level_Rect;
	RECT Ball_Rect, Prev_Ball_Rect;

	static const int Global_Scale = 3; // scale of the game
	static const int Brick_Width = 15; // width of the brick
	static const int Brick_Height = 7; // height of the brick
	static const int Cell_Width = 16; // width of the cell
	static const int Cell_Height = 8; // height of the cell
	static const int Level_X_Offset = 8; // offset of the level
	static const int Level_Y_Offset = 6; // offset of the level
	static const int Circle_Size = 7;
	static const int Platform_Y_Pos = 185; // Y position of the platform
	static const int Platform_Height = 7; // width of the platform
	static const int Ball_Size = 4; // size of the ball
	static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width; // max X position of the ball
	static const int Max_Y_Pos = 199 - Ball_Size; // max Y position of the ball
	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;

};

// --------------------------------------------------------------------------------------------------------------------------------------