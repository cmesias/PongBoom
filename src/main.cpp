
#include <iostream>
#include <sstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "Particle.h"
#include "Player.h"


// Create window dimensions
int mWidth;
int mHeight;

// Create pointer for events
SDL_Event events;

// Create window
SDL_Window *mWindow = NULL;

// Create renderer
SDL_Renderer *gRenderer;

// Create player hitting pong sound effect
Mix_Chunk *sPongPlayer = NULL;

// Create pong hitting screen sound effect
Mix_Chunk *sPongScreen = NULL;

// Create pong scoring
Mix_Chunk *sPongScore = NULL;

// Create player hurt
Mix_Chunk *sPlayerHurt = NULL;

// Create shoot audio
Mix_Chunk *sShoot = NULL;

// Create font variable
TTF_Font* viga = NULL;

// Create color
SDL_Color white = {255, 255, 255};

// Create Surface to put font text on
SDL_Surface *surface1 = NULL;

// Create Texture from a Surface
SDL_Texture *texture1 = NULL;

LTexture gText;

// Generate a random double number
double randDouble(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

bool checkCollision(float x, float y, int w, int h, float x2, float y2, int w2, int h2) {
	if (x+w > x2 && x < x2+w2 && y+h > y2 && y < y2+h2) {
		return true;
	}else {
		return false;
	}
}

// Initialize SDL
void initSDL() {
	// Initialize Video
	SDL_Init( SDL_INIT_VIDEO ); // initializes the subsystems specified, in this video

	// Initialize font loading
	TTF_Init();

	// Initialize audio loading
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	// Declare window dimensions
	mWidth = 640;
	mHeight = 360;

	// Declare window
	mWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_SHOWN);

	// Declare renderer
	gRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

// Load media
void loadMedia() {

	// load audio
	sPongPlayer = Mix_LoadWAV("pong-player.wav");
	sPongScreen = Mix_LoadWAV("pong-screen.wav");
	sPongScore = Mix_LoadWAV("pong-score.wav");
	sPlayerHurt = Mix_LoadWAV("player-hurt.wav");
	sShoot = Mix_LoadWAV("player-shoot.wav");

	// load fonts
	viga = TTF_OpenFont("viga-regular.ttf", 24); //this opens a font style and sets a size

}

// Free SDL
void freeSDL() {
	// Free texture
	SDL_DestroyTexture(texture1);

	// Free surface
	SDL_FreeSurface(surface1);
	gText.free();

	// Free audio
	Mix_FreeChunk(sPongPlayer);
	Mix_FreeChunk(sPongScreen);
	Mix_FreeChunk(sPongScore);
	Mix_FreeChunk(sPlayerHurt);
	Mix_FreeChunk(sShoot);

	// Close upon exit
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(mWindow);

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}

void addPlayerScore(int &pongX, int pongW, int &score, int &yDir) {
	// center pong
	pongX = 640/2 - pongW/2;

	// Add score
	score++;

	// Set random pong y speed
	int randNum = rand() % 2;
	if (randNum == 0)
		yDir = (rand() % 5 + 1) * 0.1;
	else
		yDir = -(rand() % 5 + 1) * 0.1;
}

int main(int argc, char *argv[]) {

	// Make random actually random
	srand(time(0));

	// Initialize SDL
	initSDL();

	// Load media
	loadMedia();

	// Game loop
	bool quit = false;

	// Particles
	Particle part;
	Particle particles[5000];
	part.init(particles);
	part.load(gRenderer);

	// Create Player 1
	Player p1;

	// Create Player 2
	Player p2;

	// Timer
    Timer fps;
    bool cap 					= true;
	int frame 					= 0;
    const int FRAMES_PER_SECOND = 60;

	// Set Player 1 position
	p1.setPosition(64, 360/2-112/2);

	// Set Player 2 position
	p2.setPosition(mWidth - p2.getWidth() - 64, 360/2-112/2);

	// Create pong
	SDL_Rect pong = {mWidth/2 - 6, mHeight/2 - 6, 12, 12};

	// Side that pong 1 is on from player 1
	std::string side1 = "right";

	// Side that pong is on from player 2
	std::string side2 = "left";

	// Pong speed
	float speedX = 5.0;
	float speedY = 5.0;

	// Pong direction
	float xDir = 1;
	float yDir = 0;

	// game booleans
	bool startgame = false;
	bool paused = false;
	bool winner = false;
	bool firepong = false;
	float fireTimer = 0;
	float fireRate = 15;

	// Game loop
	while (!quit) {

		// reset Player's positions
		p1.setX(64);

		// Start FPS cap
		fps.start();

		int mx, my;
		SDL_GetMouseState(&mx, &my);

		// Handle Events
		while (SDL_PollEvent(&events)) {

			// Handle window quit events
			if (events.type == SDL_QUIT) {
				quit = true;
			}

			// Handle window input events
			else{

				// Key Pressed
				if (events.type == SDL_KEYDOWN && events.key.repeat == 0) {

					// Switch case of all possible keyboard presses
					switch (events.key.keysym.sym) {
						case SDLK_w:	// If W Pressed, set Player moveUp to true
							p1.moveUp = true;
							break;
						case SDLK_s	:	// If S Pressed, set Player moveUp to true
							p1.moveDown = true;
							break;
						case SDLK_UP:	// If W Pressed, set Player moveUp to true
							p2.moveUp = true;
							break;
						case SDLK_DOWN:	// If S Pressed, set Player moveUp to true
							p2.moveDown = true;
							break;
						case SDLK_ESCAPE:	// pause game
							paused = (!paused);
							break;
						case SDLK_SPACE:	// start game
							if (!startgame) {
								startgame = true;
								paused = false;
								// reset variables
								p1.score = 0;
								p2.score = 0;
								winner = false;
							}
							break;
						case SDLK_d:	// Player 1, shoot
							if (startgame) {
								p1.shoot = true;
								p1.shootTimer = 60 * 5;
							}
							break;
					}
				}

				// Key Released
				else if (events.type == SDL_KEYUP && events.key.repeat == 0) {

					// Switch case of all possible keyboard presses
					switch (events.key.keysym.sym) {
						case SDLK_w:	// If W Released, set Player moveUp to false
							p1.moveUp = false;
							break;
						case SDLK_s:	// If S Released, set Player moveDown to false
							p1.moveDown = false;
							break;
						case SDLK_UP:	// If W Released, set Player moveUp to true
							p2.moveUp = false;
							break;
						case SDLK_DOWN:	// If S Released, set Player moveUp to true
							p2.moveDown = false;
							break;
					}
				}
			}
		}


		if (startgame && !paused && !winner) {

			// Fire-pong
			if (firepong) {
				// pong particle effects
				fireTimer += 60;
				if (fireTimer > 60) {
					fireTimer = 0;

					// spawn explosion
					part.SpawnFireTrail(particles, pong.x+pong.w/2, pong.y+pong.h/2, {244,144,20});
				}
			}
			// Regular pong
			else{
				// pong particle effects
				fireTimer += 60;
				if (fireTimer > 60) {
					fireTimer = 0;

					// spawn explosion
					part.SpawnTrail(particles, pong.x+pong.w/2, pong.y+pong.h/2, white);
				}
			}

			// Update Players
			p1.update();
			p2.update();

			// Update Player's, manually
			if (p1.shoot) {
				if (p1.shootTimer > 0) {
					p1.shootTimer -= 1;
					// shoot
					for (int i=0; i <= 60*5; i+= 8) {
						if (i == int(p1.shootTimer)) {
							int randPosition = rand() % p1.getHeight();
							int randSetBack = rand() % 3 + 1;
							p1.setX( p1.getX() - randSetBack );
							float newX = p1.getX() + p1.getWidth() + 4;
							float newY = p1.getY() + randPosition - 2;
							part.spawnParticleAngle(particles, "slow", 4,
											   newX, newY,
											   4, 4,
											   0.0, randDouble(15.9, 19.99),
											   0.0,
											   {200,200,100}, 1,
											   1, 1,
											   255, 0,
											   rand() % 30 + 90, 1,
											   false, 0.11,
											   false, 0.11);
							// play sfx
							Mix_PlayChannel(-1, sShoot, false);
							// spawn explosion
							part.SpawnExplosion(particles, newX, newY, {200,200,100});
						}
					}
				}else{
					p1.shoot = false;
				}
			}

			// Particle collision player 2, slow down Player 2
			for (int i = 0; i < part.max; i++) {
				if (particles[i].alive && particles[i].tag == "slow") {
					if (checkCollision(particles[i].x, particles[i].y, particles[i].w, particles[i].h, p2.getX(), p2.getY(), p2.getWidth(), p2.getHeight()) ) {
						p2.speedY = p2.speedY - p2.speedY * 1.3;
						particles[i].alive = false;
						part.count--;
					}
				}
			}

			// Side Player 1 is on from pong
			if (p1.getRightSide() < pong.x) {
				side1 = "right";
			}
			if (p1.getBottomSide() < pong.y) {
				side1 = "down";
			}
			if (p1.getLeftSide() > pong.x+pong.w) {
				side1 = "left";
			}
			if (p1.getTopSide() > pong.y+pong.h) {
				side1 = "up";
			}

			// Side Player 2 is on from pong
			if (p2.getRightSide() < pong.x) {
				side2 = "right";
			}
			if (p2.getBottomSide() < pong.y) {
				side2 = "down";
			}
			if (p2.getLeftSide() > pong.x+pong.w) {
				side2 = "left";
			}
			if (p2.getTopSide() > pong.y+pong.h) {
				side2 = "up";
			}

			// Bounce on Player 1
			if (pong.x + pong.w > p1.getX() && pong.x < p1.getX() + p1.getWidth() &&
				pong.y + pong.h > p1.getY() && pong.y < p1.getY() + p1.getHeight()) {

				/// Fire-pong
				if (firepong) {

					// spawn explosion
					part.SpawnFireExplosion(particles, pong.x+pong.w, pong.y+pong.h, {200,200,100});

					// center pong
					pong.x = 640/2 - pong.w/2;
					speedX = 5.4;
					xDir = 1;

					// Add score
					p2.score++;

					// Set random pong y speed
					int randNum = rand() % 2;
					if (randNum == 0)
						yDir = (rand() % 2 + 2) * 0.1;
					else
						yDir = -(rand() % 2 + 2) * 0.1;

					// Play sound effect
					Mix_PlayChannel(-1, sPongScore, false);
				}
				/// Regular pong
				else{

					// spawn explosion
					part.SpawnExplosion(particles, pong.x+pong.w, pong.y+pong.h, white);

					// Do collision stuff
					if (side1 == "up") {
						pong.y = p1.getTopSide()-pong.h;
						yDir = yDir * -1;
					}
					else if (side1 == "down") {
						pong.y = p1.getBottomSide();
						yDir = yDir * -1;
					}else {

						// Bounce pong
						xDir = xDir * -1;

						// Determine pong y direction bounce
						if (p1.moveUp) {
							yDir = -(rand() % 5 + 1) * 0.1;
						}else if (p1.moveDown) {
							yDir = (rand() % 5 + 1) * 0.1;
						}else {
							// Set random pong y speed
							/*int randNum = rand() % 2;
							if (randNum == 0)
								yDir = (rand() % 5 + 1) * 0.1;
							else
								yDir = -(rand() % 5 + 1) * 0.1;*/
						}
					}

					// Play sound effect
					Mix_PlayChannel(-1, sPongPlayer, false);
				}
			}

			// Bounce on Player 2
			if (pong.x + pong.w > p2.getX() && pong.x < p2.getX() + p2.getWidth() &&
				pong.y + pong.h > p2.getY() && pong.y < p2.getY() + p2.getHeight()) {

				/// Fire-pong
				if (firepong) {

					// spawn explosion
					part.SpawnFireExplosion(particles, pong.x+pong.w, pong.y+pong.h, {200,200,100});

					// center pong
					pong.x = 640/2 - pong.w/2;
					speedX = 5.4;
					xDir = -1;

					// Add score
					p1.score++;

					// Set random pong y speed
					int randNum = rand() % 2;
					if (randNum == 0)
						yDir = (rand() % 2 + 2) * 0.1;
					else
						yDir = -(rand() % 2 + 2) * 0.1;

					// Play sound effect
					Mix_PlayChannel(-1, sPongScore, false);
				}
				/// Regular pong
				else{

					// spawn explosion
					part.SpawnExplosion(particles, pong.x+pong.w, pong.y+pong.h, white);

					// Do collision stuff
					if (side2 == "up") {
						pong.y = p2.getTopSide()-pong.h;
						yDir = yDir * -1;
					}
					else if (side2 == "down") {
						pong.y = p2.getBottomSide();
						yDir = yDir * -1;
					}else {

						// Bounce pong
						xDir = xDir * -1;

						// Determine pong y direction bounce
						if (p2.moveUp) {
							yDir = -(rand() % 5 + 1) * 0.1;
						}else if (p2.moveDown) {
							yDir = (rand() % 5 + 1) * 0.1;
						}else {
							// Set random pong y speed
							/*int randNum = rand() % 2;
							if (randNum == 0)
								yDir = (rand() % 3 + 3) * 0.1;
							else
								yDir = -(rand() % 3 + 3) * 0.1;*/
						}
					}

					// Play sound effect
					Mix_PlayChannel(-1, sPongPlayer, false);
				}
			}

			// Scoring on Player 1
			if (pong.x < 0) {
				xDir = xDir * -1;

				// if not fire-pong, bounce as usually and score
				if (!firepong) {

					// spawn explosion
					part.SpawnFireExplosion(particles, pong.x+pong.w, pong.y+pong.h, {200,200,100});

					// center pong
					pong.x = 640/2 - pong.w/2;

					// Add score
					p2.score++;

					// Set random pong y speed
					int randNum = rand() % 2;
					if (randNum == 0)
						yDir = (rand() % 2 + 2) * 0.1;
					else
						yDir = -(rand() % 2 + 2) * 0.1;

					// Play sound effect
					Mix_PlayChannel(-1, sPongScore, false);
				}
				// fire-pong mode, goes faster each hit
				else{

					// spawn explosion
					part.SpawnExplosion(particles, pong.x+pong.w, pong.y+pong.h, white);

					if (speedX < 30) {
						speedX += speedX * 0.2;
					}
					yDir += 0.2;

					// Play sound effect
					Mix_PlayChannel(-1, sPongScreen, false);
				}
			}

			// Scoring on Player 2
			if (pong.x + pong.w > 0 + mWidth) {
				xDir = xDir * -1;

				// if not fire-pong, bounce as usually and score
				if (!firepong) {

					// spawn explosion
					part.SpawnFireExplosion(particles, pong.x+pong.w, pong.y+pong.h, {200,200,100});

					// center pong
					pong.x = 640/2 - pong.w/2;

					// Add score
					p1.score++;

					// Set random pong y speed
					int randNum = rand() % 2;
					if (randNum == 0)
						yDir = (rand() % 5 + 1) * 0.1;
					else
						yDir = -(rand() % 5 + 1) * 0.1;

					// Play sound effect
					Mix_PlayChannel(-1, sPongScore, false);
				}
				// fire-pong mode, goes faster each hit
				else{

					// spawn explosion
					part.SpawnExplosion(particles, pong.x+pong.w, pong.y+pong.h, white);

					if (speedX < 30) {
						speedX += speedX * 0.2;
					}
					yDir += 0.2;

					// Play sound effect
					Mix_PlayChannel(-1, sPongScreen, false);
				}
			}

			// Bounce on top of screen
			if (pong.y < 0) {
				yDir = yDir * -1;

				// spawn explosion
				part.SpawnExplosion(particles, pong.x+pong.w, pong.y+pong.h, white);

				// Play sound effect
				Mix_PlayChannel(-1, sPongScreen, false);
			}

			// Bounce on bottom of screen
			if (pong.y + pong.h > 0 + mHeight) {
				yDir = yDir * -1;

				// spawn explosion
				part.SpawnExplosion(particles, pong.x+pong.w, pong.y+pong.h, white);

				// Play sound effect
				Mix_PlayChannel(-1, sPongScreen, false);
			}

			// Pong movement
			pong.x += speedX * xDir;
			pong.y += speedY * yDir;

			// Game over, reset game
			if (p1.score == 7 || p2.score == 7) {
				winner = true;
				startgame = false;
				paused = false;
				p1.setPosition(64, 360/2-112/2);
				p2.setPosition(mWidth - p2.getWidth() - 64, 360/2-112/2);
				pong.x = 640/2-pong.w/2;
				pong.y = 360/2-pong.h/2;
			}
		}

		// Update Particles
		part.Update(particles, 0, 0, mWidth, mHeight, 0, 0);
		part.updateStarParticles(particles, 0, 0, mWidth, mHeight);
		part.updateBulletParticles(particles, 0, 0, mWidth, mHeight);

		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			////// Render game objects //////

			// Render particles
			part.Render(gRenderer, particles, 0, 0);

			// Render pong
			if (firepong) {
				SDL_Rect tempRect = {pong.x, pong.y, pong.w, pong.h};
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
				SDL_RenderFillRect(gRenderer, &tempRect);
			}else{
				SDL_Rect tempRect = {pong.x, pong.y, pong.w, pong.h};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderFillRect(gRenderer, &tempRect);
			}

			// Render Player 1
			p1.render(gRenderer);

			// Render Player 2
			p2.render(gRenderer);

			////// Render GUI //////

			// Render score
			std::stringstream tempss;
			tempss << p1.score;
			gText.setAlpha(255);
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, viga);
			gText.render(gRenderer, 640/2-gText.getWidth()/2-100, 5,
									gText.getWidth(), gText.getHeight());
			tempss.str(std::string());
			tempss << p2.score;
			gText.setAlpha(255);
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, viga);
			gText.render(gRenderer, 640/2-gText.getWidth()/2+100, 5,
									gText.getWidth(), gText.getHeight());

			// Game start
			if (!startgame) {
				gText.setAlpha(255);
				gText.loadFromRenderedText(gRenderer, "Press Spacebar to Start", {255,255,255}, viga);
				gText.render(gRenderer, 640/2-gText.getWidth()/2, 360 - gText.getHeight()*2 - 20,
										gText.getWidth(), gText.getHeight());
			}else{
				// Paused
				if (paused) {
					gText.setAlpha(255);
					gText.loadFromRenderedText(gRenderer, "Paused", {255,255,255}, viga);
					gText.render(gRenderer, 640/2-gText.getWidth()/2, 360 - gText.getHeight() - 20,
											gText.getWidth(), gText.getHeight());
				}
			}

			// Winnder screen
			if (winner) {
				if (p1.score == 7) {
					std::stringstream tempss;
					tempss << "Winner!";
					gText.setAlpha(255);
					gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {60,60,255}, viga);
					gText.render(gRenderer, 640/2-gText.getWidth()/2-125, 48,
											gText.getWidth(), gText.getHeight());
				}
				if (p2.score == 7) {
					std::stringstream tempss;
					tempss << "Winner!";
					gText.setAlpha(255);
					gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,60,60}, viga);
					gText.render(gRenderer, 640/2-gText.getWidth()/2+125, 48,
											gText.getWidth(), gText.getHeight());
				}
			}

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		frame++;
		if((cap == true ) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND ))
			SDL_Delay((1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
	}

	// Free resources
	part.free();
	freeSDL();

	return 0;
}
