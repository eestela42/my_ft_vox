#ifndef HEADER_HPP
# define HEADER_HPP


// # include "base_header.hpp"

# include "classes/mesh.hpp"
# include "classes/chunk.hpp"
# include "classes/game.hpp"



#include "stb_image.h"

# define STB_IMAGE_IMPLEMENTATION







ee::mesh		ft_parsing(std::string file_name);
char* ft_get_file(std::string file_name);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif
