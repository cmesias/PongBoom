/*
 * Particless.h
 *
 *  Created on: Dec 16, 2016
 *      Author: Carl
 */



#ifndef LOCAL_PARTICLE_H_
#define LOCAL_PARTICLE_H_

#include "Helper.h"
#include "LTexture.h"

class Particle: public Helper {
public:	// other variables
	int ptimer = 0;
	int count;
	//const int max = 1024;
	const int max = 1000;
	LTexture gParticles;					// Particle Textures
	SDL_Rect cParticles[ 2 ];				// [0: Blue], [1: Green], [2: Orange], [3: Red], [4: White], [5: Yellow] Particle

public:	// variables
	float x2, y2;							// particle center
	int radius;								// particle radius
	float x, y;
	float w, h;
	double alpha;
	double alphaspeed;
	double time;							// Current tick
	double deathTimer;						// Max ticks before particle death
	double deathTimerSpeed;					// Death tick rate
	float angle;
	double vX, vY;
	double speed;
	double angleSpe, angleDir;
	bool onScreen;
	bool alive;
	bool collide;
	std::string side;
	/* 0: player particle, does damage
	 * 1: enemies particle, does damage
	 * 2: stars, no damage
	 * 3: grenade, does damage
	 * 3: enemy particle II, does damage
	 */
	int type;
	std::string tag;		// who the owner of this particle is
	float damage;
	SDL_Color color;
	int layer;

	// Death by size
	bool sizeDeath;
	float deathSpe;

	// Trail of particles
	bool trail;
	float trailTimer;
	float trailRate;
	SDL_Color trailColor;
	float trailMinSize;
	float trailMaxSize;
	float timerBeforeMoving;		// when it hits 0, it will move
	bool goTowardsTarget;			// If true, after timerBeforeMoving is over, the Particles will move towards the target
	bool playSFXBeforeMoving;		// After waiting to move, should particle play a sound (i.e.: make a swoosh sound)
	int bounces;					// amount of times a particle may bounce before being destroyed
public:	// SAT theorem for collision during rotation's

	/* Frigate Corners
	 * A: Top Right
	 * B: Bottom Right
	 * C: Top Left
	 * D: Bottom Left
	 * */
	Point A, B, C, D;

public:
	bool decay;				// decay particle speed?
	float decaySpeed;		// how much decay are we doing?
	int timeri;				// timer that may be used for anything SPECIFICALLY for something (i.e.: this particle spawns out more particles such as a smoke)

public:	// basic functions
	void init(Particle particle[]);
	void Remove(Particle particle[], int i);
	void RemoveAll(Particle particle[]);
	void load(SDL_Renderer* gRenderer);
	void free();

public:	// functions
	void spawnParticleAngle(Particle particle[], std::string tag, int type,
			float spawnX, float spawnY,
			float spawnW, float spawnH,
			double angle, double speed,
			double damage,
			SDL_Color color, int layer,
			int angleSpe, int angleDir,
			double alpha, double alphaspeed,
			double deathTimer, double deathTimerSpeed,
			bool sizeDeath = false, float deathSpe = 0.0,
			bool decay = false, float decaySpeed = 0.0,
			bool trail = false, float trailRate = 0.0, SDL_Color trailColor = {0, 255, 0},
			float trailMinSize = 0.0, float trailMaxSize = 0.0,
			float timerBeforeMoving = 0.0,
			bool goTowardsTarget = false, float targetXe = 0.0, float targetYe = 0.0,
			bool playSFXBeforeMoving = false, int bounces = 0);
	/*void fireParticle(Particle particle[], int type, int damage,
										   int spawnx, int spawny, int w, int h,
										   double speed, float vX, float vY,
										   int alphaspeed, int alpha,
										   int deathTimer, int deathTimerSpeed, double angle, double angleSpe, double angleDir,
										   SDL_Color color, int layer);
	void fireParticle2(Particle particle[], int type, int damage, int spawnx, int spawny, int w, int h, int targetx,int targety,
											int alphaspeed, int alpha,
											int deathTimer, int deathTimerSpeed,
											double speed,
											SDL_Color color, int layer);*/

	// Update every particle
	void Update(Particle particle[], int mapX, int mapY, int mapW, int mapH, float camx, float camy);

	// Update Bullet Particles
	void updateBulletParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH);

	// Render Particles
	void Render(SDL_Renderer* gRenderer, Particle particle[], int camX, int camY);

public:	// Function extensions

	// Spawn an explosion
	void SpawnExplosion(Particle particle[], float x, float y, SDL_Color explosionColor);
	void SpawnFireExplosion(Particle particle[], float x, float y, SDL_Color explosionColor);
	void SpawnTrail(Particle particle[], float x, float y, SDL_Color explosionColor);
	void SpawnFireTrail(Particle particle[], float x, float y, SDL_Color explosionColor);

public:	// Star
	void genStars(Particle particle[], int startX, int startYy, int endW, int endH);
	void updateStarParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH);
	void renderStarParticle(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer);
};

#endif /* LOCAL_PARTICLE_H_ */
