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

	//initialise Shader...
	Shader shd = Shader();
	shd.CreateFromFile(vShader, fShader);

	cam = Camera(glm::vec3(1.0f, 20.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 10.0f, 15.0f);
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 1500.0f);
	dl = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 1.0f, glm::vec3(-1.0f, -1.0f, 1.0f));

	//model transformation...
	glm::mat4 model(1.0f);
	glm::mat4 NormalMat(1.0f);
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	NormalMat = glm::transpose(glm::inverse(model));


	//BUILD TERRAIN---------------------------------------------------
	EndlessTerrain endless(cam.GetCameraPosition()/10.0f, 390.0f);
	EndlessTerrain::ColorData colordata[4];

	colordata[0].color = glm::vec3(14.0f / 255.0f, 86.0f / 255.0f, 114.0f / 255.0f);
	colordata[0].baseHeight = 0.0f;
	colordata[0].blendStrength = 0.0f;
	colordata[1].color = glm::vec3(215.0f / 255.0f, 136.0f / 255.0f, 79.0f / 255.0f);
	colordata[1].baseHeight = 0.25f;
	colordata[1].blendStrength = 0.1f;
	colordata[2].color = glm::vec3(16.0f / 255.0f, 113.0f / 255.0f, 35.0f / 255.0f);
	colordata[2].baseHeight = 0.3f;
	colordata[2].blendStrength = 0.1f;
	colordata[3].color = glm::vec3(247.0f / 255.0f, 252.0f / 255.0f, 254.0f / 255.0f);
	colordata[3].baseHeight = 0.7f;
	colordata[3].blendStrength = 0.3f;

	//Set uniform parameters in shader...
	shd.UseShader();
	shd.SetMat4("proj", projection);
	shd.SetMat4("model", model);
	shd.SetMat4("NormalMatrix", NormalMat);
	shd.SetFloat1("maxHeight", 50.0f*10.0f);
	dl.UseLight(&shd);
	endless.SetColorDataArray(&shd, colordata, 4);

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
		endless.GetViewerPosition(cam.GetCameraPosition()/10.0f);

		cam.KeyControls(keys, deltaTime);
		cam.MouseControls(mainWindow.getXchange(), mainWindow.getYchange(), deltaTime);

		shd.UseShader();
		shd.SetMat4("view", cam.GetViewMatrix());
		endless.RenderVisiblePlanes();

		glUseProgram(0);
		mainWindow.SwapBuffers();
		glfwPollEvents();
	}
	return 0;
}