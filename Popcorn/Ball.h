#pragma once

#include "Config.h"
#include "Level.h"
// --------------------------------------------------------------------------------------------------------------------------------------
enum Eball_State
{
	EBS_Normal,
	EBS_Lost,
	EBS_On_Platform
};

// --------------------------------------------------------------------------------------------------------------------------------------
class ABall
{
public:

	ABall();

	void Init();
	void Draw(HDC hdc, RECT& paint_area);
	void Move(ALevel* level, int platform_x_pos, int platform_width);
	Eball_State Get_State();
	void Set_State(Eball_State new_state, int x_pos);

private:
	void Redraw_Ball();

	Eball_State Ball_State;
	HPEN Ball_Pen;
	HBRUSH Ball_Brush;

	double Ball_Direction;
	double Ball_Speed;
	double Ball_X_Pos, Ball_Y_Pos;
	static const double Start_Ball_Y_Pos; // initial Y position of the ball

	RECT Ball_Rect, Prev_Ball_Rect;
};
// --------------------------------------------------------------------------------------------------------------------------------------
