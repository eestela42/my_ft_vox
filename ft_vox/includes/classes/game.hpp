#ifndef GAME_HPP
# define GAME_HPP

# include "../base_header.hpp"
# include "../types.hpp"



namespace ee
{
class game
{
	
private:

	int 							screenWidth;
	int 							screenHeight;

	bool 							isRunning;
	
	GLFWwindow*						window;
	unsigned int 					VBO, VAO, EBO;
	unsigned int 					shaderProgram;
	
	ee::mesh 						mesh;
	unsigned int 					texture;
	
	ee::chunk*						chunks;
	
	
	int 							size_vertex = 3;	
	u_int 							size_x = 16;
	u_int 							size_y = 16;
	u_int 							size_z = 255;
	
	std::vector<u_int> 				vertexes;
	std::vector<u_int> 				triangles;
	
	
public:
	
		/*****	1 - constructors 		*****/

	~game();
	game(/* args */);
	game(int width, int height);

		/*****	 2 - initialisations 	*****/

	int 							init();
	int 							initShadder();
	int								initBuffers();
	int								initTexture();

	int								initChunks();

		/*****	3 - gameLoop 			*****/

	void 							gameLoop();

		/*****	4 - data getters 		*****/

	ee::chunk*						getChunks();
	std::vector<u_int> 				getVertexes();
	std::vector<u_int> 		getTriangles();

		/*****	5 - openGlIDs getters 	*****/

	unsigned int 					getVAO();
	unsigned int 					getVBO();
	unsigned int 					getEBO();

	unsigned int 					getShaderProgram();

	unsigned int 					getTexture();

		/*****	6 - variables getters	*****/

	bool	 						getIsRunning();
	void 							updateIsRunning(bool tmp);

	ee::mesh 						getMesh();
	void 							upDateMesh(ee::mesh tmp);

	GLFWwindow* 					getWindow();
	void 							upDateWindow(GLFWwindow *tmp);


		/*****	7 - uniforms setters 	*****/

	void							setUniformBool(const std::string &name, bool value) const;
	void 							setUniformInt(const std::string &name, int value) const;
	void							setUniformFloat(const std::string &name, float value) const;

		/*****	8 - screen getters 	*****/
	int 							getScreenWidth();
	int 							getScreenHeight();
};
}






#endif