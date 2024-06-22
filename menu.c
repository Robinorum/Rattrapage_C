#include "header.h"

int menu_screen(SDL_Window **window, SDL_Renderer **renderer){
  SDL_Event events;
  bool isOpen = true;
  int exit_status = 0;

  int x_mouse, y_mouse;

  TTF_Font* texte_font = TTF_OpenFont("assets/fonts/Samson.ttf", 10);
  if(!texte_font){
    printf("error : %s\n", TTF_GetError());
  }

  char *quit = "QUITTER";
  char *play = "JOUER";
  SDL_Color color_percent = { 255, 255, 0, 255 };
  SDL_Rect rect_font1;
  SDL_Rect rect_font2;

  while(isOpen){

    /*affichage de la fenetre blanche*/
    if(SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255) != 0){
        printf("probleme lors du changement de la couleur: %s\n", SDL_GetError() );
    }
    SDL_RenderClear(*renderer);
    SDL_Surface *surface = SDL_GetWindowSurface(*window); //pour récuperer les info de la fenetre, dans le while pour avoir les forme en fonction de la fenetre
    /*------------------------------*/

    while (SDL_PollEvent(&events))
    {
        switch (events.type)
        {
          case SDL_QUIT:
            isOpen = false;
            exit_status = 1;
            break;

          case SDL_KEYDOWN:
            break;
          case SDL_KEYUP:
            break;

          case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x_mouse, &y_mouse);

            if(x_mouse >= surface->w*0.25 && x_mouse <= surface->w*0.75){
              if(y_mouse >= surface->h*0.25 && y_mouse <= surface->h*0.35){
                exit_status = 0;
              }

              else if(y_mouse >= surface->h*0.75 && y_mouse <= surface->h*0.85){
                exit_status = 1;
              }
            }
            isOpen = false;
        }
    }


    if(SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255) != 0){
        printf("probleme lors du changement de la couleur: %s\n", SDL_GetError() );
    }
    SDL_Rect play_button = {surface->w * 0.25, surface->h* 0.25, surface->w * 0.5, surface->h * 0.1};
    SDL_Rect quit_button = {surface->w * 0.25, surface->h* 0.75, surface->w * 0.5, surface->h * 0.1};
    SDL_RenderFillRect(*renderer, &play_button);
    SDL_RenderFillRect(*renderer, &quit_button);


    SDL_Surface *surface_button1 = TTF_RenderText_Solid(texte_font, play, color_percent);
    SDL_Surface *surface_button2 = TTF_RenderText_Solid(texte_font, quit, color_percent);

    SDL_Texture* texture_button1 = SDL_CreateTextureFromSurface(*renderer, surface_button1);
    SDL_Texture* texture_button2 = SDL_CreateTextureFromSurface(*renderer, surface_button2);

    SDL_RenderCopy(*renderer, texture_button1, NULL, &rect_font1);
    SDL_RenderCopy(*renderer, texture_button2, NULL, &rect_font2);

    rect_font1.x = surface->w * 0.375;
    rect_font1.y = surface->h * 0.25;
    rect_font1.w = surface->w * 0.25;
    rect_font1.h = surface->h * 0.1;

    rect_font2.x = surface->w * 0.375;
    rect_font2.y = surface->h * 0.75;
    rect_font2.w = surface->w * 0.25;
    rect_font2.h = surface->h * 0.1;

    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface_button1);
    SDL_FreeSurface(surface_button2);
    SDL_DestroyTexture(texture_button1);
    SDL_DestroyTexture(texture_button2);


    /*mise a jour de la fenetre*/
    SDL_RenderPresent(*renderer);

  }

  TTF_CloseFont(texte_font);
  return exit_status;   //pour savoir comment le joueur à quitter le jeu.

}
