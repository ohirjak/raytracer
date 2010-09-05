/*
 * @file: renderMT.cpp
 * @author: Ondrej Hirjak, 2010
 * @description: Multi-threaded renderer.
 */

#include "V2Base.h"
#include "colors.h"
#include "geometries.h"
#include "scene.h"
#include "render.h"
#include "renderMT.h"


RenderMT::RenderMT(long init_hRes, long init_vRes) : Render(init_hRes, init_vRes)
{
}


RenderMT::~RenderMT()
{
}


void RenderMT::RenderThread(const V2::Thread *thread, void *attr)
{
	RenderData *rd = (RenderData*)attr;

	// Set thread affinity
	thread->SetThreadAffinity(rd->cpu);

	// Perform rendering
	double aspectRatio = (double)rd->render->hRes / rd->render->vRes;
	Point3D eye = Point3D(0.0, 0.0, rd->viewDist);
	Point3D leftTop = Point3D(-rd->viewSize * aspectRatio, rd->viewSize, 0.0);
	Point3D rightBottom = Point3D(rd->viewSize * aspectRatio, -rd->viewSize, 0.0);

	Vector3D d = (Vector3D)rightBottom - leftTop;

	ColorBGRA *rgba_buffer = (ColorBGRA*) rd->render->color_buffer;

	// FIXME: For better performance implement load-balancing

	float time1, time2, res;
	time1 = V2::GetTime();

	// Y
	for (long iterY = rd->part1; iterY < rd->part2; iterY++)
	{
		// X
		for (long iterX = 0; iterX < rd->render->hRes; iterX++)
		{
			Point3D p = (Vector3D)leftTop + Point3D(d.x * iterX / rd->render->hRes, d.y * iterY / rd->render->vRes, 0.0);
			//cout << "View point " << p.x << ":" << p.y << ":" << p.z << endl;

			Ray ray = Ray(eye, p);

			int recDepth;
			Color color = rd->scene->TraceRay(ray, recDepth);
			color = color.Cap();
			//cout << "Color " << color.r << "," << color.g << "," << color.b << endl;

			// write color
			rgba_buffer[iterY * rd->render->hRes + iterX].x.R = (unsigned char)(color.r * 255);
			rgba_buffer[iterY * rd->render->hRes + iterX].x.G = (unsigned char)(color.g * 255);
			rgba_buffer[iterY * rd->render->hRes + iterX].x.B = (unsigned char)(color.b * 255);
			rgba_buffer[iterY * rd->render->hRes + iterX].x.A = (unsigned char)(color.a * 255);

			rd->render->recursion_buffer[iterY * rd->render->hRes + iterX] = recDepth;
		}
	}

	time2 = V2::GetTime();

	res = time2 - time1;

	printf("Render time (MT), cpu %d in ms %3.3f\n", rd->cpu, res);
}


void RenderMT::RenderScene(Scene *scene, double viewDist, double viewSize)
{
	// Multi-threaded renderer
	// Renderer uses OpenGL right-handed coords

	// Get CPU count and create one thread for each of them
	int thread_count = V2::Thread::GetProcessorNumber();

	// Error occured, set only 1 thread
	if (thread_count == -1)
	{
		fprintf(stderr, "Cannot get CPU count, defaulting to 1.\n");
		thread_count = 1;
	}

	int i;
	RenderData *rd = new RenderData[thread_count];

	for (i = 0; i < thread_count; i++)
	{
		rd[i].scene = scene;
		rd[i].render = this;
		rd[i].part1 = i * vRes / thread_count;
		rd[i].part2 = (i + 1) * vRes / thread_count;
		rd[i].cpu = i;
		rd[i].viewDist = viewDist;
		rd[i].viewSize = viewSize;
	}

	V2::Thread **threads = new V2::Thread*[thread_count];

	// Execute threads
	for (i = 0; i < thread_count; i++) {
		threads[i] = new V2::Thread(RenderThread, &rd[i]);
	}

	// Wait for threads to finish and delete
	for (i = 0; i < thread_count; i++) {
		delete threads[i];
	}

	delete [] threads;
	delete [] rd;
}

