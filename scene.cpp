/*
 * @file: scene.cpp
 * @author: Ondrej Hirjak, 2009
 * @description: 
 */

#include "main.h"
#include "colors.h"
#include "geometries.h"
#include "scene.h"


// Maximum recursion depth can be 255
const int MAX_RECURSION_DEPTH = 10;


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


GeometryObject* Scene::GetIntersectObject(Ray &ray, IntersectData *intersectData)
{
	GeometryObject *object = NULL; // get closest intersecting object with ray
	double t = INFINITY;

	for (list<GeometryObject *>::const_iterator it = objects.begin(); it != objects.end(); ++it)
	{
		double tObject = (*it)->RayIntersect(ray, intersectData);

		if (tObject != INFINITY)
		{
			if ((t != INFINITY && tObject < t) || t == INFINITY)
			{
				object = *it;
				t = tObject;
			}
		}
	}

	return object;
}


Color Scene::InnerTraceRay(Ray &ray, int &rayRecursionDepth, int recursionDepth)
{
	IntersectData intersectData;
	rayRecursionDepth = recursionDepth;

	// FIXME: Add profiling
	// Hit recursion depth
	if (recursionDepth > MAX_RECURSION_DEPTH)
	{
		printf("Hit recursion depth %d\n", MAX_RECURSION_DEPTH);

		// Return backgound color
		return Color(0.0, 0.0, 0.0, 0.0);
	}

	GeometryObject *object = GetIntersectObject(ray, &intersectData); // get closest intersecting object with ray

	// Nothing hit
	if (object == NULL)
	{
		// Return backgound color
		return Color(0.0, 0.0, 0.0, 0.0);
	}

	rayRecursionDepth = recursionDepth + 1;

	Color pointColor; // Get objects color at intersect
	Color reflectColor; // get reflected ray color
	bool reflection = false;
//	Color refractColor; // get refracted ray color
	Color resultColor; // Result color
	Point3D point; // Get intersection point
	Vector3D pointNormal; // Get objects normal at intersect

	point = intersectData.contact;
	pointNormal = intersectData.normal;
	pointColor = intersectData.color;

	if (object->GetReflection() > 0.0)
	{
		double c1 = -Vector3D::DotProduct(pointNormal, ray.Vector());
		Vector3D Rl = ray.Vector() + (pointNormal * 2 * c1);

		// Prevents auto-reflection with a offset 0.001
		Ray reflectedRay = Ray((Vector3D)point + Rl * 0.001, Rl);

		reflectColor = InnerTraceRay(reflectedRay, rayRecursionDepth, recursionDepth + 1);

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

		if (GetIntersectObject(toLightRay, NULL) != NULL)
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


Color Scene::TraceRay(Ray &ray, int &rayRecursionDepth)
{
	return InnerTraceRay(ray, rayRecursionDepth);
}

