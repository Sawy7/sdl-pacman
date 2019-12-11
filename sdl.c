#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/* vytvorte strukturu reprezentujici pohybujici se atom (SDL_rect a rychlosti ve smeru x,y) */
typedef struct
{
    SDL_Rect rect;
    int speed_x;
    int speed_y;
    int nexist;
} Atom;

typedef struct
{
    SDL_Texture* texture;
    Atom* text_field;
    int width;
    int height;
} Text;

Atom* atom_create(int pos_x, int pos_y, int w, int h, int speed_x, int speed_y)
{
    Atom* a = (Atom*) malloc(sizeof(Atom));
    a->rect.x = pos_x;
    a->rect.y = pos_y;
    a->rect.w = w;
    a->rect.h = h;
    a->speed_x = speed_x;
    a->speed_y = speed_y;
    a->nexist = 0;
    return a;
}

void atom_delete(Atom* a)
{
    free(a);
}

void atom_move(Atom* a)
{
    a->rect.x += a->speed_x;
    a->rect.y += a->speed_y;
    
    if (a->rect.x + a->rect.w > WINDOW_WIDTH)
    {
        a->rect.x = 0;
    }
    if (a->rect.x < 0)
    {
        a->rect.x = WINDOW_WIDTH - a->rect.w;
    }
    if (a->rect.y + a->rect.h > WINDOW_HEIGHT)
    {
        a->rect.y = 0;
    }
    if (a->rect.y < 0)
    {
        a->rect.y = WINDOW_HEIGHT - a->rect.h;
    }
}

void ghost_chase(Atom* a, Atom* b)
{
    if (a->rect.x > b->rect.x)
    {
        b->speed_x = 1;
        //b->speed_y = 0;
    } else if (a->rect.x < b->rect.x) {
        b->speed_x = -1;
        //b->speed_y = 0;
    } else if (a->rect.x == b->rect.x) {
        b->speed_x = 0;
    }
    if (a->rect.y > b->rect.y)
    {
        //b->speed_x = 0;
        b->speed_y = 1;
    } else if (a->rect.y < b->rect.y) {
        //b->speed_x = 0;
        b->speed_y = -1;
    } else if (a->rect.y == b->rect.y) {
        b->speed_y = 0;
    }
}

void atom_collision(Atom* a, Atom* wall)
{
    //Player
    int a_left = a->rect.x;
    int a_right = a->rect.x + a->rect.w;
    int a_top = a->rect.y;
    int a_bottom = a->rect.y + a->rect.h;
    //Wall
    int wall_left = wall->rect.x;
    int wall_right = wall->rect.x + wall->rect.w;
    int wall_top = wall->rect.y;
    int wall_bottom = wall->rect.y + wall->rect.h;
    if (a_bottom > wall_top && a_bottom < (wall_top + (wall->rect.h*0.25)) && a->speed_y > 0 && a_right > wall_left && a_left < wall_right)
    {
        a->speed_y = 0;
        a->rect.y = wall_top - a->rect.h;
    }
    if (a_top < wall_bottom && a_top > (wall_bottom - (wall->rect.h*0.25)) && a->speed_y < 0 && a_right > wall_left && a_left < wall_right)
    {
        a->speed_y = 0;
        a->rect.y = wall_bottom;
    }
    if (a_right > wall_left && a_right < (wall_left + (wall->rect.w*0.25)) && a->speed_x > 0 && a_bottom > wall_top && a_top < wall_bottom)
    {
        a->speed_x = 0;
        a->rect.x = wall_left - a->rect.w;
    }
    if (a_left < wall_right && a_left > (wall_right - (wall->rect.w*0.25)) && a->speed_x < 0 && a_bottom > wall_top && a_top < wall_bottom)
    {
        a->speed_x = 0;
        a->rect.x = wall_right;
    }
}

void eat (Atom* a, Atom* p)
{
    int state = 0;
    ///Player
    int a_left = a->rect.x;
    int a_right = a->rect.x + a->rect.w;
    int a_top = a->rect.y;
    int a_bottom = a->rect.y + a->rect.h;
    //Point
    int p_center_x = p->rect.x + p->rect.w*0.5;
    int p_center_y = p->rect.y + p->rect.h*0.5;

    if (p_center_x < a_right && p_center_x > a_left && p_center_y < a_bottom && p_center_y > a_top)
    {
        state += 1;
        //printf("hit\n");
    }
    p->nexist = state;
}

Atom* point_gen (int r)
{
    int rows = (WINDOW_HEIGHT/r);
    int cols = (WINDOW_WIDTH/r);
    int count = cols * rows;
    Atom* array = (Atom*) malloc(sizeof(Atom)*count);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            array[i*cols+j] = *(atom_create(j*r, i*r, r, r, 0, 0));
        }
    }
    return array;    
}

int point_count (Atom array[])
{
    int count = (WINDOW_WIDTH/array[0].rect.w) * (WINDOW_HEIGHT/array[0].rect.w);
    //printf("count: %d\n", count);
    int score = 0;
    for (int i = 0; i < count; i++)
    {
        score = score + array[i].nexist;
    }
    //printf("score: %d\n", score);
    return score;
}

void just_go(Atom* a, char direction[])
{
    if (direction == "left")
    {
        a->speed_x = -2;
        a->speed_y = 0;
    } else if (direction == "right") {
        a->speed_x = 2;
        a->speed_y = 0;
    } else if (direction == "up") {
        a->speed_x = 0;
        a->speed_y = -2;
    } else if (direction == "down") {
        a->speed_x = 0;
        a->speed_y = 2;
    }
}

Text* gen_text(char text[], TTF_Font* font, SDL_Renderer* ren, int x, int y) {
    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    int width;
    int height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height); 
    Atom* text_field = atom_create(x, y, width, height, 0, 0);
    Text* t_field_ret = (Text*) malloc(sizeof(Text));
    t_field_ret->text_field = text_field;
    t_field_ret->texture = texture;
    t_field_ret->width = width;
    t_field_ret->height = height;
    return t_field_ret;
}

char* read_map(int r) 
{
    int rows = (WINDOW_HEIGHT/r);
    int cols = (WINDOW_WIDTH/r);
    int count = cols * rows;
    char blob;
    char* marray = (char*) malloc(sizeof(char)*1200);
    FILE* f = fopen("map.txt", "r");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        { 
            fscanf(f, "%c", &blob);
            if (blob != '\n')
            {
                marray[i*cols+j] = blob;
            } else {
                fscanf(f, "%c", &blob);
                marray[i*cols+j] = blob;
            }
            printf("%c", blob);
        }
        printf("\n");
    }
    fclose(f);
    return marray;    
}

int main()
{
    srand(time(NULL));

    //reading map from txt file
    char* marray = read_map(20);

    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    //Text rendering init
    if( TTF_Init() == -1 )
    {
        return false;    
    }

    SDL_Window* win = SDL_CreateWindow("SDL experiments", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        SDL_DestroyWindow(win);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("fonts/OpenSans-Bold.ttf", 25);
    
    //Nacitani textur:
    //Pacman
    SDL_Surface* img = IMG_Load("pacman.svg");
    if(!img)
    {
        fprintf(stderr, "SDL_IMG_Load Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, img);
    if(!texture)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }

    //Ghost
    img = IMG_Load("ghost.png");
    if(!img)
    {
        fprintf(stderr, "SDL_IMG_Load Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }

    SDL_Texture* gtexture = SDL_CreateTextureFromSurface(ren, img);
    if(!gtexture)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }

    //Point
    img = IMG_Load("point.png");
    if(!img)
    {
        fprintf(stderr, "SDL_IMG_Load Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }

    SDL_Texture* ptexture = SDL_CreateTextureFromSurface(ren, img);
    if(!gtexture)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }
    
    SDL_FreeSurface(img);    

    SDL_Event e;

    //pacman definitions
    int pcm_r = 20;
    int pcm_x = 20;
    int pcm_y = 20;
    int pcm_sx = 2;
    int pcm_sy = 0;
    Atom* pacman = atom_create(pcm_x, pcm_y, pcm_r, pcm_r, pcm_sx, pcm_sy);

    Atom* ghost = atom_create(200, 300, 40, 40, 0, 0);

    //generate points
    Atom* array = point_gen(20);
    
    double angle = 0;
    int life = 5;
    bool scoreboard = true;

    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            //key events
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_r)
                {
                    quit = true;
                }
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    scoreboard = false;
                }
                if (e.key.keysym.sym == SDLK_LEFT)
                {
                    just_go(pacman, "left");
                    angle = 180;
                }
                if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    just_go(pacman, "right");
                    angle = 0;
                }
                if (e.key.keysym.sym == SDLK_UP)
                {
                    just_go(pacman, "up");
                    angle = -90;
                }
                if (e.key.keysym.sym == SDLK_DOWN)
                {
                    just_go(pacman, "down");
                    angle = 90;
                }
            }
        }

        //render background
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        if (scoreboard != true)
        {
           //multiple points
            for (int i = 0; i < ((WINDOW_WIDTH/array[0].rect.h)*(WINDOW_HEIGHT/array[0].rect.h)); i++)
            {
                if (array[i].nexist !=1 && marray[i] == '.')
                {
                    SDL_RenderCopyEx(ren, ptexture, NULL, &(array[i].rect), 0, NULL, SDL_FLIP_NONE);
                    eat(pacman, &array[i]);
                }
            }        

            //multiple walls
            for (int i = 0; i < ((WINDOW_WIDTH/array[0].rect.h)*(WINDOW_HEIGHT/array[0].rect.h)); i++)
            {
                if (marray[i] == '#')
                {
                    SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
                    SDL_RenderFillRect(ren, &array[i].rect);
                    SDL_RenderCopyEx(ren, NULL, NULL, &(array[i].rect), 0, NULL, SDL_FLIP_NONE);
                    atom_collision(pacman, &(array[i]));
                    atom_collision(ghost, &(array[i]));
                }
            }

            //count points
            int score = point_count(array);
            //printf("%d\n", score);
            char strscore[10];
            sprintf(strscore, "%d", score);

            
            //score text render
            char stext[20] = "Score: ";
            strcat(stext, strscore);
            Text* scorecount = gen_text(stext, font, ren, 0, 20);
            scorecount->text_field->rect.x = 5;
            scorecount->text_field->rect.y = WINDOW_HEIGHT-20;
            scorecount->text_field->rect.w = scorecount->text_field->rect.w / 2;
            scorecount->text_field->rect.h = scorecount->text_field->rect.h / 2;
            SDL_RenderCopyEx(ren, scorecount->texture, NULL, &(scorecount->text_field->rect), 0, NULL, SDL_FLIP_NONE);
            SDL_DestroyTexture(scorecount->texture);

            //life text rende
            char strlife[10];
            sprintf(strlife, "%d", life);
            char ltext[20] = "Lifes: ";
            strcat(ltext, strlife);
            Text* lifecount = gen_text(ltext, font, ren, 0, 20);
            lifecount->text_field->rect.x = WINDOW_WIDTH-(lifecount->text_field->rect.w/2)-5;
            lifecount->text_field->rect.y = WINDOW_HEIGHT-20;
            lifecount->text_field->rect.w = lifecount->text_field->rect.w / 2;
            lifecount->text_field->rect.h = lifecount->text_field->rect.h / 2;
            SDL_RenderCopyEx(ren, lifecount->texture, NULL, &(lifecount->text_field->rect), 0, NULL, SDL_FLIP_NONE);
            SDL_DestroyTexture(lifecount->texture);
            
            //pacman exist - eat
            if (pacman->nexist == 0)
            {
                SDL_RenderCopyEx(ren, texture, NULL, &(pacman->rect), angle, NULL, SDL_FLIP_NONE);
                atom_move(pacman);
                eat(ghost, pacman);
            } else if (life > 1) {
                life -= 1;
                printf("life: %d\n", life);
                pacman->nexist = 0;
                angle = 0;
                pacman->rect.x = pcm_x;
                pacman->rect.y = pcm_y;
                pacman->speed_x = pcm_sx;
                pacman->speed_y = pcm_sy;
            } else {
                life = 0;
                printf("Final score: %d\n", score);
                quit = true;
            }

            //ghost render
            SDL_RenderCopyEx(ren, gtexture, NULL, &(ghost->rect), 0, NULL, SDL_FLIP_NONE);
            atom_move(ghost);
            ghost_chase(pacman, ghost);  

        } else {
            //Rendering scoreboard
            Text* scoreboard = gen_text("Scoreboard", font, ren, 500, 20);
            scoreboard->text_field->rect.x = (WINDOW_WIDTH / 2) - (scoreboard->width / 2);
            SDL_RenderCopyEx(ren, scoreboard->texture, NULL, &(scoreboard->text_field->rect), 0, NULL, SDL_FLIP_NONE);
            SDL_DestroyTexture(scoreboard->texture);
            
            //gen_text("...", font, ren, 330, 50);
            //gen_text("Honza - 1000 pts", font, ren, 330, 80);
            //gen_text("Honza - 400 pts", font, ren, 330, 110);
            //gen_text("Press RETURN to proceed", font, ren, 330, 200);
        }
        SDL_RenderPresent(ren);
    }
    
    //flush ram
    atom_delete(pacman);
    atom_delete(ghost);
    atom_delete(array);

    SDL_DestroyTexture(texture);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    IMG_Quit();

    TTF_CloseFont(font);
    TTF_Quit();
    
    SDL_Quit();

    return 0;
}
