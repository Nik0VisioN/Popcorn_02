#pragma once

#include <Windows.h>
enum Ekey_Type
{
	EKT_None,
	EKT_Left,
	EKT_Right,
	EKT_Space,
};
const int Timer_ID = WM_USER + 1; // ID of the timer


void Init_Engine(HWND Hwnd);
void Draw_Frame(HDC hdc, RECT& paint_area);
int On_Key_Down(Ekey_Type key_type);
int On_Timer();
