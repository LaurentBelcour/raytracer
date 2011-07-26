#include "Scene.h"
#include "Camera.h"
#include "Integrator.h"
#include "TriangleMesh.h"
#include "Whitted.h"

#include <string>

int main()
{
	Scene* scene = new Scene() ;
	Camera* camera = new Camera() ;
/*
	scene->addObject(new Sphere(2.0f, Vector(5.0, 10.0, 5.0))) ;
	scene->addObject(new Sphere(1.0f, Vector(2.0f, 0.0f, 0.0f))) ;
*/

	scene->addObject(TriangleMesh::load_obj(std::string("cube.obj"))) ;
	scene->addLight(new PointLight(Vector(0.0, -10.0, 10.0), Vector(1.0, 1.0, 1.0))) ;

	Whitted* integrator = new Whitted(scene, camera, 1) ;
	integrator->filename(std::string("Whitted.png")) ;
	integrator->integrate() ;

	delete scene;

	return 0;
}
