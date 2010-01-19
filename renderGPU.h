/*
 * @file: renderGPU.h
 * @author: Ondrej Hirjak, 2010
 * @description: GPU rendered.
 */

#ifndef rendergpu_h
#define rendergpu_h


class RenderGPU : public Render
{
	public:
		RenderGPU(long init_hRes, long init_vRes);
		~RenderGPU();

		virtual void RenderScene(Scene *scene, double viewDist, double viewSize);
};


#endif // rendergpu_h

