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
            int screenCount = ((WINDOW_WIDTH/array[0].rect.h)*(WINDOW_HEIGHT/array[0].rect.h));
            int pointCount = 0;
            int pointTotal = screenCount;

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

            //multiple walls
            for (int i = 0; i < screenCount; i++)
            {
                if (marray[i] == '#')
                {
                    SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
                    SDL_RenderFillRect(ren, &array[i].rect);
                    SDL_RenderCopyEx(ren, NULL, NULL, &(array[i].rect), 0, NULL, SDL_FLIP_NONE);
                    atom_collision(pacman, &(array[i]));
                    atom_collision(ghost, &(array[i]));
                    pointTotal -= 1;
                }
            }
            //printf("How many points TOTAL: %d\n", pointTotal);

            //count points
            int score = point_count(array);
            //printf("%d\n", score);
            char strscore[10];
            sprintf(strscore, "%d", score);

            //endgame
            endgame(life, pointCount, pointTotal);
            
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
                endgame(life, pointCount, pointTotal);
                quit = true;
            }

            //ghost render
            SDL_RenderCopyEx(ren, gtexture, NULL, &(ghost->rect), 0, NULL, SDL_FLIP_NONE);
            atom_move(ghost);
            ghost_chase_imp(pacman, ghost);  

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
