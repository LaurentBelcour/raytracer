#pragma once

#include "Integrator.h"

class Whitted : public Integrator
{
	public:
		// Basic constructor
		//
		Whitted(Scene* scene, Camera* camera, unsigned int max_depth) ;

		// Integrate the scene view from the camera, using the  described
		// whitted algorithm.
		virtual void integrate() const ;

	private:
		// Raytrace in the scene with the depth i
		//
		Vector raytrace(Vector origin, Vector direction, unsigned int i) const ;

		// Maximum depth in while raytracing in the scene
		//
		unsigned int _max_depth ;
} ;
