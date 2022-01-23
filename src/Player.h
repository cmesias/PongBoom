/*
 * Player.h
 *
 *  Created on: May 8, 2017
 *      Author: Carl
 */



// Player class
class Player {
public:
	// Default constructor
	Player();

	// Set Player position
	void setPosition(int spawnX, int spawnY);

	// Set Player position
	void setX(int spawnX);

	// Set Player position
	void setY(int spawnY);

	// Get Player x position
	float getX();

	// Get Player y position
	float getY();

	// Get Player left side
	int getLeftSide();

	// Get Player right side
	int getRightSide();

	// Get Player top side
	int getTopSide();

	// Get Player bottom side
	int getBottomSide();

	// Get Player width
	int getWidth();

	// Get Player width
	int getHeight();

	// Get Player y speed
	float getSpeedY();

	// Update Player
	void update();

	// Draw Player
	void render(SDL_Renderer *gRenderer);

	// Move Player up
	bool moveUp;

	// Move Player down
	bool moveDown;

	// Player score
	int score;

	// Player y speed
	float speedY;

public: // abilities

	bool shoot;
	float shootTimer;

private:
	float x;
	float y;
	int w;
	int h;
};
