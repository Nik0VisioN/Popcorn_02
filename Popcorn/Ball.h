#pragma once
#include "Config.h"
// --------------------------------------------------------------------------------------------------------------------------------------
enum EBall_State
{
	EBS_Normal,
	EBS_Lost,
	EBS_On_Platform,
	EBS_On_Parachute,
	EBS_Off_Parachute,
	EBS_Teleporting
};

// --------------------------------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) = 0;

	bool Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x);
};
// --------------------------------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void Draw(HDC hdc, RECT& paint_area);
	void Draw_Teleporting(HDC hdc, int step);
	void Move();
	void Set_For_Test();
	bool Is_Test_Finished();
	EBall_State Get_State();
	void Set_State(EBall_State new_state, double x_pos, double y_pos = Start_Ball_Y_Pos);
	void Get_Center(double& x_pos, double& y_pos);
	double Get_Direction();
	void Set_Direction(double new_direction);
	void Reflect(bool from_horizontal);
	bool Is_Moving_Up();
	bool Is_Moving_Left();
	void Set_On_Parachute(int brick_x, int brick_y);

	double Ball_Speed;

	static void Add_Hit_Checker(AHit_Checker *hit_checker);
	static const double Radius;

private:
	void Redraw_Ball();
	void Redraw_Parachute();
	void Draw_Parachute(HDC hdc, RECT& paint_area);
	void Clear_Parachute(HDC hdc);

	EBall_State Ball_State, Prev_Ball_State;
	double Rest_Distance;
	double Center_X_Pos, Center_Y_Pos;
	double Ball_Direction;
	double Rest_Test_Distance;
	bool Testing_Is_Active;
	int Test_Iteration;

	RECT Ball_Rect, Prev_Ball_Rect;
	RECT Parachute_Rect, Prev_Parachute_Rect;

	static const double Start_Ball_Y_Pos; // initial Y position of the ball
	static const int Parachute_Size = 15;
	static int Hit_Checkers_Count; // number of hit checkers
	static AHit_Checker *Hit_Checkers[3];
};
// --------------------------------------------------------------------------------------------------------------------------------------
