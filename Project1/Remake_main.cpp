#include <iostream>

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <Windows.h>

#include "OpenGL.h"

#include "Renderer.h"
#include "Camera.h"
#include "DirectionLight.h"
#include "Controller.h"

#include "ObjectManager.h"


#define RenderFriquency 10 //100밀리 초 마다 한번
#define WINDOW_WIDTH 1900
#define WINDOW_HEIGHT 1000

float WorldSeconds{ 0.0 };

Renderer* G_Renderer = NULL;
DirectionLight* G_Light = nullptr;

ObjectManager* G_ObjMgr = nullptr;
Importer* G_Importer = nullptr;

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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE); //depth+
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("OpenGL_MyProject");

	glewInit();
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS); // or glDepthFunc(GL_LEQUAL);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);*/
	


	G_Importer = new Importer;
	G_Renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT, G_Importer);
	G_Camera = new Camera(glm::vec3(0.f));
	G_Controller = new Controller;
	G_ObjMgr = new ObjectManager(G_Importer);

	Player = G_ObjMgr->AddObject(type_Player, { 0,5.0,0 });
	Player->SetCamera(G_Camera);
	G_Controller->MappingController(Player);
	G_Renderer->SetCamera(G_Camera);

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

	G_Renderer->DrawScene(G_ObjMgr->GetAllObjects());

	G_Controller->TickEvent((float)RenderFriquency / 1000.0);
	for (auto& v : G_ObjMgr->GetAllObjects()) { v->TickEvent((float)RenderFriquency / 1000.0); }

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
			Player->SetLocation({ 0,5.0,0 });
			Player->SetVelocity({ 0,0,0 });
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
	int window_center_x = glutGet(GLUT_WINDOW_X) + glutGet(GLUT_WINDOW_WIDTH) / 2;
	int window_center_y = glutGet(GLUT_WINDOW_Y) + glutGet(GLUT_WINDOW_HEIGHT) / 2;

	glm::vec2 point{ window_center_x, window_center_y };

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
	G_Light = new DirectionLight({ 5, 5, 5 });
	G_Light->AttachDirectionLight(Player);

	int range = 3;
	
	for (int x{ -range }; x <= range; ++x)
	{
		for (int z{ -range }; z <= range; ++z)
		{
			G_ObjMgr->AddObject(type_Coin, { x * 5, 2, z * 5 });
		}
	}
	/*G_ObjMgr->AddObject("Coin", { 0, 2, 5});
	G_ObjMgr->AddObject("Coin", { 5, 2, 0});*/
	
	G_ObjMgr->AddObject(type_Base, { 0, 0, 0 });
	
	

	G_Renderer->SetLight(G_Light);
	G_Camera->SetLookLocation(Player->GetLocation());
}


