#include <iostream>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <stdlib.h>
#include <time.h>
#include <math.h>


#define window_width 600
#define window_high 600
#define VAO_Max 1
#define VBO_Max 2
#define random_num 100

#define cubesize 0.1

#define Map_length 40



//전역변수 구역 =======================================================

struct Ball_Option {
	float animation_timer = 0;
	float ball_radius = 0.1;
	float bounce_speed = 0.001;
	float bounce_height = 3;
	float bounce_scale_dinamic = 0.3;
};
Ball_Option BO;

struct Ball_Control {
	float ball_move_speed_x = 0.0;
	float ball_move_speed_z = 0.0;
	float move_speed_max = 0.015;

	float acceleration_x = 0.0002;
	float acceleration_z = 0.0002;
	bool acceleration_check = false;

	float delta_jump_power = 0.1;
	float jump_power = 0.02;
	bool keySPCAE_down = false;

	bool keyUP_down = false;
	bool keyDOWN_down = false;
	bool keyRIGHT_down = false;
	bool keyLEFT_down = false;
};
Ball_Control BC;

struct Map_Setting {
	//변수
	int road[Map_length][Map_length] = {
	  1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
	  0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
	  0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,
	  0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,
	  0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
	  0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
	  0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
	};
};
Map_Setting MS;

struct Camera_Setting {
	float cmera_rotate;
};
Camera_Setting CS;

float ball_move_x;
float ball_move_z;

float gravity_power = 3.0;

bool animation_timer_flag = true;
float ball_scale_endline_check;



float test1, test2, test3;

bool onoff_timer = true;
bool onoff_culling = false;
bool perspective = true;
bool isSolid = false;

using namespace std;
//====================================================================



//함수 구역===========================================================
GLvoid make_vertexShaders();
GLvoid make_fragmentShader();
GLvoid InitShader();
GLvoid InitBuffer();
GLchar* filetobuf(const GLchar* file);

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid KeyBoardDOWN(unsigned char Key, int x, int y);
GLvoid KeyBoardUP(unsigned char Key, int x, int y);
GLvoid Timer(int value);
GLvoid specialkeyDOWN(int key, int x, int y);
GLvoid specialkeyUP(int key, int x, int y);

float RandomColor(int color);
float Ball_Bounce_PosY(float ball_y, float timer);
float Ball_Bounce_Ani(char mode, float timer, float base_size);
void Make_Hurdle(float hurdle_x, float hurdle_y, float hurdle_z, float rotate);
//====================================================================

GLuint s_program;
GLuint VAO[VAO_Max], VBO[VBO_Max];
GLuint shaderID;
GLuint vertexShader;
GLuint fragmentShader;

int g_window_w;
int g_window_h;
float pi = 3.141592;

GLUquadricObj* qobj;

float cube[] = {
	//앞면
	-cubesize,cubesize,cubesize,
	cubesize,-cubesize,cubesize,
	-cubesize,-cubesize,cubesize,

	cubesize,-cubesize,cubesize,
	-cubesize,cubesize,cubesize,
	cubesize,cubesize,cubesize,

	//윗면
	-cubesize,cubesize,-cubesize,
	cubesize,cubesize,cubesize,
	-cubesize,cubesize,cubesize,

	cubesize,cubesize,-cubesize,
	cubesize,cubesize,cubesize,
	-cubesize,cubesize,-cubesize,

	//아랫면
	cubesize,-cubesize,cubesize,
	-cubesize,-cubesize,-cubesize,
	-cubesize,-cubesize,cubesize,

	-cubesize,-cubesize,-cubesize,
	cubesize,-cubesize,cubesize,
	cubesize,-cubesize,-cubesize,

	//뒷면
	cubesize,-cubesize,-cubesize,
	-cubesize,cubesize,-cubesize,
	-cubesize,-cubesize,-cubesize,

	-cubesize,cubesize,-cubesize,
	cubesize,-cubesize,-cubesize,
	cubesize,cubesize,-cubesize,

	//오른면
	cubesize,cubesize,cubesize,
	cubesize,-cubesize,-cubesize,
	cubesize,-cubesize,cubesize,

	cubesize,cubesize,cubesize,
	cubesize,cubesize,-cubesize,
	cubesize,-cubesize,-cubesize,

	//외면
	-cubesize,-cubesize,-cubesize,
	 -cubesize,cubesize,cubesize,
	-cubesize,-cubesize,cubesize,

	-cubesize,cubesize,-cubesize,
	-cubesize,cubesize,cubesize,
	-cubesize,-cubesize,-cubesize

};
float color[] = {
	//앞면
	0.4,0.4,0.4,
	0.4,0.4,0.4,
	0.4,0.4,0.4,

	0.4,0.4,0.4,
	0.4,0.4,0.4,
	0.4,0.4,0.4,

	//윗면
	0.6,0.6,0.6,
	0.6,0.6,0.6,
	0.6,0.6,0.6,

	0.6,0.6,0.6,
	0.6,0.6,0.6,
	0.6,0.6,0.6,

	//아랫면
	0.1,0.1,0.1,
	0.1,0.1,0.1,
	0.1,0.1,0.1,

	0.1,0.1,0.1,
	0.1,0.1,0.1,
	0.1,0.1,0.1,

	//뒷면
	0.2,0.2,0.2,
	0.2,0.2,0.2,
	0.2,0.2,0.2,

	0.2,0.2,0.2,
	0.2,0.2,0.2,
	0.2,0.2,0.2,

	//오른면
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,

	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,

	//왼면
	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3,

	0.3,0.3,0.3,
	0.3,0.3,0.3,
	0.3,0.3,0.3

};

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //depth+
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_width, window_high);
	glutCreateWindow("Example 1");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cerr << "NOT INIT" << endl;
	}
	else
		cout << "INIT<<endl";

	srand((unsigned)time(NULL));


	glEnable(GL_DEPTH_TEST);

	InitShader();
	InitBuffer();
	glutTimerFunc(70, Timer, 1);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(KeyBoardDOWN);
	glutKeyboardUpFunc(KeyBoardUP);
	glutSpecialFunc(specialkeyDOWN);
	glutSpecialUpFunc(specialkeyUP);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{
	GLfloat rColor = 1.0;
	GLfloat gColor = 1.0;
	GLfloat bColor = 1.0;
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //깊이 체크 (컬링)
	InitBuffer();


	glUseProgram(s_program);
	int objColorLocation = glGetUniformLocation(s_program, "objectColor");
	unsigned isCheck = glGetUniformLocation(s_program, "isCheck");
	glm::mat4 S_Matrix = glm::mat4(1.0f);
	unsigned int EllipsetransformLocation = glGetUniformLocation(s_program, "transform");


	//카메라세팅 =============================================================
	glm::vec3 cameraPos = glm::vec3(0.0 + ball_move_x, 0.0, 2.0 + ball_move_z); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(ball_move_x, 0.0, ball_move_z); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	view = glm::rotate(view, glm::radians(CS.cmera_rotate), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


	//원근법 유무(Perspective = 원근투영)
	if (perspective == false) {
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f); //--- 투영 공간을 [-100.0, 100.0] 공간으로 설정
		unsigned int projectionLocation = glGetUniformLocation(s_program, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
	else {

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- 공간을 약간 뒤로 미뤄줌
		unsigned int projectionLocation = glGetUniformLocation(s_program, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
		//cout << "peron" << endl;
	}


	//은면제거
	if (onoff_culling == false) {
		glDisable(GL_CULL_FACE);
	}
	else {
		glEnable(GL_CULL_FACE);
	}
	//========================================================================



	//bottom ================================================================= ( y == 0인 좌표) 
	glBindVertexArray(VAO[0]);
	S_Matrix = glm::mat4(1.0f);
	S_Matrix = glm::translate(S_Matrix, glm::vec3(0.0, -0.1, 0.0));
	S_Matrix = glm::rotate(S_Matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	S_Matrix = glm::scale(S_Matrix, glm::vec3(20.0, 20.0, 1.0));
	EllipsetransformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(EllipsetransformLocation, 1, GL_FALSE, glm::value_ptr(S_Matrix));
	glUniform1f(isCheck, true);
	/*glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 0.7f, 0.8f, 0.2f, 1.0);*/
	glDrawArrays(GL_TRIANGLES, 0,36);
	//========================================================================




	//ball ===================================================================
	S_Matrix = glm::mat4(1.0f);
	S_Matrix = glm::translate(S_Matrix, glm::vec3(ball_move_x, Ball_Bounce_PosY(0.0, BO.animation_timer), ball_move_z));
	S_Matrix = glm::rotate(S_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(0.0, 0.0, 0.0)); // 스케일 피봇 설정
	S_Matrix = glm::scale(S_Matrix, glm::vec3(Ball_Bounce_Ani('x', 1.0, BO.animation_timer), Ball_Bounce_Ani('y', 1.0, BO.animation_timer), Ball_Bounce_Ani('z', 1.0, BO.animation_timer)));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(0.0, 0.1, 0.0)); // 스케일 피봇 설정
	EllipsetransformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(EllipsetransformLocation, 1, GL_FALSE, glm::value_ptr(S_Matrix));
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 0.0f, 0.0f, 1.0);
	gluSphere(qobj, BO.ball_radius, 30, 30);
	//========================================================================


	//map ====================================================================
	for (int i = 0; i < Map_length; ++i) {
		for (int j = 0; j < Map_length; ++j) {
			glm::mat4 M_Matrix = glm::mat4(1.0f);
			if (MS.road[i][j] == 1) {
				M_Matrix = glm::translate(M_Matrix, glm::vec3(cubesize * j, 0.0, cubesize * i));  //z방향 음으로 이동 
				M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, -0.1, 0.0));  //원점으로 이동 
				M_Matrix = glm::scale(M_Matrix, glm::vec3(1.0, 0.2, 1.0)); // 
				int objColorLocation = glGetUniformLocation(s_program, "objectColor");
				unsigned isCheck = glGetUniformLocation(s_program, "isCheck");
				glUniform1f(isCheck, true);
				unsigned int FloortransformLocation = glGetUniformLocation(s_program, "transform");
				glUniformMatrix4fv(FloortransformLocation, 1, GL_FALSE, glm::value_ptr(M_Matrix));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}
	//========================================================================


	//hurdle =================================================================
	Make_Hurdle(0.0, 0.0, 0.0, 0.0);
	//========================================================================


	glutSwapBuffers();
}

GLvoid KeyBoardDOWN(unsigned char Key, int x, int y)
{
	switch (Key) {

	case ' ':
		BC.keySPCAE_down = true;
		cout << "dd" << endl;
		break;

		//test구역=====================================
	case 'u':
		test1 += 0.1f;
		BO.bounce_height += 0.1f;
		cout << "test1= " << BO.bounce_height << endl;
		break;
	case 'j':
		test1 -= 0.1f;
		BO.bounce_height -= 0.1f;
		cout << "test1= " << BO.bounce_height << endl;
		break;

	case 'i':
		test2 += 1.0f;
		cout << "test2= " << test2 << endl;
		break;
	case 'k':
		test2 -= 1.0f;
		cout << "test2= " << test2 << endl;
		break;
		//==============================================
	case 'p':
		cout << "원근투영" << endl;
		perspective = true;
		break;
	case 'o':
		cout << "직각투영" << endl;
		perspective = false;
		break;

		/*case 'h':
			if (onoff_culling == false) {
				onoff_culling = true;
			}
			else
				onoff_culling = false;
			break;*/

	case 'q':
		exit(1);
		break;
	}
	glutPostRedisplay();
}
GLvoid KeyBoardUP(unsigned char Key, int x, int y) {
	switch (Key) {
	case ' ':
		BC.keySPCAE_down = false;
		cout << "ss" << endl;
		break;
	}
}
GLvoid specialkeyDOWN(int key, int x, int y) {
	switch (key) {

	case (GLUT_KEY_UP):
		BC.acceleration_check = true;
		BC.keyUP_down = true;
		break;

	case (GLUT_KEY_DOWN):
		BC.acceleration_check = true;
		BC.keyDOWN_down = true;
		break;

	case (GLUT_KEY_RIGHT):
		BC.acceleration_check = true;
		BC.keyRIGHT_down = true;
		break;

	case (GLUT_KEY_LEFT):
		BC.acceleration_check = true;
		BC.keyLEFT_down = true;
		break;
	}

	glutPostRedisplay();
}
GLvoid specialkeyUP(int key, int x, int y) {
	switch (key) {

	case (GLUT_KEY_UP):
		BC.acceleration_check = false;
		BC.keyUP_down = false;
		break;

	case (GLUT_KEY_DOWN):
		BC.acceleration_check = false;
		BC.keyDOWN_down = false;
		break;

	case (GLUT_KEY_RIGHT):
		BC.acceleration_check = false;
		BC.keyRIGHT_down = false;
		break;

	case (GLUT_KEY_LEFT):
		BC.acceleration_check = false;
		BC.keyLEFT_down = false;
		break;
	}
}
GLvoid Timer(int value)
{
	if (onoff_timer == true) {

		//bounce animation===================================================
		BO.animation_timer += BO.bounce_speed;
		if (BO.animation_timer > 1.0) {
			BO.animation_timer = 0;
			animation_timer_flag = true;
		}
		//===================================================================


		//move animation=====================================================
		if (BC.keyUP_down) {
			BC.ball_move_speed_z -= BC.acceleration_z;
		}
		if (BC.keyDOWN_down) {
			BC.ball_move_speed_z += BC.acceleration_z;
		}

		if (BC.keyRIGHT_down) {
			BC.ball_move_speed_x += BC.acceleration_x;
		}

		if (BC.keyLEFT_down) {
			BC.ball_move_speed_x -= BC.acceleration_x;
		}

		if (BC.acceleration_check == false) {
			if (BC.ball_move_speed_x < 0) {
				BC.ball_move_speed_x += BC.acceleration_x;
			}
			else if (BC.ball_move_speed_x > 0) {
				BC.ball_move_speed_x -= BC.acceleration_x;
			}

			if (BC.ball_move_speed_z < 0) {
				BC.ball_move_speed_z += BC.acceleration_z;
			}
			else if (BC.ball_move_speed_z > 0) {
				BC.ball_move_speed_z -= BC.acceleration_z;
			}
		}

		if (BC.ball_move_speed_x > BC.move_speed_max) BC.ball_move_speed_x = BC.move_speed_max;
		else if (BC.ball_move_speed_x < -BC.move_speed_max) BC.ball_move_speed_x = -BC.move_speed_max;
		if (BC.ball_move_speed_z > BC.move_speed_max) BC.ball_move_speed_z = BC.move_speed_max;
		else if (BC.ball_move_speed_z < -BC.move_speed_max) BC.ball_move_speed_z = -BC.move_speed_max;

		ball_move_x += BC.ball_move_speed_x;
		ball_move_z += BC.ball_move_speed_z;
		//===================================================================


		//ball jump control==================================================
		if (BC.keySPCAE_down) {
			BO.bounce_height += BC.jump_power;
		}
		else {
			BO.bounce_height -= BC.jump_power;
			if (BO.bounce_height < 3.0) {
				BO.bounce_height = 3.0;
			}
		}

		//===================================================================
	}

	glutPostRedisplay();
	glutTimerFunc(10, Timer, 1);
}

float RandomColor(int color) {
	return (float)(rand() % random_num) / 100 - 0.5;
}

float Ball_Bounce_PosY(float ball_y, float timer) {
	// Ball의 기존 위치에서부터 시작하여 bounce하는 Ball의 위치를 계산하여 return 해주는 함수
	// 대신 공의 반지름은 0.1기준

	float delta_y; // Ball 움직임의 y 변화량 (시작점부터 최조점까지)
	float gravity_y; // 최고점에 도달 한 후 이후에 적용되는 y변화량

	float result_y = 0.0;

	delta_y   = -1 * (BO.bounce_height) * timer * (timer - 1) - BO.ball_radius;
	gravity_y = -1 * (gravity_power)    * timer * (timer - 1);


	//ball 스케일 변화를 위한 flag
	if (animation_timer_flag == true && delta_y >= BO.ball_radius) {
		ball_scale_endline_check = timer;
		animation_timer_flag = false;
	}

	//바닥과 충돌처리
	if (delta_y < 0.0) {
		delta_y = 0.0;
		gravity_y = 0.0;
	}
	
	result_y = ball_y + delta_y;
	/*if (timer <= 0.5) {
		
	}
	else if(timer > 0.5) {
		
	}*/
	return result_y;
}
float Ball_Bounce_Ani(char mode, float base_size, float timer) {
	//공이 바닥에 닿을 때, 스케일 값 변환 해 주는 함수

	switch (mode) {

	case('x'):
		if (timer <= ball_scale_endline_check) {
			base_size = -1 * (BO.bounce_scale_dinamic * BO.bounce_height / 3) / (ball_scale_endline_check * ball_scale_endline_check) * timer * timer + base_size + (BO.bounce_scale_dinamic * BO.bounce_height / 3);
		}
		else if (1.0 - ball_scale_endline_check <= timer) {
			base_size = -1 * (BO.bounce_scale_dinamic * BO.bounce_height / 3) / (ball_scale_endline_check * ball_scale_endline_check) * (timer - 1) * (timer - 1) + base_size + (BO.bounce_scale_dinamic * BO.bounce_height / 3);
		}
		return base_size;
		break;


	case('z'):
		if (timer <= ball_scale_endline_check) {
			base_size = -1 * (BO.bounce_scale_dinamic * BO.bounce_height / 3) / (ball_scale_endline_check * ball_scale_endline_check) * timer * timer + base_size + (BO.bounce_scale_dinamic * BO.bounce_height / 3);
		}
		else if (1.0 - ball_scale_endline_check <= timer) {
			base_size = -1 * (BO.bounce_scale_dinamic * BO.bounce_height / 3) / (ball_scale_endline_check * ball_scale_endline_check) * (timer - 1) * (timer - 1) + base_size + (BO.bounce_scale_dinamic * BO.bounce_height / 3);
		}
		return base_size;
		break;

	case('y'):
		if (timer <= ball_scale_endline_check) {
			base_size = 1 * (BO.bounce_scale_dinamic * BO.bounce_height / 3) / (ball_scale_endline_check * ball_scale_endline_check) * timer * timer + base_size - (BO.bounce_scale_dinamic * BO.bounce_height / 3);
		}
		else if (1.0 - ball_scale_endline_check <= timer) {
			base_size = 1 * (BO.bounce_scale_dinamic * BO.bounce_height / 3) / (ball_scale_endline_check * ball_scale_endline_check) * (timer - 1) * (timer - 1) + base_size - (BO.bounce_scale_dinamic * BO.bounce_height / 3);
		}
		return base_size;
		break;
	}

}

void Make_Hurdle(float hurdle_x, float hurdle_y, float hurdle_z, float rotate) {

	glUseProgram(s_program);
	int objColorLocation = glGetUniformLocation(s_program, "objectColor");
	unsigned isCheck = glGetUniformLocation(s_program, "isCheck");
	glm::mat4 S_Matrix = glm::mat4(1.0f);
	unsigned int EllipsetransformLocation = glGetUniformLocation(s_program, "transform");

	//hurdle_base
	glBindVertexArray(VAO[0]);
	S_Matrix = glm::mat4(1.0f);
	S_Matrix = glm::rotate(S_Matrix, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(hurdle_x, hurdle_y, hurdle_z));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(0.0, 0.0, 0.0));
	S_Matrix = glm::rotate(S_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	S_Matrix = glm::scale(S_Matrix, glm::vec3(4.0, 3.5, 0.1));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(0.0, 0.1, 0.0)); // 스케일 피봇 설정
	EllipsetransformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(EllipsetransformLocation, 1, GL_FALSE, glm::value_ptr(S_Matrix));
	//glUniform1f(isCheck, true);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 1.0f, 0.0f, 1.0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//hurdle_r_stick
	glBindVertexArray(VAO[0]);
	S_Matrix = glm::mat4(1.0f);
	S_Matrix = glm::rotate(S_Matrix, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(hurdle_x, hurdle_y, hurdle_z));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(0.4, 0.0, 0.0));
	S_Matrix = glm::rotate(S_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	S_Matrix = glm::scale(S_Matrix, glm::vec3(0.2, 4.0, 0.2));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(0.0, 0.1, 0.0)); // 스케일 피봇 설정
	EllipsetransformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(EllipsetransformLocation, 1, GL_FALSE, glm::value_ptr(S_Matrix));
	//glUniform1f(isCheck, true);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 0.0f, 0.0f, 0.0f, 1.0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//hurdle_r_stick_end
	glBindVertexArray(VAO[0]);
	S_Matrix = glm::mat4(1.0f);
	S_Matrix = glm::rotate(S_Matrix, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(hurdle_x, hurdle_y, hurdle_z));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(0.4, 0.8, 0.0));
	S_Matrix = glm::rotate(S_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	S_Matrix = glm::scale(S_Matrix, glm::vec3(0.3, 0.3, 0.3));
	EllipsetransformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(EllipsetransformLocation, 1, GL_FALSE, glm::value_ptr(S_Matrix));
	//glUniform1f(isCheck, true);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 0.0f, 0.0f, 1.0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//hurdle_l_stick
	glBindVertexArray(VAO[0]);
	S_Matrix = glm::mat4(1.0f);
	S_Matrix = glm::rotate(S_Matrix, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(hurdle_x, hurdle_y, hurdle_z));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(-0.4, 0.0, 0.0));
	S_Matrix = glm::rotate(S_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	S_Matrix = glm::scale(S_Matrix, glm::vec3(0.2, 4.0, 0.2));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(0.0, 0.1, 0.0)); // 스케일 피봇 설정
	EllipsetransformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(EllipsetransformLocation, 1, GL_FALSE, glm::value_ptr(S_Matrix));
	//glUniform1f(isCheck, true);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 0.0f, 0.0f, 0.0f, 1.0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// hurdle_l_stick_end
	glBindVertexArray(VAO[0]);
	S_Matrix = glm::mat4(1.0f);
	S_Matrix = glm::rotate(S_Matrix, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(hurdle_x, hurdle_y, hurdle_z));
	S_Matrix = glm::translate(S_Matrix, glm::vec3(-0.4, 0.8, 0.0));
	S_Matrix = glm::rotate(S_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	S_Matrix = glm::scale(S_Matrix, glm::vec3(0.3, 0.3, 0.3));
	EllipsetransformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(EllipsetransformLocation, 1, GL_FALSE, glm::value_ptr(S_Matrix));
	//glUniform1f(isCheck, true);
	glUniform1f(isCheck, false);
	glUniform4f(objColorLocation, 1.0f, 0.0f, 0.0f, 1.0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

GLvoid make_vertexShaders()
{
	GLchar* vertexSource;
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

}
GLvoid make_fragmentShader()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

}
GLvoid InitShader()
{
	make_vertexShaders();
	make_fragmentShader();

	s_program = glCreateProgram();

	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR :  fragment Shader Fail Compile \n" << errorLog << endl;
		exit(-1);
	}

	else
		cout << "good";
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(s_program);
}
GLvoid InitBuffer()
{
	glGenVertexArrays(VAO_Max, VAO);
	glGenBuffers(VBO_Max, VBO);

	//Cube
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//->color
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); // 색상
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	////조명
	//glUseProgram(s_program);
	//int lightColorLocation = glGetUniformLocation(s_program, "lightColor");  //lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	//glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	//int objColorLocation = glGetUniformLocation(s_program, "objectColor");  //object Color값 전달: (1.0, 0.5, 0.3)의 색
	//glUniform3f(objColorLocation, 1.0, 0.5, 0.3);
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}
GLchar* filetobuf(const GLchar* file)
{
	FILE* fptr;
	long length;
	GLchar* buf;

	fopen_s(&fptr, file, "rb");
	if (!fptr)
		return NULL;

	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (GLchar*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}