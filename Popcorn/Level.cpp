#include "Level.h"


// --------------------------------------------------------------------------------------------------------------------------------------
char AsLevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 3, 7, 6, 5, 4,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char AsLevel::Test_Level[AsConfig::Level_Height][AsConfig::Level_Width] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// AsLevel
// --------------------------------------------------------------------------------------------------------------------------------------
AsLevel::AsLevel()
: Active_Brick(EBT_Blue, 0, 0), Level_Rect{}, Current_Brick_Left_X(0), Current_Brick_Right_X(0), Current_Brick_Top_Y(0),
Current_Brick_Low_Y(0), Active_Bricks_Count(0), Falling_Letters_Count(0), Active_Bricks{}, Falling_Letters{}
{
   // Initialize Current_Level to zero  
   memset(Current_Level, 0, sizeof(Current_Level));
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{// correct the position of the ball when it is out of bricks

   int i, j;
   int min_level_x, max_level_x;
   int min_level_y, max_level_y;
   double direction;
   double min_ball_x, max_ball_x;
   double min_ball_y, max_ball_y;
	double horizontal_reflection_pos, vertical_reflection_pos;

	bool got_horizontal_hit, got_vertical_hit;


   if(next_y_pos +ball->Radius > AsConfig::Level_Y_Offset + (AsConfig::Level_Height - 1) * AsConfig::Cell_Height + AsConfig::Brick_Height)
		return false; // the ball is below the level, no hit

   direction = ball->Get_Direction();

	min_ball_x = next_x_pos - ball->Radius;
	max_ball_x = next_x_pos + ball->Radius;
	min_ball_y = next_y_pos - ball->Radius;
	max_ball_y = next_y_pos + ball->Radius;

	min_level_x = (int)((min_ball_x - AsConfig::Level_X_Offset) / (double)AsConfig::Cell_Width);
	max_level_x = (int)((max_ball_x - AsConfig::Level_X_Offset) / (double)AsConfig::Cell_Width);
	min_level_y = (int)((min_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height);
	max_level_y = (int)((max_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height);

   for (i = max_level_y; i >= min_level_y; i--)
   {
      Current_Brick_Top_Y = AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height;
      Current_Brick_Low_Y = Current_Brick_Top_Y + AsConfig::Brick_Height;

      for (j = min_level_x; j <= max_level_x; j++)
      {
         if (Current_Level[i][j] == 0)
            continue;

         Current_Brick_Left_X = AsConfig::Level_X_Offset + j * AsConfig::Cell_Width;
         Current_Brick_Right_X = Current_Brick_Left_X + AsConfig::Brick_Width;

         got_horizontal_hit = Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball, horizontal_reflection_pos);

         got_vertical_hit = Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball, vertical_reflection_pos);

         if (got_horizontal_hit && got_vertical_hit)
         {
            if(vertical_reflection_pos < horizontal_reflection_pos)
            ball->Reflect(true); // reflect vertically
				else
				ball->Reflect(false); // reflect horizontally

            On_Hit(j, i);
				return true;
         }
            else
			if (got_horizontal_hit)
         {
			ball->Reflect(false);
         On_Hit(j, i);
			return true;
         }
            else
         if (got_vertical_hit)
         {
         ball->Reflect(true);
         On_Hit(j, i);
         return true;
			}
      }
   }
   return false;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Init()
{
   Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
   Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
   Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::Level_Width * AsConfig::Global_Scale;
   Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Height * AsConfig::Level_Height * AsConfig::Global_Scale;


	// initialize the level
	memset(Current_Level, 0, sizeof(Current_Level) );
   memset(Active_Bricks, 0, sizeof(Active_Bricks) );
   memset(Falling_Letters, 0, sizeof(Falling_Letters));
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Set_Current_Level(char level[AsConfig::Level_Height][AsConfig::Level_Width])
{
   memcpy(Current_Level, level, sizeof(Current_Level));
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Act()
{
	Act_Objects((AGraphics_Object**)&Active_Bricks, AsConfig::Max_Active_Bricks_Count);
   Act_Objects((AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Draw(HDC hdc, RECT &paint_area)
{ // output all bricks of the level
   int i, j;
   RECT intersection_rect, brick_rect;
  
	//AFalling_Letter falling_letter(EBT_Blue, ELT_Plus, 8 * AsConfig::Global_Scale, 150 * AsConfig::Global_Scale);
 //  falling_letter.Test_Draw_All_Steps(hdc);

   if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
   {
   for (i = 0; i < AsConfig::Level_Height; i++)
      for (j = 0; j < AsConfig::Level_Width; j++)
         {
			brick_rect.left = (AsConfig::Level_X_Offset + j * AsConfig::Cell_Width) * AsConfig::Global_Scale;
			brick_rect.top = (AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height) * AsConfig::Global_Scale;
			brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
			brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;


            if (IntersectRect(&intersection_rect, &paint_area, &brick_rect))
            Draw_Brick(hdc, brick_rect, (EBrick_Type)Current_Level[i][j]);
         }
      Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Active_Bricks, AsConfig::Max_Active_Bricks_Count);
   }
	 Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AsLevel::Get_Next_Falling_Letter(int& index, AFalling_Letter** falling_letter)
{
	AFalling_Letter* current_letter;
   
   if (Falling_Letters_Count == 0)
		return false; // no falling letters

   if (index < 0 || index >= AsConfig::Max_Falling_Letters_Count)
		return false; // incorrect index
   
   while ( index < AsConfig::Max_Falling_Letters_Count)
   {
		current_letter = Falling_Letters[index++];

      if (current_letter != 0)
      {
			*falling_letter = current_letter;
			return true; // found the next falling letter
      }
   }
	return false; // no more falling letters
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::On_Hit(int brick_x, int brick_y)
{
   EBrick_Type brick_type;

   brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

   if(Add_Falling_Letter(brick_x, brick_y, brick_type) )
		Current_Level[brick_y][brick_x] = EBT_None; // remove the brick from the level
	else
      Add_Active_Brick(brick_x, brick_y, brick_type);

   Redraw_Brick(brick_x, brick_y);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Redraw_Brick(int brick_x, int brick_y)
{
	RECT brick_rect;

	brick_rect.left = (AsConfig::Level_X_Offset + brick_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
	brick_rect.top = (AsConfig::Level_Y_Offset + brick_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
	brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

	InvalidateRect(AsConfig::Hwnd, &brick_rect, FALSE);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type)
{ // add an active brick to the level if we can
   int i;
   AActive_Brick *active_brick = 0;

   if (Active_Bricks_Count >= AsConfig::Max_Active_Bricks_Count)
   return; //When active_bricks too much.
   
   switch(brick_type)
   {
      case EBT_None:
         return;

      case EBT_Red:
      case EBT_Blue:
         active_brick = new AActive_Brick_Red_And_Blue(brick_type, brick_x, brick_y);
			Current_Level[brick_y][brick_x] = EBT_None; // remove the brick from the level
         break;

		case EBT_Unbreakable:
         active_brick = new AActive_Brick_Unbreakable(brick_x, brick_y);
			break;

		case EBT_Multihit_1:
			active_brick = new AActive_Brick_Multihit(brick_x, brick_y);
         Current_Level[brick_y][brick_x] = EBT_None; // remove the brick from the level
         break;

		case EBT_Multihit_2:
		case EBT_Multihit_3:
		case EBT_Multihit_4:
         Current_Level[brick_y][brick_x] = brick_type - 1;
         break;

      default:
         AsConfig::Throw();
   }
	// Added brick to the active bricks array
   for (i = 0; i < AsConfig::Max_Active_Bricks_Count; i++ )
   {
      if (Active_Bricks[i] == 0)
      {
         Active_Bricks[i] = active_brick;
         ++Active_Bricks_Count;
         break; 
      }
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AsLevel::Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type)
{ // add a falling letter to the level if we can
   int i;
	int letter_x, letter_y;
	ELetter_Type letter_type;
   AFalling_Letter* falling_letter;

   if (! (brick_type == EBT_Red || brick_type == EBT_Blue) )
		return false; // only red and blue bricks can give letters

   if (AsConfig::Rand(AsConfig::Hits_Per_Letter) != 0)
		return false; // not this time

   if (Falling_Letters_Count >= AsConfig::Max_Falling_Letters_Count)
		return false; // too many falling letters already   
      
            for (i = 0; i < AsConfig::Max_Falling_Letters_Count; i++)
            {
               if (Falling_Letters[i] == 0) // initialize the falling letters array
               {
						letter_x = (brick_x * AsConfig::Cell_Width + AsConfig::Level_X_Offset) * AsConfig::Global_Scale;
						letter_y = (brick_y * AsConfig::Cell_Height + AsConfig::Level_Y_Offset)* AsConfig::Global_Scale;

                  //letter_type = (ELetter_Type)AsConfig::Rand(ELT_Max - 1);
						letter_type = AFalling_Letter::Get_Random_Letter_Type();

                  falling_letter = new AFalling_Letter(brick_type, letter_type, letter_x, letter_y);
                  Falling_Letters[i] = falling_letter;
                  ++Falling_Letters_Count;
                  return true;; // add the letter to the array
               }
            }
            return false; // letter added
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double &reflection_pos)
{
   double direction = ball->Get_Direction();

   if (ball->Is_Moving_Up())
   {  //check the hit on the low edge of the brick
      if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Low_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos))
      {
         // checking the possibility of a downward rebound
         if (level_y < AsConfig::Level_Height - 1 && Current_Level[level_y + 1][level_x] == 0)
            return true;
         else
            return false;
      }
   }
   else
   {  //check the hit on the top edge of the brick
      if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Top_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos))
      {
         // checking the possibility of a upward rebound
         if (level_y > 0 && Current_Level[level_y - 1][level_x] == 0)
            return true;
         else
            return false;
      }
   }
   return false;
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double &reflection_pos)
{
   double direction = ball->Get_Direction();

   if (! ball->Is_Moving_Left())
   {  //check the hit on the left edge of the brick
      if (Hit_Circle_On_Line(Current_Brick_Left_X - next_x_pos, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos))
      {
         // checking the possibility of a rebound to the left
         if (level_x > 0 && Current_Level[level_y][level_x - 1] == 0)
            return true;
         else
            return false;
      }
   }
   else
	{  //check the hit on the right edge of the brick
      if (Hit_Circle_On_Line(Current_Brick_Right_X - next_x_pos, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos))
      {
         // checking the possibility of a rebound to the right
         if (level_x < AsConfig::Level_Width - 1 && Current_Level[level_y][level_x + 1] == 0)
            return true;
         else
            return false;
      }
   }
   return false;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Brick(HDC hdc, RECT &brick_rect, EBrick_Type brick_type)
{// drawer brick
   const AColor *color = 0;

   switch (brick_type)
   {
   case EBT_None:
   case EBT_Red:
   case EBT_Blue:
      AActive_Brick_Red_And_Blue::Draw_In_Level(hdc, brick_rect, brick_type);
      break;

	case EBT_Unbreakable:
      AActive_Brick_Unbreakable::Draw_In_Level(hdc, brick_rect);
      break;
   
   case EBT_Multihit_1:
   case EBT_Multihit_2:
   case EBT_Multihit_3:
   case EBT_Multihit_4:
      AActive_Brick_Multihit::Draw_In_Level(hdc, brick_rect, brick_type);
		break;

   default:
      AsConfig::Throw();
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Objects(HDC hdc, RECT& paint_area, AGraphics_Object** objects_array, int objects_max_count)
{
   int i;

   for (i = 0; i < objects_max_count; i++)
   {
      if (objects_array[i] != 0)
         objects_array[i]->Draw(hdc, paint_area);
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Act_Objects(AGraphics_Object** objects_array, int objects_max_count)
{
   int i;

   for (i = 0; i < objects_max_count; i++)
   {
      if (objects_array[i] != 0)
      {
         objects_array[i]->Act();

         if (objects_array[i]->Is_Finished())
         {
            delete objects_array[i];
            objects_array[i] = 0; // remove the finished active brick
            --Falling_Letters_Count;
         }
      }
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
