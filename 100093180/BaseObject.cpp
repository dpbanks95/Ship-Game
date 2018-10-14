#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLu.h>
#include <ctime>

#include "BaseObject.h"

BaseObject::BaseObject(){
	origX = 0;
	origY = 0;
	Xtri = 0;
	Ytri = 0;
	Vtri = 0;
	heading = 0;
	count = 0;
	hp = 1;
	collisionCheck = true;
	spotted = false;
	shoot = false;
	std::string type = "baseObject";
}

BaseObject::BaseObject(GLuint tex){
	origX = 0;
	origY = 0;
	Xtri = 0;
	Ytri = 0;
	Vtri = 0;
	heading = 0;
	count = 0;
	hp = 1;
	collisionCheck = true;
	spotted = false;
	shoot = false;
	std::string type = "baseObject";
	texture = tex;
	obb = new OBB();
}

BaseObject::BaseObject(float x, float y){
	origX = 0;
	origY = 0;
	Xtri = 0;
	Ytri = 0;
	Vtri = 0;
	heading = 0;
	count = 0;
	hp = 1;
	collisionCheck = true;
	spotted = false;
	shoot = false;
	std::string type = "baseObject";
	origX = x;
	origY = y;
	obb = new OBB();
	obb->vertOriginal[0].x = -origX;
	obb->vertOriginal[0].y = -origY;

	obb->vertOriginal[1].x = -origX;
	obb->vertOriginal[1].y = origY;

	obb->vertOriginal[2].x = origX;
	obb->vertOriginal[2].y = origY;

	obb->vertOriginal[3].x = origX;
	obb->vertOriginal[3].y = -origY;
}

BaseObject::BaseObject(float x, float y, GLuint tex){
	origX = 0;
	origY = 0;
	Xtri = 0;
	Ytri = 0;
	Vtri = 0;
	heading = 0;
	count = 0;
	hp = 1;
	collisionCheck = true;
	spotted = false;
	shoot = false;
	std::string type = "baseObject";
	texture = tex;
	obb = new OBB();
	origX = x;
	origY = y;

	obb->vertOriginal[0].x = -origX;
	obb->vertOriginal[0].y = -origY;

	obb->vertOriginal[1].x = -origX;
	obb->vertOriginal[1].y = origY;

	obb->vertOriginal[2].x = origX;
	obb->vertOriginal[2].y = origY;

	obb->vertOriginal[3].x = origX;
	obb->vertOriginal[3].y = -origY;
}

Player::Player(float origX, float origY, GLuint tex, GLuint* animation) : BaseObject(tex){
	this->origX = origX;
	this->origY = origY;
	this->animation = animation;
	i = 0;
	frame = 0;
	Xtri = 0;
	Ytri = 0;
	this->heading = 1;
	collisionCheck = true;
	float x = origX - 4;
	float y = origY - 7;
	type = "player";
	score = 0;
	count = 0;

	obb->vertOriginal[0].x = -x;
	obb->vertOriginal[0].y = -y;

	obb->vertOriginal[1].x = -x;
	obb->vertOriginal[1].y = y;

	obb->vertOriginal[2].x = x;
	obb->vertOriginal[2].y = y;

	obb->vertOriginal[3].x = x;
	obb->vertOriginal[3].y = -y;

}

Enemy::Enemy(float worldX, float worldY, float origX, float origY, GLuint* tex, GLuint* damageTex) :BaseObject(tex[0]){
	damage = damageTex;
	animation = tex;
	i = 0;
	frame = 0;
	int minX = -worldX;
	int maxX = worldX;
	int minY = -worldY;
	int maxY = worldY;
	Vtri = 0.5f;
	cooldown = 0;
	collisionCheck = true;
	type = "enemy";

	Xtri = (rand() % (maxX - minX)) + minX;
	Ytri = (rand() % (maxY - minY)) + minY;
	heading = (rand() % (359 - 1)) + 1;

	this->origX = origX;
	this->origY = origY;

	float x = origX - 1;
	float y = origY - 6;

	obb->vertOriginal[0].x = -x;
	obb->vertOriginal[0].y = -y;

	obb->vertOriginal[1].x = -x;
	obb->vertOriginal[1].y = y;

	obb->vertOriginal[2].x = x;
	obb->vertOriginal[2].y = y;

	obb->vertOriginal[3].x = x;
	obb->vertOriginal[3].y = -y;

}

Boss::Boss(float worldX, float worldY, float origX, float origY, GLuint* tex, GLuint* damageTex) :BaseObject(tex[0]){
	bossShotAngle = 0;
	damage = damageTex;
	animation = tex;
	i = 0;
	frame = 0;
	Vtri = 0.5f;
	cooldown = 0;
	collisionCheck = true;
	type = "boss";
	this->hp = 10;

	Xtri = worldX;
	Ytri = worldY;
	heading = (rand() % (359 - 1)) + 1;

	this->origX = origX;
	this->origY = origY;

	float x = origX - 18;
	float y = origY - 55;

	obb->vertOriginal[0].x = -x;
	obb->vertOriginal[0].y = -y;

	obb->vertOriginal[1].x = -x;
	obb->vertOriginal[1].y = y;

	obb->vertOriginal[2].x = x;
	obb->vertOriginal[2].y = y;

	obb->vertOriginal[3].x = x;
	obb->vertOriginal[3].y = -y;

}

Background::Background(GLuint tex) : BaseObject(tex){
	collisionCheck = false;
	type = "background";
}

Background::Background(float origX, float origY, GLuint tex, float repeatX, float repeatY) : BaseObject(origX, origY, tex){
	this->repeatX = repeatX;
	this->repeatY = repeatY;
	collisionCheck = false;
	type = "background";
}

Explosion::Explosion(float origX, float origY, float sizex, float sizey, GLuint* textures){
	obb = new OBB();
	this->origX = origX;
	this->origY = origY;
	sizeX = sizex;
	sizeY = sizey;
	type = "explosion";
	i = 0;
	explosion = textures;
	hp = 18;
	collisionCheck = false;
	obb->vertOriginal[0].x = -2;
	obb->vertOriginal[0].y = -2;

	obb->vertOriginal[1].x = -2;
	obb->vertOriginal[1].y = 2;

	obb->vertOriginal[2].x = 2;
	obb->vertOriginal[2].y = 2;

	obb->vertOriginal[3].x = 2;
	obb->vertOriginal[3].y = -2;
}

Shot::Shot(float hp, float heading, float origX, float origY, float Vtri, GLuint tex, std::string type, float scale) : BaseObject(origX, origY, tex){
	this->scale = scale;
	this->heading = heading;
	this->Xtri = origX;
	this->Ytri = origY;
	this->Vtri = Vtri;
	this->type = type;
	this->hp = hp;
	type = "playerShot";
	collisionCheck = true;
	obb->vertOriginal[0].x = -1 * scale;
	obb->vertOriginal[0].y = -2 * scale;

	obb->vertOriginal[1].x = -1 * scale;
	obb->vertOriginal[1].y = 2 * scale;

	obb->vertOriginal[2].x = 1 * scale;
	obb->vertOriginal[2].y = 2 * scale;

	obb->vertOriginal[3].x = 1 * scale;
	obb->vertOriginal[3].y = -2 * scale;
}

Boarder::Boarder(float transx, float transy, float angle, float sizex, float sizey, float vert1, float vert2, GLuint texture) : BaseObject(sizex, sizey, texture){
	this->Xtri = transx;
	this->Ytri = transy;
	this->angle = angle;
	this->sizex = sizex;
	this->sizey = sizey;
	this->vert1 = vert1;
	this->vert2 = vert2;
	type = "boarder";
	collisionCheck = true;
}

Shallows::Shallows(float worldX, float worldY, float sizex, float sizey, GLuint texture) : BaseObject(texture){
	int minX = -worldX;
	int maxX = worldX;
	int minY = -worldY;
	int maxY = worldY;

	Xtri = (rand() % (maxX - minX)) + minX;
	Ytri = (rand() % (maxY - minY)) + minY;
	heading = (rand() % (359 - 1)) + 1;

	this->sizex = sizex;
	this->sizey = sizey;
	type = "shallows";
	collisionCheck = true;

	this->origX = sizex;
	this->origY = sizey;

	float x = origX - 5;
	float y = origY - 5;

	obb->vertOriginal[0].x = -x;
	obb->vertOriginal[0].y = -y;

	obb->vertOriginal[1].x = -x;
	obb->vertOriginal[1].y = y;

	obb->vertOriginal[2].x = x;
	obb->vertOriginal[2].y = y;

	obb->vertOriginal[3].x = x;
	obb->vertOriginal[3].y = -y;

}

Scrap::Scrap(float origX, float origY, float size, float heading, GLuint texture) :BaseObject(texture){
	obb = new OBB();
	this->origX = origX;
	this->origY = origY;
	this->size = size;
	Vtri = 0;
	this->heading = heading;
	type = "scrap";
	count = 0;
	hp = 1;
	collisionCheck = true;
	obb->vertOriginal[0].x = -size;
	obb->vertOriginal[0].y = -size;

	obb->vertOriginal[1].x = -size;
	obb->vertOriginal[1].y = size;

	obb->vertOriginal[2].x = size;
	obb->vertOriginal[2].y = size;

	obb->vertOriginal[3].x = size;
	obb->vertOriginal[3].y = -size;
}

void Player::CollisionReaction(BaseObject* obj){
	if (obj->getType().compare("shallows") == 0){
		//Slow down
		if (Vtri > 1){
			Vtri = 0.5f;
		}
		else if (Vtri > 0.1f){
			Vtri = 0.05f;
		}
		
	}
	else{
		if (count <= 0 
			&& obj->getType().compare("playerShot") != 0 
			&& obj->getType().compare("enemyShot") != 0 
			&& obj->getType().compare("scrap") != 0 
			&& obj->getType().compare("enemy") != 0){
			heading += 180;
			if (heading>360){
				heading -= 360;
			}
			Vtri = 0.01f;
				
			hp -= 0.3f;
			count = 20;
		}
	}

	if (obj->getType().compare("enemy") == 0){
		hp -= 0.001f;
		Vtri = -0.3f;
	}

	if (obj->getType().compare("enemyShot") == 0){
		hp -= 0.01f;
		obj->hp -= 1;
	}
}

void Shot::CollisionReaction(BaseObject* obj){
	if (this->type.compare("ultiShot") != 0){
		hp -= 1;
	}
}

void Enemy::CollisionReaction(BaseObject* obj){
	//Enemy collide with player
	if (obj->getType().compare("player") == 0){
		hp -= 0.025f;
	}

	//Enemy collide with boarder or boss
	else if (count <= 0 && (obj->getType().compare("boarder") == 0 || 
		obj->getType().compare("boss") == 0)){
		heading += 180;
		if (heading>360){
			heading -= 360;
		}
		count = 5;
	}

	//Enemy colide with playerShot
	else if (obj->getType().compare("playerShot") == 0){
		hp -= 0.25f;
	}

	else if (obj->getType().compare("ultiShot") == 0){
		hp -= 0.25f;
	}

	if(hp <= 0){
		killedBy = obj->getType();
	}
}

void Boss::CollisionReaction(BaseObject* obj){
	if (obj->getType().compare("ultiShot") == 0){
		hp -= 0.01f;
	}
	else if (obj->getType().compare("playerShot") == 0){
		hp -= 0.01f;
		obj->hp -= 1;
	}
	else if (obj->getType().compare("player") != 0){
		obj->hp = 0;
	}
}

void Boarder::CollisionReaction(BaseObject* obj){

}

void Shallows::CollisionReaction(BaseObject* obj){

}

void Background::CollisionReaction(BaseObject* obj){

}

void Explosion::CollisionReaction(BaseObject* obj){

}

void Scrap::CollisionReaction(BaseObject* obj){
	hp -= 1;
}

void Player::drawHealth(float health) {
	glPushMatrix();
	glTranslatef(120, -70, 0.0);
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(5, 0);
	glVertex2f(5, health * 10);
	glVertex2f(0, health * 10);
	glEnd();
	glPopMatrix();
}

void Shot::Render(){
	glLoadIdentity();
	//draw Shot
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslatef(Xtri, Ytri, 0.0);
	glRotatef(heading, 0, 0, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixOBB);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);	glVertex2f(-1 * scale, -2 * scale);
	glTexCoord2f(0.0, 1);	glVertex2f(-1 * scale, 2 * scale);
	glTexCoord2f(1, 1);		glVertex2f(1 * scale, 2 * scale);
	glTexCoord2f(1, 0.0);	glVertex2f(1 * scale, -2 * scale);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	double PI = 3.1415926535897932384626433832795;
	Xtri += Vtri*cos((90 + heading)*(PI / 180.0f));
	Ytri += Vtri*sin((90 + heading)*(PI / 180.0f));

	hp -= 0.025;
}

void Player::Render(){
	if (count > 0){
		count--;
	}
	if (i>=4){
		i = 0;
	}
	glLoadIdentity();
	//draw player
	glPushMatrix();
	if (Vtri < -0.01){
		glBindTexture(GL_TEXTURE_2D, animation[i + 5]);
	}
	else if (Vtri > 0.01){
		glBindTexture(GL_TEXTURE_2D, animation[i]);
	}
	else{
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	

	glTranslatef(Xtri, Ytri, 0.0);
	glRotatef(heading, 0, 0, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixOBB);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);	glVertex2f(-origX, -origY);
		glTexCoord2f(0.0, 1);	glVertex2f(-origX, origY);
		glTexCoord2f(1, 1);		glVertex2f(origX, origY);
		glTexCoord2f(1, 0.0);	glVertex2f(origX, -origY);
		glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	double PI = 3.1415926535897932384626433832795;
	Xtri += Vtri*cos((90 + heading)*(PI / 180.0f));
	Ytri += Vtri*sin((90 + heading)*(PI / 180.0f));

	if (frame == 5){
		i++;
	}
	if (frame > 5){
		frame = 0;
	}
	frame++;
}

void Enemy::Render(){
	if (!spotted){
		Vtri = 0.2f;
	}
	if (count > 0){
		count--;
	}
	if (Vtri>0){
		Vtri -= 0.01;
	}
	if (i >= 4){
		i = 0;
	}
	glLoadIdentity();
	//draw enemy
	glPushMatrix();
	
	if (hp>0.3){
		if (Vtri < -0.01){
			glBindTexture(GL_TEXTURE_2D, animation[i + 6]);
		}
		else if (Vtri > 0.01){
			glBindTexture(GL_TEXTURE_2D, animation[i + 1]);
		}
		else{
			glBindTexture(GL_TEXTURE_2D, animation[0]);
		}
	}
	else{
		if (Vtri < -0.01){
			glBindTexture(GL_TEXTURE_2D, damage[i + 6]);
		}
		else if (Vtri > 0.01){
			glBindTexture(GL_TEXTURE_2D, damage[i + 1]);
		}
		else{
			glBindTexture(GL_TEXTURE_2D, damage[0]);
		}
	}
	
	glTranslatef(Xtri, Ytri, 0.0);
	glRotatef(heading, 0, 0, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixOBB);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);	glVertex2f(-origX, -origY);
		glTexCoord2f(0.0, 1);	glVertex2f(-origX, origY);
		glTexCoord2f(1, 1);		glVertex2f(origX, origY);
		glTexCoord2f(1, 0.0);	glVertex2f(origX, -origY);
		glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Draw Enemy Health Bar
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(Xtri-2.5, Ytri-8, 0.0);
		glBegin(GL_QUADS);
		glColor3f(1, 0, 0);
		glVertex2f(0, 0);
		glVertex2f(hp * 5, 0);
		glVertex2f(hp * 5, 0.5);
		glVertex2f(0, 0.5);
		glEnd();
	glPopMatrix();

	double PI = 3.1415926535897932384626433832795;
	Xtri += Vtri*cos((90 + heading)*(PI / 180.0f));
	Ytri += Vtri*sin((90 + heading)*(PI / 180.0f));

	if (frame == 5){
		i++;
	}
	if (frame > 5){
		frame = 0;
	}
	frame++;
}

void Boss::Render(){
	if (count > 0){
		count--;
	}
	if (Vtri>0){
		Vtri -= 0.01;
	}
	if (i >= 4){
		i = 0;
	}
	glLoadIdentity();
	//draw enemy
	glPushMatrix();

	if (Vtri > 0.01){
		glBindTexture(GL_TEXTURE_2D, animation[i + 1]);
	}
	else{
		glBindTexture(GL_TEXTURE_2D, animation[0]);
	}

	glTranslatef(Xtri, Ytri, 0.0);
	glRotatef(heading, 0, 0, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixOBB);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);	glVertex2f(-origX, -origY);
	glTexCoord2f(0.0, 1);	glVertex2f(-origX, origY);
	glTexCoord2f(1, 1);		glVertex2f(origX, origY);
	glTexCoord2f(1, 0.0);	glVertex2f(origX, -origY);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Draw boss Health Bar
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(Xtri - 2.5, Ytri - 30, 0.0);
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(hp * 5, 0);
	glVertex2f(hp * 5, 1);
	glVertex2f(0, 1);
	glEnd();
	glPopMatrix();

	double PI = 3.1415926535897932384626433832795;
	Xtri += Vtri*cos((90 + heading)*(PI / 180.0f));
	Ytri += Vtri*sin((90 + heading)*(PI / 180.0f));

	if (frame == 5){
		i++;
	}
	if (frame > 5){
		frame = 0;
	}
	frame++;
}

void Explosion::Render(){
	glLoadIdentity();
	//draw Explosion
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, explosion[i]);

	glTranslatef(origX, origY, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixOBB);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);	glVertex2f(-sizeX, -sizeY);
		glTexCoord2f(0.0, 1);	glVertex2f(-sizeX, sizeY);
		glTexCoord2f(1, 1);		glVertex2f(sizeX, sizeY);
		glTexCoord2f(1, 0.0);	glVertex2f(sizeX, -sizeY);
		glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	hp--;
	i++;
}

void Background::Render(){
	glLoadIdentity();
	//draw Background
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslatef(0.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixOBB);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);			glVertex2f(-origX, -origY);
		glTexCoord2f(0.0, repeatY);		glVertex2f(-origX, origY);
		glTexCoord2f(repeatX, repeatY); glVertex2f(origX, origY);
		glTexCoord2f(repeatX, 0.0);		glVertex2f(origX, -origY);
		glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Boarder::Render(){
	glLoadIdentity();
	//Draw Boarder
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslatef(Xtri, Ytri, 0.0);
	glRotatef(angle, 0, 0, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixOBB);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);		glVertex2f(-sizex, -sizey);
	glTexCoord2f(0.0, vert1);	glVertex2f(-sizex, sizey);
	glTexCoord2f(vert2, vert1);	glVertex2f(sizex, sizey);
	glTexCoord2f(vert2, 0.0);	glVertex2f(sizex, -sizey);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

void Shallows::Render(){
	glLoadIdentity();
	//Draw Boarder
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslatef(Xtri, Ytri, 0.0);
	glRotatef(heading, 0, 0, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixOBB);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);	glVertex2f(-sizex, -sizey);
	glTexCoord2f(0.0, 1);	glVertex2f(-sizex, sizey);
	glTexCoord2f(1, 1);		glVertex2f(sizex, sizey);
	glTexCoord2f(1, 0.0);	glVertex2f(sizex, -sizey);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Scrap::Render(){
	if (size>2.5){
		spotted = true;
	}
	else if (size<2){
		spotted = false;
	}

	if (spotted){
		size -= 0.01;
	}
	else{
		size += 0.01;
	}

	glLoadIdentity();
	//draw Scrap
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslatef(origX, origY, 0.0);
	glRotatef(heading, 0, 0, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixOBB);
	glColor3f(1, 1, 1);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);	glVertex2f(-size, -size);
	glTexCoord2f(0.0, 1);	glVertex2f(-size, size);
	glTexCoord2f(1, 1);		glVertex2f(size, size);
	glTexCoord2f(1, 0.0);	glVertex2f(size, -size);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	double PI = 3.1415926535897932384626433832795;
	origX += Vtri*cos((90 + heading)*(PI / 180.0f));
	origY += Vtri*sin((90 + heading)*(PI / 180.0f));
}