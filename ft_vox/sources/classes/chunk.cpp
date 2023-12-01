#include "../../includes/classes/chunk.hpp"

using namespace ee;

chunk::chunk() : pos_x(0), pos_y(0)
{
	std::cout << "chunk()" << std::endl;
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
	for (int i = 0; i < this->size_x * this->size_y * this->size_z; i++)
		this->data[i] = 0;
	char block_type[2] = {1, 2};
	for (int z = 0; z < 10; z++)
	{
		for (int y = 0; y < size_y; y++)
		{
			for (int x = 0; x < size_x; x++)
			{
				this->data[x + y * this->size_x + z * this->size_x * this->size_y] = block_type[(x % 2 + y % 2 + z % 2) % 2];
			}
		}
	}
	this->data[0] = 1;
}

/*
	ma pos = true
	si next_pos = block -> create face
	else ->  go


*/
void chunk::createVertex(std::vector<glm::vec3> &vertexes, glm::vec3 pos, char type)
{
	pos.x += this->pos_x * this->size_x;
	pos.y += this->pos_y * this->size_y;
	vertexes.push_back(pos);
	vertexes.push_back(glm::vec3(0.0f));
	if (type == 1)
		vertexes.push_back(glm::vec3(0.0f));
	else
		vertexes.push_back(glm::vec3(1.0f));
}

void chunk::createTrianglesFace(int start, std::vector<unsigned int> &triangles)
{
	triangles.push_back(start + 0);
	triangles.push_back(start + 1);
	triangles.push_back(start + 2);


	triangles.push_back(start + 0);
	triangles.push_back(start + 2);
	triangles.push_back(start + 3);


}

void chunk::parkour(std::vector<glm::vec3> &vertexes,std::vector<unsigned int> &triangles, bool *tab, int pos)
{
	tab[pos] = true;
	int x = pos % this->size_x;
	int y = (pos / this->size_x) % this->size_y;
	int z = pos / (this->size_x * this->size_y);
	char type = this->data[pos];
	if ( x == size_x - 1 || data[pos + 1] == 0)
	{
		createTrianglesFace(vertexes.size() / 3, triangles);

		createVertex(vertexes, glm::vec3(x + 1, y, z), type);
		createVertex(vertexes, glm::vec3(x + 1, y + 1, z), type);
		createVertex(vertexes, glm::vec3(x + 1, y + 1, z + 1), type);
		createVertex(vertexes, glm::vec3(x + 1, y, z + 1), type);
	}
	if (x == 0 || data[pos - 1] == 0)
	{
		createTrianglesFace(vertexes.size() / 3, triangles);

		createVertex(vertexes, glm::vec3(x, y, z), type);
		createVertex(vertexes, glm::vec3(x, y + 1, z), type);
		createVertex(vertexes, glm::vec3(x, y + 1, z + 1), type);
		createVertex(vertexes, glm::vec3(x, y, z + 1), type);

	}
	if (y == size_y - 1 || data[pos + this->size_x] == 0)
	{
		createTrianglesFace(vertexes.size() / 3, triangles);

		createVertex(vertexes, glm::vec3(x, y + 1, z), type);
		createVertex(vertexes, glm::vec3(x + 1, y + 1, z), type);
		createVertex(vertexes, glm::vec3(x + 1, y + 1, z + 1), type);
		createVertex(vertexes, glm::vec3(x, y + 1, z + 1), type);
	}
	if (y == 0 || data[pos - this->size_x] == 0)
	{
		createTrianglesFace(vertexes.size() / 3, triangles);

		createVertex(vertexes, glm::vec3(x, y, z), type);
		createVertex(vertexes, glm::vec3(x + 1, y, z), type);
		createVertex(vertexes, glm::vec3(x + 1, y, z + 1), type);
		createVertex(vertexes, glm::vec3(x, y, z + 1), type);
	}
	if (z == size_z - 1 || data[pos + this->size_x * this->size_y] == 0)
	{
		createTrianglesFace(vertexes.size() / 3, triangles);

		createVertex(vertexes, glm::vec3(x, y, z + 1), type);
		createVertex(vertexes, glm::vec3(x + 1, y, z + 1), type);
		createVertex(vertexes, glm::vec3(x + 1, y + 1, z + 1), type);
		createVertex(vertexes, glm::vec3(x, y + 1, z + 1), type);
	}
	if (z == 0 || data[pos - this->size_x * this->size_y] == 0)
	{
		createTrianglesFace(vertexes.size() / 3, triangles);

		createVertex(vertexes, glm::vec3(x, y, z), type);
		createVertex(vertexes, glm::vec3(x + 1, y, z), type);
		createVertex(vertexes, glm::vec3(x + 1, y + 1, z), type);
		createVertex(vertexes, glm::vec3(x, y + 1, z), type);
	}
	return;
}

void chunk::dataToVBO(std::vector<glm::vec3> &vertexes, std::vector<unsigned int> &triangles)
{
	// std::vector<glm::vec3> vertexes;
	// std::vector<unsigned int> triangles;
	bool tab[this->size_x*this->size_y*this->size_z];
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
		tab[i] = false;
	std::cout << "start parkour " << this->size_x*this->size_y*this->size_z << std::endl;
	std::cout << "vertexes.size() : " << vertexes.size() << std::endl;
	std::cout << "triangles.size() : " << triangles.size() << std::endl;
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
	{
		if (this->data[i] != 0)
			parkour(vertexes, triangles, tab, i);
	}
	std::cout << "end parkour" << std::endl;
	this->vertexes = vertexes;
	this->triangles = triangles;
	std::cout << "vertexes.size() : " << vertexes.size() << std::endl;
	std::cout << "triangles.size() : " << triangles.size() << std::endl;
}


/**getter and setters**/

void chunk::setPos(int x, int y)
{
	this->pos_x = x;
	this->pos_y = y;
}

int chunk::getPos_x()
{
	return this->pos_x;
}

int chunk::getPos_y()
{
	return this->pos_y;
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