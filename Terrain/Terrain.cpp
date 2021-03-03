#include "Terrain.h"
#include <iostream>

//SET PARAMS------------------------------------------------------
GLuint Terrain::octaves = 5;
GLfloat Terrain::lacunarity = 2.0f;
GLfloat Terrain::persistence = 0.4751f;
PerlinNoise Terrain::heightMap = PerlinNoise(10);
GLfloat* Terrain::octOffset = new GLfloat[2 * Terrain::octaves];
bool Terrain::has_set = false;
//----------------------------------------------------------------

void Terrain::TerrainThread() {
	BuildTerrainFBM();
}

Terrain::Terrain() : Plane() {
	mapScale = 10.0f;
	heightScale = 1.0f;
}

Terrain::Terrain(int size, unsigned int lod, glm::vec2 pos,
				GLfloat s, GLfloat hScale) : Plane(size, lod, pos) {
	mapScale = s;
	heightScale = hScale;
}

void Terrain::SetTerrainParameters(GLuint seed) {
	srand(seed);
	for (unsigned int k = 0; k < octaves; k++) {
		octOffset[2 * k] = (GLfloat)(rand() % 255 + 1);
		octOffset[2 * k + 1] = (GLfloat)(rand() % 255 + 1);
	}
	has_set = true;
}

void Terrain::BuildTerrainFBM() {
	CreatePlane();
	GLfloat maxheight = CalculateMaxHeight();
	GLfloat minheight = 0.0f;

	if (!has_set) SetTerrainParameters(10);

	for (int i = 0; i <= borderSize; i++) {
		for (int j = 0; j <= borderSize; j++) {
			int vertexIndex = verticesIndexMap[i * (borderSize + 1) + j];

			GLfloat* dataX;
			GLfloat* dataY;
			GLfloat* dataZ;
			if (vertexIndex >= 0) {	//mesh vertices...
				int ind = vertexIndex * 3;
				dataX = &vertices[ind];
				dataY = &vertices[ind + 1];
				dataZ = &vertices[ind + 2];
			}
			else {	//border vertices...
				int ind = -(vertexIndex + 1) * 3;
				dataX = &borderVertices[ind];
				dataY = &borderVertices[ind + 1];
				dataZ = &borderVertices[ind + 2];
			}

			GLfloat frequency = 1;
			GLfloat amplitude = 1;
			GLfloat netHeight = 0.0f;
			for (unsigned int k = 0; k < octaves; k++) {
				float sampleX = (*dataX + octOffset[2*k]) / mapScale * frequency;
				float sampleZ = (*dataZ + octOffset[2*k + 1]) / mapScale * frequency;
				GLfloat perlinValue = heightMap.Noise2D(sampleX, sampleZ) + 1;
				netHeight += perlinValue * amplitude;
				frequency *= lacunarity;
				amplitude *= persistence;
			}
			*dataY = heightScale * GetHeightProfile((netHeight - minheight) / (maxheight - minheight));
		}
	}
	RecalculateNormals();
}

GLfloat Terrain::GetHeightProfile(GLfloat val) {
	//quadratic profile...
	return pow(val, 2);

	//Bezier Curve Profile...
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

Terrain::~Terrain() {}