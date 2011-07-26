#pragma once

#include <list>
#include <vector>
#include <cmath>
#include <iostream>
#include <float.h>
#include <blitz/tinyvec.h>
#include <blitz/tinyvec-et.h>

typedef blitz::TinyVector<float, 3> Vector;

#define EPSILON 1.0E-05f


class Object ;
class Material ;
class Light ;

class Material
{
	public:
		virtual Vector reflectance(Vector local_in, Vector local_out) const = 0;
} ;

class Phong : public Material
{
	public:
		Phong(float shininess = 1.0f, Vector spectrum = Vector(1.0f, 1.0f, 1.0f)) : _shininess(shininess), _spectrum(spectrum) {} ;

		virtual Vector reflectance(Vector local_in, Vector local_out) const
		{
			Vector local_r(-local_in[0], -local_in[1], local_in[2]) ;
			float rho = pow(fmax(dot(local_r, local_out), 0.0f), _shininess) ;
			return rho * _spectrum ;
		} ;

		float shininess() const { return _shininess ; } ;

	private:
		float _shininess ;
		Vector _spectrum ;
} ;

class Hit
{
	public:
		Hit() : occlusion_dist(FLT_MAX), t(FLT_MAX) { } ;

		float t ;			// Distance of hit along the ray
		Vector p ;			// Hit position
		const Object* o ;	// Object hitted
		Vector n ;			// Normal at the hit position

		Vector occlusion_dir ;	// Direction of occlusion
		float occlusion_dist ;	// Distance of occlusion
		float occlusion_t ;		// Distance along the ray at 
										// which the occlusion occurs
} ;

class Object
{
	public:
		// Constructor
		//
		Object(Material* material) : _material(material) { } ;
		Object() { _material = new Phong(0.0f) ; } ;

		// Intersection query function
		//
		virtual bool intersect(const Vector& origin, const Vector& direction, Hit& hit, float& d) const = 0;

		// Get/Set the current material
		//
		const Material* material() const { return _material ; } ;
		void material(Material* material) { _material = material ; } ;

	private:
		// Each object must have a material information
		//
		Material* _material ;
} ;

class Sphere : public Object
{
	public:
		Sphere() : _radius(1.0f), _center(0.0f, 0.0f, 0.0), Object() { } ;
		Sphere(float radius, const Vector& center) : _radius(radius), _center(center), Object() { } ;

		virtual bool intersect(const Vector& origin, const Vector& direction, Hit& hit, float& d) const 
		{
			Vector oc = _center - origin ;
			float oc_sqrd_norm = dot(oc, oc) ;
			Vector normed_oc = oc / sqrt(oc_sqrd_norm) ;
			float cosine = dot(normed_oc, direction) ;
			float opp_sqrd_norm = oc_sqrd_norm * (1.0f-cosine*cosine) ;

			hit.occlusion_dist = fmax(sqrt(opp_sqrd_norm) - _radius, 0.0f) ;
			hit.occlusion_t    = dot(oc, direction) ;
			hit.occlusion_dir  = oc - hit.occlusion_t*direction ;
			hit.occlusion_dir /= norm(hit.occlusion_dir) ;

			if(hit.occlusion_dist == 0.0f)
			{
				hit.t = sqrt(oc_sqrd_norm) * cosine - sqrt(_radius*_radius - opp_sqrd_norm) ;
				hit.p = origin + hit.t*direction ;
				hit.o = this ;
				hit.n = (hit.p - _center) / _radius ;
				return hit.t >= 0.0f;
			}
			else
			{
				return false ;
			}
		};

	private:
		float _radius ;
		Vector _center ;
} ;

class Triangle : public Object
{
	public:
		Triangle(const Vector& A, const Vector& B, const Vector& C) : _A(A), _B(B), _C(C), Object()
		{
			Vector v1 = _B-_A ;
			Vector v2 = _C-_A ;
			_n = cross(v1, v2) / (norm(v1) * norm(v2));
		}																

		virtual bool intersect(const Vector& origin, const Vector& direction, Hit& hit, float& d) const 
		{
#ifdef MOLLER_ALGORITHM
			Vector e1 = _B - _A ;
			Vector e2 = _C - _A ;
			Vector s  = origin - _A ;

			Vector s1 = cross(direction, e2) ;
			Vector s2 = cross(s, e1) ;

			float det = dot(s1, e1) ;
			if(det > EPSILON)
			{
				float t = dot(s2, e2) / det ;
				float u = dot(s1, s ) / det ;
				float v = dot(s2, direction) / det ;

				if(u < 0.0f || v < 0.0f || u+v > 1.0f)
				{
					Vector G = (_A+_B+_C) / 3.0f ;
					Vector p = origin + hit.t * direction ;
					hit.occlusion_dist = norm(p - G) ;

					return false ;
				}
				else
				{
					hit.t = t ;
					hit.p = origin + hit.t * direction ;
					hit.n = _n ;
					hit.o = this ;
					hit.occlusion_dist = 0.0f ;

					return t >= 0.0f ;
				}
			}
			else
			{
				Vector ao = _A - origin ;
				hit.occlusion_dist = dot(ao, _n) ;
				return false ;
			}
#else
			Vector e1 = _B - _A ; float n1 = norm(e1) ;
			Vector e2 = _C - _A ; float n2 = norm(e2) ;
			
			float denom = dot(direction, _n) ;

			if(denom == 0.0f)
			{
				// The ray is parallel to the triangle,
				// the min distance is the projection of distance
				Vector ao = _A - origin ;
				hit.occlusion_dist = dot(ao, _n) ;
				hit.occlusion_dir  = -_n ;

				return false ;
			}
			else
			{
				Vector ao = _A - origin ;
				float t = dot(ao, _n) / denom ;
				
				Vector p = origin + t*direction ;
				Vector e3 = (p - _A) ;
				
				float u = dot(e3, e1) / (n1*n1);
				float v = dot(e3, e2) / (n2*n2);

				if(t >= 0.0f && u >= 0.0f && v >= 0.0f && u+v <= 1.0f)
				{
					hit.t = t ;
					hit.p = p ;
					hit.n = _n ;
					hit.o = this ;
					hit.occlusion_dist = 0.0f ;

					return t >= 0.0f ;
				}
				else
				{
					Vector G = (_A+_B+_C) / 3.0f ;
					hit.occlusion_dist = norm(p - G) ;
					hit.occlusion_dir  = (p-G) / hit.occlusion_dist ;
					hit.occlusion_t    = t ;

					return false ;
				}
			}
#endif
		} ;
	private:
		Vector _A, _B, _C;	// Three points of the triangle
		Vector _n ;				// Associated normal
} ;

class Scene
{
	public:
		void addObject(const Object* object) { _objects.push_back(object) ; } ;
		void addLight(const Light* light) { _lights.push_back(light) ; } ;
		const std::list<const Object*>* objects() { return &_objects ; } ; 
		const Light* one_light() const
		{
			// TODO sample one source with respect to the total power
			// of all the sources.
			return (*_lights.begin()) ;
		} ;
		bool intersect(const Vector& origin, const Vector& direction, Hit& hit, float & d) const
		{
			std::list<const Object*>::const_iterator iter ;
			hit.t = FLT_MAX ;
			hit.occlusion_dist = FLT_MAX ;
			float dtemp ;
			Hit hittemp ;
			for(iter = _objects.begin(); iter != _objects.end(); ++iter)
			{
				hittemp.t = FLT_MAX;
				hittemp.occlusion_dist = FLT_MAX ;
				if((*iter)->intersect(origin+EPSILON*direction, direction, hittemp, dtemp) && hittemp.t < hit.t)
				{
					hit = hittemp ;
				}

				if(hit.occlusion_dist > hittemp.occlusion_dist)
				{
					hit.occlusion_dist = hittemp.occlusion_dist ;
					hit.occlusion_t    = hittemp.occlusion_t ;
					hit.occlusion_dir  = hittemp.occlusion_dir ;
				}
			}

			return hit.t != FLT_MAX;
		} ;
	private:
		std::list<const Object*> _objects ;
		std::list<const Light*> _lights ;
} ;

class Light
{
	public:
		virtual Vector sample(Vector& position) const = 0 ;
} ;

class PointLight : public Light
{
	public:
		PointLight(const Vector& position, const Vector& spectrum) : _position(position), _spectrum(spectrum) { } ;
		virtual Vector sample(Vector& position) const 
		{
			position = _position ;
			return _spectrum ;
		};

	protected:
		Vector _position ;
		Vector _spectrum ;
} ;
