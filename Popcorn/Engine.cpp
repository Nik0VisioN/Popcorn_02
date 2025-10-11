#include "Engine.h"



// AsEngine
// --------------------------------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
:Game_State(EGS_Lost_Ball)
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{// setting the game on start
   
	SYSTEMTIME sys_time;
	FILETIME file_time;

   GetSystemTime(&sys_time);
	SystemTimeToFileTime(&sys_time, &file_time);

   srand(file_time.dwLowDateTime);
   AsConfig::Hwnd = hwnd;

   AActive_Brick_Purple_And_Blue::Setup_Colors();

   Level.Init();
   AFalling_Letter::Init();

   ABall::Add_Hit_Checker(&Border);
   ABall::Add_Hit_Checker(&Level);
   ABall::Add_Hit_Checker(&Platform);

	Level.Set_Current_Level(AsLevel::Level_01);
   //Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);
   //Platform.Set_State(EPS_Normal);
   Platform.Redraw_Platform();

   SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{ // drawer screen game

   SetGraphicsMode(hdc, GM_ADVANCED);

   Level.Draw(hdc, paint_area);   
   Border.Draw(hdc, paint_area);
   Platform.Draw(hdc, paint_area);
   Ball.Draw(hdc, paint_area);
}
// --------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
		Platform.Move(true);
      break;


   case EKT_Right:
      Platform.Move(false);
      break;


   case EKT_Space:
      if (Platform.Get_State() == EPS_Ready)
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
         if (Platform.Get_State() == EPS_Missing)
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
   Act();

   return 0; 
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
   int index = 0;
	AFalling_Letter *falling_letter;

   Platform.Act();
   Level.Act();

   while (Level.Get_Next_Falling_Letter(index, &falling_letter) )
   {
      if (Platform.Hit_By(falling_letter) )
         On_Falling_Letter(falling_letter);
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter *falling_letter)
{
	falling_letter->Finalize();
}
// --------------------------------------------------------------------------------------------------------------------------------------
