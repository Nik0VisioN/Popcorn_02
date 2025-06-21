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

   Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);

   Platform.Set_State(EPS_Normal);

   Platform.Redraw_Platform();

   SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{ // drawer screen game

      Level.Draw(hdc, paint_area);

   /* int i;

    for (i = 0; i < 16; i++)
    {
       Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_0, i);
       Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Red, ELT_0, i);
    }*/
   
   Ball.Draw(hdc, paint_area);
   Border.Draw(hdc, paint_area);
   Platform.Draw(hdc, paint_area);
}
// --------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(Ekey_Type key_type)
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
	   case EGS_Play_Level:
      Ball.Move();

	   if (Ball.Get_State() == EBS_Lost)
	   {
	   	Game_State = EGS_Lost_Ball;
	   	Platform.Set_State(EPS_Meltdown);
	   }
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

   //Level.Active_Brick.Act();

   //if(AsConfig::Current_Timer_Tick % 10 == 0)

   return 0; 
}
// --------------------------------------------------------------------------------------------------------------------------------------
