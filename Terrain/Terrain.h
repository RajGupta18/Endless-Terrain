#ifndef __TGEN_TERRAIN_H__
#define __TGEN_TERRAIN_H__

#include <stdlib.h>

#include "Plane.h"
#include "PerlinNoise.h"

#include <GLFW/glfw3.h>

class Terrain :
	public Plane
{
public:
	void TerrainThread();

	Terrain();
	Terrain(int size, unsigned int lod, glm::vec2 pos,
			GLfloat s, GLfloat hScale);
	void BuildTerrainFBM();
	~Terrain();

	static void SetTerrainParameters(GLuint seed);
private:
	GLfloat mapScale, heightScale;
	GLfloat CalculateMaxHeight();

	GLfloat GetHeightProfile(GLfloat val);

	static PerlinNoise heightMap;
	static GLuint octaves;
	static GLfloat lacunarity, persistence;
	static GLfloat *octOffset;
	static bool has_set;
};
#endif