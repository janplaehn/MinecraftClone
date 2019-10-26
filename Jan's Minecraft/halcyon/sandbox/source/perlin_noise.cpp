#include "perlin_noise.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <halcyon.h>


//Got most of this from here: https://cs.nyu.edu/~perlin/noise/ and here: https://mrl.nyu.edu/~perlin/paper445.pdf

perlin_noise::perlin_noise() {
	permutation = {
			151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142, 8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
			35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71, 134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
			55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89, 18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
			250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182, 189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
			43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246, 97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
			107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254, 138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

	permutation.insert(permutation.end(), permutation.begin(), permutation.end()); //duplicate
}

perlin_noise::perlin_noise(int seed) {
	if (seed < 0) seed *= -1;

	permutation.resize(256);
	std::iota(permutation.begin(), permutation.end(), 0); //https://en.cppreference.com/w/cpp/algorithm/iota

	std::default_random_engine engine(seed);						//shuffle using random engine
	std::shuffle(permutation.begin(), permutation.end(), engine);

	permutation.insert(permutation.end(), permutation.begin(), permutation.end()); //duplicate

}


perlin_noise::~perlin_noise() {}

double perlin_noise::noise(double positionX, double positionY) {
	double z = 0.8;

	// https://en.wikipedia.org/wiki/Bitwise_operation#AND
	int X = (int)floor(positionX) & 255;
	int Y = (int)floor(positionY) & 255;
	int Z = (int)floor(z) & 255;

	positionX -= floor(positionX);
	positionY -= floor(positionY);
	z -= floor(z);

	double u = fade(positionX);
	double v = fade(positionY);
	double w = fade(z);

	int A	=	permutation[X] + Y;
	int AA	=	permutation[A] + Z;
	int AB	=	permutation[A + 1] + Z;
	int B	=	permutation[X + 1] + Y;
	int BA	=	permutation[B] + Z;
	int BB	=	permutation[B + 1] + Z;

	double result =lerp(w, lerp(v, lerp(u, grad(permutation[AA], positionX, positionY, z),
				grad(permutation[BA], positionX - 1, positionY, z)),
				lerp(u, grad(permutation[AB], positionX, positionY - 1, z),
				grad(permutation[BB], positionX - 1, positionY - 1, z))),
				lerp(v, lerp(u, grad(permutation[AA + 1], positionX, positionY, z - 1),
				grad(permutation[BA + 1], positionX - 1, positionY, z - 1)),
				lerp(u, grad(permutation[AB + 1], positionX, positionY - 1, z - 1),
				grad(permutation[BB + 1], positionX - 1, positionY - 1, z - 1))));
	return (result + 1.0) / 2.0;
}

double perlin_noise::fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double perlin_noise::lerp(double t, double a, double b) {
	return a + t * (b - a);
}

double perlin_noise::grad(int hash, double x, double y, double z) {
	int		h = hash & 15;
	double	u = h < 8 ? x : y,
			v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
