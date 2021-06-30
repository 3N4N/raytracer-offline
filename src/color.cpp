#include "color.h"

void Color::clip()
{
    if (r < 0) r = 0;
    else if (r > 1) r = 1;

    if (g < 0) g = 0;
    else if (g > 1) g = 1;

    if (b < 0) b = 0;
    else if (b > 1) b = 1;
}

Color Color::operator+(const Color &o) const
{
    return Color(r+o.r, g+o.g, b+o.b);
}

Color Color::operator-(const Color &o) const
{
    return Color(r-o.r, g-o.g, b-o.b);
}

Color Color::operator*(const Color &o) const
{
    return Color(r*o.r, g*o.g, b*o.b);
}

Color Color::operator/(const Color &o) const
{
    return Color(r/o.r, g/o.g, b/o.b);
}

Color Color::operator*(double m) const
{
    return Color(r*m, g*m, b*m);
}

Color Color::operator/(double d) const
{
    return Color(r/d, g/d, b/d);
}

Color& Color::operator+=(const Color &o)
{
    r += o.r;
    g += o.g;
    b += o.b;
    return *this;
}

Color& Color::operator-=(const Color &o)
{
    r -= o.r;
    g -= o.g;
    b -= o.b;
    return *this;
}

Color& Color::operator*=(double m)
{
    r *= m;
    g *= m;
    b *= m;
    return *this;
}

Color& Color::operator/=(double d)
{
    r /= d;
    g /= d;
    b /= d;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Color &c)
{
    os << "(" << c.r << ", " << c.g << ", " << c.b << ")";
    return os;
}

std::istream &operator>>(std::istream &is, Color &c)
{
    is >> c.r >> c.g >> c.b;
    return is;
}
