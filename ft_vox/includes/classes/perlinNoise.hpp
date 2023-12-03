#ifndef PERLINNOISE_HPP
#define PERLINNOISE_HPP

# include "../base_header.hpp"


namespace ee
{
class perlinNoise
{
	
private :

	std::vector<int> 				Permutation;

public:

		/*****	1 - constructors 		*****/

	~perlinNoise();
	perlinNoise();
	perlinNoise(const 				perlinNoise &copy);
	
		/*****	2 - noise makers	 	*****/

	float 							fade(float t);
	float 							lerp(float t, float a, float b);
	
	void							Shuffle(std::vector<int> &arrayToShuffle);
	std::vector<int> 				MakePermutation();
	
	glm::vec2 						GetConstantVector(int v);	
	
	float 							noise2d(float x, float y);

		/*****	3 - data getters 		*****/

	std::vector<int>& 				getPermutation();

		/*****	4 - operators 			*****/

	perlinNoise&					operator=(const perlinNoise &copy);
};

	
}




#endif