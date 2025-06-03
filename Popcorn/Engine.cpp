#include "Engine.h"

// --------------------------------------------------------------------------------------------------------------------------------------

char Level_01[ALevel::Level_Height][ALevel::Level_Width] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};



//ABall
// --------------------------------------------------------------------------------------------------------------------------------------
ABall::ABall()
: Ball_X_Pos(20), Ball_Y_Pos(170), Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4)
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT& paint_area, AsEngine *engine)
{
   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
		return; // if the ball is not in the paint area, do not draw it

   // clear the previous ball
   SelectObject(hdc, engine->BG_Pen);
   SelectObject(hdc, engine->BG_Brush);

   Rectangle(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

   // draw the ball
   SelectObject(hdc, Ball_Pen);
   SelectObject(hdc, Ball_Brush);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);

}
// --------------------------------------------------------------------------------------------------------------------------------------
void ABall::Move(AsEngine *engine, ALevel *level)
{
   int next_x_pos, next_y_pos;
   int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
   int platform_y_pos = AsEngine::Platform_Y_Pos - Ball_Size;

   Prev_Ball_Rect = Ball_Rect;

   next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction));
   next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

   // adjusting the position when reflecting the ball
   if (next_x_pos < AsEngine::Border_X_Offset)
   {
      next_x_pos = ALevel::Level_X_Offset - (next_x_pos - ALevel::Level_X_Offset);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if (next_y_pos < AsEngine::Border_Y_Offset)
   {
      next_y_pos = AsEngine::Border_Y_Offset - (next_y_pos - AsEngine::Border_Y_Offset);
      Ball_Direction = -Ball_Direction;
   }

   if (next_x_pos > max_x_pos)
   {
      next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if (next_y_pos > AsEngine::Max_Y_Pos)
   {
      next_y_pos = AsEngine::Max_Y_Pos - (next_y_pos - AsEngine::Max_Y_Pos);
      Ball_Direction = M_PI + (M_PI - Ball_Direction);;
   }

   // correct the position of the ball when it is on the platform
   if (next_y_pos > platform_y_pos)
   {
      if (next_x_pos >= engine->Platform_X_Pos && next_x_pos <= engine->Platform_X_Pos + engine->Platform_Width)
      {
         next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
         Ball_Direction = M_PI + (M_PI - Ball_Direction);
      }
   }

   // correct the position of the ball when it is out of bricks
   level->Chech_Level_Brick_Hit(next_y_pos, Ball_Direction);

   // move the ball
   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Ball_X_Pos += (int)(Ball_Speed * cos(Ball_Direction));
   Ball_Y_Pos -= (int)(Ball_Speed * sin(Ball_Direction));

   Ball_Rect.left = Ball_X_Pos * AsEngine::Global_Scale;
   Ball_Rect.top = Ball_Y_Pos * AsEngine::Global_Scale;
   Ball_Rect.right = Ball_Rect.left + Ball_Size * AsEngine::Global_Scale;
   Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsEngine::Global_Scale;

   InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);

}
// --------------------------------------------------------------------------------------------------------------------------------------






// ALevel
// --------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
   
   Letter_Pen = CreatePen(PS_SOLID, AsEngine::Global_Scale, RGB(255, 255, 255));

   AsEngine::Create_Pen_Brush(255, 85, 255, Brick_Red_Pen, Brick_Red_Brush);
   AsEngine::Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
   
   Level_Rect.left = ALevel::Level_X_Offset * AsEngine::Global_Scale;
   Level_Rect.top = ALevel::Level_Y_Offset * AsEngine::Global_Scale;
   Level_Rect.right = Level_Rect.left + ALevel::Cell_Width * ALevel::Level_Width * AsEngine::Global_Scale;
   Level_Rect.bottom = Level_Rect.top + ALevel::Cell_Height * ALevel::Level_Height * AsEngine::Global_Scale;

}
// --------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Chech_Level_Brick_Hit(int& next_y_pos, double &ball_direction)
{// correct the position of the ball when it is out of bricks

   int i, j;
   int brick_y_pos = Level_Y_Offset + Level_Height * Cell_Height;

   for (i = Level_Height - 1; i >= 0; i--)
   {
      for (j = 0; j < Level_Width; j++)
      {
         if (Level_01[i][j] == 0)
            continue;

         if (next_y_pos < brick_y_pos)
         {
            next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
            ball_direction = -ball_direction;
         }
      }
      brick_y_pos -= Cell_Height;
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type)
{// drawer brick

   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBT_None:
      return;

   case EBT_Red:
      pen = Brick_Red_Pen;
      brush = Brick_Red_Brush;
      break;

   case EBT_Blue:
      pen = Brick_Blue_Pen;
      brush = Brick_Blue_Brush;
      break;

   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   RoundRect(hdc, x * AsEngine::Global_Scale, y * AsEngine::Global_Scale, (x + Brick_Width) * AsEngine::Global_Scale, (y + Brick_Height) * AsEngine::Global_Scale, 2 * AsEngine::Global_Scale, 2 * AsEngine::Global_Scale);

}
// --------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
   if (is_switch_color)
   {
      front_pen = Brick_Red_Pen;
      front_brush = Brick_Red_Brush;

      back_pen = Brick_Blue_Pen;
      back_brush = Brick_Blue_Brush;
   }
   else
   {
      front_pen = Brick_Blue_Pen;
      front_brush = Brick_Blue_Brush;

      back_pen = Brick_Red_Pen;
      back_brush = Brick_Red_Brush;
   }

}
// --------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, Ebrick_Type brick_type, Eletter_Type letter_type, int rotation_step)
{  // drawer brick with letter
   bool switch_color;
   double offset;
   double rotation_angle; // rotation angle in radians
   int brick_half_height = Brick_Height * AsEngine::Global_Scale / 2;
   int back_part_offset;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;
   XFORM xform, old_xform;

   if (!(brick_type == EBT_Blue || brick_type == EBT_Red))
      return; // the brick type with letter can be only red or blue


   // We adjust the rotation step and rotation angle
   rotation_step = rotation_step % 16;

   if (rotation_step < 8)
      rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
   else
      rotation_angle = 2.0 * M_PI / 16.0 * (double)(8L - (long long)rotation_step);

   if (rotation_step > 4 && rotation_step <= 12)
   {
      if (brick_type == EBT_Blue)
         switch_color = true;
      else
         switch_color = false;
   }
   else
   {
      if (brick_type == EBT_Red)
         switch_color = true;
      else
         switch_color = false;
   }

   Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

   if (rotation_step == 4 || rotation_step == 12)
   {
      // draw the back part
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, x, y + brick_half_height - AsEngine::Global_Scale, x + Brick_Width * AsEngine::Global_Scale, y + brick_half_height);

      // draw the front part
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * AsEngine::Global_Scale, y + brick_half_height + AsEngine::Global_Scale);
   }
   else
   {

      SetGraphicsMode(hdc, GM_ADVANCED);

      // set the transformation matrix
      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)cos(rotation_angle);
      xform.eDx = (float)x;
      xform.eDy = (float)y + (float)(brick_half_height);
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      // draw the back part
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)AsEngine::Global_Scale;
      back_part_offset = (int)round(offset);
      Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * AsEngine::Global_Scale, brick_half_height - (int)round(offset));

      // draw the front part
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, -brick_half_height, Brick_Width * AsEngine::Global_Scale, brick_half_height);

      if (rotation_step > 4 && rotation_step < 12)
      {
         if (letter_type == ELT_0)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * AsEngine::Global_Scale, (-5 * AsEngine::Global_Scale) / 2, 0 + 10 * AsEngine::Global_Scale, 5 * AsEngine::Global_Scale / 2);
         }
      }
      SetWorldTransform(hdc, &old_xform);
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Level(HDC hdc, RECT& paint_area)
{ // drawer level
   int i, j;
   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
		return; // if the level is not in the paint area, do not draw it

   for (i = 0; i < Level_Height; i++)
      for (j = 0; j < Level_Width; j++)
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (Ebrick_Type)Level_01[i][j]);
}
// --------------------------------------------------------------------------------------------------------------------------------------






// AsEngine
// --------------------------------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
: Inner_Width(21), Platform_X_Pos(Border_X_Offset), Platform_X_Step(Global_Scale * 2), Platform_Width(28)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{// setting the game on start

   Hwnd = hwnd;

   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
   Create_Pen_Brush(15, 73, 31, BG_Pen, BG_Brush);
   Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
   Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
   Create_Pen_Brush(255, 255, 255, Ball.Ball_Pen, Ball.Ball_Brush);
   Create_Pen_Brush(85, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
   Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);

   Level.Init();
   Redraw_Platform();

   SetTimer(Hwnd, Timer_ID, 50, 0);

}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)
{ // drawer screen game

   RECT intersection_rect;

      Level.Draw_Level(hdc, paint_area);

   if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      Draw_Platform(hdc, Platform_X_Pos, Platform_Y_Pos);

   /* int i;

    for (i = 0; i < 16; i++)
    {
       Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_0, i);
       Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Red, ELT_0, i);
    }*/
   
   Ball.Draw(hdc, paint_area, this);

   Draw_Bounds(hdc, paint_area);
}
// --------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(Ekey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform_X_Pos -= Platform_X_Step;

      if (Platform_X_Pos <= Border_X_Offset)
         Platform_X_Pos = Border_X_Offset;

      Redraw_Platform();
      break;

   case EKT_Right:
      Platform_X_Pos += Platform_X_Step;

      if (Platform_X_Pos >= Max_X_Pos - Platform_Width - 1)
         Platform_X_Pos = Max_X_Pos - Platform_Width + 1;

      Redraw_Platform();
      break;

   case EKT_Space:
      break;
   }
   return 0;
}
// --------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
   Ball.Move(this, &Level);
   return 0; 
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{

   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));

};

void AsEngine::Redraw_Platform()
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = Platform_X_Pos * Global_Scale;
   Platform_Rect.top = Platform_Y_Pos * Global_Scale;
   Platform_Rect.right = Platform_Rect.left + Platform_Width * Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Global_Scale;

   InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(Hwnd, &Platform_Rect, FALSE);

}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Platform(HDC hdc, int x, int y)
{ // drawer platform

   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);

	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   // draw the side balls
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);

   Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Circle_Size + Inner_Width) * Global_Scale, (y + Circle_Size) * Global_Scale);


	// draw highlight
   SelectObject(hdc, Highlight_Pen);

   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale,
      (x + 1 + 1)* Global_Scale, (y + 1)* Global_Scale, (x + 1) * Global_Scale, (y + 1 + 2) * Global_Scale);

	// draw the inner part ( platform )
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);

   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale, 3 * Global_Scale, 3 * Global_Scale);

}

// --------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Draw_Border(HDC hdc, int x, int y, bool top_border)
{// drawer border around the game

   // draw color blue in the border
   SelectObject(hdc, Border_Blue_Pen);
   SelectObject(hdc, Border_Blue_Brush);

   if(top_border)
      Rectangle(hdc, x * Global_Scale, (y + 1) * Global_Scale, (x + 4) * Global_Scale, (y + 4) * Global_Scale);
   else
		Rectangle(hdc, (x + 1) * Global_Scale, y * Global_Scale, (x + 4) * Global_Scale, (y + 4) * Global_Scale);

   // draw the white strip in border
   SelectObject(hdc, Border_White_Pen);
   SelectObject(hdc, Border_White_Brush);

   if (top_border)
      Rectangle(hdc, x * Global_Scale, y * Global_Scale, (x + 4) * Global_Scale, (y + 1) * Global_Scale);
   else
		Rectangle(hdc, x * Global_Scale, y * Global_Scale, (x + 1) * Global_Scale, (y + 4) * Global_Scale);

   // draw square in the border
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);

   if (top_border)
      Rectangle(hdc, (x + 2) * Global_Scale, (y + 2) * Global_Scale, (x + 3) * Global_Scale, (y + 3) * Global_Scale);
	else
		Rectangle(hdc, (x + 2) * Global_Scale, (y + 1) * Global_Scale, (x + 3) * Global_Scale, (y + 2) * Global_Scale);

}

// --------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Draw_Bounds(HDC hdc, RECT& paint_area)
{ // drawer bounds around the game

   int i;

	// 1. draw line on the left side
   for (i = 0; i < 50; i++)
      Draw_Border(hdc, 2, 1 + i * 4, false);

	// 2. draw line on the right side
   for (i = 0; i < 50; i++)
      Draw_Border(hdc, 201, 1 + i * 4, false);

	// 3. draw line on the top side
	for (i = 0; i < 50; i++)
		Draw_Border(hdc, 3 + i * 4, 0, true);

}
// --------------------------------------------------------------------------------------------------------------------------------------
