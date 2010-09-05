/*
 * @file: main.cpp
 * @author: Ondrej Hirjak, 2009-2010
 * @description: 
 */

#include "V2Base.h"
#include "colors.h"
#include "geometries.h"
#include "scene.h"
#include "render.h"
#include "renderMT.h"


struct VideoMode
{
	char short_name[6];
	char name[14];
	int sizeW;
	int sizeH;
};


const struct VideoMode videoModes[] =
{
/*	{ "180", "320x180",        320, 180 },
	{ "360", "640x360",        640, 360 },*/
	{ "576p2", "704x576",      704, 576 },
	{ "720p",  "1280x720 HD",  1280, 720 },
	{ "1080i", "1440x1080 HD", 1440, 1080 },
	{ "1080p", "1920x1080 HD", 1920, 1080 },
	{ "2K",    "2048x1536",    2048, 1536 },
	{ "2160p", "3840x2160",    3840, 2160 },
	{ "4K",    "4096x3072",    4096, 3072 },
	{ "2540p", "4520x2540",    4520, 2540 },
	{ "4320p", "7680x4320",    7680, 4320 }
};


const int modeCount = 9;


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

	s->AddLight(new Light(Point3D(-1.0, 0.0, -2.0), Color(0.3, 0.2, 0.1, 0.0)));
	s->AddLight(new Light(Point3D(2.0, -1.0, -4.0), Color(1.0, 1.0, 1.0, 0.0)));
//	s->AddLight(new Light(Point3D(-0.5, 2.0, 0.0), Color(1.0, 0.0, 0.5, 0.0)));
//	s->AddLight(new Light(Point3D(2.0, -1.0, -2.0), Color(0.0, 0.6, 0.0, 0.0)));
}


int main()
{
	printf("%s by %s\nUsing %s\n", PROGRAM_NAME, PROGRAM_AUTHOR, PROGRAM_ENGINE);

	Scene *scene = new Scene();

	initScene(scene);

	const int videoMode = 0;

	printf("Render size = %dx%d\n", videoModes[videoMode].sizeW, videoModes[videoMode].sizeH);

	Render *render = new Render(videoModes[videoMode].sizeW, videoModes[videoMode].sizeH);

	float time1, time2, res;

	time1 = V2::GetTime();
	render->RenderScene(scene, 5.0, 1.2);
	time2 = V2::GetTime();

	res = time2 - time1;

	printf("Run time in ms %3.3f\n", res);

	delete render;

	render = new RenderMT(videoModes[videoMode].sizeW, videoModes[videoMode].sizeH);

	time1 = V2::GetTime();
	render->RenderScene(scene, 5.0, 0.9);
	time2 = V2::GetTime();

	res = time2 - time1;

	printf("Run time (MT) in ms %3.3f\n", res);

	render->OutputBuffers("output.bmp", "debug.bmp");

	delete render;
	delete scene;

	printf("GOAL: < 33.33ms for minimum performance, < 16.66ms for optimal performance, < 8.33ms for 3D experience.\n");
	system("pause");

	return 0;
}

