#pragma once

#include <list>
#include <vector>
#include <string>

#include "Scene.h"

class TriangleMesh : public Object
{
	public:
		TriangleMesh(const std::vector<Vector>& vertices, const std::vector<unsigned int>& faces) ;

		virtual bool intersect(const Vector& origin, const Vector& direction, Hit& hit, float& d) const ;

		// Load a triangle mesh from a .obj file
		//
		static TriangleMesh* load_obj(std::string filename) ;

	private:
		inline bool intersect_triangle(unsigned int face_id, const Vector& origin, const Vector& direction, Hit& hit) const
		{
			Vector e1 = _vertices[_faces[face_id+1]] - _vertices[_faces[face_id+0]] ; float n1 = norm(e1) ;
			Vector e2 = _vertices[_faces[face_id+2]] - _vertices[_faces[face_id+0]] ; float n2 = norm(e2) ;
			
			float denom = dot(direction, _per_face_normals[face_id]) ;

			if(denom == 0.0f)
			{
				// The ray is parallel to the triangle,
				// the min distance is the projection of distance
				Vector ao = _vertices[_faces[face_id+0]] - origin ;
				hit.occlusion_dist = dot(ao, _per_face_normals[face_id]) ;
				hit.occlusion_dir  = -_per_face_normals[face_id] ;

				return false ;
			}
			else
			{
				Vector ao = _vertices[_faces[face_id+0]] - origin ;
				float t = dot(ao, _per_face_normals[face_id]) / denom ;
				
				Vector p = origin + t*direction ;
				Vector e3 = (p - _vertices[_faces[face_id+0]]) ;
				
				float u = dot(e3, e1) / (n1*n1);
				float v = dot(e3, e2) / (n2*n2);

				if(t >= 0.0f && u >= 0.0f && v >= 0.0f && u+v <= 1.0f)
				{
					hit.t = t ;
					hit.p = p ;
					hit.n = _per_face_normals[face_id] ;
					hit.o = this ;
					hit.occlusion_dist = 0.0f ;

					return t >= 0.0f ;
				}
				else
				{
					Vector G = (_vertices[_faces[face_id+0]]+_vertices[_faces[face_id+1]]+_vertices[_faces[face_id+2]]) / 3.0f ;
					hit.occlusion_dist = norm(p - G) ;
					hit.occlusion_dir  = (p-G) / hit.occlusion_dist ;
					hit.occlusion_t    = t ;

					return false ;
				}
			}
			Triangle triangle(_vertices[_faces[face_id+0]], _vertices[_faces[face_id+1]], _vertices[_faces[face_id+2]]) ;
		}

		std::vector<Vector> _vertices ;
		std::vector<Vector> _per_face_normals ;
		std::vector<unsigned int> _faces ;
} ;
