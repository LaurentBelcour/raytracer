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
//	scene->addObject(new Triangle(Vector(1.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), Vector(0.0, 0.0, -1.0))) ;

	std::vector<Vector> vertices ;
	std::vector<unsigned int> faces ;
	vertices.push_back(Vector(1.0, 0.0, 0.0)) ; faces.push_back(0) ;
	vertices.push_back(Vector(0.0, 0.0, 1.0)) ; faces.push_back(1) ;
	vertices.push_back(Vector(0.0, 0.0, -1.0)) ; faces.push_back(2) ;
	scene->addObject(new TriangleMesh(vertices, faces)) ;
*/
	scene->addObject(new Sphere(2.0f, Vector(5.0, 10.0, 5.0))) ;
	scene->addObject(new Sphere(1.0f, Vector(2.0f, 0.0f, 0.0f))) ;
	
	scene->addLight(new PointLight(Vector(0.0, -10.0, 10.0), Vector(1.0, 1.0, 1.0))) ;

//	FrequencyIntegrator* integrator = new FrequencyIntegrator(scene, camera) ;	
	Whitted* integrator = new Whitted(scene, camera, 1) ;
	integrator->filename(std::string("Whitted.png")) ;
	integrator->integrate() ;

	delete scene;

	return 0;
}
