#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "../base_header.hpp"

# ifndef RLE_HPP
#  include "RLE.hpp"
# endif

namespace ee
{


class chunk
{
	
private :

	u_char*							data;
	ee::RLE							rle;

	int 							size_x = 16;
	int 							size_y = 16;
	int 							size_z = 256;
	int 							pos_x;
	int 							pos_y;
	int 							size_vertex = 3;

	std::vector<unsigned int> 		triangles;
	std::vector<int> 				vertexes;

public :

		/*****	1 - constructors 		*****/

	~chunk();
	chunk();
	chunk(int x, int y);
	chunk(int x, int y, ee::RLE &other);
	chunk(float seed);
	chunk(int x, int y, float seed);

		/*****	2 - fillers	 			*****/

	void 							fillFromData();

		/*****	3 - toVbo		 		*****/				
	
	void 							createPointVertex(std::vector<int> &vertexes, int pos, u_char orientation, u_char type);
	void 							createTrianglesFace(int start, std::vector<unsigned int> &triangles);
	void 							parkour(int start_vert, std::vector<int> &vertexes,std::vector<unsigned int> &triangles, bool *tab, int pos);
									
	void 							dataToVBO(std::vector<int> &vertexes, std::vector<unsigned int> &triangles);
	
	void 							rleToVbo(std::vector<int> &out_vertexes, std::vector<unsigned int> &out_triangles);

		/*****	4 - getters	 			*****/


	void 							setPos(int x, int y);
	int 							getPos_x();
	int 							getPos_y();

	bool 							setData(u_char *tmp);
	u_char*							getData();

	void							setRle(ee::RLE &other);
	ee::RLE&						getRle();


};


}


#endif