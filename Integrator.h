#pragma once

class Integrator
{
	public:
		Integrator(Scene* scene, Camera* camera) : _camera(camera), _scene(scene) { } ;

		virtual void integrate() const = 0 ;

	protected:
		const Camera* _camera ;
		const Scene* _scene ;
} ;

class FrequencyIntegrator : public Integrator
{
	public:
		FrequencyIntegrator(Scene* scene, Camera* camera) : Integrator(scene, camera)	{ } ;

		virtual void integrate() const
		{
			Vector origin, direction ;
			float d ;
			Hit hit ;
			cimg_library::CImg<float> frq_buffer(_camera->width(), _camera->height(), 1, 3, 0.0f) ;

			for(unsigned int i=0; i<_camera->width(); ++i)
				for(unsigned int j=0; j<_camera->height(); ++j)
				{
					Vector frequency(0.0f, 0.0f, 0.0f) ;

					_camera->sample(i+0.5f, j+0.5f, origin, direction) ;
					if(_scene->intersect(origin, direction, hit, d))
					{
						Vector position ;
						Vector spectrum = _scene->one_light()->sample(position) ;

									
						Vector to_light = position-hit.p ;
						Hit light_hit ;
						float d ;
						if(!_scene->intersect(hit.p, to_light/norm(to_light), light_hit, d))
						{
//							frequency = Vector(FLT_MAX, -FLT_MAX, FLT_MAX) ;
						}
					}


					if(hit.occlusion_dist > 0.0f)
					{
						float occlusion_factor = fmin(1.0f / pow(hit.occlusion_dist, 2), FLT_MAX) ;
/*
						frq_buffer(i, j, 0, 0) = hit.occlusion_t ;
						frq_buffer(i, j, 0, 1) = hit.occlusion_t ;
						frq_buffer(i, j, 0, 2) = hit.occlusion_t ;
/*/
						frq_buffer(i, j, 0, 0) = 10.0f * occlusion_factor * fabs(hit.occlusion_dir[0]);
						frq_buffer(i, j, 0, 1) = 10.0f * occlusion_factor * fabs(hit.occlusion_dir[1]);
						frq_buffer(i, j, 0, 2) = 10.0f * occlusion_factor * fabs(hit.occlusion_dir[2]);
//*/
					}
				}

			frq_buffer.save("frq_test.bmp") ;
			
		} ;

} ;
