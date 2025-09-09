#include "Config.h"


//AColor
// --------------------------------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
: R(r), G(g), B(b)
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
int AColor::Get_RGB() const
{
   return RGB(R, G, B);
}
// --------------------------------------------------------------------------------------------------------------------------------------



//AsConfig
bool AsConfig::Level_Has_Floor = false; // indicates if the level has a floor (for the ball to bounce off)
int AsConfig::Current_Timer_Tick = 0;

const AColor AsConfig::Red_Brick_Color (255, 85, 255);
const AColor AsConfig::Blue_Brick_Color(85, 255, 255);
const AColor AsConfig::BG_Color(15, 73, 31);

HPEN AsConfig::BG_Pen;
HBRUSH AsConfig::BG_Brush;
HWND AsConfig::Hwnd;
HPEN  AsConfig::Letter_Pen, AsConfig::Brick_Red_Pen, AsConfig::Brick_Blue_Pen;
HBRUSH AsConfig::Brick_Red_Brush, AsConfig::Brick_Blue_Brush;

const double AsConfig::Moving_Step_Size = 1.0 / AsConfig::Global_Scale;

// --------------------------------------------------------------------------------------------------------------------------------------
void AsConfig::Setup_Colors()
{
   AsConfig::Letter_Pen = CreatePen(PS_SOLID, AsConfig::Global_Scale, RGB(255, 255, 255));

   AsConfig::Create_Pen_Brush(AsConfig::BG_Color, BG_Pen, BG_Brush);
   AsConfig::Create_Pen_Brush(AsConfig::Red_Brick_Color, AsConfig::Brick_Red_Pen, AsConfig::Brick_Red_Brush);
   AsConfig::Create_Pen_Brush(AsConfig::Blue_Brick_Color, AsConfig::Brick_Blue_Pen, AsConfig::Brick_Blue_Brush);

}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(const AColor &color, HPEN& pen, HBRUSH& brush)
{

   pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
   brush = CreateSolidBrush(RGB(color.R, color.G, color.B));

};
// --------------------------------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{

   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));

};
// --------------------------------------------------------------------------------------------------------------------------------------
int AsConfig::Rand(int range)
{
	// Generate a random number in the range [0, ... , range - 1]
   return rand() * range / RAND_MAX;
}
// --------------------------------------------------------------------------------------------------------------------------------------
