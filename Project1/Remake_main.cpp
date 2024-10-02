#include <iostream>

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <Windows.h>

#include "Renderer.h"
#include "Camera.h"
#include "Controller.h"
#include "ObjectManager.h"
#include "Importer_obj.h"


#define RenderFriquency 10 //100밀리 초 마다 한번
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

Renderer* G_Renderer = NULL;
ObjectManager* G_ObjMgr = nullptr;
Importer_obj* G_Importer = nullptr;

Controller* G_Controller = nullptr;
Camera* G_Camera = nullptr;
Object* Player = nullptr;

GLvoid RenderScene();
GLvoid RenderSceneTimer(int value);

GLvoid KeyBoardDown(unsigned char Key, int x, int y);
GLvoid KeyBoardUP(unsigned char Key, int x, int y);

void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void FixMouseInSrcreen(glm::vec2& get_mouse_delta);
bool DoFixMouse{ true };

void LevelDisign();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //depth+
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("OpenGL_MyProject");

	glewInit();
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS); // or glDepthFunc(GL_LEQUAL);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);

	G_Renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
	G_Camera = new Camera(G_Renderer, glm::vec3(0.f));
	G_Controller = new Controller;
	G_Importer = new Importer_obj;
	G_ObjMgr = new ObjectManager(G_Importer);

	Player = G_ObjMgr->AddObject("Player", { 0,0,0 });
	G_Controller->MappingController(Player);

	glutSetCursor(GLUT_CURSOR_NONE);
	ShowCursor(FALSE);

	LevelDisign();

	glutDisplayFunc(RenderScene);
	glutTimerFunc(RenderFriquency, RenderSceneTimer, 1);

	glutKeyboardFunc(KeyBoardDown);
	glutKeyboardUpFunc(KeyBoardUP);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	//glutReshapeFunc(Reshape);
	glutMainLoop();

	delete G_Renderer;
	delete G_Camera;
	delete G_Controller;
}

GLvoid RenderScene()
{

}
GLvoid RenderSceneTimer(int value)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	G_Renderer->DrawScene(G_ObjMgr->GetAllObjects());
	G_Camera->DoWorking(G_Renderer);

	G_Controller->TickEvent();
	
	if (DoFixMouse)
	{	
		glm::vec2 mouse_Delta{ 0 };
		FixMouseInSrcreen(mouse_Delta);
		G_Camera->BindWithMouseRotation(mouse_Delta);
	}

	glutSwapBuffers();
	glutTimerFunc(RenderFriquency, RenderSceneTimer, 1);
}

GLvoid KeyBoardDown(unsigned char Key, int x, int y)
{
	if (G_Controller)
	{
		switch (Key)
		{
		case 'W':
		case 'w':
			G_Controller->Key[press(w)] = true;
			break;

		case 'S':
		case 's':
			G_Controller->Key[press(s)] = true;
			break;

		case 'A':
		case 'a':
			G_Controller->Key[press(a)] = true;
			break;

		case 'D':
		case 'd':
			G_Controller->Key[press(d)] = true;
			break;

		case 'u':
		case 'U':

			break;

		case 'P':
		case 'p':
			if (DoFixMouse)
			{
				glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
				DoFixMouse = false;
			}
			else 
			{ 
				glutSetCursor(GLUT_CURSOR_NONE);
				DoFixMouse = true; 
			}
			break;

		case 27:
			exit(0);
			break;
		}
	}
}
GLvoid KeyBoardUP(unsigned char Key, int x, int y)
{
	if (G_Controller)
	{
		switch (Key)
		{
		case 'W':
		case 'w':
			G_Controller->Key[press(w)] = false;
			break;

		case 'S':
		case 's':
			G_Controller->Key[press(s)] = false;
			break;

		case 'A':
		case 'a':
			G_Controller->Key[press(a)] = false;
			break;

		case 'D':
		case 'd':
			G_Controller->Key[press(d)] = false;
			break;
		}
	}
}

void Mouse(int button, int state, int x, int y)
{
	if (button == 3) G_Camera->BindWithMouseWheel(-1.f);
	else if (button == 4)G_Camera->BindWithMouseWheel(1.f);
}
void Motion(int x, int y)
{
	
}
void FixMouseInSrcreen(glm::vec2 &get_mouse_delta)
{
	glm::vec2 point{ 500,500 };

	POINT mousepos;
	GetCursorPos(&mousepos);
	
	glm::vec2 deltaPos;
	deltaPos.x = -point.x + mousepos.x;
	deltaPos.y = point.y - mousepos.y;

	get_mouse_delta = deltaPos;
	SetCursorPos(point.x, point.y);
}


void LevelDisign()
{
	//G_ObjMgr->AddObject("Base", { 0,0,0 });
	Light* newLight = new Light({ 0, 0, 3 });
	G_Renderer->SetLight(newLight);
}


