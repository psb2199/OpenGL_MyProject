#include <iostream>

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Renderer.h"
#include "Camera.h"
#include "Controller.h"
#include "ObjectManager.h"

#define RenderFriquency 100 //100�и� �� ���� �ѹ�
Renderer* G_Renderer = NULL;
Camera* G_Camera = nullptr;
Controller* G_Controller = nullptr;
ObjectManager* G_ObjMgr = nullptr;
Object* Player = nullptr;

GLvoid RenderScene()
{

}
GLvoid RenderSceneTimer(int value)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    G_Renderer->DrawScene(G_ObjMgr->GetAllObjects());
    G_Controller->TickEvent();
 

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

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //depth+
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("OpenGL_MyProject");

    glewInit();
    G_Renderer = new Renderer(500, 500);
    G_Camera = new Camera(G_Renderer, 0, 0, 1);
    G_Controller = new Controller;
    G_ObjMgr = new ObjectManager;

    Player = G_ObjMgr->AddObject("Player", { 0,0,0 });
    G_Controller->MappingController(Player);

    glutDisplayFunc(RenderScene);
    glutTimerFunc(RenderFriquency, RenderSceneTimer, 1);
  
    glutKeyboardFunc(KeyBoardDown);
    glutKeyboardUpFunc(KeyBoardUP);
    //glutMouseFunc(Mouse);
    //glutMotionFunc(Motion);

    //glutReshapeFunc(Reshape);
    glutMainLoop();

    delete G_Renderer;
    delete G_Camera;
    delete G_Controller;
}

