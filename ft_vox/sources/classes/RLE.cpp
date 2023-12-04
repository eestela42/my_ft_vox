#include "../../includes/classes/RLE.hpp"
#include "../../includes/header.hpp"

namespace ee
{

		/*****	1 - constructors 		*****/

RLE::~RLE()
{
}

RLE::RLE()
{
}

RLE::RLE(const RLE &other)
{
	*this = other;
}

RLE::RLE(u_int max_x, u_int max_y, u_int max_z, u_char type)
{
	rubans.reserve(size_x * size_y * 4);
	for (u_int x = 0; x < max_x && x < this->size_x; x++)
	{
		for (u_int y = 0; y < max_y && y < this->size_y; y++)
		{
			if (x == 5 & y == 5)
			{
				this->rubans.push_back(0);
				this->rubans.push_back(255);
				continue;
			}
			this->rubans.push_back(type);
			this->rubans.push_back(max_z);
			if (max_z != size_z)
			{
				this->rubans.push_back(0);
				this->rubans.push_back((u_char)this->size_z - max_z);
			}
		}
	}
}

RLE::RLE(u_int max_x, u_int max_y, u_int max_z)
{
	// rubans.resize(size_x * size_y);
	// std::srand(std::time(nullptr));	
	// for (u_int x = 0; x < max_x && x < this->size_x; x++)
	// {
	// 	for (u_int y = 0; y < max_y && y < this->size_y; y++)
	// 	{
	// 		u_char rand = std::rand() % 10;
	// 		this->rubans[x + y * this->size_x].push_back(rand + 1);
	// 		this->rubans[x + y * this->size_x].push_back((u_char)max_z);
	// 	}
	// }
}

RLE::RLE(float seed)
{
	// std::cout << "RLE seed" << std::endl;
	// rubans.resize(size_x * size_y);
	// ee::perlinNoise noise = ee::perlinNoise();
	// for (u_int x = 0; x < this->size_x; x++)
	// {
	// 	for (u_int y = 0; y < this->size_y; y++)
	// 	{
	// 		int h = std::abs(noise.noise2d(x + seed, y + seed) * 5) + 10;
			
	// 		this->rubans[x + y * this->size_x].push_back((u_char)((x * y) % 255 + 1));
	// 		this->rubans[x + y * this->size_x].push_back((u_char)h);
	// 	}
	// }

}

		/*****	2 - fillers	 		*****/

void RLE::generateChunk(int chunk_x, int chunk_y,float seed)
{
	// rubans.clear();
	// rubans.resize(size_x * size_y);
	// ee::perlinNoise noise = ee::perlinNoise();
	// for (u_int x = 0; x < this->size_x; x++)
	// {
	// 	std::srand(glfwGetTime() * 10000);

	// 	for (u_int y = 0; y < this->size_y; y++)
	// 	{
	// 		int h = std::abs(noise.noise2d(x + seed, y + seed) * 12) + 10;
			
	// 		this->rubans[x + y * this->size_x].push_back((u_char)(std::rand() % 35 + 1));
	// 		this->rubans[x + y * this->size_x].push_back((u_char)h);
	// 	}
	// }
}

void	RLE::createFromData(u_char *data)
{
	// this->rubans.resize(this->size_x * this->size_y);
	
	// for (u_int x = 0; x < this->size_x; x++)
	// {
	// 	for (u_int y = 0; y < this->size_y; y++)
	// 	{
	// 		u_char type = data[x + y * this->size_x];
	// 		u_int size = 0;
	// 		for (u_int z = 0; z < this->size_z; z++)
	// 		{
	// 			if (data[x + y * this->size_x + z * this->size_x * this->size_y] != type)
	// 			{
	// 				this->rubans[x + y * this->size_x].push_back(type);
	// 				this->rubans[x + y * this->size_x].push_back(size);
	// 				size = 0;
	// 				type = data[x + y * this->size_x + z * this->size_x * this->size_y];
	// 			}
	// 			size++;
	// 		}
	// 		this->rubans[x + y * this->size_x].push_back(type);
	// 		this->rubans[x + y * this->size_x].push_back(size - 1);
	// 	}
	// }
	
}

u_char*	RLE::createDataFromRle()
{
	u_char *data = new u_char[this->size_x * this->size_y * this->size_z];
	u_int max = size_x * size_y * size_z;
	for (int i = 0; i < max; i++)
		data[i] = 0;
	u_int x = 0;
	u_int y = 0;
	u_int z = 0;
	for (u_int i = 0; i < this->rubans.size(); i += 2)
	{
		data[x + y * size_y + z * (size_x * size_y)] = this->rubans[i];
		if (++x  % size_x == 0 && ++y)
			x = 0;
		if (!x && ++y % size_x == 0 && ++z)
			y = 0;
	}
	return (data);
}

		/*****	3 - data getters 		*****/

u_int								RLE::getSize_x()
{
	return (this->size_x);
}

u_int								RLE::getSize_y()
{
	return (this->size_y);
}

u_int								RLE::getSize_z()
{
	return (this->size_z);
}

std::vector<u_char>&	RLE::getRubans()
{
	return (this->rubans);
}


		/*****	4 - operators 		*****/

void	RLE::print()
{
	std::cout << "RLE" << std::endl;
	int pos = 0;
	for (int i = 0; i < this->rubans.size(); i += 2)
	{
		//pos = x + y * size_x + z * (size_x * size_y);
		std::cout << "pos : {" << pos << "}" 	<< " x : " << pos / size_z % size_x
												<< " y : " << (pos / (size_z * size_x)) % size_y
												<< " - "
		<< (u_int)this->rubans[i] << " " << (u_int)this->rubans[i + 1] << " - " << std::endl;
		pos += this->rubans[i + 1];
	}
}



RLE& RLE::operator=(const RLE &other)
{
	this->size_x = other.size_x;
	this->size_y = other.size_y;
	this->size_z = other.size_z;
	this->rubans = other.rubans;
	return (*this);
}

}