/*
 * Player.cpp
 *
 *  Created on: May 8, 2017
 *      Author: Carl
 */

#include <SDL2/SDL.h>
#include "Player.h"

// Default constructor
Player::Player() {
	x = 0;
	y = 0;
	w = 24;
	h = 112;
	score = 0;
	moveUp = false;
	moveDown = false;
	shoot = false;
	shootTimer = -1;
	speedY = 0.0;
}

// Set Player position
void Player::setPosition(int spawnX, int spawnY) {
	x = spawnX;
	y = spawnY;
}

// Set Player x position
void Player::setX(int spawnX) {
	x = spawnX;
}

// Set Player y position
void Player::setY(int spawnY) {
	y = spawnY;
}

// Return Player x position
float Player::getX() {
	return x;
}

// Return Player y position
float Player::getY() {
	return y;
}

// Get Player left side
int Player::getLeftSide() {
	return x;
}

// Get Player right side
int Player::getRightSide() {
	return x+w;
}

// Get Player top side
int Player::getTopSide() {
	return y;
}

// Get Player bottom side
int Player::getBottomSide() {
	return y+h;
}

// Return Player width
int Player::getWidth() {
	return w;
}

// Return Player height
int Player::getHeight() {
	return h;
}

// Return Player y speed
float Player::getSpeedY() {
	return speedY;
}

// Update Player
void Player::update() {
	// Move player up
	if (moveUp && speedY > -6) {
		speedY -= 2;
	}
	// Move player down
	else if (moveDown && speedY < 6) {
		speedY += 2;
	}

	// No movement, start decay
	if (!moveUp && !moveDown) {
		speedY = speedY - speedY * 0.25;
	}

	// Add movement
	y += speedY;

	// Player top of screen bound
	if (y < 0) {
		y = 0;
		speedY = 0.0;
	}
	// Player bottom of screen bound
	if (y + h > 360) {
		y = 360 - h;
		speedY = 0.0;
	}
}

// Draw Player
void Player::render(SDL_Renderer *gRenderer) {
	// Create rectangle
	SDL_Rect tempRect = {x, y, w, h};

	// Set rectangle color
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

	// Draw rectangle
	SDL_RenderFillRect(gRenderer, &tempRect);
}
