/*
 * Helper.h
 *
 *  Created on: May 18, 2017
 *      Author: Carl
 */

#ifndef LOCAL_HELPER_H_
#define LOCAL_HELPER_H_

#include "Timers.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.h"

class Helper {
public:
    Timer fps;

    // Define Infinite (Using INT_MAX caused overflow problems)
    #define INF 10000

    struct Point
    {
        int x;
        int y;
    };

    struct PointEr
    {
        float *x;
        float *y;
        int id;
    };
public:
    // fps
    bool cap 					= true;
	int frame 					= 0;
    const int FRAMES_PER_SECOND = 60;
	//const int screenWidth = 2880;
	//const int screenHeight = 1620;
    //const int screenWidth = 2304;
    //const int screenHeight = 1296;
	//const int screenWidth = 480;
	//const int screenHeight = 270;
	//const int screenWidth = 1280;
	//const int screenHeight = 720;
	const int screenWidth = 270;
	const int screenHeight = 152;
	//const int screenWidth = 640;
	//const int screenHeight = 360;
public:

	// Render instances from a class
	/*void RenderInstances(SDL_Renderer *gRenderer, LTexture &gTexture,
					 int maxInstances,
					 float x, float y, float w, float h, float alpha, SDL_Rect clip,
					 float camx, float camy);*/

	// Check sollision
	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	// Generate a random double number
	double randDouble(double fMin, double fMax);

	// Set Texture clips
	void setClips(SDL_Rect &tempRect, int x, int y, int w, int h);


public:	// Check if 2 line segments intersect

	bool onSegment(Point p, Point q, Point r);

	int orientation(Point p, Point q, Point r);

	bool doIntersect(Point p1, Point q1, Point p2, Point q2);

public:	// Render dialogue text, and render status bars such as health bars

	// Render dialog
	void renderDialogText(SDL_Renderer *gRenderer,
						  std::string speakerName, std::string speakerText, std::string indicator,
						  float dialogueX, float dialogueY, float dialogueW, float dialogueH,
						  float speakerX, float speakerY, float speakerW, float speakerH,
						  SDL_Color speakerNameColor, SDL_Color speakerTextColor,
						  SDL_Color colorBG, SDL_Color colorBorder,
						  SDL_Color colorBGSpeaker, SDL_Color colorBorderSpeaker,
						  TTF_Font *gFontName, TTF_Font *gFontDialog, LTexture &gText,
						  Uint32 wrapLength, bool drawBorders = false);

	// Render status bar
	void renderStatusBar(SDL_Renderer *gRenderer, float x, float y, float w, float h,
						 float yDistanceFromTarget,
						 double health, double healthDecay, double maxHealth,
						 SDL_Color bgColor, SDL_Color decayColor,
						 SDL_Color healthColor, SDL_Color borderColor,
						 bool border = false);

};

#endif /* LOCAL_HELPER_H_ */
