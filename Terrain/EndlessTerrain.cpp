#include "EndlessTerrain.h"

const int chunksize = 120;
const int LODS[] = { 1,4,6,12 };

struct ivec2_hash {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const {
		int hash = 17;
		hash = hash * 23 + std::hash<T1>{}(p.first);
		hash = hash * 23 + std::hash<T2>{}(p.second);
		return hash;
	}
};

struct TerrainData {
	Terrain* terrain;
	int LOD;
};

//store currently visible planes...
std::unordered_map<ivec2, TerrainData, ivec2_hash> VisiblePlanes;

//store(singly linked list) newly added visible planes in VisiblePlanes...
std::forward_list<TerrainData> newPlanes;

int RoundtoInt(float val) {
	return (val < 0.0f) ? (int)val - 1 : (int)val;
}

void planethread(ivec2 key, glm::vec2 pos, int lodInd) {
	VisiblePlanes[key].terrain = new Terrain(chunksize, LODS[lodInd], pos, 100.0f, 50.0f);
	VisiblePlanes[key].LOD = lodInd;
	VisiblePlanes[key].terrain->BuildTerrainFBM();
	//VisiblePlanes[key]->RecalculateNormals();
	newPlanes.push_front(VisiblePlanes[key]);
}

EndlessTerrain::EndlessTerrain() {
	currKey = ivec2(0, 0);
	maxViewDist = 300.0f/2;
	viewIndexRange = (int)(maxViewDist / chunksize);
	UpdateVisiblePlanes();
}

EndlessTerrain::EndlessTerrain(glm::vec3 pos, float viewDist) {
	currKey = ivec2(RoundtoInt(pos.x / chunksize), RoundtoInt(pos.z / chunksize));
	maxViewDist = viewDist;
	viewIndexRange = (int)(maxViewDist / chunksize);
	UpdateVisiblePlanes();
}

void EndlessTerrain::GetViewerPosition(glm::vec3 pos) {
	ivec2 newKey = ivec2(RoundtoInt(pos.x / chunksize), RoundtoInt(pos.z / chunksize));
	if (currKey != newKey) {
		currKey = newKey;
		UpdateVisiblePlanes();
	}
}

void EndlessTerrain::UpdateVisiblePlanes() {
	//checking for planes to be deleted-----------------------------------
	auto it = VisiblePlanes.begin();
	while ( it != VisiblePlanes.end())
	{
		ivec2 key = it->first;
		if (abs(key.first - currKey.first) > viewIndexRange || abs(key.second - currKey.second) > viewIndexRange) {
			Terrain* temp = VisiblePlanes[key].terrain;
			delete temp;
			it = VisiblePlanes.erase(it);
		}
		else ++it;
	}

	//for adding new Planes-----------------------------------------------
	std::vector<std::thread> threads;
	for (int Z = -viewIndexRange; Z <= viewIndexRange; Z++) {
		for (int X = -viewIndexRange; X <= viewIndexRange; X++) {
			//visible plane indices...
			ivec2 visKey = ivec2(currKey.first + X, currKey.second + Z);

			//LOD index for the current plane...
			int lodIndex = GetLODIndex(visKey);

			//if mesh is already contained in visible planes...
			if (VisiblePlanes.find(visKey) != VisiblePlanes.end()) {
				//update each terrain chunk's LOD...
				if (lodIndex == VisiblePlanes[visKey].LOD) continue;	//continue if lod doesn't change...
				else {
					//first delete previous lod's mesh...
					Terrain* temp = VisiblePlanes[visKey].terrain;
					delete temp;

					//create mesh for new lod...
					glm::vec2 vispos(visKey.first * chunksize, visKey.second * chunksize);
					std::thread t(planethread, visKey, vispos, lodIndex);
					threads.push_back(std::move(t));
				}
			}
			//create mesh if not contained in visible mesh...
			else {
				glm::vec2 vispos(visKey.first * chunksize, visKey.second * chunksize);
				std::thread t(planethread, visKey, vispos, lodIndex);
				threads.push_back(std::move(t));
			}
		}
	}
	for (auto &t : threads) {
		t.join();
	}
	for (auto p : newPlanes) {
		p.terrain->CreatePlaneMesh(GL_STATIC_DRAW);
	}
	newPlanes.clear();
}

int EndlessTerrain::GetLODIndex(ivec2 &viskey) {
	int dist = abs(viskey.first - currKey.first) + abs(viskey.second - currKey.second);
	if (dist <= 1) return 0;
	return dist-1;
}

void EndlessTerrain::RenderVisiblePlanes() {
	for (auto &pl : VisiblePlanes) {
		pl.second.terrain->RenderMesh();
	}
}

EndlessTerrain::~EndlessTerrain() {}