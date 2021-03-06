#include "BorderTerrain.h"

GLuint BorderTerrain::octaves = 5;
GLfloat BorderTerrain::lacunarity = 2.0f;
GLfloat BorderTerrain::persistence = 0.4751f;
PerlinNoise BorderTerrain::heightMap = PerlinNoise(10);
GLfloat* BorderTerrain::octOffset = new GLfloat[2 * BorderTerrain::octaves];
bool BorderTerrain::has_set = false;

void BorderTerrain::SetTerrainParameters(GLuint seed) {
	srand(seed);
	for (unsigned int k = 0; k < octaves; k++) {
		octOffset[2 * k] = (GLfloat)(rand() % 255 + 1);
		octOffset[2 * k + 1] = (GLfloat)(rand() % 255 + 1);
	}
	has_set = true;
}

void BorderTerrain::TerrainThread() {
	BuildTerrainFBM();
}

BorderTerrain::BorderTerrain() : BorderPlane() {
	mapScale = 10.0f;
	heightScale = 1.0f;

	if (!has_set) SetTerrainParameters(10);
}

BorderTerrain::BorderTerrain(unsigned int lodInd, glm::vec2 pos, GLfloat s, GLfloat hScale) : BorderPlane(pos, lodInd) {
	mapScale = s;
	heightScale = hScale;

	if (!has_set) SetTerrainParameters(10);
}

void BorderTerrain::BuildTerrainFBM() {
	CreateBorderPlane();
	GLfloat maxheight = CalculateMaxHeight();
	GLfloat minheight = 0.0f;

	int LOD = lodIndexMap[LODIndex].LOD;
	for (int i = 0; i < verticesPerLine; i++) {
		for (int j = 0; j < verticesPerLine; j++) {
			bool isSkip = (i > 2 && i < verticesPerLine - 3 && j>2 && j < verticesPerLine - 3) && ((i - 2) % LOD != 0 || (j - 2) % LOD != 0);

			if (isSkip) continue;

			int vertexIndex = lodIndexMap[LODIndex].verticesIndexMap[i][j];

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
				float sampleX = (*dataX + octOffset[2 * k]) / mapScale * frequency;
				float sampleZ = (*dataZ + octOffset[2 * k + 1]) / mapScale * frequency;
				GLfloat perlinValue = heightMap.Noise2D(sampleX, sampleZ) + 1;
				netHeight += perlinValue * amplitude;
				frequency *= lacunarity;
				amplitude *= persistence;
			}
			*dataY = heightScale * GetHeightProfile((netHeight - minheight) / (maxheight - minheight));
		}
	}

	//stiching edge connection vertices...
	int currMainIndex[4], nextMainIndex[4];
	for (int j = 2; j < verticesPerLine - 3; j++) {
		bool isMain = (j - 2) % LOD == 0;
		if (isMain) {
			currMainIndex[0] = lodIndexMap[LODIndex].verticesIndexMap[2][j];
			nextMainIndex[0] = lodIndexMap[LODIndex].verticesIndexMap[2][j+LOD];
			currMainIndex[1] = lodIndexMap[LODIndex].verticesIndexMap[verticesPerLine - 3][j];
			nextMainIndex[1] = lodIndexMap[LODIndex].verticesIndexMap[verticesPerLine - 3][j + LOD];

			currMainIndex[2] = lodIndexMap[LODIndex].verticesIndexMap[j][2];
			nextMainIndex[2] = lodIndexMap[LODIndex].verticesIndexMap[j + LOD][2];
			currMainIndex[3] = lodIndexMap[LODIndex].verticesIndexMap[j][verticesPerLine - 3];
			nextMainIndex[3] = lodIndexMap[LODIndex].verticesIndexMap[j + LOD][verticesPerLine - 3];
		}
		else {
			int ind1 = lodIndexMap[LODIndex].verticesIndexMap[2][j];
			int ind2 = lodIndexMap[LODIndex].verticesIndexMap[verticesPerLine - 3][j];
			int ind3 = lodIndexMap[LODIndex].verticesIndexMap[j][2];
			int ind4 = lodIndexMap[LODIndex].verticesIndexMap[j][verticesPerLine - 3];
			GLfloat percent = (GLfloat)(ind1 - currMainIndex[0]) / LOD;

			vertices[3 * ind1 + 1] = vertices[3 * currMainIndex[0] + 1] + percent * (vertices[3*nextMainIndex[0] + 1] - vertices[3 * currMainIndex[0] + 1]);
			vertices[3*ind2 + 1] = vertices[3 * currMainIndex[1] + 1] + percent * (vertices[3 * nextMainIndex[1] + 1] - vertices[3 * currMainIndex[1] + 1]);
			vertices[3 * ind3 + 1] = vertices[3 * currMainIndex[2] + 1] + percent * (vertices[3 * nextMainIndex[2] + 1] - vertices[3 * currMainIndex[2] + 1]);
			vertices[3 * ind4 + 1] = vertices[3 * currMainIndex[3] + 1] + percent * (vertices[3 * nextMainIndex[3] + 1] - vertices[3 * currMainIndex[3] + 1]);
		}
	}

	RecalculateNormals();
}

GLfloat BorderTerrain::GetHeightProfile(GLfloat val) {
	//quadratic profile...
	return pow(val, 2);

	//Bezier Curve Profile...
}

GLfloat BorderTerrain::CalculateMaxHeight() {
	GLfloat maxheight = 0.0f;
	GLfloat ampli = 1.0f;
	for (unsigned int i = 0; i < octaves; i++) {
		maxheight += ampli;
		ampli *= persistence;
	}
	return 2 * maxheight;
}

BorderTerrain::~BorderTerrain() {}