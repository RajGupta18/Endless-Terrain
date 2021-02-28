#include "Terrain.h"
#include <iostream>

//SET PARAMS------------------------------------------------------
GLuint Terrain::octaves = 3;
GLfloat Terrain::lacunarity = 1.75f;
GLfloat Terrain::persistence = 0.75f;
PerlinNoise Terrain::heightMap = PerlinNoise(10);
//GLfloat* Terrain::octOffset = new GLfloat[2 * Terrain::octaves];
//bool Terrain::pressed = false;
//----------------------------------------------------------------

Terrain::Terrain() : Plane() {
	mapScale = 10.0f;
	heightScale = 1.0f;
}

Terrain::Terrain(GLuint w, GLuint h, GLuint lod, glm::vec2 pos,
				GLfloat s, GLfloat hScale) : Plane(w, h, lod, pos) {
	mapScale = s;
	heightScale = hScale;
}

//void Terrain::SetTerrainParameters(GLuint seed) {
//	srand(seed);
//	for (unsigned int k = 0; k < octaves; k++) {
//		octOffset[2 * k] = (rand() % 2001 - 1000)*(rand() % 100 + 1);	//range of -100000 to 100000
//		octOffset[2 * k + 1] = (rand() % 2001 - 1000)*(rand() % 100 + 1);
//	}
//}
//
//void Terrain::UpdateParameters(bool *keys) {
//	if (keys[GLFW_KEY_L] && keys[GLFW_KEY_UP]) {
//		lacunarity += 0.1f; pressed = true;
//	}
//	if (keys[GLFW_KEY_L] && keys[GLFW_KEY_DOWN]) {
//		lacunarity = maxT(0.0f, lacunarity - 0.1f); pressed = true;
//	}
//	if (keys[GLFW_KEY_P] && keys[GLFW_KEY_UP]) {
//		persistence = minT(1.0f, persistence + 0.05f); pressed = true;
//	}
//	if (keys[GLFW_KEY_P] && keys[GLFW_KEY_DOWN]) {
//		persistence = maxT(0.0f, persistence - 0.05f); pressed = true;
//	}
//	/*if (keys[GLFW_KEY_K] && keys[GLFW_KEY_UP]) {
//		mapScale += 1.0f; pressed = true;
//	}
//	if (keys[GLFW_KEY_K] && keys[GLFW_KEY_DOWN]) {
//		mapScale = maxT(1.0f, mapScale - 1.0f); pressed = true;
//	}*/
//}

void Terrain::BuildTerrainFBM() {
	CreatePlane();
	GLfloat maxheight = CalculateMaxHeight(); //-100000.0f;
	GLfloat minheight = 0.0f; //100000.0f;
	for (unsigned int j = 0;j <= height; j++) {
		for (unsigned int i = 0; i <= width; i++) {
			unsigned int ind = (j * (width + 1) + i) * 3;

			GLfloat frequency = 1;
			GLfloat amplitude = 1;
			GLfloat netHeight = 0.0f;
			for (unsigned int k = 0; k < octaves; k++) {
				GLfloat perlinValue = heightMap.Noise2D(vertices[ind] / mapScale * frequency, vertices[ind + 2] / mapScale * frequency)+1;
				netHeight += perlinValue * amplitude;
				frequency *= lacunarity;
				amplitude *= persistence;
			}
			vertices[ind + 1] = netHeight;
		}
	}
	//normalize value between 0 and 1 then scale with heightscale...
	for (unsigned int j = 0;j <= height; j++) {
		for (unsigned int i = 0; i <= width; i++) {
			unsigned int ind = (j * (width + 1) + i) * 3;
			vertices[ind + 1] = heightScale * pow((vertices[ind+1]-minheight)/(maxheight-minheight),2);
		}
	}
	RecalculateNormals();
}

GLfloat Terrain::CalculateMaxHeight() {
	GLfloat maxheight = 0.0f;
	GLfloat ampli = 1.0f;
	for (unsigned int i = 0; i < octaves; i++) {
		maxheight += ampli;
		ampli *= persistence;
	}
	return 2 * maxheight;
}

//void Terrain::UpdateTerrain(bool *keys) {
//	UpdateParameters(keys);
//	if (pressed) {
//		BuildTerrainFBM();
//		UpdatePlaneMesh(GL_NORMAL_ARRAY_POINTER);
//	}
//}

Terrain::~Terrain() {
	//delete[] octOffset;
}