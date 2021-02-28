#include "EndlessTerrain.h"

struct ivec2_hash {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const {
		int hash = 17;
		hash = hash * 23 + std::hash<T1>{}(p.first);
		hash = hash * 23 + std::hash<T2>{}(p.second);
		return hash;
	}
};

//store currently visible planes...
std::unordered_map<ivec2, Terrain*, ivec2_hash> VisiblePlanes;

//store(singly linked list) newly added visible planes in VisiblePlanes...
std::forward_list<Terrain*> newPlanes;

void planethread(ivec2 key, glm::vec2 pos) {
	VisiblePlanes[key] = new Terrain(240, 240, 1, pos, 100.0f, 50.0f);
	VisiblePlanes[key]->BuildTerrainFBM();
	//VisiblePlanes[key]->RecalculateNormals();
	newPlanes.push_front(VisiblePlanes[key]);
}

EndlessTerrain::EndlessTerrain() {
	currKey = ivec2(0, 0);
	xOffset = 1;
	zOffset = 1;
	UpdateVisiblePlanes();
}

EndlessTerrain::EndlessTerrain(glm::vec3 pos, int xoff, int zoff) {
	currKey = ivec2(pos.x / 240, pos.z / 240);
	xOffset = xoff;
	zOffset = zoff;
	UpdateVisiblePlanes();
}

void EndlessTerrain::GetViewerPosition(glm::vec3 pos) {
	ivec2 newKey = ivec2(pos.x / 240, pos.z / 240);
	if (currKey != newKey) {
		currKey = newKey;
		std::cout << "Visible Planes Updated!!" << std::endl;
		UpdateVisiblePlanes();
	}
}

void EndlessTerrain::UpdateVisiblePlanes() {
	//checking for planes to be deleted-----------------------------------
	auto it = VisiblePlanes.begin();
	while ( it != VisiblePlanes.end())
	{
		ivec2 key = it->first;
		if (abs(key.first - currKey.first) > xOffset || abs(key.second - currKey.second) > zOffset) {
			Terrain* temp = VisiblePlanes[key];
			delete temp;
			it = VisiblePlanes.erase(it);
		}
		else ++it;
	}

	//for adding new Planes-----------------------------------------------
	std::vector<std::thread> threads;
	for (int Z = -zOffset; Z <= zOffset; Z++) {
		for (int X = -xOffset; X <= xOffset; X++) {
			ivec2 visKey = ivec2(currKey.first + X, currKey.second + Z);
			if (VisiblePlanes.find(visKey) == VisiblePlanes.end()) {
				glm::vec2 vispos(visKey.first * 240, visKey.second * 240);
				std::thread t(planethread, visKey, vispos);
				threads.push_back(std::move(t));
			}
		}
	}
	for (auto &t : threads) {
		t.join();
	}
	for (auto p : newPlanes) {
		p->CreatePlaneMesh(GL_NORMAL_ARRAY_POINTER, GL_STATIC_DRAW);
	}
	newPlanes.clear();
}

void EndlessTerrain::RenderVisiblePlanes() {
	for (auto &pl : VisiblePlanes) {
		pl.second->RenderMesh();
	}
}

EndlessTerrain::~EndlessTerrain() {}