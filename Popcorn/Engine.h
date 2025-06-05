#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
// --------------------------------------------------------------------------------------------------------------------------------------
enum Eletter_Type
{
	ELT_None,

	ELT_0,
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
class AsEngine;
class ALevel;
class AsPlatform;
class AsBorder;
// --------------------------------------------------------------------------------------------------------------------------------------



//CLASSES
// --------------------------------------------------------------------------------------------------------------------------------------
class ABall
{
public:

	ABall();

	void Init();

	void Draw(HDC hdc, RECT& paint_area, AsEngine *engine);
	void Move(AsEngine *engine, ALevel *level, AsPlatform *platform);

	double Ball_Direction;

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;

	static const int Ball_Size = 4; // size of the ball

private:
	int Ball_X_Pos, Ball_Y_Pos;
	double Ball_Speed;

	RECT Ball_Rect, Prev_Ball_Rect;
};
// --------------------------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------------------------
class ALevel
{
public:
	void Init();
	void Chech_Level_Brick_Hit(int& next_y_pos, double &ball_direction);
	void Draw_Level(HDC hdc, RECT& paint_area);

	static const int Level_Width = 12; // width of the level
	static const int Level_Height = 14; // height of the level
	static const int Level_X_Offset = 8; // offset of the level
	static const int Level_Y_Offset = 6; // offset of the level
	static const int Cell_Width = 16; // width of the cell
	static const int Cell_Height = 8; // height of the cell

private:
	void Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, Ebrick_Type brick_type, Eletter_Type letter_type, int rotation_step);

	HPEN  Letter_Pen, Brick_Red_Pen, Brick_Blue_Pen;
	HBRUSH Brick_Red_Brush, Brick_Blue_Brush;
	RECT Level_Rect;

	static const int Brick_Width = 15; // width of the brick
	static const int Brick_Height = 7; // height of the brick
};
// --------------------------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
	AsPlatform();

	void Init();
	void Redraw_Platform(AsEngine* engine);
	void Draw(HDC hdc, AsEngine *engine, RECT &paint_area);

	int X_Pos;
	int Width;
	int X_Step;

	static const int Y_Pos = 185; // Y position of the platform

private:
	int Inner_Width;

	HPEN Highlight_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

	RECT Platform_Rect, Prev_Platform_Rect;

	static const int Height = 7; // width of the platform
	static const int Circle_Size = 7;
};
// --------------------------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------------------------
class AsBorder
{
public:
	void Init();

	void Draw_Element(HDC hdc, int x, int y, bool top_border, AsEngine *engine);
	void Draw(HDC hdc, RECT& paint_area, AsEngine *engine);

	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;

	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;

private:
};
// --------------------------------------------------------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND Hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(Ekey_Type key_type);
	int On_Timer();

	static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

	HWND Hwnd;

	HPEN BG_Pen;
	HBRUSH BG_Brush;

	static const int Global_Scale = 3; // scale of the game
	static const int Max_X_Pos = ALevel::Level_X_Offset + ALevel::Cell_Width * ALevel::Level_Width; // max X position of the ball
	static const int Max_Y_Pos = 199 - ABall::Ball_Size; // max Y position of the ball

private:

	ABall Ball; // ball object
	ALevel Level; // level object
	AsPlatform Platform; // platform object
	AsBorder Border; // border object
};

// --------------------------------------------------------------------------------------------------------------------------------------