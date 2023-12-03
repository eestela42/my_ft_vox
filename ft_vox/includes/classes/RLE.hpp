#ifndef RLE_HPP
# define RLE_HPP
# include "../base_header.hpp"


namespace ee
{
	
	class RLE
	{
		public :
		u_int size_x = 16;
		u_int size_y = 16;
		u_int size_z = 256;
		std::vector<std::vector<u_char>> rubans;


		public :

		~RLE();
		RLE();
		RLE(const RLE &other);
		RLE(u_int x, u_int y, u_int z);
		RLE(u_int x, u_int y, u_int z, u_char type);
		RLE(float seed);

		void				 	createFromData(u_char *data);
		u_char*					createDataFromRle();

		void 					generateChunk(int chunk_x, int chunk_y, float seed);

		void print();

		RLE& operator=(const RLE &other);
	};
}



#endif