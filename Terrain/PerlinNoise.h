#ifndef _INCLUDE_PERLIN_NOISE_H
#define _INCLUDE_PERLIN_NOISE_H

#include<cmath>
#include<cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PerlinNoise
{
public:
	PerlinNoise();
	PerlinNoise(unsigned int s);
	float Noise2D(float x, float y);
	~PerlinNoise();

private:
	int P[2*256];
	unsigned int seed;

	void SetPermuationTable();
	void ShufflePermuationTable();
	glm::vec2 GetConstantVector(int val);
	float Fade(float t);
	float Lerp(float t, float a, float b);
};
#endif