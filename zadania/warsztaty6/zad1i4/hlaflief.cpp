
// #include <vector>
// #include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../../../shader_stuff.h"
#include "../../../text.h"
#include <iostream>
#include "../../../obj_loader.hpp"
#include "../../../texture_loader.hpp"
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <shlwapi.h>
#include <mmsystem.h>
#include <string>
#include <bits/stdc++.h> 
#define NO_OF_TRAWA 10
#define NO_OF_SPARKS 40
std::vector<glm::vec3> trawy;
std::vector<glm::vec3> drzewa0;
std::vector<glm::vec3> sparks;
std::vector<glm::vec3> sparkDirs;
// ---------------------------------------
// Identyfikatory obiektow
Camera *cam;
Viewport *viewport;
Program *program;
Program *programInstanced;
Light * light;
SpecularLight * specLight;

CGround *blenderFile;
ObjectFromArrayWithNormalsAndTexture *pistol;
ObjectFromArrayWithNormalsAndTexture *klucz;
ObjectFromArrayWithNormalsAndTextureInstanced *flash;
ObjectFromArrayWithNormalsAndTextureInstanced *trawa;
ObjectFromArrayWithNormalsAndTextureInstanced *drzewa;
vector<Oponent*> zombies;
std::vector<Oponent*>::iterator oponentIterator;
ObjectFromArrayWithNormalsAndTexture *zombie;
HUD *hud;

// ---------------------------------------
// Macierze przeksztalcen i rzutowania

glm::mat4x4 Matrix_proj;	// projection matrix
glm::mat4x4 Matrix_mv;		// modelview matrix
glm::mat4x4 Matrix_proj_mv;	// projection*modelview matrix

glm::vec3 lightPos;
glm::vec3 shotDir;
glm::vec3 lightDir;

glm::vec3 camPos;
time_t czas=0;
time_t czas0=0;
time_t czasDiff = 0.0001;
time_t loop_time = 0;
time_t frameTime = 0;
int frameNumber = 0;
int currWidth;
int currHeight;
char txt[255];
float life = 100;
int killed = 0;
time_t newZombies = 0;
char path[MAX_PATH]; 


// ---------------------------------------
void DisplayScene()
{	
	glBindVertexArray(vertexArray);
	if (loop_time >= 35000){
		loop_time = 0;
		// mciSendString(TEXT("seek hell to start"),NULL,0,NULL);
		// mciSendString(TEXT("play hell"),NULL,0,NULL);
	}
	specLight->display();
	light->display();
	clear();
	camPos = cam->getPosition();
	// cout<<glm::to_string(lightPos)<<endl;
	// cout<<gunShot<<endl;

	cam->moveForward(_scene_translate_y);
	cam->moveRight(_scene_translate_x);
	glm::vec3 cam_pos = cam->getPosition();
	cam->setPosition(cam_pos.x, blenderFile->getAltitute(glm::vec2(cam_pos.x, cam_pos.z)) + 2.f, cam_pos.z);

	cam->rotateAroundUp(_scene_rotate_y * 10.f);
	cam->rotateAroundRight(_scene_rotate_x * 10.f);

	pistol->alignFrontWith(cam->getRight());
	pistol->alignRightWith(cam->getFront());
	pistol->setPosition(cam->getPosition());
	pistol->moveRight(2.f);
	pistol->moveUp(-1.f);
	pistol->moveForward(0.5f);
	pistol->moveRight(-gunShot);
	pistol->moveUp(-reload);
	lightPos = pistol->getPosition();
	lightPos += pistol->getUp() * 0.5f;
	lightDir = -pistol->getRight();
	// lightPos += shotDir * ((1 - gunShot) * 100.f);

	glUseProgram(0);
	programInstanced->useThis();

	glUniform3fv(glGetUniformLocation(currentProgram, "lightPos"), 1, &lightPos[0]);
	glUniform3fv(glGetUniformLocation(currentProgram, "lightDir"), 1, &lightDir[0]);
	glUniform1f(glGetUniformLocation(currentProgram, "lightStrength"), 1.f);
	glUniform3fv(glGetUniformLocation(currentProgram, "cameraPos"), 1, &camPos[0]);

	viewport->display();
	cam->display();
	trawa->display();
	drzewa->display();
	// if (gunShot > 0.f){
	// 	for (int i=0;i<NO_OF_SPARKS;i++){
	// 		cout<<glm::to_string(sparks[i])<<"/"<<glm::to_string(lightPos)<<endl;
	// 	}
	// }
	if (gunShot > 0.f){
		flash->assignNew(sparks);
		flash->display();
	}

	program->useThis();

	glUniform3fv(glGetUniformLocation(currentProgram, "lightPos"), 1, &lightPos[0]);
	glUniform3fv(glGetUniformLocation(currentProgram, "lightDir"), 1, &lightDir[0]);
	glUniform1f(glGetUniformLocation(currentProgram, "lightStrength"), 1.f);
	glUniform3fv(glGetUniformLocation(currentProgram, "cameraPos"), 1, &camPos[0]);

	viewport->display();
	cam->display();

	pistol->alignFrontWith(cam->getRight());
	pistol->alignRightWith(cam->getFront());
	pistol->setPosition(cam->getPosition());
	pistol->moveRight(2.f);
	pistol->moveUp(-1.f);
	pistol->moveForward(0.5f);
	pistol->moveRight(-gunShot);
	pistol->moveUp(-reload);
	lightPos = pistol->getPosition();
	lightPos += pistol->getUp() * 0.5f;
	// flash->alignFrontWith(pistol->getRight());
	// flash->alignRightWith(pistol->getFront());
	// flash->setPosition(lightPos);
	// lightPos += shotDir * ((1 - gunShot) * 100.f);
	if (gunShot > 0.f){
		gunShot -= (czasDiff) * 0.001;
		for (int i=0;i<NO_OF_SPARKS;i++){
			sparks[i] = (lightPos + glm::normalize(sparkDirs[i] + pistol->getRight() * 3.f) * (1.f - gunShot) * 30.f) + pistol->getRight();
		}
	}
	if (gunShot < 0.f){
		gunShot = 0.f;
	}
	if (reload > 0.f){
		reload -= (czasDiff) * 0.001;
	}
	if (reload < 0.f){
		reloadBool = false;
		reload = 0.f;
	}
	czasDiff = czas - czas0;
	czas0 = czas;
	blenderFile->display();
	pistol->display();

	glm::vec2 mean = glm::vec2(0.f);
	glm::mat4x4 projMat;
	glm::mat4x4 viewMat;
	if (gunShotBool){
		shotDir = pistol->getRight();
		projMat = viewport->getProjMatrix();
		viewMat = cam->getMatrix();
	}
	bool hurtPlayed = false;
	bool deathPlayed = false;
	for (oponentIterator=zombies.begin();oponentIterator!=zombies.end();oponentIterator++){
		zombie->setPosition((*oponentIterator)->getPosition().x,blenderFile->getAltitute((*oponentIterator)->getPosition()), (*oponentIterator)->getPosition().y);
		
		if(!(*oponentIterator)->alive()){
			zombie->translate(0,0.5,0);
			zombie->setRotation(M_PI / 2.f,0.f,0.f);
			zombie->display();
			continue;
		}

		glm::vec3 zombieCenter = zombie->getPosition();
		
		float dist = glm::length((*oponentIterator)->getPosition() - glm::vec2(cam_pos.x,cam_pos.z));
		if (dist < 15.f){
			if (gunShotBool && !reloadBool){
				glm::vec4 zombieOnScreen = projMat * viewMat * glm::vec4(zombieCenter + glm::vec3(0.f,0.5,0.f), 1.f);
				zombieOnScreen /= zombieOnScreen.w;
				if (glm::length(glm::vec2(zombieOnScreen)) < 0.1){
					if (!(*oponentIterator)->hit()){
						ammo += 3;
						killed++;
						if (!deathPlayed){
							deathPlayed = true;
							mciSendString(TEXT("seek death to start"),NULL,100,NULL);
							mciSendString(TEXT("play death"),NULL,0,NULL);
						}
					}
					if (!hurtPlayed){
						hurtPlayed = true;
						mciSendString(TEXT("seek hurt to start"),NULL,100,NULL);
						mciSendString(TEXT("play hurt"),NULL,0,NULL);
					}
				}
			}
		}
		if (dist < 10.f){
			mean = (mean + (*oponentIterator)->getPosition()) / 2.f;
			(*oponentIterator)->moveTowards(glm::vec2(cam_pos.x, cam_pos.z), (czasDiff / 1000.f));
		}

		if (dist < 1.f && life > 0.f){
			life -= czasDiff * 0.001;
		}
		if (life < 0.f){
			exit(0);
			break;
		}
		zombie->alignFrontWith(-glm::normalize(cam_pos - zombie->getPosition()));
		zombie->alignLocalVectorWith(glm::vec3(0.f,1.f,0.f), glm::vec3(0.f,1.f,0.f));
		zombie->display();
	}
	gunShotBool = false;

	// for (int i = 0; i<NO_OF_TRAWA; i++){
	// 	trawa->setPosition(trawy[i].x, blenderFile->getAltitute(glm::vec2(trawy[i].x, trawy[i].z)), trawy[i].z);
	// 	trawa->display();
	// }

	DrawText8x16(currWidth / 2,currHeight / 2, "x", glm::vec4(1.f,1.f,1.f, 1.f));
	DrawText8x16(currWidth-50,currHeight-50, std::to_string(life), glm::vec4(1.f, life / 100.f,life / 100.f, 1.f));
	DrawText8x16(50,currHeight-50, std::to_string(killed), glm::vec4(1.f, 1.f,1.f,1.f));
	DrawText8x16(50,50, std::to_string(magazine) + "/" + std::to_string(ammo), glm::vec4(1.f, (float)ammo / 50.f, (float)ammo / 50.f,1.f));
	for (oponentIterator=zombies.begin();oponentIterator!=zombies.end();oponentIterator++){
		float dist = glm::length((*oponentIterator)->getPosition() - glm::vec2(cam_pos.x,cam_pos.z));
			projMat = viewport->getProjMatrix();
			viewMat = cam->getMatrix();
		// if (dist < 15.f){
			// zombie->setPosition((*oponentIterator)->getPosition().x,blenderFile->getAltitute((*oponentIterator)->getPosition()), (*oponentIterator)->getPosition().y);
			// glm::vec3 zombieCenter = zombie->getCircleCenter();
			glm::vec4 zombieOnScreen0 = projMat * viewMat * glm::vec4((*oponentIterator)->getPosition().x,blenderFile->getAltitute((*oponentIterator)->getPosition()), (*oponentIterator)->getPosition().y, 1.f);
			zombieOnScreen0 /= zombieOnScreen0.w;
			// printf("overlay");
			// DrawText8x16(currWidth /2,currHeight / 2, "std::to_string((*oponentIterator)->getLife())", glm::vec4(1.f,1.f,1.f,1.f));
		
			DrawText8x16((int)((float)currWidth * (zombieOnScreen0.x + 1.f) / 2.f),(int)((float)currHeight * (zombieOnScreen0.y + 1.f) / 2.f) + 20, std::to_string((*oponentIterator)->getLife()), glm::vec4(1.f, (float)(*oponentIterator)->getLife() / 4.f,(float)(*oponentIterator)->getLife() / 4.f,(15.f - dist)/15.f));
		// }
	}
	glutSwapBuffers();
}

// ---------------------------------------
void Reshape( int width, int height )
{	
	currWidth = width;
	currHeight = height;

	viewport->setScaling(width, height, 0);
}

void Animation (int keyframe)
{	
	glutPostRedisplay();
	glutTimerFunc(1, Animation, 0);
	newZombies += 1;
	// if (newZombies > 100){
	// 	srand(time(NULL));
	// 	printf("new zombies");
	// 	for (int i=0;i<10;i++){
	// 	zombies.push_back(new Oponent(*zombie));
	// 	zombies.back()->setPosition(((float)rand() / (float)RAND_MAX)*10.f, ((float)rand() / (float)RAND_MAX)*10.f);
	// }
	// newZombies = 0;
	// }
	czas += 50;
	loop_time += 50;
}

char* stringToChar(string s){
	char* char_array = (char*)malloc((s.length() + 1) * sizeof(char));
	strcpy(char_array, s.c_str()); 
	return char_array;
}

// ---------------------------------------------------
void Initialize()
{	
	glBufferData(GL_UNIFORM_BUFFER, sizeof(specLights), NULL, GL_DYNAMIC_DRAW);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lights), NULL, GL_DYNAMIC_DRAW);
	specLight = new SpecularLight();
	light = new Light();
	cam = new Camera();
	cam->setPosition(0.f,0.5f,0.f);
	glEnable(GL_BLEND);
	glClearColor(0.f, 0.f, 0.f, 1.0f );

	program = new Program("v.glsl", "f.glsl");

	programInstanced = new Program("instancedVertex.glsl", "fInstanced.glsl");

	viewport = new Viewport(glm::vec2(0.f,0.f), glm::vec2(glutGet(GLUT_WINDOW_WIDTH),glutGet(GLUT_WINDOW_HEIGHT)));

	currWidth = glutGet(GLUT_WINDOW_WIDTH);
	currHeight = glutGet(GLUT_WINDOW_HEIGHT);

	int tex_width;
	int tex_height;
	unsigned char *tex_data;

	blenderFile = new CGround("terrain.obj", "grass.bmp");
	pistol = new ObjectFromArrayWithNormalsAndTexture("pistol.obj", "metal.bmp", glm::vec3(0.5f), glm::vec3(0.7f), glm::vec3(1.f), 10.f);
	klucz = new ObjectFromArrayWithNormalsAndTexture("klucz.obj", "wood.jpg", glm::vec3(0.5f), glm::vec3(0.7f), glm::vec3(0.5f, 0.8f, 0.f), 10.f);
	float x;
	float z;
	float y;
	srand(time(NULL));
	for (int i=0;i<NO_OF_TRAWA;i++){
		x = (((float)rand() / (float)RAND_MAX)*100.f);
		z = (((float)rand() / (float)RAND_MAX)*100.f);
		y = blenderFile->getAltitute(glm::vec2(x, z));
		trawy.push_back(glm::vec3(x, y, z));
	}
	for (int i=0;i<NO_OF_TRAWA;i++){
		x = (((float)rand() / (float)RAND_MAX)*-100.f);
		z = (((float)rand() / (float)RAND_MAX)*100.f);
		y = blenderFile->getAltitute(glm::vec2(x, z));
		trawy.push_back(glm::vec3(x, y, z));
	}
	for (int i=0;i<NO_OF_TRAWA;i++){
		x = (((float)rand() / (float)RAND_MAX)*-100.f);
		z = (((float)rand() / (float)RAND_MAX)*-100.f);
		y = blenderFile->getAltitute(glm::vec2(x, z));
		trawy.push_back(glm::vec3(x, y, z));
	}
	for (int i=0;i<NO_OF_TRAWA;i++){
		x = (((float)rand() / (float)RAND_MAX)*100.f);
		z = (((float)rand() / (float)RAND_MAX)*-100.f);
		y = blenderFile->getAltitute(glm::vec2(x, z));
		trawy.push_back(glm::vec3(x, y, z));
	}

	for (int i=0;i<NO_OF_TRAWA;i++){
		x = (((float)rand() / (float)RAND_MAX)*100.f);
		z = (((float)rand() / (float)RAND_MAX)*100.f);
		y = blenderFile->getAltitute(glm::vec2(x, z));
		drzewa0.push_back(glm::vec3(x, y, z));
	}
	for (int i=0;i<NO_OF_TRAWA;i++){
		x = (((float)rand() / (float)RAND_MAX)*-100.f);
		z = (((float)rand() / (float)RAND_MAX)*100.f);
		y = blenderFile->getAltitute(glm::vec2(x, z));
		drzewa0.push_back(glm::vec3(x, y, z));
	}
	for (int i=0;i<NO_OF_TRAWA;i++){
		x = (((float)rand() / (float)RAND_MAX)*-100.f);
		z = (((float)rand() / (float)RAND_MAX)*-100.f);
		y = blenderFile->getAltitute(glm::vec2(x, z));
		drzewa0.push_back(glm::vec3(x, y, z));
	}
	for (int i=0;i<NO_OF_TRAWA;i++){
		x = (((float)rand() / (float)RAND_MAX)*100.f);
		z = (((float)rand() / (float)RAND_MAX)*-100.f);
		y = blenderFile->getAltitute(glm::vec2(x, z));
		drzewa0.push_back(glm::vec3(x, y, z));
	}

	for (int i=0;i<NO_OF_SPARKS;i++){
		sparkDirs.push_back(glm::normalize(glm::vec3((float)rand() - ((float)RAND_MAX / 2.f), (float)rand() - ((float)RAND_MAX / 2.f), (float)rand() - ((float)RAND_MAX / 2.f))));
		sparks.push_back(glm::vec3(0.f));
	}

	flash = new ObjectFromArrayWithNormalsAndTextureInstanced("spark.obj", "flash.jpg", sparks);
	trawa = new ObjectFromArrayWithNormalsAndTextureInstanced("drzewo.obj", "drzewo.jpg", trawy);
	drzewa = new ObjectFromArrayWithNormalsAndTextureInstanced("drzewo0.obj", "drzewo0.jpg", drzewa0);
	// trawa->setRotation(M_PI,0.f,0.f);
	// trawa->setScaling(1.f,3.f,1.f);
	zombie = new ObjectFromArrayWithNormalsAndTexture("zombie.obj", "miesko.jpg");
	zombie->setScaling(0.03,0.03,0.03);

	for (int i=0;i<10;i++){
		zombies.push_back(new Oponent(*zombie));
		zombies.back()->setPosition(((float)rand() / (float)RAND_MAX)*10.f, ((float)rand() / (float)RAND_MAX)*10.f);
	}
	InitializeBuffers();
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// winHeight = glutGet(GLUT_WINDOW_HEIGHT);
	// winWidth = glutGet(GLUT_WINDOW_WIDTH);
	string pth(path);
	mciSendString(TEXT(stringToChar("open \"" + pth + "/hell.wav\" type waveaudio alias hell")), NULL, 0, NULL);
	mciSendString(TEXT(stringToChar("open \"" + pth + "/gunshot.wav\" type waveaudio alias gun")), NULL, 0, NULL);
	mciSendString(TEXT(stringToChar("open \"" + pth + "/zombie_hurt.wav\" type waveaudio alias hurt")), NULL, 0, NULL);
	mciSendString(TEXT(stringToChar("open \"" + pth + "/death.wav\" type waveaudio alias death")), NULL, 0, NULL);
	mciSendString(TEXT(stringToChar("open \"" + pth + "/noammo.wav\" type waveaudio alias noammo")), NULL, 0, NULL);
	mciSendString(TEXT(stringToChar("open \"" + pth + "/reload.wav\" type waveaudio alias reload")), NULL, 0, NULL);
	// mciSendString(TEXT("seek hell to start"),NULL,100,NULL);
	// mciSendString(TEXT("play hell"),NULL,0,NULL);
	InitDrawText();
	// mciSendString(TEXT("play C:/Users/jakub/projects/helloworld/hell.wav"), 0, 0, 0);
}

// ---------------------------------------------------
int main( int argc, char *argv[] )
{	
	// GLUT
	GetModuleFileNameA(NULL, path, sizeof(path));
	PathRemoveFileSpecA(path);
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitWindowSize( 1024, 768 );
	glutCreateWindow( "Sterowanie strzalklami, klik strzelanie, prawy przeladowanie" );

	// GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		printf("GLEW Error\n");
		exit(1);
	}

	// OpenGL
	if( !GLEW_VERSION_3_2 )
	{
		printf("Brak OpenGL 3.2!\n");
		exit(1);
	}

	Initialize();
	glutTimerFunc(1, Animation, 0);
	glutDisplayFunc( DisplayScene );
	glutReshapeFunc( Reshape );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutMouseWheelFunc( MouseWheel );
	glutKeyboardFunc( Keyboard );
	glutSpecialFunc( SpecialKeys );
	glutSpecialUpFunc(SpecialKeysUp);
	glutPassiveMotionFunc( PassiveMotion );

	glutMainLoop();

	delete &program;
	delete &blenderFile;
	delete &pistol;
	delete &cam;
	delete &viewport;
	glDeleteBuffers( 1, &vertexArray );
	glDeleteBuffers( 1, &coordBuffer );
	glDeleteVertexArrays( 1, &vIndexBuffer );
	mciSendString(TEXT("close hell"), NULL, 0, NULL);
	mciSendString(TEXT("close gun"), NULL, 0, NULL);
	mciSendString(TEXT("close hurt"), NULL, 0, NULL);
	mciSendString(TEXT("close death"), NULL, 0, NULL);
	mciSendString(TEXT("close noammo"), NULL, 0, NULL);
	mciSendString(TEXT("close reload"), NULL, 0, NULL);
	DeleteDrawText();
	return 0;
}
