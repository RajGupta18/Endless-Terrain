#include "EndlessTerrain.h"

int EndlessTerrain::chunksize = BorderPlane::chunkSize;
EndlessTerrain::LODInfo EndlessTerrain::LODS[4] = { EndlessTerrain::LODInfo(0,170.0f),
													EndlessTerrain::LODInfo(1,340.0f),
													EndlessTerrain::LODInfo(2,510.0f),
													EndlessTerrain::LODInfo(3, 680.0f) };
const float EndlessTerrain::MoveThreshold = 25.0f*25.0f;

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
	//Terrain* terrain;
	BorderTerrain* terrain;
	int LOD;
};

//store currently visible planes...
std::unordered_map<ivec2, TerrainData, ivec2_hash> VisiblePlanes;

//store(singly linked list) newly added visible planes in VisiblePlanes...
std::forward_list<TerrainData> newPlanes;

int RoundtoInt(float val) {
	return (val < 0.0f) ? (int)val - 1 : (int)val;
}

EndlessTerrain::EndlessTerrain() {
	viewerPos = glm::vec3(0.0f, 0.0f, 0.0f);
	viewerPosOld = viewerPos;
	currKey = ivec2(0, 0);
	maxViewDist = 300.0f/2;
	viewIndexRange = (int)(maxViewDist / chunksize);
	UpdateVisiblePlanes();
}

EndlessTerrain::EndlessTerrain(glm::vec3 pos, float viewDist) {
	viewerPos = pos;
	viewerPosOld = viewerPos;
	currKey = ivec2(RoundtoInt(pos.x / chunksize), RoundtoInt(pos.z / chunksize));
	maxViewDist = viewDist;
	viewIndexRange = (int)(maxViewDist / chunksize);
	UpdateVisiblePlanes();
}

void EndlessTerrain::GetViewerPosition(glm::vec3 pos) {
	viewerPos = pos;
	
	//check if viewer has moved more than the threshold(If yes then updateTerrains)...
	float dist = (viewerPos.x - viewerPosOld.x)*(viewerPos.x - viewerPosOld.x) + (viewerPos.z - viewerPosOld.z)*(viewerPos.z - viewerPosOld.z);
	if (dist > MoveThreshold) {
		ivec2 newKey = ivec2(RoundtoInt(pos.x / chunksize), RoundtoInt(pos.z / chunksize));
		currKey = newKey;
		UpdateVisiblePlanes();
		viewerPosOld = viewerPos;
	}
}

void EndlessTerrain::UpdateVisiblePlanes() {
	//checking for planes to be deleted-----------------------------------
	auto it = VisiblePlanes.begin();
	while ( it != VisiblePlanes.end())
	{
		ivec2 key = it->first;
		if (abs(key.first - currKey.first) > viewIndexRange || abs(key.second - currKey.second) > viewIndexRange) {
			//Terrain* temp = VisiblePlanes[key].terrain;
			BorderTerrain* temp = VisiblePlanes[key].terrain;
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
					//Terrain* temp = VisiblePlanes[visKey].terrain;
					BorderTerrain* temp = VisiblePlanes[visKey].terrain;
					delete temp;

					//create mesh for new lod...
					glm::vec2 vispos(visKey.first * chunksize, visKey.second * chunksize);
					//VisiblePlanes[visKey].terrain = new Terrain(chunksize, lodIndex, vispos, 80.0f, 50.0f);
					VisiblePlanes[visKey].terrain = new BorderTerrain(lodIndex, vispos, 80.0f, 50.0f);
					VisiblePlanes[visKey].LOD = lodIndex;
					newPlanes.push_front(VisiblePlanes[visKey]);
					std::thread t(&BorderTerrain::TerrainThread, VisiblePlanes[visKey].terrain);
					threads.push_back(std::move(t));
				}
			}
			//create mesh if not contained in visible mesh...
			else {
				glm::vec2 vispos(visKey.first * chunksize, visKey.second * chunksize);
				//VisiblePlanes[visKey].terrain = new Terrain(chunksize, lodIndex, vispos, 80.0f, 50.0f);
				VisiblePlanes[visKey].terrain = new BorderTerrain(lodIndex, vispos, 80.0f, 50.0f);
				VisiblePlanes[visKey].LOD = lodIndex;
				newPlanes.push_front(VisiblePlanes[visKey]);
				std::thread t(&BorderTerrain::TerrainThread, VisiblePlanes[visKey].terrain);
				threads.push_back(std::move(t));
			}
		}
	}

	for (auto &t : threads) {
		t.join();
	}

	for (auto p : newPlanes) {
		p.terrain->CreateBorderPlaneMesh(GL_STATIC_DRAW);
		//p.terrain->CreatePlaneMesh(GL_STATIC_DRAW);
		//p.terrain->DeletePlane();
	}
	newPlanes.clear();
}

int EndlessTerrain::GetLODIndex(ivec2 &viskey) {
	glm::vec2 tpos = glm::vec2(viskey.first*chunksize + 60.0f, viskey.second*chunksize + 60.0f);
	float dist = (tpos.x - viewerPos.x)*(tpos.x - viewerPos.x) + (tpos.y - viewerPos.z)*(tpos.y - viewerPos.z);
	for (int i = 0; i < 4; i++) {
		if (dist < LODS[i].viewerDist*LODS[i].viewerDist) return LODS[i].LOD;
	}
	return LODS[3].LOD;
}

void EndlessTerrain::RenderVisiblePlanes() {
	for (auto &pl : VisiblePlanes) {
		pl.second.terrain->RenderMesh();
	}
}

void EndlessTerrain::SetColorDataArray(Shader *shader, ColorData colordata[], int colorCount) {
	shader->SetInt1("ColorDataCount", colorCount);

	for (int i = 0; i < colorCount; i++) {
		std::string s = "coldata[";
		s += std::to_string(i);
		shader->SetFloat3(s + "].color", colordata[i].color.x, colordata[i].color.y, colordata[i].color.z);
		shader->SetFloat1(s + "].baseHeight", colordata[i].baseHeight);
		shader->SetFloat1(s + "].blendStrength", colordata[i].blendStrength);
	}
}

EndlessTerrain::~EndlessTerrain() {}