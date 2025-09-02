#include "Platform.h"

// AsPlatform
// --------------------------------------------------------------------------------------------------------------------------------------
AsPlatform::~AsPlatform()
{
	delete[] Normal_Platform_Image;
}
// --------------------------------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
: X_Pos(AsConfig::Border_X_Offset), X_Step(AsConfig::Global_Scale * 2), Platform_State(EPS_Normal), Inner_Width(Normal_Platform_Inner_Width), Rolling_Step(0),
  Normal_Platform_Image_Width(0), Normal_Platform_Image_Height(0), Normal_Platform_Image(0),
  Meltdown_Platform_Y_Pos{}, Width(Normal_Width), Platform_Rect{}, Prev_Platform_Rect{}, Highlight_Pen(0), Platform_Circle_Pen(0), Platform_Inner_Pen(0), 
  Platform_Circle_Brush(0), Platform_Inner_Brush(0)
{
   X_Pos = (AsConfig::Max_X_Pos - Width) / 2;
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
	double inner_left_x, inner_right_x;
   double inner_top_y, inner_low_y;
   double inner_y;
	double reflection_pos;

   if (next_y_pos + ball->Radius < AsConfig::Platform_Y_Pos)
   return false;

   inner_left_x = (double)(X_Pos + Circle_Size - 1);
   inner_right_x = (double)(X_Pos + Width - (Circle_Size - 1));
   inner_top_y = (double)(AsConfig::Platform_Y_Pos - 1);
   inner_low_y = (double)(AsConfig::Platform_Y_Pos + Height - 1);


   // Check for reflection from the left ball of the platform
	if(Reflect_On_Circle(next_x_pos, next_y_pos, 0.0, ball))
      return true;

   // Check for reflection from the right ball of the platform
   if (Reflect_On_Circle(next_x_pos, next_y_pos, Width - Circle_Size, ball))
      return true;

   if (ball->Is_Moving_Up())
      inner_y = inner_low_y;   // Check the reflection from the bottom edge of the platform
	else
		inner_y = inner_top_y;   // the top edge of the platform

   if (Hit_Circle_On_Line(next_y_pos - inner_y, next_x_pos, inner_left_x, inner_right_x, ball->Radius, reflection_pos))
   {
      ball->Reflect(true);
      return true;
   }
	return false;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

   AsConfig::Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
   AsConfig::Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Act()
{
   switch(Platform_State)
      case EPS_Meltdown:
      case EPS_Roll_In:
		case EPS_Expand_Roll_In:
      Redraw_Platform();
}
// --------------------------------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform::Get_State()
{
	return Platform_State;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_State new_state)
{
   int i, len;

   if (Platform_State == new_state)
		return; // if the state is the same, do nothing

   switch (new_state)
   {
   case EPS_Meltdown:

       len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);  // lifehack ( sizeof() operator returns the size of the array in bytes, so we divide it by the size of one element to get the number of elements in the array)

      for (i = 0; i < len; i++)
         Meltdown_Platform_Y_Pos[i] = Platform_Rect.bottom;
      break;

	case EPS_Roll_In:
		X_Pos = AsConfig::Max_X_Pos - 1;
      Rolling_Step = Max_Rolling_Step - 1;
		break;

   }
         Platform_State = new_state;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform()
{
   Prev_Platform_Rect = Platform_Rect;

	int platform_width;

   if (Platform_State == EPS_Roll_In)
      platform_width = Circle_Size;
   else
      platform_width = Width;


      Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
      Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
      Platform_Rect.right = Platform_Rect.left + platform_width * AsConfig::Global_Scale;
      Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

	if (Platform_State == EPS_Meltdown)
      Prev_Platform_Rect.bottom = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

   InvalidateRect(AsConfig::Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Platform_Rect, FALSE);

}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

   if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      return;


   switch(Platform_State)
   {
   case EPS_Normal:
	   Draw_Normal_State(hdc, paint_area);
	   break;

	case EPS_Meltdown:
	   Draw_Meltdown_State(hdc, paint_area);
	   break;

   case EPS_Roll_In:
      Draw_Roll_In_State(hdc, paint_area);
      break;

   case EPS_Expand_Roll_In:
      Draw_Expanding_Roll_In_State(hdc, paint_area);
      break;
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Clear_BG(HDC hdc)
{   // CLEARING PREVIOUS SPACE WITH BACKGROUND

   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);

   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle_Highlight(HDC hdc, int x, int y)
{ // draw circle highlight on the platform
   SelectObject(hdc, Highlight_Pen);

   Arc(hdc, x + AsConfig::Global_Scale, y + AsConfig::Global_Scale, x + (Circle_Size - 1) * AsConfig::Global_Scale - 1, y + (Circle_Size - 1) * AsConfig::Global_Scale - 1,
      x + 2 * AsConfig::Global_Scale, y + AsConfig::Global_Scale, x + AsConfig::Global_Scale, y + 3 * AsConfig::Global_Scale);

}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc, RECT &paint_area)
{ // drawer platform in normal state
   
	int i, j;
	int offset = 0;
   int x = X_Pos;
   int y = AsConfig::Platform_Y_Pos;

   // CLEANING PREVIOUS SPACE WITH BACKGROUND
	Clear_BG(hdc);

   // draw the side balls
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);

   Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size) * AsConfig::Global_Scale - 1, (y + Circle_Size) * AsConfig::Global_Scale - 1);
   Ellipse(hdc, (x + Inner_Width) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size + Inner_Width) * AsConfig::Global_Scale - 1, (y + Circle_Size) * AsConfig::Global_Scale - 1);


   // draw highlight
	AsPlatform::Draw_Circle_Highlight(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale);

   // draw the inner part ( platform )
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);

   RoundRect(hdc, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4 + Inner_Width - 1) * AsConfig::Global_Scale - 1, (y + 1 + 5) * AsConfig::Global_Scale - 1, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);

   if(Normal_Platform_Image == 0)
   {
		Normal_Platform_Image_Width = Width * AsConfig::Global_Scale;
		Normal_Platform_Image_Height = Height * AsConfig::Global_Scale;

      Normal_Platform_Image = new int[Normal_Platform_Image_Width * Normal_Platform_Image_Height]; 

      for (i = 0; i < Normal_Platform_Image_Height; i++);
         for (j = 0; j < Normal_Platform_Image_Width; j++);
            Normal_Platform_Image[offset++] = GetPixel(hdc, x + j, y + i);
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT &paint_area)
{// draw platform in meltdown state

   int i, j;
	int x, y;
   int y_offset;
	int area_width, area_height;
   int moved_columns_count = 0;
   int max_platform_y;
	COLORREF pixel;
   COLORREF bg_pixel = RGB (AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);
 
	area_width = Width * AsConfig::Global_Scale;
	area_height = Height * AsConfig::Global_Scale + 1;
   
	max_platform_y = AsConfig::Max_Y_Pos * AsConfig::Global_Scale + area_height;

   for(i = 0; i < area_width; i++)
   {
      if(Meltdown_Platform_Y_Pos[i] > max_platform_y)
         continue;

      ++moved_columns_count;

      y_offset = AsConfig::Rand(Meltdown_Speed) + 1;
      x = Platform_Rect.left + i;

		for (j = 0; j < area_height; j++)
      {
			y = Meltdown_Platform_Y_Pos[i] - j;

         pixel = GetPixel(hdc, x, y);
			SetPixel(hdc, x, y + y_offset, pixel);
      }

      for (j = 0; j < y_offset; j++)
      {
      y = Meltdown_Platform_Y_Pos[i] - area_height + 1 + j;
      SetPixel(hdc, x, y, bg_pixel);
      }

      Meltdown_Platform_Y_Pos[i] += y_offset;
   }
   if (moved_columns_count == 0)
		Platform_State = EPS_Missing; // the entire platform moved beyond the window
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT& paint_area)
{ // draw platform in roll-in state

   int x = X_Pos * AsConfig::Global_Scale;
   int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   int roller_size = Circle_Size * AsConfig::Global_Scale;
   double alpha;
   XFORM xform, old_xform;
   
   Clear_BG(hdc);

   //1. ball
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);

   Ellipse(hdc, x , y , x + roller_size - 1, y + roller_size - 1);


   //2. dividing line
	alpha = -2.0 * M_PI / (double)Max_Rolling_Step * (double)Rolling_Step; // rotation of the platform ball when rolling out

   xform.eM11 = (float)cos(alpha);
   xform.eM12 = (float)sin(alpha);
   xform.eM21 = (float)-sin(alpha);
   xform.eM22 = (float)cos(alpha);
   xform.eDx = (float)(x + roller_size / 2);
   xform.eDy = (float)(y + roller_size / 2);
   GetWorldTransform(hdc, &old_xform);
   SetWorldTransform(hdc, &xform);

   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);

   Rectangle(hdc, -AsConfig::Global_Scale / 2, -roller_size / 2, AsConfig::Global_Scale / 2 - 1, roller_size / 2 - 1);
   
   SetWorldTransform(hdc, &old_xform);

	//3. highlight
   Draw_Circle_Highlight(hdc, x, y);

   ++Rolling_Step;

	if (Rolling_Step >= Max_Rolling_Step)
		Rolling_Step -= Max_Rolling_Step;

	X_Pos -= Rolling_Platform_Speed;

   if (X_Pos <= Roll_In_Platform_End_X_Pos)
   {
      X_Pos += Rolling_Platform_Speed;
		Platform_State = EPS_Expand_Roll_In;
      Inner_Width = 1;
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Expanding_Roll_In_State(HDC hdc, RECT& paint_area)
{  //We draw a platform that expands after rolling out

	Draw_Normal_State(hdc, paint_area);

   --X_Pos;
	Inner_Width += 2;

	if (Inner_Width >= Normal_Platform_Inner_Width)
   {
		Inner_Width = Normal_Platform_Inner_Width;
		Platform_State = EPS_Ready; // return to normal state
		Redraw_Platform();
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AsPlatform::Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall* ball)
{
   double dx, dy;
   double platform_ball_x, platform_ball_y, platform_ball_radius;
   double distance, two_radiuses;
   double beta, alpha, gamma;
   double related_ball_direction;
   const double pi_2 = 2.0 * M_PI;

   platform_ball_radius = (double)Circle_Size / 2.0;
   platform_ball_x = (double)X_Pos + platform_ball_radius + platform_ball_x_offset;
   platform_ball_y = (double)AsConfig::Platform_Y_Pos + platform_ball_radius;

   // Check the reflection with the left and right edges of the platform
   // Check the left edge of the platform
   dx = next_x_pos - platform_ball_x;
   dy = next_y_pos - platform_ball_y;

   distance = sqrt(dx * dx + dy * dy);
   two_radiuses = platform_ball_radius + ball->Radius;

   if (fabs(distance - two_radiuses) < AsConfig::Moving_Step_Size)
   {// The ball is touching the left edge of the platform
      beta = atan2(-dy, dx);

      related_ball_direction = ball->Get_Direction();
      related_ball_direction -= beta;

      if (related_ball_direction > pi_2)
         related_ball_direction -= pi_2;

      if (related_ball_direction < 0.0)
         related_ball_direction += pi_2;

      if (related_ball_direction > M_PI_2 && related_ball_direction < M_PI + M_PI_2)
      {
         alpha = beta + M_PI - ball->Get_Direction();
         gamma = beta + alpha;

         ball->Set_Direction(gamma);

         return true;
      }
   }
   return false;
}
// --------------------------------------------------------------------------------------------------------------------------------------


