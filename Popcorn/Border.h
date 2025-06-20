#pragma once
#include "Ball.h"
// --------------------------------------------------------------------------------------------------------------------------------------
class AsBorder: public AHit_Checker
{
public:

	AsBorder();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

	void Init();
	void Draw(HDC hdc, RECT &paint_area);

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border);

	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;
};
// --------------------------------------------------------------------------------------------------------------------------------------






