#include <iostream>

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Renderer.h"

#define RenderFriquency 100 //100밀리 초 마다 한번

GLvoid RenderScene()
{

}
GLvoid RenderSceneTimer(int value)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);


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
    //glEnable(GL_DEPTH_TEST);

    Renderer* G_Renderer = new Renderer(100, 100);

    glutDisplayFunc(RenderScene);
    glutTimerFunc(RenderFriquency, RenderSceneTimer, 1);
  
    //glutKeyboardFunc(KeyBoard);
    //glutKeyboardUpFunc(KeyBoardUP);
    //glutMouseFunc(Mouse);
    //glutMotionFunc(Motion);

    //glutReshapeFunc(Reshape);
    glutMainLoop();

    delete G_Renderer;
}