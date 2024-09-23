#include <iostream>

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Renderer.h"
#include "Camera.h"

#define RenderFriquency 100 //100밀리 초 마다 한번
Renderer* G_Renderer = NULL;
Camera* G_Camera = nullptr;

GLvoid RenderScene()
{

}
GLvoid RenderSceneTimer(int value)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    G_Renderer->DrawScene();
    G_Camera->DoWorking();

    glutSwapBuffers();
    glutTimerFunc(RenderFriquency, RenderSceneTimer, 1);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //depth+
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("OpenGL_MyProject");

    /*glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) std::cerr << "NOT INIT" << std::endl;
    else std::cout << "INIT" << std::endl;*/
    glEnable(GL_DEPTH_TEST);
    glewInit();
    G_Renderer = new Renderer(500, 500);
    G_Camera = new Camera(G_Renderer, 0, 0, 1);
    G_Camera->SetLookLocation(0, 0, 0);

    glutDisplayFunc(RenderScene);
    glutTimerFunc(RenderFriquency, RenderSceneTimer, 1);
  
    //glutKeyboardFunc(KeyBoard);
    //glutKeyboardUpFunc(KeyBoardUP);
    //glutMouseFunc(Mouse);
    //glutMotionFunc(Motion);

    //glutReshapeFunc(Reshape);
    glutMainLoop();

    delete G_Renderer;
    delete G_Camera;
}