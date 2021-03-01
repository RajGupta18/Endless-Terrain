#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "DirectionalLight.h"

#include "EndlessTerrain.h"

//Shader Code Location...
static const char* vShader = "Shader/VertexShader.glsl";
static const char* fShader = "Shader/FragmentShader.glsl";

const float toRadian = 3.14159265f / 180.f;

//time variable...
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Window mainWindow;
Camera cam;
DirectionalLight dl;

int main() {
	mainWindow = Window(1366, 768);
	if (mainWindow.Init()) return 1;

	cam = Camera(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 10.0f, 6.0f);
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 1000.0f);
	dl = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 1.0f, glm::vec3(-1.0f, -1.0f, 1.0f));

	//initialise Shader...
	Shader shd = Shader();
	shd.CreateFromFile(vShader, fShader);
	shd.UseShader();
	shd.SetMat4("proj", projection);
	dl.UseLight(&shd);

	//BUILD TERRAIN---------------------------------------------------
	//Terrain::SetTerrainParameters(10);
	/*std::thread t1(&Terrain::BuildTerrainFBM, terr);
	std::thread t2(&Terrain::BuildTerrainFBM, terr1);
	t1.join();
	t2.join();
	terr->CreatePlaneMesh(GL_NORMAL_ARRAY_POINTER, GL_DYNAMIC_DRAW);
	terr1->CreatePlaneMesh(GL_NORMAL_ARRAY_POINTER, GL_DYNAMIC_DRAW);*/
	//----------------------------------------------------------------
	EndlessTerrain endless(cam.GetCameraPosition(), 150.0f);

	bool * keys = mainWindow.getsKeys();
	while (!mainWindow.WindowClose()) {
		//Clear Window
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainWindow.ChangePloygonMode();

		GLfloat now = (float)glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//reset camera...
		if (keys[GLFW_KEY_F]) {
			cam.SetCameraPosition(glm::vec3(0.0f,1.0f,0.0f));
		}
		endless.GetViewerPosition(cam.GetCameraPosition());
		/*terr->UpdateTerrain(keys);
		terr1->UpdateTerrain(keys);
		Terrain::SetPressedFalse();*/

		cam.KeyControls(keys, deltaTime);
		cam.MouseControls(mainWindow.getXchange(), mainWindow.getYchange(), deltaTime);

		shd.UseShader();
		shd.SetMat4("view", cam.GetViewMatrix());

		glm::mat4 model(1.0f);
		glm::mat4 NormalMat(1.0f);
		NormalMat = glm::transpose(glm::inverse(model));
		shd.SetMat4("model", model);
		shd.SetMat4("NormalMatrix", NormalMat);
		endless.RenderVisiblePlanes();

		glUseProgram(0);
		mainWindow.SwapBuffers();
		glfwPollEvents();
	}
	return 0;
}