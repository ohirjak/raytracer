/*
 * @file: colors.h
 * @author: Ondrej Hirjak, 2010
 * @description: Defines color structures.
 */

#ifndef colors_h
#define colors_h


struct Color
{
	public:
		double r, g, b, a;

		Color() : r(0.0), g(0.0), b(0.0), a(0.0) { }
		Color(const Color &c) : r(c.r), g(c.g), b(c.b), a(c.a) { }
		Color(double c1, double c2, double c3, double c4) : r(c1), g(c2), b(c3), a(c4) { }

		Color Cap();

		Color operator +(Color c);
		Color operator *(Color c);

		Color operator *(double t);
};


union ColorRGBA
{
	unsigned char color[4];

	struct {
		unsigned char R;
		unsigned char G;
		unsigned char B;
		unsigned char A;
	} x;
};


union ColorBGRA
{
	unsigned char color[4];

	struct {
		unsigned char B;
		unsigned char G;
		unsigned char R;
		unsigned char A;
	} x;
};


#endif // colors_h

