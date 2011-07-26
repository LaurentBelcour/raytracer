#include "Whitted.h"

#include "CImg.h"

Whitted::Whitted(Scene* scene, Camera* camera, unsigned int max_depth) 
	: Integrator(scene, camera), _max_depth(max_depth)
{
}

void Whitted::integrate() const
{
	Vector origin, direction ;
	float d ;
	Hit hit ;
	cimg_library::CImg<float> buffer(_camera->width(), _camera->height(), 1, 3, 0.0f) ;

	for(unsigned int i=0; i<_camera->width(); ++i)
		for(unsigned int j=0; j<_camera->height(); ++j)
		{
			Vector spectrum(0.0f, 0.0f, 0.0f) ;

			_camera->sample(i+0.5f, j+0.5f, origin, direction) ;
			spectrum = raytrace(origin, direction, 0) ;

			buffer(i, j, 0, 0) = 255.0f * spectrum[0] ;
			buffer(i, j, 0, 1) = 255.0f * spectrum[1] ;
			buffer(i, j, 0, 2) = 255.0f * spectrum[2] ;
		}

	buffer.save(filename().c_str()) ;

}

Vector Whitted::raytrace(Vector origin, Vector direction, unsigned int i) const
{
	if(i >= _max_depth)
		return Vector(0.0f, 0.0f, 0.0f);
		
	Vector spectrum(0.0f, 0.0f, 0.0f) ;
	Hit hit; float d;
	if(_scene->intersect(origin, direction, hit, d))
	{
		// TODO all the lights
		Vector position;
		Vector light = _scene->one_light()->sample(position) ;

		Vector to_light = (position-hit.p) / norm(position-hit.p) ;
		Hit light_hit ;
		float d ;
		if(!_scene->intersect(hit.p, to_light, light_hit, d))
			spectrum += fmax(dot(to_light, hit.n), 0.0f) * light ;

		// TODO the specular term
	}

	return spectrum ;
}
