/*
 * @file: main.cpp
 * @author: Ondrej Hirjak, 2009
 * @description: 
 */

#include <iostream>
#include "geometries.h"
#include "scene.h"

using namespace std;


void initScene(Scene *s)
{
	// TODO: Load scene conf. from file
	// TODO: Create test file Cornell Box and get Stanford Bunny
	Sphere *sphere;

	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
	{
		//s->AddObject(sphere = new Sphere(Point3D(-1.0 + i * 1.0, -1.0 + j * 1.0, -5.0), Color(i * 0.5, 1.0 - j * 0.5, 1.0 - (i + j) * 0.25, 0.0), 0.2));
		s->AddObject(sphere = new Sphere(Point3D(-1.0 + i * 1.0, -1.0 + j * 1.0, -5.0 + (i - j) * 1.0), Color(i * 0.5, 1.0 - j * 0.5, 1.0 - (i + j) * 0.25, 0.0), 0.3));
		if (i == 1 || j == 1)
			sphere->SetReflection(0.5);
	}

	s->AddLight(new Light(Point3D(-0.5, 2.0, 0.0), Color(1.0, 0.0, 0.5, 0.0)));
	s->AddLight(new Light(Point3D(2.0, -1.0, -2.0), Color(0.0, 0.6, 0.0, 0.0)));
}


int main()
{
	Scene *scene = new Scene();

	initScene(scene);

	scene->Render(20.0, 1.2, 1280, 720);

	delete scene;

	return 0;
}

