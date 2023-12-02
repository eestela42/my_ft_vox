#include "../../includes/classes/RLE.hpp"

namespace ee
{
	RLE::~RLE()
	{
	}

	RLE::RLE()
	{
	}

	RLE::RLE(u_int x, u_int y, u_int z)
	{
		
	}


	void	RLE::createFromData(u_char *data)
	{
		this->rubans.resize(this->size_x * this->size_y);
		for (u_int x = 0; x < this->size_x; x++)
		{
			for (u_int y = 0; y < this->size_y; y++)
			{
				u_char type = data[x + y * this->size_x];
				u_int size = 0;
				for (u_int z = 0; z < this->size_z; z++)
				{
					if (data[x + y * this->size_x + z * this->size_x * this->size_y] != type)
					{
						// std::cout << std::endl <<  "type : " << (u_int)type << " size : " << (u_int)size << std::endl;
						this->rubans[x + y * this->size_x].push_back(type);
						this->rubans[x + y * this->size_x].push_back(size);
						size = 0;
						type = data[x + y * this->size_x + z * this->size_x * this->size_y];
					}
					size++;
				}
				// std::cout << std::endl <<  "END loop type : " << (u_int)type << " size : " << (u_int)size << std::endl;
				this->rubans[x + y * this->size_x].push_back(type);
				this->rubans[x + y * this->size_x].push_back(size - 1);
			}
		}
		
	}

	typedef struct s_face
	{
		u_char type;
		u_char x;
		u_char y;
		u_char z;
		u_char facing;
	}	t_face;





	void	makeFace(std::vector<u_char> &faces, u_char type, u_char x, u_char y, u_char z, u_char facing)
	{
		faces.push_back(type);
		faces.push_back(x);
		faces.push_back(y);
		faces.push_back(z);
		faces.push_back(facing);
		// std::cout << "make face" << " type " << (u_int)type << " x " << (u_int)x << " y " << (u_int)y << " z " << (u_int)z << " facing " << (u_int)facing << std::endl;
	}

	void	RLE::makeRowFaces(std::vector<u_char> &faces, u_int i, u_int end, std::vector<u_char> &ruban, u_int *pos, u_char facing)
	{
		u_int start = pos[1];
		// std::cout << "facing " << (u_int)facing << std::endl;
		while (pos[1] < end )
		{
			if (ruban[pos[0]] == 0)
			{
				pos[1] += ruban[pos[0] + 1]; //nbr of block in the ruban
				pos[0] += 2;				//pos in the ruban
				continue;
			}
			if (pos[1] >= start + ruban[pos[0] + 1])
			{
				start = pos[1];
				pos[0] += 2;
				continue;
			}
			u_char type = ruban[pos[0]];
			u_char x = i % size_x;
			u_char y = i / size_x;
			u_char z = pos[1];
			makeFace(faces, type, x, y, z, facing);
			pos[1] += 1;
		}
	}

	std::vector<u_char>	RLE::getVisibleFaces()
	{
		std::cout << "getVisibleFaces" << std::endl;
		std::vector<u_char> faces;
		for (u_int i = 0; i < this->rubans.size(); i++)
		{
			//keeps in [0] the ruban and in [0] the height (z value)
			u_int north[2] = {0, 0};
			u_int south[2] = {0, 0};
			u_int east[2] = {0, 0};
			u_int west[2] = {0, 0};

			u_int pos = 0;
			u_int next_pos = 0;
			for (u_int rub = 0; rub < this->rubans[i].size(); rub += 2)
			{
				
				if(this->rubans[i][rub] != 0)
				{
					makeFace(faces, this->rubans[i][rub], i % size_x, i / size_x, pos + this->rubans[i][rub+1], 0);
					pos += this->rubans[i][rub+1] + 1;
					continue;
				}
				if (rub != 0)
					makeFace(faces, this->rubans[i][rub-2], i % size_x, i / size_x, pos - 1, 1);
				
				next_pos = pos + this->rubans[i][rub+1];

				if (i + size_x < this->rubans.size())	{
					north[1] = pos;
					makeRowFaces(faces, i, next_pos, this->rubans[i + size_x], north, 2);
				}
				if (i - size_x < this->rubans.size())	{
					south[1] = pos;
					makeRowFaces(faces, i, next_pos, this->rubans[i - size_x], south, 3);
				}
				if (i + 1 < this->rubans.size())	{
					east[1] = pos;
					makeRowFaces(faces, i, next_pos, this->rubans[i + 1], east, 4);
				}
				if (i - 1 < this->rubans.size())	{
					west[1] = pos;
					makeRowFaces(faces, i, next_pos, this->rubans[i - 1], west, 5);
				}
				pos = next_pos;
			}
		}
		std::cout << std::endl << "faces size : " << faces.size() / 5 << std::endl;
		for (u_int i = 0; i < faces.size(); i += 5)
		{
			std::cout << "type : " << (u_int)faces[i] << " x : " << (u_int)faces[i+1] << " y : " << (u_int)faces[i+2] << " z : " << (u_int)faces[i+3] << " facing : " << (u_int)faces[i+4] << std::endl;
		}
		std::cout << "OUT getVisibleFaces" << std::endl ;
		return (faces);
	}


}