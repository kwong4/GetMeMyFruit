#ifndef _GETMEMYFRUIT_H
#define _GETMEMYFRUIT_H

#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 640
#define HEIGHT 480
#define JUMPIT 1600

#define BLACK makecol(0,0,0)
#define WHITE makecol(255,255,255)
#define YELLOW makecol(255,216,0)
#define ORANGE makecol(255,127,39)
#define GREEN makecol(80,226,83)
#define RED makecol(255,47,63)
#define BLUE makecol(70,101,200)
#define PLAYER_SPRITE_WAIT1 "sprites/character_wait1.bmp"
#define PLAYER_SPRITE_WAIT2 "sprites/character_wait2.bmp"
#define PLAYER_SPRITE_WALK1 "sprites/character_walk1.bmp"
#define PLAYER_SPRITE_WALK2 "sprites/character_walk2.bmp"
#define PLAYER_SPRITE_JUMP "sprites/character_jump.bmp"
#define ORANGE_SPRITE "sprites/orange.bmp"
#define WATERMELON_SPRITE "sprites/watermelon.bmp"
#define APPLE_SPRITE "sprites/apple.bmp"
#define BERRIES_SPRITE "sprites/berries.bmp"
#define GAME_WORLD "sprites/GameWorld.FMP"
#define GETMEMYFRUIT "sprites/GetMeMyFruit.bmp"
#define BACKGROUND_SOUND "sounds/background.wav"
#define CLICK_SOUND "sounds/click.wav"
#define START_POINT_X 59 * 16
#define START_POINT_Y 24 * 14
#define WALKFRAME_MIN 2 
#define WALKFRAME_MAX 3
#define WAITFRAME_MIN 0
#define WAITFRAME_MAX 1
#define FRUIT_MAX 4
#define FRAME_DELAY 6
#define PLAYER_MAX_FRAME 4
#define JUMPFRAME 4
#define WAITCOUNT_MAX 5
#define TOP_BUFFER 10
#define FRUIT_CONSTANT 20

//define the sprite structure
typedef struct SPRITE
{
    int dir, alive;
    int x,y;
    int width,height;
    int xspeed,yspeed;
    int xdelay,ydelay;
    int xcount,ycount;
    int curframe,maxframe,animdir;
    int framecount,framedelay;
    int data;
}SPRITE;

//declare the bitmaps and sprites
BITMAP *player_image[5];
SPRITE *player;
BITMAP *fruits_image[4];
SPRITE *fruits[4];
BITMAP *buffer;	
BITMAP *temp;
BITMAP *title;
SAMPLE *background_music;
SAMPLE *click_sound;
int facing = 0;
int jump = JUMPIT;
int mapxoff, mapyoff;
int oldpx, oldpy;
int gameover = 0;
int win = 0;
int sound = 1;
int waitcount = 0;
int selection = 0;
int max_selection = 2;
int hardmode = 0;
int fruit_collected = 0;

#endif
