#include "TriangleMesh.h"


TriangleMesh::TriangleMesh(const std::vector<Vector>& vertices, const std::vector<unsigned int>& faces) :
	_vertices(vertices), _faces(faces), Object()
{ 
	for(int i=0; i<_faces.size(); ++i)
	{
		Vector v1 = _vertices[i*3+1] - _vertices[i*3+0] ;
		Vector v2 = _vertices[i*3+2] - _vertices[i*3+0] ;
		Vector n = cross(v1, v2) ;
		_per_face_normals.push_back( n / norm(n) );
	}
} ;


bool TriangleMesh::intersect(const Vector& origin, const Vector& direction, Hit& hit, float& d) const
{
	Hit hittemp ;
	hittemp.t = FLT_MAX ;
	hittemp.occlusion_dist = FLT_MAX ;
	bool intersect = false ;

	for(int i=0; i<_faces.size(); i+=3)
	{
		if(intersect_triangle(i, origin, direction, hittemp) && hittemp.t < hit.t)
		{
			hit = hittemp ;
			hit.o = this;
			intersect = true ;
		}

		if(hit.occlusion_dist > hittemp.occlusion_dist)
		{
			hit.occlusion_dist = hittemp.occlusion_dist ;
			hit.occlusion_t    = hittemp.occlusion_t ;
			hit.occlusion_dir  = hittemp.occlusion_dir ;
		}
	}

	return intersect ;
}

void TriangleMesh::load_obj(std::string filename)
{
} ;

