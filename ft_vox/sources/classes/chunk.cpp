#include "../../includes/classes/chunk.hpp"

using namespace ee;

chunk::chunk() : pos_x(0), pos_y(0)
{
	this->data = new u_char[this->size_x * this->size_y * this->size_z];
	for (int i = 0; i < this->size_x * this->size_y * this->size_z; i++)
		this->data[i] = 0;
}

chunk::chunk(int x, int y) : pos_x(x), pos_y(y)
{
	this->data = new u_char[this->size_x * this->size_y * this->size_z];
	for (int i = 0; i < this->size_x * this->size_y * this->size_z; i++)
		this->data[i] = 0;
}

chunk::~chunk()
{
	delete [] this->data;
}

void chunk::fill()
{
	u_char block_type[2] = {(u_char)(pos_x % 255 + 1) , u_char(pos_y % 255 + 1) };
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

	this->data[size_x * size_y * 10 + 2 + size_x] = 1;
	// this->data[size_x] = 2;

}

void chunk::createPointVertex(std::vector<int> &vertexes, int pos, char orientation, char type)
{
	for (char i = 0; i < 4; i++)
	{
		vertexes.push_back(this->pos_x);
		vertexes.push_back(this->pos_y);
		vertexes.push_back(pos);
		int tmp = 0;
		tmp = (orientation<< 24) + (i << 8) + (type);
		vertexes.push_back(tmp);
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
	int size_vert = 4;
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
	bool tab[this->size_x*this->size_y*this->size_z];
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
		tab[i] = false;

	int size_vert = 4;
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
	{
		if (this->data[i] != 0)
			parkour( vertexes.size() / size_vert, this->vertexes, this->triangles, tab, i);
	}

	vertexes.reserve(vertexes.size() + this->vertexes.size());
	vertexes.insert(vertexes.end(), this->vertexes.begin(), this->vertexes.end());

	triangles.reserve(triangles.size() + this->triangles.size());
	triangles.insert(triangles.end(), this->triangles.begin(), this->triangles.end());

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



void chunk::setData(u_char *tmp)
{
	if (!tmp && tmp == this->data)
		return ;
	delete [] this->data;
	this->data = tmp;
}

u_char *chunk::getData()
{
	return this->data;
}



void* chunk::getToEBO()
{
	return &this->triangles[0];
}

int chunk::memoryToEBO()
{
	return (this->triangles.size() * sizeof(t_triangle));
}