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

#define Map_length 41

#define Acceleration 0.0002

//전역변수 구역 =======================================================
struct Map_Setting {
    //변수
    int road[Map_length][Map_length] = {
      1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      1,11,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   // 파이널 지점 [2][20] 
      1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,3,0,0,0,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,2,0,0,2,0,0,2,5,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,5,0,4,0,4,6,0,0,6,0,4,0,3,0,0,3,0,4,0,3,0,0,0,0,0,0,0,7,7,7,0,0,
      0,0,0,9,0,8,0,0,0,0,8,0,5,0,4,1,0,0,0,0,1,4,0,0,0,0,0,0,0,0,4,1,1,1,1,1,7,10,7,0,0,
      0,0,0,0,0,0,0,7,0,5,0,4,0,4,6,0,0,3,0,4,0,6,0,0,3,0,4,0,3,0,0,0,0,0,0,0,7,7,7,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,2,0,0,5,0,0,2,2,0,0,2,0,0,5,0,0,0,0,0,0,0,9,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,3,0,0,0,0,3,0,3,0,0,0,0,0,0,0,0,0,6,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,
      0,0,0,0,0,0,8,8,8,8,8,8,8,0,0,0,2,0,3,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,
      0,0,0,3,3,0,0,0,0,0,0,0,0,0,0,0,2,0,3,0,1,0,2,0,5,6,5,6,1,4,3,2,5,2,0,0,0,0,0,0,0,   // 두번 째 리스폰 지역   [15][20]
      0,0,0,2,0,0,8,8,8,8,8,8,8,0,0,0,2,0,3,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,1,1,1,0,1,1,0,4,0,4,0,0,0,4,4,1,4,1,4,1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,
      0,0,1,10,1,1,1,1,1,0,1,1,0,1,0,0,0,4,1,4,1,1,1,1,2,4,1,4,5,6,5,4,5,3,2,1,1,10,1,0,0,
      0,0,1,1,1,0,4,0,1,0,4,0,4,0,4,0,4,4,0,0,1,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,4,4,1,2,3,6,3,2,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,4,1,1,1,2,5,2,5,3,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,4,4,0,0,0,0,0,0,0,1,2,3,0,0,0,2,5,0,1,10,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   // 첫번 째 리스폰 지역 [35][20]
      3,4,0,0,0,0,0,0,0,0,0,0,1,2,0,5,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      3,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      3,3,2,5,2,5,2,1,3,2,3,6,3,6,3,6,3,6,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 //40.20
    };

    float first_x;
    float first_z;

    float tile_size = 0.4;

    float shaking_tile[Map_length][Map_length];  //흔들리는 회전 각도 표현
    float delta_trans_y[Map_length][Map_length]; //흔들리는 타일 밟았을 때 낙하하게 하는 변수

    bool isCollison[Map_length][Map_length];
    int isDown[Map_length][Map_length];
    float tile_height[4] = { 0.0 , 0.5, 1.0, 1.5 };
    bool isRotateTile = false;
};
Map_Setting MS;
Map_Setting MS_temp;

struct Road_Type {
    int none = 0;
    int road1 = 1;
    int road2 = 2;
    int road3 = 3;
    int shaking_road1 = 4;
    int shaking_road2 = 5;
    int shaking_road3 = 6;
    int hurdle1 = 7;
    int hurdle2 = 8;
    int hurdle3 = 9;
    int check_point = 10;
    int arrive_point = 11;
};
Road_Type road_type;
Road_Type road_type_temp;

struct Ball_State {
    int falling = 0;
    int bouncing = 1;
    int jumping = 2;

    bool collided = false;
};
Ball_State B_state;

struct Bounce_Timer {
    float animation = 0;
    float shadow_animation = 0;
    float jumping = 0;
    float falling = 0;
};
Bounce_Timer B_timer;
Bounce_Timer B_timer_temp;


struct Ball_Option {
    // 공의 크기
    float ball_radius = 0.1;

    // 튀기는 속도
    float bounce_speed = 0.0125;

    // 튀기는 높이
    float bounce_height = 3;
    //점프최고 높이
    float bounce_height_Highest = 5;
    //점프 최소 높이
    float bounce_height_Lowest = 3;


    // 공 찌그러짐 정도
    float bounce_scale_dinamic = 250;

    //중력 세기
    float gravity_power = 3;

};
Ball_Option BO;

struct Ball_Control {

    // 공의 가속도
    float acceleration_fb_x = Acceleration;
    float acceleration_fb_z = Acceleration;
    float acceleration_rl_x = Acceleration;
    float acceleration_rl_z = Acceleration;

    // 최고 속도
    float ball_move_speed_max = 0.03;

    //미끄러짐 정도
    float ball_move_slippiness = 2.0;

    //공 초기 속도
    float ball_move_speed_fb_x = 0.0;
    float ball_move_speed_fb_z = 0.0;
    float ball_move_speed_rl_x = 0.0;
    float ball_move_speed_rl_z = 0.0;

    //공 점프력
    float ball_jump_power = 0.01;

    float speed_reject = 1;

    bool acceleration_fb_check = false;
    bool acceleration_rl_check = false;

    bool keyW = false;
    bool keyS = false;
    bool keyA = false;
    bool keyD = false;
    bool Space = false;

    float ball_move_x = 0;
    float ball_move_y = 0;
    float ball_move_z = 0;
    float ball_collison_pos_y = 0;

    //그림자 높이 정도
    float shadow_trans_y = 0.0f;

};
Ball_Control BC;
Ball_Control BC_temp;

struct Camera_Option {
    float camera_distance = 3.0;

    float mouse_sensitivity = 200;

    float camera_drgree_x = 90;
    float camera_drgree_y = -30;

    float camera_delta_y = 0;

    float camera_fov = 60.0;

    float camera_moving_timer = 0;
};
Camera_Option CO;
Camera_Option CO_temp;

struct Mouse_Handling {
    bool left_button;
    bool first_mouse_pos_flag = true;

    float first_mouse_pos_x;
    float first_mouse_pos_y;

    float delta_mouse_x;
    float delta_mouse_y;

    float mouse_x;
    float mouse_y;
};
Mouse_Handling MH;

struct Light_Option {
    float checkpoint_light_timer = 0;
    float Arrive_light_timer = 0;

    float r_light = 1.3;
    float g_light = 1.0;
    float b_light = 0.8;

    float base_color[3] = { 1.3, 1.0, 0.8 };
    float checkpoint_color[3] = { 0.5, 1.0, 0.5 };
    //float checkpoint_color[3] = { 0.5, 1.0, 0.5 };

    bool onoff_checkpoint_light;
    bool onoff_Arrive_light;
    bool onoff_Arrive_light_finish;
};
Light_Option LO;

int ball_state = 0;

float ball_collison_pos_y;

float respone_x, respone_y, respone_z;

float background_color_r, background_color_g, background_color_b;

float test1 = 60;
float test2, test3;

bool onoff_timer = true;
bool onoff_culling = false;
bool perspective = true;
bool isSolid = false;
bool isrespone = false;

//====================================================================

void collision_y(float collision_pos_y);

void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void MouseChange(int x, int y);
void Ball_Handling();
void Ball_Bouncing_timers();


float RandomColor(int color);
float Ball_Bounce_PosY();
float Ball_Bounce_Ani(char mode, float base_size);
void Make_Hurdle(float hurdle_x, float hurdle_y, float hurdle_z, float rotate);
void Make_Map();
void Map_Collision_check();
void Make_arrivalpoint(float arrivalpoint_x, float arrivalpoint_y, float arrivalpoint_z);
void Reset_All();
void Copy_All();
void Make_shadow();

void Change_Light_Colors();

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid KeyBoard(unsigned char Key, int x, int y);
GLvoid KeyBoardUP(unsigned char Key, int x, int y);
GLvoid Timer(int value);



using namespace std;

GLvoid make_vertexShaders();
GLvoid make_fragmentShader();
GLvoid InitShader();
GLvoid InitBuffer();
GLchar* filetobuf(const GLchar* file);

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
    -cubesize,cubesize,cubesize,0.0, 0.0, 1.0,
    cubesize,-cubesize,cubesize,0.0, 0.0, 1.0,
    -cubesize,-cubesize,cubesize,0.0, 0.0, 1.0,

    cubesize,-cubesize,cubesize,0.0, 0.0, 1.0,
    -cubesize,cubesize,cubesize,0.0, 0.0, 1.0,
    cubesize,cubesize,cubesize,0.0, 0.0, 1.0,

    //윗면
    -cubesize,cubesize,-cubesize,0.0, 1.0, 0.0,
    cubesize,cubesize,cubesize,0.0, 1.0, 0.0,
    -cubesize,cubesize,cubesize,0.0, 1.0, 0.0,

    cubesize,cubesize,-cubesize,0.0, 1.0, 0.0,
    cubesize,cubesize,cubesize,0.0, 1.0, 0.0,
    -cubesize,cubesize,-cubesize,0.0, 1.0, 0.0,

    //아랫면
    cubesize,-cubesize,cubesize,0.0, -1.0, 0.0,
    -cubesize,-cubesize,-cubesize,0.0, -1.0, 0.0,
    -cubesize,-cubesize,cubesize,0.0, -1.0, 0.0,

    -cubesize,-cubesize,-cubesize,0.0, -1.0, 0.0,
    cubesize,-cubesize,cubesize,0.0, -1.0, 0.0,
    cubesize,-cubesize,-cubesize,0.0, -1.0, 0.0,

    //뒷면
    cubesize,-cubesize,-cubesize,0.0, 0.0, -1.0,
    -cubesize,cubesize,-cubesize,0.0, 0.0, -1.0,
    -cubesize,-cubesize,-cubesize,0.0, 0.0, -1.0,

    -cubesize,cubesize,-cubesize,0.0, 0.0, -1.0,
    cubesize,-cubesize,-cubesize,0.0, 0.0, -1.0,
    cubesize,cubesize,-cubesize,0.0, 0.0, -1.0,

    //오른면
    cubesize,cubesize,cubesize,1.0, 0.0, 0.0,
    cubesize,-cubesize,-cubesize,1.0, 0.0, 0.0,
    cubesize,-cubesize,cubesize,1.0, 0.0, 0.0,

    cubesize,cubesize,cubesize,1.0, 0.0, 0.0,
    cubesize,cubesize,-cubesize,1.0, 0.0, 0.0,
    cubesize,-cubesize,-cubesize,1.0, 0.0, 0.0,

    //외면
    -cubesize,-cubesize,-cubesize,-1.0, 0.0, 0.0,
     -cubesize,cubesize,cubesize,-1.0, 0.0, 0.0,
    -cubesize,-cubesize,cubesize,-1.0, 0.0, 0.0,

    -cubesize,cubesize,-cubesize,-1.0, 0.0, 0.0,
    -cubesize,cubesize,cubesize,-1.0, 0.0, 0.0,
    -cubesize,-cubesize,-cubesize,-1.0, 0.0, 0.0,

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
    //else
    //    cout << "INIT<<endl";

    srand((unsigned)time(NULL));
    Copy_All();
    glEnable(GL_DEPTH_TEST);



    InitShader();
    InitBuffer();
    glutTimerFunc(100, Timer, 1);
    glutKeyboardFunc(KeyBoard);
    glutKeyboardUpFunc(KeyBoardUP);
    glutDisplayFunc(drawScene);

    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);

    glutReshapeFunc(Reshape);
    glutMainLoop();
}


GLvoid drawScene()
{
    glClearColor(background_color_r, background_color_g, background_color_b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //깊이 체크 (컬링)


    //카메라세팅 =============================================================
    glm::vec3 cameraPos = glm::vec3(BC.ball_move_x + CO.camera_distance * cos((CO.camera_drgree_y) * pi / 180) * cos((CO.camera_drgree_x) * pi / 180),
        CO.camera_delta_y - CO.camera_distance * sin((CO.camera_drgree_y) * pi / 180),
        BC.ball_move_z + CO.camera_distance * cos((CO.camera_drgree_y) * pi / 180) * sin((CO.camera_drgree_x) * pi / 180)); //--- 카메라 위치
    glm::vec3 cameraDirection = glm::vec3(BC.ball_move_x, CO.camera_delta_y, BC.ball_move_z); //--- 카메라 바라보는 방향
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    unsigned int viewLocation = glGetUniformLocation(s_program, "view"); //--- 뷰잉 변환 설정
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


    //원근법 유무(Perspective = 원근투영)
    if (perspective == false) {
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f); //--- 투영 공간을 [-100.0, 100.0] 공간으로 설정
        unsigned int projectionLocation = glGetUniformLocation(s_program, "projection"); //--- 투영 변환 값 설정
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
    else {

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(CO.camera_fov), 1.0f, 0.1f, 100.0f);
        projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- 공간을 약간 뒤로 미뤄줌
        unsigned int projectionLocation = glGetUniformLocation(s_program, "projection"); //--- 투영 변환 값 설정
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


    // light =================================================================
    unsigned int lightPosLocation = glGetUniformLocation(s_program, "lightPos");
    glUniform3f(lightPosLocation, 0.0, 20.0, -40.0);
    unsigned int lightColorLocation = glGetUniformLocation(s_program, "lightColor");
    glUniform3f(lightColorLocation, LO.r_light, LO.g_light, LO.b_light);
    //========================================================================


    glUseProgram(s_program);
    int objColorLocation = glGetUniformLocation(s_program, "objectColor");

    glm::mat4 B_Matrix = glm::mat4(1.0f);
    unsigned int BalltransformLocation = glGetUniformLocation(s_program, "transform");
    //ball ===================================================================
    B_Matrix = glm::mat4(1.0f);
    B_Matrix = glm::translate(B_Matrix, glm::vec3(BC.ball_move_x, BC.ball_move_y, BC.ball_move_z));
    B_Matrix = glm::rotate(B_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    B_Matrix = glm::scale(B_Matrix, glm::vec3(Ball_Bounce_Ani('x', BO.ball_radius * 10), Ball_Bounce_Ani('y', BO.ball_radius * 10), Ball_Bounce_Ani('z', BO.ball_radius * 10)));
    B_Matrix = glm::translate(B_Matrix, glm::vec3(0.0, 0.1, 0.0)); // 스케일 피봇 설정
    BalltransformLocation = glGetUniformLocation(s_program, "transform");
    glUniformMatrix4fv(BalltransformLocation, 1, GL_FALSE, glm::value_ptr(B_Matrix));
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    objColorLocation = glGetUniformLocation(s_program, "objectColor");
    glUniform3f(objColorLocation, 1.0f, 0.0f, 0.0f);
    gluSphere(qobj, 0.1, 30, 30);

    BC.ball_move_y = Ball_Bounce_PosY();
    //========================================================================


    //bottom
    glBindVertexArray(VAO[0]);
    B_Matrix = glm::mat4(1.0f);
    B_Matrix = glm::translate(B_Matrix, glm::vec3(0.0, -0.1, 0.0));
    B_Matrix = glm::rotate(B_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    B_Matrix = glm::scale(B_Matrix, glm::vec3(1.0, 1.0, 1.0));
    BalltransformLocation = glGetUniformLocation(s_program, "transform");
    glUniformMatrix4fv(BalltransformLocation, 1, GL_FALSE, glm::value_ptr(B_Matrix));
    objColorLocation = glGetUniformLocation(s_program, "objectColor");
    glUniform3f(objColorLocation, 0.5f, 0.5f, 0.5f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //Map
    Make_Map();

    //Shadow
    if (BC.shadow_trans_y <= BC.ball_move_y) {
        Make_shadow();
    }

    glutSwapBuffers();
}
GLvoid KeyBoard(unsigned char Key, int x, int y)
{
    switch (Key) {

    case 'w':
        BC.acceleration_fb_check = true;
        BC.keyW = true;
        break;

    case 's':
        BC.acceleration_fb_check = true;
        BC.keyS = true;
        break;

    case 'a':
        BC.acceleration_rl_check = true;
        BC.keyA = true;
        break;

    case 'd':
        BC.acceleration_rl_check = true;
        BC.keyD = true;
        break;

    case ' ':
        BC.Space = true;
        break;

        //test구역=====================================
    case 'u':
        test1 += 1.0f;
        cout << "test1= " << test1 << endl;
        break;
    case 'j':
        test1 -= 1.0f;
        cout << "test1= " << test1 << endl;
        break;

    case 'i':
        test2 += 0.1f;
        BO.bounce_height += 0.1;
        cout << "test2= " << test2 << endl;
        break;
    case 'k':
        test2 -= 0.1f;
        BO.bounce_height -= 0.1;
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
    switch (Key)
    {
    case 'w':
        BC.acceleration_fb_check = false;
        BC.keyW = false;
        break;

    case 's':
        BC.acceleration_fb_check = false;
        BC.keyS = false;
        break;

    case 'a':
        BC.acceleration_rl_check = false;
        BC.keyA = false;
        break;

    case 'd':
        BC.acceleration_rl_check = false;
        BC.keyD = false;
        break;

    case ' ':
        BC.Space = false;
        break;
    }

}
GLvoid Timer(int value)
{
    if (onoff_timer == true) {

        MH.first_mouse_pos_flag = true;

        //bounce animation
        Ball_Bouncing_timers();

        //move animation
        Ball_Handling();

        if (ball_collison_pos_y - 0.02 > CO.camera_delta_y) {
            CO.camera_delta_y += 0.01;
        }
        else if (ball_collison_pos_y + 0.02 < CO.camera_delta_y) {
            CO.camera_delta_y -= 0.01;
        }
        else {
            CO.camera_delta_y = ball_collison_pos_y;
        }

        CO.camera_fov += 1;
        if (CO.camera_fov > 60) {
            CO.camera_fov = 60;
        }

    }
    Change_Light_Colors();


    //cout << B_timer.shadow_animation << endl;

    //충돌로 인한 타일 애니메이션
    for (int i = 0; i < Map_length; ++i) {
        for (int j = 0; j < Map_length; ++j) {
            if (MS.isCollison[i][j] == 1) {
                if (MS.delta_trans_y[i][j] >= -2.0) {
                    MS.delta_trans_y[i][j] -= 0.02f;
                }
                else {
                    MS.isDown[i][j] = 1;
                    MS.road[i][j] = road_type.none;
                }

                if (MS.isRotateTile == false) {
                    if (MS.shaking_tile[i][j] <= 10.0f) {
                        MS.shaking_tile[i][j] += 1.0f;
                    }
                    else {
                        MS.isRotateTile = true;
                    }
                }
                else {
                    if (MS.shaking_tile[i][j] >= -10.0f) {
                        MS.shaking_tile[i][j] -= 1.0f;
                    }
                    else {
                        MS.isRotateTile = false;
                    }
                }
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(10, Timer, 1);
}


void collision_y(float collision_pos_y) {
    if (BC.ball_move_y < collision_pos_y) {

        ball_collison_pos_y = BC.ball_move_y;
        ball_state = B_state.bouncing;
        B_timer.falling = 0;
        CO.camera_moving_timer = 0;

        B_state.collided = true;
    }
    B_state.collided = false;
}
float Ball_Bounce_PosY() {
    // Ball의 기존 위치에서부터 시작하여 bounce하는 Ball의 위치를 계산하여 return 해주는 함수

    float result_y = 0;
    float ball_highest_y = BO.bounce_height * 0.25;


    if (ball_state == B_state.falling) {
        result_y = -BO.gravity_power * B_timer.falling * B_timer.falling + ball_highest_y + ball_collison_pos_y;
    }
    else if (ball_state == B_state.bouncing) {
        result_y = ball_collison_pos_y;
    }
    else if (ball_state == B_state.jumping) {
        result_y = -BO.bounce_height * B_timer.jumping * (B_timer.jumping - 1) + ball_collison_pos_y;
    }

    return result_y;
}
float Ball_Bounce_Ani(char mode, float base_size) {
    //공이 바닥에 닿을 때, 스케일 값 변환 해 주는 함수

    switch (mode) {

    case('x'):
        base_size = -BO.bounce_scale_dinamic * B_timer.animation * (B_timer.animation - 0.1) + base_size;
        return base_size;
        break;

    case('z'):
        base_size = -BO.bounce_scale_dinamic * B_timer.animation * (B_timer.animation - 0.1) + base_size;
        return base_size;
        break;

    case('y'):
        base_size = BO.bounce_scale_dinamic * B_timer.animation * (B_timer.animation - 0.1) + base_size;
        return base_size;
        break;


    case('s'):
        if (ball_state == B_state.bouncing) {
            base_size = -BO.bounce_scale_dinamic * B_timer.animation * (B_timer.animation - 0.1) + base_size;
        }
        else {
            base_size = 2 * B_timer.shadow_animation * (B_timer.shadow_animation - 1) + 1;
        }
        return base_size;
        break;


    }

}
void Ball_Bouncing_timers() {
    //ball의 상태변화에 따른 animation시간변화

    if (ball_state == B_state.falling) {
        B_timer.falling += BO.bounce_speed;
        B_timer.shadow_animation += BO.bounce_speed;
    }

    if (ball_state == B_state.bouncing) {
        B_timer.animation += BO.bounce_speed;
        B_timer.shadow_animation = 0;
        if (B_timer.animation > 0.1) {
            ball_state = B_state.jumping;
            B_timer.animation = 0;
        }
    }

    if (ball_state == B_state.jumping) {
        B_timer.jumping += BO.bounce_speed;
        B_timer.shadow_animation += BO.bounce_speed;
        if (B_timer.jumping > 0.5) {
            B_timer.jumping = 0;
            ball_state = B_state.falling;
        }
    }
}

void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        MH.left_button = true;
    }

}
void Motion(int x, int y)
{
    MouseChange(x, y);


    if (MH.left_button == true)
    {
        if (MH.first_mouse_pos_flag) {
            MH.first_mouse_pos_x = MH.mouse_x;
            MH.first_mouse_pos_y = MH.mouse_y;

            MH.first_mouse_pos_flag = false;
        }

        MH.delta_mouse_x = MH.mouse_x - MH.first_mouse_pos_x;
        MH.delta_mouse_y = MH.mouse_y - MH.first_mouse_pos_y;

        CO.camera_drgree_x += MH.delta_mouse_x * CO.mouse_sensitivity;
        CO.camera_drgree_y += MH.delta_mouse_y * CO.mouse_sensitivity;
        if (CO.camera_drgree_y > 89.0) {
            CO.camera_drgree_y = 89.0;
        }
        if (CO.camera_drgree_y < -89.0) {
            CO.camera_drgree_y = -89.0;
        }

    }
}
void MouseChange(int x, int y) {

    MH.mouse_x = 2 / (float)window_width * (float)x - 1;
    MH.mouse_y = -2 / (float)window_high * (float)y + 1;

} //마우스 좌표계 변환 함수
void Ball_Handling() {
    // Ball handling : 카메라가 바라보고 있는 각도 기준에서 앞뒤,좌우 이동
    if (BC.keyW) {
        BC.ball_move_speed_fb_x -= BC.acceleration_fb_x;
        BC.ball_move_speed_fb_z -= BC.acceleration_fb_z;
    }
    if (BC.keyS) {
        BC.ball_move_speed_fb_x += BC.acceleration_fb_x;
        BC.ball_move_speed_fb_z += BC.acceleration_fb_z;
    }
    if (BC.keyA) {
        BC.ball_move_speed_rl_x -= BC.acceleration_rl_x;
        BC.ball_move_speed_rl_z -= BC.acceleration_rl_z;
    }
    if (BC.keyD) {
        BC.ball_move_speed_rl_x += BC.acceleration_rl_x;
        BC.ball_move_speed_rl_z += BC.acceleration_rl_z;
    }



    // Ball slow down : 엑셀을 밟지 않았을 때 속도는 (가속도 / 미끄러짐) 만큼 감속
    if (BC.acceleration_fb_check == false) {
        if (BC.ball_move_speed_fb_x < 0) {
            BC.ball_move_speed_fb_x += BC.acceleration_fb_x / BC.ball_move_slippiness;
        }
        else if (BC.ball_move_speed_fb_x > 0) {
            BC.ball_move_speed_fb_x -= BC.acceleration_fb_x / BC.ball_move_slippiness;
        }
        if (BC.ball_move_speed_fb_z < 0) {
            BC.ball_move_speed_fb_z += BC.acceleration_fb_z / BC.ball_move_slippiness;
        }
        else if (BC.ball_move_speed_fb_z > 0) {
            BC.ball_move_speed_fb_z -= BC.acceleration_fb_z / BC.ball_move_slippiness;
        }
    }

    if (BC.acceleration_rl_check == false) {
        if (BC.ball_move_speed_rl_x < 0) {
            BC.ball_move_speed_rl_x += BC.acceleration_rl_x / BC.ball_move_slippiness;
        }
        else if (BC.ball_move_speed_rl_x > 0) {
            BC.ball_move_speed_rl_x -= BC.acceleration_rl_x / BC.ball_move_slippiness;
        }
        if (BC.ball_move_speed_rl_z < 0) {
            BC.ball_move_speed_rl_z += BC.acceleration_rl_z / BC.ball_move_slippiness;
        }
        else if (BC.ball_move_speed_rl_z > 0) {
            BC.ball_move_speed_rl_z -= BC.acceleration_rl_z / BC.ball_move_slippiness;
        }
    }


    // ball maximum speed fixing : 어느 속도에 도달 할 시, 가속을 더하지 않음
    if (BC.ball_move_speed_fb_x > BC.ball_move_speed_max) {
        BC.ball_move_speed_fb_x = BC.ball_move_speed_max;
    }
    else if (BC.ball_move_speed_fb_x < -BC.ball_move_speed_max) {
        BC.ball_move_speed_fb_x = -BC.ball_move_speed_max;
    }
    if (BC.ball_move_speed_fb_z > BC.ball_move_speed_max) {
        BC.ball_move_speed_fb_z = BC.ball_move_speed_max;
    }
    else if (BC.ball_move_speed_fb_z < -BC.ball_move_speed_max) {
        BC.ball_move_speed_fb_z = -BC.ball_move_speed_max;
    }

    if (BC.ball_move_speed_rl_x > BC.ball_move_speed_max) {
        BC.ball_move_speed_rl_x = BC.ball_move_speed_max;
    }
    else if (BC.ball_move_speed_rl_x < -BC.ball_move_speed_max) {
        BC.ball_move_speed_rl_x = -BC.ball_move_speed_max;
    }
    if (BC.ball_move_speed_rl_z > BC.ball_move_speed_max) {
        BC.ball_move_speed_rl_z = BC.ball_move_speed_max;
    }
    else if (BC.ball_move_speed_rl_z < -BC.ball_move_speed_max) {
        BC.ball_move_speed_rl_z = -BC.ball_move_speed_max;
    }

    if (BC.Space) {
        BO.bounce_height += BC.ball_jump_power;
        if (BO.bounce_height > BO.bounce_height_Highest) {
            BO.bounce_height = BO.bounce_height_Highest;
        }
    }
    else {
        BO.bounce_height -= BC.ball_jump_power;
        if (BO.bounce_height < BO.bounce_height_Lowest) {
            BO.bounce_height = BO.bounce_height_Lowest;
        }
    }

    //허들과 충돌 시 튕겨나와 완화되는 속도반전
    if (BC.speed_reject > 1) {
        BC.speed_reject = 1;
    }
    else {
        BC.speed_reject += 0.005;
    }

    // ball moving
    BC.ball_move_x += BC.speed_reject * (BC.ball_move_speed_fb_x * cos(CO.camera_drgree_x * pi / 180) + BC.ball_move_speed_rl_x * cos((CO.camera_drgree_x - 90) * pi / 180));
    BC.ball_move_z += BC.speed_reject * (BC.ball_move_speed_fb_z * sin(CO.camera_drgree_x * pi / 180) + BC.ball_move_speed_rl_z * sin((CO.camera_drgree_x - 90) * pi / 180));
}


void Make_Map()
{
    for (int i = 0; i < Map_length; ++i) {
        for (int j = 0; j < Map_length; ++j) {
            glm::mat4 M_Matrix = glm::mat4(1.0f);

            if (MS.road[i][j] == road_type.road1) {
                M_Matrix = glm::translate(M_Matrix, glm::vec3(MS.tile_size * (2 * j - Map_length + 1), MS.tile_height[0], 2 * MS.tile_size * (i - Map_length + 3)));
                M_Matrix = glm::scale(M_Matrix, glm::vec3(MS.tile_size * 10, 0.2, MS.tile_size * 10));
                M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, -0.1, 0.0));
                int objColorLocation = glGetUniformLocation(s_program, "objectColor");
                glUniform3f(objColorLocation, 0.5f, 0.5f, 0.5f);
                unsigned int FloortransformLocation = glGetUniformLocation(s_program, "transform");
                glUniformMatrix4fv(FloortransformLocation, 1, GL_FALSE, glm::value_ptr(M_Matrix));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            else if (MS.road[i][j] == road_type.road2) {
                M_Matrix = glm::translate(M_Matrix, glm::vec3(MS.tile_size * (2 * j - Map_length + 1), MS.tile_height[1], 2 * MS.tile_size * (i - Map_length + 3)));
                M_Matrix = glm::scale(M_Matrix, glm::vec3(MS.tile_size * 10, 0.2, MS.tile_size * 10));
                M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, -0.1, 0.0));
                int objColorLocation = glGetUniformLocation(s_program, "objectColor");
                glUniform3f(objColorLocation, 0.5f, 0.5f, 0.5f);
                unsigned int FloortransformLocation = glGetUniformLocation(s_program, "transform");
                glUniformMatrix4fv(FloortransformLocation, 1, GL_FALSE, glm::value_ptr(M_Matrix));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            else if (MS.road[i][j] == road_type.road3) {
                M_Matrix = glm::translate(M_Matrix, glm::vec3(MS.tile_size * (2 * j - Map_length + 1), MS.tile_height[2], 2 * MS.tile_size * (i - Map_length + 3)));
                M_Matrix = glm::scale(M_Matrix, glm::vec3(MS.tile_size * 10, 0.2, MS.tile_size * 10));
                M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, -0.1, 0.0));
                int objColorLocation = glGetUniformLocation(s_program, "objectColor");
                glUniform3f(objColorLocation, 0.5f, 0.5f, 0.5f);
                unsigned int FloortransformLocation = glGetUniformLocation(s_program, "transform");
                glUniformMatrix4fv(FloortransformLocation, 1, GL_FALSE, glm::value_ptr(M_Matrix));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            else if (MS.road[i][j] == road_type.shaking_road1) {
                if (MS.isDown[i][j] == false) {
                    M_Matrix = glm::translate(M_Matrix, glm::vec3(MS.tile_size * (2 * j - Map_length + 1), MS.tile_height[0], 2 * MS.tile_size * (i - Map_length + 3)));
                    M_Matrix = glm::scale(M_Matrix, glm::vec3(MS.tile_size * 10, 0.2, MS.tile_size * 10));
                    M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, -0.1, 0.0));  //y축으로 이동 
                    if (MS.isCollison[i][j] == true) {
                        M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, MS.delta_trans_y[i][j], 0.0));  //밟으면 아래로 낙하
                        M_Matrix = glm::rotate(M_Matrix, glm::radians(MS.shaking_tile[i][j]), glm::vec3(0.0f, 1.0f, 0.0f));

                    }
                    int objColorLocation = glGetUniformLocation(s_program, "objectColor");
                    glUniform3f(objColorLocation, 0.5f, 0.5f, 0.5f);
                    unsigned int FloortransformLocation = glGetUniformLocation(s_program, "transform");
                    glUniformMatrix4fv(FloortransformLocation, 1, GL_FALSE, glm::value_ptr(M_Matrix));
                }
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            else if (MS.road[i][j] == road_type.shaking_road2) {
                if (MS.isDown[i][j] == false) {
                    M_Matrix = glm::translate(M_Matrix, glm::vec3(MS.tile_size * (2 * j - Map_length + 1), MS.tile_height[1], 2 * MS.tile_size * (i - Map_length + 3)));
                    M_Matrix = glm::scale(M_Matrix, glm::vec3(MS.tile_size * 10, 0.2, MS.tile_size * 10));
                    M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, -0.1, 0.0));  //y축으로 이동 
                    if (MS.isCollison[i][j] == true) {
                        M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, MS.delta_trans_y[i][j], 0.0));  //밟으면 아래로 낙하
                        M_Matrix = glm::rotate(M_Matrix, glm::radians(MS.shaking_tile[i][j]), glm::vec3(0.0f, 1.0f, 0.0f));

                    }
                    int objColorLocation = glGetUniformLocation(s_program, "objectColor");
                    glUniform3f(objColorLocation, 0.5f, 0.5f, 0.5f);
                    unsigned int FloortransformLocation = glGetUniformLocation(s_program, "transform");
                    glUniformMatrix4fv(FloortransformLocation, 1, GL_FALSE, glm::value_ptr(M_Matrix));
                }
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            else if (MS.road[i][j] == road_type.shaking_road3) {
                if (MS.isDown[i][j] == false) {
                    M_Matrix = glm::translate(M_Matrix, glm::vec3(MS.tile_size * (2 * j - Map_length + 1), MS.tile_height[2], 2 * MS.tile_size * (i - Map_length + 3)));
                    M_Matrix = glm::scale(M_Matrix, glm::vec3(MS.tile_size * 10, 0.2, MS.tile_size * 10));
                    M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, -0.1, 0.0));  //y축으로 이동 
                    if (MS.isCollison[i][j] == true) {
                        M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, MS.delta_trans_y[i][j], 0.0));  //밟으면 아래로 낙하
                        M_Matrix = glm::rotate(M_Matrix, glm::radians(MS.shaking_tile[i][j]), glm::vec3(0.0f, 1.0f, 0.0f));

                    }
                    int objColorLocation = glGetUniformLocation(s_program, "objectColor");
                    glUniform3f(objColorLocation, 0.5f, 0.5f, 0.5f);
                    unsigned int FloortransformLocation = glGetUniformLocation(s_program, "transform");
                    glUniformMatrix4fv(FloortransformLocation, 1, GL_FALSE, glm::value_ptr(M_Matrix));
                }
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            else if (MS.road[i][j] == road_type.hurdle1) {
                Make_Hurdle(MS.tile_size * (2 * j - Map_length + 1), MS.tile_height[0], 2 * MS.tile_size * (i - Map_length + 3), 0);
            }

            else if (MS.road[i][j] == road_type.hurdle2) {
                Make_Hurdle(MS.tile_size * (2 * j - Map_length + 1), MS.tile_height[1], 2 * MS.tile_size * (i - Map_length + 3), 0);
            }

            else if (MS.road[i][j] == road_type.hurdle3) {
                Make_Hurdle(MS.tile_size * (2 * j - Map_length + 1), MS.tile_height[2], 2 * MS.tile_size * (i - Map_length + 3), 0);
            }

            if (MS.road[i][j] == road_type.check_point) {
                M_Matrix = glm::translate(M_Matrix, glm::vec3(MS.tile_size * (2 * j - Map_length + 1), MS.tile_height[0], 2 * MS.tile_size * (i - Map_length + 3)));
                M_Matrix = glm::scale(M_Matrix, glm::vec3(MS.tile_size * 10, 0.2, MS.tile_size * 10));
                M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, -0.1, 0.0));
                int objColorLocation = glGetUniformLocation(s_program, "objectColor");
                glUniform3f(objColorLocation, 0.0f, 1.0f, 0.0f);
                unsigned int FloortransformLocation = glGetUniformLocation(s_program, "transform");
                glUniformMatrix4fv(FloortransformLocation, 1, GL_FALSE, glm::value_ptr(M_Matrix));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            if (MS.road[i][j] == road_type.arrive_point) {
                M_Matrix = glm::translate(M_Matrix, glm::vec3(MS.tile_size * (2 * j - Map_length + 1), MS.tile_height[0], 2 * MS.tile_size * (i - Map_length + 3)));
                M_Matrix = glm::scale(M_Matrix, glm::vec3(MS.tile_size * 10, 0.2, MS.tile_size * 10));
                M_Matrix = glm::translate(M_Matrix, glm::vec3(0.0, -0.1, 0.0));
                int objColorLocation = glGetUniformLocation(s_program, "objectColor");
                glUniform3f(objColorLocation, 0.0f, 0.0f, 2.0f);
                unsigned int FloortransformLocation = glGetUniformLocation(s_program, "transform");
                glUniformMatrix4fv(FloortransformLocation, 1, GL_FALSE, glm::value_ptr(M_Matrix));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }
    Map_Collision_check();
}
void Map_Collision_check() {
    for (int i = 0; i < Map_length; ++i) {
        for (int j = 0; j < Map_length; ++j) {

            if (MS.road[i][j] == road_type.road1) {
                if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                    MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                    2 * MS.tile_size * (i - Map_length + 3) - MS.tile_size < BC.ball_move_z &&
                    2 * MS.tile_size * (i - Map_length + 3) + MS.tile_size > BC.ball_move_z
                    )
                {
                    if (BC.ball_move_y > MS.tile_height[0] - 0.05) {
                        collision_y(MS.tile_height[0]);
                        BC.shadow_trans_y = MS.tile_height[0];
                    }
                }
            }

            if (MS.road[i][j] == road_type.road2) {
                if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                    MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                    2 * MS.tile_size * (i - Map_length + 3) - MS.tile_size < BC.ball_move_z &&
                    2 * MS.tile_size * (i - Map_length + 3) + MS.tile_size > BC.ball_move_z
                    )
                {
                    if (BC.ball_move_y > MS.tile_height[1] - 0.05) {
                        BC.shadow_trans_y = MS.tile_height[1];
                        collision_y(MS.tile_height[1]);
                    }
                }
            }

            if (MS.road[i][j] == road_type.road3) {
                if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                    MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                    2 * MS.tile_size * (i - Map_length + 3) - MS.tile_size < BC.ball_move_z &&
                    2 * MS.tile_size * (i - Map_length + 3) + MS.tile_size > BC.ball_move_z
                    )
                {
                    if (BC.ball_move_y > MS.tile_height[2] - 0.05) {
                        collision_y(MS.tile_height[2]);
                        BC.shadow_trans_y = MS.tile_height[2];
                    }
                }
            }

            if (MS.road[i][j] == road_type.shaking_road1) {
                if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                    MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                    2 * MS.tile_size * (i - Map_length + 3) - MS.tile_size < BC.ball_move_z &&
                    2 * MS.tile_size * (i - Map_length + 3) + MS.tile_size > BC.ball_move_z
                    )
                {
                    if (MS.tile_height[0] - 0.05 <= BC.ball_move_y && BC.ball_move_y <= MS.tile_height[0] + 0.05) {
                        MS.isCollison[i][j] = 1;
                        collision_y(MS.tile_height[0]);
                        BC.shadow_trans_y = MS.tile_height[0];
                    }

                }
            }

            if (MS.road[i][j] == road_type.shaking_road2) {
                if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                    MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                    2 * MS.tile_size * (i - Map_length + 3) - MS.tile_size < BC.ball_move_z &&
                    2 * MS.tile_size * (i - Map_length + 3) + MS.tile_size > BC.ball_move_z
                    )
                {
                    if (MS.tile_height[1] - 0.05 <= BC.ball_move_y && BC.ball_move_y <= MS.tile_height[1] + 0.05) {
                        MS.isCollison[i][j] = 1;
                        collision_y(MS.tile_height[1]);
                        BC.shadow_trans_y = MS.tile_height[1];
                    }

                }
            }

            if (MS.road[i][j] == road_type.shaking_road3) {
                if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                    MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                    2 * MS.tile_size * (i - Map_length + 3) - MS.tile_size < BC.ball_move_z &&
                    2 * MS.tile_size * (i - Map_length + 3) + MS.tile_size > BC.ball_move_z
                    )
                {
                    if (MS.tile_height[2] - 0.05 <= BC.ball_move_y && BC.ball_move_y <= MS.tile_height[2] + 0.05) {
                        MS.isCollison[i][j] = 1;
                        collision_y(MS.tile_height[2]);
                        BC.shadow_trans_y = MS.tile_height[2];
                    }
                }
            }


            if (MS.road[i][j] == road_type.hurdle1) {
                if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                    MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                    2 * MS.tile_size * (i - Map_length + 3) - MS.tile_size < BC.ball_move_z &&
                    2 * MS.tile_size * (i - Map_length + 3) + MS.tile_size > BC.ball_move_z
                    )
                {
                    if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                        MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                        2 * MS.tile_size * (i - Map_length + 3) - BO.ball_radius < BC.ball_move_z &&
                        2 * MS.tile_size * (i - Map_length + 3) + BO.ball_radius > BC.ball_move_z)
                    {
                        if (BC.ball_move_y < MS.tile_height[0] + 0.7) {
                            BC.speed_reject = -0.5;
                            CO.camera_fov = 55;
                        }
                    }
                    if (BC.ball_move_y > MS.tile_height[0] - 0.05) {
                        collision_y(MS.tile_height[0]);
                        BC.shadow_trans_y = MS.tile_height[0];
                    }
                }
            }

            if (MS.road[i][j] == road_type.hurdle2) {
                if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                    MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                    2 * MS.tile_size * (i - Map_length + 3) - MS.tile_size < BC.ball_move_z &&
                    2 * MS.tile_size * (i - Map_length + 3) + MS.tile_size > BC.ball_move_z
                    )
                {
                    if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                        MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                        2 * MS.tile_size * (i - Map_length + 3) - BO.ball_radius < BC.ball_move_z &&
                        2 * MS.tile_size * (i - Map_length + 3) + BO.ball_radius > BC.ball_move_z)
                    {
                        if (BC.ball_move_y < MS.tile_height[1] + 0.7) {
                            BC.speed_reject = -0.5;
                            CO.camera_fov = 55;
                        }
                    }
                    if (BC.ball_move_y > MS.tile_height[1] - 0.05) {
                        collision_y(MS.tile_height[1]);
                        BC.shadow_trans_y = MS.tile_height[1];
                    }

                }
            }

            if (MS.road[i][j] == road_type.hurdle3) {
                if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                    MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                    2 * MS.tile_size * (i - Map_length + 3) - MS.tile_size < BC.ball_move_z &&
                    2 * MS.tile_size * (i - Map_length + 3) + MS.tile_size > BC.ball_move_z
                    )
                {
                    if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                        MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                        2 * MS.tile_size * (i - Map_length + 3) - BO.ball_radius < BC.ball_move_z &&
                        2 * MS.tile_size * (i - Map_length + 3) + BO.ball_radius > BC.ball_move_z)
                    {
                        if (BC.ball_move_y < MS.tile_height[2] + 0.7) {
                            BC.speed_reject = -0.5;
                            CO.camera_fov = 55;
                        }
                    }
                    if (BC.ball_move_y > MS.tile_height[2] - 0.05) {
                        collision_y(MS.tile_height[2]);
                        BC.shadow_trans_y = MS.tile_height[2];
                    }

                }
            }




            //-------------------------------------------------------------------------------------------------------
            // 도착 지점
            if (MS.road[i][j] == road_type.arrive_point) {
                if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                    MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                    2 * MS.tile_size * (i - Map_length + 3) - MS.tile_size < BC.ball_move_z &&
                    2 * MS.tile_size * (i - Map_length + 3) + MS.tile_size > BC.ball_move_z
                    )
                {
                    if (BC.ball_move_y > MS.tile_height[0] - 0.05) {
                        collision_y(MS.tile_height[0]);
                        LO.onoff_Arrive_light = true;
                    }
                }
            }

            //-------------------------------------------------------------------------------------------------------
            // 리스폰 지역 
            if (MS.road[i][j] == road_type.check_point) {
                if (MS.tile_size * (2 * j - Map_length + 1) - MS.tile_size < BC.ball_move_x &&
                    MS.tile_size * (2 * j - Map_length + 1) + MS.tile_size > BC.ball_move_x &&
                    2 * MS.tile_size * (i - Map_length + 3) - MS.tile_size < BC.ball_move_z &&
                    2 * MS.tile_size * (i - Map_length + 3) + MS.tile_size > BC.ball_move_z
                    )
                {
                    if (BC.ball_move_y > MS.tile_height[0] - 0.05) {
                        collision_y(MS.tile_height[0]);
                        respone_x = MS.tile_size * (2 * j - Map_length + 1);
                        respone_y = BC.ball_move_y;
                        respone_z = 2 * MS.tile_size * (i - Map_length + 3);

                        LO.onoff_checkpoint_light = true;

                        isrespone = true;
                    }
                }
            }

            //-------------------------------------------------------------------------------------------------------
        }
    }



    //죽었을때 초기화
    if (BC.ball_move_y < -5) {
        Reset_All();
        if (isrespone == true) {
            BC.ball_move_x = respone_x;
            BC.ball_move_y = respone_y;
            BC.ball_move_z = respone_z;
        }
    }
}
void Make_Hurdle(float hurdle_x, float hurdle_y, float hurdle_z, float rotate)
{

    glUseProgram(s_program);
    int objColorLocation = glGetUniformLocation(s_program, "objectColor");
    glm::mat4 H_Matrix = glm::mat4(1.0f);
    unsigned int HurdletransformLocation = glGetUniformLocation(s_program, "transform");

    //hurdle_base
    glBindVertexArray(VAO[0]);
    H_Matrix = glm::mat4(1.0f);
    H_Matrix = glm::rotate(H_Matrix, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(hurdle_x, hurdle_y, hurdle_z));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(0.0, 0.0, 0.0));
    H_Matrix = glm::rotate(H_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    H_Matrix = glm::scale(H_Matrix, glm::vec3(4.0, 3.5, 0.1));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(0.0, 0.1, 0.0)); // 스케일 피봇 설정
    HurdletransformLocation = glGetUniformLocation(s_program, "transform");
    glUniformMatrix4fv(HurdletransformLocation, 1, GL_FALSE, glm::value_ptr(H_Matrix));
    //glUniform1f(isCheck, true);
    objColorLocation = glGetUniformLocation(s_program, "objectColor");
    glUniform3f(objColorLocation, 1.0f, 1.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //hurdle_r_stick
    glBindVertexArray(VAO[0]);
    H_Matrix = glm::mat4(1.0f);
    H_Matrix = glm::rotate(H_Matrix, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(hurdle_x, hurdle_y, hurdle_z));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(0.4, 0.0, 0.0));
    H_Matrix = glm::rotate(H_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    H_Matrix = glm::scale(H_Matrix, glm::vec3(0.2, 4.0, 0.2));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(0.0, 0.1, 0.0)); // 스케일 피봇 설정
    HurdletransformLocation = glGetUniformLocation(s_program, "transform");
    glUniformMatrix4fv(HurdletransformLocation, 1, GL_FALSE, glm::value_ptr(H_Matrix));
    //glUniform1f(isCheck, true);
    objColorLocation = glGetUniformLocation(s_program, "objectColor");
    glUniform3f(objColorLocation, 0.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //hurdle_r_stick_end
    glBindVertexArray(VAO[0]);
    H_Matrix = glm::mat4(1.0f);
    H_Matrix = glm::rotate(H_Matrix, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(hurdle_x, hurdle_y, hurdle_z));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(0.4, 0.8, 0.0));
    H_Matrix = glm::rotate(H_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    H_Matrix = glm::scale(H_Matrix, glm::vec3(0.3, 0.3, 0.3));
    HurdletransformLocation = glGetUniformLocation(s_program, "transform");
    glUniformMatrix4fv(HurdletransformLocation, 1, GL_FALSE, glm::value_ptr(H_Matrix));
    objColorLocation = glGetUniformLocation(s_program, "objectColor");
    glUniform3f(objColorLocation, 1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //hurdle_l_stick
    glBindVertexArray(VAO[0]);
    H_Matrix = glm::mat4(1.0f);
    H_Matrix = glm::rotate(H_Matrix, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(hurdle_x, hurdle_y, hurdle_z));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(-0.4, 0.0, 0.0));
    H_Matrix = glm::rotate(H_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    H_Matrix = glm::scale(H_Matrix, glm::vec3(0.2, 4.0, 0.2));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(0.0, 0.1, 0.0)); // 스케일 피봇 설정
    HurdletransformLocation = glGetUniformLocation(s_program, "transform");
    glUniformMatrix4fv(HurdletransformLocation, 1, GL_FALSE, glm::value_ptr(H_Matrix));
    objColorLocation = glGetUniformLocation(s_program, "objectColor");
    glUniform3f(objColorLocation, 0.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // hurdle_l_stick_end
    glBindVertexArray(VAO[0]);
    H_Matrix = glm::mat4(1.0f);
    H_Matrix = glm::rotate(H_Matrix, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(hurdle_x, hurdle_y, hurdle_z));
    H_Matrix = glm::translate(H_Matrix, glm::vec3(-0.4, 0.8, 0.0));
    H_Matrix = glm::rotate(H_Matrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    H_Matrix = glm::scale(H_Matrix, glm::vec3(0.3, 0.3, 0.3));
    HurdletransformLocation = glGetUniformLocation(s_program, "transform");
    glUniformMatrix4fv(HurdletransformLocation, 1, GL_FALSE, glm::value_ptr(H_Matrix));
    objColorLocation = glGetUniformLocation(s_program, "objectColor");
    glUniform3f(objColorLocation, 1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //tile
    glBindVertexArray(VAO[0]);
    glm::mat4 T_Matrix = glm::mat4(1.0f);
    T_Matrix = glm::translate(T_Matrix, glm::vec3(hurdle_x, hurdle_y, hurdle_z));
    T_Matrix = glm::translate(T_Matrix, glm::vec3(0.0, 0.0, 0.0));
    T_Matrix = glm::scale(T_Matrix, glm::vec3(MS.tile_size * 10, 0.2, MS.tile_size * 10));
    T_Matrix = glm::translate(T_Matrix, glm::vec3(0.0, -0.1, 0.0));
    unsigned int TiletransformLocation = glGetUniformLocation(s_program, "transform");
    glUniformMatrix4fv(TiletransformLocation, 1, GL_FALSE, glm::value_ptr(T_Matrix));
    objColorLocation = glGetUniformLocation(s_program, "objectColor");
    glUniform3f(objColorLocation, 0.5f, 0.5f, 0.5f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

void Change_Light_Colors() {
    if (LO.onoff_checkpoint_light) {
        LO.checkpoint_light_timer += 0.005;
        if (LO.checkpoint_light_timer > 1.0) {

            LO.r_light = LO.base_color[0];
            LO.g_light = LO.base_color[1];
            LO.b_light = LO.base_color[2];

            background_color_r = 0;
            background_color_g = 0;
            background_color_b = 0;

            LO.checkpoint_light_timer = 0;
            LO.onoff_checkpoint_light = false;
        }

        if (0 < LO.checkpoint_light_timer && LO.checkpoint_light_timer < 0.05) {
            LO.r_light = LO.checkpoint_color[0];
            LO.g_light = LO.checkpoint_color[1];
            LO.b_light = LO.checkpoint_color[2];

            background_color_r = 0.0;
            background_color_g = 0.2;
            background_color_b = 0.0;
        }
        else if (0.05 < LO.checkpoint_light_timer && LO.checkpoint_light_timer < 0.1) {
            LO.r_light = LO.base_color[0];
            LO.g_light = LO.base_color[1];
            LO.b_light = LO.base_color[2];

            background_color_r = 0;
            background_color_g = 0;
            background_color_b = 0;
        }
        else if (0.1 < LO.checkpoint_light_timer && LO.checkpoint_light_timer < 0.15) {
            LO.r_light = LO.checkpoint_color[0];
            LO.g_light = LO.checkpoint_color[1];
            LO.b_light = LO.checkpoint_color[2];

            background_color_r = 0.0;
            background_color_g = 0.2;
            background_color_b = 0.0;
        }
        else {
            LO.r_light = LO.base_color[0];
            LO.g_light = LO.base_color[1];
            LO.b_light = LO.base_color[2];

            background_color_r = 0;
            background_color_g = 0;
            background_color_b = 0;
        }

    }
    else if (LO.onoff_Arrive_light) {
        LO.Arrive_light_timer += 0.005;
        //초기값 설정
        if (LO.Arrive_light_timer > 1.0) {

            LO.r_light = LO.base_color[0];
            LO.g_light = LO.base_color[1];
            LO.b_light = LO.base_color[2];

            background_color_r = 0;
            background_color_g = 0;
            background_color_b = 0;

            LO.Arrive_light_timer = 0;
            LO.onoff_Arrive_light = false;
            LO.onoff_Arrive_light_finish = true;
        }
        if (LO.Arrive_light_timer > 0.0 && LO.Arrive_light_timer <= 1.0) {
            LO.r_light = RandomColor(0);
            LO.g_light = RandomColor(0);
            LO.b_light = RandomColor(0);
        }
    }

    if (LO.onoff_Arrive_light_finish == true) {
        cout << "Game Clear !!" << endl;
        exit(0);
    }

}


void Copy_All()
{
    memcpy(&MS_temp, &MS, sizeof Map_Setting);
    memcpy(&road_type_temp, &road_type, sizeof Road_Type);
    memcpy(&B_timer_temp, &B_timer, sizeof Bounce_Timer);

    memcpy(&BC_temp, &BC, sizeof Ball_Control);
    memcpy(&CO_temp, &CO, sizeof Camera_Option);
}
void Reset_All()
{
    memcpy(&MS, &MS_temp, sizeof Map_Setting);
    memcpy(&road_type, &road_type_temp, sizeof Road_Type);
    memcpy(&B_timer, &B_timer_temp, sizeof Bounce_Timer);
    memcpy(&BC, &BC_temp, sizeof Ball_Control);
    memcpy(&CO, &CO_temp, sizeof Camera_Option);
}
void Make_shadow()
{
    glm::mat4 S_Matrix = glm::mat4(1.0f);
    S_Matrix = glm::translate(S_Matrix, glm::vec3(BC.ball_move_x, BC.shadow_trans_y, BC.ball_move_z));
    S_Matrix = glm::scale(S_Matrix, glm::vec3(Ball_Bounce_Ani('s', BO.ball_radius * 10), 0.1f, Ball_Bounce_Ani('s', BO.ball_radius * 10)));
    unsigned int ShadowtransformLocation = glGetUniformLocation(s_program, "transform");
    glUniformMatrix4fv(ShadowtransformLocation, 1, GL_FALSE, glm::value_ptr(S_Matrix));
    int objColorLocation = glGetUniformLocation(s_program, "objectColor");
    glUniform3f(objColorLocation, 0.0f, 0.0f, 0.0f);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    if (BC.ball_move_y >= -0.5)
        gluSphere(qobj, 0.1, 30, 30);
}
float RandomColor(int color) {
    return (float)(rand() % random_num) / 100 * 5.0;
}
GLvoid make_vertexShaders()
{
    GLchar* vertexSource;
    vertexSource = filetobuf("vertexlight.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

}
GLvoid make_fragmentShader()
{
    GLchar* fragmentSource;
    fragmentSource = filetobuf("fragmentlight.glsl");
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

    /*else
        cout << "good";*/
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //--- 위치 속성
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
    glEnableVertexAttribArray(1);


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