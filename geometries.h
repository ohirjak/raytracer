/*
 * @file: geometries.h
 * @author: Ondrej Hirjak, 2009
 * @description: 
 */

#ifndef geometries_h
#define geometries_h


class Vector3D;

class Point3D
{
	public:
		double x, y, z;

		Point3D() : x(0.0), y(0.0), z(0.0) { }
		Point3D(const Point3D &p) : x(p.x), y(p.y), z(p.z) { }
		Point3D(double d1, double d2, double d3) : x(d1), y(d2), z(d3) { }
};


class Vector3D : public Point3D
{
	public:
		Vector3D() : Point3D() { }
		Vector3D(const Point3D &p) : Point3D(p) { }
		Vector3D(double d1, double d2, double d3) : Point3D(d1, d2, d3) { }

		Vector3D Normalize() const;

		static double DotProduct(Vector3D v1, Vector3D v2)
		{
			return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		}

		Vector3D operator +(Vector3D p);
		Vector3D operator -(Vector3D p);

		Vector3D operator *(double t);
};


class Ray
{
	private:
		Point3D p;
		Vector3D v;

	public:
		Ray(Point3D p1, Point3D p2)
		{
			p = p1;
			v = (Vector3D)p2 - p1;
			v = v.Normalize();
		}

		Ray(Point3D p1, Vector3D v1)
		{
			p = p1;
			v = v1.Normalize();
		}

		Point3D& Point()
		{
			return p;
		}

		Vector3D& Vector()
		{
			return v;
		}
};


class Color
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


class GeometryObject
{
	protected:
		Point3D center;
		Color color;
		double reflection;
		double refraction;

	public:
		GeometryObject(Point3D p, Color c) : center(p), color(c), reflection(0.0), refraction(0.0) { }

		void SetReflection(double r)
		{
			reflection = r;
		}

		double GetReflection()
		{
			return reflection;
		}

		void SetRefraction(double r)
		{
			refraction = r;
		}

		double GetRefraction()
		{
			return refraction;
		}

		virtual double RayIntersect(Ray &ray) = 0;
		virtual void GetPointStats(Point3D &pPoint, Vector3D &pNormal, Color &pColor) = 0;
};


class Sphere : public GeometryObject
{
	private:
		double radius;

		// Last intersection data
		Point3D contact;
		Vector3D normal;

	public:
		Sphere(Point3D p, Color c, double r) : GeometryObject(p, c), radius(r) { }

		double RayIntersect(Ray &ray);
		void GetPointStats(Point3D &pPoint, Vector3D &pNormal, Color &pColor);
};


#endif // geometries_h

