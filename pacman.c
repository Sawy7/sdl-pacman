#include "pacman.h"

Entity* entity_create(int pos_x, int pos_y, int w, int h, int speed_x, int speed_y)
{
    Entity* a = (Entity*) malloc(sizeof(Entity));
    a->rect.x = pos_x;
    a->rect.y = pos_y;
    a->rect.w = w;
    a->rect.h = h;
    a->speed_x = speed_x;
    a->speed_y = speed_y;
    a->nexist = 0;
    return a;
}

void entity_delete(Entity* a)
{
    free(a);
}

void entity_move(Entity* a)
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

void ghost_chase(Entity* a, Entity* b)
{
    if (a->rect.x > b->rect.x)
    {
        b->speed_x = 1;
    } else if (a->rect.x < b->rect.x) {
        b->speed_x = -1;
    } else if (a->rect.x == b->rect.x) {
        b->speed_x = 0;
    }
    if (a->rect.y > b->rect.y)
    {
        b->speed_y = 1;
    } else if (a->rect.y < b->rect.y) {
        b->speed_y = -1;
    } else if (a->rect.y == b->rect.y) {
        b->speed_y = 0;
    }
}

void ghost_chase_imp(Entity* a, Entity* b)
{
    int xabs = abs(a->rect.x - b->rect.x);
    int yabs = abs(a->rect.y - b->rect.y);
    int speed = 1;
    if (xabs >= yabs)
    {
        b->speed_y = 0;
        if (a->rect.x > b->rect.x)
        {
            b->speed_x = speed;
        } else {
            b->speed_x = -speed;
        }        
    } else {
        b->speed_x = 0;
        if (a->rect.y > b->rect.y)
        {
            b->speed_y = speed;
        } else {
            b->speed_y = -speed;
        }
    }
}

void entity_collision(Entity* a, Entity* wall)
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

void ghost_collision(Entity* a, Entity* wall)
{
    int speed = 1;
    //Ghost
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
        a->speed_x = speed;
        a->rect.y = wall_top - a->rect.h;
    }
    if (a_top < wall_bottom && a_top > (wall_bottom - (wall->rect.h*0.25)) && a->speed_y < 0 && a_right > wall_left && a_left < wall_right)
    {
        a->speed_y = 0;
        a->speed_x = speed;
        a->rect.y = wall_bottom;
    }
    if (a_right > wall_left && a_right < (wall_left + (wall->rect.w*0.25)) && a->speed_x > 0 && a_bottom > wall_top && a_top < wall_bottom)
    {
        a->speed_x = 0;
        a->speed_y = speed;
        a->rect.x = wall_left - a->rect.w;
    }
    if (a_left < wall_right && a_left > (wall_right - (wall->rect.w*0.25)) && a->speed_x < 0 && a_bottom > wall_top && a_top < wall_bottom)
    {
        a->speed_x = 0;
        a->speed_y = speed;
        a->rect.x = wall_right;
    }
}

void ghost_boundaries(Entity* a, int sector)
{
    int v_divide = WINDOW_WIDTH/2;
    int h_divide = WINDOW_HEIGHT/2;

    if (sector == 0)
    {
        if (a->rect.x+a->rect.w >= v_divide)
        {
            a->rect.x = v_divide - a->rect.w;
        }
        if (a->rect.y+a->rect.h >= h_divide)
        {
            a->rect.y = h_divide - a->rect.h;
        }
    } else if (sector == 1) {
        if (a->rect.x <= v_divide)
        {
            a->rect.x = v_divide;
        }
        if (a->rect.y+a->rect.h >= h_divide)
        {
            a->rect.y = h_divide - a->rect.h;
        }
    } else if (sector == 2) {
        if (a->rect.x+a->rect.w >= v_divide)
        {
            a->rect.x = v_divide - a->rect.w;
        }
        if (a->rect.y <= h_divide)
        {
            a->rect.y = h_divide;
        }
    } else if (sector == 3) {
        if (a->rect.x <= v_divide)
        {
            a->rect.x = v_divide;
        }
        if (a->rect.y <= h_divide)
        {
            a->rect.y = h_divide;
        }
    }
}

void eat (Entity* a, Entity* p)
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

Entity* point_gen (int r)
{
    int rows = (WINDOW_HEIGHT/r);
    int cols = (WINDOW_WIDTH/r);
    int count = cols * rows;
    Entity* array = (Entity*) malloc(sizeof(Entity)*count);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            array[i*cols+j] = *(entity_create(j*r, i*r, r, r, 0, 0));
        }
    }
    return array;    
}

int point_count (Entity array[])
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

void just_go(Entity* a, char direction[])
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

Text* gen_text(char text[], TTF_Font* font, SDL_Renderer* ren, int x, int y)
{
    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    int width;
    int height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height); 
    Entity* text_field = entity_create(x, y, width, height, 0, 0);
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

Text** read_scoreboard(TTF_Font* font, SDL_Renderer* ren)
{
    int count = 5;
    Text** array = (Text**) malloc(sizeof(Text)*6);
    FILE* f = fopen("scoreboard.txt", "r");
    int score;
    char score_str[10];
    for (int i = 0; i < count; i++)
    {
        fscanf(f,"%d", &score);
        sprintf(score_str, "%d.     %d", (i+1), score);
        array[i] = gen_text(score_str, font, ren, 500, 100+35*i);
        //printf("%s\n", score_str);
    }
    array[5] = gen_text("Press RETURN to proceed...", font, ren, 500, WINDOW_HEIGHT-100);
    fclose(f);
    return array;
}

void endgame(Entity array[])
{
    int latestScore = point_count (array);
    int count = 5;
    printf("latest score: %d\n", latestScore);
    //printf("latest score: %d\n", latestScore);
    int score;

    FILE* f = fopen("scoreboard.txt", "r");
    int newScores[5];
    for (int i = 0; i < count; i++)
    {
        fscanf(f,"%d", &score);
        if (score < latestScore)
        {
            newScores[i] = latestScore;
            newScores[i+1] = score;
            latestScore = 0;
            i += 1;
        } else {
            newScores[i] = score;
        }
    }
    fclose(f);
    FILE* g = fopen("scoreboard.txt", "w");
    for (int i = 0; i < count; i++)
    {
        fprintf(g, "%d\n", newScores[i]);
        printf("saving score: %d\n", newScores[i]);
    }
}