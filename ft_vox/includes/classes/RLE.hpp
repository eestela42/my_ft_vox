#ifndef RLE_HPP
# define RLE_HPP
# include "../base_header.hpp"


namespace ee
{
	
	class RLE
	{
		public :
		u_int size_x = 32;
		u_int size_y = 32;
		u_int size_z = 256;
		std::vector<std::vector<u_char>> rubans;


		public :

		~RLE();
		RLE();
		RLE(const RLE &other);
		RLE(u_int x, u_int y, u_int z);
		RLE(u_int x, u_int y, u_int z, u_char type);

		void				 	createFromData(u_char *data);
		u_char*					createDataFromRle();
		void					makeRowFaces(std::vector<u_char> &faces, u_int i, u_int end, std::vector<u_char> &ruban, u_int *pos, u_char facing);
		std::vector<u_char>		getVisibleFaces();

		void print();

		RLE& operator=(const RLE &other);
	};
}



#endif