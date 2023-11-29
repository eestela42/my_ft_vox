#include "../../includes/classes/chunk.hpp"
#include "../../includes/classes/vec3.hpp"

using namespace ee;

chunk::chunk() : pos_x(0), pos_y(0)
{
	this->data = new char[this->size_x * this->size_y * this->size_z];
}

chunk::~chunk()
{
	delete [] this->data;
}


void chunk::fill()
{
	char block_type[2] = {1, 2};
	for (int z = 0; z < this->size_z / 2; z++)
	{
		for (int y = 0; y < this->size_y; y++)
		{
			for (int x = 0; x < this->size_x; x++)
			{
				this->data[x + y * this->size_x + z * this->size_x * this->size_y] = block_type[(x % 2 + y % 2 + z % 2) % 2];
			}
		}
	}
	
}


struct ThreeBitVariable {
    unsigned int value : 3;
};


void createCube(int x, int y, int z, int zero, std::vector<vec3> &base_vertexes, std::vector<t_triangle> &triangles)
{
	// ThreeBitVariable 	count;
	
	// count.value = 0;
	// std::vector<ThreeBitVariable> posib;
	// while(1)
	// {
	// 	posib.push_back(count);
	// 	unsigned int bit = 0;
	// 	vec3 pos;
	// 	bit = (count.value >> 2) & 1;
	// 	pos[0] = x + 1 * bit;
	// 	bit = (count.value >> 1) & 1;
	// 	pos[1] = y + 1 * bit;
	// 	bit = (count.value >> 0) & 1;
	// 	pos[2] =  + 1 * bit;
	// 	base_vertexes.push_back(pos);
	// 	if (count.value == 7)
	// 		break;
	// 	count.value++;
	// }

	// bool yes = false;
	// int pos = 0;
	// for (int i = 1; i <= 6; i++)
	// {
	// 	std::vector<unsigned int> tmp;
	// 	for (int j = 0; j < posib.size(); j++)
	// 	{
	// 		if ((posib[j].value >> (2 - pos) & 1) == yes)
	// 		{
	// 			tmp.push_back(zero + j);
	// 		}
	// 	}
	// 	triangles.push_back({tmp[0], tmp[1], tmp[2]});
	// 	triangles.push_back({tmp[0], tmp[2], tmp[3]});
		

	// 	yes = !yes;
	// 	if (!yes)
	// 		pos++;
	// }
//000 = 0 001 = 1 010 = 2 011 = 3 100 = 4 101 = 5 110 = 6 111 = 7


/*
v 0.0f 0.0f 0.0f
v 0.0f 1.0f 0.0f
v 1.0f 1.0f 0.0f
v 1.0f 0.0f 0.0f
v 0.0f 0.0f 1.0f
v 0.0f 1.0f 1.0f
v 1.0f 1.0f 1.0f
v 1.0f 0.0f 1.0f







*/

	base_vertexes.push_back({x,		y, 		z});
	base_vertexes.push_back({x, 	y+1, 	z});
	base_vertexes.push_back({x+1, 	y+1, 	z});
	base_vertexes.push_back({x+1, 	y, 		z});
	base_vertexes.push_back({x, 	y, 		z+1});
	base_vertexes.push_back({x, 	y+1, 	z+1});
	base_vertexes.push_back({x+1, 	y+1, 	z+1});
	base_vertexes.push_back({x+1, 	y, 		z+1});

	t_triangle tmp;
	zero--;
// f 1 2 3
// f 1 3 4
	tmp.v[0] = zero + 1; tmp.v[1] = zero + 2; tmp.v[2] = zero + 3;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 1; tmp.v[1] = zero + 3; tmp.v[2] = zero + 4;
	triangles.push_back(tmp);

// f 4 3 7
// f 4 7 8
	tmp.v[0] = zero + 4; tmp.v[1] = zero + 3; tmp.v[2] = zero + 7;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 4; tmp.v[1] = zero + 7; tmp.v[2] = zero + 8;
	triangles.push_back(tmp);

// f 8 7 6
// f 8 6 5
	tmp.v[0] = zero + 8; tmp.v[1] = zero + 7; tmp.v[2] = zero + 6;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 8; tmp.v[1] = zero + 6; tmp.v[2] = zero + 5;
	triangles.push_back(tmp);

// f 5 6 2
// f 5 2 1
	tmp.v[0] = zero + 5; tmp.v[1] = zero + 6; tmp.v[2] = zero + 2;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 5; tmp.v[1] = zero + 2; tmp.v[2] = zero + 1;
	triangles.push_back(tmp);

// f 2 6 7
// f 2 7 3
	tmp.v[0] = zero + 2; tmp.v[1] = zero + 6; tmp.v[2] = zero + 7;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 2; tmp.v[1] = zero + 7; tmp.v[2] = zero + 3;
	triangles.push_back(tmp);

// f 8 5 1
// f 8 1 4
	tmp.v[0] = zero + 8; tmp.v[1] = zero + 5; tmp.v[2] = zero + 1;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 8; tmp.v[1] = zero + 1; tmp.v[2] = zero + 4;
	triangles.push_back(tmp);




}


void chunk::dataToVBO()
{
	std::vector<vec3> base_vertexes;
	// std::vector<t_triangle> triangles;
	for (int z = 0; z < this->size_z; z++)
	{
		for (int y = 0; y < this->size_y; y++)
		{
			for (int x = 0; x < this->size_x; x++)
			{
				std::cout << "max = " << 256*32*32 << " me = " << x + y * this->size_x + z * this->size_x * this->size_y << std::endl;
				if (this->data[x + y * this->size_x + z * this->size_x * this->size_y] == 0)
					continue;

				
				int zero = base_vertexes.size();
				
				// if (!zero)
				// 	zero = 1;
				// std::cout << "create cube " << std::endl;
				createCube(x, y, z, zero, base_vertexes, triangles);

				for (;zero < base_vertexes.size(); zero++)
				{
					t_block_info tmp;
					tmp.pos = base_vertexes[zero];
					tmp.normal = 0;
					tmp.type = this->data[x + y * this->size_x + z * this->size_x * this->size_y];
					// std::cout << "tmp.pos : " << tmp.pos[0] << " " << tmp.pos[1] << " " << tmp.pos[2] << std::endl;
					this->toVBO.push_back(tmp);
				}
				
				// for (int i = 0; i < triangles.size(); i++)
				// {
				// 	std::cout << "triangles[" << i << "] : " << triangles[i].v[0] << " " << triangles[i].v[1] << " " << triangles[i].v[2] << std::endl;
				// }
			}
		}
	}
		
}

void chunk::setData(char *tmp)
{
	if (!tmp && tmp == this->data)
		return ;
	delete [] this->data;
	this->data = tmp;
}

char *chunk::getData()
{
	return this->data;
}


void* chunk::getToVBO()
{
	return &this->toVBO[0];
}

int chunk::memoryToVBO()
{
	std::cout << "sizeof(this->toVBO[0]) : " << this->toVBO.size() * sizeof(t_block_info) << std::endl;
	// std::cout << "sizeof(this->toVBO) : " << sizeof(this->toVBO.) << std::endl;

	return (this->toVBO.size() * sizeof(t_block_info));
}

void* chunk::getToEBO()
{
	return &this->triangles[0];
}

int chunk::memoryToEBO()
{
	return (this->triangles.size() * sizeof(t_triangle));
}