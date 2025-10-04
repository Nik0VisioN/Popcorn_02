#pragma once

#include "Active_Brick.h"

// --------------------------------------------------------------------------------------------------------------------------------------
enum ELetter_Type
{
	ELT_O, // cancel
	ELT_I, // inversion
	ELT_C, // speed
	ELT_M, // monster
	ELT_G, // +helth
	ELT_K, // glue
	ELT_W, // wide
	
	ELT_P, // floor
	ELT_L, // lazer
	ELT_T, // three balls
		
	ELT_Plus, // move to the next level
	
	ELT_Max
};
// --------------------------------------------------------------------------------------------------------------------------------------
enum EFalling_Letter_State
{
	EFLS_Normal,
	EFLS_Finalizing, // Starting the remove process (object)
	EFLS_Finished // When the object is finished and can be deleted
};
// --------------------------------------------------------------------------------------------------------------------------------------
class AFalling_Letter : public AGraphics_Object
{
public:
	AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Get_Letter_Cell(RECT &rect);
	void Finalize();
	void Test_Draw_All_Steps(HDC hdc);

	static void Init();
	static ELetter_Type Get_Random_Letter_Type();

	const ELetter_Type Letter_Type;
	const EBrick_Type Brick_Type;
private:
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc);
	void Draw_Line(HDC hdc, int x_1, int y_1, int x_2, int y_2);
	void Draw_Line_To(HDC hdc, int x, int y);

	EFalling_Letter_State Falling_Letter_State;
	int X, Y;
	int Rotation_Step;
	int Next_Rotation_Tick;
	RECT Letter_Cell, Prev_Letter_Cell;

	static const int Ticks_Per_Step = 4; // how many ticks to wait before the next rotation step
	static const int Max_Rotation_Steps = 16;
	static const int Brick_Half_Height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;

	static int All_Letters_Popularity; // the sum of the popularity of all letters
	static int Letters_Popularity[ELT_Max]; // the popularity of each letter (the higher the more popular)
};
// --------------------------------------------------------------------------------------------------------------------------------------