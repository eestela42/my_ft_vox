#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "../base_header.hpp"

namespace ee
{







	class chunk
	{
		// private :
		public :

		u_char *data;
		std::vector<unsigned int> triangles;
		std::vector<int> vertexes;
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

		void createVertex(std::vector<int> &vertexes, glm::vec3 pos);
		void createPointVertex(std::vector<int> &vertexes, int pos, char orientation, char type);


		void createTrianglesFace(int start, std::vector<unsigned int> &triangles);
		void parkour(int start_vert, std::vector<int> &vertexes,std::vector<unsigned int> &triangles, bool *tab, int pos);
		void dataToVBO(std::vector<int> &vertexes, std::vector<unsigned int> &triangles);

		void setPos(int x, int y);
		int getPos_x();
		int getPos_y();

		void setData(u_char *tmp);
		u_char *getData();

		void* getToVBO();
		int memoryToVBO();
		
		void* getToEBO();
		int memoryToEBO();

	};


}


#endif