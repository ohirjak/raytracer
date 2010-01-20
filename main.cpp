/*
 * @file: main.cpp
 * @author: Ondrej Hirjak, 2009-2010
 * @description: 
 */

#include "main.h"
#include "colors.h"
#include "geometries.h"
#include "scene.h"
#include "render.h"
#include "renderMT.h"


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


timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}


int main()
{
	Scene *scene = new Scene();

	initScene(scene);

	const int sizeX = 1280, sizeY = 720;

	printf("Render size = %d:%d\n", sizeX, sizeY);

	Render *render = new Render(sizeX, sizeY);

	timespec ts1, ts2, res;

	clock_gettime(CLOCK_MONOTONIC, &ts1);
	render->RenderScene(scene, 20.0, 1.2);
	clock_gettime(CLOCK_MONOTONIC, &ts2);

	res = diff(ts1, ts2);

	printf("Run time in ms %ld%03ld.%ld\n", (long)res.tv_sec, res.tv_nsec / 1000000, res.tv_nsec % 1000000);

	delete render;

	render = new RenderMT(sizeX, sizeY);

	clock_gettime(CLOCK_MONOTONIC, &ts1);
	render->RenderScene(scene, 20.0, 1.2);
	clock_gettime(CLOCK_MONOTONIC, &ts2);

	res = diff(ts1, ts2);

	printf("Run time (MT) in ms %ld%03ld.%ld\n", (long)res.tv_sec, res.tv_nsec / 1000000, res.tv_nsec % 1000000);

	render->OutputBuffers("output.bmp", "debug.bmp");

	delete render;
	delete scene;

	return 0;
}

