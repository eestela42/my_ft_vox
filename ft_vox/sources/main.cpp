#include "../includes/header.hpp"
//g++ openGL.cpp -lglut -lGLU -lGL; ./a.out
#include "../includes/stb_image.h"

using namespace ee;

int zoom = 0;

double fps = 0.0;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                bool rotate = true;
bool color = true;

bool color_p = false;

glm::vec3 cameraPos = glm::vec3(-2.0f, 0.0f, 0.0f);


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
        glfwSetWindowShouldClose(window, true);
	}


	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		std::cout << "fps : " << fps << std::endl;

	zoom = 0;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		zoom = -1;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		zoom = 1;


	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !color_p)
	{
		color = !color;
		color_p = !color_p;
	}
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && color_p)
		color_p = 0;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos.x += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos.x -= 0.1f;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos.z -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos.z += 0.1f;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos.y -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos.y += 0.1f;
}




int main(int ac, char** av)
{
	
	game game(1200, 800);
	game.init();

	game.initShadder();
	
	game.initBuffers();

	// game.initTexture();

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




	glBindVertexArray(game.getVAO()); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glUseProgram(game.getShaderProgram());




	//textures

	glUniform1i(glGetUniformLocation(game.getShaderProgram(), "ourTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, game.getTexture());

	glEnable(GL_DEPTH_TEST);

	game.setBool("isColor", true);
	


	// game.displayEBO();
	// game.displayVBO();


	glm::mat4 model(1.0f);

	float const_rota_time = 0.0f;
	float zoom_value = 1.0f;
	


 

	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); 
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));

	glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	
	


	glm::mat4 projection;

	projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	
	


	const int target_fps = 60;
    const std::chrono::duration<double> target_frame_duration(1.0 / target_fps);
    std::chrono::high_resolution_clock clock;
    auto last_frame_time = clock.now();
    
	// glEnable(GL_CULL_FACE);  
	// glCullFace(GL_BACK); 

	// std::cout << "in main vertexes : " << std::endl;
	// for (int i = 0; i < game.vertexes.size(); i+=3)
	// 	std::cout << game.vertexes[i][0] << " " << game.vertexes[i][1] << " " << game.vertexes[i][2] << "  - ";
	// std::cout << std::endl << "triangles : " << std::endl;
	// for (int i = 0; i < game.triangles.size(); i+=3)
	// 	std::cout << game.triangles[i] << " " << game.triangles[i+1] << " " << game.triangles[i+2] << " - ";
	// std::cout << std::endl;

	


	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

 	while(!glfwWindowShouldClose(game.getWindow()))
	{
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//FPS
		auto current_time = clock.now();
        std::chrono::duration<double> frame_duration = current_time - last_frame_time;
        last_frame_time = current_time;

        // Calculate frames per second
        fps = 1.0 / frame_duration.count();


		glfwPollEvents();
		game.setFloat("zoom", zoom_value);
		/****matrixes****/
		
		

		if (zoom)
		{
			if (zoom_value + zoom * 0.1f > 0.0f)
				zoom_value += zoom * 0.1f;
			std::cout << "zoom : " << zoom_value << std::endl;
		}
		if (color)
			game.setBool("isColor", true);
		else
			game.setBool("isColor", false);


		/****matrixes END****/

		processInput(game.getWindow());

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw our first triangle
		glm::mat4 modelView =  projection * view * model;
		unsigned int modelViewLoc = glGetUniformLocation(game.getShaderProgram(), "modelView");
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, &modelView[0][0]);

		glDrawElements(GL_TRIANGLES, game.triangles.size(), GL_UNSIGNED_INT, 0);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(game.getWindow());
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cout << "error : " << error << std::endl;
		}
		
    } 

  
	return 0;
}