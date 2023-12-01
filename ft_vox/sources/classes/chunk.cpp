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
	for (int z = 0; z < 64; z++)
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


// void chunk::createVertex(std::vector<int> &vertexes, glm::vec3 pos)
// {
// 	pos.x += this->pos_x * this->size_x;
// 	pos.y += this->pos_y * this->size_y;
// 	vertexes.push_back(pos);
// 	vertexes.push_back(glm::vec3(0.0f));
// 	vertexes.push_back(glm::vec3(1.0f));
// }

void chunk::createPointVertex(std::vector<int> &vertexes, int pos, int orientation, int type)
{
	for (int i = 0; i < 4; i++)
	{
		vertexes.push_back(this->pos_x);
		vertexes.push_back(this->pos_y);
		vertexes.push_back(pos);
		vertexes.push_back(orientation);
		vertexes.push_back(type);
		vertexes.push_back(i);
	}
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

void chunk::parkour(int start_vert, std::vector<int> &vertexes,std::vector<unsigned int> &triangles, bool *tab, int pos)
{
	tab[pos] = true;
	int x = pos % this->size_x;
	int y = (pos / this->size_x) % this->size_y;
	int z = pos / (this->size_x * this->size_y);
	int size_vert = 6;
	if ( x == size_x - 1 || data[pos + 1] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vert, triangles);

		createPointVertex(vertexes, pos, 0, data[pos]);
	}
	if (x == 0 || data[pos - 1] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vert, triangles);

		createPointVertex(vertexes, pos, 1, data[pos]);

	}
	if (y == size_y - 1 || data[pos + this->size_x] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vert, triangles);

		createPointVertex(vertexes, pos, 2, data[pos]);
	}
	if (y == 0 || data[pos - this->size_x] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vert, triangles);

		createPointVertex(vertexes, pos, 3, data[pos]);
	}
	if (z == size_z - 1 || data[pos + this->size_x * this->size_y] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vert, triangles);

		createPointVertex(vertexes, pos, 4, data[pos]);
	}
	if (z == 0 || data[pos - this->size_x * this->size_y] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vert, triangles);

		createPointVertex(vertexes, pos, 5, data[pos]);
	}
	return;
}

void chunk::dataToVBO(std::vector<int> &vertexes, std::vector<unsigned int> &triangles)
{
	// std::vector<glm::vec3> vertexes;
	// std::vector<unsigned int> triangles;
	bool tab[this->size_x*this->size_y*this->size_z];
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
		tab[i] = false;
	std::cout << "start parkour " << this->size_x*this->size_y*this->size_z << std::endl;
	std::cout << "vertexes.size() : " << vertexes.size() << std::endl;
	std::cout << "triangles.size() : " << triangles.size() << std::endl;

	int size_vert = 6;
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
	{
		if (this->data[i] != 0)
			parkour( vertexes.size() / size_vert, this->vertexes, this->triangles, tab, i);
	}
	std::cout << "end parkour" << std::endl;

	vertexes.reserve(vertexes.size() + this->vertexes.size());
	vertexes.insert(vertexes.end(), this->vertexes.begin(), this->vertexes.end());

	triangles.reserve(triangles.size() + this->triangles.size());
	triangles.insert(triangles.end(), this->triangles.begin(), this->triangles.end());

	std::cout << "vertexes.size() : " << vertexes.size() / 6 << std::endl;
	std::cout << "triangles.size() : " << triangles.size() / 3 << std::endl;
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