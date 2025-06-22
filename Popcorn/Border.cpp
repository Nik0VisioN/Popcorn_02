#include "Border.h"

// AsBorder
// --------------------------------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
: Border_Blue_Pen(0), Border_White_Pen(0), Border_Blue_Brush(0), Border_White_Brush(0)
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Init()
{
   AsConfig::Create_Pen_Brush(85, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
   AsConfig::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);

}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, RECT& paint_area)
{ // drawer bounds around the game

   int i;

   // 1. draw line on the left side
   for (i = 0; i < 50; i++)
      Draw_Element(hdc, 2, 1 + i * 4, false);

   // 2. draw line on the right side
   for (i = 0; i < 50; i++)
      Draw_Element(hdc, 201, 1 + i * 4, false);

   // 3. draw line on the top side
   for (i = 0; i < 50; i++)
      Draw_Element(hdc, 3 + i * 4, 0, true);
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
   bool got_hit = false;


   // adjusting the position when reflecting the ball
   if (next_x_pos - ball->Radius < AsConfig::Border_X_Offset)
   {
      got_hit = true;
      ball->Reflect(false);
   }

   if (next_y_pos - ball->Radius < AsConfig::Border_Y_Offset)
   {
      got_hit = true;
      ball->Reflect(true);
   }

   if (next_x_pos + ball->Radius > AsConfig::Max_X_Pos)
   {
      got_hit = true;
      ball->Reflect(false);
   }

   if (next_y_pos + ball->Radius > AsConfig::Max_Y_Pos)
   {
      if (AsConfig::Level_Has_Floor)
      {
         got_hit = true;
         ball->Reflect(true);
      }
      else
      {
         if (next_y_pos + ball->Radius > AsConfig::Max_Y_Pos + ball->Radius * 4)
            ball->Set_State(EBS_Lost, next_x_pos);
      }
   }
   return got_hit;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border)
{// drawer border around the game

   // draw color blue in the border
   SelectObject(hdc, Border_Blue_Pen);
   SelectObject(hdc, Border_Blue_Brush);

   if (top_border)
      Rectangle(hdc, x * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);
   else
      Rectangle(hdc, (x + 1) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

   // draw the white strip in border
   SelectObject(hdc, Border_White_Pen);
   SelectObject(hdc, Border_White_Brush);

   if (top_border)
      Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale);
   else
      Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

   // draw square in the border
   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);

   if (top_border)
      Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale, (y + 3) * AsConfig::Global_Scale);
   else
      Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale);
}
// --------------------------------------------------------------------------------------------------------------------------------------




