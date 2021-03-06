// Assimp.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opengl.h"
#include "camera.h"
#include "shader.h"
#include "light.h"
#include "scene.h"
#include "skybox.h"
#include "action.h"
#include "script.h"

Camera* camera;
ShaderManager *shader_scene, *shader_sky;
LightManager lights;
Scene scene;
Skybox skybox;

mat4 projection;

#pragma region 资源路径
char shader_scene_path[3][27] = {
	"shader/scene_lighting.vert",
	"",
	"shader/scene_lighting.frag" };
char shader_skybox_path[3][19] = {
	"shader/skybox.vert",
	"",
	"shader/skybox.frag"
};
vector<string> skybox_path = {
	"skybox/skybox2/right.jpg",
	"skybox/skybox2/left.jpg",
	"skybox/skybox2/top.jpg",
	"skybox/skybox2/bottom.jpg",
	"skybox/skybox2/front.jpg",
	"skybox/skybox2/back.jpg"
};
#pragma endregion

#pragma region screen
GLint window_width = 1024, window_height = 768;
GLint view_width = 1024, view_height = 768;
#pragma endregion

void renderScene(ShaderManager* shader);

void init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1.0);

	shader_sky = ShaderManager::NewInstance();
	shader_sky->load(shader_skybox_path[0], shader_skybox_path[1], shader_skybox_path[2]);
	shader_sky->use();

	skybox.init(skybox_path);

	shader_scene = ShaderManager::NewInstance();
	shader_scene->load(shader_scene_path[0], shader_scene_path[1], shader_scene_path[2]);
	shader_scene->use();

	scene.init();

	UnicornBase* player = Tag::getObjectByTag("Player");
	player->addComponent(new PlayerScript());

	UnicornBase* cameraObj = Tag::getObjectByTag("Camera");
	camera = new GameCamera();
	cameraObj->addComponent(camera);
	cameraObj->addComponent(new CameraScript());

	lights.initData();
	lights.setRenderFunc(renderScene);

}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader_scene->use();
	lights.refresh(shader_scene);
	shader_scene->setUniformMatrix4fv("view", 1, value_ptr(camera->view));
	shader_scene->setUniformMatrix4fv("projection", 1, value_ptr(projection));
	renderScene(shader_scene);

	shader_sky->use();
	shader_sky->setUniformMatrix4fv("view", 1, value_ptr(camera->view));
	shader_sky->setUniformMatrix4fv("projection", 1, value_ptr(projection));
	skybox.draw(shader_sky);
	
	glutSwapBuffers();
}


void reshape(int w, int h)
{
	window_width = w;
	window_height = h;
	projection = perspective(radians(90.0f), float(view_width / view_height), 0.1f, 300.0f);
	//projection = ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 200.0f);
}

void idle() {
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	Mouse::updateMouse(button, state, x, y);
}

void mouse_motion(int x, int y) {
	
}

void special(int key, int x, int y) {
	Keyboard::updateSpecial(key);
}

void specialUp(int key, int x, int y) {
	Keyboard::updateUpSpecial(key);
}

void keyboard(unsigned char button, int x, int y) {
	Keyboard::updateButton(button);
	switch (button)
	{
	case '1':
		lights.setDirectionalEnable("ld0", !(lights.getDirectional("ld0")->getEnable()));
		break;
	case '2':
		lights.setPointEnable("lp0", !lights.getPoint("lp0")->getEnable());
		break;
	case '3':
		lights.setSpotEnable("ls0", !lights.getSpot("ls0")->getEnable());
		break;
	case '4':
		break;
	default:
		break;
	}
}

void keyboardUp(unsigned char button, int x, int y) {
	Keyboard::updateUpButton(button);
}

void renderScene(ShaderManager* shader) 
{
	scene.draw(shader, vec3(0.01f));
	
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL 坦克大战");
	GLuint result = glewInit();
	if (result != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(result));
		return 1;
	}
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutMainLoop();
	return 0;
}