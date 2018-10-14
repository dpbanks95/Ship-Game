#ifndef BASEOBJECT_H
#define	BASEOBJECT_H

#include <set>
#include "OBB.h"

class BaseObject
{
public:
	float origX;
	float origY;
	float Xtri;
	float Ytri;
	float Vtri;
	float heading;
	float hp;
	float position;
	bool collisionCheck;
	bool shoot;
	int count;
	std::string type;
	bool spotted;
	int cooldown;

	GLuint texture;
	OBB *obb;
	float matrixOBB[16];
	BaseObject();
	BaseObject(float x, float y);
	BaseObject(GLuint tex);
	BaseObject(float origX, float origY, GLuint tex);
	virtual void Render() = 0;
	virtual std::string getType() = 0;
	virtual void CollisionReaction(BaseObject*) = 0;
};

class Shot : public BaseObject
{
public:
	float scale;

	Shot(float hp, float heading, float origX, float origY, float Vtri, GLuint tex, std::string type, float scale);
	virtual void Render();
	virtual std::string getType(){ return type; }
	virtual void CollisionReaction(BaseObject*);
};

class Player : public BaseObject
{
public:
	float score;
	int frame;
	int i;
	GLuint* animation;

	Player(float origX, float origY, GLuint tex, GLuint* animation);
	virtual void Render();
	virtual std::string getType(){ return type; }
	virtual void CollisionReaction(BaseObject*);
	void drawHealth(float);
};

class Enemy : public BaseObject
{
public:
	int frame;
	int i;
	GLuint* damage;
	GLuint* animation;
	std::string killedBy;

	Enemy(float min, float max, float origX, float origY, GLuint* tex, GLuint* damageTex);
	virtual void Render();
	virtual std::string getType(){ return type; }
	std::string getKilledBy(){ return killedBy; }
	virtual void CollisionReaction(BaseObject*);
};

class Boss : public BaseObject
{
public:
	int frame;
	int i;
	GLuint* damage;
	GLuint* animation;
	std::string killedBy;
	int bossShotAngle;

	Boss(float min, float max, float origX, float origY, GLuint* tex, GLuint* damageTex);
	virtual void Render();
	virtual std::string getType(){ return type; }
	std::string getKilledBy(){ return killedBy; }
	virtual void CollisionReaction(BaseObject*);
};

class Explosion : public BaseObject
{
public:
	GLuint *explosion;
	int i;
	float sizeX;
	float sizeY;

	Explosion(float origX, float origY, float sizeX, float sizeY, GLuint* textures);
	virtual void Render();
	virtual std::string getType(){ return type; }
	virtual void CollisionReaction(BaseObject*);
};

class Background : public BaseObject
{
public:
	float repeatX;
	float repeatY;

	Background(GLuint tex);
	Background(float origX, float origY, GLuint tex, float repeatX, float repeatY);
	virtual void Render();
	virtual std::string getType(){ return type; }
	virtual void CollisionReaction(BaseObject*);
};

class Boarder : public BaseObject
{
public:
	float angle;
	float sizex;
	float sizey;
	float vert1;
	float vert2;


	Boarder(float transx, float transy, float angle, float sizex, float sizey, float vert1, float vert2, GLuint texture);
	virtual void Render();
	virtual std::string getType(){ return type; }
	virtual void CollisionReaction(BaseObject*);

};

class Shallows : public BaseObject
{
public:
	float sizex;
	float sizey;

	Shallows(float worldX, float worldY, float sizex, float sizey, GLuint texture);
	virtual void Render();
	virtual std::string getType(){ return type; }
	virtual void CollisionReaction(BaseObject*);

};

class Scrap : public BaseObject
{
public:
	float size;

	Scrap(float origX, float origY, float size, float heading, GLuint texture);
	virtual void Render();
	virtual std::string getType(){ return type; }
	virtual void CollisionReaction(BaseObject*);
};

#endif
