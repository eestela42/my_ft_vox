#ifndef PERLINNOISE_HPP
#define PERLINNOISE_HPP

# include "../base_header.hpp"


namespace ee
{
	class perlinNoise
	{
		private :
		public:
			std::vector<int> Permutation;

			perlinNoise();
			perlinNoise(const perlinNoise &copy);
			~perlinNoise();
			perlinNoise &operator=(const perlinNoise &copy);

			float noise2d(float x, float y);
			std::vector<int>  MakePermutation();
	};

	
}




#endif