/*
 * @file: renderGrid.cpp
 * @author: Ondrej Hirjak, 2010
 * @description: Grid-based renderer.
 */

#include "colors.h"
#include "geometries.h"
#include "scene.h"
#include "render.h"
#include "renderGrid.h"


RenderGrid::RenderGrid(long init_hRes, long init_vRes) : Render(init_hRes, init_vRes)
{
}


RenderGrid::~RenderGrid()
{
}


void RenderGrid::RenderScene(Scene *scene, double viewDist, double viewSize)
{
	// TODO: Implement grid-based renderer
	// Renderer uses OpenGL right-handed coords

	Render::RenderScene(scene, viewDist, viewSize);
}

