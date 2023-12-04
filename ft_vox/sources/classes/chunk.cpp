#include "../../includes/header.hpp"
#include "../../includes/classes/chunk.hpp"

using namespace ee;

		/*****	1 - constructors 		*****/

chunk::~chunk()
{
	delete [] this->data;
	delete [] this->neighbours;
}

chunk::chunk() : pos_x(0), pos_y(0)
{
	this->data = new u_char[this->size_x * this->size_y * this->size_z];

	for (int i = 0; i < this->size_x * this->size_y * this->size_z; i++)
		this->data[i] = 0;

	this->neighbours = new ee::chunk*[4];
	for (int i = 0; i < 4; i++)
		this->neighbours[i] = NULL;

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


		/*****	2 - fillers	 	*****/

void chunk::fillFromData()
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

		/*****	3 - toVbo		 	*****/

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

void chunk::dataToVBO(std::vector<int> &vertexes, std::vector<unsigned int> &triangles)
{
	bool tab[this->size_x*this->size_y*this->size_z];
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
		tab[i] = false;

	int size_vertex = 4;
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
	{
		// if (this->data[i] != 0)
		// 	parkour( vertexes.size() / size_vertex, this->vertexes, this->triangles, tab, i);
	}
	

	vertexes.reserve(vertexes.size() + this->vertexes.size());
	vertexes.insert(vertexes.end(), this->vertexes.begin(), this->vertexes.end());

	triangles.reserve(triangles.size() + this->triangles.size());
	triangles.insert(triangles.end(), this->triangles.begin(), this->triangles.end());

}

std::vector<u_char>&	chunk::getNeighbourRuban(u_int neigh, u_int &x, u_int &y, bool &error)
{
	switch (neigh)
	{
	case 0:
		if (x == this->size_x - 1)
		{
			if (!this->neighbours[neigh])
			{
				error = true;
				return this->rle.getRubans();
			}
			x = 0;
			return this->neighbours[neigh]->getRle().getRubans();
		}
		x += 1;
		return this->rle.getRubans();
	case 1:
		if (x == 0)
		{
			if (!this->neighbours[neigh])
			{
				error = true;
				return this->rle.getRubans();
			}
			x = this->size_x - 1;
			return this->neighbours[neigh]->getRle().getRubans();
		}
		x -= 1;
		return this->rle.getRubans();
	case 2:
		if (y == this->size_y - 1)
		{
			if (!this->neighbours[neigh])
			{
				error = true;
				return this->rle.getRubans();
			}
			y = 0;
			return this->neighbours[neigh]->getRle().getRubans();
		}
		y += 1;
		return this->rle.getRubans();
	case 3:
		if (y == 0)
		{
			if (!this->neighbours[neigh])
			{
				error = true;
				return this->rle.getRubans();
			}
			y = this->size_y - 1;
			return this->neighbours[neigh]->getRle().getRubans();
		}
		y -= 1;
		return this->rle.getRubans();
	}
	return this->rle.getRubans();

}

void	chunk::createRleVertex(std::vector<u_int> &vertexes, int pos, u_char orientation, u_char type)
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

void	chunk::createFaces(u_int start, u_int end, u_int x, u_int y, u_int z, u_char type, u_int face, u_int start_vert,
							std::vector<u_int> &vertexes, std::vector<u_int> &triangles)
{
	for (u_int pos = start; pos < end; pos++)
	{
		createTrianglesFace(vertexes.size() / this->size_vertex, triangles);
		start_vert += 4;
		createRleVertex(vertexes, x + y * this->size_x + pos * this->size_x * this->size_y, face, type);
	}
}

void	adjust_neigbour_z(u_int &neigh_z, u_int &neigh_pos, u_int &neigh_over, u_int ruban_size, u_int pos)
{
	if (!neigh_over && (neigh_z + ruban_size) <= pos)
	{
		neigh_z += ruban_size;
		neigh_pos += 2;
	}
	else if (!neigh_over && (neigh_z + ruban_size) > pos)
	{
		neigh_over = pos - neigh_z;
		neigh_z = pos;
	}
	else
	{
		neigh_over -= ruban_size;
		if (neigh_over <= 0)
		{
			neigh_z += -neigh_over;
			neigh_pos += 2;
		}
	}
}

void	chunk::rleToVbo(std::vector<u_int> &out_vertexes, std::vector<u_int> &out_triangles)
{
	u_int start_vert = out_vertexes.size();

	u_int XYZ = 0;

	std::vector<u_char> &rubans = this->rle.getRubans();

	u_int					neigh_z[4] = {0, 0, 0, 0};
	u_int					neigh_pos[4] = {0, 0, 0, 0};
	u_int					neigh_over[4] = {0, 0, 0, 0};

	for (u_int pos = 0; pos < rubans.size(); pos += 2)
	{
		u_char type = rubans[pos];
		u_char size = rubans[pos + 1];
		u_int x = (XYZ / size_z) % size_x;
		u_int y = (XYZ / (size_z * size_x)) % size_y;
		u_int z = XYZ % size_z;

		if (type != 0)
		{
			for (u_int neigh = 0; neigh < 4; neigh++)// front right back left
			{
				u_int pos_ruban_x = x;
				u_int pos_ruban_y = y;
				bool error = false;
				std::vector<u_char> &neigh_ruban = getNeighbourRuban(neigh, pos_ruban_x, pos_ruban_y, error);
				if (error)
				{
					createFaces(z, z + size, x, y, z, 1, neigh, out_vertexes.size(),
								out_vertexes, out_triangles);
					continue;
				}
				// neigh_pos[neigh] = pos_ruban_x + pos_ruban_y * this->size_x;
				// neigh_over[neigh] = 0;
				// neigh_z[neigh] = 0;
				while (neigh_z[neigh] < z + size)
				{
					// std::cout << "z = " << z << " size = " << (u_int)size << " neigh_z[neigh] = " << neigh_z[neigh] << std::endl;
					u_int ruban_size = 0;
					ruban_size = neigh_ruban[neigh_pos[neigh] + 1];
					if (neigh_z[neigh] + ruban_size > z + size)
						ruban_size = z + size;

					if (neigh_ruban[neigh_pos[neigh]] == 0)
					{
						createFaces(neigh_z[neigh], neigh_z[neigh] + ruban_size, x, y, z, type, neigh, out_vertexes.size(),
								out_vertexes, out_triangles);
					}
					adjust_neigbour_z(neigh_z[neigh], neigh_pos[neigh], neigh_over[neigh], ruban_size, z + size);
					
					//increment neigh_pos
				}
			}
		}
		// std::cout << "XYZ = " << XYZ << " size = " << (u_int)size << std::endl;
		XYZ += size;
	}

}




		/*****	4 - getters	 	*****/

void chunk::setNeighbours(int index, ee::chunk* other)
{
	this->neighbours[index] = other;
}

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


