#include "Active_Brick.h"


//AGraphics_Object
// --------------------------------------------------------------------------------------------------------------------------------------
AGraphics_Object::~AGraphics_Object()
{
}
// --------------------------------------------------------------------------------------------------------------------------------------




//AActive_Brick
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick::Get_Level_Pos(int& dest_brick_x, int& dest_brick_y)
{
	dest_brick_x = Level_X;
	dest_brick_y = Level_Y;
}
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick::~AActive_Brick()
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick(EBrick_Type brick_type, int level_x, int level_y)
   : Brick_Type(brick_type), Level_X(level_x), Level_Y(level_y),  Brick_Rect{}
{
   Brick_Rect.left = (AsConfig::Level_X_Offset + level_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
   Brick_Rect.top = (AsConfig::Level_Y_Offset + level_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
   Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
   Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick::Clear(HDC hdc, RECT& paint_area)
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
double AActive_Brick::Get_Brick_X_Pos(bool of_center)
{
   double pos = (double)(AsConfig::Level_X_Offset + Level_X * AsConfig::Cell_Width);

   if (of_center)
      pos += (double)AsConfig::Brick_Width / 2.0;

   return pos;
}
// --------------------------------------------------------------------------------------------------------------------------------------
double AActive_Brick::Get_Brick_Y_Pos(bool of_center)
{
   double pos = (double)(AsConfig::Level_Y_Offset + Level_Y * AsConfig::Cell_Height);

   if (of_center)
      pos += (double)AsConfig::Brick_Height / 2.0;

   return pos;
}
// --------------------------------------------------------------------------------------------------------------------------------------




//AActive_Brick_Purple_And_Blue
// --------------------------------------------------------------------------------------------------------------------------------------
AColor AActive_Brick_Purple_And_Blue::Fading_Purple_Brick_Colors[Max_Fade_Step];
AColor AActive_Brick_Purple_And_Blue::Fading_Blue_Brick_Colors[Max_Fade_Step];
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick_Purple_And_Blue::~AActive_Brick_Purple_And_Blue()
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick_Purple_And_Blue::AActive_Brick_Purple_And_Blue(EBrick_Type brick_type, int level_x, int level_y)
	: AActive_Brick(brick_type, level_x, level_y), Fade_Step(0)
{
   if (!(brick_type == EBT_Purple || brick_type == EBT_Blue) )
      AsConfig::Throw(); // only red and blue bricks can be fading
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Purple_And_Blue::Act()
{

   if (Fade_Step < Max_Fade_Step - 1)
   {
      ++Fade_Step;
      InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
   }
}
void AActive_Brick_Purple_And_Blue::Draw(HDC hdc, RECT& paint_area)  
{  // drawer fading brick  
	AColor *color = 0;

   switch (Brick_Type)  
   {  
   case EBT_Purple:  
      color = &Fading_Purple_Brick_Colors[Fade_Step];  
      break;  

   case EBT_Blue:  
      color = &Fading_Blue_Brick_Colors[Fade_Step];  
      break;  
   }  

   if (color != 0)
		color->Select(hdc);

   AsConfig::Round_Rect(hdc, Brick_Rect, 2);
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Purple_And_Blue::Is_Finished()
{
   if (Fade_Step >= Max_Fade_Step - 1)
		return true; // the brick is finished
   else
		return false; // the brick is not finished
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Purple_And_Blue::Setup_Colors()
{
   int i;

   for (i = 0; i < Max_Fade_Step; i++)
	{
      Get_Fading_Color(AsConfig::Purple_Color, i, Fading_Purple_Brick_Colors[i]);
      Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Brick_Colors[i]);
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Purple_And_Blue::Draw_In_Level(HDC hdc, RECT& brick_rect, EBrick_Type brick_type)
{
   const AColor* color = 0;

   switch (brick_type)
   {
   case EBT_None:
      color = &AsConfig::BG_Color;
      break;

   case EBT_Purple:
      color = &AsConfig::Purple_Color;
      break;

   case EBT_Blue:
      color = &AsConfig::Blue_Color;
      break;

   default:
      AsConfig::Throw();
   }

   if (color != 0)
      color -> Select(hdc);

   AsConfig::Round_Rect(hdc, brick_rect, 2);
}
// --------------------------------------------------------------------------------------------------------------------------------------
unsigned char AActive_Brick_Purple_And_Blue::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
   return color - step * (color - bg_color) / (Max_Fade_Step - 1);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Purple_And_Blue::Get_Fading_Color(const AColor &origin_color, int step, AColor &result_color)
{
   unsigned char r, g, b;

   r = Get_Fading_Channel(origin_color.R, AsConfig::BG_Color.R, step);
   g = Get_Fading_Channel(origin_color.G, AsConfig::BG_Color.G, step);
   b = Get_Fading_Channel(origin_color.B, AsConfig::BG_Color.B, step);

	result_color = AColor(r, g, b);
}
// --------------------------------------------------------------------------------------------------------------------------------------




//AActive_Brick_Unbreakable
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::~AActive_Brick_Unbreakable()
{
	DeleteObject(Region);
}
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::AActive_Brick_Unbreakable(int level_x, int level_y)
	: AActive_Brick(EBT_Unbreakable, level_x, level_y), Animation_Step(0), Region(0)
{
   Region = CreateRoundRectRgn(Brick_Rect.left, Brick_Rect.top, Brick_Rect.right + 1, Brick_Rect.bottom + 1, 2 * AsConfig::Global_Scale - 1, 2 * AsConfig::Global_Scale - 1);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Act()
{
   if (Animation_Step <= Max_Animation_Step)
   {
      ++Animation_Step;
      InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw(HDC hdc, RECT& paint_area)
{
   int offset;
	const int scale = AsConfig::Global_Scale;
  
   Draw_In_Level(hdc, Brick_Rect);

	SelectClipRgn(hdc, Region);

   offset = 2 * Animation_Step * scale - AsConfig::Brick_Width * scale;

   AsConfig::Unbreakable_Blue_Highlight.Select_Pen(hdc);
   MoveToEx(hdc, Brick_Rect.left + 4 * scale + offset, Brick_Rect.bottom + scale, 0);
   LineTo(hdc, Brick_Rect.left + 13 * scale + offset - 1, Brick_Rect.top - 1 * scale);

   AsConfig::Unbreakable_Purple_Highlight.Select_Pen(hdc);
   MoveToEx(hdc, Brick_Rect.left + 6 * scale + offset, Brick_Rect.bottom + scale, 0);
   LineTo(hdc, Brick_Rect.left + 15 * scale + offset - 1, Brick_Rect.top - 1 * scale);

   SelectClipRgn(hdc, 0);
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Unbreakable::Is_Finished()
{
   if (Animation_Step >= Max_Animation_Step)
      return true; // the brick is finished
   else
      return false; // the brick is not finished
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw_In_Level(HDC hdc, RECT& brick_rect)
{// drawer unbreakable brick
	AsConfig::White_Color.Select(hdc);
   AsConfig::Round_Rect(hdc, brick_rect);
}
// --------------------------------------------------------------------------------------------------------------------------------------




//AActive_Brick_Multihit
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick_Multihit::~AActive_Brick_Multihit()
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick_Multihit::AActive_Brick_Multihit(int level_x, int level_y)
	: AActive_Brick(EBT_Multihit_1, level_x, level_y), Rotation_Step(0)
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Act()
{
  // if (AsConfig::Current_Timer_Tick % 10 != 0)
		//return; // rotate the brick every 10 ticks (only for testing)

   if (Rotation_Step <= Max_Rotation_Step)
   {
      ++Rotation_Step;
      InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw(HDC hdc, RECT& paint_area)
{  
	int step;
   const int scale = AsConfig::Global_Scale;
   double rotation_angle, x_ratio;
   RECT zero_rect;
	XFORM xform, old_xform;

   //1. clear the background
   AsConfig::BG_Color.Select(hdc);
   AsConfig::Round_Rect(hdc, Brick_Rect);

   //2. set the transformation matrix
   step = Rotation_Step % Steps_Per_Turn;
	rotation_angle = M_PI_4 / 2.0 * (double)step;
	x_ratio = cos(rotation_angle);

   xform.eM11 = (float)x_ratio;
   xform.eM12 = 0.0f;
   xform.eM21 = 0.0f;
   xform.eM22 = 1.0f;
   xform.eDx = (float)Brick_Rect.left + (float)(1.0 - x_ratio) * (float)(AsConfig::Brick_Width * scale) / 2.0f;
   xform.eDy = (float)Brick_Rect.top;
   GetWorldTransform(hdc, &old_xform);
   SetWorldTransform(hdc, &xform);

   //3. draw the number "100"
   AsConfig::Letter_Color.Select_Pen(hdc);
	MoveToEx(hdc, 0 + 1 * scale + 1, 0 + 3 * scale, 0);
	LineTo(hdc, 0 + 3 * scale + 1, 0 + 1 * scale);
	LineTo(hdc, 0 + 3 * scale + 1, 0 + 6 * scale - 1);

	zero_rect.left = 0 + 5 * scale + 1;
	zero_rect.top = 0 + 1 * scale;
	zero_rect.right = zero_rect.left + 3 * scale + 1;
	zero_rect.bottom = zero_rect.top + 5 * scale;
   AsConfig::Round_Rect(hdc, zero_rect);

	zero_rect.left += 5 * scale;
   zero_rect.right += 5 * scale;
	AsConfig::Round_Rect(hdc, zero_rect); 
 
   SetWorldTransform(hdc, &old_xform);
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Multihit::Is_Finished()
{
   if (Rotation_Step >= Max_Rotation_Step)
      return true;
   else
      return false;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw_In_Level(HDC hdc, RECT& brick_rect, EBrick_Type brick_type)
{// drawer unbreakable brick
	const int scale = AsConfig::Global_Scale;

	// Draw the border
   AsConfig::White_Color.Select(hdc);
   AsConfig::Round_Rect(hdc, brick_rect);

	AsConfig::Purple_Color.Select(hdc);
   Rectangle(hdc, brick_rect.left + scale, brick_rect.top + scale, brick_rect.right - scale - 1, brick_rect.bottom - scale - 1);

	// Draw the stage
   switch (brick_type)
	{
   case EBT_Multihit_1:
      Draw_Stage(hdc, brick_rect, 2, 10);
      break;

	case EBT_Multihit_2:
      Draw_Stage(hdc, brick_rect, 2, 4);
      Draw_Stage(hdc, brick_rect, 8, 4);
		break;

   case EBT_Multihit_3:
      Draw_Stage(hdc, brick_rect, 2, 3);
      Draw_Stage(hdc, brick_rect, 6, 3);
		Draw_Stage(hdc, brick_rect, 10, 3);
		break;

   case EBT_Multihit_4:
      Draw_Stage(hdc, brick_rect, 2, 2);
      Draw_Stage(hdc, brick_rect, 5, 2);
		Draw_Stage(hdc, brick_rect, 8, 2);
		Draw_Stage(hdc, brick_rect, 11, 2);
		break;

   default:
		AsConfig::Throw();
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw_Stage(HDC hdc, RECT& brick_rect, int x, int width)
{ // drawer the stage of multihit brick
   const int scale = AsConfig::Global_Scale;
   RECT stage_rect;

   stage_rect.left = brick_rect.left + x * scale;
   stage_rect.top = brick_rect.top + 2 * scale;
   stage_rect.right = stage_rect.left + width * scale;
   stage_rect.bottom = stage_rect.top + 3 * scale;

   AsConfig::BG_Color.Select(hdc);
   Rectangle(hdc, stage_rect.left + scale, stage_rect.top + scale, stage_rect.right + scale - 1, stage_rect.bottom + scale - 1);

   AsConfig::Blue_Color.Select(hdc);
   Rectangle(hdc, stage_rect.left, stage_rect.top, stage_rect.right - 1, stage_rect.bottom - 1);
}
// --------------------------------------------------------------------------------------------------------------------------------------




//AActive_Brick_Teleport
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick_Teleport::~AActive_Brick_Teleport()
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick_Teleport::AActive_Brick_Teleport(int level_x, int level_y, ABall *ball, AActive_Brick_Teleport *destination_teleport)
	: AActive_Brick(EBT_Teleport, level_x, level_y), Teleport_Step(ETS_Starting), Animation_Step(0), Ball(0), Destination_Teleport(destination_teleport)
{
	Set_Ball(ball);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Act()
{
	double ball_x = 0, ball_y = 0;
   double direction;

   if (Animation_Step <= Max_Animation_Step)
   {
      ++Animation_Step;
      InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
   }
   else
   {
      switch(Teleport_Step)
		{
         case ETS_Starting:
				Animation_Step = 0;
            Teleport_Step = ETS_Finishing;

            if (Destination_Teleport != 0)
            {
					Destination_Teleport->Set_Ball(Ball);
               Ball = 0;
            }
            break;

			case ETS_Finishing:
				Teleport_Step = ETS_Done;

            if (Ball != 0)
            {
               switch(Release_Direction)
               {
					case EDT_Left:
                  ball_x = Get_Brick_X_Pos(false) - (double)ABall::Radius;
						ball_y = Get_Brick_Y_Pos(true);
                  break;

               case EDT_Up:
                  ball_x = Get_Brick_X_Pos(true);
                  ball_y = Get_Brick_Y_Pos(false) - (double)ABall::Radius;
                  break;

               case EDT_Right:
                  ball_x = Get_Brick_X_Pos(false) + (double)AsConfig::Brick_Width + (double)ABall::Radius;
                  ball_y = Get_Brick_Y_Pos(true);
                  break;

               case EDT_Down:
                  ball_x = Get_Brick_X_Pos(true);
                  ball_y = Get_Brick_Y_Pos(false) + (double)AsConfig::Brick_Height + (double)ABall::Radius;
                  break;

					default:
						AsConfig::Throw();
               }
					direction = Ball->Get_Direction();
					Ball->Set_State(EBS_Normal, ball_x, ball_y);
					Ball->Set_Direction(direction);

					Ball = 0; // the ball has been released
               InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
            }
            break;
      }
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Draw(HDC hdc, RECT& paint_area)
{
	int step;

   switch (Teleport_Step)
   {
   case ETS_Starting:
      step = Animation_Step;
		break;

   case ETS_Finishing:
	   step = Max_Animation_Step - Animation_Step;
      break;

	default:
      step = 0;
   }
   Draw_In_Level(hdc, Brick_Rect, step);

	if (Ball != 0)
      Ball->Draw_Teleporting(hdc, step);
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Teleport::Is_Finished()
{
	if (Teleport_Step == ETS_Done)
      return true;
   else
      return false;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Draw_In_Level(HDC hdc, RECT& brick_rect, int step)
{
   const int scale = AsConfig::Global_Scale;
   int top_y = brick_rect.top + step + 1;
	int low_y = brick_rect.top + 6 * scale - step + 1;

	// drawer teleport brick
	AsConfig::Purple_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, brick_rect);

	// Draw the teleport symbol
	AsConfig::Teleport_Portal_Color.Select(hdc);

   Ellipse(hdc, brick_rect.left + 3 * scale + 1, top_y, brick_rect.left + 11 * scale + 1, low_y);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Set_Ball(ABall* ball)
{
   double ball_x, ball_y;

   // set the ball to teleporting state ( in the middle of the brick )
   ball_x = Get_Brick_X_Pos(true);
   ball_y = Get_Brick_Y_Pos(true);

   if (ball != 0)
      ball->Set_State(EBS_Teleporting, ball_x, ball_y);

   Ball = ball;
}
// --------------------------------------------------------------------------------------------------------------------------------------




//AAdvertisement
// --------------------------------------------------------------------------------------------------------------------------------------
AAdvertisement::AAdvertisement(int level_x, int level_y, int width, int height)
: Level_X(level_x), Level_Y(level_y), Width(width), Height(height)
{
   Ad_Rect.left = (AsConfig::Level_X_Offset + Level_X * AsConfig::Cell_Width) * AsConfig::Global_Scale;
   Ad_Rect.top = (AsConfig::Level_Y_Offset + Level_Y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
   Ad_Rect.right = Ad_Rect.left + Width * AsConfig::Cell_Width * AsConfig::Global_Scale;
	Ad_Rect.bottom = Ad_Rect.top + Height * AsConfig::Cell_Height * AsConfig::Global_Scale;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AAdvertisement::Act()
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AAdvertisement::Clear(HDC hdc, RECT& paint_area)
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AAdvertisement::Draw(HDC hdc, RECT& paint_area)
{
	const int scale = AsConfig::Global_Scale;
   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Ad_Rect))
		return; // no intersection

   // draw table in the advertisement
   AsConfig::White_Color.Select(hdc);
   MoveToEx(hdc, Ad_Rect.left + 1, Ad_Rect.top + 15 * scale, 0);
   LineTo(hdc, Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 10 * scale);
   LineTo(hdc, Ad_Rect.left + 30 * scale + 1, Ad_Rect.top + 15 * scale);
   LineTo(hdc, Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 20 * scale);
   LineTo(hdc, Ad_Rect.left + 1, Ad_Rect.top + 15 * scale);

	FloodFill(hdc, Ad_Rect.left + 15 * scale, Ad_Rect.top + 15 * scale, AsConfig::White_Color.Get_RGB());

	// draw the ball shadow in the advertisement
   AsConfig::Blue_Color.Select(hdc);
	Ellipse(hdc, Ad_Rect.left + 11 * scale, Ad_Rect.top + 14 * scale, Ad_Rect.left + 20 * scale - 1, Ad_Rect.top + 18 * scale - 1);


	// draw the blue outline in the advertisement
   AsConfig::Advertisement_Blue.Select(hdc);
   MoveToEx(hdc, Ad_Rect.left + 1, Ad_Rect.top + 15 * scale, 0);
   LineTo(hdc, Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 10 * scale);
   LineTo(hdc, Ad_Rect.left + 30 * scale + 1, Ad_Rect.top + 15 * scale);
   LineTo(hdc, Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 20 * scale);
   LineTo(hdc, Ad_Rect.left + 1, Ad_Rect.top + 15 * scale);


	// draw the purple outline in the advertisement
   AsConfig::Advertisement_Purple.Select(hdc);
   MoveToEx(hdc, Ad_Rect.left + scale - 1, Ad_Rect.top + 16 * scale, 0);
   LineTo(hdc, Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 21 * scale);
   LineTo(hdc, Ad_Rect.left + 30 * scale, Ad_Rect.top + 16 * scale);

	// draw the ball symbol in the advertisement
	AsConfig::Purple_Color.Select(hdc);
	Ellipse(hdc, Ad_Rect.left + 9 * scale + 1, Ad_Rect.top + 2 * scale, Ad_Rect.left + 21 * scale + 1, Ad_Rect.top + 14 * scale);

}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AAdvertisement::Is_Finished()
{
	return false; // advertisement is never finished ;)
}
// --------------------------------------------------------------------------------------------------------------------------------------




//AActive_Brick_Ad
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick_Ad::~AActive_Brick_Ad()
{
   /*DeleteObject(Region);*/
}
// --------------------------------------------------------------------------------------------------------------------------------------
AActive_Brick_Ad::AActive_Brick_Ad(int level_x, int level_y)
   : AActive_Brick(EBT_Ad, level_x, level_y) /*Animation_Step(0), Region(0)*/
{
   //Region = CreateRoundRectRgn(Brick_Rect.left, Brick_Rect.top, Brick_Rect.right + 1, Brick_Rect.bottom + 1, 2 * AsConfig::Global_Scale - 1, 2 * AsConfig::Global_Scale - 1);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Ad::Act()
{
   //if (Animation_Step <= Max_Animation_Step)
   //{
   //   ++Animation_Step;
      InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
   //}
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Ad::Draw(HDC hdc, RECT& paint_area)
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Ad::Is_Finished()
{
   //if (Animation_Step >= Max_Animation_Step)
   //   return true; // the brick is finished
   //else
      return false; // the brick is not finished
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AActive_Brick_Ad::Draw_In_Level(HDC hdc, RECT& brick_rect)
{
   int i;
   const int scale = AsConfig::Global_Scale;
   int x = brick_rect.left;
	int y = brick_rect.top;
   int size = (Circle_Size - 1) * scale - 1;


   for (i = 0; i < 2; i++)
   {
      AsConfig::Purple_Color.Select(hdc);
      Ellipse(hdc, x, y, x + 7 * scale, brick_rect.bottom);

	   // drow highlight in the ad brick
      AsConfig::White_Color.Select(hdc);
      Arc(hdc, x + scale, y + scale, x + size, y + size, x + 2 * scale, y + scale, x + scale, y + 3 * scale);
   
		x += 8 * scale;
   }
}
// --------------------------------------------------------------------------------------------------------------------------------------
