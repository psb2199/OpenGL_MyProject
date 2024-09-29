#include <iostream>

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Renderer.h"
#include "Camera.h"
#include "Controller.h"
#include "ObjectManager.h"
#include "Importer_obj.h"

#define RenderFriquency 10 //100밀리 초 마다 한번
Renderer* G_Renderer = NULL;
ObjectManager* G_ObjMgr = nullptr;
Importer_obj* G_Importer = nullptr;

Controller* G_Controller = nullptr;
Camera* G_Camera = nullptr;
Object* Player = nullptr;


GLvoid RenderScene()
{

}
GLvoid RenderSceneTimer(int value)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    G_Renderer->DrawScene(G_ObjMgr->GetAllObjects());
    G_Camera->DoWorking(G_Renderer);

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

        case 'P':
        case 'p':

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
    int window_width{ 700 };
    int window_height{ 700 };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //depth+
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("OpenGL_MyProject");

    glewInit();
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    //glEnable(GL_CULL_FACE);

    G_Renderer = new Renderer(window_width, window_height);
    G_Camera = new Camera(G_Renderer, 2, 2, 2);
    G_Controller = new Controller;
    G_Importer = new Importer_obj;
    G_ObjMgr = new ObjectManager(G_Importer);
    
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

