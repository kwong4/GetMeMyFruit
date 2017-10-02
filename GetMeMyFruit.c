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

int inside(SPRITE* sprite) {
	//is sprite visible on screen?
    if (sprite->y > mapyoff - FRUIT_CONSTANT && sprite->y < mapyoff + HEIGHT + FRUIT_CONSTANT)
    {
    	if (sprite->x > mapxoff - FRUIT_CONSTANT && sprite->x < mapxoff + WIDTH + FRUIT_CONSTANT) {
    		return 1;	
    	}
    }
    return 0;
}

// Print insturction screen
void instructions() {
	
	// Clear screen
	rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
	
	// Print game info
	textout_centre_ex(screen, font, "GAME INFO:", WIDTH/2, HEIGHT/4, WHITE, BLACK);
	textout_ex(screen, font, "- You are tasked with collecting 4 different types of fruit:", WIDTH/8, HEIGHT/4 + 20, WHITE, BLACK);
	textout_ex(screen, font, "    - Orange", WIDTH/8, HEIGHT/4 + 30, ORANGE, BLACK);
	textout_ex(screen, font, "    - Watermelon", WIDTH/8, HEIGHT/4 + 40, GREEN, BLACK);
	textout_ex(screen, font, "    - Apple", WIDTH/8, HEIGHT/4 + 50, RED, BLACK);
	textout_ex(screen, font, "    - Berries", WIDTH/8, HEIGHT/4 + 60, BLUE, BLACK);
	textout_ex(screen, font, "- Avoid enemies that may look like fruit, but move!", WIDTH/8, HEIGHT/4 + 70, WHITE, BLACK);
	textout_ex(screen, font, "- Touching an enemy will result in an instant death!", WIDTH/8, HEIGHT/4 + 80, WHITE, BLACK);
	textout_ex(screen, font, "- Easy gamemode will make enemies move at standard speed", WIDTH/8, HEIGHT/4 + 90, WHITE, BLACK);
	textout_ex(screen, font, "- Hard gamemode will make enemies move faster", WIDTH/8, HEIGHT/4 + 100, WHITE, BLACK);
	
	// Print game instructions
	textout_centre_ex(screen, font, "INSTRUCTIONS:", WIDTH/2, HEIGHT/2 + 40, WHITE, BLACK);
    textout_ex(screen, font, "1. Use the arrow keys to move your Character", WIDTH/8, HEIGHT/2 + 60, WHITE, BLACK);
    textout_ex(screen, font, "   Use the LEFT key to move LEFT", WIDTH/8, HEIGHT/2 + 70, YELLOW, BLACK);
    textout_ex(screen, font, "   Use the RIGHT key to move RIGHT", WIDTH/8, HEIGHT/2 + 80, YELLOW, BLACK);
    textout_ex(screen, font, "2. Use the SPACE bar key to JUMP", WIDTH/8, HEIGHT/2 + 90, WHITE, BLACK);
    textout_ex(screen, font, "4. Press Ctrl + h to bring up the instructions at any time!", WIDTH/8, HEIGHT/2 + 120, WHITE, BLACK);
    textout_ex(screen, font, "5. Press Ctrl + m to toggle the background music at any time!", WIDTH/8, HEIGHT/2 + 130, WHITE, BLACK);
    textout_ex(screen, font, "6. Press Esc to exit the game!", WIDTH/8, HEIGHT/2 + 140, WHITE, BLACK);
    
    // ENTER to return
    textout_centre_ex(screen, font, "Press ENTER to return", WIDTH/2, HEIGHT/2 + 200, WHITE, BLACK);
    
    // Slow down game
    rest(250);
    
    // Check input from user
    while(1) {
    	if (key[KEY_ENTER]) {
    		play_sample(click_sound, 128, 128, 1000, FALSE);
			break;
		}
		
		// Background music toggle
		if (key[KEY_LCONTROL] && key[KEY_M]) {
			if (sound == 1) {
				sound = 0;
				stop_sample(background_music);
			}
			else {
				sound = 1;
				play_sample(background_music, 128, 128, 1000, TRUE);
			}
			rest(80);
		}
		
		if (key[KEY_ESC]) {
			allegro_exit();
			exit(0);	
		}
    }
}

// Get Game menu input
int getmenuinput() {
	
	// If user quits game
	if (key[KEY_ESC]) {
		allegro_exit();
		exit(0);
	}
	
	// Move cursor for selection
	// Play sound for each movement
	if (key[KEY_DOWN] && selection != max_selection) {
		play_sample(click_sound, 128, 128, 1000, FALSE);
		rectfill(screen, WIDTH/3 + 1, HEIGHT/2 + selection * 15 + 81, WIDTH/3 + 9, HEIGHT/2 + selection * 15 + 89, BLACK);
		selection++;
		rectfill(screen, WIDTH/3 + 1, HEIGHT/2 +  selection * 15 + 81, WIDTH/3 + 9, HEIGHT/2 + selection * 15 + 89, WHITE);
	}
	else if (key[KEY_UP] && selection != 0) {
		play_sample(click_sound, 128, 128, 1000, FALSE);
		rectfill(screen, WIDTH/3 + 1, HEIGHT/2 + selection * 15 + 81, WIDTH/3 + 9, HEIGHT/2 +  selection * 15 + 89, BLACK);
		selection--;
		rectfill(screen, WIDTH/3 + 1, HEIGHT/2 + selection * 15 + 81, WIDTH/3 + 9, HEIGHT/2 + selection * 15 + 89, WHITE);
	}
	else if (key[KEY_ENTER]) {
		play_sample(click_sound, 128, 128, 1000, FALSE);
		return -1;
	}
}

// Print the welcome screen menu
void welcome_screen() {
	
	// Blit the title
	blit(title, screen, 0, 0, 65, 50, title->w, title->h);
	
	// Options and insturctions
    textout_centre_ex(screen, font, "Press use your ARROW KEYS and ENTER to select an option!", WIDTH/2, HEIGHT/2 + 40, WHITE, BLACK);
    rect(screen, WIDTH/3, HEIGHT/2 + 80 , WIDTH/3 + 10, HEIGHT/2 + 90, WHITE);
    textout_ex(screen, font, "Start!", WIDTH/3 + 25, HEIGHT/2 + 82, WHITE, BLACK);
    rect(screen, WIDTH/3, HEIGHT/2 + 95, WIDTH/3 + 10, HEIGHT/2 + 105, WHITE);
    textout_ex(screen, font, "Instructions", WIDTH/3 + 25, HEIGHT/2 + 97, WHITE, BLACK);
    rect(screen, WIDTH/3, HEIGHT/2 + 110, WIDTH/3 + 10, HEIGHT/2 + 120, WHITE);
    textout_ex(screen, font, "Gamemode:", WIDTH/3 + 25, HEIGHT/2 + 112, WHITE, BLACK);
    selection = 0;
    rectfill(screen, WIDTH/3 + 1, HEIGHT/2 + 81, WIDTH/3 + 9, HEIGHT/2 + 89, WHITE);
    
    // Check if user selects Hard or Easy mode
    if (hardmode == 0) {
    	textout_ex(screen, font, "Easy", WIDTH/3 + 100, HEIGHT/2 + 112, YELLOW, BLACK);
    }
    else {
    	textout_ex(screen, font, "Hard", WIDTH/3 + 100, HEIGHT/2 + 112, YELLOW, BLACK);
    }	
}

// Draw initial start screen instructions
void draw_startscreen() {
    
    // Run welcome screen print
    welcome_screen();
    
    // Check for user input and update for user selection
    while (1) {
    	if (getmenuinput() == -1) {
    		if (selection == 0) {
    			break;
    		}
    		else if (selection == 1) {
    			instructions();
    			rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    			welcome_screen();
    		}
    		else {
    			hardmode = (hardmode + 1) % 2;
    			if (hardmode == 0) {
		    		textout_ex(screen, font, "Easy", WIDTH/3 + 100, HEIGHT/2 + 112, YELLOW, BLACK);
			    }
			    else {
			    	textout_ex(screen, font, "Hard", WIDTH/3 + 100, HEIGHT/2 + 112, YELLOW, BLACK);
			    }
    		}
    	}
    	
    	// Slow game down
	    rest(100);
    }
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
	
	//load title screen
	title = load_bitmap(GETMEMYFRUIT, NULL);
	if (!title) {
		allegro_message("Error loading title screen");
		return;
	}
	
	// Load sound files
	background_music = load_sample(BACKGROUND_SOUND);
	click_sound = load_sample(CLICK_SOUND);
	
	//install a digital sound driver
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, "") != 0) {
    	allegro_message("Error initalizing sound system");
		return;	
    }
    
    // Check if sound files loaded
    if (!background_music || !click_sound) {
    	allegro_message("Error reading wave files");
    	return;
    }
	
	//load level
	if (MapLoad(GAME_WORLD)) {
		allegro_message("Error loading map");
		return;
	}
	
	//create the double buffer
	buffer = create_bitmap(WIDTH, HEIGHT);
	clear(buffer);
	
	// Draw start screen
	draw_startscreen();
}

// Setup game
void setupgame() {
	
	// Loop variable
	int i;
	
	//Play background music
	play_sample(background_music, 128, 128, 1000, TRUE);
	
	//Setup Player
	player_image[0] = load_bitmap(PLAYER_SPRITE_WAIT1, NULL);
	player_image[1] = load_bitmap(PLAYER_SPRITE_WAIT2, NULL);
	player_image[2] = load_bitmap(PLAYER_SPRITE_WALK1, NULL);
	player_image[3] = load_bitmap(PLAYER_SPRITE_WALK2, NULL);
	player_image[4] = load_bitmap(PLAYER_SPRITE_JUMP, NULL);

	// Error checking
	if (!player_image[0] || !player_image[1] || !player_image[2] || !player_image[3] || !player_image[4]) {
		allegro_message("Error loading fruit sprites");
		return;
	}

    player = malloc(sizeof(SPRITE));
    player->x = START_POINT_X;
    player->y = START_POINT_Y;
    player->curframe= 0;
    player->framecount= 0;
    player->framedelay= FRAME_DELAY;
    player->maxframe= PLAYER_MAX_FRAME;
    player->width= player_image[0]->w;
    player->height= player_image[0]->h;
    
    //Setup Fruit
	fruits_image[0] = load_bitmap(ORANGE_SPRITE, NULL);
	fruits_image[1] = load_bitmap(WATERMELON_SPRITE, NULL);
	fruits_image[2] = load_bitmap(APPLE_SPRITE, NULL);
	fruits_image[3] = load_bitmap(BERRIES_SPRITE, NULL);
	
	// Error checking
	if (!fruits_image[0] || !fruits_image[1] || !fruits_image[2] || !fruits_image[3]) {
		allegro_message("Error loading fruit sprites");
		return;
	}
	
	for (i = 0; i < FRUIT_MAX; i++) {
		fruits[i] = malloc(sizeof(SPRITE));	
	}
	
	// Setup fruit sprite locations
	fruits[0]->x = mapblockwidth;
	fruits[0]->y = mapblockheight * 3;
	fruits[0]->data = 0;
	
	fruits[1]->x = mapblockwidth * 3;
	fruits[1]->y = mapblockheight * 26;
	fruits[1]->data = 1;
	
	fruits[2]->x = mapblockwidth * 116;
	fruits[2]->y = mapblockheight * 3;
	fruits[2]->data = 2;
	
	fruits[3]->x = mapblockwidth * 118;
	fruits[3]->y = mapblockheight * 26;
	fruits[3]->data = 3;
	
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
	
	//Loop variable
	int i;
	
	// Old coordinates
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

	if (jump<0) { 
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
	if (facing == -1) { 
        if (collided(player->x, player->y + player->height)) 
            player->x = oldpx; 
    }
	else { 
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
    	exit(0);
    }
    if (player->y < 0) {
    	player->y = 0;	
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

	//draw fruit
    for (i = 0; i < FRUIT_MAX; i++) {
    	if (inside(fruits[i])) {
	        draw_sprite(buffer, fruits_image[i],
	        fruits[i]->x- mapxoff, fruits[i]->y - mapyoff);
	    }
    }

    //draw the player's sprite
	if (facing == 1) {
		if (player->y > player->width * -1) {
			draw_sprite(buffer, player_image[player->curframe], 
            	(player->x-mapxoff), (player->y-mapyoff+1));
		}
    }
	else {
		if (player->y > player->width * -1) {
        draw_sprite_h_flip(buffer, player_image[player->curframe], 
            (player->x-mapxoff), (player->y-mapyoff));
        }    
    }
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
			sound = 0;
			stop_sample(background_music);
		}
		else {
			sound = 1;
			play_sample(background_music, 128, 128, 1000, TRUE);
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
	destroy_bitmap(title);
	for (i = 0; i < FRUIT_MAX; i++) {
		destroy_bitmap(fruits_image[i]);
		free(fruits[i]);
	}
	destroy_sample(background_music);
	destroy_sample(click_sound);
	MapFreeMem();
	allegro_exit();
	return 0;
}
END_OF_MAIN()
