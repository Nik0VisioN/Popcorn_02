#pragma once
#include "Falling_Letter.h"

// --------------------------------------------------------------------------------------------------------------------------------------
struct SPoint
{
	int X, Y;
};
// --------------------------------------------------------------------------------------------------------------------------------------
class AsLevel: public AHit_Checker
{
public:

	~AsLevel();
	AsLevel();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

	void Init();
	void Set_Current_Level(char level[AsConfig::Level_Height][AsConfig::Level_Width]);
	void Act();
	void Draw(HDC hdc, RECT& paint_area);
	bool Get_Next_Falling_Letter(int &index, AFalling_Letter **falling_letter);

	AActive_Brick_Purple_And_Blue Active_Brick;

	static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];
	static char Test_Level[AsConfig::Level_Height][AsConfig::Level_Width];

private:
	bool On_Hit(int brick_x, int brick_y, ABall* ball, bool vertical_hit);
	void Redraw_Brick(int brick_x, int brick_y);
	bool Create_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type, ABall* ball, bool vertical_hit);
	void Add_Active_Brick_Teleport(int brick_x, int brick_y, ABall* ball, bool vertical_hit);
	void Add_New_Active_Brick(AActive_Brick* active_brick);
	AActive_Brick_Teleport* Select_Destination_Teleport(int source_x, int source_y);
	bool Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type);
	bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double &reflection_pos);
	bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double &reflection_pos);
	void Draw_Brick(HDC hdc, RECT &brick_rect, EBrick_Type brick_type);
	void Draw_Parachute_In_Level(HDC hdc, RECT& brick_rect);
	void Draw_Parachute_Part(HDC hdc, RECT& brick_rect, int offset, int width);
	void Clear_Objects(HDC hdc, RECT& paint_area, AGraphics_Object** objects_array, int objects_max_count);
	void Draw_Objects(HDC hdc, RECT& paint_area, AGraphics_Object** objects_array, int objects_max_count);
	void Act_Objects(AGraphics_Object** objects_array, int &objects_count, const int objects_max_count);

	RECT Level_Rect;

	double Current_Brick_Left_X, Current_Brick_Right_X;
	double Current_Brick_Top_Y, Current_Brick_Low_Y;

	AColor Parachute_Color;

	char Current_Level[AsConfig::Level_Height][AsConfig::Level_Width];
	int Active_Bricks_Count;
	AActive_Brick *Active_Bricks[AsConfig::Max_Active_Bricks_Count];
	int Falling_Letters_Count;
	AFalling_Letter *Falling_Letters[AsConfig::Max_Falling_Letters_Count];
	int Teleport_Bricks_Count;
	SPoint *Teleport_Bricks_Pos;
	AAdvertisement *Advertisement;
};
// --------------------------------------------------------------------------------------------------------------------------------------
