#include "../../includes/classes/perlinNoise.hpp"

using namespace ee;

perlinNoise::~perlinNoise()
{}

perlinNoise::perlinNoise()
{
	this->Permutation = MakePermutation();
}

perlinNoise::perlinNoise(const perlinNoise &copy)
{
	*this = copy;
}

perlinNoise &perlinNoise::operator=(const perlinNoise &copy)
{
	if (this != &copy)
	{
		*this = copy;
	}
	return (*this);
}

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

void	Shuffle(std::vector<int> &arrayToShuffle) {
	for(int e = arrayToShuffle.size()-1; e > 0; e--) {
		int index = std::round(((float)std::rand()/(float)RAND_MAX) * (e-1));
		int temp = arrayToShuffle[e];
		// std::cout << "index : " << index  << std::endl;
		// std::cout << "div = " << ((float)rand/(float)RAND_MAX) << std::endl;
		// std::cout << "mult = " << (rand/RAND_MAX)*(e-1) << std::endl;
		arrayToShuffle[e] = arrayToShuffle[index];
		arrayToShuffle[index] = temp;
	}
}


std::vector<int>  perlinNoise::MakePermutation() {
	std::vector<int> permu;
	for(int i = 0; i < 256; i++) {
		permu.push_back(i);
	}
	Shuffle(permu);

	for(int i = 0; i < 256; i++) {
		permu.push_back(permu[i]);
	}
	
	return permu;
}

glm::vec2 GetConstantVector(int v) {
	// v is the value from the permutation table
	const int h = v & 3;
	if(h == 0)
		return  glm::vec2(1.0, 1.0);
	else if(h == 1)
		return  glm::vec2(-1.0, 1.0);
	else if(h == 2)
		return  glm::vec2(-1.0, -1.0);
	else
		return  glm::vec2(1.0, -1.0);
}


float perlinNoise::noise2d(float x, float y)
{
	std::srand(std::time(nullptr));
	Permutation = MakePermutation();
	// std::cout << "noise2d" << std::endl;
	x = x  * 0.2654f;
	y = y  * 0.2654f;	
	int xi = (int)std::floor(x) % 255;
	int yi =  (int)std::floor(y) % 255;

	float xf = x - (int)std::floor(x);
	float yf = y - (int)std::floor(y);

	// std::cout << "x = " << x << " y = " << y << std::endl;

	// std::cout << "xi = " << xi << " yi = " << yi << std::endl;
	// std::cout << "xf = " << xf << " yf = " << yf << std::endl;

	glm::vec2 topRight = glm::vec2(xf-1.0, yf-1.0);
	glm::vec2 topLeft =  glm::vec2(xf, yf-1.0);
	glm::vec2 bottomRight =  glm::vec2(xf-1.0, yf);
	glm::vec2 bottomLeft =  glm::vec2(xf, yf);

	// std::cout << "topRight = " << topRight.x << " " << topRight.y << std::endl;
	// std::cout << "topLeft = " << topLeft.x << " " << topLeft.y << std::endl;
	// std::cout << "bottomRight = " << bottomRight.x << " " << bottomRight.y << std::endl;
	// std::cout << "bottomLeft = " << bottomLeft.x << " " << bottomLeft.y << std::endl;

	int valueTopRight = Permutation[Permutation[xi+1]+yi+1];
	int valueTopLeft = Permutation[Permutation[xi]+yi+1];
	int valueBottomRight = Permutation[Permutation[xi+1]+yi];
	int valueBottomLeft = Permutation[Permutation[xi]+yi];

	// std::cout << "valueTopRight = " << valueTopRight << std::endl;
	// std::cout << "valueTopLeft = " << valueTopLeft << std::endl;
	// std::cout << "valueBottomRight = " << valueBottomRight << std::endl;
	// std::cout << "valueBottomLeft = " << valueBottomLeft << std::endl;

	float  dotTopRight = glm::dot(topRight, GetConstantVector(valueTopRight));
	float  dotTopLeft = glm::dot(topLeft, GetConstantVector(valueTopLeft));
	float  dotBottomRight = glm::dot(bottomRight, GetConstantVector(valueBottomRight));
	float  dotBottomLeft = glm::dot(bottomLeft, GetConstantVector(valueBottomLeft));

	// std::cout << "dotTopRight = " << dotTopRight << std::endl;
	// std::cout << "dotTopLeft = " << dotTopLeft << std::endl;
	// std::cout << "dotBottomRight = " << dotBottomRight << std::endl;
	// std::cout << "dotBottomLeft = " << dotBottomLeft << std::endl;

	float u = fade(xf);
	float v = fade(yf);



	// std::cout << "u = " << u << " v = " << v  << " ret = " << lerp(u,
	// 	lerp(v, dotBottomLeft, dotTopLeft),
	// 	lerp(v, dotBottomRight, dotTopRight)) << std::endl;

	return lerp(u,
		lerp(v, dotBottomLeft, dotTopLeft),
		lerp(v, dotBottomRight, dotTopRight));
}
