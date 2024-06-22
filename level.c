#include "header.h"

Terrain constructeur_terrain(float x, float y, bool sol){
  Terrain terrain;
  terrain.x_factor = x;
  terrain.y_factor = y;
  terrain.w_factor = 0.0625;    // 2/32
  if(sol) terrain.h_factor = 0.056;    // 1/18
  else if(!sol) terrain.h_factor = 0.028;   // 0.5/18
  terrain.sol = sol;

  return terrain;
}

void constructeur_niveau(Niveau *niveau){
  Terrain a = constructeur_terrain(0.1875,0.7, true);
  Terrain b = constructeur_terrain(0.25,0.7, true);
  Terrain c = constructeur_terrain(0.3125,0.7, true);
  Terrain d = constructeur_terrain(0.375,0.7, true);
  Terrain e = constructeur_terrain(0.4375,0.7, true);
  Terrain f = constructeur_terrain(0.5,0.7, true);
  Terrain g = constructeur_terrain(0.5625,0.7, true);
  Terrain h = constructeur_terrain(0.625,0.7, true);
  Terrain i = constructeur_terrain(0.6875,0.7, true);
  Terrain j = constructeur_terrain(0.75,0.7, true);

  Terrain k = constructeur_terrain(0.1875,0.5, false);
  Terrain l = constructeur_terrain(0.75,0.5, false);

  niveau->nb_surfaces = 12;
  niveau->surfaces = malloc(sizeof(Terrain)*niveau->nb_surfaces);

  niveau->surfaces[0] = a;
  niveau->surfaces[1] = b;
  niveau->surfaces[2] = c;
  niveau->surfaces[3] = d;
  niveau->surfaces[4] = e;
  niveau->surfaces[5] = f;
  niveau->surfaces[6] = g;
  niveau->surfaces[7] = h;
  niveau->surfaces[8] = i;
  niveau->surfaces[9] = j;
  niveau->surfaces[10] = k;
  niveau->surfaces[11] = l;

  niveau->src_sol = malloc(sizeof(char)*256);
  sprintf(niveau->src_sol, "assets/terrain.png");
  niveau->src_plateform = malloc(sizeof(char)*256);
  sprintf(niveau->src_plateform, "assets/platerform.jpg");
  niveau->src_fond = malloc(sizeof(char)*256);
  sprintf(niveau->src_fond, "assets/fond.png");
}

void destructeur_niveau(Niveau *niveau){
  free(niveau->surfaces);
  free(niveau->src_sol);
  free(niveau->src_plateform);
  free(niveau->src_fond);
  niveau = NULL;
}
