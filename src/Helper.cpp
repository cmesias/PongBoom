/*
 * Helper.cpp
 *
 *  Created on: May 18, 2017
 *      Author: Carl
 */

#include <iostream>
#include <sstream>
#include <algorithm>    // std::max
#include <stdlib.h>

#include "Helper.h"

/*void Helper::RenderInstances(SDL_Renderer *gRenderer, LTexture &gTexture,
						 int maxInstances,
						 float x, float y, float w, float h, float alpha, SDL_Rect clip,
						 float camx, float camy) {
	for( int i = 0; i < maxInstances; ++i ) {
		gTexture.setAlpha(alpha);
		gTexture.render(gRenderer, x - camx, y - camy, w, h, &clip);
	}
}*/

bool Helper::checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2) {
	bool collide;
	if (x+w > x2 && x < x2 + w2 && y+h > y2 && y < y2 + h2) {
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}

bool Helper::checkCollisionRect( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA,   leftB;
    int rightA,  rightB;
    int topA, 	 topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA 	= a.x;
    rightA 	= a.x + a.w;
    topA 	= a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB 	= b.x;
    rightB 	= b.x + b.w;
    topB 	= b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA < topB )
    {
        return false;
    }

    if( topA > bottomB )
    {
        return false;
    }

    if( rightA < leftB )
    {
        return false;
    }

    if( leftA > rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

// Generate a random double number
double Helper::randDouble(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

// Set Texture clips
void Helper::setClips(SDL_Rect &tempRect, int x, int y, int w, int h){
	tempRect.x = x;
	tempRect.y = y;
	tempRect.w = w;
	tempRect.h = h;
}


// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool Helper::onSegment(Point p, Point q, Point r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
       return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int Helper::orientation(Point p, Point q, Point r)
{
    // See http://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool Helper::doIntersect(Point p1, Point q1, Point p2, Point q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


void Helper::renderDialogText(SDL_Renderer *gRenderer,
					  std::string speakerName, std::string speakerText, std::string indicator,
					  float speakerX, float speakerY, float speakerW, float speakerH,
					  float dialogueX, float dialogueY, float dialogueW, float dialogueH,
					  SDL_Color speakerNameColor, SDL_Color speakerTextColor,
					  SDL_Color colorBG, SDL_Color colorBorder,
					  SDL_Color colorBGSpeaker, SDL_Color colorBorderSpeaker,
					  TTF_Font *gFontName, TTF_Font *gFontDialog, LTexture &gText,
					  Uint32 wrapLength, bool drawBorders){


	// Speaker Name
	std::stringstream tempss;
	tempss.str(std::string());
	tempss << speakerName;
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), speakerNameColor, gFontName);
	int newWidth = gText.getWidth()/4;
	int newHeight = gText.getHeight()/4;
		speakerW = newWidth + 8;
		speakerH = newHeight + 4;
		speakerX = dialogueX;
		speakerY = dialogueY - newHeight - 6;

		// Draw boxes with SDL? (false if we are drawing our own
	if (drawBorders) {
		// Speaker name dialogue box
		SDL_Rect tempRect = {speakerX, speakerY, speakerW, speakerH};
		SDL_SetRenderDrawColor(gRenderer, colorBGSpeaker.r, colorBGSpeaker.g, colorBGSpeaker.b, 255);
		SDL_RenderFillRect(gRenderer, &tempRect);
		SDL_SetRenderDrawColor(gRenderer, colorBorderSpeaker.r, colorBorderSpeaker.g, colorBorderSpeaker.b, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);

		// Dialogue text boxes
		SDL_Rect tempr = {dialogueX, dialogueY, dialogueW, dialogueH};
		SDL_SetRenderDrawColor(gRenderer, colorBG.r, colorBG.g, colorBG.b, 255);
		SDL_RenderFillRect(gRenderer, &tempr);
		SDL_SetRenderDrawColor(gRenderer, colorBorder.r, colorBorder.g, colorBorder.b, 255);
		SDL_RenderDrawRect(gRenderer, &tempr);
	}

	// Render speaker name
	gText.render(gRenderer, speakerX + speakerW/2-newWidth/2,
							speakerY + speakerH/2-newHeight/2,
							newWidth, newHeight);

	// Dialogue Text
	tempss.str(std::string());
	tempss << speakerText;
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), speakerTextColor, gFontDialog, wrapLength);
		newWidth = gText.getWidth()/4;
		newHeight = gText.getHeight()/4;

	// Render speaker dialogue
	gText.render(gRenderer, dialogueX+6, dialogueY+5, newWidth, newHeight);

	// Indicator
	tempss.str(std::string());
	tempss << indicator;
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), speakerTextColor, gFontDialog, wrapLength);
		newWidth = gText.getWidth()/4;
		newHeight = gText.getHeight()/4;

	// Render ">" indicator
	gText.render(gRenderer, dialogueX + dialogueW - newWidth - 3,
						    dialogueY + dialogueH - newHeight - 2,
							newWidth, newHeight);
}

void Helper::renderStatusBar(SDL_Renderer *gRenderer, float x, float y, float w, float h,
		 	 	 	 float yDistanceFromTarget,
					 double health, double healthDecay, double maxHealth,
					 SDL_Color bgColor, SDL_Color decayColor,
					 SDL_Color healthColor, SDL_Color borderColor,
					 bool border){

	SDL_Rect healthRect;
	healthRect.x = x-w/2;
	healthRect.y = y-h-yDistanceFromTarget;
	healthRect.h = h;

	/* Healthbar Background */
	healthRect.w = ((w*maxHealth)/maxHealth);
	SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
	SDL_RenderFillRect(gRenderer, &healthRect);

	/* Healthbar Decay */
	healthRect.w = (w*healthDecay)/maxHealth;
	SDL_SetRenderDrawColor(gRenderer, decayColor.r, decayColor.g, decayColor.b, 255);
	SDL_RenderFillRect(gRenderer, &healthRect);

	/* Healthbar current Health */
	healthRect.w = ((w*health)/maxHealth);
	SDL_SetRenderDrawColor(gRenderer, healthColor.r, healthColor.g, healthColor.b, 255);
	SDL_RenderFillRect(gRenderer, &healthRect);

	/* Healthbar border */
	if (border) {
		healthRect.w = ((w*maxHealth)/maxHealth);
		SDL_SetRenderDrawColor(gRenderer, borderColor.r, borderColor.g, borderColor.b, 255);
		SDL_RenderDrawRect(gRenderer, &healthRect);
	}
}
