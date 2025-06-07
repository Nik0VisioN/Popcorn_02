#pragma once

#include <Windows.h>

#include "Border.h"
#include "Ball.h"
#include "Platform.h"

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

// --------------------------------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND Hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(Ekey_Type key_type);
	int On_Timer();

private:
	HWND Hwnd;

	ABall Ball; // ball object
	ALevel Level; // level object
	AsPlatform Platform; // platform object
	AsBorder Border; // border object
};

// --------------------------------------------------------------------------------------------------------------------------------------