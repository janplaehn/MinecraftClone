#ifndef PERLIN_NOISE_H_INCLUDED
#define PERLIN_NOISE_H_INCLUDED

#include <vector>

class perlin_noise {

public:
	std::vector<int> permutation;

	perlin_noise();
	perlin_noise(int seed);
	~perlin_noise();

	double noise(double positionX, double positionY);

private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};

#endif

