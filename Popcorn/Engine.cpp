#include "Engine.h"



// AsEngine
// --------------------------------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
:Game_State(EGS_Play_Level)
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{// setting the game on start

   AsConfig::Hwnd = hwnd;

   AActive_Brick::Setup_Colors();

   Level.Init();
   Platform.Init();
   Ball.Init();
   Border.Init();

   ABall::Add_Hit_Checker(&Border);
   ABall::Add_Hit_Checker(&Level);
   ABall::Add_Hit_Checker(&Platform);

	Level.Set_Current_Level(ALevel::Level_01);

   Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);

   Platform.Set_State(EPS_Normal);

   Platform.Redraw_Platform();

   SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{ // drawer screen game

   //int i;
   //COLORREF pixel;

   SetGraphicsMode(hdc, GM_ADVANCED);

   Level.Draw(hdc, paint_area);   
   Border.Draw(hdc, paint_area);
   Platform.Draw(hdc, paint_area);
   Ball.Draw(hdc, paint_area);

 //  for (i = 0; i < 84 * 21 * 100; i++ )
 //  {
 //     pixel = GetPixel(hdc, 100, 100);
 //     SetPixel(hdc, 100, 100, pixel);
	//}

 /*  for (i = 0; i < 84 ; i++)
   {
		MoveToEx(hdc, 100, 555, 0);

		SelectObject(hdc, AsConfig::BG_Pen);
		LineTo(hdc, 100, 557);

      SelectObject(hdc, AsConfig::Brick_Blue_Pen);
      LineTo(hdc, 100, 572);

      SelectObject(hdc, AsConfig::BG_Pen);
      LineTo(hdc, 100, 575);

   }*/

	//int yy = 0;
}
// --------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform.X_Pos -= Platform.X_Step;

      if (Platform.X_Pos <= AsConfig::Border_X_Offset)
         Platform.X_Pos = AsConfig::Border_X_Offset;

      Platform.Redraw_Platform();
      break;


   case EKT_Right:
      Platform.X_Pos += Platform.X_Step;

      if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width - 1)
         Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;

      Platform.Redraw_Platform();
      break;


   case EKT_Space:
      if(Platform.Get_State() == EPS_Ready)
      {
         Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);
         Platform.Set_State(EPS_Normal);
      }
      break;
   }
   return 0;
}
// --------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
   ++AsConfig::Current_Timer_Tick;

	switch (Game_State)
   {
   case EGS_Test_Ball:
               // Test the ball movement
      Ball.Set_For_Test();
		Game_State = EGS_Play_Level;
			break;

	   case EGS_Play_Level:
      Ball.Move();

	   if (Ball.Get_State() == EBS_Lost)
	   {
	   	Game_State = EGS_Lost_Ball;
	   	Platform.Set_State(EPS_Meltdown);
	   }

      if (Ball.Is_Test_Finished())
         Game_State = EGS_Test_Ball;
      break;


	   case EGS_Lost_Ball:
         if(Platform.Get_State() == EPS_Missing)
         {
            Game_State = EGS_Restart_Level;
				Platform.Set_State(EPS_Roll_In);
         }
	   	break;


      case EGS_Restart_Level:
         if (Platform.Get_State() == EPS_Ready)
         {
            Game_State = EGS_Play_Level;
				Ball.Set_State(EBS_On_Platform, Platform.X_Pos + Platform.Width / 2);
         }
	   	break;
   }
   Platform.Act();
   Level.Act();

   //if(AsConfig::Current_Timer_Tick % 10 == 0)

   return 0; 
}
// --------------------------------------------------------------------------------------------------------------------------------------
