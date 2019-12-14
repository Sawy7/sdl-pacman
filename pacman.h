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
} Entity;

typedef struct
{
    SDL_Texture* texture;
    Entity* text_field;
    int width;
    int height;
} Text;


Entity* entity_create(int pos_x, int pos_y, int w, int h, int speed_x, int speed_y);
void entity_delete(Entity* a);
void entity_move(Entity* a);
void ghost_chase(Entity* a, Entity* b);
void ghost_chase_imp(Entity* a, Entity* b);
void entity_collision(Entity* a, Entity* wall);
void ghost_collision(Entity* a, Entity* wall);
void ghost_boundaries(Entity* a, int sector);
void eat (Entity* a, Entity* p);
Entity* point_gen (int r);
int point_count (Entity array[]);
void just_go(Entity* a, char direction[]);
Text* gen_text(char text[], TTF_Font* font, SDL_Renderer* ren, int x, int y);
char* read_map(int r);
Text** read_scoreboard(TTF_Font* font, SDL_Renderer* ren);
void endgame(Entity array[]);