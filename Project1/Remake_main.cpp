#include <iostream>

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Renderer.h"
#include "Camera.h"
#include "Controller.h"

#define RenderFriquency 100 //100밀리 초 마다 한번
Renderer* G_Renderer = NULL;
Camera* G_Camera = nullptr;
Controller* G_Controller = nullptr;

GLvoid RenderScene()
{

}
GLvoid RenderSceneTimer(int value)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    G_Renderer->DrawScene();
    //G_Camera->DoWorking();
    G_Controller->Debug_print();

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

    glEnable(GL_DEPTH_TEST);
    G_Renderer = new Renderer(500, 500);
    G_Camera = new Camera(G_Renderer, 0, 0, 1);
    G_Controller = new Controller;

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

