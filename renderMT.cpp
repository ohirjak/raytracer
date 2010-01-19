/*
 * @file: renderMT.cpp
 * @author: Ondrej Hirjak, 2010
 * @description: Multi-threaded renderer.
 */

#include <pthread.h>
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
	// TODO: Bugged, probably because of static.
	RenderData *rd = (RenderData*)attr;
	double aspectRatio = (double)rd->render->hRes / rd->render->vRes;
	Point3D eye = Point3D(0.0, 0.0, rd->viewDist);
	Point3D leftTop = Point3D(-rd->viewSize * aspectRatio, rd->viewSize, 0.0);
	Point3D rightBottom = Point3D(rd->viewSize * aspectRatio, -rd->viewSize, 0.0);

	Vector3D d = (Vector3D)rightBottom - leftTop;

	ColorRGBA *rgba_buffer = (ColorRGBA*) rd->render->color_buffer;

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

			unsigned char outColor[4];

			outColor[0] = color.b * 255;
			outColor[1] = color.g * 255;
			outColor[2] = color.r * 255;
			outColor[3] = color.a * 255;

			// write color
			rgba_buffer[iterY * rd->render->hRes + iterX].x.R = outColor[0];
			rgba_buffer[iterY * rd->render->hRes + iterX].x.G = outColor[1];
			rgba_buffer[iterY * rd->render->hRes + iterX].x.B = outColor[2];
			rgba_buffer[iterY * rd->render->hRes + iterX].x.A = outColor[3];

			rd->render->recursion_buffer[iterY * rd->render->hRes + iterX] = recDepth;
		}
	}

	return NULL;
}


void RenderMT::RenderScene(Scene *scene, double viewDist, double viewSize)
{
	// Multi-threaded renderer
	// Renderer uses OpenGL right-handed coords

	// Creating 2 threads for now
	RenderData rd1, rd2;

	rd1.scene = scene;
	rd1.render = this;
	rd1.part1 = 0;
	rd1.part2 = vRes / 2;
	rd1.viewDist = viewDist;
	rd1.viewSize = viewSize;

	rd2 = rd1;
	rd2.part1 = rd1.part1 + 1;
	rd2.part2 = vRes;

	pthread_t threads[2];

	pthread_create(&threads[0], NULL, RenderThread, &rd1);
	pthread_create(&threads[1], NULL, RenderThread, &rd2);

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
}

