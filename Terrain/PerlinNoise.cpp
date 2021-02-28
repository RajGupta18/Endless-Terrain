#include "PerlinNoise.h"

PerlinNoise::PerlinNoise() { 
	seed = 0; 
	SetPermuationTable();
}

PerlinNoise::PerlinNoise(unsigned int s) {
	seed = s; 
	SetPermuationTable();
}

float PerlinNoise::Noise2D(float x, float y) {
	//X, Y are the bottom left corner coordinates (or largest nearest integer to the given value of x and y)
	int X = (int)std::floor(x) & 255;
	int Y = (int)std::floor(y) & 255;
	float xf = x - std::floor(x);
	float yf = y - std::floor(y);

	//4 input vectors from input coordinate to 4 corners
	glm::vec2 bLeft = glm::vec2(xf,yf);
	glm::vec2 bRight = glm::vec2(xf-1.0f, yf);
	glm::vec2 tLeft = glm::vec2(xf, yf-1.0f);
	glm::vec2 tRight = glm::vec2(xf-1.0f, yf-1.0f);

	//Selecting value for each corner from Permutation table (for a given seed)...
	int bLeftVal = P[P[X] + Y];
	int bRightVal = P[P[X + 1] + Y];
	int tLeftVal = P[P[X] + Y + 1];
	int tRightVal = P[P[X + 1] + Y + 1];

	//now dot product for each corner with their respective input vectors...
	float dotBleft = glm::dot(bLeft, GetConstantVector(bLeftVal));		//bLeft.Dot(GetConstantVector(bLeftVal));
	float dotBright = glm::dot(bRight, GetConstantVector(bRightVal));	//bRight.Dot(GetConstantVector(bRightVal));
	float dotTleft = glm::dot(tLeft, GetConstantVector(tLeftVal));		//tLeft.Dot(GetConstantVector(tLeftVal));
	float dotTright = glm::dot(tRight, GetConstantVector(tRightVal));	//tRight.Dot(GetConstantVector(tRightVal));

	//Get Interpolated Value for the given x,y provided...
	float u = Fade(xf);
	float v = Fade(yf);

	return Lerp(v, Lerp(u, dotBleft, dotBright), Lerp(u, dotTleft, dotTright)) * 1.41421356f;	//gives a range of (-1 , 1)
}

float PerlinNoise::Lerp(float t, float a, float b) {
	return a + t * (b - a);
}

float PerlinNoise::Fade(float t) {
	return ((6 * t - 15)*t + 10)*t*t*t;
}

glm::vec2 PerlinNoise::GetConstantVector(int val) {
	//We are using 4 constant vectors...
	//(1,1) , (-1,1) , (-1,-1) , (1,-1)
	int h = val % 3;
	if (h == 0) return glm::vec2(1.0f, 1.0f);
	else if (h == 1) return glm::vec2(-1.0f, 1.0f);
	else if (h == 2) return glm::vec2(-1.0f, -1.0f);
	return glm::vec2(1.0f, -1.0f);

	//For now sticking to only the above 4 vectors...
	//Also can use 4 more vectors....
	//(root(2),0) , (-root(2),0) , (0,root(2)) , (0, -root(2))
}

void PerlinNoise::SetPermuationTable() {
	//P = new int[2*size];
	for (int i = 0; i < 256; i++) {
		P[i] = i;
	}
	ShufflePermuationTable();
}

void PerlinNoise::ShufflePermuationTable() {
	srand(seed);
	for (int i = 0; i < 256; i++) {
		//generate random number between 0 to (size-1)...
		int index = rand() % 256;

		//swapping index with current i in permutation table to shuffle...
		int temp = P[index];
		P[index] = P[i];
		P[i] = temp;
	}
	for (int i = 256; i < 512; i++) {
		P[i] = P[i - 256];
	}
}

PerlinNoise::~PerlinNoise() {}