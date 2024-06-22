#ifndef HEADER_H
#define HEADER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <stdbool.h>

#define SPEED 0.004
#define GRAVITE 0.003
#define JUMPFORCE 0.02
static int HEIGHTJUMP = 10;
static int KNOCKBACK = 2;
static int KNOCKBACKFORCE = 3;

typedef struct Terrain {
    float x_factor, y_factor, w_factor, h_factor;
    bool sol;
} Terrain;

typedef struct Niveau {
    Terrain *surfaces;
    int nb_surfaces;
    char *src_sol;
    char *src_plateform;
    char *src_fond;
} Niveau;

typedef struct Vector {
    short int x, y;
} Vector;

typedef struct Joueur {
    int vie;
    float x_factor, y_factor, w_factor, h_factor;
    Vector direction;
    short percent;
    // pour les déplacements
    int isJumping;
    bool jump;
    bool isGrounded;
    // pour les coups reçus
    int cpt_knockBack;
    bool isKnockBack;
    Vector knockBack_direction;
} Joueur;

Terrain constructeur_terrain(float x, float y, bool sol);
void constructeur_niveau(Niveau *niveau);
void destructeur_niveau(Niveau *niveau);
Joueur constructeur_joueur();
void deplacement(Joueur *joueur1, Joueur *joueur2, Niveau Niveau, SDL_Surface *surface);
void gravite(Joueur *joueur1, Niveau niveau, SDL_Surface *surface);
void frappe(Joueur *attaquant, Joueur *cible, SDL_Surface *surface);
void forces(Joueur *joueur);
void is_FarAway(Joueur *joueur1, SDL_Surface *window);

int menu_screen(SDL_Window **window, SDL_Renderer **renderer);
int gameover_screen(SDL_Window **window, SDL_Renderer **renderer, char *whowin);

#endif // HEADER_H

