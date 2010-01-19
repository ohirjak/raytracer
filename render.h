/*
 * @file: render.h
 * @author: Ondrej Hirjak, 2010
 * @description: Generic scene rendered (no optimalizations).
 */

#ifndef render_h
#define render_h


class Render
{
	private:
		unsigned char *color_buffer;
		int *recursion_buffer;

		long output_hRes, output_vRes;

	public:
		Render();
		~Render();

		void RenderScene(Scene *scene, double viewDist, double viewSize, long hRes, long vRes);

		void OutputBuffers(const char *color_output_file, const char *debug_output_file = NULL);
};


#endif // render_h

