#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>

int WIDTH = 1280;
int HEIGHT = 720;
int HeroX = 0;
int HeroZ = 0;
float rotAng =0.0;
bool LookLeft = false;		// Determine which direction the hero will be facing
bool LookRight = false;
bool LookForward = true;
bool LookBackward = false;
float rotAngleWep = 0.0; // used for rotating the weapon while moving.
float WepX = 0;
float WepZ = 0;
bool WepFire = false;
int bulletDirection = 0;
//Enemies movement variables
int Enemy1BackForward = 0;		//Flag for Choose if the Enemy moving forward or backward
int Enemy2BackForward = 0;
int Enemy3BackForward = 0;
int Enemy4BackForward = 0;
int Enemy5BackForward = 0;
int Enemy6BackForward = 0;

float MoveEnemy1X = -35;
float MoveEnemy2X = -38;
float MoveEnemy3X = -35;
float MoveEnemy4X = -25;
float MoveEnemy5X = 25;
float MoveEnemy6X = -27;

float MoveEnemy1Z = -35;
float MoveEnemy2Z = 39;
float MoveEnemy3Z = 34;
float MoveEnemy4Z = -35;
float MoveEnemy5Z = -35;
float MoveEnemy6Z = 21;

int RotationAngleEnemi1 = 0;
int RotationAngleEnemi2 =90;
int RotationAngleEnemi3 = 90;
int RotationAngleEnemi4 = 0;
int RotationAngleEnemi5 = 0;
int RotationAngleEnemi6 = 90;

//Eenemy and Hero collision variables
int Collided = 0;

int countEnemy1 = 0;
int countEnemy2 = 0;
int countEnemy3 = 0;
int countEnemy4 = 0;
int countEnemy5 = 0;
int countEnemy6 = 0;

int HitEnemy1 = 0;
int HitEnemy2 = 0;
int HitEnemy3 = 0;
int HitEnemy4 = 0;
int HitEnemy5 = 0;
int HitEnemy6 = 0;

float wallX = -40.0;
float stoneY = 5.0;

int level2start = 0;
int level2 = 0;

int movetowardsplayer = 1;
int dropbomb = 0;
//Second field varibales 
float DragonX = 0;
float DragonZ = -80;
int dragonMovement = 0;
float DragonRoationAngle = 90;

bool lastDir = true;   // Boolean to check the direction for "S" and "D" keys
bool powerup1 = false;
bool powerup2 = false; 
bool donePower1 = false; // Powers are activated or not yet
bool donePower2 = false; 
int timer1Count = 0;
bool freeze = false;
bool cameraType = false;
float SphereX = 30;
float SphereY = 30;
float SphereZ = 30;
bool health1 = true;
bool health2 = true;
bool health3 = false;
bool hit = true;
bool delay = false;
GLuint tex;
char title[] = "3D Model Loader Sample";
void Anim();
// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = ((GLdouble)WIDTH / (GLdouble)HEIGHT);
GLdouble zNear = 0.1;
GLdouble zFar = 900;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};


Vector Eye(0, 80, -50);
Vector At(0, -120, -100);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_boss;
Model_3DS model_princess;
Model_3DS model_weapon;
Model_3DS model_dragon;
Model_3DS model_enemy1;
Model_3DS model_enemy2;
Model_3DS model_enemy3;
Model_3DS model_enemy4;
Model_3DS model_baby;
Model_3DS model_cage;
Model_3DS model_stone;
Model_3DS model_hg;
Model_3DS model_potion;
Model_3DS model_apple;
// Textures
GLTexture tex_ground;
GLTexture tex_bricks;
//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	//gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-40, 0, -40);
	glTexCoord2f(5, 0);
	glVertex3f(40, 0, -40);
	glTexCoord2f(5, 5);
	glVertex3f(40, 0, 40);
	glTexCoord2f(0, 5);
	glVertex3f(-40, 0, 40);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}



void RenderGround2()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_bricks.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad2 normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-40, 0, -120);
	glTexCoord2f(5, 0);
	glVertex3f(40, 0, -120);
	glTexCoord2f(5, 5);
	glVertex3f(40, 0, -40);
	glTexCoord2f(0, 5);
	glVertex3f(-40, 0, -40);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void drawWall(double thickness) {
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}




//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
	// Draw Ground
	RenderGround();
	RenderGround2();
	// Draw Tree Model
	glPushMatrix();
	glTranslatef(20, 0, -10);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();




	glPushMatrix();
	glTranslated(HeroX, 0, HeroZ);
	if (LookLeft) {
		glRotated(90, 0, 1, 0);
	}
	if (LookRight) {
		glRotated(-90, 0, 1, 0);
	}
	if (LookForward) {
		glRotated(0, 0, 1, 0);
	}
	if (LookBackward) {
		glRotated(180, 0, 1, 0);
	}
	model_boss.Draw();
	glPopMatrix();
	if (WepFire) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslated(WepX, 1, WepZ);
		glScaled(1, 0.6, 1);
		glRotated(rotAngleWep, 0, 1, 0);
		model_weapon.Draw();
		glPopMatrix();
	}
	glColor3f(1, 1, 1);
	//sky box
	glPushMatrix();

	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 400, 400, 400);
	gluDeleteQuadric(qobj);


	glPopMatrix();

	//walls for first field
	glPushMatrix();
	glTranslated(-40, 0, -40);
	glScaled(1.0, 7.0, 80.0);
	drawWall(0.7);
	glPopMatrix();

	glPushMatrix();
	glTranslated(40, 0, -40);
	glScaled(1.0, 7.0, 80.0);
	drawWall(0.7);
	glPopMatrix();

	glPushMatrix();
	glTranslated(wallX, 0, -40);
	glRotated(90, 0.0, 1.0, 0.0);
	glScaled(1.0, 7.0, 80.0);
	drawWall(0.7);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-40, 0, 40);
	glRotated(90, 0.0, 1.0, 0.0);
	glScaled(1.0, 7.0, 80.0);
	drawWall(0.7);
	glPopMatrix();
	//-------------------------------------

	//walls for 2nd field
	glPushMatrix();
	glTranslated(-40, 0, -120);
	glScaled(1.0, 7.0, 80.0);
	drawWall(0.7);
	glPopMatrix();

	glPushMatrix();
	glTranslated(40, 0, -120);
	glScaled(1.0, 7.0, 80.0);
	drawWall(0.7);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-40, 0, -120);
	glRotated(90, 0.0, 1.0, 0.0);
	glScaled(1.0, 7.0, 80.0);
	drawWall(0.7);
	glPopMatrix();

	//glPushMatrix();
	//glTranslated(-40, 0, -40);
	//glRotated(90, 0.0, 1.0, 0.0);
	//glScaled(1.0, 7.0, 80.0);
	//drawWall(0.7);
	//glPopMatrix();
	//-------------------------------------


		//wall of trees

	glPushMatrix();
	glTranslated(-2, 0, -5);

	glPushMatrix();
	glTranslatef(40, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(35, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPopMatrix();

	//--------------------------------



		//wall of trees vol.2

	glPushMatrix();
	glTranslated(-35, 0, 20);
	glRotatef(90.f, 0, 1, 0);


	glPushMatrix();
	glTranslatef(40, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(35, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPopMatrix();

	//--------------------------------



		//wall of trees vol.2 round 2

	glPushMatrix();
	glTranslated(-30, 0, 20);
	glRotatef(90.f, 0, 1, 0);


	glPushMatrix();
	glTranslatef(40, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(35, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 30);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	glPopMatrix();

	//--------------------------------


	//draw potion
	if (!powerup1) {
		glPushMatrix();
		glTranslatef(-22, 0, -20);
		glScaled(0.6, 0.6, 0.6);
		glRotated(rotAng, 0, 1, 0);
		model_potion.Draw();
		glPopMatrix();
	}


		//draw hg
	if (!powerup2) {
		glPushMatrix();
		glTranslatef(22, 2, -20);
		glScaled(0.008, 0.008, 0.008);
		glRotated(rotAng, 0, 0, 1);
		model_hg.Draw();
		glPopMatrix();
	}
	



	//stones for testing

	//fo2 3al ymeeen
	glPushMatrix();
	glTranslatef(2, 5, -21);
	glScaled(0.4, 0.8, 0.4);
	model_stone.Draw();
	glPopMatrix();

	//fo2 3al shemal
	glPushMatrix();
	glTranslatef(-7, 5, -21);
	glScaled(0.4, 0.8, 0.4);
	model_stone.Draw();
	glPopMatrix();


	//ta7t 3al shemal
	glPushMatrix();
	glTranslatef(-7, 5, 12);
	glScaled(0.4, 0.8, 0.4);
	model_stone.Draw();
	glPopMatrix();


	//========================

	//draw stones

	glPushMatrix();
	glTranslatef(38, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(33, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(28, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(23, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(18, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(13, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-12, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-17, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-22, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-27, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-32, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-37, stoneY, -41);
	//glScaled(0.2, 0.2, 0.2);
	model_stone.Draw();
	glPopMatrix();





	//----------------------------------

	//draw cage

	glPushMatrix();
	glTranslatef(25, 0, -74);
	glScaled(0.2, 0.2, 0.2);
	model_cage.Draw();
	glPopMatrix();
	//--------------------

	//draw princess
	glPushMatrix();
	glTranslated(29, 5, -78);
	model_princess.Draw();
	glPopMatrix();

	//----------



		//draw baby

	glPushMatrix();
	glTranslatef(-5, 15, -82);
	glScaled(0.2, 0.2, 0.2);
	glRotated(rotAng, 0, 1, 0);

	model_baby.Draw();
	glPopMatrix();

	

	//Draw enemies
	if (HitEnemy1==0) {
		glPushMatrix();
		glTranslated(MoveEnemy1X, 0, MoveEnemy1Z);
		glRotated(RotationAngleEnemi1, 0, 1, 0);
		glScaled(0.07, 0.07, 0.07);
		model_enemy1.Draw();
		glPopMatrix();
	}
	

	if (HitEnemy2 == 0) {
		glPushMatrix();
		glTranslated(MoveEnemy2X, 0, MoveEnemy2Z);
		glRotated(RotationAngleEnemi2, 0, 1, 0);
		glScaled(0.07, 0.07, 0.07);
		model_enemy1.Draw();
		glPopMatrix();
	}
	
	if (HitEnemy3 == 0) {
		glPushMatrix();
		glTranslated(MoveEnemy3X, 0, MoveEnemy3Z);
		glRotated(RotationAngleEnemi3, 0, 1, 0);
		glScaled(0.07, 0.07, 0.07);
		model_enemy1.Draw();
		glPopMatrix();
	}

	if (HitEnemy4 == 0) {
		glPushMatrix();
		glTranslated(MoveEnemy4X, 0, MoveEnemy4Z);
		glRotated(RotationAngleEnemi4, 0, 1, 0);
		glScaled(0.07, 0.07, 0.07);
		model_enemy1.Draw();
		glPopMatrix();
	}
	
	if (HitEnemy5 == 0) {
		glPushMatrix();
		glTranslated(MoveEnemy5X, 0, MoveEnemy5Z);
		glRotated(RotationAngleEnemi5, 0, 1, 0);
		glScaled(0.07, 0.07, 0.07);
		model_enemy1.Draw();
		glPopMatrix();
	}
	
	if (HitEnemy6 == 0) {
		glPushMatrix();
		glTranslated(MoveEnemy6X, 0, MoveEnemy6Z);
		glRotated(RotationAngleEnemi6, 0, 1, 0);
		glScaled(0.07, 0.07, 0.07);
		model_enemy1.Draw();
		glPopMatrix();
	}
	if (health1) {
		glPushMatrix();
		glTranslated(At.x, At.y + 1.25, At.z);
		glScaled(0.005, 0.005, 0.005);
		model_apple.Draw();
		glPopMatrix();
	}
	if (health2) {
		glPushMatrix();
		glTranslated(At.x + 1, At.y + 1.25, At.z);
		glScaled(0.005, 0.005, 0.005);
		model_apple.Draw();
		glPopMatrix();
	}
	if (powerup1 && health3) {
		glPushMatrix();
		glTranslated(At.x + 0.5, At.y + 1.25, At.z);
		glScaled(0.005, 0.005, 0.005);
		model_apple.Draw();
		glPopMatrix();
	}
















	//draw dragon
	glPushMatrix();
	glTranslatef(DragonX, 0, DragonZ);
	glScaled(4.0, 4.0, 4.0);
	//glRotatef(90.f, 1, 0, 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glRotated(DragonRoationAngle, 0, 1, 0);
	model_dragon.Draw();
	glPopMatrix();

	if (dropbomb == 1) {
		glPushMatrix();
		glTranslatef(SphereX, SphereY, SphereZ);
		glScaled(0.2, 0.2, 0.2);
		glColor3f(1.0, 0.0, 0.0);
		glutSolidSphere(10, 15, 15);
		glPopMatrix();
	}

	glutSwapBuffers();
}
// End of display function
//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'd':
		LookLeft = true;
		LookRight = false;
		LookForward = false;
		LookBackward = false;
		if (!cameraType) {
			if (lastDir) {
				LookLeft = true;
				LookRight = false;
				LookForward = false;
				LookBackward = false;
				if (HeroX != 38 && (HeroZ <= 23 || HeroZ >= 27)) {
					HeroX += 1;
					Eye.x = HeroX + 1;

				}
				if (HeroX != 38 && HeroX < 5) {
					HeroX += 1;
					Eye.x = HeroX + 1;
				}
				Eye.z = HeroZ + 20;
				Eye.y = 30;
				At.x = HeroX + 1;
				At.y = 3;
				At.z = HeroZ;
			}
			else {
				LookLeft = false;
				LookRight = true;
				LookForward = false;
				LookBackward = false;
				if (HeroX != -38) {
					HeroX -= 1;
					Eye.x = HeroX - 1;
				}
				Eye.z = HeroZ - 20;
				Eye.y = 30;
				At.x = HeroX + 1;
				At.y = 3;
				At.z = HeroZ;
			}
		
			//if (HeroX != 38 && (HeroZ <= -20 || HeroZ >= 13)) {
			//	HeroX += 1;
			//}

			//if (HeroX != 38 && HeroX < 5) {
			//	HeroX += 1;
			//}
			//if (HeroX != 38 && (HeroX <= -6 || HeroX >= 3)) {
			//	HeroX += 1;
			//}


		}
		else {
			if (lastDir) {
				LookLeft = true;
				LookRight = false;
				LookForward = false;
				LookBackward = false;
				if (HeroX != 38 && (HeroZ <= 23 || HeroZ >= 27)) {
					HeroX += 1;

				}
				if (HeroX != 38 && HeroX < 5) {
					HeroX += 1;
				}
				Eye.x = HeroX + 2;
				Eye.z = HeroZ;
				Eye.y = 3;
				At.x = HeroX + 20;
				At.y = 3;
				At.z = HeroZ;

			}
			else {
				LookLeft = false;
				LookRight = true;
				LookForward = false;
				LookBackward = false;
				if (HeroX != -38) {
					HeroX -= 1;
				}
				Eye.x = HeroX - 2;
				Eye.z = HeroZ;
				Eye.y = 3;
				At.x = HeroX - 20;
				At.y = 3;
				At.z = HeroZ;

			}
		}
		glLoadIdentity();
		gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, 0, 1, 0);

		break;
	case'a':
		LookLeft = false;
		LookBackward = false;
		LookForward = false;
		LookRight = true;
		if (!cameraType) {
			if (lastDir) {

				LookRight = true;
				LookLeft = false;
				LookBackward = false;
				LookForward = false;
				if (HeroX != -38) {
					Eye.z = HeroZ + 20;
					HeroX -= 1;
					Eye.x = HeroX - 1;
				}



			}
			else {
				LookRight = false;
				LookLeft = true;
				LookBackward = false;
				LookForward = false;
				if (HeroX != 38 && (HeroZ <= 23 || HeroZ >= 27)) {
					Eye.z = HeroZ - 20;
					HeroX += 1;
					Eye.x = HeroX + 1;
				}
				if (HeroX != 38 && HeroX < 5) {
					Eye.z = HeroZ - 20;
					HeroX += 1;
					Eye.x = HeroX + 1;
				}
			}
			Eye.y = 30;
			At.x = HeroX - 1;
			At.y = 3;
			At.z = HeroZ;
		}
		else {
			if (lastDir) {
				LookRight = true;
				LookLeft = false;
				LookBackward = false;
				LookForward = false;
				if (HeroX != -38) {
					Eye.z = HeroZ;
					HeroX -= 1;
					Eye.x = HeroX - 2;
					Eye.y = 2;
					At.x = HeroX - 20;
					At.z = Eye.z;
					At.y = 2;
				}
			}
			else {
				LookRight = false;
				LookLeft = true;
				LookBackward = false;
				LookForward = false;
				if (HeroX != 38 && (HeroZ <= 23 || HeroZ >= 27)) {
					Eye.z = HeroZ;
					HeroX += 1;
					Eye.x = HeroX + 2;
					Eye.y = 2;
					At.x = HeroX + 20;
					At.z = Eye.z;
					At.y = 2;
				}
				if (HeroX != 38 && HeroX < 5) {
					Eye.z = HeroZ;
					HeroX += 1;
					Eye.x = HeroX + 2;
					Eye.y = 2;
					At.x = HeroX + 20;
					At.z = Eye.z;
					At.y = 2;
				}
			}
		}
		glLoadIdentity();
		gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, 0, 1, 0);

		break;

	case 's':
		lastDir = false;
		LookForward = true;
		LookLeft = false;
		LookRight = false;
		LookBackward = false;
		if (!cameraType) {
			if (HeroZ != 38) {
				if ((HeroZ <= 20 || HeroZ >= 28)) {
					HeroZ += 1;
				}

				if (HeroX < 5) {
					HeroZ += 1;
				}
			}
			Eye.x = HeroX;
			Eye.z = HeroZ - 20;
			Eye.y = 30;
			At.x = HeroX;
			At.y = 3;
			At.z = HeroZ;
		}
		else {
			if (HeroZ != 38) {
				if ((HeroZ <= 20 || HeroZ >= 28)) {
					HeroZ += 1;
				}

				if (HeroX < 5) {
					HeroZ += 1;
				}
			}
			Eye.x = HeroX;
			Eye.y = 3;
			Eye.z = HeroZ+2;
			At.x = HeroX;
			At.y = 3;
			At.z = HeroZ+20;
		}
		glLoadIdentity();
		gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, 0, 1, 0);
		break;
	case 'w':
		lastDir = true;
		LookForward = false;;
		LookLeft = false;
		LookRight = false;
		LookBackward = true;
		if (!cameraType) {
			if (HeroZ != -38) {

				if (HeroZ == 29 && HeroX >= 7) {
					HeroZ += 1;

				}

				if (HeroZ >= 14 || HeroZ <= -23)
				{
					HeroZ -= 1;
				}

				else
					if (HeroX <= -8 || HeroX >= 3) {
						HeroZ -= 1;

					}
			}
			Eye.x = HeroX;
			Eye.y = 30;

			Eye.z = HeroZ + 20;
			At.x = HeroX;
			At.y = 3;
			At.z = HeroZ;
			glLoadIdentity();
			gluLookAt(Eye.x, 60, Eye.z, At.x, At.y, At.z, 0, 1, 0);
		}
		else {
			if (HeroZ != -338) {

				if (HeroZ == 29 && HeroX >= 7) {
					HeroZ += 1;

				}

				if (HeroZ >= 14 || HeroZ <= -23)
				{
					HeroZ -= 1;
				}

				else
					if (HeroX <= -8 || HeroX >= 3) {
						HeroZ -= 1;

					}
			}
			Eye.x = HeroX;
			Eye.z = HeroZ -2;
			Eye.y = 3;
			At.x = HeroX;
			At.y = 3;
			At.z = HeroZ - 20;

		}
		glLoadIdentity();
		gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, 0, 1, 0);
		break;
	case 'o':
		movetowardsplayer = 1 - movetowardsplayer;
		break;
	case 'p':
		level2 = 1-level2;
		break;
	case 'c':
		cameraType = !cameraType;
		break;

	case'f' :
		if (!WepFire) {
			WepFire = true;
			if (LookForward) {
				bulletDirection = 1;
			}
			if (LookBackward) {
				bulletDirection = 2;
			}
			if (LookLeft) {
				bulletDirection = 3;
			}	
			if (LookRight) {
				bulletDirection = 4;
			}
			WepX = HeroX;
			WepZ = HeroZ;
		}
		break;
	case 27:
		exit(0);
		break;
	default:
		break;

		glutPostRedisplay();
		}
	}

//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		//Eye.x += -0.1;

		Eye.z += -5;
	}
	else
	{
		//Eye.x += 0.1;

		Eye.z += 5;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	//gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}


void testTimer(int val) {
	DragonZ = (rand() % 120) * -1;
	DragonX = rand() % 40;
	dropbomb = 1 - dropbomb;
	if (dropbomb == 1) {
		SphereX = HeroX;
		SphereZ = HeroZ;
	}

	if (DragonZ > -40)
		DragonZ = -50;

	if (dragonMovement == 0)
	{
		DragonRoationAngle = 180;
		DragonX = DragonX * -1;
		dragonMovement = 1;
	}
	else
	{
		DragonRoationAngle = 90;
		dragonMovement = 0;
	}
	glutPostRedisplay();						// redraw 		
	glutTimerFunc(8000, testTimer, 0);
}
void HitTimer(int value) {
	if (delay == false) {
		delay = true;
	}
	else{ 
		hit = true;
		//delay = false;
	}
	
	glutPostRedisplay();						// redraw 		
	glutTimerFunc(1500, HitTimer, 0);
}

void timerPower(int val) {
	if (timer1Count < 2) {
		
		timer1Count++;
		glutPostRedisplay();						// redraw 		
		glutTimerFunc(4000, timerPower, 0);
	}
	else {
		donePower1 = true;
		freeze = false;
	}
	
}
//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

void Anim() {
	if (SphereY <= 1 && SphereX == HeroX && SphereZ == HeroZ) {
		if (health2 && health3 && health1 && hit) {
			health3 = !health3;
			hit = false;
			delay = false;

		}
		else if(health2  && health1 && hit){
			health2 = !health2;
			hit = false;
			delay = false;

		}
		else if(health1 && hit) {
			health1 = !health1;
			hit = false;
			delay = false;

		}
	}
	if (!health1 && !health2 && !health3) {
		exit(0);
	}
	if (level2 == 1) {
		stoneY -= 0.07;
		wallX += 0.4	;
	}
	
	//if (level2start == 1) {
	//}
	
	if (dropbomb==1) {
		SphereY -= 0.3;
	}
	if (dropbomb == 0) {
		SphereY = 30;
	}
	if (SphereY <= -10) {
		SphereY = 30;
	}
	if (movetowardsplayer == 1) {
		if (DragonX != HeroX) {
			if (DragonX > HeroX) {
				DragonX -= 0.05;
			}
			else if (DragonX < HeroX) {
				DragonX += 0.05;

			}

		}
		if (DragonZ != HeroZ) {
			if (DragonZ > HeroZ) {
				DragonZ -= 0.06;
			}
			else if (DragonZ < HeroZ) {
				DragonZ += 0.06;

			}

		}
	}
	// Powerup conditionals
	if (HeroX == 22 && HeroZ == -20 && !powerup2) {
		powerup2 = true;
		freeze = true;
		timerPower(0);
	}
	if (HeroX == -22 && HeroZ == -20 && !powerup1) {
		powerup1 = true;
		health3 = true;
	}


	// Motions functions for Zombies
	//motion gunctions for Enemy 1

	rotAng += 5;
	if (!freeze) {
			if (Enemy1BackForward == 0)
				MoveEnemy1Z += 0.1;
			else
				MoveEnemy1Z -= 0.1;

			if (MoveEnemy1Z >= 10) {
				Enemy1BackForward = 1;
				MoveEnemy1X = -31;
				RotationAngleEnemi1 = -180;
			}
			else if (MoveEnemy1Z <= -35) {

				Enemy1BackForward = 0;
				MoveEnemy1X = -35;
				RotationAngleEnemi1 = 0;
			}
		
		//motion functions for Enemy 2
			if (Enemy2BackForward == 0)
				MoveEnemy2X += 0.1;
			else
				MoveEnemy2X -= 0.1;

			if (MoveEnemy2X >= 35) {
				Enemy2BackForward = 1;
				MoveEnemy2Z = 35;
				RotationAngleEnemi2 = -90;
			}
			else if (MoveEnemy2X <= -35) {
				MoveEnemy2Z = 39;
				Enemy2BackForward = 0;
				RotationAngleEnemi2 = 90;
			}

		//motion functions for Enemy 3
			if (Enemy3BackForward == 0)
				MoveEnemy3X += 0.2;
			else
				MoveEnemy3X -= 0.2;

			if (MoveEnemy3X >= 35) {
				Enemy3BackForward = 1;
				MoveEnemy3Z = 30;
				RotationAngleEnemi3 = -90;
			}
			else if (MoveEnemy3X <= -35) {
				Enemy3BackForward = 0;
				MoveEnemy3Z = 34;
				RotationAngleEnemi3 = 90;
			}
		
		//motion gunctions for Enemy 4
			if (Enemy4BackForward == 0)
				MoveEnemy4Z += 0.3;
			else
				MoveEnemy4Z -= 0.2;

			if (MoveEnemy4Z >= 10) {
				Enemy4BackForward = 1;
				MoveEnemy4X = -21;
				RotationAngleEnemi4 = -180;
			}
			else if (MoveEnemy4Z <= -35) {
				Enemy4BackForward = 0;
				MoveEnemy4X = -25;
				RotationAngleEnemi4 = 0;
			}
		
		//motion gunctions for Enemy 5
			if (Enemy5BackForward == 0)
				MoveEnemy5Z += 0.3;
			else
				MoveEnemy5Z -= 0.2;

			if (MoveEnemy5Z >= 10) {
				Enemy5BackForward = 1;
				MoveEnemy5X = 21;
				RotationAngleEnemi5 = -180;
			}
			else if (MoveEnemy5Z <= -35) {
				Enemy5BackForward = 0;
				MoveEnemy5X = 25;
				RotationAngleEnemi5 = 0;
			}
		
		//motion gunctions for Enemy 6
			if (Enemy6BackForward == 0)
				MoveEnemy6X += 0.6;
			else
				MoveEnemy6X -= 0.4;

			if (MoveEnemy6X >= 35) {
				Enemy6BackForward = 1;
				MoveEnemy6Z = 17;
				RotationAngleEnemi6 = -90;
			}
			else if (MoveEnemy6X <= -35) {
				Enemy6BackForward = 0;
				MoveEnemy6Z = 21;
				RotationAngleEnemi6 = 90;
			}

		}
	
	//Collision between enemy and Hero
	if ((MoveEnemy1X <= HeroX + 2 && MoveEnemy1X >= HeroX - 2) && (MoveEnemy1Z <= HeroZ + 2 && MoveEnemy1Z >= HeroZ - 2)) {
		Collided = 1; //to be changed to zero at the heakth bar
		if (health3 && hit) {
			health3 = false;
			hit = false;
			delay = false;
		}
		else if (health2 && hit) {
			health2 = false;
			hit = false;
			delay = false;
		}
		else if (health1 && hit) {
			health1 = false;
			hit = false;
			delay = false;
		}
	}
	if ((MoveEnemy2X <= HeroX+2 && MoveEnemy2X >= HeroX-2) && (MoveEnemy2Z<=HeroZ+2 && MoveEnemy2Z >= HeroZ - 2)) {
		Collided = 1; //to be changed to zero at the heakth bar
		if (health3 && hit) {
			health3 = false;
			hit = false;
			delay = false;
		}
		else if (health2 && hit) {
			health2 = false;
			hit = false;
			delay = false;
		}
		else if (health1 && hit) {
			health1 = false;
			hit = false;
			delay = false;
		}
	}
	if ((MoveEnemy3X <= HeroX + 2 && MoveEnemy3X >= HeroX - 2) && (MoveEnemy3Z <= HeroZ + 2 && MoveEnemy3Z >= HeroZ - 2)) {
		Collided = 1; //to be changed to zero at the heakth bar
		if (health3 && hit) {
			health3 = false;
			hit = false;
			delay = false;
		}
		else if (health2 && hit) {
			health2 = false;
			hit = false;
			delay = false;
		}
		else if (health1 && hit) {
			health1 = false;
			hit = false;
			delay = false;
		}
	}
	if ((MoveEnemy4X <= HeroX + 2 && MoveEnemy4X >= HeroX - 2) && (MoveEnemy4Z <= HeroZ + 2 && MoveEnemy4Z >= HeroZ - 2)) {
		Collided = 1; //to be changed to zero at the heakth bar
		if (health3 && hit) {
			health3 = false;
			hit = false;
			delay = false;
		}
		else if (health2 && hit) {
			health2 = false;
			hit = false;
			delay = false;
		}
		else if (health1 && hit) {
			health1 = false;
			hit = false;
			delay = false;
		}
	}
	if ((MoveEnemy5X <= HeroX + 2 && MoveEnemy5X >= HeroX - 2) && (MoveEnemy5Z <= HeroZ + 2 && MoveEnemy5Z >= HeroZ - 2)) {
		Collided = 1; //to be changed to zero at the heakth bar
		if (health3 && hit) {
			health3 = false;
			hit = false;
			delay = false;
		}
		else if (health2 && hit) {
			health2 = false;
			hit = false;
			delay = false;
		}
		else if (health1 && hit) {
			health1 = false;
			hit = false;
			delay = false;
		}
	}
	if ((MoveEnemy6X <= HeroX + 2 && MoveEnemy6X >= HeroX - 2) && (MoveEnemy6Z <= HeroZ + 2 && MoveEnemy6Z >= HeroZ - 2)) {
		Collided = 1; //to be changed to zero at the heakth bar
		if (health3 && hit) {
			health3 = false;
			hit = false;
			delay = false;
		}
		else if (health2 && hit) {
			health2 = false;
			hit = false;
			delay = false;
		}
		else if (health1 && hit) {
			health1 = false;
			hit = false;
			delay = false;
		}
	}




	///////WEAPON///////////
	rotAngleWep += 15;
	if (rotAngleWep == 720) {
		rotAngleWep = 0;
	}
	if (WepFire) {
		if (WepX > 39 || WepX < -39 || WepZ> 39 || WepZ < -39) {
			WepFire = false;
			
		}
		else {
			if (bulletDirection == 1) {
				WepZ += 1.25;
			}
			else if (bulletDirection == 2) {
				WepZ -= 1.25;
			}
			else if (bulletDirection == 3) {
				WepX += 1.25;
			}
			else if (bulletDirection == 4) {
				WepX -= 1.25;
			}
		}
		//Bullet collisions with enemies
		if ((WepX <= MoveEnemy1X + 2.5 && WepX >= MoveEnemy1X - 2.5) && (WepZ <= MoveEnemy1Z + 2.5 && WepZ >= MoveEnemy1Z - 2.5)) {
			if (countEnemy1 >= 2) {
				HitEnemy1 = 1;
				MoveEnemy1X = 10000;
				MoveEnemy1Z = 10000;
			}
			if (countEnemy1 == 0)
				countEnemy1 = 1;
			if (countEnemy1 == 1)
				countEnemy1 = 2;
			WepFire = false;
			WepX = HeroX;
			WepZ = HeroZ;
		}

		if ((WepX <= MoveEnemy2X + 2.5 && WepX >= MoveEnemy2X - 2.5) && (WepZ <= MoveEnemy2Z + 2.5 && WepZ >= MoveEnemy2Z - 2.5)) {
			if (countEnemy2 >= 2) {
				HitEnemy2 = 1;
				MoveEnemy2X = 10000;
				MoveEnemy2Z = 10000;
			}
			if (countEnemy2 == 0)
				countEnemy2 = 1;
			if (countEnemy2 == 1)
				countEnemy2 = 2;
			WepFire = false;
			WepX = HeroX;
			WepZ = HeroZ;
		}
		if ((WepX <= MoveEnemy3X + 2.5 && WepX >= MoveEnemy3X - 2.5) && (WepZ <= MoveEnemy3Z + 2.5 && WepZ >= MoveEnemy3Z - 2.5)) {
			if (countEnemy3 >= 2) {
				HitEnemy3 = 1;
				MoveEnemy3X = 10000;
				MoveEnemy3Z = 10000;
			}
			if (countEnemy3 == 0)
				countEnemy3 = 1;
			if (countEnemy3 == 1)
				countEnemy3 = 2;
			WepFire = false;
			WepX = HeroX;
			WepZ = HeroZ;
		}
		if ((WepX <= MoveEnemy4X + 2.5 && WepX >= MoveEnemy4X - 2.5) && (WepZ <= MoveEnemy4Z + 2.5 && WepZ >= MoveEnemy4Z - 2.5)) {
			if (countEnemy4 >= 2) {
				HitEnemy4 = 1;
				MoveEnemy4X = 10000;
				MoveEnemy4Z = 10000;
			}
			if (countEnemy4 == 0)
				countEnemy4 = 1;
			if (countEnemy4 == 1)
				countEnemy4 = 2;
			WepFire = false;
			WepX = HeroX;
			WepZ = HeroZ;
		}
		if ((WepX <= MoveEnemy5X + 2.5 && WepX >= MoveEnemy5X - 2.5) && (WepZ <= MoveEnemy5Z + 2.5 && WepZ >= MoveEnemy5Z - 2.5)) {
			if (countEnemy5 >= 2) {
				HitEnemy5 = 1;
				MoveEnemy5X = 10000;
				MoveEnemy5Z = 10000;
			}
			if (countEnemy5 == 0)
				countEnemy5 = 1;
			if (countEnemy5 == 1)
				countEnemy5 = 2;
			WepFire = false;
			WepX = HeroX;
			WepZ = HeroZ;
		}
		if ((WepX <= MoveEnemy6X + 2.5 && WepX >= MoveEnemy6X - 2.5) && (WepZ <= MoveEnemy6Z + 2.5 && WepZ >= MoveEnemy6Z - 2.5)) {
			if (countEnemy6 >= 2) {
				HitEnemy6 = 1;
				MoveEnemy6X = 10000;
				MoveEnemy6Z = 10000;

			}
			if (countEnemy6 == 0)
				countEnemy6 = 1;
			if (countEnemy6 == 1)
				countEnemy6 = 2;
			WepFire = false;
			WepX = HeroX;
			WepZ = HeroZ;
		}
	}


	//printf("%f\n weapon", WepX);
	glutPostRedisplay();
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/Tree/Tree1.3ds");
	model_boss.Load("Models/Muham/Muhammer.3DS");
	model_princess.Load("Models/tron/tron.3DS");
	model_weapon.Load("Models/sword/sword/shurkin.3ds");
	model_dragon.Load("Models/blooddragon/blooddragon.3ds");
	model_enemy1.Load("Models/femalezombie/Zumbi_Female.3ds");
	model_apple.Load("Models/apple/Apple/Apple.3ds");
	//model_baby.Load("Models/baby/baby.3ds");
	model_cage.Load("Models/birdcage/Cage.3ds");
	model_stone.Load("Models/stone/stone podest 3DS.3ds");
	model_hg.Load("Models/hg/hourgalss.3ds");
	model_potion.Load("Models/Potion/Potion.3ds");


	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	tex_bricks.Load("Textures/fire.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	glutTimerFunc(100, testTimer, 0);
	glutTimerFunc(2000, HitTimer, 0);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);
	glutIdleFunc(Anim);
	glutMainLoop();
}