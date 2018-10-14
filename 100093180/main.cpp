#include <windows.h>
#include <iostream>
#include "Image_Loading/nvImage.h"
#include "Header.h"
#include "resource.h"
#include "BaseObject.h"
#include "OBB.h"
#include <ctime>
#include <sstream>
#include <math.h>

#include "freetype.h"
using namespace freetype;

font_data font;
font_data fontSmall;

/**********************************************************************************************/
/*
----TODO----
+ Different Enemies
+ Enemy Movement
+ Enemy Pathing
+ Enemy Shooting
+ Enemies drop Parts
+ Parts shop (Powerups/Upgrade Tree)
	+ Faster Ship
	+ Faster Fire Rate
	- Front/Rear Guns
	+ More Broadside Guns
	- Cursor Swivel Gun
	- Ally Ships
	+ Repair
	- Shallows Dont Slow
+ Boss Ship
- Use scrap to summon boss
- Kill Boss go to next stage
- Stage complete screen
- Difficulty Select
- Choose World Size
- Ship Select
- Minimap/Radar
- Sound
+ Wakes
*/

GLuint loadPNG(char* name)
{
	// Texture loading object
	nv::Image img;

	GLuint myTextureID;

	// Return true on success
	if(img.loadImageFromFile(name))
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glGenTextures(1, &myTextureID);
		glBindTexture(GL_TEXTURE_2D, myTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	else
		MessageBox(NULL, name, "Failed to load texture", MB_OK | MB_ICONINFORMATION);

	return myTextureID;
}

GLuint loadBackPNG(char* name)
{
	// Texture loading object
	nv::Image img;

	GLuint myTextureID;

	// Return true on success
	if (img.loadImageFromFile(name))
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glGenTextures(1, &myTextureID);
		glBindTexture(GL_TEXTURE_2D, myTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	else
		MessageBox(NULL, "Failed to load texture", "Failed to load texture", MB_OK | MB_ICONINFORMATION);

	return myTextureID;
}

void initVariables(){
	mouse_x = 0, mouse_y = 0;
	LeftPressed = false;
	pause = false;
	done = false;
	replay = true;
	ortho = 6;
	hudOrtho = 8;
	cooldown = 0;
	cooldownReset = 50;
	scrapLevel = 0; //SET 0
	firstPass = true;
	numEnemy = 50; //SET 50
	score = numEnemy;
	islandScale = 10;
	kills = 0;
	reqKills = 40; //SET 100
	limit = 5; //SET 5
	range = 1;
	enemyRange = 1;
	maxSpeed = 0.5f;
	objectLimit = 400;

	worldSizeX = 1280;
	worldSizeY = 720;

	// This stores a handle to the texture
	playerTexture = 0;
	playerAnim[10];
	EnemyTexture[11];
	BossTextures[6];
	EnemyDamageTexture[11];
	Explosiontextures[18];
	IslandPrefabs[5];
	ShallowPrefabs[6];

	topTexture = 0;
	sideTexture = 0;
	tlCorner = 0;

	shallowWater = 0;
	deepWater = 0;
	sand = 0;

	shallowDeepTop = 0;
	shallowDeepSide = 0;
	shallowDeepCorner = 0;

	shotTexture = 0;
	enemyShotTexture = 0;
	cross = 0;
	cooldownIcon = 0;
	scrapIcon = 0;
	killIcon = 0;
	scrapTexture = 0;

	speedCount = 0;
	fireRateCount = 0;
	rangeCount = 0;
	shotCount = 0;
	upgradesMax = 5;
	upgradeCost = 3;
	maxKeyCount = 20;
	enemyLevel = 1;
	playerHP = 1;
	
	ultiCooldownMax = 500; //SET 500
	ultiCooldown = ultiCooldownMax;

	collision = false;
	bossSpawn = false;
	bossDead = false;
	drawBoss = false;

	//PlaySound("sounds/water.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

}

void init()
{
	initVariables();
	glClearColor(0.0,0.0,0.0,0.0);

	font.init("arialbd.ttf", 22);
	fontSmall.init("arialbd.ttf", 12);

	playerTexture = loadPNG("textures/ships/ship_large_body.png");
	playerAnim[0] = loadPNG("textures/ships/animated_player/ship_large_body_0.png");
	playerAnim[1] = loadPNG("textures/ships/animated_player/ship_large_body_1.png");
	playerAnim[2] = loadPNG("textures/ships/animated_player/ship_large_body_2.png");
	playerAnim[3] = loadPNG("textures/ships/animated_player/ship_large_body_3.png");
	playerAnim[4] = loadPNG("textures/ships/animated_player/ship_large_body_4.png");

	playerAnim[5] = loadPNG("textures/ships/animated_player/ship_large_body_back_0.png");
	playerAnim[6] = loadPNG("textures/ships/animated_player/ship_large_body_back_1.png");
	playerAnim[7] = loadPNG("textures/ships/animated_player/ship_large_body_back_2.png");
	playerAnim[8] = loadPNG("textures/ships/animated_player/ship_large_body_back_3.png");
	playerAnim[9] = loadPNG("textures/ships/animated_player/ship_large_body_back_4.png");

	EnemyTexture[0] = loadPNG("textures/ships/animated_enemy/ship_small_body.png");
	EnemyTexture[1] = loadPNG("textures/ships/animated_enemy/ship_small_body_0.png");
	EnemyTexture[2] = loadPNG("textures/ships/animated_enemy/ship_small_body_1.png");
	EnemyTexture[3] = loadPNG("textures/ships/animated_enemy/ship_small_body_2.png");
	EnemyTexture[4] = loadPNG("textures/ships/animated_enemy/ship_small_body_3.png");
	EnemyTexture[5] = loadPNG("textures/ships/animated_enemy/ship_small_body_4.png");
	EnemyTexture[6] = loadPNG("textures/ships/animated_enemy/ship_small_body_back_0.png");
	EnemyTexture[7] = loadPNG("textures/ships/animated_enemy/ship_small_body_back_1.png");
	EnemyTexture[8] = loadPNG("textures/ships/animated_enemy/ship_small_body_back_2.png");
	EnemyTexture[9] = loadPNG("textures/ships/animated_enemy/ship_small_body_back_3.png");
	EnemyTexture[10] = loadPNG("textures/ships/animated_enemy/ship_small_body_back_4.png");

	EnemyDamageTexture[0] = loadPNG("textures/ships/animated_enemy_damaged/ship_small_body.png");
	EnemyDamageTexture[1] = loadPNG("textures/ships/animated_enemy_damaged/ship_small_body_0.png");
	EnemyDamageTexture[2] = loadPNG("textures/ships/animated_enemy_damaged/ship_small_body_1.png");
	EnemyDamageTexture[3] = loadPNG("textures/ships/animated_enemy_damaged/ship_small_body_2.png");
	EnemyDamageTexture[4] = loadPNG("textures/ships/animated_enemy_damaged/ship_small_body_3.png");
	EnemyDamageTexture[5] = loadPNG("textures/ships/animated_enemy_damaged/ship_small_body_4.png");
	EnemyDamageTexture[6] = loadPNG("textures/ships/animated_enemy_damaged/ship_small_body_back_0.png");
	EnemyDamageTexture[7] = loadPNG("textures/ships/animated_enemy_damaged/ship_small_body_back_1.png");
	EnemyDamageTexture[8] = loadPNG("textures/ships/animated_enemy_damaged/ship_small_body_back_2.png");
	EnemyDamageTexture[9] = loadPNG("textures/ships/animated_enemy_damaged/ship_small_body_back_3.png");
	EnemyDamageTexture[10] = loadPNG("textures/ships/animated_enemy_damaged/ship_small_body_back_4.png");

	BossTextures[0] = loadPNG("textures/ships/animated_boss/boss_0.png");
	BossTextures[1] = loadPNG("textures/ships/animated_boss/boss_1.png");
	BossTextures[2] = loadPNG("textures/ships/animated_boss/boss_2.png");
	BossTextures[3] = loadPNG("textures/ships/animated_boss/boss_3.png");
	BossTextures[4] = loadPNG("textures/ships/animated_boss/boss_4.png");
	BossTextures[5] = loadPNG("textures/ships/animated_boss/boss_5.png");

	cross = loadPNG("textures/cross.png");
	cooldownIcon = loadPNG("textures/cannon.png");
	scrapIcon = loadPNG("textures/spanner.png");
	shotTexture = loadBackPNG("textures/shot.png");
	ultiShot = loadBackPNG("textures/ultiShot.png");
	enemyShotTexture = loadBackPNG("textures/enemyShot.png");
	scrapTexture = loadBackPNG("textures/scrap.png");
	killIcon = loadPNG("textures/killIcon.png");

	//Water textures
	deepWater = loadBackPNG("textures/beach/deep0/straight/0/0.png");
	shallowWater = loadBackPNG("textures/beach/shallow0/straight/0/0.png");
	shallowDeepTop = loadBackPNG("textures/beach/shallow-deep0/straight/90/0.png");
	shallowDeepSide = loadBackPNG("textures/beach/shallow-deep0/straight/180/0.png");
	shallowDeepCorner = loadBackPNG("textures/beach/shallow-deep0/curve_in/180/0.png");

	//Sand Textures
	topTexture = loadBackPNG("textures/beach/beach-shallow0/straight/90/0.png");
	sideTexture = loadBackPNG("textures/beach/beach-shallow0/straight/180/0.png");
	tlCorner = loadBackPNG("textures/beach/beach-shallow0/curve_in/180/0.png");
	sand = loadBackPNG("textures/beach/beach0/straight/0/0.png");

	Explosiontextures[0] = loadPNG("textures/boom/1.png");
	Explosiontextures[1] = loadPNG("textures/boom/3.png");
	Explosiontextures[2] = loadPNG("textures/boom/5.png");
	Explosiontextures[3] = loadPNG("textures/boom/7.png");
	Explosiontextures[4] = loadPNG("textures/boom/9.png");
	Explosiontextures[5] = loadPNG("textures/boom/11.png");
	Explosiontextures[6] = loadPNG("textures/boom/13.png");
	Explosiontextures[7] = loadPNG("textures/boom/15.png");
	Explosiontextures[8] = loadPNG("textures/boom/17.png");
	Explosiontextures[9] = loadPNG("textures/boom/19.png");
	Explosiontextures[10] = loadPNG("textures/boom/21.png");
	Explosiontextures[11] = loadPNG("textures/boom/23.png");
	Explosiontextures[12] = loadPNG("textures/boom/25.png");
	Explosiontextures[13] = loadPNG("textures/boom/27.png");
	Explosiontextures[14] = loadPNG("textures/boom/29.png");
	Explosiontextures[15] = loadPNG("textures/boom/31.png");
	Explosiontextures[16] = loadPNG("textures/boom/33.png");
	Explosiontextures[17] = loadPNG("textures/boom/35.png");

	ShallowPrefabs[0] = loadPNG("textures/beach/prefabs/Shallow-prefabs/Shallow-Deep_Square.png");
	ShallowPrefabs[1] = loadPNG("textures/beach/prefabs/Shallow-prefabs/Shallow-Deep Rectagle_2x4.png");
	ShallowPrefabs[2] = loadPNG("textures/beach/prefabs/Shallow-prefabs/Shallow-Deep Rectagle_3x4.png");
	ShallowPrefabs[3] = loadPNG("textures/beach/prefabs/Shallow-prefabs/Shallow-Deep Rectagle_3x6.png");
	ShallowPrefabs[4] = loadPNG("textures/beach/prefabs/Shallow-prefabs/Shallow-Deep Rectagle_4x6.png");

	IslandPrefabs[0] = loadPNG("textures/beach/prefabs/Island-prefabs/Island_Square.png");
	IslandPrefabs[1] = loadPNG("textures/beach/prefabs/Island-prefabs/Island_Rectagle_4x6.png");
	IslandPrefabs[2] = loadPNG("textures/beach/prefabs/Island-prefabs/Island_Rectagle_5x6.png");
	IslandPrefabs[3] = loadPNG("textures/beach/prefabs/Island-prefabs/Island_Rectagle_5x8.png");
	
	float sizelongX = worldSizeX;
	float sizelongY = worldSizeY;
	float sizeshort = 5;
	float repeat = 5;

	player = new Player(6.1, 18.4, playerTexture, playerAnim);
	background = new Background(worldSizeX, worldSizeY, deepWater, worldSizeX / repeat, worldSizeY / repeat);
	sandBack = new Background(worldSizeX * 5, worldSizeY * 5, sand, worldSizeX * 5 / repeat, worldSizeY * 5 / repeat);
	
	boarderTop = new Boarder(0, worldSizeY, 0, sizelongX, sizeshort, 1, worldSizeX / 5, topTexture);
	boarderBottom = new Boarder(0, -worldSizeY, 180, sizelongX, sizeshort, 1, worldSizeX / 5, topTexture);

	boarderLeft = new Boarder(-worldSizeX, 0, 0, sizeshort, sizelongY, worldSizeY / 5, 1, sideTexture);
	boarderRight = new Boarder(worldSizeX, 0, 180, sizeshort, sizelongY, worldSizeY / 5, 1, sideTexture);

	cornerTL = new Boarder(-worldSizeX, worldSizeY, 0, 5, 5, 1, 1, tlCorner);
	cornerTR = new Boarder(worldSizeX, worldSizeY, 270, 5, 5, 1, 1, tlCorner);
	cornerBL = new Boarder(-worldSizeX, -worldSizeY, 90, 5, 5, 1, 1, tlCorner);
	cornerBR = new Boarder(worldSizeX, -worldSizeY, 180, 5, 5, 1, 1, tlCorner);
	srand(time(NULL));

	objects.push_back(boarderTop);
	objects.push_back(boarderBottom);
	objects.push_back(boarderLeft);
	objects.push_back(boarderRight);
	objects.push_back(cornerTL);
	objects.push_back(cornerTR);
	objects.push_back(cornerBL);
	objects.push_back(cornerBR);
	
	genShallows(5);
	genEnemies(numEnemy);
	objects.push_back(player);

}

void genEnemies(int numEnemy){
	for (int i = 0; i < numEnemy; i++){
		objects.push_back(new Enemy(worldSizeX-50, worldSizeY-50, 3, 10, EnemyTexture, EnemyDamageTexture));
	}
}

void genShallows(int numShallows){
	for (int i = 0; i < numShallows; i++){
		int j = (rand() % (4 - 0)) + 0;
		switch (j){
		case 0:
			objects.push_back(new Shallows(worldSizeX - 50, worldSizeY - 50, 5 * islandScale, 5 * islandScale, ShallowPrefabs[0]));
			break;
		case 1:
			objects.push_back(new Shallows(worldSizeX - 50, worldSizeY - 50, 2 * islandScale, 4 * islandScale, ShallowPrefabs[1]));
			break;
		case 2:
			objects.push_back(new Shallows(worldSizeX - 50, worldSizeY - 50, 3 * islandScale, 4 * islandScale, ShallowPrefabs[2]));
			break;
		case 3:
			objects.push_back(new Shallows(worldSizeX - 50, worldSizeY - 50, 3 * islandScale, 6 * islandScale, ShallowPrefabs[3]));
			break;
		case 4:
			objects.push_back(new Shallows(worldSizeX - 50, worldSizeY - 50, 4 * islandScale, 6 * islandScale, ShallowPrefabs[4]));
			break;
		}
	}
}

bool checkCollision(BaseObject* obj1, BaseObject* obj2){
	obj1->obb->transformPoints(obj1->matrixOBB);
	obj2->obb->transformPoints(obj2->matrixOBB);

	bool coll = obj1->obb->SAT2D(obj2->obb);
	if (coll){
		glColor3f(1.0, 0.0, 0.0);
	}
	else
		glColor3f(0.0, 1.0, 0.0);

	//obj1->obb->drawOBB();
	//obj2->obb->drawOBB();

	return coll;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);	
	
	glViewport(0, 0, screenWidth, screenHeight);
	glLoadIdentity();
	view();

	if (score<numEnemy && !bossSpawn){
		genEnemies(numEnemy-score);
		score += numEnemy - score;
	}

	drawHealth();
	drawCooldown();
	drawScrap();
	drawKills();
	drawUpgrades();
	if (playerHP<=0.2f)
		healthWarning();
}

void drawHealth(){
	glMatrixMode(GL_PROJECTION);
	double matrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(-screenWidth / hudOrtho, screenWidth / hudOrtho, -screenHeight / hudOrtho, screenHeight / hudOrtho, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef((screenWidth / hudOrtho) - 8, (-screenHeight / hudOrtho) + 8, 0.0);
		glBegin(GL_QUADS);
		glColor3f(1, 0, 0);
		glVertex2f(0, 0);
		glVertex2f(5, 0);
		glVertex2f(5, player->hp * 10);
		glVertex2f(0, player->hp * 10);
		glEnd();


	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cross);
	glTranslatef((screenWidth / hudOrtho) - 8, (-screenHeight / hudOrtho) + 2, 0.0);
	glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0, 0);
		glTexCoord2f(0.0, 1);
		glVertex2f(0, 5);
		glTexCoord2f(1, 1);
		glVertex2f(5, 5);
		glTexCoord2f(1, 0.0);
		glVertex2f(5, 0);
		glEnd();
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void drawCooldown(){
	glMatrixMode(GL_PROJECTION);
	double matrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(-screenWidth / hudOrtho, screenWidth / hudOrtho, -screenHeight / hudOrtho, screenHeight / hudOrtho, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef((screenWidth / hudOrtho) - 8, (-screenHeight / hudOrtho) + 29, 0.0);
		glBegin(GL_QUADS);
		if (ultiCooldown < ultiCooldownMax){
			glColor3f(.112, .138, .144);
		}
		else{
			glColor3f(0, 1, 0);
		}
		glVertex2f(0, 0);
		glVertex2f(5, 0);
		glVertex2f(5, ultiCooldown / 50);
		glVertex2f(0, ultiCooldown / 50);
		glEnd();

	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cooldownIcon);
	glTranslatef((screenWidth / hudOrtho) - 8, (-screenHeight / hudOrtho) + 23, 0.0);
	glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0, 0);
		glTexCoord2f(0.0, 1);
		glVertex2f(0, 5);
		glTexCoord2f(1, 1);
		glVertex2f(5, 5);
		glTexCoord2f(1, 0.0);
		glVertex2f(5, 0);
		glEnd();
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}
void drawScrap(){
	glMatrixMode(GL_PROJECTION);
	double matrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(-screenWidth / hudOrtho, screenWidth / hudOrtho, -screenHeight / hudOrtho, screenHeight / hudOrtho, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0, 0, 0);
	print(font, screenWidth - 65, screenHeight - 39, "%d", scrapLevel);

	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, scrapIcon);
	glTranslatef((screenWidth / hudOrtho) - 22, (screenHeight / hudOrtho) - 10, 0.0);
	glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0, 0);
		glTexCoord2f(0.0, 1);
		glVertex2f(0, 5);
		glTexCoord2f(1, 1);
		glVertex2f(5, 5);
		glTexCoord2f(1, 0.0);
		glVertex2f(5, 0);
		glEnd();
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void drawKills(){
	glMatrixMode(GL_PROJECTION);
	double matrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(-screenWidth / hudOrtho, screenWidth / hudOrtho, -screenHeight / hudOrtho, screenHeight / hudOrtho, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0, 0, 0);

	print(font, screenWidth - 190, screenHeight - 39, "%d/%d", kills, reqKills);

	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, killIcon);
	glTranslatef((screenWidth / hudOrtho) - 55, (screenHeight / hudOrtho) - 10, 0.0);
	glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0, 0);
		glTexCoord2f(0.0, 1);
		glVertex2f(0, 5);
		glTexCoord2f(1, 1);
		glVertex2f(5, 5);
		glTexCoord2f(1, 0.0);
		glVertex2f(5, 0);
		glEnd();
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void drawUpgrades(){
	glMatrixMode(GL_PROJECTION);
	double matrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(-screenWidth / hudOrtho, screenWidth / hudOrtho, -screenHeight / hudOrtho, screenHeight / hudOrtho, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0, 0, 0);
	print(fontSmall, 15, 5, "%d      per Upgrade", upgradeCost);
	print(fontSmall, 15, 30, "1: Speed - ");
	print(fontSmall, 115, 30, "%d/%d", speedCount, upgradesMax);
	print(fontSmall, 15, 50, "2: Range - ");
	print(fontSmall, 115, 50, "%d/%d", rangeCount, upgradesMax);
	print(fontSmall, 15, 70, "3: Fire Rate - ");
	print(fontSmall, 115, 70, "%d/%d", fireRateCount, upgradesMax);
	print(fontSmall, 15, 90, "4: # Shots - ");
	print(fontSmall, 115, 90, "%d/%d", shotCount, upgradesMax);
	print(fontSmall, 15, 110, "Q: Heal");

	print(fontSmall, (screenWidth/2)+53, 5, "B: Zoom In");
	print(fontSmall, (screenWidth/2)-53, 5, "V: Zoom Out");

	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, scrapIcon);
	glTranslatef(-(screenWidth / hudOrtho) + 6.5, -(screenHeight / hudOrtho) + 0.5, 0.0);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(0, 0);
	glTexCoord2f(0.0, 1);
	glVertex2f(0, 4);
	glTexCoord2f(1, 1);
	glVertex2f(4, 4);
	glTexCoord2f(1, 0.0);
	glVertex2f(4, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void healthWarning(){
	glMatrixMode(GL_PROJECTION);
	double matrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(-screenWidth / hudOrtho, screenWidth / hudOrtho, -screenHeight / hudOrtho, screenHeight / hudOrtho, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0.9, 0, 0);
	print(font, (screenWidth/2)-325,(screenHeight/2)+200, "WARNING!!!! HEALTH LOW!!!! NEED REPAIR!!!!");

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void view(){
	
	if (player->hp<=0){
		const int result = MessageBox(NULL, "GAME OVER! - Play Again?", "You Died!", MB_YESNO | MB_ICONSTOP);
		switch(result){
			case IDYES:
				done = true;
				replay = true;
			break;
		case IDNO:
				done = true;
				replay = false;
			break;
		}
	}

	if (bossDead){
		bossKillDelay++;
		if (bossKillDelay>=50){
			char win[200];
			sprintf(win, "YOU WIN BOSS DEFEATED! WITH %d KILLS - Play Again?", kills);
			const int result = MessageBox(NULL, win, "You Win!", MB_YESNO | MB_ICONSTOP);
			switch (result){
			case IDYES:
				done = true;
				replay = true;
				break;
			case IDNO:
				done = true;
				replay = false;
				break;
			}
		}
	}

	sandBack->Render();
	background->Render();

	for (int i = 0; i < objects.size(); i++){
		objects[i]->Render();
		if (objects[i]->getType().compare("player") == 0){
			playerID = i;
			playerX = objects[i]->Xtri;
			playerY = objects[i]->Ytri;
			playerHeading = objects[i]->heading;
			playerHP = objects[i]->hp;
		}
	}

	for (int i = 0; i < objects.size(); i++){
		for (int j = i; j < objects.size(); j++){

			//If both enemies, run avaid function
			if (objects[i]->getType().compare("enemy") == 0 && objects[j]->getType().compare("enemy") == 0){
				enemyAvoid(objects[i], objects[j]);
			}
			
			if (objects[j]->hp <= 0){
				if (objects[j]->getType().compare("player") == 0){
					return;
				}
				if (objects[j]->getType().compare("scrap") == 0){
					scrapLevel++;
				}
				else if (objects[j]->getType().compare("playerShot") == 0 || objects[j]->getType().compare("enemyShot") == 0){
					objects.push_back(new Explosion(objects[j]->Xtri, objects[j]->Ytri, 2, 2, Explosiontextures));
				}
				else if (objects[j]->getType().compare("ultiShot") == 0){
					objects.push_back(new Explosion(objects[j]->Xtri, objects[j]->Ytri, 6, 6, Explosiontextures));
				}
				else if (objects[j]->getType().compare("boss") == 0){
					bossDead = true;
					objects.push_back(new Explosion(objects[j]->Xtri, objects[j]->Ytri, 20, 20, Explosiontextures));
					objects.push_back(new Explosion(objects[j]->Xtri + 20, objects[j]->Ytri + 20, 20, 20, Explosiontextures));
					objects.push_back(new Explosion(objects[j]->Xtri - 20, objects[j]->Ytri - 20, 20, 20, Explosiontextures));
				}
				else if (objects[j]->getType().compare("enemy") == 0){
					objects.push_back(new Scrap(objects[j]->Xtri, objects[j]->Ytri, 2, objects[j]->heading, scrapTexture));
					objects.push_back(new Scrap(objects[j]->Xtri, objects[j]->Ytri, 2, objects[j]->heading, scrapTexture));
					objects.push_back(new Explosion(objects[j]->Xtri, objects[j]->Ytri, 4, 4, Explosiontextures));
					objects.push_back(new Explosion(objects[j]->Xtri+2, objects[j]->Ytri+2, 4, 4, Explosiontextures));
					objects.push_back(new Explosion(objects[j]->Xtri - 2, objects[j]->Ytri - 2, 4, 4, Explosiontextures));
					score--;
					Enemy *temp = (Enemy*)objects[j];
					if(temp->getKilledBy().compare("playerShot") == 0 || temp->getKilledBy().compare("ultiShot") == 0){
						kills++;
					}
				}
				removeColl(objects[j]);
			}
			else if (i != j){
				if (objects[i]->collisionCheck == true && objects[j]->collisionCheck == true && shouldCollide(objects[i], objects[j])){
					collision = checkCollision(objects[i], objects[j]);
					if (collision){
						collisionReaction(objects[i], objects[j]);
					}
				}
			}
		}
	}

	for (int i = 0; i < objects.size(); i++){
		if (objects[i]->getType().compare("enemy") == 0){
			moveEnemytoPlayer(objects[i]);
		}else if (objects[i]->getType().compare("scrap") == 0){
			moveScraptoPlayer(objects[i]);
		}
		else if (objects[i]->getType().compare("boss") == 0){
			moveBoss(objects[i]);
		}
	}

	if(kills >= reqKills && !bossSpawn){
		bossSpawn = true;
		drawBoss = true;
		Boss* boss = new Boss(50, 50, 12, 36, BossTextures, BossTextures);
		objects.push_back(boss);
	}

	if (drawBoss){
		glMatrixMode(GL_PROJECTION);
		double matrix[16];
		glGetDoublev(GL_PROJECTION_MATRIX, matrix);
		glLoadIdentity();
		glOrtho(-screenWidth / hudOrtho, screenWidth / hudOrtho, -screenHeight / hudOrtho, screenHeight / hudOrtho, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(0, 0, 0);
		print(font, (screenWidth / 2) - 240, screenHeight - 50, "ENEMY BOSS HAS APPEARED!!!!!");

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(matrix);
		glMatrixMode(GL_MODELVIEW);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluOrtho2D(player->Xtri - screenWidth / ortho, player->Xtri + screenWidth / ortho, player->Ytri - screenHeight / ortho, player->Ytri + screenHeight / ortho);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glFlush();

	firstPass = false;
}

bool shouldCollide(BaseObject* obj1, BaseObject* obj2){
	std::string obj1type = obj1->getType();
	std::string obj2type = obj2->getType();

	if (obj1type == "player" && obj2type == "enemy"){
		return true;
	}
	if (obj2type == "player" && obj1type == "enemy"){
		return true;
	}
	if (obj1type == "enemy" && (obj2type == "playerShot" || obj2type == "ultiShot")){
		return true;
	}
	if (obj2type == "enemy" && (obj1type == "playerShot" || obj1type == "ultiShot")){
		return true;
	}
	if (obj1type == "boss" && (obj2type == "playerShot" || obj2type == "ultiShot")){
		return true;
	}
	if (obj2type == "boss" && (obj1type == "playerShot" || obj1type == "ultiShot")){
		return true;
	}
	if (obj1type == "enemy" && (obj2type == "playerShot" || obj2type == "ultiShot")){
		return true;
	}
	if (obj2type == "enemy" && (obj1type == "playerShot" || obj1type == "ultiShot")){
		return true;
	}
	if (obj1type == "boss" && (obj2type == "playerShot" || obj2type == "ultiShot")){
		return true;
	}
	if (obj2type == "boss" && (obj1type == "playerShot" || obj1type == "ultiShot")){
		return true;
	}
	if (obj1type == "player" && obj2type == "enemyShot"){
		return true;
	}
	if (obj2type == "player" && obj1type == "enemyShot"){
		return true;
	}
	if (obj1type == "boarder" && obj2type == "player"){
		return true;
	}
	if (obj2type == "boarder" && obj1type == "player"){
		return true;
	}
	if (obj1type == "boarder" && obj2type == "enemy"){
		return true;
	}
	if (obj2type == "boarder" && obj1type == "enemy"){
		return true;
	}
	if (obj1type == "shallows" && obj2type == "player"){
		return true;
	}
	if (obj2type == "shallows" && obj1type == "player"){
		return true;
	}
	if (obj1type == "scrap" && obj2type == "player"){
		return true;
	}
	if (obj2type == "scrap" && obj1type == "player"){
		return true;
	}
	if (obj1type == "shallows" && obj2type == "shallows" && !firstPass){
		return true;
	}
	if (obj2type == "shallows" && obj1type == "shallows" && !firstPass){
		return true;
	}
	if (obj1type == "boss" && obj2type == "player"){
		return true;
	}
	if (obj2type == "boss" && obj1type == "player"){
		return true;
	}
	if (obj1type == "boss" && obj2type == "enemy"){
		return true;
	}
	if (obj2type == "boss" && obj1type == "enemy"){
		return true;
	}

	return false;
}

void removeColl(BaseObject* obj){
	for (int i = 0; i < objects.size(); i++){
		if (objects[i]==obj){
			objects.erase(objects.begin() + i);
			break;
		}
	}
}

void collisionReaction(BaseObject* obj1, BaseObject* obj2){
	//Player Colliding
	if (firstPass){
		if (obj1->getType() == "player"){
			if (obj2->getType() == "enemy"){
				score--;
				obj2->hp = 0;
				return;
			}
		}
		else if (obj2->getType() == "player"){
			if (obj1->getType() == "enemy"){
				score--;
				obj1->hp = 0;
				return;
			}
		}
	}

	//Enemy Colliding
	if (firstPass){
		if (obj1->getType() == "enemy"){		
			if (obj2->getType() != "player"){
				score--;
				obj1->hp = 0;
				return;
			}
		}
		else if (obj2->getType() == "enemy"){
			if (obj1->getType() != "player"){
				score--;
				obj2->hp = 0;
				return;
			}
		}
	}

	//Island/Shallows Colliding
	if (firstPass){
		if (obj1->getType() == "island" || obj1->getType() == "shallows"){
			if (obj2->getType() != "player"){
				obj2->hp = 0;
				return;
			}
			else{
				obj1->hp = 0;
				return;
			}
		}
		else if (obj2->getType() == "island" || obj2->getType() == "shallows"){
			if (obj1->getType() != "player"){
				obj1->hp = 0;
				return;
			}
			else{
				obj2->hp = 0;
				return;
			}
		}
	}
	obj1->CollisionReaction(obj2);
	obj2->CollisionReaction(obj1);
}

void moveScraptoPlayer(BaseObject* scrap){
	Point dir;
	float hyp;

	dir.x = playerX - scrap->origX;
	dir.y = playerY - scrap->origY;
	hyp = sqrt(dir.x*dir.x + dir.y*dir.y);

	float scrapHeading = std::atan2(-dir.x, dir.y);
	scrapHeading = (scrapHeading *(180.0f / PI));
	if (scrapHeading<0){
		scrapHeading += 360;
	}

	//Move to player
	if ((hyp < 200 && hyp>0)){
		scrap->heading = scrapHeading;
		scrap->Vtri = maxSpeed +1;
	}
}

void moveEnemytoPlayer(BaseObject* enemy){
	Point dir;
	float hyp;

	dir.x = playerX - enemy->Xtri;
	dir.y = playerY - enemy->Ytri;
	hyp = sqrt(dir.x*dir.x + dir.y*dir.y);

	int enemydetectRange = 200;
	int enemyShootRange = 70;

	switch (enemyLevel){
		case 1:
			enemydetectRange = 200;
			enemyShootRange = 70;
			enemyRange = 1;
			break;
		case 2:
			enemydetectRange = 225;
			enemyShootRange = 90;
			enemyRange = 1.25;
			break;
		case 3:
			enemydetectRange = 250;
			enemyShootRange = 110;
			enemyRange = 1.5;
			break;
		case 4:
			enemydetectRange = 275;
			enemyShootRange = 130;
			enemyRange = 1.75;
			break;
		case 5:
			enemydetectRange = 300;
			enemyShootRange = 150;
			enemyRange = 2;
			break;
	}

	float enemyHeading = std::atan2(-dir.x, dir.y);
	enemyHeading = (enemyHeading *(180.0f / PI));
	if (enemyHeading<0){
		enemyHeading += 360;
	}

	//Move to player
	if ((hyp < enemydetectRange && hyp>enemyShootRange + 5) ||
		(enemy->spotted && hyp>enemyShootRange+5)){
		enemy->spotted = true;
		enemy->heading = enemyHeading;
		enemy->Vtri = 0.5f;
	}

	//Shoot in range
	if ((hyp < enemyShootRange && hyp>10)){
		enemy->heading = enemyHeading + 90;
		enemyShoot(enemy);
	}

	//Change speed
	if ((hyp < enemyShootRange-1 && hyp>0)){
		enemy->Vtri = 0;
	}
}


void moveBoss(BaseObject* bos){
	Boss *boss = (Boss*)bos;
	Point dir;
	float hyp;

	dir.x = playerX - boss->Xtri;
	dir.y = playerY - boss->Ytri;
	hyp = sqrt(dir.x*dir.x + dir.y*dir.y);

	float enemyHeading = std::atan2(-dir.x, dir.y);
	enemyHeading = (enemyHeading *(180.0f / PI));
	if (enemyHeading<0){
		enemyHeading += 360;
	}

	if ((hyp>100)){
		boss->heading = enemyHeading;
		boss->Vtri = 1;
	}

	if ((hyp < 200 && hyp>0)){
		float bossShotRange = 1;
		int bossSpinSpeed = 1;
		int bossCooldown = 2;

		if (boss->cooldown <= 0){
			if (boss->hp <= 1){
				bossSpinSpeed = 25;
				bossCooldown = 10;
				bossShootCircle(boss, boss->bossShotAngle,       bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 36,  bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 72,  bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 108, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 144, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 180, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 216, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 252, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 288, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 324, bossShotRange);
			}else if (boss->hp <= 2){
				bossSpinSpeed = 25;
				bossCooldown = 9;
				bossShootCircle(boss, boss->bossShotAngle,       bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 45,  bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 90,  bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 135, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 180, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 225, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 270, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 315, bossShotRange);
			}else if (boss->hp <= 3){
				bossSpinSpeed = 25;
				bossCooldown = 8;
				bossShootCircle(boss, boss->bossShotAngle,       bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 60,  bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 120, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 180, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 240, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 300, bossShotRange);
			}
			else if (boss->hp <= 4){
				bossSpinSpeed = 25;
				bossCooldown = 7;
				bossShootCircle(boss, boss->bossShotAngle, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 90, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 180, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 270, bossShotRange);
			}
			else{
				bossSpinSpeed = 25;
				bossCooldown = 6;
				bossShootCircle(boss, boss->bossShotAngle, bossShotRange);
				bossShootCircle(boss, boss->bossShotAngle + 180, bossShotRange);
			}

			boss->cooldown = bossCooldown;
		}
		if (boss->cooldown>0){
			boss->cooldown -= 1;
		}

		if (boss->bossShotAngle <= 0){
			boss->bossShotAngle = 360;
		}
		if (boss->bossShotAngle>0){
			boss->bossShotAngle -= bossSpinSpeed;
		}
	}
}

void bossShootCircle(BaseObject* boss, int count, int hp){
		objects.push_back(new Shot(hp, 360 - count, boss->Xtri, boss->Ytri, 3, enemyShotTexture, "enemyShot", 1));
}

void enemyAvoid(BaseObject* obj1, BaseObject* obj2){
	Point dir;
	float hyp;

	dir.x = obj2->Xtri - obj1->Xtri;
	dir.y = obj2->Ytri - obj1->Ytri;

	hyp = sqrt(dir.x*dir.x + dir.y*dir.y);
	dir.x /= hyp;
	dir.y /= hyp;

	if (hyp < 10 && hyp>0){
		obj1->Xtri -= dir.x*0.5;
		obj1->Ytri -= dir.y*0.5;
		obj2->Xtri += dir.x*0.5;
		obj2->Ytri += dir.y*0.5;
	}
}

void enemyShoot(BaseObject* enemy){
	if (kills>=limit*8){
		if (enemy->cooldown <= 0 && objects.size() < objectLimit){
			enemyLevel = 5;
			objects.push_back(new Shot(enemyRange, enemy->heading - 70, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			objects.push_back(new Shot(enemyRange, enemy->heading - 80, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			objects.push_back(new Shot(enemyRange, enemy->heading - 90, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			objects.push_back(new Shot(enemyRange, enemy->heading - 100, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			objects.push_back(new Shot(enemyRange, enemy->heading - 110, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			enemy->cooldown = 100;
		}
		if (enemy->cooldown>0){
			enemy->cooldown -= 9;
		}
	}
	else if (kills >= limit*6){
		if (enemy->cooldown <= 0 && objects.size() < objectLimit){
			enemyLevel = 4;
			objects.push_back(new Shot(enemyRange, enemy->heading - 70, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			objects.push_back(new Shot(enemyRange, enemy->heading - 80, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			objects.push_back(new Shot(enemyRange, enemy->heading - 90, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			objects.push_back(new Shot(enemyRange, enemy->heading - 100, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			enemy->cooldown = 100;
		}
		if (enemy->cooldown>0){
			enemy->cooldown -= 7;
		}
	}
	else if (kills >= limit*4){
		if (enemy->cooldown <= 0 && objects.size() < objectLimit){
			enemyLevel = 3;
			objects.push_back(new Shot(enemyRange, enemy->heading - 80, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			objects.push_back(new Shot(enemyRange, enemy->heading - 90, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			objects.push_back(new Shot(enemyRange, enemy->heading - 100, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			enemy->cooldown = 100;
		}
		if (enemy->cooldown>0){
			enemy->cooldown -= 5;
		}
	}
	else if (kills >= limit*2){
		if (enemy->cooldown <= 0 && objects.size() < objectLimit){
			enemyLevel = 2;
			objects.push_back(new Shot(enemyRange, enemy->heading - 80, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			objects.push_back(new Shot(enemyRange, enemy->heading - 90, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			enemy->cooldown = 100;
		}
		if (enemy->cooldown>0){
			enemy->cooldown -= 3;
		}
	}
	else{
		if (enemy->cooldown <= 0 && objects.size() < objectLimit){
			enemyLevel = 1;
			objects.push_back(new Shot(enemyRange, enemy->heading - 90, enemy->Xtri, enemy->Ytri, 3, enemyShotTexture, "enemyShot", 1));
			enemy->cooldown = 100;
		}
		if (enemy->cooldown>0){
			enemy->cooldown -= 1;
		}
	}
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width; screenHeight = height;           // to ensure the mouse coordinates match 
	// we will use these values to set the coordinate system

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	gluOrtho2D(-30, 30, -30, 30);           // set the coordinate system for the window

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void processKeys()
{
	if (keys[VK_RIGHT]){
		if (cooldown<= 0){
			switch (shotCount){
			case 0:
				objects.push_back(new Shot(range, player->heading - 90 + (speedCount*3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;
			case 1:
				objects.push_back(new Shot(range, player->heading - 90 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 100 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;
			case 2:
				objects.push_back(new Shot(range, player->heading - 80 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 90 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 100 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;
			case 3:
				objects.push_back(new Shot(range, player->heading - 70 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 80 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 90 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 100 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;
			case 4:
				objects.push_back(new Shot(range, player->heading - 70 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 80 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 90 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 100 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 110 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;
			case 5:
				objects.push_back(new Shot(range, player->heading - 65 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 75 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 85 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 95 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 105 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading - 115 + (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;
			}
			
			cooldown = cooldownReset;
		}
	}
	else if (keys[VK_LEFT]){
		if (cooldown <= 0){
			switch (shotCount){
			case 0:
				objects.push_back(new Shot(range, player->heading + 90 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;
			case 1:
				objects.push_back(new Shot(range, player->heading + 90 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 100 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;
			case 2:
				objects.push_back(new Shot(range, player->heading + 80 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 90 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 100 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;											  
			case 3:												  
				objects.push_back(new Shot(range, player->heading + 70 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 80 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 90 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 100 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;											  
			case 4:												  
				objects.push_back(new Shot(range, player->heading + 70 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 80 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 90 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 100 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 110 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;
			case 5:
				objects.push_back(new Shot(range, player->heading + 65 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 75 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 85 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 95 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 105 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				objects.push_back(new Shot(range, player->heading + 115 - (speedCount * 3), player->Xtri, player->Ytri, 3, shotTexture, "playerShot", 1));
				break;
			}

			cooldown = cooldownReset;
		}
	}
	if (cooldown>0){
		cooldown--;
	}
	if (keys['A'])
	{
		if (player->Vtri>0){
			player->heading += player->Vtri + .1f;
		}
		else if (player->Vtri<0){
			player->heading += player->Vtri - .1f;
		}
	}
	else if (keys['D'])
	{
		if (player->Vtri>0){
			player->heading -= player->Vtri + .1f;
		}
		else if (player->Vtri<0){
			player->heading -= player->Vtri - .1f;
		}
		
	}
	else if (keys['W'])
	{
		if (player->Vtri<maxSpeed){
			player->Vtri += maxSpeed/10;
		}
	}
	else if (keys['S'])
	{
		if (player->Vtri>-maxSpeed/2){
			player->Vtri -= (maxSpeed/2)/10;
		}
	}
	else
	{
		if (player->Vtri>0){
			player->Vtri -= 0.001f;
		}
		else if (player->Vtri<0){
			player->Vtri += 0.001f;
		}
	}
	//Heal
	if (keys['Q'])
	{
		if (scrapLevel >= upgradeCost && objects[playerID]->hp < 1 && keyCount <= 0){
			objects[playerID]->hp = 1;
			scrapLevel -= upgradeCost;
			keyCount = maxKeyCount;
		}
	}
	//Upgrade Num Shots
	if (keys['4'])
	{
		if (scrapLevel >= upgradeCost && keyCount <= 0){
			if (shotCount<upgradesMax){
				scrapLevel -= upgradeCost;
				keyCount = maxKeyCount;
				shotCount++;
			}
		}
	}
	//Upgrade Fire Rate
	if (keys['3'])
	{
		if (scrapLevel >= upgradeCost && keyCount <= 0){
			if(fireRateCount<upgradesMax){
				cooldownReset -= 8;
				scrapLevel -= upgradeCost;
				keyCount = maxKeyCount;
				fireRateCount++;
			}
		}
	}
	//Upgrade Shot Range
	if (keys['2'])
	{
		if (scrapLevel >= upgradeCost && keyCount <= 0){
			if(rangeCount<upgradesMax){
				range += 0.5;
				scrapLevel -= upgradeCost;
				keyCount = maxKeyCount;
				rangeCount++;
			}
		}
	}
	//Upgrade speed
	if (keys['1'])
	{
		if (scrapLevel >= upgradeCost && keyCount <= 0){
			if(speedCount<upgradesMax){
				maxSpeed += 0.25;
				scrapLevel -= upgradeCost;
				keyCount = maxKeyCount;
				speedCount++;
			}
		}
	}

	if (keys[VK_SPACE]){
		if (ultiCooldown >= ultiCooldownMax && keyCount <= 0){
			objects.push_back(new Shot(2, player->heading      , player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 24 , player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 48 , player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 72 , player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 96 , player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 120, player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 144, player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 168, player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 192, player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 216, player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 240, player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 264, player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 288, player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 312, player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));
			objects.push_back(new Shot(2, player->heading + 336, player->Xtri, player->Ytri, 3, ultiShot, "ultiShot", 6));

			keyCount = maxKeyCount;
			ultiCooldown = 0;
		}
	}

	if (ultiCooldown < ultiCooldownMax){
		ultiCooldown+=0.1f;
	}

	if (keys['B'])
	{
		if (ortho <15){
			ortho += 0.1;
		}
	}
	else if (keys['V'])
	{
		if (ortho >2){
			ortho -= 0.1;
		}
	}

	if (keys['P'] ){
		if (keyCount <= 0){
			paused = MessageBox(NULL, "GAME PAUSED", "PAUSE", MB_OK | MB_ICONSTOP);
			keyCount = maxKeyCount;
			keys['P'] = false;
			keys['V'] = false;
			keys['B'] = false;
			keys['1'] = false;
			keys['2'] = false;
			keys['3'] = false;
			keys['4'] = false;
			keys['W'] = false;
			keys['A'] = false;
			keys['S'] = false;
			keys['D'] = false;
			keys['H'] = false;
			keys[VK_LEFT] = false;
			keys[VK_RIGHT] = false;
			keys[VK_SPACE] = false;
		}
	}
	keyCount--;
}

/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{

	DWORD next_game_tick = GetTickCount();
	int sleep_time = 0;
	MSG		msg;									// Windows Message Structure
	replay = true;
	while(replay){
	// Create Our OpenGL Window
	if (!CreateGLWindow("Battleship", screenWidth, screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}
		FRAMES_PER_SECOND = 60;
		SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

		replay = false;
		done = false;
		while(!done)									// Loop That Runs While done=FALSE
		{
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
			{
				if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
				{
					done=true;							// If So done=TRUE
				}
				else									// If Not, Deal With Window Messages
				{
					TranslateMessage(&msg);				// Translate The Message
					DispatchMessage(&msg);				// Dispatch The Message
				}
			}
			else										// If There Are No Messages
			{
				processKeys();

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				display();						// Draw The Scene
				SwapBuffers(hDC);				// Swap Buffers (Double Buffering)

				next_game_tick += SKIP_TICKS;
				sleep_time = next_game_tick - GetTickCount();
				if (sleep_time >= 0) {
					Sleep(sleep_time);
				}
			}
		}
		objects.clear();
		KillGLWindow();
	}
	return (int)(msg.wParam); // Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight - HIWORD(lParam);
				LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
	            LeftPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight  - HIWORD(lParam);
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon = (HICON)LoadImage( // returns a HANDLE so we have to cast to HICON
		NULL,             // hInstance must be NULL when loading from a file
		"textures/ship.ico",   // the icon file name
		IMAGE_ICON,       // specifies that the file is an icon
		0,                // width of the image (we'll specify default later on)
		0,                // height of the image
		LR_LOADFROMFILE |  // we want to load a file (as opposed to a resource)
		LR_DEFAULTSIZE |   // default metrics based on the type (IMAGE_ICON, 32x32)
		LR_SHARED         // let the system release the handle when it's no longer used
		);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();

	return true;									// Success
}



