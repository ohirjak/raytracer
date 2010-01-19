/*
 * @file: render.h
 * @author: Ondrej Hirjak, 2010
 * @description: Generic scene rendered (no optimalizations).
 */

#ifndef render_h
#define render_h


class Render
{
	protected:
		unsigned char *color_buffer;
		int *recursion_buffer;

		long hRes, vRes;

	public:
		Render(long init_hRes, long init_vRes);
		virtual ~Render();

		virtual void RenderScene(Scene *scene, double viewDist, double viewSize);

		void OutputBuffers(const char *color_output_file, const char *debug_output_file = NULL);
};


#endif // render_h

