#ifndef __SHADER_STUFF
#define __SHADER_STUFF
#define M_PI 3.14159265358979323846

// References:
// https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/loading.php

#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "texture_loader.hpp"

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include "obj_loader.hpp"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/SOIL.h>
#include <windows.h>
#include <mmsystem.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// ---------------------------------------------

// Variables for control the view

GLfloat _scene_rotate_x = 0.0f;
GLfloat _scene_rotate_y = 0.0f;
GLfloat _scene_translate_x = 0.0f;
GLfloat _scene_translate_y = 0.0f;
GLfloat _scene_translate_z = 0.0f;
GLuint coordBuffer;
GLuint vertexArray;
GLuint vIndexBuffer;
GLuint vNormalBuffer;
GLuint vBufferUV;
GLuint lightsBuffer;
GLuint specLightsBuffer;
GLuint vInstances;
int _mouse_buttonState = GLUT_UP;
int right_btn_state = GLUT_UP;
int _mouse_buttonX, _mouse_buttonY;
GLuint currentProgram;
GLuint viewMatrixLocation;
GLuint modelMatrixLocation;
GLuint projMatrixLocation;
int lightsPointer = 0;
float lights[12];

int specLightsPointer = 0;
float specLights[16];

GLuint TextureID;
GLuint Texture1ID;

float gunShot = 0.f;
bool gunShotBool = false;
int magazine = 17;
int ammo = 17 * 4;
float reload = 0.f;
bool reloadBool = false;

// --------------------------------------------------------------
void SpecialKeysUp( int key, int x, int y )
{
    switch( key )
    {

        case GLUT_KEY_LEFT:
            _scene_translate_x = 0.f;
            break;

        case GLUT_KEY_RIGHT:
            _scene_translate_x = 0.f;
            break;
		case GLUT_KEY_DOWN:
            _scene_translate_y = 0.f;
            break;
        case GLUT_KEY_UP:
            _scene_translate_y = 0.f;
            break;
    }
	// glutPostRedisplay();
}

void SpecialKeys( int key, int x, int y )
{
    switch( key )
    {

        case GLUT_KEY_LEFT:
            _scene_translate_x = -0.1f;
            break;

        case GLUT_KEY_RIGHT:
            _scene_translate_x = 0.1f;
            break;
		case GLUT_KEY_DOWN:
            _scene_translate_y = -0.1f;
            break;
        case GLUT_KEY_UP:
            _scene_translate_y = 0.1f;
            break;
    }
	// glutPostRedisplay();
}
// --------------------------------------------------------------
void Keyboard( unsigned char key, int x, int y )
{
    switch(key)
    {
		case 27:	// ESC key
			exit(0);
			break;
    }



}

// --------------------------------------------------------------
void MouseButton( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON )
    {

        _mouse_buttonState = state;

        if( state == GLUT_DOWN )
        {	
			// SoundEngine->play2D("C:/Users/jakub/projects/helloworld/gunshot.wav", false);
			// mciSendString(TEXT("play C:/Users/jakub/projects/helloworld/gunshot.wav"), NULL, 0, NULL);
			if (magazine > 0 && !reloadBool){
				mciSendString(TEXT("seek gun to start"),NULL,100,NULL);
				mciSendString(TEXT("play gun"),NULL,0,NULL);
				// PlaySound(TEXT("C:/Users/jakub/projects/helloworld/gunshot.wav"), NULL, SND_FILENAME | SND_ASYNC);
				gunShot = 1.f;
				gunShotBool = true;
				magazine--;
			}
			else{
				if (!reloadBool){
					mciSendString(TEXT("seek noammo to start"),NULL,100,NULL);
					mciSendString(TEXT("play noammo"),NULL,0,NULL);
				}
			}
            _mouse_buttonX = x;
            _mouse_buttonY = y;
        }
		else {
			_scene_rotate_y = 0.f;
			_scene_rotate_x = 0.f;
		}

    }
	if (button == GLUT_RIGHT_BUTTON){
		if (magazine < 17 && ammo > 0){
			reload = 1.f;
			reloadBool = true;
			mciSendString(TEXT("seek reload to start"),NULL,100,NULL);
			mciSendString(TEXT("play reload"),NULL,0,NULL);
			if (ammo < 17 - magazine) {
				magazine += ammo;
				ammo = 0;
			}
			else{
				ammo -= 17 - magazine;
				magazine = 17;
			}
		}
	}
}

// --------------------------------------------------------------
void MouseMotion( int x, int y )
{
    if( _mouse_buttonState == GLUT_DOWN )
    {
        _scene_rotate_y = 2.f*(float)(x - _mouse_buttonX)/(float)glutGet( GLUT_WINDOW_WIDTH );
        _mouse_buttonX = glutGet(GLUT_WINDOW_HEIGHT) / 2;
        _scene_rotate_x = -2.f*(float)(_mouse_buttonY - y)/(float)glutGet( GLUT_WINDOW_HEIGHT );
        _mouse_buttonY = glutGet(GLUT_WINDOW_WIDTH) / 2;
		glutWarpPointer(glutGet(GLUT_WINDOW_HEIGHT) / 2, glutGet(GLUT_WINDOW_WIDTH) / 2);
    }
	if (right_btn_state == GLUT_DOWN){
		_scene_translate_z = 2*(_mouse_buttonY - y)/(float)glutGet( GLUT_WINDOW_HEIGHT );
        _mouse_buttonY = y;
	}
	else {
		_scene_translate_z = 0.f;
	}

	// glutPostRedisplay();
}

// int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
// int winWidth = glutGet(GLUT_WINDOW_HEIGHT);

void PassiveMotion(int x, int y){
	_scene_rotate_y = 2.f*(float)(x - _mouse_buttonX)/(float)glutGet( GLUT_WINDOW_WIDTH );
	_mouse_buttonX = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	_scene_rotate_x = -2.f*(float)(_mouse_buttonY - y)/(float)glutGet( GLUT_WINDOW_HEIGHT );
	_mouse_buttonY = glutGet(GLUT_WINDOW_WIDTH) / 2;
	glutWarpPointer(glutGet(GLUT_WINDOW_HEIGHT) / 2, glutGet(GLUT_WINDOW_WIDTH) / 2);
}

// --------------------------------------------------------------
void MouseWheel(int button, int dir, int x, int y)
{

}

unsigned long getFileLength(std::ifstream& file)
{
	if(!file.good()) return 0;

	unsigned long pos=file.tellg();
	file.seekg(0,std::ios::end);
	unsigned long len = file.tellg();
	file.seekg(std::ios::beg);

	return len;
}

// ---------------------------------------------
GLchar * LoadShaderFile(const char* filename)
{
	std::ifstream file;
	file.open(filename, std::ios::in); // opens as ASCII!
	if(!file) { printf("Can't open file %s !\n", filename); exit(1); }

	unsigned long len = getFileLength(file);

	if (len==0) { printf("File %s is empty!\n", filename); exit(1); };   // Error: Empty File

	GLchar *ShaderSource = new GLchar[len+1];
	if (ShaderSource == NULL) { printf("Can't reserve memory %d \n", len+1); exit(1); }   // can't reserve memory

	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value...
	ShaderSource[len] = 0;

	unsigned int i=0;
	while (file.good())
	{
		ShaderSource[i] = file.get();
		if (!file.eof())
		i++;
	}

	ShaderSource[i] = 0;
	file.close();
	return ShaderSource;
}

// ---------------------------------------
void CheckForErrors_Shader(GLuint shader)
{
	GLint status;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
	if( status != GL_TRUE )
	{
		printf("Blad!\n");
		GLint logLength;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
		char *log = new char[logLength];
		glGetShaderInfoLog( shader, logLength, NULL, log );
		printf("LOG: %s\n", log);
		delete[] log;
		exit(1);
	}
}

// ---------------------------------------
void CheckForErrors_Program(GLuint program, GLenum mode)
{
	GLint status;
	glGetProgramiv( program, mode, &status );
	if( status != GL_TRUE )
	{
		switch (mode)
		{
			case GL_LINK_STATUS:
				printf("Blad konsolidacji programu!\n");
				break;
			case GL_VALIDATE_STATUS:
				printf("Blad walidacji programu!\n");
				break;
			default:
				printf("Inny blad programu!\n");

		}
		GLint logLength;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
		char *log = new char[logLength];
	    glGetProgramInfoLog( program, logLength, NULL, log );
		printf("LOG: %s\n", log);
		delete[] log;
		exit(1);
	}
}

// ---------------------------------------
GLuint LoadShader( GLuint MODE, const char *filename  )
{
	// utworzenie obiektu shadera
	GLuint shader = glCreateShader( MODE );

	// Wczytanie kodu shadera z pliku
	GLchar *code = LoadShaderFile(filename);

	glShaderSource( shader, 1, &code, NULL );
	glCompileShader( shader );
	CheckForErrors_Shader(shader);

	delete[] code;
	return shader;
}

GLuint LoadShader(GLuint MODE, char shader_stream_start[], char shader_stream_end[])
{
	GLuint shader = glCreateShader( MODE );
	
	size_t size = (shader_stream_end - shader_stream_start) / sizeof(char);
	
	GLchar * code = new GLchar[size + 1];
	code[size] = 0;
	int iter=0;
	for (char* p = shader_stream_start; p != shader_stream_end; p++) {
		code[iter] = *p;
		iter++;
    }
	code[iter] = 0;
	cout << "=============================================================" << endl;
	if (MODE == GL_VERTEX_SHADER){
		cout << "Loaded vertex shader:" << endl;
	}
	if (MODE == GL_FRAGMENT_SHADER){
		cout << "Loaded fragment shader:" << endl;
	}
	cout << "=============================================================" << endl << endl;
	cout << code << endl;

	glShaderSource( shader, 1, &code, NULL );
	glCompileShader( shader );
	CheckForErrors_Shader(shader);

	delete[] code;
	return shader;
}

void clear(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	specLightsPointer = 0;
	lightsPointer = 0;
	GLuint bindingPoint;
	GLuint blockID;
	// spec lights
	// glBufferData(GL_UNIFORM_BUFFER, specLights.size()*sizeof(float), NULL, GL_DYNAMIC_DRAW);
	blockID = glGetUniformBlockIndex(currentProgram, "specLights");

	bindingPoint = 1;
	glUniformBlockBinding(currentProgram, blockID, bindingPoint);

	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, specLightsBuffer);
	
	glBindBuffer(GL_UNIFORM_BUFFER, specLightsBuffer);

	// glBufferSubData(GL_UNIFORM_BUFFER, 0, specLights.size()*sizeof(float), &specLights[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(float), specLights, GL_DYNAMIC_DRAW);

	// just lights
	// glBufferData(GL_UNIFORM_BUFFER, lights.size()*sizeof(float), NULL, GL_DYNAMIC_DRAW);
	blockID = glGetUniformBlockIndex(currentProgram, "lights");

	bindingPoint = 2;
	glUniformBlockBinding(currentProgram, blockID, bindingPoint);

	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, lightsBuffer);
	
	glBindBuffer(GL_UNIFORM_BUFFER, lightsBuffer);

	// glBufferSubData(GL_UNIFORM_BUFFER, 0, lights.size()*sizeof(float), &lights[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lights), lights, GL_DYNAMIC_DRAW);


	
}

void InitializeBuffers(){
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers( 1, &coordBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, coordBuffer );

	glGenBuffers( 1, &vNormalBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vNormalBuffer );

	glGenBuffers( 1, &vNormalBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vNormalBuffer );

	glGenBuffers( 1, &vBufferUV );
	glBindBuffer( GL_ARRAY_BUFFER, vBufferUV );

	glGenBuffers(1, &lightsBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightsBuffer);

	glGenBuffers(1, &specLightsBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, specLightsBuffer);

	glGenBuffers(1, &vInstances);
	glBindBuffer(GL_ARRAY_BUFFER, vInstances);

	glGenTextures(1, &TextureID);
}

// ---------------------------------------
void LinkAndValidateProgram(GLuint program)
{
    // Konsolidacja programu
    glLinkProgram( program );
	CheckForErrors_Program(program, GL_LINK_STATUS);

	// Walidacja programu
    glValidateProgram( program );
    CheckForErrors_Program(program, GL_VALIDATE_STATUS);
}

glm::vec3 extractCameraPos(const glm::mat4 & a_modelView)
{
  glm::mat4 modelViewT = transpose(a_modelView);

  glm::vec3 n1(modelViewT[0]);
  glm::vec3 n2(modelViewT[1]);
  glm::vec3 n3(modelViewT[2]);

  float d1(modelViewT[0].w);
  float d2(modelViewT[1].w);
  float d3(modelViewT[2].w);

  glm::vec3 n2n3 = cross(n2, n3);
  glm::vec3 n3n1 = cross(n3, n1);
  glm::vec3 n1n2 = cross(n1, n2);

  glm::vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
  float denom = dot(n1, n2n3);

  return top / -denom;
}

glm::quat rotationBetweenVectors(glm::vec3 start, glm::vec3 dest){
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (length2(rotationAxis) < 0.01 )
		{
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
		}

		rotationAxis = normalize(rotationAxis);
		return angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = glm::cross(start, dest);

	float s = glm::sqrt( (1+cosTheta)*2 );
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f, 
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);

}

glm::mat3x3 rodriguesMatrix(const double degrees, const glm::vec3& axis) {
	glm::mat3x3 v = glm::mat3x3(
		axis.x*axis.x, axis.x*axis.y, axis.x*axis.z,
		axis.x*axis.y, axis.y*axis.y, axis.y*axis.z,
		axis.x*axis.z, axis.y*axis.z, axis.z*axis.z
		);

	glm::mat3x3 v2 = glm::mat3x3(
		0, -axis.z, axis.y,
		axis.z, 0, -axis.x,
		-axis.y, axis.x, 0
		);

	glm::mat3x3 rotation = cosf(degrees * M_PI/180.f) * glm::mat3x3(1.0f) + (1.f-cosf(degrees * M_PI/180.f)) * v + sinf(degrees * M_PI/180.f) * v2;

	return rotation;
}

class CTriangle
{
public:

	// wspolrzedne punktow trojkata
	// uwaga! os pionowa w tym wypadku to z
	glm::vec3 p[3];


	// rownanie plaszczyzny Ax + By + Cz + D = 0
	float A,B,C,D;

	CTriangle() { }
	CTriangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3)
	{
		p[0] = p1;
		p[1] = p2;
		p[2] = p3;

		calculateEquation();
	}

	// obliczanie wspolczynnikow rownania
	void calculateEquation(void)
	{
		glm::vec3 v1, v2;

		v1 = p[1] - p[0];
		v2 = p[2] - p[0];

		A = v1.y * v2.z - v1.z * v2.y;	 // A
		B = v1.z * v2.x - v1.x * v2.z;   // B
		C = v1.x * v2.y - v1.y * v2.x;   // C


		D = - (A*p[0].x + B*p[0].y + C*p[0].z);	// D

		if (C == 0)
		{
			printf("Uwaga! Trojkat pionowy.\n");
		}

	}

	// czy punkt p1 jest po lewej stronie odcinka p2-p3
	inline float sign (glm::vec2 p1, glm::vec3 p2, glm::vec3 p3)
	{
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}

	// czy punkt jest wewnatrz trojkata rzutowanego na plaszczyzne xy
	inline bool isInside (glm::vec2 point)
	{
		float s1, s2, s3;
		bool has_neg, has_pos;

		s1 = sign(point, p[0], p[1]);
		s2 = sign(point, p[1], p[2]);
		s3 = sign(point, p[2], p[0]);

		has_neg = (s1 < 0) || (s2 < 0) || (s3 < 0);
		has_pos = (s1 > 0) || (s2 > 0) || (s3 > 0);

		return !(has_neg && has_pos);
	}

	glm::vec3 getNormal(){
		return glm::vec3(this->A,this->B,this->C);
	}

	// obliczamy wysokosc punktu w trojkacie
	inline float altitude(glm::vec2 point)
	{
		if (C) return -(A*point.x + B*point.y + D)/C;
		else { return 99999; }
	}

};

class GameObject{
	protected:
		glm::vec3 position = glm::vec3(0.f);
		glm::vec3 scaling = glm::vec3(1.f, 1.f, 1.f);
		glm::vec3 rotation = glm::vec3(0.f);
	public:
		glm::vec3 getScaling(){
			return this->scaling;
		}
		glm::vec3 getPosition(){
			return this->position;
		}
		glm::vec3 getUp(){
			return glm::vec3(this->getMatrix()[1]);
		}
		glm::vec3 getFront(){
			return glm::vec3(this->getMatrix()[2]);
		}
		void setScaling(float x, float y, float z){
			this->scaling = glm::vec3(x,y,z);
		}
		void setPosition(float x, float y, float z){
			this->position = glm::vec3(x,y,z);
		}
		void setRotation(float x, float y, float z){
			this->rotation = glm::vec3(x,y,z);
		}
		void setPosition(glm::vec3 pos){
			this->position = pos;
		}
		void translate(float x, float y, float z){
			this->position += glm::vec3(x,y,z);
		}
		void rotate(float x, float y, float z){
			this->rotation += glm::vec3(x,y,z);
		}
		void scale(float x, float y, float z){
			this->scaling += glm::vec3(x,y,z);
		}
		glm::vec3 getRight(){
			return glm::vec3(this->getMatrix()[0]);
		}
		void moveInLocalCoor(float amount, glm::vec3 direction){
			this->position += glm::normalize(
									direction.x * glm::normalize(this->getRight()) + 
									direction.y * glm::normalize(this->getUp()) + 
									direction.z * glm::normalize(this->getFront())
								) * amount;
		}

		void moveForward(float amount){
			this->moveInLocalCoor(amount, glm::vec3(0.f,0.f,1.f));
		}
		void moveUp(float amount){
			this->moveInLocalCoor(amount, glm::vec3(0.f,1.f,0.f));
		}void moveRight(float amount){
			this->moveInLocalCoor(amount, glm::vec3(1.f,0.f,0.f));
		}
		glm::mat4x4 getMatrix(){
			glm::mat4x4 mat = glm::mat4x4(1.f);
			mat = glm::translate(mat, this->position);
			mat = mat * glm::mat4_cast(glm::quat(this->rotation));
			
			mat = glm::scale(mat, this->scaling);
			return mat;
		}
		void display(){

		}
};

class Light : public GameObject
{
	protected:
		glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		glm::vec3 position = glm::vec3(0.f,0.f,0.f);
	public:
	Light(){

	}
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 position){
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->position = position;
	}
	void pushAttribs(float * list, int & pointer){
		list[pointer] = this->ambient.x;
		list[pointer + 1] = this->ambient.y;
		list[pointer + 2] = this->ambient.z;
		list[pointer + 3] = 1.f;
		pointer += 4;

		list[pointer] = this->diffuse.x;
		list[pointer + 1] = this->diffuse.y;
		list[pointer + 2] = this->diffuse.z;
		list[pointer + 3] = 1.f;
		pointer += 4;

		list[pointer] = this->position.x;
		list[pointer + 1] = this->position.y;
		list[pointer + 2] = this->position.z;
		list[pointer + 3] = 1.f;
		pointer += 4;
	}
	void display(){
		this->pushAttribs(lights, lightsPointer);
	}
};

class SpecularLight : public Light{
	private:
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	public:
		SpecularLight(){}
		SpecularLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 position, glm::vec3 specular){
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->position = position;
			this->specular = specular;
		}
		void pushAttribs(float * list, int & pointer){
			Light::pushAttribs(list, pointer);

			list[pointer] = this->specular.x;
			list[pointer + 1] = this->specular.y;
			list[pointer + 2] = this->specular.z;
			list[pointer + 3] = 1.f;
			pointer += 4;
		}
		void display(){
			this->pushAttribs(specLights, specLightsPointer);
		}
};

class CompositeGameObject : public GameObject
{
	private:
		vector<GameObject> objects;
	public:
		CompositeGameObject(vector<GameObject> objects): objects(objects){}
		void addObject(GameObject object){
			objects.push_back(object);
		}
		void display(){
			// for (auto &object : this->objects){
			// 	object->display();
			// }
		}
};

class Viewport : public GameObject 
{
	public:
		Viewport(glm::vec2 position, glm::vec2 size){
			this->position = glm::vec3(position, 0.f);
			this->scaling = glm::vec3(size, 0.f);
		}
		glm::mat4x4 getProjMatrix(){
			return glm::perspectiveFov(glm::radians(60.0f), (float)this->scaling.x, (float)this->scaling.y, 0.1f, 100.f );
		}
		void display(){
			glViewport((float)glutGet(GLUT_WINDOW_WIDTH) * this->position.x, (float)glutGet(GLUT_WINDOW_HEIGHT) * this->position.y, this->scaling.x, this->scaling.y );
			glUniformMatrix4fv( projMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->getProjMatrix()) );
		}
};

class Camera : public GameObject
{	
	private:
		glm::vec3 front = glm::vec3(0.f, 0.f, -1.f);
		glm::vec3 up =  glm::vec3(0.f, 1.f, 0.f);
	public:
		Camera(){
			this->front = glm::vec3(0.f,0.f,1.f);
		}
		Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp){
			this->position = cameraPos;
			this->front = cameraFront;
			this->up = cameraUp;
		}
		void rotateCoor(float amount, glm::vec3 axis){
			if (abs(amount) > 0.1f){
				glm::mat3x3 rotation = rodriguesMatrix(amount, axis);
				this->front = glm::vec3(rotation * this->front);
			}
		}
		void moveInLocalCoor(float amount, glm::vec3 direction){
			this->position += glm::normalize(
									direction.x * glm::normalize(this->getRight()) + 
									direction.y * glm::normalize(this->getUp()) + 
									direction.z * glm::normalize(this->getFront())
								) * amount;
		}

		void moveForward(float amount){
			this->moveInLocalCoor(amount, glm::vec3(0.f,0.f,1.f));
		}
		void moveUp(float amount){
			this->moveInLocalCoor(amount, glm::vec3(0.f,1.f,0.f));
		}void moveRight(float amount){
			this->moveInLocalCoor(amount, glm::vec3(1.f,0.f,0.f));
		}
		void rotateAroundUp(float amount){
			this->rotateCoor(amount, this->getUp());
		}
		void rotateAroundRight(float amount){
			this->rotateCoor(amount, this->getRight());
		}
		void rotateAroundFront(float amount){
			this->rotateCoor(amount, this->getFront());
		}
		void lookAt(float x, float y, float z){
			this->front = glm::vec3(x,y,z) - this->position;
		}
		glm::vec3 getRight(){
			return glm::cross(this->getFront(), this->getUp());
		}
		glm::vec3 getUp(){
			return this->up;
		}
		void setUp(glm::vec3 up){
			this->up = up;
		}
		void setFront(glm::vec3 front){
			this->front = front;
		}
		glm::mat4x4 getMatrix(){
			return glm::lookAt(this->position, this->position - this->front, this->up);
		}
		glm::vec3 getPosition(){
			glm::vec3 returnVect = this->position;
			return returnVect;
		}
		glm::vec3 getFront(){
			return -this->front;
		}
		void display(){
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->getMatrix()));
		}
};

class Program
{
	private:
		GLuint program;
	public:
		Program(const char *vShaderFilename, const char *fShaderFilename){
			this->program = glCreateProgram();
			glAttachShader(this->program, LoadShader(GL_VERTEX_SHADER, vShaderFilename));
			glAttachShader(this->program, LoadShader(GL_FRAGMENT_SHADER, fShaderFilename));
			LinkAndValidateProgram(this->program);
		}
		Program(){
			this->program = glCreateProgram();
		}
		GLuint getProgram(){
			return this->program;
		}
		void useThis(){
			currentProgram = this->program;
			glUseProgram(currentProgram);
			viewMatrixLocation = glGetUniformLocation( currentProgram, "viewMatrix" );
			modelMatrixLocation = glGetUniformLocation( currentProgram, "modelMatrix" );
			projMatrixLocation = glGetUniformLocation( currentProgram, "projMatrix" );
		}
		~Program(){
			glDeleteProgram(this->program);
		}
		void setUniformMatrix(const char *name, glm::mat4x4 matrix){
			glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(matrix));
		}
};

class Object : public GameObject
{
	public:
		Object(){}

		void alignLocalVectorWith(glm::vec3 vectorToAlign, glm::vec3 vectorToAlignWith){
			glm::vec3 toAlign = this->getRight() * vectorToAlign.x + this->getUp() * vectorToAlign.y + this->getFront() * vectorToAlign.z;
			glm::quat rotation = rotationBetweenVectors(toAlign, vectorToAlignWith);
			this->rotation = glm::eulerAngles(rotation * glm::quat(this->rotation));
		}

		void alignFrontWith(glm::vec3 vectorToAlignWith){
			this->alignLocalVectorWith(glm::vec3(0.f,0.f,1.f), vectorToAlignWith);
		}

		void alignRightWith(glm::vec3 vectorToAlignWith){
			this->alignLocalVectorWith(glm::vec3(1.f,0.f,0.f), vectorToAlignWith);
		}

		void drawFunc(){}

		void prepareBuffers(){}

		void prepareShader(){}

		void initialize(){}

		void display(){}
};

class ObjectFromArray: public Object
{
	protected:
		std::vector<glm::vec3> array;
		glm::vec3 boundingBoxOrigin = glm::vec3(0.f);
		glm::vec3 boundingBoxWidth = glm::vec3(0.f);
		glm::vec3 boundingBoxHeight = glm::vec3(0.f);
		glm::vec3 boundingBoxDepth = glm::vec3(0.f);
		glm::vec3 centerOfGravity = glm::vec3(0.f);
		float radius = 0.f;
	public:
		ObjectFromArray(){}

		ObjectFromArray(std::vector<glm::vec3> array){
			this->array = array;
		}

		ObjectFromArray(const char * path){
			std::vector<glm::vec3> out_vertices;
			std::vector<glm::vec2> out_uvs;
			std::vector<glm::vec3> out_normals;
			loadOBJ(path, out_vertices, out_uvs, out_normals);
			this->array = out_vertices;
			this->boundingBoxOrigin = this->array.front();
			this->boundingBoxWidth = glm::vec3(0,0,0);
			this->boundingBoxHeight = glm::vec3(0,0,0);
			this->boundingBoxDepth = glm::vec3(0,0,0);
			
			this->centerOfGravity = this->array[0];

			for( int i=1;i < this->array.size(); i++){
				this->centerOfGravity += this->array[i];
				if(this->array[i].x < this->boundingBoxOrigin.x){
					this->boundingBoxOrigin.x = this->array[i].x;
				}
				if(this->array[i].y < this->boundingBoxOrigin.y){
					this->boundingBoxOrigin.y = this->array[i].y;
				}
				if(this->array[i].z < this->boundingBoxOrigin.z){
					this->boundingBoxOrigin.z = this->array[i].z;
				}
			}
			this->centerOfGravity /= this->array.size();
			float width = 0;
			float height = 0;
			float depth = 0;
			for(int i=1;i < this->array.size(); i++){
				if (glm::length(this->centerOfGravity - this->array[i]) > this->radius){
					this->radius = glm::length(this->centerOfGravity - this->array[i]);
				}
				if (this->array[i].x - this->boundingBoxOrigin.x > width){
					width = this->array[i].x - this->boundingBoxOrigin.x;
				}
				if (this->array[i].y - this->boundingBoxOrigin.y > height){
					height = this->array[i].y - this->boundingBoxOrigin.y;
				}
				if (this->array[i].z - this->boundingBoxOrigin.z > width){
					depth = this->array[i].z - this->boundingBoxOrigin.z;
				}
			}
			this->boundingBoxWidth.x = width;
			this->boundingBoxHeight.y = height;
			this->boundingBoxDepth.z = depth;
		}

		glm::vec3 getBoundingBoxOrigin(){
			glm::vec4 origin = glm::vec4(this->boundingBoxOrigin, 1.f);
			return glm::vec3(this->getMatrix() * origin);
		}

		glm::vec3 getBoundingBoxWidth(){
			glm::vec4 width = glm::vec4(this->boundingBoxWidth, 0.f);
			return glm::vec3(this->getMatrix() * width);
		}

		glm::vec3 getBoundingBoxHeight(){
			glm::vec4 height = glm::vec4(this->boundingBoxHeight, 0.f);
			return glm::vec3(this->getMatrix() * height);
		}

		glm::vec3 getBoundingBoxDepth(){
			glm::vec4 depth = glm::vec4(this->boundingBoxDepth, 0.f);
			return glm::vec3(this->getMatrix() * depth);
		}

		glm::vec3 getCenterOfGravity(){
			glm::vec4 center = glm::vec4(this->centerOfGravity, 1.f);
			return glm::vec3(this->getMatrix() * center);
		}

		float getRadius(){
			return this->radius;
		}

		bool collidesWith(ObjectFromArray *other){
			if (glm::length(other->getCenterOfGravity() - this->getCenterOfGravity()) < this->radius){
				return true;
				// glm::vec4 firstPlane = glm::vec4(other->getBoundingBoxWidth(), -(other->getBoundingBoxWidth().x * other->getBoundingBoxOrigin().x + other->getBoundingBoxWidth().y * other->getBoundingBoxOrigin().y + other->getBoundingBoxWidth().z * other->getBoundingBoxOrigin().z));
			}
			return false;
		}

		void prepareBuffers(){
			glBindBuffer( GL_ARRAY_BUFFER, coordBuffer );
			glBufferData( GL_ARRAY_BUFFER, this->array.size() * sizeof(glm::vec3), &this->array[0], GL_STATIC_DRAW );
			
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
			glEnableVertexAttribArray(0);
		}

		void drawFunc(){
			glDrawArrays(GL_TRIANGLES, 0, this->array.size());
		}

		void display(){
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->getMatrix()));
			this->prepareBuffers();
			this->drawFunc();
			// glutPostRedisplay();
		}
};

class ObjectFromArrayWithNormals: public ObjectFromArray
{
	protected:
		std::vector<glm::vec3> normals;
	public:
		ObjectFromArrayWithNormals(){}

		ObjectFromArrayWithNormals(std::vector<glm::vec3> array,std::vector<glm::vec3> normals){
			this->array = array;
			this->normals = normals;
		}

		ObjectFromArrayWithNormals(const char * path){
			std::vector<glm::vec3> out_vertices;
			std::vector<glm::vec2> out_uvs;
			std::vector<glm::vec3> out_normals;
			loadOBJ(path, out_vertices, out_uvs, out_normals);
			this->array = out_vertices;
			this->normals = out_normals;
		}
		void prepareBuffers(){
			ObjectFromArray::prepareBuffers();
			glBindBuffer( GL_ARRAY_BUFFER, vNormalBuffer );
			glBufferData( GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW );
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
			glEnableVertexAttribArray( 1 );
		}

		void display(){
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->getMatrix()) );
			this->prepareBuffers();
			this->drawFunc();
			// glutPostRedisplay();
		}
};


class ObjectFromArrayWithNormalsAndTexture: public ObjectFromArray
{
	protected:
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		unsigned char *tex_data;
		int tex_width;
		int tex_height;
		GLuint textureID;
		glm::vec3 ambient = glm::vec3(0.8f, 0.8f, 0.2f);
		glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 specular = glm::vec3(0.7f, 0.7f, 0.7f);
		float shininess = 32.0f;
		glm::vec3 radPoint = glm::vec3(0.f);
	public:
		ObjectFromArrayWithNormalsAndTexture(){}

		ObjectFromArrayWithNormalsAndTexture(std::vector<glm::vec3> array,std::vector<glm::vec3> normals){
			this->array = array;
			this->normals = normals;
			this->setBoundingCircle();
		}

		ObjectFromArrayWithNormalsAndTexture(const char * objPath, const char * texPath, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shine){
			std::vector<glm::vec3> out_vertices;
			std::vector<glm::vec2> out_uvs;
			std::vector<glm::vec3> out_normals;
			loadOBJ(objPath, out_vertices, out_uvs, out_normals);
			this->array = out_vertices;
			this->normals = out_normals;
			this->uvs = out_uvs;
			this->tex_data = SOIL_load_image(texPath, &this->tex_width, &this->tex_height, 0, SOIL_LOAD_RGBA);
			glGenTextures(1, &this->textureID);
			glBindTexture(GL_TEXTURE_2D, this->textureID);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->tex_width, this->tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->tex_data);

			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			this->shininess = shine;
			this->setBoundingCircle();
		}

		ObjectFromArrayWithNormalsAndTexture(const char * objPath, const char * texPath){
			std::vector<glm::vec3> out_vertices;
			std::vector<glm::vec2> out_uvs;
			std::vector<glm::vec3> out_normals;
			loadOBJ(objPath, out_vertices, out_uvs, out_normals);
			this->array = out_vertices;



			this->normals = out_normals;
			this->uvs = out_uvs;
			this->tex_data = SOIL_load_image(texPath, &this->tex_width, &this->tex_height, 0, SOIL_LOAD_RGBA);
			glGenTextures(1, &this->textureID);
			glBindTexture(GL_TEXTURE_2D, this->textureID);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->tex_width, this->tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->tex_data);

			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			this->setBoundingCircle();
		}
		
		void loadTexture(){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->textureID);
			glUniform1i(glGetUniformLocation(currentProgram, "texture"), 0);
		}

		void prepareBuffers(){
			ObjectFromArray::prepareBuffers();
			glBindBuffer( GL_ARRAY_BUFFER, vNormalBuffer );
			glBufferData( GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW );
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
			glEnableVertexAttribArray( 1 );

			glBindBuffer( GL_ARRAY_BUFFER, vBufferUV );
			glBufferData( GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW );
			glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, NULL );
			glEnableVertexAttribArray( 2 );
		}

		void setBoundingCircle(){
			this->centerOfGravity = glm::vec3(0.f);
			for (int i=0;i < this->array.size();i++){
				this->centerOfGravity += this->array[i];
			}
			this->centerOfGravity /= (float)this->array.size();
			float radius = 0.f;
			for (int i=0;i < this->array.size();i++){
				glm::vec3 thisPoint = this->array[i];
				float lng = glm::length(this->centerOfGravity - thisPoint);
				if (lng > radius){
					radius = lng;
					this->radPoint = thisPoint;
				}
			}
		}

		glm::vec3 getCircleCenter(){
			return glm::vec3(this->getMatrix() * glm::vec4(this->centerOfGravity,1.f));
		}

		glm::vec3 getCircleRad(){
			return glm::vec3(this->getMatrix() * glm::vec4(this->radPoint,1.f));
		}

		void display(){
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->getMatrix()));
			glUniform3fv(glGetUniformLocation(currentProgram, "materialAmbient"), 1, &this->ambient[0]);
			glUniform3fv(glGetUniformLocation(currentProgram, "materialDiffuse"), 1, &this->diffuse[0]);
			glUniform3fv(glGetUniformLocation(currentProgram, "materialSpecular"), 1, &this->specular[0]);
			glUniform1f(glGetUniformLocation(currentProgram, "materialShine"), this->shininess);
			this->prepareBuffers();
			this->loadTexture();
			this->drawFunc();
		}
};

class HUD : public ObjectFromArray {
	private:
		std::vector<glm::vec2> uvs;
		unsigned char *tex_data;
		int tex_width;
		int tex_height;
		GLuint textureID;
		int width = 1024;
		int height = 768;
	
	public:
		HUD(const char * texPath){
			this->tex_data = SOIL_load_image(texPath, &this->tex_width, &this->tex_height, 0, SOIL_LOAD_RGBA);
			glGenTextures(1, &this->textureID);
			glBindTexture(GL_TEXTURE_2D, this->textureID);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->tex_width, this->tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->tex_data);

			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			this->array.push_back(glm::vec3(0.f,1.f * (float)this->height,0.3));
			this->array.push_back(glm::vec3(1.f* (float)this->width, 1.f* (float)this->height, 0.3));
			this->array.push_back(glm::vec3(0.f, 0.f, 0.3));
			this->array.push_back(glm::vec3(1.f* (float)this->width, 0.f, 0.3));

			this->uvs.push_back(glm::vec2(1.f,0.f));
			this->uvs.push_back(glm::vec2(0.f,1.f));
			this->uvs.push_back(glm::vec2(0.f,0.f));
			this->uvs.push_back(glm::vec2(1.f,1.f));
		}

		void prepareBuffers(){
			ObjectFromArray::prepareBuffers();
			glBindBuffer( GL_ARRAY_BUFFER, vBufferUV );
			glBufferData( GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW );
			glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, NULL );
			glEnableVertexAttribArray( 2 );
		}

		void loadTexture(){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->textureID);
			glUniform1i(glGetUniformLocation(currentProgram, "texture"), 0);
		}

		void display(){
			glm::mat4x4 orthProj = glm::ortho(0,this->width / 2, 0, this->height / 2, -1,1);
			glUniformMatrix4fv( projMatrixLocation, 1, GL_FALSE, glm::value_ptr(orthProj));
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4x4(1.f)));
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4x4(1.f)));
			this->prepareBuffers();
			this->loadTexture();
			this->drawFunc();
		}

};

// class AlignedWithCam: public ObjectFromArrayWithNormalsAndTexture{
// 	Camera cam;

// 	AlignedWithCam(){

// 	}
// }

class CGround: public ObjectFromArrayWithNormalsAndTexture
{
public:

	int Number_of_triangles = 0;
	std::vector<CTriangle> triangles;

	CGround(const char * objPath, const char * texPath): ObjectFromArrayWithNormalsAndTexture(objPath, texPath){
		glm::vec3 p1,p2,p3;

		std::vector<glm::vec3>::iterator it = this->array.begin();
		while (it != this->array.end())
		{
			p1 = *it++;
			p2 = *it++;
			p3 = *it++;
			// uwaga ! zamiana wspolrzednych, tak aby z byl wysokoscia
			triangles.push_back(CTriangle(glm::vec3(p1.x,p1.z,p1.y), glm::vec3(p2.x,p2.z,p2.y), glm::vec3(p3.x,p3.z,p3.y)));

		}

		Number_of_triangles = triangles.size();
		printf("Created CGround of %d triangles.\n", Number_of_triangles);
	}

	// obliczenie wysokosci
	glm::vec3 getNormal(glm::vec2 point){
		CTriangle *collTriangle = NULL;


		std::vector<CTriangle>::iterator it;
		for (it = triangles.begin(); it != triangles.end(); it++)
		{
			if ((*it).isInside(point))
			{
				collTriangle = &(*it);
				return glm::normalize(collTriangle->getNormal());
			}
		}
		return glm::vec3(0,1.f,0);
	}
	float getAltitute(glm::vec2 point)
	{

		// obliczenie listy trojkatow nad ktorymi jestesmy
		CTriangle *collTriangle = NULL;


		std::vector<CTriangle>::iterator it;
		for (it = triangles.begin(); it != triangles.end(); it++)
		{
			if ((*it).isInside(point))
			{
				collTriangle = &(*it);
				break;
			}
		}

		// jesli zostal znaleziony
		if (collTriangle)
		{
			return collTriangle->altitude(point);
		}
		else
		{
			return 0.0;
		}
	}

};

class ObjectFromArrayWithNormalsAndTextureInstanced: public ObjectFromArrayWithNormalsAndTexture{
	protected:
		std::vector<glm::vec3> instanceMatrices;

	public:
		ObjectFromArrayWithNormalsAndTextureInstanced(const char * objPath, const char * texPath, std::vector<glm::vec3> instanceMatrices):ObjectFromArrayWithNormalsAndTexture(objPath, texPath){
			this->instanceMatrices.assign(instanceMatrices.begin(), instanceMatrices.end());
		}

		void assignNew(std::vector<glm::vec3> instanceMatrices){
			this->instanceMatrices.assign(instanceMatrices.begin(), instanceMatrices.end());
		}

		void drawFunc(){
			glDrawArraysInstanced(GL_TRIANGLES, 0, this->array.size(), this->instanceMatrices.size());
		}

		void prepareBuffers(){
			ObjectFromArrayWithNormalsAndTexture::prepareBuffers();
			glBindBuffer(GL_ARRAY_BUFFER, vInstances);
			glBufferData(GL_ARRAY_BUFFER, this->instanceMatrices.size() * sizeof(glm::vec3), &this->instanceMatrices[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnableVertexAttribArray(3);
			glBindBuffer(GL_ARRAY_BUFFER, vInstances);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glVertexAttribDivisor(3, 1); 
		}

		void display(){
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->getMatrix()));
			// glUniform3fv(glGetUniformLocation(currentProgram, "materialAmbient"), 1, &this->ambient[0]);
			// glUniform3fv(glGetUniformLocation(currentProgram, "materialDiffuse"), 1, &this->diffuse[0]);
			// glUniform3fv(glGetUniformLocation(currentProgram, "materialSpecular"), 1, &this->specular[0]);
			// glUniform1f(glGetUniformLocation(currentProgram, "materialShine"), this->shininess);
			this->prepareBuffers();
			this->loadTexture();
			this->drawFunc();
		}
};

class Oponent{
	protected:
		GameObject renderer;
		glm::vec2 position;
		int life = 4;
	public:
		Oponent(GameObject& renderer){
			this->renderer = renderer;
		}
		void display(){
			this->renderer.setPosition(this->position.x, this->renderer.getPosition().y, this->position.y);
			this->renderer.display();
		}
		void moveRandomly(float amount){
			srand(time(NULL));
			float rand1 = (((float)rand() / (float)RAND_MAX) - 0.5) * 2.f;
			srand(time(NULL));
			float rand2 = (((float)rand() / (float)RAND_MAX) - 0.5) * 2.f;
			this->position += glm::vec2(rand1, rand2)*amount;
		}
		void moveTowards(glm::vec2 point, float amount){
			this->position += glm::normalize(point - this->position) * amount;
		}
		GameObject getRenderer(){
			return this->renderer;
		}
		void setPosition(float x, float z){
			this->position = glm::vec2(x, z);
		}
		glm::vec2 getPosition(){
			return this->position;
		}

		bool alive(){
			return this->life > 0;
		}

		int getLife(){
			return this->life;
		}

		bool hit(){
			this->life--;
			return this->life > 0;
		}
};

class ObjectFromIndexArray: public ObjectFromArray
{
	protected:
		std::vector<int> indexArray;
	public:
		ObjectFromIndexArray(){}

		ObjectFromIndexArray(std::vector<glm::vec3> array, std::vector<int> indexArray){
			this->array = array;
			this->indexArray = indexArray;
		}

		void drawFunc(){
			glDrawElements( GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, NULL );
		}

		void prepareBuffers(){
			ObjectFromArray::prepareBuffers();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vIndexBuffer );
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexArray.size() * sizeof(int), &this->indexArray[0], GL_STATIC_DRAW );
		}
};
#endif

