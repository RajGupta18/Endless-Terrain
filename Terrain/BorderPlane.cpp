#include "BorderPlane.h"

int BorderPlane::meshSize = 120;
int BorderPlane::chunkSize = BorderPlane::meshSize + 2;
int BorderPlane::verticesPerLine = BorderPlane::meshSize + 5;

bool BorderPlane::has_map = false;
int BorderPlane::LODCount = 4;
BorderPlane::LODIndexMap BorderPlane::lodIndexMap[] = { BorderPlane::LODIndexMap(1, BorderPlane::verticesPerLine),
														BorderPlane::LODIndexMap(2, BorderPlane::verticesPerLine),
														BorderPlane::LODIndexMap(6, BorderPlane::verticesPerLine),
														BorderPlane::LODIndexMap(12, BorderPlane::verticesPerLine) };

void BorderPlane::SetIndexMap() {
	//set index map...
	for (int k = 0; k < LODCount; k++) {
		int actualMeshSize = meshSize / lodIndexMap[k].LOD;
		lodIndexMap[k].vertexCount = (unsigned int)((actualMeshSize + 1)*(actualMeshSize + 1) + 4 * chunkSize + 4 * actualMeshSize *(lodIndexMap[k].LOD - 1));
		lodIndexMap[k].indexCount = (unsigned int)(6 * actualMeshSize * actualMeshSize + 24 * (chunkSize - 1));

		int borderIndex = -1;
		int meshIndex = 0;
		for (int i = 0; i < verticesPerLine; i++) {
			for (int j = 0; j < verticesPerLine; j++) {
				bool isBorder = i == 0 || j == 0 || i == verticesPerLine - 1 || j == verticesPerLine - 1;
				bool isSkip = (i > 2 && i < verticesPerLine - 3 && j>2 && j < verticesPerLine - 3) && ((i - 2) % lodIndexMap[k].LOD != 0 || (j - 2) % lodIndexMap[k].LOD != 0);

				if (isBorder) {
					lodIndexMap[k].verticesIndexMap[i][j] = borderIndex;
					borderIndex--;
				}
				else if (!isSkip){
					lodIndexMap[k].verticesIndexMap[i][j] = meshIndex;
					meshIndex++;
				}
			}
		}
	}
	has_map = true;
}

BorderPlane::BorderPlane() : Mesh() {
	if (!has_map) SetIndexMap();

	LocalPos = glm::vec2(0.0f, 0.0f);
	LODIndex = 0;
	vertexCount = lodIndexMap[LODIndex].vertexCount;
	indexCount = lodIndexMap[LODIndex].indexCount;
}
BorderPlane::BorderPlane(glm::vec2 pos, int lodInd) {
	if (!has_map) SetIndexMap();

	LODIndex = lodInd;
	LocalPos = pos;
	vertexCount = lodIndexMap[LODIndex].vertexCount;
	indexCount = lodIndexMap[LODIndex].indexCount;
}

void BorderPlane::CreateBorderPlane() {

	vertices = new GLfloat[vertexCount * 3];
	normals = new GLfloat[vertexCount * 3];
	indices = new GLuint[indexCount];

	borderVertices = new GLfloat[12 * verticesPerLine - 12];
	int LOD = lodIndexMap[LODIndex].LOD;

	//for creating vertices (Corner based)...
	for (int i = 0; i < verticesPerLine; i++) {
		for (int j = 0; j < verticesPerLine; j++) {
			bool isBorder = i == 0 || j == 0 || i == verticesPerLine - 1 || j == verticesPerLine - 1;
			bool isSkip = (i > 2 && i < verticesPerLine - 3 && j>2 && j < verticesPerLine - 3) && ((i - 2) % LOD != 0 || (j - 2) % LOD != 0);

			int vertexIndex = lodIndexMap[LODIndex].verticesIndexMap[i][j];
			if (isBorder) {
				int ind = -(vertexIndex + 1) * 3;
				borderVertices[ind] = (i - 1) + LocalPos.x;
				borderVertices[ind + 1] = 0.0f;
				borderVertices[ind + 2] = (j - 1) + LocalPos.y;
			}
			else if (!isSkip) {
				int ind = vertexIndex * 3;
				vertices[ind] = (i - 1) + LocalPos.x;
				vertices[ind + 1] = 0.0f;
				vertices[ind + 2] = (j - 1) + LocalPos.y;
			}
		}
	}

	//for creating indices...
	int indpos = 0;
	for (int i = 1; i < verticesPerLine - 2; i++) {
		for (int j = 1; j < verticesPerLine - 2; j++) {
			int inc = 1;
			bool isSmallSQ = i == 1 || j == 1 || i == verticesPerLine - 3 || j == verticesPerLine - 3;
			if (!isSmallSQ) {
				bool isSkip = (i > 2 && i < verticesPerLine - 3 && j>2 && j < verticesPerLine - 3) && ((i - 2) % LOD != 0 || (j - 2) % LOD != 0);
				bool isEdge = (i == 2 && (j - 2) % LOD != 0) || (j == 2  && (i - 2) % LOD != 0);
				if (isSkip || isEdge) continue;
				else {
					inc = LOD;
				}
			}

			//first triangle.
			indices[indpos] = lodIndexMap[LODIndex].verticesIndexMap[i][j];
			indices[indpos + 1] = lodIndexMap[LODIndex].verticesIndexMap[i][j + inc];
			indices[indpos + 2] = lodIndexMap[LODIndex].verticesIndexMap[i + inc][j + inc];
			//second triangle.
			indices[indpos + 3] = lodIndexMap[LODIndex].verticesIndexMap[i][j];
			indices[indpos + 4] = lodIndexMap[LODIndex].verticesIndexMap[i + inc][j + inc];
			indices[indpos + 5] = lodIndexMap[LODIndex].verticesIndexMap[i + inc][j];

			//increment to add next 6 inidces of next 2 triangles.
			indpos += 6;
		}
	}
}

void BorderPlane::CreateBorderPlaneMesh(GLenum draw_type) {
	CreateMesh(vertices, indices, normals, vertexCount * 3, indexCount, draw_type);
}

void BorderPlane::RecalculateNormals() {
	//reset normals to zero....
	for (unsigned int i = 0; i < vertexCount * 3; i++) {
		normals[i] = 0.0f;
	}

	int LOD = lodIndexMap[LODIndex].LOD;
	for (int i = 0; i < verticesPerLine-1; i++) {
		for (int j = 0; j < verticesPerLine-1; j++) {
			int inc = 1;
			bool isSmallSQ = i < 2 || j < 2 || i >= verticesPerLine - 3 || j >= verticesPerLine - 3;
			if (!isSmallSQ) {
				bool isSkip = (i > 2 && i < verticesPerLine - 3 && j>2 && j < verticesPerLine - 3) && ((i - 2) % LOD != 0 || (j - 2) % LOD != 0);
				bool isEdge = (i == 2 && (j - 2) % LOD != 0) || (j == 2 && (i - 2) % LOD != 0);
				if (isSkip || isEdge) continue;
				else {
					inc = LOD;
				}
			}
			
			int a = lodIndexMap[LODIndex].verticesIndexMap[i][j];
			int b = lodIndexMap[LODIndex].verticesIndexMap[i][j + inc];
			int c = lodIndexMap[LODIndex].verticesIndexMap[i + inc][j + inc];
			int d = lodIndexMap[LODIndex].verticesIndexMap[i + inc][j];

			glm::vec3 avec;
			if (a < 0) {
				int ind = -(a + 1) * 3;
				avec = glm::vec3(borderVertices[ind], borderVertices[ind + 1], borderVertices[ind + 2]);
			}
			else {
				int ind = a * 3;
				avec = glm::vec3(vertices[ind], vertices[ind + 1], vertices[ind + 2]);
			}

			glm::vec3 bvec;
			if (b < 0) {
				int ind = -(b + 1) * 3;
				bvec = glm::vec3(borderVertices[ind], borderVertices[ind + 1], borderVertices[ind + 2]);
			}
			else {
				int ind = b * 3;
				bvec = glm::vec3(vertices[ind], vertices[ind + 1], vertices[ind + 2]);
			}

			glm::vec3 cvec;
			if (c < 0) {
				int ind = -(c + 1) * 3;
				cvec = glm::vec3(borderVertices[ind], borderVertices[ind + 1], borderVertices[ind + 2]);
			}
			else {
				int ind = c * 3;
				cvec = glm::vec3(vertices[ind], vertices[ind + 1], vertices[ind + 2]);
			}

			glm::vec3 dvec;
			if (d < 0) {
				int ind = -(d + 1) * 3;
				dvec = glm::vec3(borderVertices[ind], borderVertices[ind + 1], borderVertices[ind + 2]);
			}
			else {
				int ind = d * 3;
				dvec = glm::vec3(vertices[ind], vertices[ind + 1], vertices[ind + 2]);
			}

			glm::vec3 normal1 = glm::normalize(-glm::cross(cvec - bvec, bvec - avec));
			glm::vec3 normal2 = glm::normalize(-glm::cross(avec - dvec, dvec - cvec));

			if (a >= 0) {
				int ind = a * 3;
				normals[ind] += (normal1.x + normal2.x);
				normals[ind + 1] += (normal1.y + normal2.y);
				normals[ind + 2] += (normal1.z + normal2.z);
			}
			if (c >= 0) {
				int ind = c * 3;
				normals[ind] += (normal1.x + normal2.x);
				normals[ind + 1] += (normal1.y + normal2.y);
				normals[ind + 2] += (normal1.z + normal2.z);
			}
			if (b >= 0) {
				int ind = b * 3;
				normals[ind] += normal1.x;
				normals[ind + 1] += normal1.y;
				normals[ind + 2] += normal1.z;
			}
			if (d >= 0) {
				int ind = d * 3;
				normals[ind] += normal2.x;
				normals[ind + 1] += normal2.y;
				normals[ind + 2] += normal2.z;
			}
		}
	}

	//average all normals on a vertex...
	for (unsigned int i = 0; i < vertexCount * 3; i += 3) {
		GLfloat mag = glm::sqrt(normals[i] * normals[i] + normals[i + 1] * normals[i + 1] + normals[i + 2] * normals[i + 2]);
		normals[i] /= mag;
		normals[i + 1] /= mag;
		normals[i + 2] /= mag;
	}
}

void BorderPlane::ClearBorderPlaneMesh() {
	delete[] vertices;
	delete[] indices;
	delete[] normals;
	delete[] borderVertices;
}

BorderPlane::~BorderPlane() {
	ClearBorderPlaneMesh();
}

//Print Border Plane--------------------------------------------------------------------------------------------------------------------------------------------------------
	/*for (int i = 0; i < verticesPerLine; i++) {
		for (int j = 0; j < verticesPerLine; j++) {
			bool isBorder = i == 0 || j == 0 || i == verticesPerLine - 1 || j == verticesPerLine - 1;
			bool isChunk = i == 1 || j == 1 || i == verticesPerLine - 2 || j == verticesPerLine - 2;
			bool isMain = (i - 2) % LOD == 0 && (j - 2) % LOD == 0 && !isBorder;
			bool isSkip = (i > 2 && i < verticesPerLine - 3 && j>2 && j < verticesPerLine - 3) && ((i - 2) % LOD != 0 || (j - 2) % LOD != 0);
			bool isEdge = (i == 2 || i == verticesPerLine - 3 && (j - 2) % LOD != 0) || (j == 2 || j == verticesPerLine - 3 && (i - 2) % LOD != 0) && !isBorder && !isChunk;

			if (isBorder) {
				std::cout << 'b' << '\t'; continue;
			}
			if (isChunk) {
				std::cout << 'c' << '\t'; continue;
			}
			if (isMain) {
				std::cout << 'o' << '\t'; continue;
			}
			if (isSkip) {
				std::cout << ' ' << '\t'; continue;
			}
			if (isEdge) {
				std::cout << 'e' << '\t'; continue;
			}
		}
		std::cout << '\n';
	}*/