/*
 * @file: geometries.cpp
 * @author: Ondrej Hirjak, 2009
 * @description: 
 */

#include <iostream>
#include <math.h>
#include "geometries.h"

using namespace std;


Vector3D Vector3D::Normalize() const
{
	Vector3D vec;
	double length = x * x + y * y + z * z;

	length = sqrt(length);

	vec.x = x / length;
	vec.y = y / length;
	vec.z = z / length;

	return vec;
}


Vector3D Vector3D::operator +(Vector3D p)
{
	return Vector3D(x + p.x, y + p.y, z + p.z);
}


Vector3D Vector3D::operator -(Vector3D p)
{
	return Vector3D(x - p.x, y - p.y, z - p.z);
}


Vector3D Vector3D::operator *(double t)
{
	return Vector3D(x * t, y * t, z * t);
}


// TODO: Move non-geometry stuff out
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


double Sphere::RayIntersect(Ray &ray)
{
	double A = 1.0; // normalized ray vector
	double B = 2.0 * (ray.Vector().x * (ray.Point().x - center.x) + ray.Vector().y * (ray.Point().y - center.y) + ray.Vector().z * (ray.Point().z - center.z));
	double C = pow(ray.Point().x - center.x, 2.0) + pow(ray.Point().y - center.y, 2.0) + pow(ray.Point().z - center.z, 2.0) - radius * radius;
	double Det = B * B - 4.0 * A * C;

	if (Det >= 0.0)
	{
		double t = (-B - sqrt(Det)) / 2;

		if (t > 0.0)
		{
			contact = (Vector3D)ray.Point() + ray.Vector() * t;
			normal = (Vector3D)contact - center;
			normal = normal.Normalize();

			return t;
		}

		t = (-B + sqrt(Det)) / 2;

		if (t > 0.0)
		{
			contact = (Vector3D)ray.Point() + ray.Vector() * t;
			normal = (Vector3D)contact - center;
			normal = normal.Normalize();

			return t;
		}
	}

	return INFINITY;
}


void Sphere::GetPointStats(Point3D &pPoint, Vector3D &pNormal, Color &pColor)
{
	pPoint = contact;
	pNormal = normal;
	pColor = color;
}

