#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "../base_header.hpp"
#ifndef VEC3_HPP
#include "vec3.hpp"
#endif

namespace ee
{




typedef struct s_block_info
{
	ee::vec3 pos;
	int normal;
	int type;
}		t_block_info;



	class chunk
	{
		// private :
		public :

		char *data;

		std::vector<t_block_info> toVBO;
		std::vector<t_triangle> triangles;
		int size_x = 32;
		int size_y = 32;
		int size_z = 256;
		int pos_x;
		int pos_y;

		public :

		chunk();
		~chunk();

		void fill();
		void dataToVBO();

		void setData(char *tmp);
		char *getData();

		void* getToVBO();
		int memoryToVBO();
		void* getToEBO();
		int memoryToEBO();

	};


}


#endif