#include <stdio.h>
#include <stdlib.h>

typedef struct police
{
    TTF_Font *police;               //  POLICE POUR LES TEXTES  //
    SDL_Color couleur;              //  COULEUR POUR LA POLICE  //
}police;
typedef struct character
{
    char *nom;
    SDL_Texture *texture;           //  PRINCIPALE          //
    SDL_Texture *texture_immo;      //  IMMOBILE A DROITE   //
    SDL_Texture *texture_D;         //  MARCHE A DROITE     //
    SDL_Texture *texture_D2;        //  MARCHE A GAUCHE     //
    SDL_Texture *texture_G;         //  MARCHE A GAUCHE     //
    SDL_Texture *texture_G2;        //  MARCHE A GAUCHE     //
    SDL_Surface *img;               //  IMG PRINCIPALE      //
    SDL_Surface *img_immo;          //  IMMOBILE A DROITE   //
    SDL_Surface *img_D;             //  MARCHE A DROITE     //
    SDL_Surface *img_D2;            //  MARCHE A DROITE     //
    SDL_Surface *img_G;             //  MARCHE A GAUCHE     //
    SDL_Surface *img_G2;            //  MARCHE A DROITE     //
    int immobile;                   //  SI IMMOBILE         //
    int marcheD;                    //  SI MARCHE A DROITE  //
    int marcheG;                    //  SI MARCHE A GAUCHE  //
    SDL_Rect location;              //  POSTION SUR ECRAN   //
    SDL_Rect location_fictive;      //  POSTITION SUR IMAGE //

}character;
typedef struct monde
{
    SDL_Texture *texture;           //  TEXTURE PRINCIPALE  //
    SDL_Surface *img;               //  IMG PRINCIPALE      //
    SDL_Rect location;              //  POSTION SUR ECRAN   //
}monde;
typedef struct bloc
{
    SDL_Rect location;              //  POSTION SUR ECRAN   //
    int type;                       // 0 = Bloc normal 1 = '?'  //
    int touche;                     //  0 ou 1   //
}bloc;
typedef struct t_texte
{
    char *texte;
    SDL_Texture *texture;           //  TEXTURE PRINCIPALE  //
    SDL_Surface *surface;           //  IMG PRINCIPALE      //
    SDL_Rect location;              //  POSTION SUR ECRAN   //
    TTF_Font *police;               //  POLICE ECRITURE     //
}t_texte;
typedef struct screen
{
    SDL_Texture *texture;           //  TEXTURE PRINCIPALE  //
    SDL_Surface *img;               //  IMG PRINCIPALE      //
    SDL_Rect location;              //  POSTION SUR ECRAN   //
}screen;

void Init_SDL();
void Init_character(character *c,SDL_Renderer *r);
void Init_monde1(monde *m,SDL_Renderer *r);
void Init_police(police *t,SDL_Renderer *r);
void Init_texte(t_texte *t,SDL_Renderer *r,int x,int y ,int h, int w,char *chaine,police *p);
void Init_screen(screen *g,screen *p,SDL_Renderer *r);
int Random_nb();
void Init_bloc(bloc *t_bloc,int nb_bloc);
int bloc_au_dessus(character *c, bloc *tbloc, int nb_bloc,int score,SDL_Renderer *r);
void texture_immobile(character *c);
int test_reset(character *c,SDL_Rect *r);
void deplacement(char direction,character *c,SDL_Rect *ecran,int x);
void saut(character *c,SDL_Renderer *r);
void reset_after_jump(character *c);
int key_up_down(SDL_Event e,character *c,SDL_Rect rect,SDL_Renderer *r);
int bloc_en_dessous(character *c);
int touche_enemie(character *c);
void render_SDL(SDL_Renderer *r,SDL_Rect e,character *c,monde *m,t_texte *s);
void free_SDL(character *c,police *t,monde *m,t_texte *s,SDL_Renderer *r,SDL_Window *f,screen *p,screen *g);
