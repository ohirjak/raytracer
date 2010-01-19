/*
 * @file: renderGrid.h
 * @author: Ondrej Hirjak, 2010
 * @description: Grid-based rendered.
 */

#ifndef rendergrid_h
#define rendergrid_h


class RenderGrid : public Render
{
	public:
		RenderGrid(long init_hRes, long init_vRes);
		~RenderGrid();

		virtual void RenderScene(Scene *scene, double viewDist, double viewSize);
};


#endif // rendergrid_h

