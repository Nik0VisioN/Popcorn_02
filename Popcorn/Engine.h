#pragma once

#include <Windows.h>

#include "Border.h"
#include "Level.h" 
#include "Platform.h"

// --------------------------------------------------------------------------------------------------------------------------------------
enum EKey_Type
{
	EKT_None,
	EKT_Left,
	EKT_Right,
	EKT_Space,
};
// --------------------------------------------------------------------------------------------------------------------------------------
enum EGame_State
{
	EGS_Test_Ball,

	EGS_Play_Level, 
	EGS_Lost_Ball, 
	EGS_Restart_Level, 
	EGS_Won 
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
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

private:
	EGame_State Game_State; // current game state
	ABall Ball; // ball object
	ALevel Level; // level object
	AsPlatform Platform; // platform object
	AsBorder Border; // border object
};

// --------------------------------------------------------------------------------------------------------------------------------------