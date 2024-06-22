#include "header.h"

Joueur constructeur_joueur(){
  Vector vect = {0,0};
  Vector kb_direction = {0,0};
  Joueur joueur = {3, 0.1875, 0.3, 0.03125, 0.056, vect, 0, 0, false, false, 0, false, kb_direction};
  //vie, coordonnées, vect de direction, pourcentage de degat, le temp de jump, le bool de jump, le bool grounded, temps knocked back, bool knockback, direction knockback

  return joueur;
}


void deplacement(Joueur *joueur1, Joueur *joueur2, Niveau niveau, SDL_Surface *surface){
  const Uint8 *clavier;

  SDL_PumpEvents();
  clavier = SDL_GetKeyboardState(NULL);


  if(clavier[SDL_SCANCODE_D]){
    bool collision = false;
    //definition du rectangle representant le cote droit du joueur
    SDL_Rect j1 = {surface->w * (joueur1->w_factor+ joueur1->w_factor), surface->h * joueur1->y_factor, surface->w * 0.005, surface->h * joueur1->h_factor};

    for(int i=0; i< niveau.nb_surfaces; i++){
      //definition de la ligne gauche de la surface
      int x1_g = (int) surface->w * niveau.surfaces[i].x_factor;
      int y1_g = (int) surface->h * niveau.surfaces[i].y_factor;
      int x2_g = (int) surface->w * niveau.surfaces[i].x_factor;
      int y2_g = (int) surface->h * (niveau.surfaces[i].y_factor + niveau.surfaces[i].h_factor);

      if(SDL_IntersectRectAndLine(&j1, &x1_g, &y1_g, &x2_g, &y2_g)){
        collision = true;
      }
    }
    if(!collision){
      joueur1->direction.x = 1;
      joueur1->x_factor += SPEED;
    }
  }


  if(clavier[SDL_SCANCODE_A]){
    bool collision = false;

    //definition du rectangle representant le cote gauche du joueur
    SDL_Rect j1 = {(surface->w * joueur1->x_factor), surface->h * joueur1->y_factor, surface->w*0.005, surface->h * joueur1->h_factor};

    for(int i=0; i< niveau.nb_surfaces; i++){
      //definition de la ligne droite de la surface
      int x1_d = (int) surface->w * (niveau.surfaces[i].x_factor + niveau.surfaces[i].w_factor);
      int y1_d = (int) surface->h * niveau.surfaces[i].y_factor;
      int x2_d = (int) surface->w * (niveau.surfaces[i].x_factor + niveau.surfaces[i].w_factor);
      int y2_d = (int) surface->h * (niveau.surfaces[i].y_factor + niveau.surfaces[i].h_factor);

      if(SDL_IntersectRectAndLine(&j1, &x1_d, &y1_d, &x2_d, &y2_d)){
        collision = true;
      }
    }
    if(!collision){
      joueur1->direction.x = -1;
      joueur1->x_factor -= SPEED;
    }

  }

  if(clavier[SDL_SCANCODE_RIGHT]){
    bool collision = false;

    //definition du rectangle representant le cote droit du joueur
    SDL_Rect j2 = {surface->w * (joueur2->x_factor + joueur2->w_factor), surface->h * joueur2->y_factor, surface->w * 0.005, surface->h * joueur2->h_factor};

    for(int i=0; i< niveau.nb_surfaces; i++){
      //definition de la ligne gauche de la surface
      int x1_g = (int) surface->w * niveau.surfaces[i].x_factor;
      int y1_g = (int) surface->h * niveau.surfaces[i].y_factor;
      int x2_g = (int) surface->w * niveau.surfaces[i].x_factor;
      int y2_g = (int) surface->h * (niveau.surfaces[i].y_factor + niveau.surfaces[i].h_factor);

      if(SDL_IntersectRectAndLine(&j2, &x1_g, &y1_g, &x2_g, &y2_g)){
        collision = true;
      }
    }
    if(!collision){
      joueur2->direction.x = 1;
      joueur2->x_factor += SPEED;
    }
  }


  if(clavier[SDL_SCANCODE_LEFT]){
    bool collision = false;

    //definition du rectangle representant le cote gauche du joueur
    SDL_Rect j2 = {surface->w * joueur2->x_factor , surface->h * joueur2->y_factor, surface->w*0.005, surface->h * joueur2->h_factor};

    for(int i=0; i< niveau.nb_surfaces; i++){
      //definition de la ligne droite de la surface
      int x1_d = (int) surface->w * (niveau.surfaces[i].x_factor + niveau.surfaces[i].w_factor);
      int y1_d = (int) surface->h * niveau.surfaces[i].y_factor;
      int x2_d = (int) surface->w * (niveau.surfaces[i].x_factor + niveau.surfaces[i].w_factor);
      int y2_d = (int) surface->h * (niveau.surfaces[i].y_factor + niveau.surfaces[i].h_factor);

      if(SDL_IntersectRectAndLine(&j2, &x1_d, &y1_d, &x2_d, &y2_d)){  //SDL_IntersectRectAndLine(&j2, &x1_d, &y1_d, &x2_d, &y2_d)
        collision = true;
      }
    }

    if(!collision){
      joueur2->direction.x = -1;
      joueur2->x_factor -= SPEED;
    }
  }

}


void gravite(Joueur *joueur, Niveau niveau, SDL_Surface *surface){
  bool collision = false;

  //definition du rectangle representant le joueur
  SDL_Rect j = {surface->w * joueur->x_factor, surface->h * joueur->y_factor, surface->w * joueur->w_factor, surface->h * joueur->h_factor};

  for(int i=0; i< niveau.nb_surfaces; i++){
    //definition de la ligne superieur de la surface
    int x1_t = (int)surface->w * niveau.surfaces[i].x_factor;
    int y1_t = (int)surface->h * (niveau.surfaces[i].y_factor - 0.003);
    int x2_t = (int)surface->w * (niveau.surfaces[i].x_factor + niveau.surfaces[i].w_factor);
    int y2_t = (int)surface->h * (niveau.surfaces[i].y_factor - 0.003);

    if(SDL_IntersectRectAndLine(&j, &x1_t, &y1_t, &x2_t, &y2_t)){
      collision = true;
    }
  }
  if(!collision){
    joueur->y_factor += GRAVITE; // += car les y descendent
    joueur->isGrounded = false;
  }
  else{
    joueur->isGrounded = true;
  }
}


void frappe(Joueur *attaquant, Joueur *cible, SDL_Surface *surface){
  //definition du rectangle representant le joueur
  SDL_Rect j = {surface->w * attaquant->x_factor, surface->h * attaquant->y_factor, surface->w * attaquant->w_factor, surface->h * attaquant->h_factor};


  if(attaquant->direction.x == 1){    //il attaque vers la droite
    //on creer une ligne representant le cote gauche de la personne attaqué
    int x = (int)surface->w * cible->x_factor;
    int y = (int)surface->h * cible->y_factor;
    int x2 = (int)surface->w * cible->x_factor;
    int y2 = (int)surface->h * (cible->y_factor + cible->w_factor);

    if(SDL_IntersectRectAndLine(&j, &x, &y, &x2, &y2)){
      cible->isKnockBack = true;
      cible->percent += KNOCKBACKFORCE;
      cible->knockBack_direction.x = 1;
    }
  }

  if(attaquant->direction.x == -1){    //il attaque vers la gauche
    //on creer une ligne representant le cote droit de la personne attaqué
    int x = (int)surface->w * (cible->x_factor + cible->w_factor);
    int y = (int)surface->h * cible->y_factor;
    int x2 = (int)surface->w * (cible->x_factor + cible->w_factor);
    int y2 = (int)surface->h * (cible->y_factor + cible->w_factor);

    if(SDL_IntersectRectAndLine(&j, &x, &y, &x2, &y2)){
      cible->isKnockBack = true;
      cible->percent += KNOCKBACKFORCE;
      cible->knockBack_direction.x = -1;
    }
  }
}

void forces(Joueur *joueur){
  if(joueur->isJumping < HEIGHTJUMP && joueur->jump == true){
    joueur->y_factor -= JUMPFORCE;
    joueur->isJumping +=1;
  }
  else{
    joueur->jump = false;
    joueur->isJumping = 0;
  }

  if(joueur->isKnockBack == true && joueur->cpt_knockBack < KNOCKBACK){
    if(joueur->knockBack_direction.x != 0) joueur->x_factor += joueur->knockBack_direction.x * joueur->percent * 0.003;
    if(joueur->knockBack_direction.y != 0) joueur->y_factor += joueur->knockBack_direction.y * joueur->percent * 0.003;
    joueur->cpt_knockBack++;
  }
  else{
    joueur->knockBack_direction.x = 0;
    joueur->knockBack_direction.y = 0;
    joueur->isKnockBack = false;
    joueur->cpt_knockBack = 0;
  }
}


void is_FarAway(Joueur *joueur1, SDL_Surface *window){
  if( (joueur1->y_factor * window->h) > window->h * 1.3 || (joueur1->y_factor * window->h) < window->h * -1.3 ){       //s'il est tombé ou s'il est aller trop haut
    float new_factor = rand() % 100;
    new_factor = new_factor / 100;
    joueur1->vie -= 1;
    joueur1->y_factor = 0.3;
    joueur1->x_factor = new_factor;
  }

  if( (joueur1->x_factor * window->w) > window->w * 1.3 || (joueur1->x_factor * window->w) < window->w * -1.3 ){       //s'il est allé trop loin
    float new_factor = rand() % 100;
    new_factor = new_factor / 100;
    joueur1->vie -= 1;
    joueur1->y_factor = 0.3;
    joueur1->x_factor = new_factor;
  }
}
