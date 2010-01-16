/*
 * @file: scene.cpp
 * @author: Ondrej Hirjak, 2009
 * @description: 
 */

#include <iostream>
#include <math.h>
#include <stdio.h>
#include "geometries.h"
#include "scene.h"

using namespace std;


void Scene::ClearScene()
{
	for (list<Light *>::const_iterator it = lights.begin(); it != lights.end(); ++it)
		delete *it;

	lights.clear();

	for (list<GeometryObject *>::const_iterator it = objects.begin(); it != objects.end(); ++it)
		delete *it;

	objects.clear();
}


void Scene::AddLight(Light *light)
{
	lights.push_back(light);
}


void Scene::AddObject(GeometryObject *object)
{
	objects.push_back(object);
}


GeometryObject* Scene::GetIntersectObject(Ray &ray)
{
	GeometryObject *object = NULL; // get closest intersecting object with ray
	double t = INFINITY;

	for (list<GeometryObject *>::const_iterator it = objects.begin(); it != objects.end(); ++it)
	{
		double tObject = (*it)->RayIntersect(ray);

		if (tObject != INFINITY)
		{
			if (t != INFINITY && tObject < t || t == INFINITY)
			{
				object = *it;
				t = tObject;
			}
		}
	}

	return object;
}


Color Scene::TraceRay(Ray &ray, int recursionDepth)
{
	// Hit recursion depth
	if (recursionDepth > 10)
	{
		cout << "Hit recursion depth 10" << endl;
		// Return backgound color
		return Color(0.0, 0.0, 0.0, 0.0);
	}

	GeometryObject *object = GetIntersectObject(ray); // get closest intersecting object with ray

	// Nothing hit
	if (object == NULL)
	{
		// Return backgound color
		return Color(0.0, 0.0, 0.0, 0.0);
	}

	Color pointColor; // Get objects color at intersect
	Color reflectColor; // get reflected ray color
	bool reflection = false;
//	Color refractColor; // get refracted ray color
	Color resultColor; // Result color
	Point3D point; // Get intersection point
	Vector3D pointNormal; // Get objects normal at intersect

	object->GetPointStats(point, pointNormal, pointColor);

	if (object->GetReflection() > 0.0)
	{
		double c1 = -Vector3D::DotProduct(pointNormal, ray.Vector());
		Vector3D Rl = ray.Vector() + (pointNormal * 2 * c1);

		Ray reflectedRay = Ray((Vector3D)point + Rl * 0.01, Rl);

		reflectColor = TraceRay(reflectedRay, recursionDepth + 1);

		reflection = true;
	}

	//cout << "contact at " << point.x << ":" << point.y << ":" << point.z << endl;
	//cout << "normal " << pointNormal.x << ":" << pointNormal.y << ":" << pointNormal.z << endl;
	//cout << "pointColor " << pointColor.r << "," << pointColor.g << "," << pointColor.b << endl;

	for (list<Light *>::const_iterator it = lights.begin(); it != lights.end(); ++it)
	{
		Vector3D lightNormal;
		double dp;

		lightNormal = (Vector3D)((*it)->p) - point;
		lightNormal = lightNormal.Normalize();

		// Check for obstruction
		Ray toLightRay = Ray((Vector3D)point + lightNormal * 0.01, lightNormal);

		if (GetIntersectObject(toLightRay) != NULL)
			dp = 0.0;
		else
		{
			dp = Vector3D::DotProduct(lightNormal, pointNormal);

			if (dp < 0.0)
				dp = 0.0;
		}

		// Combine colors
		resultColor = resultColor + pointColor * (0.05 + 0.95 * dp) * (*it)->c + (*it)->c * object->GetReflection() * pow(dp, 3.0);
	}

	if (reflection)
	{
		resultColor = resultColor + reflectColor * object->GetReflection();
	}

	//cout << "resultColor " << resultColor.r << "," << resultColor.g << "," << resultColor.b << endl;

	return resultColor;
}


struct BMP_header
{
	char magic[2];	
	int size;
	short reserved1;
	short reserved2;
	int offset;

	int headerSize;
	int width;
	int height;
	short planes;
	short bpp;
	int compression;
	int imageSize;
	int hImageRes;
	int vImageRes;
	int colors;
	int iColors;
};

void Scene::Render(double viewDist, double viewSize, long hRes, long vRes)
{
	// Renderer uses OpenGL right-handed coords
	// Outputs to BMP file output.bmp

	// Exchanged top & bottom because of BMP bitmap layout
	double aspectRatio = (double)hRes / vRes;
	Point3D eye = Point3D(0.0, 0.0, viewDist);
	Point3D leftTop = Point3D(-viewSize * aspectRatio, -viewSize, 0.0);
	Point3D rightBottom = Point3D(viewSize * aspectRatio, viewSize, 0.0);

	Vector3D d = (Vector3D)rightBottom - leftTop;

	BMP_header bmpHead;
	bmpHead.magic[0] = 'B';
	bmpHead.magic[1] = 'M';
	bmpHead.size = 54 + hRes * vRes * 4;
	bmpHead.reserved1 = 0;
	bmpHead.reserved2 = 0;
	bmpHead.offset = 54;
	bmpHead.headerSize = 40;
	bmpHead.width = hRes;
	bmpHead.height = vRes;
	bmpHead.planes = 1;
	bmpHead.bpp = 32;
	bmpHead.compression = 0;
	bmpHead.imageSize = hRes * vRes * 4;
	bmpHead.hImageRes = 2835;
	bmpHead.vImageRes = 2835;
	bmpHead.colors = 0;
	bmpHead.iColors = 0;

	FILE *bmpOut = fopen("output.bmp", "w");

	fwrite(&bmpHead.magic, 2, 1, bmpOut);
	fwrite(&bmpHead.size, 4, 1, bmpOut);
	fwrite(&bmpHead.reserved1, 2, 1, bmpOut);
	fwrite(&bmpHead.reserved2, 2, 1, bmpOut);
	fwrite(&bmpHead.offset, 4, 1, bmpOut);

	fwrite(&bmpHead.headerSize, 4, 1, bmpOut);
	fwrite(&bmpHead.width, 4, 1, bmpOut);
	fwrite(&bmpHead.height, 4, 1, bmpOut);
	fwrite(&bmpHead.planes, 2, 1, bmpOut);
	fwrite(&bmpHead.bpp, 2, 1, bmpOut);
	fwrite(&bmpHead.compression, 4, 1, bmpOut);
	fwrite(&bmpHead.imageSize, 4, 1, bmpOut);
	fwrite(&bmpHead.hImageRes, 4, 1, bmpOut);
	fwrite(&bmpHead.vImageRes, 4, 1, bmpOut);
	fwrite(&bmpHead.colors, 4, 1, bmpOut);
	fwrite(&bmpHead.iColors, 4, 1, bmpOut);

	// Y
	for (long iterY = 0; iterY < vRes; iterY++)
	{
		// X
		for (long iterX = 0; iterX < hRes; iterX++)
		{
			Point3D p = (Vector3D)leftTop + Point3D(d.x * iterX / hRes, d.y * iterY / vRes, 0.0);
			//cout << "View point " << p.x << ":" << p.y << ":" << p.z << endl;

			Ray ray = Ray(eye, p);

			Color color = TraceRay(ray);
			color = color.Cap();
			//cout << "Color " << color.r << "," << color.g << "," << color.b << endl;

			unsigned char outColor[4];

			outColor[0] = color.b * 255;
			outColor[1] = color.g * 255;
			outColor[2] = color.r * 255;
			outColor[3] = color.a * 255;

			fwrite(outColor, 4, 1, bmpOut);
		}
	}

	fclose(bmpOut);
}

