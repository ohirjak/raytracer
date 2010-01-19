/*
 * @file: colors.cpp
 * @author: Ondrej Hirjak, 2009
 * @description: Implements colors functions.
 */

#include "colors.h"


Color Color::Cap()
{
	Color c = *this;

	if (c.r > 1.0)
		c.r = 1.0;
	if (c.r < 0.0)
		c.r = 0.0;

	if (c.g > 1.0)
		c.g = 1.0;
	if (c.g < 0.0)
		c.g = 0.0;

	if (c.b > 1.0)
		c.b = 1.0;
	if (c.b < 0.0)
		c.b = 0.0;

	if (c.a > 1.0)
		c.a = 1.0;
	if (c.a < 0.0)
		c.a = 0.0;

	return c;
}


Color Color::operator +(Color c)
{
	return Color(r + c.r, g + c.g, b + c.b, a + c.a);
}


Color Color::operator *(Color c)
{
	return Color(r * c.r, g * c.g, b * c.b, a * c.a);
}


Color Color::operator *(double t)
{
	return Color(r * t, g * t, b * t, a * t);
}

