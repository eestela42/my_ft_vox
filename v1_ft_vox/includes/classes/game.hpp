#ifndef GAME_HPP
# define GAME_HPP

# include "../base_header.hpp"
// # include "vec3.hpp"
// # include "mat4.hpp"
// # include "mesh.hpp"
# include "../types.hpp"

#ifndef MESH_HPP
# include "mesh.hpp"
#endif

namespace ee
{

	


class game
{
private:
	int width;
	int height;
	bool isRunning;
	ee::mesh mesh;
	GLFWwindow *window;
	unsigned int VBO, VAO, EBO;
	unsigned int shaderProgram;
	unsigned int texture;
	ee::chunk *chunks;

	


public:
	std::vector<vec3> base_vertexes;
	std::vector<t_block_info> toVBO;
	std::vector<t_triangle> triangles;
	~game();
	game(/* args */);
	game(int width, int height);

	int 			init();
	int 			initShadder();
	int				initBuffers();
	int				initTexture();
	void 			gameLoop();



	unsigned int 	getVAO();
	unsigned int 	getVBO();
	void			upDateVBO(std::vector<ee::vec3> tmp);
	unsigned int 	getEBO();
	unsigned int 	getTexture();

	unsigned int 	getShaderProgram();

	bool	 		getIsRunning();
	void 			updateIsRunning(bool tmp);

	void 			chunkToVBO();
	ee::chunk& 			getChunk();

	ee::mesh 			getMesh();
	void 			upDateMesh(ee::mesh tmp);

	GLFWwindow* 	getWindow();
	void 			upDateWindow(GLFWwindow *tmp);

	float* 			vbo_to_vertexes();
	unsigned int* 	ebo_to_indices();

	

	void			setBool(const std::string &name, bool value) const;
	void 			setInt(const std::string &name, int value) const;
	void			setFloat(const std::string &name, float value) const;

	void 			displayEBO();
	void 			displayVBO();
};
}






#endif