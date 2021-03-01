#include "EndlessTerrain.h"

const int chunksize = 120;

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

int RoundtoInt(float val) {
	return (val < 0.0f) ? (int)val - 1 : (int)val;
}

void planethread(ivec2 key, glm::vec2 pos) {
	VisiblePlanes[key] = new Terrain(chunksize, chunksize, 12, pos, 100.0f, 50.0f);
	VisiblePlanes[key]->BuildTerrainFBM();
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
		if (abs(key.first - currKey.first) > viewIndexRange || abs(key.second - currKey.second) > viewIndexRange) {
			Terrain* temp = VisiblePlanes[key];
			delete temp;
			it = VisiblePlanes.erase(it);
		}
		else ++it;
	}

	//for adding new Planes-----------------------------------------------
	std::vector<std::thread> threads;
	for (int Z = -viewIndexRange; Z <= viewIndexRange; Z++) {
		for (int X = -viewIndexRange; X <= viewIndexRange; X++) {
			ivec2 visKey = ivec2(currKey.first + X, currKey.second + Z);
			if (VisiblePlanes.find(visKey) == VisiblePlanes.end()) {
				glm::vec2 vispos(visKey.first * chunksize, visKey.second * chunksize);
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