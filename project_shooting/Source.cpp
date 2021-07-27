#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gl/glut.h>
#include<GL/glut.h>
#include <math.h>
#include <fstream>
#define PI 3.14159
#define GAME_SCREEN 0			
#define MENU_SCREEN 1			
#define MAX_STONES  100      
#define MAX_STONE_TYPES 3
#define stoneRotationSpeed 20
#define SPACESHIP_SPEED 20
int stoneTranslationSpeed = 5;
GLint m_viewport[4];
int x, y;
int i;
int randomStoneIndices[100];
int index;
int Score = 0;
int alienLife = 100;
int GameLvl = 1;
int k = 0;
float mouseX, mouseY;				//Cursor coordinates;
float LaserAngle = 0, stoneAngle = 0, lineWidth = 1;
float xOne = 0, yOne = 0;				//Spaceship coordinates
float xStone[100], yStone[100];
float xHealthBarStart = 1200;				//Health bar starting coodinate
GLint stoneAlive[100];		//check to see if stone is killed
bool mButtonPressed = false, startGame = false, gameOver = false;
bool startScreen = true, nextScreen = false, previousScreen = false;
bool gameQuit = false, instructionsGame = false, optionsGame = false, st = false;

GLfloat a[][2] = { 0,-50, 70,-50, 70,70, -70,70 };
GLfloat AlienBody[][2] = { {0,0}, {1,9}, {0,12}, {-15,10}, {-16,0},{0,0} };
char highScore[100], ch;
void display();
void StoneGenerate();
void displayRasterText(float x, float y, float z, const char* stringToDisplay) {
	int length;
	glRasterPos3f(x, y, z);
	length = strlen(stringToDisplay);

	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, stringToDisplay[i]);
	}
}
void displayRasterText1(float x, float y, float z, const char* stringToDisplay) {
	int length;
	glRasterPos3f(x, y, z);
	length = strlen(stringToDisplay);
	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, stringToDisplay[i]);
	}
}
void SetDisplayMode(int modeToDisplay) {
	switch (modeToDisplay) {
	case GAME_SCREEN: glClearColor(0, 0, 0, 1); break;
	case MENU_SCREEN: glClearColor(153.0 / 255, 204.0 / 255, 1, 1); break;
	}
}
void initializeStoneArray() {
	for (int i = 0; i < MAX_STONES; i++) {
		randomStoneIndices[i] = rand() % MAX_STONE_TYPES;
		stoneAlive[i] = true;
	}

	xStone[0] = -(200 * MAX_STONES) - 600;             //START LINE for stone appearance

	for (int i = 0; i < MAX_STONES; i++) {				//ramdom appearance yIndex for each stone
		yStone[i] = rand() % 600;
		if (int(yStone[i]) % 2)
			yStone[i] *= -1;
		xStone[i + 1] = xStone[i] + 200;				//xIndex of stone aligned with 200 units gap
	}
}
void circle(int x, int y)
{
	float th;
	glColor3f(0, 0, 1);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		th = i * (3.1416 / 180);
		glVertex2f(x + 13 * cos(th), y + 13 * sin(th));
	}

	glEnd();
}

void DrawAlienBody()
{
	glColor3f(0, 1, 0);				//BODY color
	glBegin(GL_POLYGON);
	for (i = 0; i <= 5; i++)
		glVertex2fv(AlienBody[i]);
	glEnd();
	glColor3f(0, 0, 0);			//BODY Outline
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= 5; i++)
		glVertex2fv(AlienBody[i]);
	glEnd();
}
void DrawAlienFace()
{
	circle(-8, 24);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glLineWidth(5);
	glVertex2f(-14, 19);
	glVertex2f(-2, 19);
	glEnd();

}

void DrawAlienEyes()
{

	glColor3f(0, 1, 1);
	glPushMatrix();
	glRotated(-5, 0, 0, 1);
	glTranslated(-7, 27.5, 0);      //Left eye
	glScalef(2.5, 4, 0);
	glutSolidSphere(1, 20, 30);
	glPopMatrix();
	glPushMatrix();
	glRotated(-1, 0, 0, 1);
	glTranslated(-13, 27.5, 0);							//Right eye	
	glScalef(2.5, 4, 0);
	glutSolidSphere(1, 100, 100);
	glPopMatrix();
}
void DrawAlien()
{
	DrawAlienBody();
	DrawAlienFace();
	DrawAlienEyes();
}
void DrawSpaceshipBody() //DONE
{
	glColor3f(1, 0, 0);				//BASE
	glPushMatrix();
	glScalef(70, 20, 1);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();
	glPushMatrix();							//LIGHTS
	glScalef(3, 3, 1);
	glTranslated(-20, 0, 0);			//1
	glColor3f(0, 1, 0);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//2
	glColor3f(0, 0, 1);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//3
	glColor3f(0, 1, 0);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//4				
	glColor3f(0, 0, 1);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//5
	glColor3f(0, 1, 0);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//6			
	glColor3f(0, 0, 1);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//7
	glColor3f(0, 1, 0);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//8				
	glColor3f(0, 0, 1);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//9
	glColor3f(0, 1, 0);
	glutSolidSphere(1, 1000, 1000);
	glPopMatrix();
}
void DrawSteeringWheel()
{
	glPushMatrix();
	glLineWidth(3);
	glColor3f(0, 0., 0);
	glScalef(7, 4, 1);
	glTranslated(-1.9, 5.5, 0);
	glutWireSphere(1, 8, 8);
	glPopMatrix();
}
void DrawSpaceshipDoom()
{
	glColor4f(0.7, 1, 1, 0.0011);
	glPushMatrix();
	glTranslated(0, 30, 0);
	glScalef(35, 50, 1);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();
}
void DrawSpaceShipLazer() {

	glColor3f(1, 0, 0);
	glPushMatrix();
	glBegin(GL_POLYGON);           //Lazer stem
	glVertex2f(-55, 10);
	glVertex2f(-55, 30);
	glVertex2f(-50, 30);
	glVertex2f(-50, 10);
	glEnd();

	float xMid = 0, yMid = 0;
	//Mid point of the lazer horizontal 	
	xMid = (55 + 50) / 2.0;
	yMid = (25 + 35) / 2.0;

	//Rotating about the pivot point ,20
	glTranslated(-xMid, yMid, 0);
	glRotated(LaserAngle, 0, 0, 1);
	glTranslated(xMid, -yMid, 0);

	//find mid point of top of lazer stem
	float midPoint = -(55 + 50) / 2.0;

	glBegin(GL_POLYGON);           //Lazer horizontal stem
	glVertex2f(midPoint + 10, 25);
	glVertex2f(midPoint + 10, 35);
	glVertex2f(midPoint - 10, 35);
	glVertex2f(midPoint - 10, 25);
	glEnd();

	glPopMatrix();
}
void DrawLazerBeam() {

	float xMid = -(55 + 50) / 2.0;
	float yMid = (25 + 35) / 2.0;

	float mouseXEnd = -((-mouseX) + xOne);
	float mouseYEnd = -((-mouseY) + yOne);
	glLineWidth(5);   //----Laser beam width

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(xMid, yMid);
	glVertex2f(mouseXEnd, mouseYEnd);
	glEnd();
	glLineWidth(1);
}
void DrawStone(int StoneIndex)
{
	glPushMatrix();
	glLoadIdentity();
	switch (StoneIndex)                           //CHANGE INDEX VALUE FOR DIFFERENT STONE VARIETY;
	{
	case 0:

		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glColor3f(0.4f, 0.0f, 0.0f);
		glScalef(35, 35, 1);
		glutSolidSphere(1, 9, 50);
		break;

	case 1:
		glColor3f(1.0f, 0.8f, 0.8f);
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(40, 30, 1);
		glutSolidSphere(1, 9, 50);
		break;

	case 2:
		glColor3f(0.2f, 0.2f, 0.0f);
		glTranslated(xStone[index], yStone[index], 0);
		glRotatef(stoneAngle, 0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60, 25, 1);
		glutSolidSphere(1, 9, 50);
		break;
	}
	glPopMatrix();
}
bool checkIfSpaceShipIsSafe() {
	for (int i = 0; i < MAX_STONES; i++) {
		if (stoneAlive[i] & ((xOne >= (xStone[i] / 2 - 70) && xOne <= (xStone[i] / 2 + 70) && yOne >= (yStone[i] / 2 - 18) && yOne <= (yStone[i] / 2 + 53)) || (yOne <= (yStone[i] / 2 - 20) && yOne >= (yStone[i] / 2 - 90) && xOne >= (xStone[i] / 2 - 40) && xOne <= (xStone[i] / 2 + 40))))
		{
			stoneAlive[i] = 0;
			return false;
		}
	}
	return true;
}
void SpaceshipCreate() {
	glPushMatrix();
	glTranslated(xOne, yOne, 0);
	if (!checkIfSpaceShipIsSafe() && alienLife) {
		alienLife -= 10;
		xHealthBarStart -= 240; //changes
	}
	DrawSpaceshipDoom();
	glPushMatrix();
	glTranslated(4, 19, 0);
	DrawAlien();
	glPopMatrix();
	DrawSteeringWheel();
	DrawSpaceshipBody();
	DrawSpaceShipLazer();
	if (mButtonPressed) {
		DrawLazerBeam();
	}
	glEnd();
	glPopMatrix();
}
void DisplayHealthBar() {

	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(-xHealthBarStart, 700);
	glVertex2f(1200, 700);
	glVertex2f(1200, 670);
	glVertex2f(-xHealthBarStart, 670);
	glEnd();
	char temp[40];
	glColor3f(153.0 / 255, 204.0 / 255, 1);
	sprintf_s(temp, "SCORE = %d", Score);
	displayRasterText(-1100, 600, 0.4, temp);
	sprintf_s(temp, "  LIFE = %d", alienLife);
	displayRasterText(800, 600, 0.4, temp);
	sprintf_s(temp, "  LEVEL : %d", GameLvl);
	displayRasterText(-100, 600, 0.4, temp);
	//glColor3f(1, 0, 0);
}
void startScreenDisplay()
{
	glLineWidth(50);
	SetDisplayMode(MENU_SCREEN);

	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);               //Border
	glVertex3f(-750, -500, 0.5);
	glVertex3f(-750, 550, 0.5);
	glVertex3f(750, 550, 0.5);
	glVertex3f(750, -500, 0.5);
	glEnd();

	glLineWidth(1);

	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//START GAME PLOYGON
	glVertex3f(-200, 300, 0.5);
	glVertex3f(-200, 400, 0.5);
	glVertex3f(200, 400, 0.5);
	glVertex3f(200, 300, 0.5);
	glEnd();

	glBegin(GL_POLYGON);				//INSTRUCTIONS POLYGON
	glVertex3f(-200, 50, 0.5);
	glVertex3f(-200, 150, 0.5);
	glVertex3f(200, 150, 0.5);
	glVertex3f(200, 50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
	glVertex3f(-200, -200, 0.5);
	glVertex3f(-200, -100, 0.5);
	glVertex3f(200, -100, 0.5);
	glVertex3f(200, -200, 0.5);
	glEnd();
	glColor3f(0, 0, 0);
	displayRasterText1(-420, -680, 0, "Developed By: Parneet Singh     Dipanshu Golan     Satyam Kapoor");
	if (mouseX >= -100 && mouseX <= 100 && mouseY >= 150 && mouseY <= 200) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			startGame = true;
			gameOver = false;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);

	displayRasterText(-100, 340, 0.4, "Start Game");

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= 30 && mouseY <= 80) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			instructionsGame = true;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);
	displayRasterText(-120, 80, 0.4, "Instructions");

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= -90 && mouseY <= -40) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			gameQuit = true;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);
	displayRasterText(-100, -170, 0.4, "    Quit");

}
void GameScreenDisplay()
{
	SetDisplayMode(GAME_SCREEN);
	DisplayHealthBar();
	glScalef(2, 2, 0);
	if (alienLife) {
		if ((Score % 10 == 0 || st == true) && Score >= 10) {
			glColor3f(224.0 / 255, 224.0 / 255, 224.0 / 255);
			int j = 0;
			st = true;
			for (int i = 0; i < 15; i++) {
				glBegin(GL_POLYGON);
				glVertex2f(-1200, 700 + j + k);
				glVertex2f(-1200, 700 + j + 20 + k);
				glVertex2f(1200, 700 + j + 20 + k);
				glVertex2f(1200, 700 + j + k);
				glEnd();
				j = j + 40;
			}

			k = k - 10;
			if (k <= -2000) {
				st = false;
				k = 0;
			}
		}
		SpaceshipCreate();
	}
	else {
		gameOver = true;
		instructionsGame = false;
		startScreen = false;
	}

	StoneGenerate();

}

void GameOverScreen()
{
	SetDisplayMode(MENU_SCREEN);
	glColor3f(0, 0, 0);
	glLineWidth(50);
	glBegin(GL_LINE_LOOP);               //Border
	glVertex3f(-650, -500, 0.5);
	glVertex3f(-650, 520, 0.5);
	glVertex3f(650, 520, 0.5);
	glVertex3f(650, -500, 0.5);
	glEnd();

	glLineWidth(1);
	stoneTranslationSpeed = 5;
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);				//GAME OVER
	glVertex3f(-550, 810, 0.5);
	glVertex3f(-550, 610, 0.5);
	glVertex3f(550, 610, 0.5);
	glVertex3f(550, 810, 0.5);
	glEnd();

	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//RESTART POLYGON
	glVertex3f(-200, 50, 0.5);
	glVertex3f(-200, 150, 0.5);
	glVertex3f(200, 150, 0.5);
	glVertex3f(200, 50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
	glVertex3f(-200, -200, 0.5);
	glVertex3f(-200, -100, 0.5);
	glVertex3f(200, -100, 0.5);
	glVertex3f(200, -200, 0.5);
	glEnd();

	glColor3f(0, 0, 0);
	displayRasterText(-300, 640, 0.4, "     G A M E    O V E R ! ! !");
	glColor3f(0, 0, 0);
	char temp[40];
	sprintf_s(temp, "Score : %d", Score);
	displayRasterText(-100, 340, 0.4, temp);

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= 25 && mouseY <= 75) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {                                                       //Reset game default values
			startGame = true;
			gameOver = false;
			mButtonPressed = false;
			initializeStoneArray();
			alienLife = 100;
			xHealthBarStart = 1200;
			Score = 0;
			GameLvl = 1;
			GameScreenDisplay();
		}
	}
	else
		glColor3f(0, 0, 0);
	displayRasterText(-70, 80, 0.4, "Restart");

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= -100 && mouseY <= -50) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			exit(0);
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);
	displayRasterText(-100, -170, 0.4, "    Quit");

}
void StoneGenerate() {

	if (xStone[0] >= 1200) {      //If the last screen hits the end of screen then go to Nxt lvl
		GameLvl++;
		stoneTranslationSpeed += 3;
		Score += 50;
		initializeStoneArray();
		GameScreenDisplay();
	}

	for (int i = 0; i < MAX_STONES; i++) {
		index = i;

		if (mouseX <= (xStone[i] / 2 + 30) && mouseX >= (xStone[i] / 2 - 30) && mouseY >= (yStone[i] / 2 - 30) && mouseY <= (yStone[i] / 2 + 30) && mButtonPressed) {
			if (stoneAlive[i]) {   // IF ALIVE KILL STONE
				stoneAlive[i] = 0;
				Score++;
				if (Score % 2 == 0) {
					stoneTranslationSpeed += 1;			//<--------------Rate of increase of game speed
				}
			}
		}
		xStone[i] += stoneTranslationSpeed;
		if (stoneAlive[i])             //stone alive
			DrawStone(randomStoneIndices[i]);
	}
	stoneAngle += stoneRotationSpeed;
	if (stoneAngle > 360) stoneAngle = 0;
}
void backButton() {
	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//START GAME PLOYGON
	glVertex3f(-1010, -500, 0.5);
	glVertex3f(-1010, -570, 0.5);
	glVertex3f(-890, -570, 0.5);
	glVertex3f(-890, -500, 0.5);
	glEnd();
	if (mouseX <= -450 && mouseX >= -500 && mouseY >= -275 && mouseY <= -250) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			mButtonPressed = false;
			instructionsGame = false;
			startScreenDisplay();
		}
	}
	else glColor3f(0, 0, 0);

	displayRasterText(-1000, -550, 0, "Back");
}
void InstructionsScreenDisplay()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetDisplayMode(MENU_SCREEN);
	glColor3f(0, 0, 0);
	displayRasterText(-900, 400, 0.4, "-> Key 'w' to move up.");
	displayRasterText(-900, 300, 0.4, "-> Key 's' to move down.");
	displayRasterText(-900, 200, 0.4, "-> Key 'd' to move right.");
	displayRasterText(-900, 100, 0.4, "-> Key 'a' to move left.");
	displayRasterText(-900, 0.0, 0.4, "-> Left mouse click to shoot laser");
	displayRasterText(-900, -200, 0.4, "-> You Get 1 point for shooting each objet and 50 points for completing each lvl ");
	displayRasterText(-900, -270, 0.4, "-> The Objective is to score maximum points");
	backButton();
	if (previousScreen)
		nextScreen = false, previousScreen = false; //as set by backButton()
}
void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 1200, 700);

	if (startGame && !gameOver)
		GameScreenDisplay();

	else if (instructionsGame)
		InstructionsScreenDisplay();

	else if (gameOver)
		GameOverScreen();

	//Make spaceship bigger
	else if (startScreen) {

		startScreenDisplay();
		if (gameQuit || startGame || optionsGame || instructionsGame) {
			//startScreen = false;

			if (startGame) {
				SetDisplayMode(GAME_SCREEN);
				startScreen = false;

			}
			else if (gameQuit)
				exit(0);

		}
		else if (instructionsGame) {
			SetDisplayMode(GAME_SCREEN);
			InstructionsScreenDisplay();
		}
	}
	glScalef(1 / 2, 1 / 2, 0);
	glFlush();
	glLoadIdentity();
	glutSwapBuffers();
}
void somethingMovedRecalculateLaserAngle() {

	float mouseXForTan = (-50 - mouseX) + xOne;
	float mouseYForTan = (35 - mouseY) + yOne;
	float LaserAngleInRadian = atan(mouseYForTan / mouseXForTan);
	LaserAngle = (180 / PI) * LaserAngleInRadian;
}
void keys(unsigned char key, int x, int y)
{
	if (key == 'd') xOne += SPACESHIP_SPEED;
	if (key == 'a') xOne -= SPACESHIP_SPEED;
	if (key == 'w') { yOne += SPACESHIP_SPEED; }
	if (key == 's') { yOne -= SPACESHIP_SPEED; }
	if (key == 'd' || key == 'a' || key == 'w' || key == 's')
		somethingMovedRecalculateLaserAngle();

	display();

}
void myinit()
{
	glClearColor(0.5, 0.5, 0.5, 0);
	glColor3f(1.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1200, 1200, -700, 700);                   //<-----CHANGE THIS TO GET EXTRA SPACE
	glMatrixMode(GL_MODELVIEW);
}
void passiveMotionFunc(int x, int y) {

	//when mouse not clicked
	mouseX = float(x) / (m_viewport[2] / 1200.0) - 600.0;  //converting screen resolution to ortho 2d spec
	mouseY = -(float(y) / (m_viewport[3] / 700.0) - 350.0);

	//Do calculations to find value of LaserAngle
	somethingMovedRecalculateLaserAngle();
	display();
}
void mouseClick(int buttonPressed, int state, int x, int y) {

	if (buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mButtonPressed = true;
	else
		mButtonPressed = false;
	display();
}

void idleCallBack() {			//when no mouse or keybord pressed
	display();
}
void main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitWindowSize(1200, 700);
	glutInitWindowPosition(90, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Shooting Game");
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutPassiveMotionFunc(passiveMotionFunc);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutIdleFunc(idleCallBack);
	glutMouseFunc(mouseClick);
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	myinit();
	SetDisplayMode(GAME_SCREEN);
	initializeStoneArray();
	glutMainLoop();
}