#ifndef __TGEN_BORDER_PLANE_H__
#define __TGEN_BORDER_PLANE_H__

#include <iostream>
#include "Mesh.h"
#include "glm/glm.hpp"

class BorderPlane :
	public Mesh
{
public:
	struct LODIndexMap {
		int LOD;
		int vertPerline;
		int** verticesIndexMap;
		GLuint vertexCount, indexCount;
		LODIndexMap() {}
		LODIndexMap(int lod, int vpl) {
			LOD = lod;
			vertPerline = vpl;
			verticesIndexMap = new int*[vpl];
			for (int i = 0; i < vpl; i++) {
				verticesIndexMap[i] = new int[vpl];
			}
		}
 		~LODIndexMap() {
			for (int i = 0; i < vertPerline; i++) {
				delete[] verticesIndexMap[i];
			}
			delete[] verticesIndexMap;
		}
	};
	static bool has_map;
	
	static int meshSize;
	static int chunkSize;
	static int verticesPerLine;
	static int LODCount;
	static LODIndexMap lodIndexMap[];

	static void SetIndexMap();

	BorderPlane();
	BorderPlane(glm::vec2 pos, int lodInd);
	~BorderPlane();

	void CreateBorderPlane();
	void RecalculateNormals();
	void CreateBorderPlaneMesh(GLenum draw_type);
	void ClearBorderPlaneMesh();

protected:
	GLfloat* vertices;
	GLfloat* normals;
	GLuint* indices;
	GLuint vertexCount, indexCount;

	GLfloat* borderVertices;

	int LODIndex;
private:
	glm::vec2 LocalPos;
};
#endif