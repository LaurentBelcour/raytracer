#pragma once

#include <iostream>
#include <blitz/tinyvec-et.h>
#include "CImg.h"

#include "Scene.h"

class Camera
{
	public:
		Camera()
		{
			_position  = Vector(0.0f, -10.0f, 0.0f) ;
			_direction = Vector(0.0f,  1.0f, 0.0f) ;
			_right     = Vector(1.0f,  0.0f, 0.0f) ;
			_up        = Vector(0.0f,  0.0f, 1.0f) ;
			_fov = M_PI / 4.0f ;
			_W = 400;
			_H = 400;
		} ;

		void raytrace(const Scene* scene) const
		{
			Vector origin, direction ;
			float d ;
			Hit hit ;
			cimg_library::CImg<float> buffer(_W, _H, 1, 3, 0.0f) ;
			cimg_library::CImg<float> frq_buffer(_W, _H, 1, 1, 0.0f) ;

			for(unsigned int i=0; i<_W; ++i)
				for(unsigned int j=0; j<_H; ++j)
				{
					sample(i+0.5f, j+0.5f, origin, direction) ;
					if(scene->intersect(origin, direction, hit, d))
					{
//*
						Vector position ;
						Vector spectrum = scene->one_light()->sample(position) ;
			
						Vector to_light = position-hit.p ;
						to_light = to_light / norm(to_light) ;
						spectrum = spectrum * fmax(dot(to_light, hit.n), 0.0f) * hit.o->material()->reflectance(Vector(0.0f,0.0f,dot(-direction, hit.n)), Vector(0.0f,0.0f,dot(to_light, hit.n))) ;

						buffer(i ,j, 0, 0) = 255.0f * spectrum[0] ;
						buffer(i ,j, 0, 1) = 255.0f * spectrum[1] ;
						buffer(i ,j, 0, 2) = 255.0f * spectrum[2] ;
/*/
						buffer(i ,j, 0, 0) = 255.0f ;
						buffer(i ,j, 0, 1) = 0.0f ;
						buffer(i ,j, 0, 2) = 0.0f ;
//*/
					}

					if(hit.occlusion_dist > 0.0f)
						frq_buffer(i, j) = fmin(255.0f / hit.occlusion_dist, 255.0f);
				}

			buffer.save("test.bmp") ;
			frq_buffer.save("frq_test.bmp") ;
			
		} ;

		void sample(float i, float j, Vector& origin, Vector& direction) const
		{
			origin = _position ;
			float x = 2.0f * tan(_fov) * (float)(_W*0.5f-i) /(float) _W ;
			float y = 2.0f * tan(_fov) * (float)(_H*0.5f-j) / (float)_H ;
			direction = x*_right + y*_up + _direction ;
			direction /= sqrt(blitz::dot(direction, direction)) ;
		} ;

		unsigned int width()  const { return _W ; } ;
		unsigned int height() const { return _H ; } ;


	private:
		Vector _position, _direction, _right, _up ;
		float _fov ;

		unsigned int _W, _H ;
} ;
