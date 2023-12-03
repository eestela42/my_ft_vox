#include "../../includes/classes/mesh.hpp"
// # include "../../includes/classes/vec3.hpp"

using namespace ee;

mesh::~mesh()
{}

mesh::mesh()
{}

mesh::mesh(const mesh &copy)
{
	*this = copy;
}

mesh::mesh(std::string file_name)
{
	std::cout << std::endl;

	std::cout << "--start extracting info from file" << std::endl;

	parseFromFile(file_name);

	std::cout << "--end extracting info from file" << std::endl;
	
	std::cout << std::endl;

	std::cout << "--start managing raw info from file" << std::endl;

	EcoDupVertex();
	center_around_orgin();

	std::cout << "--end managing raw info from file" << std::endl;

	std::cout << std::endl;

/*
	std::cout << "base_vertexes : 	" << base_vertexes.size() << std::endl;
	std::cout << "base_faces : 		" << base_faces.size() << std::endl;
	std::cout << "vertexes : 		" << vertexes.size() << std::endl;
	std::cout << "triangles : 		" << triangles.size() << std::endl;

	std::cout << std::endl;
*/
	std::cout << "--start managing data" << std::endl;

	addInfoToVertexesEco();

	std::cout << "--end managing data" << std::endl;


}


void	mesh::parseFromFile(std::string file_name)
{
	std::ifstream file(file_name);
    if (!file.is_open())
    {
        std::cout << "Error opening file : " << file_name << std::endl;
        exit(1);
    }

    std::string line;
	int count = 0;
    while (std::getline(file, line))
	{
		count++;
		if (line.size() <= 2)
			continue;
        if (line[0] == 'v' && line[1] == ' ')
        {
			bool point = false;
            glm::vec3 vert;
            std::string str[3];
            int i = 2;
			for (int pos = 0; pos < 3; pos++)
			{
				while (line[i] && line[i] == ' ')
					{i++;}
				if (line[i] && line[i] == '-')
					str[pos] += line[i++];
				while (line[i] && ((line[i] >= '0' && line[i] <= '9') || line[i] == '.' ))
				{
					if (line[i] == '.')
					{
						if (!point)
							point = true;
						else
						{
							std::cout << "Error line " << count << " : 2 points in a number" << std::endl;
							str[pos].clear();
							break ;
						}
					}
					str[pos] += line[i];
					i++;
				}
				point = false;
			}for (int i = 0; i < 32*32*255; i++)
	{
		// std::cout << (int)tmp[i] << " ";
		if (i % 32 == 0)
			std::cout << std::endl;
	}
			if (str[0].empty() || str[0] == "." || str[1].empty() || str[1] == "." || str[2].empty() || str[2] == ".")
			{
				std::cout << "Error line " << count << " : missing number" << std::endl;
			}
			else 
			{
				for (int pos = 0; pos < 3; pos++)
					vert[pos] = std::stof(str[pos]);
				this->base_vertexes.push_back(vert);
			}
            str[0].clear();
			str[1].clear();
			str[2].clear();
			
			
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
			std::vector<unsigned int>face;
			int i = 2;
			while(line[i] && line[i] == ' '){i++;}
			while (line[i])
			{
				std::string tmp;
				while(line[i] && line[i] <= '9' && line[i] >= '0')
				{
					tmp += line[i];
					i++;
				}
				if (!tmp.empty())
					face.push_back(std::stoi(tmp));
				while(line[i] && line[i] != ' '){i++;}
				while(line[i] && line[i] == ' '){i++;}
			}
			if (face.size() < 3)
			{
				std::cout << "Error line " << count << " : face with less than 3 vertexes" << std::endl;
			}
			else
				this->base_faces.push_back(face);
		}
	}

    file.close();
}


std::vector<t_triangle> mesh::facesToTriangles()
{
	std::vector<t_triangle> new_triangles;
	for (std::vector<std::vector<unsigned int>>::iterator it = this->base_faces.begin(); it != this->base_faces.end(); ++it)
	{
		if (it->size() < 3)
		{
			std::cout << "Error : face with less than 3 vertexes" << std::endl;
			exit(1);
		}

		std::size_t i = 2;
		while (i < it->size())
		{
			t_triangle triangle;
			triangle.v[0] = it->at(0) - 1;
			triangle.v[1] = it->at(i - 1) - 1;
			triangle.v[2] = it->at(i) - 1;
			new_triangles.push_back(triangle);
			i++;
		}
	}
	return (new_triangles);
}

void mesh::EcoDupVertex()
{
	std::vector<glm::vec3> new_vertexes/* = this->base_vertexes*/;
	std::vector<t_triangle> new_triangles;

	int i = 0;
	for (std::vector<std::vector<unsigned int>>::iterator it = this->base_faces.begin(); it != this->base_faces.end(); ++it)
	{
		if (it->size() < 3)
		{
			std::cout << "Error : face with less than 3 vertexes" << std::endl;
			exit(1);
		}

		//create triangle from the face
		std::vector<t_triangle> base_face_triangles;
		base_face_triangles = createTriangleFromFace(*it);

		


		//create triangles with shared vertexes by face
		std::vector<t_triangle>	new_face_triangles;
		int j = 0;
		for (std::vector<t_triangle>::iterator it2 = base_face_triangles.begin(); it2 != base_face_triangles.end(); ++it2)
		{
			new_face_triangles.push_back(getSemioptiVerticesTriangle(*it2, j, &new_vertexes, &base_face_triangles, &new_face_triangles));
			j++;
		}

		// get all vertexes of the face
		std::vector<unsigned int> tmp;
		for (std::size_t y = 0; y < new_face_triangles.size(); y++)
		{
			new_triangles.push_back(new_face_triangles[y]);
			for(int z = 0; z < 3; z++)
			{
				bool to_push = true;
				for (std::size_t x = 0; x < tmp.size(); x++)
				{
					if (tmp[x] == new_face_triangles[y].v[z])
					{
						to_push = false;
						break;
					}
				}
				if (to_push)
					tmp.push_back(new_face_triangles[y].v[z]);
			}
		}

		this->vertByFace.push_back(tmp);
		base_triangles.push_back(base_face_triangles);

		i++;
	}
	

	this->triangles = new_triangles;
	this->vertexes = new_vertexes;
}

t_triangle mesh::getSemioptiVerticesTriangle(t_triangle &triangle, std::size_t pos, std::vector<glm::vec3> *new_vertexes,
						std::vector<t_triangle> *base_face_triangles, std::vector<t_triangle> *new_face_triangles)
{
	t_triangle ret;
	bool found = false;
	for (int i = 0; i < 3; i++)
	{
		
		found = false;

		for (std::size_t y = 0; !found && y < pos && y < base_face_triangles->size(); y++)
		{
			for (int z = 0; !found && z < 3; z++)
			{

				if (base_face_triangles->at(y).v[z] == triangle.v[i])
					{
						ret.v[i] = new_face_triangles->at(y).v[z];
						found = true;
					}
			}
		}
		
		if (!found)
		{

			ret.v[i] = new_vertexes->size();
			//le probleme est la
			new_vertexes->push_back(this->base_vertexes[triangle.v[i]]);
		
		}
		
	}
	return (ret);
}



std::vector<t_triangle> mesh::createTriangleFromFace(std::vector<unsigned int> face)
{
	std::vector<t_triangle> new_triangles;
	std::size_t i = 2;
	while (i < face.size())
	{
		t_triangle triangle;
		triangle.v[0] = face[0] - 1;
		triangle.v[1] = face[i - 1] - 1;
		triangle.v[2] = face[i] - 1;
		new_triangles.push_back(triangle);
		i++;
	}
	return (new_triangles);
}

void mesh::addInfoToVertexesEco()
{
	std::vector<glm::vec3> new_vbo;
	glm::vec3 colors[] = {
		{0.1f, 0.1f, 0.1f}, // red
		{0.2f, 0.2f, 0.2f}, // green
		{0.3f, 0.3f, 0.3f}  // blue
	};
	glm::vec3 textureCoo[] = {
		{1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 1.0f}
	};


	int pos = 0;
	
	for (std::size_t face = 0; face < this->vertByFace.size(); face++)
	{
		for (std::size_t vertInFace = 0; vertInFace < this->vertByFace[face].size(); vertInFace++)
		{
			new_vbo.push_back(this->vertexes[this->vertByFace[face][vertInFace]]);
			float clor = (face % 50) * 0.01 ;
			glm::vec3 tmp = { clor, clor,clor};
			new_vbo.push_back(tmp);
			new_vbo.push_back(textureCoo[vertInFace % 4]);
			pos++;
		}
	}


	this->vertexes = new_vbo;
}

void mesh::min_max_bounds(glm::vec3& min_bound, glm::vec3& max_bound)
{
	for (std::vector<glm::vec3>::iterator vert = this->vertexes.begin(); vert != this->vertexes.end();)  {
		min_bound[0] = std::min(min_bound[0], (*vert)[0]);
		min_bound[1] = std::min(min_bound[1], (*vert)[1]);
		min_bound[2] = std::min(min_bound[2], (*vert)[2]);

		max_bound[0] = std::max(max_bound[0], (*vert)[0]);
		max_bound[1] = std::max(max_bound[1], (*vert)[1]);
		max_bound[2] = std::max(max_bound[2], (*vert)[2]);
		vert++;
	}
}

void	mesh::center_around_orgin()
{
	glm::vec3 min_bound = {FLT_MAX, FLT_MAX, FLT_MAX};
	glm::vec3 max_bound = {FLT_MIN, FLT_MIN, FLT_MIN};

	min_max_bounds(min_bound, max_bound);

	glm::vec3 center = {0, 0, 0};
	
	center[0] = (min_bound[0] + max_bound[0]) / 2;
	center[1] = (min_bound[1] + max_bound[1]) / 2;
	center[2] = (min_bound[2] + max_bound[2]) / 2;

	for (std::vector<glm::vec3>::iterator vert = this->vertexes.begin(); vert != this->vertexes.end(); ++vert)  {
		(*vert)[0] -= center[0];
		(*vert)[1] -= center[1];
		(*vert)[2] -= center[2];
	}
}


mesh &mesh::operator=(const mesh &copy)
{
	this->base_vertexes = copy.base_vertexes;
	this->base_faces = copy.base_faces;

	this->vertexes = copy.vertexes;
	this->triangles = copy.triangles;

	this->vertByFace = copy.vertByFace;
	this->base_triangles = copy.base_triangles;
	return (*this);
}

