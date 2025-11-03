#include "Config.h"


//AColor
// --------------------------------------------------------------------------------------------------------------------------------------
AColor::AColor()
   : R(0), G(0), B(0), Pen(0), Brush(0)
{
}
// --------------------------------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
: R(r), G(g), B(b), Pen(0), Brush(0)
{
   Pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   Brush = CreateSolidBrush(RGB(r, g, b));
}
// --------------------------------------------------------------------------------------------------------------------------------------
AColor::AColor(const AColor &color, int pen_size)
: R(color.R), G(color.G), B(color.B), Pen(0), Brush(0)
{
   Pen = CreatePen(PS_SOLID, pen_size, color.Get_RGB() );
}
// --------------------------------------------------------------------------------------------------------------------------------------
AColor::AColor(const AColor &pen_color, const AColor& brush_color, int pen_size)
: R(0), G(0), B(0), Pen(0), Brush(0)
{
   Pen = CreatePen(PS_SOLID, pen_size, pen_color.Get_RGB());
	Brush = CreateSolidBrush(brush_color.Get_RGB());
}
// --------------------------------------------------------------------------------------------------------------------------------------
int AColor::Get_RGB() const
{
   return RGB(R, G, B);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AColor::Select(HDC hdc) const
{
   SelectObject(hdc, Pen);
   SelectObject(hdc, Brush);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AColor::Select_Pen(HDC hdc) const
{
   SelectObject(hdc, Pen);
}
// --------------------------------------------------------------------------------------------------------------------------------------
HBRUSH AColor::Get_Brush() const
{
	return Brush;
}
// --------------------------------------------------------------------------------------------------------------------------------------




//AsConfig
bool AsConfig::Level_Has_Floor = false; // indicates if the level has a floor (for the ball to bounce off)
int AsConfig::Current_Timer_Tick = 0;

const AColor AsConfig::BG_Color(15, 73, 31);
const AColor AsConfig::Purple_Color(255, 85, 255);
const AColor AsConfig::Blue_Color(85, 255, 255);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Letter_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Unbreakable_Blue_Highlight(AsConfig::Blue_Color, AsConfig::Global_Scale);
const AColor AsConfig::Unbreakable_Purple_Highlight(AsConfig::Purple_Color, 3 * AsConfig::Global_Scale);
const AColor AsConfig::Teleport_Portal_Color(AsConfig::Blue_Color, AsConfig::BG_Color, AsConfig::Global_Scale);

HWND AsConfig::Hwnd;
const double AsConfig::Moving_Step_Size = 1.0 / AsConfig::Global_Scale;
// --------------------------------------------------------------------------------------------------------------------------------------
int AsConfig::Rand(int range)
{
	// Generate a random number in the range [0, ... , range - 1]
   return rand() * range / RAND_MAX;
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsConfig::Round_Rect(HDC hdc, RECT& rect, int corner_radius)
{
	int radius = corner_radius * AsConfig::Global_Scale;
   RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, radius, radius);
}
// --------------------------------------------------------------------------------------------------------------------------------------
void AsConfig::Throw()
{
   throw 13;
}
// --------------------------------------------------------------------------------------------------------------------------------------
