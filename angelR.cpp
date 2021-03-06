/*
 * Author: Angel Rojas
 * filename: angelR.cpp
 *
 *
 */
#include <iostream>
#include "Common.h"
#include "angelR.h"
#include "fonts.h"
#include "time.h"
#include <ctime>
#include <cstring>
using namespace std;

// Define some 'Magic Numbers', or other usable variables
static string POW = "Pow!";
static int YOFFSET = 31;
static int MOREYOFFSET = 41;
static int XOFFSET = 50;
static char FULLH[10] = "[====>]";
static char THREE4sH[10] = "[===>  ]";
static char HALFH[10] = "[==>   ]";
static char QUARTERH[10] = "[=>    ]";
static char EMPTYH[10] = "[       ]";
static char deadStr[50] = "You haven't lost all of your health...";
static int HEALTHOFFSET = 40;
static int HEALTHPOS = 11;
static int HALVED = 2;
static int xPosition = 0;
static int yPosition = 0;
#define blue 0x3b5998
#define lt_blue 0x87cefa
#define red 0x00ff0000
#define yellow 0x00ffff00
#define gold 0xc9ae59
#define navy_green 0x88b985

// Global Variables
int zombie_kills = 0;
int zombie_pos = 0;
static int next_level = 1;
static int wave_count = 1;
static int counter = 0;
bool Next = false;
int State = 0;
int Game_mode = 0;
static double my_timer = 0.0;

// enumerator of Health Bars.
enum HealthBar {
	FULL,
	THREE4s,
	HALF,
	QUARTER,
	EMPTY
};

// enumerator of Games' States.
enum HealthStates {
	HEALTHY,
	HITONCE,
	HITTWICE,
	HITTHRICE,
	DEAD
};

enum GameMode {
	MENU,
	PLAY,
	PAUSED,
	WIN,
	GAMEOVER,
	CREDITS,
	INSTR
};

// Function that actually displays player health bar.
void displayHealth(int input,int ypos, int xpos)
{
	int Meter = input;
	State++;
	switch (Meter) {
		//case FULL:
		case 0:
			Rect text;
			text.bot = ypos - 20;
			text.left = xpos / HEALTHPOS;
			text.center = 0;
			ggprint8b(&text, 16, yellow, "Health ");
			text.bot = ypos - 20;
			text.left = (xpos / HEALTHPOS) + HEALTHOFFSET;
			text.center = 0;
			ggprint8b(&text, 16, red, "%s", FULLH);
			break;
		//case THREE4s:
		case 1:
			Rect a;
			a.bot = ypos - 20;
			a.left = xpos / HEALTHPOS;
			a.center = 0;
			ggprint8b(&a,16,yellow,"Health ");
			a.bot = ypos - 20;
			a.left = (xpos / HEALTHPOS) + HEALTHOFFSET;
			a.center = 0;
			ggprint8b(&a, 16, red, "%s", THREE4sH);
			break;
		//case HALF:
		case 2:
			Rect b;
			b.bot = ypos - 20;
			b.left = xpos / HEALTHPOS;
			b.center = 0;
			ggprint8b(&b,16,yellow,"Health ");
			b.bot = ypos - 20;
			b.left = (xpos / HEALTHPOS) + HEALTHOFFSET;
			b.center = 0;
			ggprint8b(&b, 16, red, "%s", HALFH);
			break;
		//case QUARTER:
		case 3:
			Rect c;
			c.bot = ypos - 20;
			c.left = xpos / HEALTHPOS;
			c.center = 0;
			ggprint8b(&c,16,yellow,"Health ");
			c.bot = ypos - 20;
			c.left = (xpos / HEALTHPOS) + HEALTHOFFSET;
			c.center = 0;
			ggprint8b(&c, 16, red, "%s", QUARTERH);
			break;
		//case EMPTY:
		case 4:
			Rect d;
			d.bot = ypos - 20;
			d.left = xpos / HEALTHPOS;
			d.center = 0;
			ggprint8b(&d,16,yellow,"Health ");
			d.bot = ypos - 20;
			d.left = (xpos / HEALTHPOS) + HEALTHOFFSET;
			d.center = 0;
			ggprint8b(&d, 16, red, "%s", EMPTYH);
			break;
		default: 
			Rect t;
			t.bot = 870;
			t.left = 250;
			t.center = 0;
			ggprint8b(&t, 16, yellow, "Health ");
	}
}

// This func serves to decide which Healthbar to Render.
void playerState(int healthbar, int ypos, int xpos) 
{
	switch (healthbar) {
        //case HITONCE:
        case 1:
            displayHealth(THREE4s,ypos,xpos);
            break;
        //case HITTWICE:
        case 2:
            displayHealth(HALF,ypos,xpos);
            break;
        //case HITTHRICE:
        case 3:
            displayHealth(QUARTER,ypos,xpos);
            break;
        //case DEAD:
        case 4:
            displayHealth(EMPTY,ypos,xpos);
            break;

        default:
            displayHealth(FULL,ypos,xpos);
    }
}

void incrementWave()
{
	wave_count++;
	return;	
}

void resetWave()
{
	if (wave_count >= 2) {
		wave_count = 1;
	} else {
		wave_count = 1;
	}
}

// This function will display the Wave counter.
void displayWave(int ypos, int xpos)
{
	Rect text;
	text.bot = ypos - MOREYOFFSET;
	text.left = xpos;
	text.center = 0;
	ggprint8b(&text, 16, yellow, "Current Wave: %i", wave_count);
}

void printWelcome()
{
    cout << "Welcome, Player 1" << endl;
    return;
}

// Screen for a paused game.
void pauseGame(int xrespos, int yrespos)
{
	//Game_mode = PAUSED;
	Game_mode = 2;
	Rect paused;
	paused.bot = yrespos - 190;
	paused.left = xrespos/HALVED;
	paused.center = 1;
	ggprint8b(&paused, 16, lt_blue, "Game Paused.");
	ggprint8b(&paused, 16, lt_blue, "P - Play mode");
    return;
}

// Screen for a Game Over.
void gameOver(int xrespos, int yrespos)
{
	//Game_mode = END;
	glClearColor(0.0,0.0,0.0,0.8);
    	Game_mode = 4;
	Rect text;
	text.bot = yrespos - 190;
	text.left = xrespos/HALVED;
	text.center = 1;
	ggprint8b(&text, 16, red, "Game OVER!");
	ggprint8b(&text, 16, gold, "B - go Back, Try again sadface");
    return;
}

void powText()
{
    cout << POW << endl;
    return;
}

int incrementZombiesKilled(int &zombehs)
{
	zombehs = zombehs + 1;
	return zombehs;	
}

// 'changeBoolean()' will "flip" the value of any bool sent to it.
// Finally works correctly. '&' is important.
bool changeBoolean(bool &input)
{
	if (input == true) {
		input = false;
	} else {
		input = true;
	}
	return input;
}

//'zombieKillCount(int)' is a func that displays total zombies killed.
void zombieKillCount(int ypos,int zombie_kills)
{
	//extern int zombie_kills;
	Rect text;
	text.bot = ypos - YOFFSET;
	text.left = 10;
	text.center = 0;
	ggprint8b(&text, 16, yellow, "Zombie Kill Count: [ %i]", zombie_kills);
} 

// This will reset the zombie kill counter when called.
void resetKillCount(int &zombie_kills)
{
	if (zombie_kills >= 1) {
		zombie_kills = 0;
	} else {
		zombie_kills = 0;
	}
}

// End the game. Func WILL check if you killed enough zombies or not.
void endTheGame(int &zombieKills,bool &next,int &gamemode)
{
	if (zombieKills >= 5) {
		//cout << "inside eTGame, Resetting values... " << endl;
		resetKillCount(zombie_kills);
		changeBoolean(Next);
		gamemode = 0;
		/*cout << "AFTER RESET: " << Game_mode 
			<< " " << zombie_kills 
			<< " " << Next << " " 
			<< endl;*/
	} else
		cout << "You're not done killin' yet!" << endl;
}

// This func. will start us over if we died
void startOver(int &zombieKills,int &state,int &gamemode)
{
	if (state == 4) {
		resetKillCount(zombie_kills);
		gamemode = 0;
	} else
		cout << deadStr << endl;
}

// 'endGameScreen()' prints the end game screen.
void endGameScreen()
{
	//next_level = 2;
	glClear(GL_COLOR_BUFFER_BIT);
	Rect text;
	text.bot = 900 / 1.7;
	text.left = 1250 / 2.1;
	text.center = 0;
	//ggprint8b(&text, 16, yellow, "NEXT LEVEL: %i", next_level);
	ggprint8b(&text, 16, yellow, "YOU WIN! :)");
	ggprint8b(&text, 16, yellow, "O - Okay, coolbeans");
	// Wait for the printed message to be read
	//sleep(1);
	next_level++;
	Game_mode = 3;
	//cout << Game_mode << " " << endl;
	return;
}

// 'checkNextLevel()' checks to see if next level is imminent.
void checkNextLevel()
{
	if (counter == 5) {
		Next = true;
		counter = 0;
	} if (zombie_kills == 5 && Next) {
		endGameScreen();
	}
}

void displayMenu(int yrespos, int xrespos)
{
	Rect menu;
	menu.bot = yrespos - 310;
	menu.left = xrespos/HALVED;
	menu.center = 1;
	ggprint8b(&menu, 16, gold, "Game Menu");
	//
	static float angle = 0.0;
	xPosition = xrespos/HALVED - 70;
	yPosition = yrespos/HALVED + 107;
	glColor3ub(100, 150 ,150);
	glPushMatrix();
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(xPosition, yPosition, 0);
		//angle = angle + 2.5;
	glBegin(GL_QUADS);
		glVertex2i(0,	0);
		glVertex2i(0,	25);
		glVertex2i(150,	25);
		glVertex2i(150,	0);
	glEnd();
	glPopMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	// The following lines will align Menu Screen for Mac display
	//menu.bot = yrespos - 274;
	//menu.left = xrespos/HALVED;
	menu.bot = yPosition + 8;
	glColor3ub(100, 150 ,150);
	glPushMatrix();
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(xPosition, yPosition-50, 0);
	glBegin(GL_QUADS);
		glVertex2i(0,	0);
		glVertex2i(0,	25);
		glVertex2i(150,	25);
		glVertex2i(150,	0);
	glEnd();
	glPopMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	menu.left = xPosition + 70;
	menu.center = 1;
	ggprint8b(&menu, 16, yellow, "P - Play");
	glColor3f(1.0f, 0.0f, 0.0f);
	Rect credits;
	credits.bot = yPosition-42;
	credits.left = xPosition + 70;
	credits.center = 1;
	ggprint8b(&credits, 16, yellow, "C - View Credits");
	//	
	glColor3ub(100, 150 ,150);
	glPushMatrix();
	//glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(xPosition, yPosition-100, 0);
	glBegin(GL_QUADS);
		glVertex2i(0,	0);
		glVertex2i(0,	25);
		glVertex2i(150,	25);
		glVertex2i(150,	0);
	glEnd();
	glPopMatrix();
	Rect inst;
	inst.bot = yPosition-93;
	inst.left = xPosition + 70;
	inst.center = 1;
	ggprint8b(&inst, 16, yellow, "H - How To Play!");
	return;
}

float initZombiePosition(int input,int &zombie_pos)
{
	zombie_pos = input;
	return zombie_pos;
}

// Lab 7 timer function for timing another function.
double angelsTimer(int inputx, int inputy)
{
	extern double timeDiff(struct timespec *start, struct timespec *end);
	extern void timeCopy(struct timespec *dest, struct timespec *source);
	struct timespec ftimeStart, ftimeEnd;
	clock_gettime(CLOCK_REALTIME,&ftimeStart);
	int a = 200, b = 300, c = 400;
	for (int i = 0; i < 10; i++) {
		b = (a&15) - (b&7) - (c&3);
		c = (b&31) - (a&7) - (c&3);
	}
	clock_gettime(CLOCK_REALTIME, &ftimeEnd);
	my_timer += timeDiff(&ftimeStart, &ftimeEnd);
	void timerBox(int&,int&);
	timerBox(inputx,inputy);
	Rect z;
	z.bot = inputy-34;
	z.left = inputx-160;
	z.center = 0;
	ggprint8b(&z, 16, yellow, "ANGEL: ");
	z.bot = inputy-34;
	z.left = inputx-85;
	z.center = 0;
	ggprint8b(&z, 16, yellow, "%lf", my_timer); 
	z.bot = inputy-54;
	z.left = inputx-115;
	z.center = 1;
	ggprint8b(&z, 16, yellow, "<spacebar> PAUSE");
	return my_timer;
}

void timerBox(int &x,int &y)
{
	static float angle = 0.0;
	// red grn blu
	// 1, 100, 200 gives a nice blue
	glColor3ub(100, 150 ,150);
	glPushMatrix();
	glTranslatef( x-175,y - 40,0);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex2i(0,	0);
		glVertex2i(0,	25);
		glVertex2i(150,	25);
		glVertex2i(150,	0);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	Rect p;
	p.bot = y/2;
	p.left = x/2;
	p.center = 0;
	ggprint8b(&p, 16, yellow, "testing: ");
	p.bot = y-XOFFSET;
	p.left = x-200;
	p.center = 1;
	ggprint8b(&p, 16, yellow, "%lf", my_timer);
	glPopMatrix(); 
}

void showCredits(int yres, int xres)
{
	if (Game_mode != 1) {
	    static float angle = 0.0;
	    Rect back;
	    back.bot = (yres - 20);
	    back.left = (xres - xres) + 79;
	    back.center = 1;
	    ggprint8b(&back, 16, lt_blue, "<-- press 'b' to go back!");
	    xPosition = xres/HALVED - 70;
	    yPosition = yres/HALVED + 107;
	    //static float bounce = 0.0;
	    glClearColor(0.0, 0.0, 0.0, 1.0);
	    Rect c;
	    c.bot = yres - 215;
	    c.left = xres/HALVED;
	    c.center = 1;
	    ggprint8b(&c, 16, navy_green, "CREDITS:");
	    xPosition = xres/HALVED - 70;
	    yPosition = yres/HALVED + 107;
	    //	
		for (int k = 0; k < 3; k++) {
			glPushMatrix();
			glColor3ub(80, 40 ,50);
			glTranslatef(angle, angle, 0);
			glColor3ub(100, 150 ,150);
			for (int i=0; i<6; i++) {
				glTranslatef(angle, angle, 0); // angle makes it scroll
				 // which axis to rotate on? x y z
				glRotatef(angle, 0.0f, 0.0f, 1.0f);
				glBegin(GL_TRIANGLES);
					glVertex2f(-12.0f,	-10.0f);
					glVertex2f(0.0f,	20.0f);
					glVertex2f(0.0f,	-6.0f);
					glVertex2f(0.0f,	-6.0f);
					glVertex2f(0.0f,	20.0f);
					glVertex2f(12.0f,	-10.0f);
				glEnd();
			}
			glPopMatrix();
			//
			glPushMatrix();
			glColor3ub(20, 60 ,90);
			glTranslatef(angle, angle, 0);
			for (int i=0; i<6; i++) { 
				glTranslatef(angle, 250, 0);
				glRotatef(angle, 0.0f, 0.0f, 1.0f);
				glBegin(GL_TRIANGLES);
					glVertex2f(-12.0f,	-10.0f);
					glVertex2f(0.0f,	20.0f);
					glVertex2f(0.0f,	-6.0f);
					glVertex2f(0.0f,	-6.0f);
					glVertex2f(0.0f,	20.0f);
					glVertex2f(12.0f,	-10.0f);
				glEnd();
			} 
			glPopMatrix();
			//
			glPushMatrix();
			glColor3ub(140, 150 ,1);
			for (int i=0; i<6; i++) { 
				glTranslatef(angle, angle/2, 0);
				glRotatef(angle, 0.0f, 0.0f, 1.0f);
				glBegin(GL_TRIANGLES);
					glVertex2f(-12.0f,	-10.0f);
					glVertex2f(0.0f,	20.0f);
					glVertex2f(0.0f,	-6.0f);
					glVertex2f(0.0f,	-6.0f);
					glVertex2f(0.0f,	20.0f);
					glVertex2f(12.0f,	-10.0f);
				glEnd();
			} 
			glPopMatrix();
		}
		//
		glPushMatrix();
	    glColor3ub(20, 60 ,90);
	    glTranslatef(xPosition, angle, 0); 
		    angle = angle + 1.5;
	if (angle >= yres + 65) {
		angle = 0;
	}
	glBegin(GL_QUADS);
		glVertex2i(0,	0);
		glVertex2i(0,	25);
		glVertex2i(150,	25);
		glVertex2i(150,	0);
	glEnd();
	glTranslatef(0, 50, 0);
	glBegin(GL_QUADS);
		glVertex2i(0,	0);
		glVertex2i(0,	25);
		glVertex2i(150,	25);
		glVertex2i(150,	0);
	glEnd();
	glTranslatef(0, 50, 0);
	glBegin(GL_QUADS);
		glVertex2i(0,	0);
		glVertex2i(0,	25);
		glVertex2i(150,	25);
		glVertex2i(150,	0);
	glEnd();
	glTranslatef(0, 50, 0);
	glBegin(GL_QUADS);
		glVertex2i(0,	0);
		glVertex2i(0,	25);
		glVertex2i(150,	25);
		glVertex2i(150,	0);
	glEnd();
	Rect names;
	names.bot = -144;
	names.left = 70;
	names.center = 1;
	ggprint8b(&names, 16, gold, "Christy Guerrero");
	names.bot = -94;
	names.left = 70;
	names.center = 1;
	ggprint8b(&names, 16, gold, "Abdullah Aljadahli");
	names.bot = -44;
	names.left = 70;
	names.center = 1;
	ggprint8b(&names, 16, gold, "Nygel Aton");
	names.bot = 8;
	names.left = 70;
	names.center = 1;
	ggprint8b(&names, 16, gold, "Angel Rojas");
	glPopMatrix();
	} else
		cout << "Bad State: you are currently playing..." << endl;
}

void instructions(int yres, int xres)
{
	Rect text;
	text.bot = (yres - 20);
	text.left = (xres - xres) + 79;
	text.center = 1;
	ggprint8b(&text, 16, lt_blue, "<-- press 'b' to go back!");
	text.bot = (yres / 2) + 40;
	text.left = xres / 2;
	glClearColor(0.0,0.0,0.0, 0.8);
	glColor3f(0.0,0.0,0);
	ggprint8b(&text, 16, lt_blue, "B - to go back to Menu");
	ggprint8b(&text, 16, navy_green, "C - to view credits");
	ggprint8b(&text, 16, gold, "Just click to shoot zombies, dummy!");
}

void change_toInstr()
{
	//Game_mode = INSTRUCTIONS;
	Game_mode = 6;
	return;
}

void change_toMenu()
{
	Game_mode = MENU;
	return;
}

void stateHitOnce(int &state)
{
	state = 1;
	return;
}

void stateHitTwice(int &state)
{
	state = 2;
	return;
}

void stateHitThrice(int &state)
{
	state = 3;
	return;
}
// player is Dead. Need to change screen
void statePlayerDead(int &state)
{
	state = 4;
	return;
}
