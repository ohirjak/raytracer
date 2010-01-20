/*
 * @file: renderMT.cpp
 * @author: Ondrej Hirjak, 2010
 * @description: Multi-threaded renderer.
 */

#include "main.h"
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


void* RenderMT::RenderThread(void *attr)
{
	RenderData *rd = (RenderData*)attr;
	/*cpu_set_t cpuset;
	pthread_t thread;

	// Set thread affinity
	thread = pthread_self();

	CPU_ZERO(&cpuset);
	CPU_SET(rd->cpu % 2, &cpuset); // TODO: use actual number of cores
	cout << "part " << rd->part1 << "-" << rd->part2 << ", cpu " << rd->cpu << endl;

	if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset))
		cerr << "setaffinity error" << endl;*/

	// Perform rendering
	double aspectRatio = (double)rd->render->hRes / rd->render->vRes;
	Point3D eye = Point3D(0.0, 0.0, rd->viewDist);
	Point3D leftTop = Point3D(-rd->viewSize * aspectRatio, rd->viewSize, 0.0);
	Point3D rightBottom = Point3D(rd->viewSize * aspectRatio, -rd->viewSize, 0.0);

	Vector3D d = (Vector3D)rightBottom - leftTop;

	ColorBGRA *rgba_buffer = (ColorBGRA*) rd->render->color_buffer;

	timespec ts1, ts2, res;
	clock_gettime(CLOCK_MONOTONIC, &ts1);

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
			rgba_buffer[iterY * rd->render->hRes + iterX].x.R = color.r * 255;
			rgba_buffer[iterY * rd->render->hRes + iterX].x.G = color.g * 255;
			rgba_buffer[iterY * rd->render->hRes + iterX].x.B = color.b * 255;
			rgba_buffer[iterY * rd->render->hRes + iterX].x.A = color.a * 255;

			rd->render->recursion_buffer[iterY * rd->render->hRes + iterX] = recDepth;
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &ts2);

	res = diff(ts1, ts2);

	printf("Render time (MT), cpu %d in ms %ld%03ld.%ld\n", rd->cpu, (long)res.tv_sec, res.tv_nsec / 1000000, res.tv_nsec % 1000000);

	return NULL;
}


void RenderMT::RenderScene(Scene *scene, double viewDist, double viewSize)
{
	// Multi-threaded renderer
	// Renderer uses OpenGL right-handed coords

	// Creating 3 threads for now
	const int thread_count = 3;
	int i;
	RenderData rd[thread_count];

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

	pthread_t threads[thread_count];

	for (i = 0; i < thread_count; i++)
		pthread_create(&threads[i], NULL, RenderThread, &rd[i]);

	for (i = 0; i < thread_count; i++)
		pthread_join(threads[i], NULL);
}

