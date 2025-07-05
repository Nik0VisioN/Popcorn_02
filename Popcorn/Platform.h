#pragma once
#include "Ball.h"

// --------------------------------------------------------------------------------------------------------------------------------------
enum EPlatform_State
{
	EPS_Missing,
	EPS_Ready,
	EPS_Normal,
	EPS_Meltdown,
	EPS_Roll_In,
	EPS_Expand_Roll_In
};
// --------------------------------------------------------------------------------------------------------------------------------------
class AsPlatform: public AHit_Checker
{
public:
	AsPlatform();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

	void Init();
	void Act();
	EPlatform_State Get_State();
	void Set_State(EPlatform_State new_state);
	void Redraw_Platform();
	void Draw(HDC hdc, RECT& paint_area);

	int X_Pos;
	int Width;
	int X_Step;

private:
	void Clear_BG(HDC hdc);
	void Draw_Circle_Highlight(HDC hdc, int x, int y);
	void Draw_Normal_State(HDC hdc, RECT& paint_area);
	void Draw_Meltdown_State(HDC hdc, RECT& paint_area);
	void Draw_Roll_In_State(HDC hdc, RECT& paint_area);
	void Draw_Expanding_Roll_In_State(HDC hdc, RECT& paint_area);

	EPlatform_State Platform_State;
	int Inner_Width;
	int Rolling_Step; // step of the platform rolling out

	static const int Normal_Width = 28; // width of the platform

	int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

	RECT Platform_Rect, Prev_Platform_Rect;

	HPEN Highlight_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

	static const int Height = 7; // height of the platform
	static const int Circle_Size = 7;
	static const int Normal_Platform_Inner_Width = Normal_Width - Circle_Size; // width of the platform inner part in normal state
	static const int Meltdown_Speed = 3; // speed of the platform meltdown
	static const int Max_Rolling_Step = 16; // max step of the platform rolling out
	static const int Roll_In_Platform_End_X_Pos = 99; // x position of the platform end in roll-in state
	static const int Rolling_Platform_Speed = 3; // speed of the platform rolling out
};
// --------------------------------------------------------------------------------------------------------------------------------------


