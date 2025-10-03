#include "Falling_Letter.h"

//AFalling_Letter
// --------------------------------------------------------------------------------------------------------------------------------------

AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
   : Brick_Type(brick_type), Letter_Type(letter_type), Falling_Letter_State(EFLS_Normal), X(x), Y(y), Rotation_Step(2), Next_Rotation_Tick(AsConfig::Current_Timer_Tick + Ticks_Per_Step)
{
   Letter_Cell.left = X;
   Letter_Cell.top = Y;
   Letter_Cell.right = Letter_Cell.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
   Letter_Cell.bottom = Letter_Cell.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

   Prev_Letter_Cell = Letter_Cell;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Act()
{
   if (Falling_Letter_State != EFLS_Normal)
		return; // do not move the letter if it got hit or finished falling

   if (Letter_Cell.top >= AsConfig::Max_Y_Pos * AsConfig::Global_Scale)
   {
      Finalize();
		return; // the letter reached the bottom of the window
   }

   Prev_Letter_Cell = Letter_Cell;

   Y += AsConfig::Global_Scale; // move the letter down
   Letter_Cell.top += AsConfig::Global_Scale;
   Letter_Cell.bottom += AsConfig::Global_Scale;

   if (AsConfig::Current_Timer_Tick >= Next_Rotation_Tick)
   {
      ++Rotation_Step;
      Next_Rotation_Tick += Ticks_Per_Step;
   }

   InvalidateRect(AsConfig::Hwnd, &Prev_Letter_Cell, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Letter_Cell, FALSE);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw(HDC hdc, RECT& paint_area)
{
   RECT intersection_rect;

   // clear background in the previous letter cell
   if (IntersectRect(&intersection_rect, &paint_area, &Prev_Letter_Cell))
   {
      SelectObject(hdc, AsConfig::BG_Pen);
      SelectObject(hdc, AsConfig::BG_Brush);

      Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right, Prev_Letter_Cell.bottom);
   }

   if(Falling_Letter_State == EFLS_Finalizing)
   {
      Falling_Letter_State = EFLS_Finished;
		return; // do not draw the letter if it got hit
   }

   if (IntersectRect(&intersection_rect, &paint_area, &Letter_Cell))
      Draw_Brick_Letter(hdc);
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AFalling_Letter::Is_Finished()
{
	if (Falling_Letter_State == EFLS_Finished)
	   return true;
   else
		return false;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Get_Letter_Cell(RECT& rect)
{
	rect = Letter_Cell;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Finalize()
{
   Falling_Letter_State = EFLS_Finalizing;
	InvalidateRect(AsConfig::Hwnd, &Prev_Letter_Cell, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Letter_Cell, FALSE);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Test_Draw_All_Steps(HDC hdc)
{
	int i;
	int x_step = AsConfig::Cell_Width * AsConfig::Global_Scale;

	Rotation_Step = 0;

   for (i = 0; i < Max_Rotation_Steps; i++)
   {
      Draw_Brick_Letter(hdc);

		++Rotation_Step;
		X += x_step;
		Letter_Cell.left += x_step;
		Letter_Cell.right += x_step;
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
   if (is_switch_color)
   {
      front_pen = AsConfig::Brick_Red_Pen;
      front_brush = AsConfig::Brick_Red_Brush;

      back_pen = AsConfig::Brick_Blue_Pen;
      back_brush = AsConfig::Brick_Blue_Brush;
   }
   else
   {
      front_pen = AsConfig::Brick_Blue_Pen;
      front_brush = AsConfig::Brick_Blue_Brush;

      back_pen = AsConfig::Brick_Red_Pen;
      back_brush = AsConfig::Brick_Red_Brush;
   }

}
// --------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
{  // drawer brick with letter
   bool switch_color;
   double offset;
   double rotation_angle; // rotation angle in radians
	double y_ratio; // ratio of the height of the brick to the original height (after rotation)
   int back_part_offset;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;
   XFORM xform, old_xform;

   if (!(Brick_Type == EBT_Blue || Brick_Type == EBT_Red))
      return; // the brick type with letter can be only red or blue

   // We adjust the rotation step and rotation angle
   Rotation_Step = Rotation_Step % Max_Rotation_Steps;

   if (Rotation_Step < 8)
      rotation_angle = 2.0 * M_PI / (double)Max_Rotation_Steps * (double)Rotation_Step;
   else
      rotation_angle = 2.0 * M_PI / (double)Max_Rotation_Steps * (double)(8 - Rotation_Step);

   if (Rotation_Step > 4 && Rotation_Step <= 12)
   {
      if (Brick_Type == EBT_Blue)
         switch_color = true;
      else
         switch_color = false;
   }
   else
   {
      if (Brick_Type == EBT_Red)
         switch_color = true;
      else
         switch_color = false;
   }

   Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

   if (Rotation_Step == 4 || Rotation_Step == 12)
   {
      // draw the back part
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, X, Y + Brick_Half_Height - AsConfig::Global_Scale, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + Brick_Half_Height);

      // draw the front part
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, X, Y + Brick_Half_Height, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + Brick_Half_Height + AsConfig::Global_Scale - 1);
   }
   else
   {
      // set the transformation matrix
      y_ratio = cos(rotation_angle);

      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)fabs(y_ratio);
      xform.eDx = (float)X;
      xform.eDy = (float)Y + (float)(Brick_Half_Height);
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      // draw the back part
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)AsConfig::Global_Scale;
      back_part_offset = (int)round(offset);

      if (y_ratio < 0.0)
			back_part_offset = -back_part_offset;

      Rectangle(hdc, 0, -Brick_Half_Height - back_part_offset, AsConfig::Brick_Width * AsConfig::Global_Scale - 1, Brick_Half_Height - back_part_offset);

      // draw the front part
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, -Brick_Half_Height, AsConfig::Brick_Width * AsConfig::Global_Scale - 1, Brick_Half_Height);
       
      if (Rotation_Step > 4 && Rotation_Step < 12)
      {
         SelectObject(hdc, AsConfig::Letter_Pen);
         switch (Letter_Type)
         {
         case ELT_O:
            Ellipse(hdc, 5 * AsConfig::Global_Scale, 1 * AsConfig::Global_Scale - Brick_Half_Height, 10 * AsConfig::Global_Scale, 6 * AsConfig::Global_Scale - Brick_Half_Height - 1);
            break;

         case ELT_I:
            Draw_Line(hdc, 7, 1, 7, 6);
				break;

         case ELT_C:
            Arc(hdc, 5 * AsConfig::Global_Scale, 1 * AsConfig::Global_Scale - Brick_Half_Height, 10 * AsConfig::Global_Scale, 6 * AsConfig::Global_Scale - Brick_Half_Height - 1, 
               10 * AsConfig::Global_Scale, 1 * AsConfig::Global_Scale - Brick_Half_Height, 10 * AsConfig::Global_Scale, 6 * AsConfig::Global_Scale - Brick_Half_Height - 1);
			break;

         case ELT_M:
            Draw_Line(hdc, 5, 6, 5, 1);
            Draw_Line_To(hdc, 7, 3);
            Draw_Line_To(hdc, 9, 1);
            Draw_Line_To(hdc, 9, 6);
            break;

         case ELT_G:
            // C
            Arc(hdc, 5 * AsConfig::Global_Scale, 1 * AsConfig::Global_Scale - Brick_Half_Height, 10 * AsConfig::Global_Scale, 6 * AsConfig::Global_Scale - Brick_Half_Height - 1,
               10 * AsConfig::Global_Scale, 1 * AsConfig::Global_Scale - Brick_Half_Height, 10 * AsConfig::Global_Scale, 6 * AsConfig::Global_Scale - Brick_Half_Height - 1);
				// middle part
            Draw_Line(hdc, 9, 3, 7, 3);
            Draw_Line(hdc, 9, 3, 9, 5);
            break;

			case ELT_K:
				Draw_Line(hdc, 5, 1, 5, 6);
				Draw_Line(hdc, 9, 1, 5, 4);
				Draw_Line(hdc, 6, 4, 9, 6);
				break;

         case ELT_W:
            Draw_Line(hdc, 5, 1, 6, 6);
            Draw_Line_To(hdc, 7, 3);
            Draw_Line_To(hdc, 8, 6);
            Draw_Line_To(hdc, 9, 1);
				break;

         case ELT_P:
				Draw_Line(hdc, 6, 6, 6, 1);
            Draw_Line_To(hdc, 9, 1);
            Draw_Line_To(hdc, 9, 4);
            Draw_Line_To(hdc, 6, 4);
            break;

         case ELT_L:
            Draw_Line(hdc, 5, 1, 5, 6);
            Draw_Line(hdc, 5, 6, 9, 6);
            break;

         case ELT_T:
            Draw_Line(hdc, 5, 1, 9, 1);
            Draw_Line(hdc, 7, 1, 7, 6);
            break;

         case ELT_Plus:
            Draw_Line(hdc, 7, 1, 7, 5);
            Draw_Line(hdc, 5, 3, 9, 3);
            break;
         }
      }
      SetWorldTransform(hdc, &old_xform);
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Line(HDC hdc, int x_1, int y_1, int x_2, int y_2)
{
	int start_y, end_y;

   start_y = y_1 * AsConfig::Global_Scale - Brick_Half_Height;
   if (y_1 == 6)
		--start_y;

   end_y = y_2 * AsConfig::Global_Scale - Brick_Half_Height;
   if (y_2 == 6)
      --end_y;

   MoveToEx(hdc, x_1 * AsConfig::Global_Scale + 1, start_y, 0);
   LineTo(hdc, x_2 * AsConfig::Global_Scale + 1, end_y);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Line_To(HDC hdc, int x, int y)
{
   int end_y = y * AsConfig::Global_Scale - Brick_Half_Height;
   if (y == 6)
      --end_y;

   LineTo(hdc, x * AsConfig::Global_Scale + 1, end_y);
}
// --------------------------------------------------------------------------------------------------------------------------------------
