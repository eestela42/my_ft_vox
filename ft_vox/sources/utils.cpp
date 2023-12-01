#include "../includes/header.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
    glViewport(0, 0, width, height);
} 


 char* ft_get_file(std::string file_name)
{
	std::ifstream file;
	file.open(file_name);
	if (!file.is_open())
	{
		std::cout << "Error opening file : " << file_name << std::endl;
		exit(1);
	}

	std::string line;
	std::stringstream s1;


	s1  << file.rdbuf();
	file.close();
	line = s1.str();

	// std::cout << line.c_str() << std::endl;
	// std::cout << "YES\n";

	char *ret = new char[line.length() + 1];
	strcpy(ret, line.c_str());
	return ret;
}

