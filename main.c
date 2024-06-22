#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"

int init_window(SDL_Window **window, SDL_Renderer **renderer);
int update_window(SDL_Window **window, SDL_Renderer **renderer);
int gameover_screen(SDL_Window **window, SDL_Renderer **renderer, char *whowin);

void run_game(SDL_Window **window, SDL_Renderer **renderer);

int main(int argc, char const *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur in SDL init: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    int init = init_window(&window, &renderer);
    if(init != 0) return EXIT_FAILURE;

    /*-------------------------------SI JAMAIS ON DOIT AFFICHER DU TEXTE----------------------*/
    if (TTF_Init() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        return EXIT_FAILURE;
    }
    /*----------------------------------------------------------------------------------------*/

    run_game(&window, &renderer);

    //destroy
    if(renderer != NULL) SDL_DestroyRenderer(renderer);
    if(window != NULL) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void run_game(SDL_Window **window, SDL_Renderer **renderer) {
    bool want_to_play = true;
    int exit;
    int choice;

    while(want_to_play) {
        choice = menu_screen(window, renderer);
        switch (choice) {
            case 0:
                exit = update_window(window, renderer);
                if (exit == 1) want_to_play = false;
                break;
            case 1:
                want_to_play = false;
                break;
        }
    }
}

int init_window(SDL_Window **window, SDL_Renderer **renderer) {
    if(SDL_CreateWindowAndRenderer(960, 540, SDL_WINDOW_RESIZABLE, window, renderer) != 0) {
        printf("%s\n", SDL_GetError());
        return -1;
    }

    SDL_SetWindowTitle(*window, "sdl_screen");

    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);
    return 0;
}

int update_window(SDL_Window **window, SDL_Renderer **renderer) {
    SDL_Event events;
    bool isOpen = true;
    int exit_status = 0;

    /*-----------------------INITIALISATION DU NIVEAU ET DE SES IMAGES------------------------*/
    Niveau niveau;
    constructeur_niveau(&niveau);
    SDL_Surface *image_sol = IMG_Load(niveau.src_sol);
    SDL_Surface *image_plateform = IMG_Load(niveau.src_plateform);
    SDL_Surface *image_fond = IMG_Load(niveau.src_fond);
    SDL_Surface *image_vie = IMG_Load("assets/gravestone.jpg");

    SDL_Texture *textureImage = SDL_CreateTextureFromSurface(*renderer, image_sol);
    SDL_Texture *texturePlateform = SDL_CreateTextureFromSurface(*renderer, image_plateform);
    SDL_Texture *textureFond = SDL_CreateTextureFromSurface(*renderer, image_fond);
    SDL_Texture *textureVie = SDL_CreateTextureFromSurface(*renderer, image_vie);
    SDL_FreeSurface(image_sol);
    SDL_FreeSurface(image_plateform);
    SDL_FreeSurface(image_fond);
    SDL_FreeSurface(image_vie);

    if(textureFond == NULL || textureImage == NULL || texturePlateform == NULL ||textureVie == NULL){
        printf("erreur lors du chargement de l'image: %s\n", SDL_GetError());
    }

    SDL_Rect rect_image = {0, 0, 0, 0};
    SDL_Rect rect_plateform = {0, 0, 0, 0};
    SDL_Rect rect_fond = {0, 0, 0, 0};

    SDL_QueryTexture(textureImage, NULL, NULL, &rect_image.w, &rect_image.h);
    SDL_QueryTexture(texturePlateform, NULL, NULL, &rect_plateform.w, &rect_plateform.h);
    SDL_QueryTexture(texturePlateform, NULL, NULL, &rect_fond.w, &rect_fond.h);
    /*----------------------------------------------------------------------------------------*/

    /*INITIALISATION DES JOUEURS*/
    Joueur joueur1 = constructeur_joueur();
    Joueur joueur2 = constructeur_joueur();
    /*--------------------------*/

    //pour afficher les pourcentage des joueurs
    TTF_Font* font_percent = TTF_OpenFont("assets/fonts/Samson.ttf", 10);
    if(!font_percent){
        printf("error : %s\n", TTF_GetError());
    }
    char percent1[256] = "";
    char percent2[256] = "";
    SDL_Color color_percent = { 255, 255, 0, 255 };
    SDL_Rect rect_font1;
    SDL_Rect rect_font2;

    while (isOpen) {
        /*affichage de la fenetre blanche*/
        if(SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255) != 0){
            printf("probleme lors du changement de la couleur: %s\n", SDL_GetError() );
        }
        SDL_RenderClear(*renderer);
        SDL_Surface *surface = SDL_GetWindowSurface(*window); //pour récuperer les info de la fenetre, dans le while pour avoir les forme en fonction de la fenetre
        /*------------------------------*/

        /*-------------------------------------------------AFFICHAGE DU FOND DU NIVEAU----------------------------------------*/
        SDL_Rect rect_fond = {0,0,surface->w, surface->h};
        if(SDL_RenderCopy(*renderer, textureFond, NULL, &rect_fond) != 0){
            SDL_RenderFillRect(*renderer, &rect_fond); //qu'on affiche quelque chose quand meme
            printf("probleme : %s", SDL_GetError());
        }
        /*--------------------------------------------------------------------------------------------------------------------*/

        /*----------------------------GESTION DES EVENEMENTS-------------------------*/
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                case SDL_QUIT:
                    exit_status = 1;
                    isOpen = false;
                    break;

                case SDL_KEYDOWN:
                    if(events.key.keysym.scancode == SDL_SCANCODE_W){
                        if(joueur1.isGrounded) joueur1.jump = true;
                    }

                    if(events.key.keysym.scancode == SDL_SCANCODE_UP){
                        if(joueur2.isGrounded) joueur2.jump = true;
                    }
                    break;
                case SDL_KEYUP:
                    if( events.key.keysym.scancode == SDL_SCANCODE_Z) frappe(&joueur1, &joueur2,surface);

                    if( events.key.keysym.scancode == SDL_SCANCODE_KP_2) frappe(&joueur2, &joueur1,surface);
            }
        }

        /*----------------------------GERE LES DEPLACEMENT + GRAVITE--------------------------------*/
        deplacement(&joueur1, &joueur2, niveau, surface);

        forces(&joueur1);
        forces(&joueur2);

        if(!joueur1.jump || !joueur1.isGrounded ) gravite(&joueur1, niveau, surface);
        if(!joueur2.jump || !joueur2.isGrounded ) gravite(&joueur2, niveau, surface);

        is_FarAway(&joueur1, surface);
        is_FarAway(&joueur2, surface);
        /*------------------------------------------------------------------------------------------*/

        //FAIRE UNE FONCTION
        /*---------------------------------------AFFICHAGES DES SURFACE-----------------------------*/
        for(int i=0; i<niveau.nb_surfaces; i++){
            SDL_Rect rectangle = {surface->w * niveau.surfaces[i].x_factor, surface->h * niveau.surfaces[i].y_factor, surface->w * niveau.surfaces[i].w_factor, surface->h * niveau.surfaces[i].h_factor};

            if(SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255) != 0){
                printf("probleme lors du changement de la couleur: %s\n", SDL_GetError() );
            }

            if(niveau.surfaces[i].sol){
                if(SDL_RenderCopy(*renderer, textureImage, NULL, &rectangle) != 0){
                    SDL_RenderFillRect(*renderer, &rectangle); //qu'on affiche quelque chose quand meme
                    printf("probleme : %s", SDL_GetError());
                }
            }
            else{
                if(SDL_RenderCopy(*renderer, texturePlateform, NULL, &rectangle) != 0){
                    SDL_RenderFillRect(*renderer, &rectangle); //qu'on affiche quelque chose quand meme
                    printf("probleme : %s", SDL_GetError());
                }
            }
        }
        /*-----------------------------------------------------------------------------------------*/


        /*------------------------------------AFFICHAGE DES JOUEURS----------------------------------*/
        SDL_Rect j1 = {surface->w * joueur1.x_factor, surface->h * joueur1.y_factor, surface->w * joueur1.w_factor, surface->h * joueur1.h_factor};
        if(SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255) != 0){
            printf("probleme lors du changement de la couleur: %s\n", SDL_GetError() );
        }
        SDL_RenderFillRect(*renderer, &j1);

        SDL_Rect j2 = {surface->w * joueur2.x_factor, surface->h * joueur2.y_factor, surface->w * joueur2.w_factor, surface->h * joueur2.h_factor};
        if(SDL_SetRenderDrawColor(*renderer, 0, 0, 255, 255) != 0){
            printf("probleme lors du changement de la couleur: %s\n", SDL_GetError() );
        }
        SDL_RenderFillRect(*renderer, &j2);
        /*-------------------------------------------------------------------------------------------*/

        /*----------------------------------------AFFICHAGE DES POURCENTAGE DES JOUEURS---------------------------------------*/
        sprintf(percent1, "%d %%",joueur1.percent);
        sprintf(percent2, "%d %%",joueur2.percent);
        SDL_Surface *texte_joueur1 = TTF_RenderText_Solid(font_percent, percent1, color_percent);
        SDL_Surface *texte_joueur2 = TTF_RenderText_Solid(font_percent, percent2, color_percent);

        SDL_Texture* texture_texte1 = SDL_CreateTextureFromSurface(*renderer, texte_joueur1);
        SDL_Texture* texture_texte2 = SDL_CreateTextureFromSurface(*renderer, texte_joueur2);

        SDL_RenderCopy(*renderer, texture_texte1, NULL, &rect_font1);
        SDL_RenderCopy(*renderer, texture_texte2, NULL, &rect_font2);

        rect_font1.x = surface->w * 0.03125;
        rect_font1.y = surface->h * 0.8;
        rect_font1.w = surface->w * 0.125;
        rect_font1.h = surface->h * 0.16;

        rect_font2.x = surface->w * 0.875;
        rect_font2.y = surface->h * 0.8;
        rect_font2.w = surface->w * 0.125;
        rect_font2.h = surface->h * 0.16;
        /*--------------------------------------------------------------------------------------------------------------------*/

        /*-----------------------------------------AFFICHAGE DE LA VIE DES JOUEURS--------------------------------------------*/
        //joueur1
        for(int i=0; i < joueur1.vie; i++){
            //afficher l'image
            SDL_Rect rectangle_image1 = {surface->w * (0.03125 + (i * 0.03125 * 1.3)), surface->h * 0.94, surface->w * 0.03125, surface->h * 0.05 };

            if(SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255) != 0){
                printf("probleme lors du changement de la couleur: %s\n", SDL_GetError() );
            }

            if(SDL_RenderCopy(*renderer, textureVie, NULL, &rectangle_image1) != 0){
                SDL_RenderFillRect(*renderer, &rectangle_image1); //qu'on affiche quelque chose quand meme
                printf("probleme : %s", SDL_GetError());
            }
        }

        //joueur2
        for(int i=0; i < joueur2.vie; i++){
            //afficher l'image
            SDL_Rect rectangle_image1 = {surface->w * (0.875 + (i * 0.03125 * 1.3)), surface->h * 0.94, surface->w * 0.03125, surface->h * 0.05 };

            if(SDL_SetRenderDrawColor(*renderer, 0, 0, 255, 255) != 0){
                printf("probleme lors du changement de la couleur: %s\n", SDL_GetError() );
            }

            if(SDL_RenderCopy(*renderer, textureVie, NULL, &rectangle_image1) != 0){
                SDL_RenderFillRect(*renderer, &rectangle_image1); //qu'on affiche quelque chose quand meme
                printf("probleme : %s", SDL_GetError());
            }
        }

        /*--------------------------------------------------------------------------------------------------------------------*/

        // Vérifier les vies des joueurs et afficher l'écran de game over si nécessaire
        if (joueur1.vie <= 0 || joueur2.vie <= 0) {
            char *winner = joueur1.vie > 0 ? "Joueur 1 a gagne!!!" : "Joueur 2 a gagne!!!";
            int gameover_choice = gameover_screen(window, renderer, winner);
            if (gameover_choice == 0) {  // Rejouer
                isOpen = false;
                update_window(window, renderer);
                break;
            } else {  // Quitter
                exit_status = 1;
                isOpen = false;
        	 break;
            }
        }

        SDL_FreeSurface(surface);
        SDL_FreeSurface(texte_joueur1);
        SDL_FreeSurface(texte_joueur2);
        SDL_DestroyTexture(texture_texte1);
        SDL_DestroyTexture(texture_texte2);
        /*mise a jour de la fenetre*/
        SDL_RenderPresent(*renderer);
    }

    SDL_DestroyTexture(textureImage);
    SDL_DestroyTexture(texturePlateform);
    SDL_DestroyTexture(textureFond);
    SDL_DestroyTexture(textureVie);
    destructeur_niveau(&niveau);
    TTF_CloseFont(font_percent);

    return exit_status;
}

