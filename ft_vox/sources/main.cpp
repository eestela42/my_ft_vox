#include "../includes/header.hpp"
//g++ openGL.cpp -lglut -lGLU -lGL; ./a.out
#include "../includes/stb_image.h"

using namespace ee;

int zoom = 0;

double fps = 0.0;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                bool rotate = true;
bool color = true;

bool color_p = false;

glm::vec3 cameraPos = glm::vec3(-2.0f, 12.0f, 0.0f);

glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); 
glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));

glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);

float baseCameraSpeed = 0.5f;

float yaw = 0.0f;
float pitch = 0.0f;

glm::vec3 direction = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
								sin(glm::radians(pitch)),
								sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

bool firstMouse = true;
float lastX = 400, lastY = 300;


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw   += xoffset;
	pitch += yoffset; 

	if(pitch > 89.0f)
		pitch =  89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
	direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
	
}



void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
        glfwSetWindowShouldClose(window, true);
	}


	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		std::cout << "fps : " << fps << std::endl;

	float cameraSpeed = baseCameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed = cameraSpeed * 5;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		cameraSpeed = cameraSpeed / 2;


	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !color_p)
	{
		color = !color;
		color_p = !color_p;
	}
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && color_p)
		color_p = 0;

	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
       cameraPos += glm:: normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
}




int main(int ac, char** av)
{
	game game(1920, 1080);
	game.init();
	game.initShadder();

	game.initChunks();
	
	game.initBuffers();

	game.initTexture();


	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




	glBindVertexArray(game.getVAO()); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glUseProgram(game.getShaderProgram());




	// textures

	glUniform1i(glGetUniformLocation(game.getShaderProgram(), "Texture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, game.getTexture());

	glEnable(GL_DEPTH_TEST);

	// game.displayEBO();
	// game.displayVBO();


	glm::mat4 model(1.0f);

	float const_rota_time = 0.0f;
	float zoom_value = 1.0f;
	


 

	

	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	
	


	glm::mat4 projection;

	projection = glm::perspective(45.0f, (float)game.getScreenWidth() / (float)game.getScreenHeight(), 0.1f, 350.0f);
	
	


	const int target_fps = 60;
    const std::chrono::duration<double> target_frame_duration(1.0 / target_fps);
    std::chrono::high_resolution_clock clock;
    auto last_frame_time = clock.now();
    
	// glEnable(GL_CULL_FACE);  
	// glCullFace(GL_FRONT); 

	


	//draw only lines
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glfwSetInputMode(game.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(game.getWindow(), mouse_callback);

	

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

		processInput(game.getWindow());

		glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw our first triangle
		unsigned int modelCameraPos = glGetUniformLocation(game.getShaderProgram(), "cameraPos");
		glUniform3f(modelCameraPos, cameraPos[0], cameraPos[1], cameraPos[2]);

		unsigned int modelCameraFront = glGetUniformLocation(game.getShaderProgram(), "cameraFront");
		glUniform3f(modelCameraFront, cameraFront[0], cameraFront[1], cameraFront[2]);


		glm::mat4 modelView =  projection * view * model;
		unsigned int modelViewLoc = glGetUniformLocation(game.getShaderProgram(), "modelView");
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, &modelView[0][0]);

		glDrawElements(GL_TRIANGLES, game.getTriangles().size(), GL_UNSIGNED_INT, 0);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(game.getWindow());
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cout << "error : " << error << std::endl;
			break ;
		}
		
    } 

  
	return 0;
}