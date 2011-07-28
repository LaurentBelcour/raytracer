#pragma once

#include "Integrator.h"
#include "Camera.h"
#include "Scene.h"
#include "CImg.h"

class DepthIntegrator : public Integrator
{
	public:
		// Virtual destructor
		//
		DepthIntegrator(Scene* scene, Camera* camera) : Integrator(scene, camera) 
		{
		  color = Vector(1.0f, 1.0f, 1.0f) ;	
		} ;
		virtual ~DepthIntegrator() { };

		// Integrate the depth into the depth buffer
		//
		virtual void integrate() const
		{
			unsigned int W = _camera->width() ;
			unsigned int H = _camera->height() ;

			cimg_library::CImg<float> buffer(W, H, 1, 3, 0.0f) ;

			for(int i=0; i<W; ++i)
				for(int j=0; j<H; ++j)
				{
					float distance = FLT_MAX ;
					Vector position, direction ;
					Hit hit ;
					float d ;

					_camera->sample(i+0.5f, j+0.5f, position, direction) ;
					if(_scene->intersect(position, direction, hit, d))
					{
						distance = norm(hit.p - position) ;
					}

					buffer(i, j, 0, 0) = 255.0f * color[0] / (1.0f+distance) ;
					buffer(i, j, 0, 1) = 255.0f * color[1] / (1.0f+distance) ;
					buffer(i, j, 0, 2) = 255.0f * color[2] / (1.0f+distance) ;
				}

			buffer.save(filename().c_str()) ;
		} ;

	protected:
		Vector color ;
} ;
