#include "TriangleMesh.h"

#include <fstream>
#include <sstream>

using namespace std ;

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

TriangleMesh* TriangleMesh::load_obj(std::string filename)
{
	// Arguments of the triangle mesh
	vector<Vector> vertices ;
	vector<unsigned int> faces ;

	// Open file
	ifstream obj_content(filename.c_str(), ios_base::in);

	// Parsing lines
	while(obj_content.good() && !obj_content.eof())
	{
		string s;
		obj_content >> s;
		
		// Parse vertex
		// v p[0] p[1] p[2]
		if(s.compare("v") == 0)
		{
			Vector p;
			obj_content >> p[0] >> p[1] >> p[2];
			vertices.push_back(p);
		}
		// Parse face
		// f v[/t/n|/t|//n] v[/t/n|/t|//n] v[/t/n|/t|//n] [v[/t/n|/t|//n]]
		else if(s.compare("f") == 0)
		{
			char c_line[1024];
			obj_content.getline(c_line, 1024);
			istringstream line(c_line);

			string v;
			while(line.good() && !line.eof())
			{
				line >> v;

				unsigned int i = 1;
				size_t pos = v.find_first_of('/');
				size_t old_pos = 0;

				unsigned int indice;
				istringstream number(v.substr(old_pos, pos), istringstream::in);
				number >> indice;
				faces.push_back(indice-1);
			}
		}
	}

	return new TriangleMesh(vertices, faces) ;
} ;

