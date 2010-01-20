/*
 * @file: renderMT.h
 * @author: Ondrej Hirjak, 2010
 * @description: Multi-threaded rendered.
 */

#ifndef rendermt_h
#define rendermt_h


class RenderMT : public Render
{
	struct RenderData
	{
		Scene *scene;
		RenderMT *render;
		long part1;
		long part2;
		int cpu;

		double viewDist;
		double viewSize;
	};

	private:
		static void* RenderThread(void *attr);

	public:
		RenderMT(long init_hRes, long init_vRes);
		~RenderMT();

		virtual void RenderScene(Scene *scene, double viewDist, double viewSize);
};


#endif // rendermt_h

