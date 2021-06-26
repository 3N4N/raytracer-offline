#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>

class Color {
public:
    double r,g,b;
    Color() : r(0), g(0), b(0) { }
    Color(double r, double g, double b) : r(r), g(g), b(b) { }

    void clip();

    Color operator+(const Color &o) const;
    Color operator-(const Color &o) const;
    Color operator*(const Color &o) const;
    Color operator/(const Color &o) const;

    Color operator*(double m) const;
    Color operator/(double d) const;

    Color& operator+=(const Color &o);
    Color& operator-=(const Color &o);
    Color& operator*=(double m);
    Color& operator/=(double d);

    friend std::ostream &operator<<(std::ostream &os, const Color &c);
    friend std::istream &operator>>(std::istream &is, Color &c);
};


#endif // _COLOR_H_
