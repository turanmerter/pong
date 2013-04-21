
//
//  main.cpp
//  Bilgisayar Proje II - Real-Time Computer Graphics
//  Project #II - Pong game
//
//  Merter TURAN - turanmerter@gmail.com
//  040080204
//

#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include "Scene.h"
#include <GLUT.h>
#include "ScreenSpaceShaderNode.h"
#include "NormalShaderNode.h"
#include "ColorFillNode.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#define WINDOW_TITLE_PREFIX "Ders 7"

void IdleFunction(void);
void ResizeFunction(int Width, int Height);
void setupScene();

unsigned FrameCount = 0;

int CurrentWidth = 800,
CurrentHeight = 600,
WindowHandle = 0, mouseX = 0, mouseY = 0;

SceneNode* pad1;
SceneNode* pad2;
SceneNode* ball;

Camera *camera;

Scene* scene;

float directionX = 1;
float directionY = 0.5;
float speed = 0.0001;

static void Draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    scene->draw();
    
    glutSwapBuffers();
    FrameCount++;
}

static void Key(unsigned char key, int x, int y)
{
    printf("%d", key);
	switch (key) {
		case 27: // ESC
			exit(0);
            break;
        case 'a': // ESC
			pad1->translate(0, -2, 0);
            break;
        case 'q': // ESC
			pad1->translate(0, 2, 0);
            break;
        case 's': // ESC
			pad2->translate(0, -2, 0);
            break;
        case 'w': // ESC
			pad2->translate(0, 2, 0);
            break;
	}
}
void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

static void timerCallback (int value)
{
    if (0 != value) {
		char* TempString = (char*)
        malloc(512 + strlen(WINDOW_TITLE_PREFIX));
        
		sprintf(
                TempString,
                "%s: %d Frames Per Second @ %d x %d",
                WINDOW_TITLE_PREFIX,
                FrameCount * 4,
                CurrentWidth,
                CurrentHeight
                );
        
		glutSetWindowTitle(TempString);
		free(TempString);
	}
	FrameCount = 0;
	glutTimerFunc(250, timerCallback, 1);
}

void IdleFunction(void)
{
    glm::mat4 ballMatrix = ball->getModelMatrix();
    glm::mat4 pad1Matrix = pad1->getModelMatrix();
    glm::mat4 pad2Matrix = pad2->getModelMatrix();
    float x = ball->getPosition().x;
    float y = ball->getPosition().y;
    
     if( y > 0.6 )
    {
        directionY *= -1;
    }
    if( y < -0.6 )
    {
        directionY *= -1;
    }
    if( x > 1.0  &&  abs( pad2->getPosition().y - ball->getPosition().y) < 0.2 )
    {
        directionX *= -1;
    }
    else if (x > 1.0)
	{
        exit(0);
    }
    if( x < -1.0 && abs( pad1->getPosition().y - ball->getPosition().y) < 0.2 ) 
    {
        directionX *= -1;
    }
    else if (x < -1.0)
	{
        exit(0);
    }

    ball->translate(directionX*speed, directionY*speed, 0);
	glutPostRedisplay();
}

void setupScene(){
    scene = new Scene();
    
    pad1 = new ScreenSpaceShaderNode("capsule.obj");
    
    pad2 = new ColorFillNode("capsule.obj");
    
    ball = new ColorFillNode("bunny.obj");
    
    pad1->scale(0.01,0.01,0.01);
    pad2->scale(0.01,0.01,0.01);
    
    scene->addNode(pad1);
    scene->addNode(pad2);
    scene->addNode(ball);
    
    camera = new Camera();
    
    camera->translate(0.f, 0.f, 3.f);
    
    pad1->translate(-100.f, 0.f, 0.f);
    
    pad2->translate( 100.f, 0.f, 0.f);
    
    scene->setCamera(camera);
}

int main (int argc, char ** argv)
{
    
    GLenum type;
    
	glutInit(&argc, argv);
	
	glutInitWindowSize(CurrentWidth,CurrentHeight);
	type = GLUT_RGB;
	type |= GLUT_DOUBLE;
	type |= GLUT_DEPTH;
    type |= GLUT_MULTISAMPLE;
	glutInitDisplayMode(type);
	glutCreateWindow("");
    glewInit();
    glClearColor(0.0, 1.0, 0.0, 1.0);
    // Z-Buffer i aciyoruz
    glEnable(GL_DEPTH_TEST);
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE); 
    
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    printf("GLSL Version: %s\n",glslVersion);
    
    const GLubyte* glVersion = glGetString(GL_VERSION);
    
    printf("GL Version: %s\n",glVersion);
       
    setupScene();
    
    timerCallback(0);
    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(Draw);
    glutKeyboardFunc(Key);
    glutIdleFunc(IdleFunction);
    glutMainLoop();
    
    return 0;
}


