#include "../../includes/header.hpp"
#include "../../includes/classes/game.hpp"

using namespace ee;

		/*****	1 - constructors 		*****/

game::~game()
{
	glfwDestroyWindow(this->window);

	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteTextures(1, &this->texture);
	glDeleteProgram(this->shaderProgram);

	delete[] this->chunks;
	
	glfwTerminate();
}

game::game()
{
	this->screenWidth = 1920;
	this->screenHeight = 1080;
	this->isRunning = false;
	this->window = NULL;
	this->mesh = ee::mesh();
}

game::game(int width, int height): game()
{
	this->screenWidth = width;
	this->screenHeight = height;
}

		/*****	 2 - initialisations 	*****/

int game::init()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(this->screenWidth, this->screenHeight, "SCOP LE PROJET COOL", NULL, NULL);
	if (this->window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 2;
	}
	glfwMakeContextCurrent(this->window);
	this->updateIsRunning(true);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 3;
	}
	glViewport(0, 0, this->screenWidth, this->screenHeight);
	glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
	int monitor = 0;

	glfwSetWindowMonitor(this->window, glfwGetMonitors(&monitor)[monitor - 1], 0, 0, this->screenWidth, this->screenHeight, GLFW_DONT_CARE);
	
	std::cout << "Start time " << glfwGetTime() << std::endl;
	return (0);
}

int game::initChunks()
{
	int size = 10;
	this->chunks = new ee::chunk[size * size];
	
	ee::RLE bp;
	
	
	// bp.print();
	std::srand(std::time(nullptr));

	
	
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{	
			std::cout << "x = " << x << " y = " << y
						<< " time " << glfwGetTime() << std::endl;
			std::srand(glfwGetTime() * 10000);
			bp.generateChunk(x, y, std::rand() / RAND_MAX);
			this->chunks[x + y * size].setPos(x, y);
			
			this->chunks[x + y * size].setRle(bp);

			this->chunks[x + y * size].setData(bp.createDataFromRle());

			this->chunks[x + y * size].rleToVbo(this->vertexes, this->triangles);
		}
	}
	return (0);
}

int game::initShadder()
{
	/*Vertex Shader*/
	const char *vertexShaderSource = ft_get_file("shaders/vertex_shader.glsl");

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);


	


	if (!vertexShader)
	{
		std::cout << "vertex shadder fail" << std::endl;
		return 1;
	}
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);
		std::cout << "vertexShader shadder fail : ";
		for (int i = 0; i < maxLength; i++)
		{
			std::cout << errorLog[i];
		}
		std::cout << std::endl;
		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(vertexShader); // Don't leak the shader.
		exit(1);
	}

	/*Fragment Shader*/
	const char *fragmentShaderSource = ft_get_file("shaders/fragment_shader.glsl");
	
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fragmentShader)
	{
		std::cout << "fragmentShader shadder fail" << std::endl;
		return 2;
	}
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	/*Shader Program Finalisation*/
	
	this->shaderProgram = glCreateProgram();
	if (!this->shaderProgram)
	{
		std::cout << "this->shaderProgram shadder fail" << std::endl;
		return 3;
	}

	glAttachShader(this->shaderProgram, vertexShader);
	glAttachShader(this->shaderProgram, fragmentShader);
	glLinkProgram(this->shaderProgram);
	glUseProgram(this->shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);  

	return (0);
}

int game::initBuffers()
{
	
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
	
	
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertexes.size() * sizeof(int), &(this->vertexes[0]), GL_STATIC_DRAW);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->triangles.size() * sizeof(u_int), &(this->triangles[0]), GL_STATIC_DRAW);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
	glVertexAttribIPointer(0, 1, GL_INT,  3 * sizeof(int), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(1, 1, GL_INT,  3 * sizeof(int), (void*)(1 * sizeof(int)));
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(2, 1, GL_INT,  3 * sizeof(int), (void*)(2 * sizeof(int)));
	glEnableVertexAttribArray(2);
	
	
	
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

	
	return (0);
}

int game::initTexture()
{


	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	// Load the image using stb_image
	unsigned char *data = stbi_load("../resources/textures.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		// Determine the format based on the number of channels
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		else
		{
			std::cout << "Unsupported number of channels: " << nrChannels << std::endl;
			stbi_image_free(data);
			return -1;
		}

		// Set the pixel storage mode
		// glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Upload the texture data to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// Free the stb_image data
	stbi_image_free(data);

	return 0;
}

		/*****	3 - gameLoop 			*****/

void game::gameLoop()
{	
    while (this->isRunning)
    {
	
	}	
}

		/*****	4 - data getters 		*****/

ee::chunk*						game::getChunks()
{
	return (this->chunks);
}

std::vector<int> 				game::getVertexes()
{
	return (this->vertexes);
}

std::vector<unsigned int> 		game::getTriangles()
{
	return (this->triangles);
}

		/*****	5 - openGlIDs getters 	*****/

unsigned int game::getVAO()
{
	return (this->VAO);
}

unsigned int game::getVBO()
{
	return (this->VBO);
}

unsigned int game::getEBO()
{
	return (this->EBO);
}

unsigned int game::getTexture()
{
	return (this->texture);
}

unsigned int game::getShaderProgram()
{
	return (this->shaderProgram);
}

		/*****	6 - openGlIDs setters 	*****/

bool game::getIsRunning()
{
	return (this->isRunning);
}

void game::updateIsRunning(bool tmp)
{
	this->isRunning = tmp;
}

mesh game::getMesh()
{
	return (this->mesh);
}


GLFWwindow* game::getWindow()
{
	return (this->window);
}

void game::upDateWindow(GLFWwindow* tmp)
{
	this->window = tmp;
}

		/*****	7 - openGlIDs setters 	*****/

void game::setUniformBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(this->shaderProgram, name.c_str()), (int)value); 
}
void game::setUniformInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(this->shaderProgram, name.c_str()), value); 
}
void game::setUniformFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(this->shaderProgram, name.c_str()), value); 
}


		/*****	8 - screen getters 	*****/

int game::getScreenWidth()
{
	return (this->screenWidth);
}

int game::getScreenHeight()
{
	return (this->screenHeight);
}




