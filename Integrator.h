#pragma once

#include <string>

#include "Camera.h"
#include "Scene.h"


/// Integrator interface. Used to create your own ray tracing algorithm.
///
class Integrator
{
	public:
		// Integrator default constructor, should be called by all integrator
		//
		Integrator(Scene* scene, Camera* camera) : _camera(camera), _scene(scene), _filename("unknown.bmp") { } ;

		// Integrate function
		//
		virtual void integrate() const = 0 ;

		// Get/Set the file the integrator will output to
		//
		std::string filename() const { return _filename ;} ;
		void filename(const std::string& filename) { _filename = filename ; } ;

	protected:
		// Fields of the integrator
		// TODO put them private
		//
		const Camera* _camera ;
		const Scene* _scene ;
		std::string _filename ;
} ;


