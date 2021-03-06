// main.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opengl.h"
#include "mainconfig.h"
#include "game.h"

mat4 projection;
Game game;
ShaderManager* shader;

void init();
void display();
void reshape(int w, int h);
void idle();
void mouse(int button, int state, int x, int y);
void mouse_motion(int x, int y);
void special(int key, int x, int y);
void keyboard(unsigned char button, int x, int y);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL打砖块");
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
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

void init() 
{
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glClearColor(0.5, 0.5, 0.5, 1.0);
	shader = ShaderManager::NewInstance();
	shader->load(
		shader_paths[0],
		shader_paths[1],
		shader_paths[2]);
	game.refreshWindow(window_width, window_height);
	game.init(shader);
	
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	game.update();
	shader->setUniformMatrix4fv("projection", 1, value_ptr(projection));
	game.render();
	
	glutSwapBuffers();
}

void reshape(int w, int h) 
{
	game.realign(float(w) / window_width, float(h) / window_height);
	window_width = w;
	window_height = h;
	game.refreshWindow(w, h);
	projection = ortho(0.0f, (float)window_width, (float)window_height, 0.0f,  -1.0f, 1.0f);
	glViewport(0, 0, window_width, window_height);
}

void idle() 
{
	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y) {}
void mouse_motion(int x, int y){}
void special(int key, int x, int y) {}
void keyboard(unsigned char button, int x, int y) 
{
	game.processButton(button);
}

