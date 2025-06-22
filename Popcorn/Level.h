#pragma once

#include "Active_Brick.h"
#include "Ball.h"

// --------------------------------------------------------------------------------------------------------------------------------------
enum Eletter_Type
{
	ELT_None,

	ELT_0,
};
// --------------------------------------------------------------------------------------------------------------------------------------
class ALevel: public AHit_Checker
{
public:

	ALevel();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

	void Init();
	void Draw(HDC hdc, RECT& paint_area);

	AActive_Brick Active_Brick;

private:
	bool Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius);

	void Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, Ebrick_Type brick_type, Eletter_Type letter_type, int rotation_step);

	HPEN  Letter_Pen, Brick_Red_Pen, Brick_Blue_Pen;
	HBRUSH Brick_Red_Brush, Brick_Blue_Brush;
	RECT Level_Rect;

	static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];

};
// --------------------------------------------------------------------------------------------------------------------------------------
