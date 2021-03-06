#ifndef __TGEN_BORDER_TERRAIN_H__
#define __TGEN_BORDER_TERRAIN_H__

#include <stdlib.h>

#include "BorderPlane.h"
#include "PerlinNoise.h"

class BorderTerrain :
	public BorderPlane
{
public:
	void TerrainThread();

	BorderTerrain();
	BorderTerrain(unsigned int lodInd, glm::vec2 pos,
		GLfloat s, GLfloat hScale);
	~BorderTerrain();

	void BuildTerrainFBM();

	static void SetTerrainParameters(GLuint seed);
	static GLfloat *octOffset;

private:
	GLfloat mapScale, heightScale;
	GLfloat CalculateMaxHeight();

	GLfloat GetHeightProfile(GLfloat val);

	static PerlinNoise heightMap;
	static GLuint octaves;
	static GLfloat lacunarity, persistence;
	static bool has_set;
};
#endif