#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "../base_header.hpp"
#ifndef VEC3_HPP
#include "vec3.hpp"
#endif

#ifndef MESH_HPP
# include "mesh.hpp"
#endif

namespace ee
{
	class chunk
	{
		// private :
		public :

		char 	*data;

		std::vector<t_block_info> toVBO;
		std::vector<t_triangle> triangles;
		int size_x = 32;
		int size_y = 32;
		int size_z = 256;
		int pos_x;
		int pos_y;

		public :

		chunk();
		chunk(int x, int y);
		~chunk();

		void fill();
		void dataToVBO(std::vector<vec3> &base_vertexes, std::vector<t_triangle> &triangles, std::vector<t_block_info> &toVBO);

		void setPos(int x, int y);
		
		void setData(char *tmp);
		char *getData();

		void* getToVBO();
		int memoryToVBO();
		void* getToEBO();
		int memoryToEBO();

	};


}


#endif