/*
 * @file: renderGPU.cpp
 * @author: Ondrej Hirjak, 2010
 * @description: GPU renderer.
 */

#include "colors.h"
#include "geometries.h"
#include "scene.h"
#include "render.h"
#include "renderGPU.h"


RenderGPU::RenderGPU(long init_hRes, long init_vRes) : Render(init_hRes, init_vRes)
{
}


RenderGPU::~RenderGPU()
{
}


void RenderGPU::RenderScene(Scene *scene, double viewDist, double viewSize)
{
	// TODO: Implement GPU renderer
	// Renderer uses OpenGL right-handed coords

	Render::RenderScene(scene, viewDist, viewSize);
}

