#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "../base_header.hpp"

namespace ee
{




typedef struct s_block_info
{
	glm::vec3 pos;
	int normal;
	int type;
}		t_block_info;



	class chunk
	{
		// private :
		public :

		char *data;

		std::vector<t_block_info> toVBO;
		std::vector<unsigned int> triangles;
		std::vector<glm::vec3> vertexes;
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

		void createVertex(std::vector<glm::vec3> &vertexes, glm::vec3 pos);
		void createTrianglesFace(int start, std::vector<unsigned int> &triangles);
		void parkour(std::vector<glm::vec3> &vertexes,std::vector<unsigned int> &triangles, bool *tab, int pos);
		void dataToVBO(std::vector<glm::vec3> &vertexes, std::vector<unsigned int> &triangles);

		void setPos(int x, int y);
		int getPos_x();
		int getPos_y();

		void setData(char *tmp);
		char *getData();

		void* getToVBO();
		int memoryToVBO();
		
		void* getToEBO();
		int memoryToEBO();

	};


}


#endif