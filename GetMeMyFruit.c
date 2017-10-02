#include <stdio.h>
#include <allegro.h>
#include "mappyal.h"
#include "GetMeMyFruit.h"

//tile grabber
BITMAP *grabframe(BITMAP *source, 
                  int width, int height, 
                  int startx, int starty, 
                  int columns, int frame)
{
    BITMAP *temp = create_bitmap(width,height);
    int x = startx + (frame % columns) * width;
    int y = starty + (frame / columns) * height;
    blit(source,temp,x,y,0,0,width,height);
    return temp;
}

int collided(int x, int y)
{
    BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	return blockdata->tl;
}

// Setup initial files
void setupscreen() {
	
	// Error variable
    int ret;
	
    //set video mode    
    set_color_depth(16);
    
    ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    if (ret != 0) {
	    allegro_message(allegro_error);
	    return;
	}
	
	//load level
	if (MapLoad(GAME_WORLD)) {
		allegro_message("Error loading map");
		return;
	}
	
	//create the double buffer
	buffer = create_bitmap (WIDTH, HEIGHT);
	clear(buffer);
}

// Setup game
void setupgame() {
	
	//Setup Player
	player_image[0] = load_bitmap(PLAYER_SPRITE_WAIT1, NULL);
	player_image[1] = load_bitmap(PLAYER_SPRITE_WAIT2, NULL);
	player_image[2] = load_bitmap(PLAYER_SPRITE_WALK1, NULL);
	player_image[3] = load_bitmap(PLAYER_SPRITE_WALK2, NULL);
	player_image[4] = load_bitmap(PLAYER_SPRITE_JUMP, NULL);

    player = malloc(sizeof(SPRITE));
    player->x = START_POINT_X;
    player->y = START_POINT_Y;
    player->curframe= 0;
    player->framecount= 0;
    player->framedelay= FRAME_DELAY;
    player->maxframe= PLAYER_MAX_FRAME;
    player->width= player_image[0]->w;
    player->height= player_image[0]->h;
}

void walk(int dir) {
	facing = dir; 
    player->x+= 2 * dir; 
    if (++player->framecount > player->framedelay)
    {
    	player->framecount=0;
    	if (player->curframe > WALKFRAME_MAX || player->curframe < WALKFRAME_MIN) {
			player->curframe = 	WALKFRAME_MIN;
		}
        if (++player->curframe > WALKFRAME_MAX)
            player->curframe=WALKFRAME_MIN;
    }
}

void wait() {
	if (++player->framecount > player->framedelay)
    {
    	player->framecount=0;
		if (player->curframe > WAITFRAME_MAX || player->curframe < WAITFRAME_MIN) {
			player->curframe = 	WAITFRAME_MIN;
		}
		else if (++player->curframe > WAITFRAME_MAX) {
			player->curframe = WAITFRAME_MIN;
		}
	}
}

void update() {
	
	oldpy = player->y; 
    oldpx = player->x;
	
	//handle jumping
    if (jump==JUMPIT)
    { 
        if (!collided(player->x + player->width/2, 
            player->y + player->height + 5))
            jump = 0; 
    }
    else
    {
    	player->curframe = JUMPFRAME;
        player->y -= jump/3; 
        jump--; 
    }

	if (jump<0) 
    { 
        if (collided(player->x + player->width/2, 
            player->y + player->height))
		{ 
            jump = JUMPIT; 
            while (collided(player->x + player->width/2, 
                player->y + player->height))
                player->y -= 2; 
        } 
    }
	
	//check for collided with foreground tiles
	if (facing == -1) 
    { 
        if (collided(player->x, player->y + player->height)) 
            player->x = oldpx; 
    }
	else 
    { 
        if (collided(player->x + player->width, 
            player->y + player->height)) 
            player->x = oldpx; 
    }
	
    //update the map scroll position
	mapxoff = player->x + player->width/2 - WIDTH/2 + 10;
	mapyoff = player->y + player->height/2 - HEIGHT/2 + 10;


    //avoid moving beyond the map edge
	if (mapxoff < 0) {
		mapxoff = 0;
	}
	if (mapxoff > (mapwidth * mapblockwidth - WIDTH)) {
		mapxoff = mapwidth * mapblockwidth - WIDTH;
	}
	if (mapyoff < 0) {
        mapyoff = 0;
    }
	if (mapyoff > (mapheight * mapblockheight - HEIGHT)) {
        mapyoff = mapheight * mapblockheight - HEIGHT;
    }
    
    //avoid player moving beyoard map edge (left and right)
    if (player->x < 0) {
    	player->x = 0;	
    }
    if (player->x > (mapwidth * mapblockwidth - player->width)) {
    	player->x = (mapwidth * mapblockwidth - player->width);
    }
    if (player->y > (mapheight * mapblockheight + player->height)) {
    	gameover = 1;
    }

	if (oldpy == player->y & oldpx == player->x) {
		waitcount++;
		
		if (waitcount > WAITCOUNT_MAX) {
			wait();	
			waitcount = 0;
		}
	}
	else {
		waitcount = 0;	
	}

    //draw the background tiles
	MapDrawBG(buffer, mapxoff, mapyoff, 0, 0, WIDTH-1, HEIGHT-1);

    //draw foreground tiles
	MapDrawFG(buffer, mapxoff, mapyoff, 0, 0, WIDTH-1, HEIGHT-1, 0);

    //draw the player's sprite
	if (facing == 1) 
        draw_sprite(buffer, player_image[player->curframe], 
            (player->x-mapxoff), (player->y-mapyoff+1));
	else 
        draw_sprite_h_flip(buffer, player_image[player->curframe], 
            (player->x-mapxoff), (player->y-mapyoff));
}

// Get game input from user
void getinput() {
	
    //hit ESC to quit
    if (key[KEY_ESC]) {
		gameover = 1;
	}
	
	// Instruction screen
	if (key[KEY_LCONTROL] && key[KEY_H]) {
		//instructions();
	}
	
	// Background music toggle
	if (key[KEY_LCONTROL] && key[KEY_M]) {
		if (sound == 1) {
			//sound = 0;
			//stop_sample(background_music);
		}
		else {
			//sound = 1;
			//play_sample(background_music, 128, 128, 1000, TRUE);
		}
		rest(20);
	}
    
    //ARROW KEYS AND SPACE BAR CONTROL
    if (key[KEY_LEFT]) {
		walk(-1);
	}	
    else if (key[KEY_RIGHT]) {
		walk(1);
	}
	
	
    if (key[KEY_SPACE]) {
    	if (jump==JUMPIT) {
    		jump = 30;
    	}
	}
}

int main (void)
{
    int mapxoff, mapyoff;
    int oldpy, oldpx;
    int facing = 0;
    int jump = JUMPIT;
    int i;

	//initialize Allegro
    allegro_init(); 
    
    //initialize the keyboard
    install_keyboard(); 
    
    //initalize timer
    install_timer();
    
    //initialize random seed
    srand(time(NULL));
    
    // Setup screen
    setupscreen();

	setupgame();

    //main loop
	while (!gameover)
	{

		//check for keypresses
        if (keypressed()) {
            getinput();
		}

		//update
		update();

        //blit the double buffer 
		vsync();
        acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH-1, HEIGHT-1);
        release_screen();

	}

    for (i = 0; i < PLAYER_MAX_FRAME; i++) {
    	destroy_bitmap(player_image[i]);
    } 
    free(player);
	destroy_bitmap(buffer);
	MapFreeMem();
	allegro_exit();
	return 0;
}
END_OF_MAIN()
