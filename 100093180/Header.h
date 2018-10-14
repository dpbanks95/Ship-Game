#ifndef RESOURCES_H
#define	RESOURCES_H
#include "BaseObject.h"

int screenWidth = 1280;
int screenHeight = 720;
void moveEnemytoPlayer(BaseObject*);
void moveScraptoPlayer(BaseObject*);
void enemyShoot(BaseObject*);
void enemyAvoid(BaseObject*, BaseObject*);
bool shouldCollide(BaseObject*, BaseObject*);
POINT pt;

int	mouse_x = 0, mouse_y = 0;
bool LeftPressed;
bool keys[256];
bool pause;
bool done;
bool replay;
float ortho;
float hudOrtho;
int cooldown;
int cooldownReset;
int scrapLevel;
bool firstPass;
int score;
int numEnemy;
int islandScale;
int kills;
int reqKills;
int limit;
int playerID;
int keyCount;
int maxKeyCount;
int speedCount;
int fireRateCount;
int rangeCount;
int shotCount;
int upgradesMax;
int upgradeCost;
int enemyLevel;
float playerHP;
float range;
float enemyRange;
float maxSpeed;
int objectLimit;
int bossKillDelay;

float ultiCooldown;
float ultiCooldownMax;

float playerX;
float playerY;
float playerHeading;

float worldSizeX;
float worldSizeY;

const double PI = 3.1415926535897932384626433832795;

//OPENGL FUNCTION PROTOTYPES
void display();				//draws everything to the screen
void reshape(int width, int height);	//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys();
void initVariables();
void genEnemies(int);
void genShallows(int);
bool checkCollision(BaseObject*, BaseObject*);
void view();
void drawHealth();
void drawCooldown();
void drawScrap();
void drawKills();
void drawUpgrades();
void healthWarning();
void removeColl(BaseObject* obj);
void collisionReaction(BaseObject* obj1, BaseObject* obj2);
void loadExplosion();
void bossShootCircle(BaseObject* boss, int count, int hp);
void moveBoss(BaseObject* boss);

// This stores a handle to the texture
GLuint playerTexture;
GLuint playerAnim[10];
GLuint EnemyTexture[11];
GLuint BossTextures[6];
GLuint EnemyDamageTexture[11];
GLuint Explosiontextures[18];
GLuint IslandPrefabs[5];
GLuint ShallowPrefabs[6];

GLuint topTexture;
GLuint sideTexture;
GLuint tlCorner;

GLuint shallowWater;
GLuint deepWater;
GLuint sand;

GLuint shallowDeepTop;
GLuint shallowDeepSide;
GLuint shallowDeepCorner;

GLuint shotTexture;
GLuint ultiShot;
GLuint enemyShotTexture;
GLuint cross;
GLuint cooldownIcon;
GLuint scrapIcon;
GLuint killIcon;
GLuint scrapTexture;

Player *player;
Background *background;
Background *sandBack;
Boarder *boarderTop;
Boarder *boarderBottom;
Boarder *boarderLeft;
Boarder *boarderRight;

Boarder *cornerTL;
Boarder *cornerTR;
Boarder *cornerBL;
Boarder *cornerBR;

int FRAMES_PER_SECOND = 60;
int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

std::vector<BaseObject*> objects;

bool collision;
bool bossSpawn;
bool drawBoss;
bool bossDead;
bool paused;

#endif // RESOURCES_H
