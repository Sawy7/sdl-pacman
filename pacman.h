#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

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


Atom* atom_create(int pos_x, int pos_y, int w, int h, int speed_x, int speed_y);
void atom_delete(Atom* a);
void atom_move(Atom* a);
void ghost_chase(Atom* a, Atom* b);
void ghost_chase_imp(Atom* a, Atom* b);
void atom_collision(Atom* a, Atom* wall);
void eat (Atom* a, Atom* p);
Atom* point_gen (int r);
int point_count (Atom array[]);
void just_go(Atom* a, char direction[]);
Text* gen_text(char text[], TTF_Font* font, SDL_Renderer* ren, int x, int y);
char* read_map(int r);
Text** read_scoreboard(TTF_Font* font, SDL_Renderer* ren);
void endgame(int life, int pointCount, int pointTotal);