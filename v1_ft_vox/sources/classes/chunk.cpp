#include "../../includes/classes/chunk.hpp"
#include "../../includes/classes/vec3.hpp"

using namespace ee;

chunk::chunk() : pos_x(0), pos_y(0)
{
	this->data = new char[this->size_x * this->size_y * this->size_z];
}

chunk::chunk(int x, int y) : pos_x(x), pos_y(y)
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
	for (int z = 0; z < size_z / 2; z++)
	{
		for (int y = 0; y < size_y / 2; y++)
		{
			for (int x = 0; x < size_x / 2; x++)
			{
				this->data[x + y * this->size_x + z * this->size_x * this->size_y] = block_type[(x % 2 + y % 2 + z % 2) % 2];
			}
		}
	}
	std::cout << "out fill" << std::endl;
	
}


struct ThreeBitVariable {
    unsigned int value : 3;
};


void createCube(int x, int y, int z, int zero, std::vector<vec3> &base_vertexes, std::vector<t_triangle> &triangles)
{
	
	//bas
	base_vertexes.push_back({x,		y, 		z});
	base_vertexes.push_back({x, 	y+1, 	z});
	base_vertexes.push_back({x+1, 	y+1, 	z});
	base_vertexes.push_back({x+1, 	y, 		z});
	//haut
	base_vertexes.push_back({x, 	y, 		z+1});
	base_vertexes.push_back({x, 	y+1, 	z+1});
	base_vertexes.push_back({x+1, 	y+1, 	z+1});
	base_vertexes.push_back({x+1, 	y, 		z+1});

	t_triangle tmp;
	zero--;
	//dessous
	tmp.v[0] = zero + 1; tmp.v[1] = zero + 2; tmp.v[2] = zero + 3;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 1; tmp.v[1] = zero + 3; tmp.v[2] = zero + 4;
	triangles.push_back(tmp);

	//est
	tmp.v[0] = zero + 4; tmp.v[1] = zero + 3; tmp.v[2] = zero + 7;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 4; tmp.v[1] = zero + 7; tmp.v[2] = zero + 8;
	triangles.push_back(tmp);

	//haut
	tmp.v[0] = zero + 8; tmp.v[1] = zero + 7; tmp.v[2] = zero + 6;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 8; tmp.v[1] = zero + 6; tmp.v[2] = zero + 5;
	triangles.push_back(tmp);

	//ouest
	tmp.v[0] = zero + 5; tmp.v[1] = zero + 6; tmp.v[2] = zero + 2;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 5; tmp.v[1] = zero + 2; tmp.v[2] = zero + 1;
	triangles.push_back(tmp);

	//sud
	tmp.v[0] = zero + 2; tmp.v[1] = zero + 6; tmp.v[2] = zero + 7;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 2; tmp.v[1] = zero + 7; tmp.v[2] = zero + 3;
	triangles.push_back(tmp);

	//nord
	tmp.v[0] = zero + 8; tmp.v[1] = zero + 5; tmp.v[2] = zero + 1;
	triangles.push_back(tmp);
	tmp.v[0] = zero + 8; tmp.v[1] = zero + 1; tmp.v[2] = zero + 4;
	triangles.push_back(tmp);




}


void chunk::dataToVBO(std::vector<vec3> &base_vertexes, std::vector<t_triangle> &triangles, std::vector<t_block_info> &toVBO)
{
	;
	// std::vector<t_triangle> triangles;
	std::cout << "in tovbo" << std::endl;

	for (int z = 0; z < this->size_z; z++)
	{
		std::cout << "z" << std::endl;
		for (int y = 0; y < this->size_y; y++)
		{
		std::cout << "y" << std::endl;

			for (int x = 0; x < this->size_x; x++)
			{
				std::cout << "x : " << x << " y : " << y << " z : " << z << std::endl;
				if (this->data[x + y * this->size_x + z * this->size_x * this->size_y] == 0)
					continue;
			std::cout << "IN 1" << std::endl;
				if ( x != this->size_x - 1 && x != 0 && y !=size_y - 1 && y != 0 && z != size_z - 1 && z != 0 &&
					this->data[x - 1 + y * this->size_x + z * this->size_x * this->size_y] != 0 &&
					this->data[x + 1 + y * this->size_x + + z * this->size_x * this->size_y] != 0 &&
					this->data[x + (y - 1) * this->size_x - 1 + z * this->size_x * this->size_y] != 0 &&
					this->data[x + (y + 1) * this->size_x + 1 + z * this->size_x * this->size_y] != 0 &&
					this->data[x + y * this->size_x + 1 + (z - 1) * this->size_x * this->size_y] != 0 &&
					this->data[x + y * this->size_x + 1 + (z + 1) * this->size_x * this->size_y] != 0)
					continue;
			std::cout << "IN 2" << std::endl;
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
					tmp.type = this->data[x + y * this->size_x + z * this->size_x * this->size_y] - 1;
					// std::cout << "tmp.pos : " << tmp.pos[0] << " " << tmp.pos[1] << " " << tmp.pos[2] << std::endl;
					toVBO.push_back(tmp);
				}
				std::cout << "end" << std::endl;
				// for (int i = 0; i < triangles.size(); i++)
				// {
				// 	std::cout << "triangles[" << i << "] : " << triangles[i].v[0] << " " << triangles[i].v[1] << " " << triangles[i].v[2] << std::endl;
				// }
			}
		}
		std::cout << "z = " << z << std::endl;
	}
	std::cout << "out tovbo" << std::endl;	
	std::cout << std::endl;
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