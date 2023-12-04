#ifndef RLE_HPP
# define RLE_HPP
# include "../base_header.hpp"


namespace ee
{
	
class RLE
{

private :

	u_int 							size_x = 16;
	u_int 							size_y = 16;
	u_int 							size_z = 255;
	std::vector<u_char>	rubans;


public :

		/*****	1 - constructors 		*****/

	~RLE();
	RLE();
	RLE(const RLE &other);
	RLE(u_int x, u_int y, u_int z);
	RLE(u_int x, u_int y, u_int z, u_char type);
	RLE(float seed);

		/*****	2 - fillers	 		*****/

	void				 			createFromData(u_char *data);
	u_char*							createDataFromRle();

	void 							generateChunk(int chunk_x, int chunk_y, float seed);

							
		/*****	3 - data getters 		*****/

	u_int							getSize_x();
	u_int							getSize_y();
	u_int							getSize_z();

	std::vector<u_char>&	getRubans();
	
		/*****	4 - operators 		*****/

	void 							print();

	RLE& 							operator=(const RLE &other);
};
}



#endif