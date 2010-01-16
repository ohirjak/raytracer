/*
 * @file: scene.h
 * @author: Ondrej Hirjak, 2009
 * @description: 
 */

#ifndef scene_h
#define scene_h

#include <list>

using namespace std;


class Light
{
	public:
		Point3D p;
		Color c;

		Light(Point3D p1, Color c1) : p(p1), c(c1) { }
};


class Scene
{
	private:
		list<Light *> lights;
		list<GeometryObject *> objects;

		GeometryObject* GetIntersectObject(Ray &ray);
		Color TraceRay(Ray &ray, int recursionDepth = 0);

	public:
		Scene() { }
		~Scene() { ClearScene(); }

		void ClearScene();

		void AddLight(Light *light);
		void AddObject(GeometryObject *object);

		void Render(double viewDist, double viewSize, long hRes, long vRes);
};


#endif // scene_h
