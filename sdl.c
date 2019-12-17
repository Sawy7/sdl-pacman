#include "pacman.h"

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
    img = IMG_Load("ghost2.png");
    if(!img)
    {
        fprintf(stderr, "SDL_IMG_Load Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }
    SDL_Texture* g2texture = SDL_CreateTextureFromSurface(ren, img);
    if(!g2texture)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }
    img = IMG_Load("ghost3.png");
    if(!img)
    {
        fprintf(stderr, "SDL_IMG_Load Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }
    SDL_Texture* g3texture = SDL_CreateTextureFromSurface(ren, img);
    if(!g3texture)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }
    img = IMG_Load("ghost4.png");
    if(!img)
    {
        fprintf(stderr, "SDL_IMG_Load Error: %s", SDL_GetError()); 
        SDL_Quit();
        return 1;
    }
    SDL_Texture* g4texture = SDL_CreateTextureFromSurface(ren, img);
    if(!g4texture)
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
    if(!ptexture)
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
    Entity* pacman = entity_create(pcm_x, pcm_y, pcm_r, pcm_r, pcm_sx, pcm_sy);

    //ghost defined
    Entity* ghost = entity_create(60, 100, 20, 20, 0, 0);
    Entity* ghost2 = entity_create(WINDOW_WIDTH-80, 100, 20, 20, 0, 0);
    Entity* ghost3 = entity_create(60, WINDOW_HEIGHT-120, 20, 20, 0, 0);
    Entity* ghost4 = entity_create(WINDOW_WIDTH-80, WINDOW_HEIGHT-120, 20, 20, 0, 0);


    //generate points
    Entity* array = point_gen(20);
    
    //keeping score
    int screenCount = ((WINDOW_WIDTH/array[0].rect.h)*(WINDOW_HEIGHT/array[0].rect.h));
    int pointTotal;
    int pointCount;
    int score;

    
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
                endgame(array);
                quit = true;
            }
            //key events
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_r)
                {
                    endgame(array);
                    quit = true;
                }
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    scoreboard = false;
                }
                if (e.key.keysym.sym == SDLK_LEFT)
                {
                    int y_correction = pacman->rect.y % pacman->rect.h;
                    if (y_correction < pacman->rect.h/2)
                    {
                        pacman->rect.y -= y_correction;
                    } else {
                        pacman->rect.y = pacman->rect.y - y_correction + pacman->rect.h;
                    }
                    just_go(pacman, "left");
                    angle = 180;
                }
                if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    int y_correction = pacman->rect.y % pacman->rect.h;
                    if (y_correction < pacman->rect.h/2)
                    {
                        pacman->rect.y -= y_correction;
                    } else {
                        pacman->rect.y = pacman->rect.y - y_correction + pacman->rect.h;
                    }
                    just_go(pacman, "right");
                    angle = 0;
                }
                if (e.key.keysym.sym == SDLK_UP)
                {
                    int x_correction = pacman->rect.x % pacman->rect.w;
                    if (x_correction < pacman->rect.w/2)
                    {
                        pacman->rect.x -= x_correction;
                    } else {
                        pacman->rect.x = pacman->rect.x - x_correction + pacman->rect.w;
                    }  
                    just_go(pacman, "up");
                    angle = -90;
                }
                if (e.key.keysym.sym == SDLK_DOWN)
                {
                    int x_correction = pacman->rect.x % pacman->rect.w;
                    if (x_correction < pacman->rect.w/2)
                    {
                        pacman->rect.x -= x_correction;
                    } else {
                        pacman->rect.x = pacman->rect.x - x_correction + pacman->rect.w;
                    }                   
                    just_go(pacman, "down");
                    angle = 90;
                }
            }
        }

        //render background
        SDL_SetRenderDrawColor(ren, 0, 89, 179, 255);
        SDL_RenderClear(ren);

        if (scoreboard != true)
        {
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_RenderClear(ren);
            pointCount = 0;
            //multiple points
            for (int i = 0; i < screenCount; i++)
            {
                if (array[i].nexist !=1 && marray[i] == '.')
                {
                    SDL_RenderCopyEx(ren, ptexture, NULL, &(array[i].rect), 0, NULL, SDL_FLIP_NONE);
                    eat(pacman, &array[i]);
                    pointCount += 1;
                }
            }
            //printf("How many points: %d\n", pointCount);
            if (pointCount == 0)
            {
                endgame(array);
                quit = true;
            }
            

            //multiple walls
            pointTotal = screenCount;
            for (int i = 0; i < screenCount; i++)
            {
                if (marray[i] == '#')
                {
                    SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
                    SDL_RenderFillRect(ren, &array[i].rect);
                    SDL_RenderCopyEx(ren, NULL, NULL, &(array[i].rect), 0, NULL, SDL_FLIP_NONE);
                    entity_collision(pacman, &(array[i]));
                    ghost_collision(ghost, &(array[i]));
                    ghost_collision(ghost2, &(array[i]));
                    ghost_collision(ghost3, &(array[i]));
                    ghost_collision(ghost4, &(array[i]));
                    pointTotal -= 1;
                }
            }
            
            //printf("How many points TOTAL: %d\n", pointTotal);

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

            //life text render
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
                entity_move(pacman);
                eat(ghost, pacman);
                if (pacman->nexist == 0)
                {
                    eat(ghost2, pacman);
                }
                if (pacman->nexist == 0)
                {
                    eat(ghost3, pacman);
                }
                if (pacman->nexist == 0)
                {
                    eat(ghost4, pacman);
                }
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
                endgame(array);
                quit = true;
            }

            //ghost render
            SDL_RenderCopyEx(ren, gtexture, NULL, &(ghost->rect), 0, NULL, SDL_FLIP_NONE);
            entity_move(ghost);
            ghost_chase_imp(pacman, ghost);
            SDL_RenderCopyEx(ren, g2texture, NULL, &(ghost2->rect), 0, NULL, SDL_FLIP_NONE);
            entity_move(ghost2);
            ghost_chase_imp(pacman, ghost2);
            SDL_RenderCopyEx(ren, g3texture, NULL, &(ghost3->rect), 0, NULL, SDL_FLIP_NONE);
            entity_move(ghost3);
            ghost_chase_imp(pacman, ghost3);
            SDL_RenderCopyEx(ren, g4texture, NULL, &(ghost4->rect), 0, NULL, SDL_FLIP_NONE);
            entity_move(ghost4);
            ghost_chase_imp(pacman, ghost4);

            //ghost sectors
            ghost_boundaries(ghost, 0);
            ghost_boundaries(ghost2, 1);
            ghost_boundaries(ghost3, 2);
            ghost_boundaries(ghost4, 3);


        } else {
            //Rendering scoreboard
            Text* scoreboard = gen_text("Best Scores:", font, ren, 500, 20);
            scoreboard->text_field->rect.x = (WINDOW_WIDTH / 2) - (scoreboard->width / 2);
            SDL_RenderCopyEx(ren, scoreboard->texture, NULL, &(scoreboard->text_field->rect), 0, NULL, SDL_FLIP_NONE);
            SDL_DestroyTexture(scoreboard->texture);

            Text** scoreboard_entry = read_scoreboard(font, ren);
            for (int i = 0; i < 6; i++)
            {
                scoreboard_entry[i]->text_field->rect.x = (WINDOW_WIDTH / 2) - (scoreboard_entry[i]->width / 2);
                SDL_RenderCopyEx(ren, scoreboard_entry[i]->texture, NULL, &(scoreboard_entry[i]->text_field->rect), 0, NULL, SDL_FLIP_NONE);
                SDL_DestroyTexture(scoreboard_entry[i]->texture);
            }
        }
        SDL_RenderPresent(ren);
    }
    
    //flush ram
    entity_delete(pacman);
    entity_delete(ghost);
    entity_delete(ghost2);
    entity_delete(ghost3);
    entity_delete(ghost4);
    entity_delete(array);

    SDL_DestroyTexture(texture);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    IMG_Quit();

    TTF_CloseFont(font);
    TTF_Quit();
    
    SDL_Quit();

    return 0;
}