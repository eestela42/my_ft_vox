#include "../../includes/header.hpp"
#include "../../includes/classes/chunk.hpp"

using namespace ee;

chunk::~chunk()
{
	delete [] this->data;
}

chunk::chunk() : pos_x(0), pos_y(0)
{
	this->data = new u_char[this->size_x * this->size_y * this->size_z];
	for (int i = 0; i < this->size_x * this->size_y * this->size_z; i++)
		this->data[i] = 0;
	rle.createFromData(this->data);
}

chunk::chunk(int x, int y) : chunk()
{
	pos_x = x;
	pos_y = y;
}

chunk::chunk(int x, int y, ee::RLE &other) : chunk()
{
	pos_x = x;
	pos_y = y;
	this->rle = other;
}

chunk::chunk(float seed): chunk()
{
	this->rle = RLE(pos_x, pos_y, seed);
}

chunk::chunk(int x, int y, float seed): chunk()
{
	pos_x = x;
	pos_y = y;
	this->rle = RLE(pos_x, pos_y, seed);
}


void chunk::fill()
{
	ee::perlinNoise noise = ee::perlinNoise();

	u_char block_type[2] = {(u_char)(pos_x % 255 + 1) , u_char(pos_y % 255 + 1) };
	for (int y = 0; y < size_y; y++)
	{
		for (int x = 0; x < size_x; x++)
		{
			int h = std::abs(noise.noise2d(x, y) * 5) + 10;
			std::cout << h << std::endl;
			for (int z = 0; z < h; z++)
			{
			this->data[x + y * this->size_x + z * this->size_x * this->size_y] = block_type[(x % 2 + y % 2 + z % 2) % 2];
			}
		}
	}

	this->data[size_x * size_y * 10 + 2 + size_x] = 1;

	this->data[2 + size_y * size_x * 15] = 2;
	rle.createFromData(this->data);

}


void chunk::createPointVertex(std::vector<int> &vertexes, int pos, u_char orientation, u_char type)
{
	for (char i = 0; i < 4; i++)
	{
		u_int data[3];
		for (int i = 0; i < 3; i++)
			data[i] = 0;

		data[0] = this->pos_x << 8;
		data[0] += (this->pos_y >> 16) & 0x000000FF;
		data[1] = (this->pos_y & 0x0000FFFF) << 16;
		data[1] += (pos >> 2) & 0x0000FFFF;
		int tmp = 0;
		tmp = (orientation << 8) + type;
		data[2] = tmp << 14;
		data[2] += pos << 30;
		vertexes.push_back(data[0]);
		vertexes.push_back(data[1]);
		vertexes.push_back(data[2]);
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
	if ( x == size_x - 1 || data[pos + 1] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		createPointVertex(vertexes, pos, 0, data[pos]);
	}
	if (x == 0 || data[pos - 1] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		createPointVertex(vertexes, pos, 1, data[pos]);

	}
	if (y == size_y - 1 || data[pos + this->size_x] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		createPointVertex(vertexes, pos, 2, data[pos]);
	}
	if (y == 0 || data[pos - this->size_x] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		createPointVertex(vertexes, pos, 3, data[pos]);
	}
	if (z == size_z - 1 || data[pos + this->size_x * this->size_y] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		createPointVertex(vertexes, pos, 4, data[pos]);
	}
	if (z == 0 || data[pos - this->size_x * this->size_y] == 0)
	{
		createTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		createPointVertex(vertexes, pos, 5, data[pos]);
	}
	return;
}

void	chunk::rleToVbo(std::vector<int> &out_vertexes, std::vector<unsigned int> &out_triangles)
{
	bool tab[this->size_x*this->size_y*this->size_z];
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
		tab[i] = false;

	int XY = 0;
	for (std::vector<std::vector<u_char>>::iterator ruban = this->rle.rubans.begin(); ruban != this->rle.rubans.end(); ruban++)
	{
		int zero = 0;
		for (int i = 0; i < (*ruban).size(); i += 2)
		{
			int pos = XY % size_x + (XY / size_x) * size_y;
			if ((*ruban)[i] == 0)
				continue;
			for (int z = 0; z < (*ruban)[i + 1]; z++)
			{
				parkour( out_vertexes.size() / size_vertex, this->vertexes, this->triangles, tab,
					pos + (zero + z) * (size_x * size_y));
			}
			zero += (*ruban)[i + 1];
		}
		XY++;
	}

	out_vertexes.reserve(out_vertexes.size() + this->vertexes.size());
	out_vertexes.insert(out_vertexes.end(), this->vertexes.begin(), this->vertexes.end());

	out_triangles.reserve(out_triangles.size() + this->triangles.size());
	out_triangles.insert(out_triangles.end(), this->triangles.begin(), this->triangles.end());

}

void chunk::dataToVBO(std::vector<int> &vertexes, std::vector<unsigned int> &triangles)
{
	bool tab[this->size_x*this->size_y*this->size_z];
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
		tab[i] = false;

	int size_vertex = 4;
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
	{
		if (this->data[i] != 0)
			parkour( vertexes.size() / size_vertex, this->vertexes, this->triangles, tab, i);
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



bool chunk::setData(u_char *tmp)
{
	if (!tmp || tmp == this->data)
		return false;

	delete [] this->data;
	this->data = tmp;
	return true;
}

u_char* chunk::getData()
{
	return this->data;
}

void	chunk::setRle(ee::RLE &other)
{
	this->rle = other;
}

ee::RLE&	chunk::getRle()
{
	return this->rle;
}


void* chunk::getToEBO()
{
	return &this->triangles[0];
}

