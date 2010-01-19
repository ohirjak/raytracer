/*
 * @file: render.cpp
 * @author: Ondrej Hirjak, 2010
 * @description: Generic renderer implementation.
 */

#include <stdio.h>
#include "colors.h"
#include "geometries.h"
#include "scene.h"
#include "render.h"


Render::Render(long init_hRes, long init_vRes)
{
	hRes = init_hRes;
	vRes = init_vRes;

	unsigned long size = hRes * vRes * 4;
	color_buffer = new unsigned char[size];

	recursion_buffer = new int[hRes * vRes];
}


Render::~Render()
{
	delete color_buffer;
	delete recursion_buffer;
}


void Render::RenderScene(Scene *scene, double viewDist, double viewSize)
{
	// Simple single-threaded renderer
	// Renderer uses OpenGL right-handed coords

	double aspectRatio = (double)hRes / vRes;
	Point3D eye = Point3D(0.0, 0.0, viewDist);
	Point3D leftTop = Point3D(-viewSize * aspectRatio, viewSize, 0.0);
	Point3D rightBottom = Point3D(viewSize * aspectRatio, -viewSize, 0.0);

	Vector3D d = (Vector3D)rightBottom - leftTop;

	ColorRGBA *rgba_buffer = (ColorRGBA*) color_buffer;

	// Y
	for (long iterY = 0; iterY < vRes; iterY++)
	{
		// X
		for (long iterX = 0; iterX < hRes; iterX++)
		{
			Point3D p = (Vector3D)leftTop + Point3D(d.x * iterX / hRes, d.y * iterY / vRes, 0.0);
			//cout << "View point " << p.x << ":" << p.y << ":" << p.z << endl;

			Ray ray = Ray(eye, p);

			int recDepth;
			Color color = scene->TraceRay(ray, recDepth);
			color = color.Cap();
			//cout << "Color " << color.r << "," << color.g << "," << color.b << endl;

			unsigned char outColor[4];

			outColor[0] = color.b * 255;
			outColor[1] = color.g * 255;
			outColor[2] = color.r * 255;
			outColor[3] = color.a * 255;

			// write color
			rgba_buffer[iterY * hRes + iterX].x.R = outColor[0];
			rgba_buffer[iterY * hRes + iterX].x.G = outColor[1];
			rgba_buffer[iterY * hRes + iterX].x.B = outColor[2];
			rgba_buffer[iterY * hRes + iterX].x.A = outColor[3];

			recursion_buffer[iterY * hRes + iterX] = recDepth;
		}
	}
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


void Render::OutputBuffers(const char *color_output_file, const char *debug_output_file)
{
	// Outputs to BMP file output.bmp
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

	FILE *bmpOut = fopen(color_output_file, "w");

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

	ColorRGBA *rgba_buffer = (ColorRGBA*) color_buffer;

	// Y
	for (long iterY = 0; iterY < vRes; iterY++)
	{
		// X
		for (long iterX = 0; iterX < hRes; iterX++)
		{
			// Exchanged top & bottom because of BMP bitmap layout
			fwrite(&rgba_buffer[(vRes - iterY) * hRes + iterX], 4, 1, bmpOut);
		}
	}

	fclose(bmpOut);

	if (debug_output_file == NULL)
		return;

	bmpOut = fopen(debug_output_file, "w");

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

	ColorBGRA recursion_scale[10];

	for (int i = 0; i < 10; i++)
	{
		recursion_scale[i].x.R = i * 28;
		recursion_scale[i].x.G = 0;
		recursion_scale[i].x.B = 0;
		recursion_scale[i].x.A = 0;
	}

	// Y
	for (long iterY = 0; iterY < vRes; iterY++)
	{
		// X
		for (long iterX = 0; iterX < hRes; iterX++)
		{
			// Exchanged top & bottom because of BMP bitmap layout
			fwrite(&recursion_scale[recursion_buffer[(vRes - iterY -1) * hRes + iterX]], 4, 1, bmpOut);
		}
	}

	fclose(bmpOut);
}

