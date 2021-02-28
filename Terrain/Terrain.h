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
	Terrain();
	Terrain(GLuint w, GLuint h, GLuint lod, glm::vec2 pos,
			GLfloat s, GLfloat hScale);
	void BuildTerrainFBM();
	/*void UpdateTerrain(bool *keys);

	static void SetTerrainParameters(GLuint seed);
	static void UpdateParameters(bool *keys);
	static void SetPressedFalse() { pressed = false; }
	static GLfloat maxT(GLfloat a, GLfloat b) { return (a >= b) ? a : b; }
	static GLfloat minT(GLfloat a, GLfloat b) { return (a < b) ? a : b; }*/
	~Terrain();
private:
	GLfloat mapScale, heightScale;
	GLfloat CalculateMaxHeight();

	static PerlinNoise heightMap;
	static GLuint octaves;
	static GLfloat lacunarity, persistence;
	/*static GLfloat *octOffset;
	static bool pressed;*/
};
#endif