#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "./fonction.h"

#define taille_ecran 450
#define taille_image 3392
#define y_max 450
#define x_max 900

//  INITIALISATION DES BIBLIOTHEQUES    //
void Init_SDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING)<0)
        printf("erreur sdl_Init");
    if(IMG_Init(IMG_INIT_PNG)<0)
        printf("erreur sdl_Init_IMG");
    if(TTF_Init()<0)
        printf("erreur sdl_Init_TTF");
}
//  INITIALISATION DE MARIO    //
void Init_character(character *c,SDL_Renderer *r)
{

    c->nom = "Mario";

    c->img = IMG_Load("./img/mario_immo.png");
    c->texture = SDL_CreateTextureFromSurface(r,c->img);

    c->img_immo = IMG_Load("./img/mario_immo.png");
    c->texture_immo = SDL_CreateTextureFromSurface(r,c->img_immo);

    c->img_D = IMG_Load("./img/mario_marche1.png");
    c->texture_D = SDL_CreateTextureFromSurface(r,c->img_D);

    c->img_D2 = IMG_Load("./img/mario_marche2.png");
    c->texture_D2= SDL_CreateTextureFromSurface(r,c->img_D2);

    c->img_G = IMG_Load("./img/mario_marche1G.png");
    c->texture_G = SDL_CreateTextureFromSurface(r,c->img_G);

    c->img_G2 = IMG_Load("./img/mario_marche2G.png");
    c->texture_G2 = SDL_CreateTextureFromSurface(r,c->img_G2);

    c->location.x = 0; //position mario
    c->location.y = y_max - 48 - 64; // taille ecran - taille sol - taille mario
    c->location.w = 32;
    c->location.h = 64;
}
//  INITIALISATION DU MONDE 1    //
void Init_monde1(monde *m,SDL_Renderer *r)
{
    m->img = IMG_Load("./img/monde1.png");
    m->texture = SDL_CreateTextureFromSurface(r,m->img);

    m->location.x = 0;
    m->location.y = 0;
    m->location.w = x_max;
    m->location.h = y_max;
}
//  INITIALISATION DE LA POLICE    //
void Init_police(police *t,SDL_Renderer *r)
{
    SDL_Color blanc = {255,255,255};
    t->couleur = blanc;
    t->police = TTF_OpenFont("./font/Super-Mario.ttf",15);
}
//  INITIAISATION DU TEXTE  //
void Init_texte(t_texte *t,SDL_Renderer *r,int x,int y ,int h, int w,char *chaine,police *p)
{
    t->surface = TTF_RenderText_Solid(p->police,chaine,p->couleur);
    t->texture =  SDL_CreateTextureFromSurface(r,t->surface);

    t->location.x = x;
    t->location.y = y;
    t->location.w = w;
    t->location.h = h;
}
void Init_screen(screen *g,screen *p,SDL_Renderer *r)
{
    p->img = IMG_Load("./img/perdu.png");
    p->texture = SDL_CreateTextureFromSurface(r,p->img);
    g->img = IMG_Load("./img/gagne.png");
    g->texture= SDL_CreateTextureFromSurface(r,g->img);
}
//  RETOURNE UN NOMBRE ENTRE 1 ET 6 POUR LES '?'  //
int Random_nb()
{
    srand(time(NULL));
    return rand() % (6 + 1);

}
//  ON TEST SI MARIO EST IMMOBILE A GAUCHE OU A DROITE  //
void Init_bloc(bloc *t_bloc,int nb_bloc)
{
    int i = 0;
    int x = 16;
    int cst = 240;
    for(i=0;i<nb_bloc;i++)
    {
        //  ON PLACE LES 21 BLOCS SUIVANT LEURS LOCALISATIONS SUR L'IMAGE   //
        if( i == 1 ) x = 4 * x;
        if(i == 6) x = 53 * x;
        if(i == 9) x = 15 * x;
        if(i == 10 || i == 15) x = 6 * x;
        if(i == 12) x = 5 * x;
        if(i == 13 || i == 14) x = 3 * x;
        if(i == 16) x = 11 * x;
        if(i == 18) x = 38 * x;

        t_bloc[i].location.x = cst + x;
        cst = t_bloc[i].location.x;
        x = 16;
        t_bloc[i].location.y = 136;
        t_bloc[i].location.h = 16;
        t_bloc[i].location.w = 16;
        t_bloc[i].type = 0;
        t_bloc[i].touche = 0;
    }
    //  ON DEFINI LE TYPE DES BLOCS '?'  //
    t_bloc[0].type = 1;t_bloc[2].type = 1;t_bloc[4].type = 1; t_bloc[7].type = 1;t_bloc[12].type = 1;t_bloc[13].type = 1;
    t_bloc[14].type = 1;t_bloc[20].type = 1;

}
//  TEST SI BLOC AU DESSUS RETOURNE LE SCORE    //
int bloc_au_dessus(character *c, bloc *tbloc, int nb_bloc,int score,SDL_Renderer *r)
{
    int i = 0;
    int x;
    for(i=0;i<nb_bloc;i++)
    {
        //  SI MARIO A UN BOC AU DESSUS //
        if(c->location_fictive.x + 8 < tbloc[i].location.x + 16 && c->location_fictive.x + 8 > tbloc[i].location.x && c->location.y == 306)
        {
            //  ET QUE CELUI CI N'A PAS ETE TOUCHE ET IL EST DU TYPE BLOC   //
            if(tbloc[i].type == 0 && tbloc[i].touche == 0)
            {
                score += 100;
                tbloc[i].touche = 1;    // IL EST MAINTENANT TOUCHE //
            }
            //  S'IL N'A PAS ETE TOUCHE ET IL EST DU TYPE '?'   //
            if(tbloc[i].type == 1 && tbloc[i].touche == 0)
            {
                //  ON MET DES CHANCES D'AVOIR 500,200 OU 100 PIECES DANS LES '?'   //
                x = Random_nb();
                if(x == 1) score += 500;
                if(x == 2) score += 200;
                if(x == 3) score += 500;
                if(x == 4) score += 200;
                if(x == 5) score += 100;
                if(x == 6) score += 100;
                tbloc[i].touche = 1;    //  IL EST MAINTENANT TOUCHE    //
            }
        }
    }
    return score;
}
// TEXTURE SI MARIO IMMOBILE    //
void texture_immobile(character *c)
{
    c->texture = c->texture_immo;
}
//  TEST SI TOUCHE UN ENEMIE, RETOURNE 1 SI TOUCHE ,0 SINON    //
int touche_enemie(character *c)
{
    if(((c->location_fictive.x <= 970 + 30 && c->location_fictive.x > 970 )||
       (c->location_fictive.x <= 1561  &&  c->location_fictive.x > 1531 )||
       (c->location_fictive.x <= 1977  && c->location_fictive.x > 1947 )||
       (c->location_fictive.x <= 2335  && c->location_fictive.x > 2305 )||
       (c->location_fictive.x <= 2944  && c->location_fictive.x > 2914 ))&& c->location.y == y_max - 48 -64)
        return 1;
    return 0;
}//  TEST SI ON DOIT REINITIALISER   //
//  TEST SI ON DOIT REPOSITIONNER MARIO , RETOUNE 1 SI ON DOIT RESET, 0 SINON    //
int test_reset(character *c,SDL_Rect *r)
{
    // SI MARIO EST TOMBE OU S'IL TOUCHE UN ENEMIE  //
    if(c->location.y >= 450 || touche_enemie(c) )
    {
        // ON REPOSITIONNE MARIO AU DEBUT EN GARDANT LE SCORE   //
        c->location.x = 0;
        c->location.y = 338;
        r->x = 0;
        return 1;
    }
   return 0;
 }
//  AVANCEMENT MARIO    //
void deplacement(char direction,character *c,SDL_Rect *ecran,int x)
{
    #define max_avant 300
    #define max_arriere 200
    //  GAUCHE  //
    if(c->location.y <= 338)
    {if(direction == 'G' )
    {
        c->marcheD = 0; c->marcheG = 1;

        SDL_Delay(1);

        // ANIMATION LORSQU'IL SE DEPLACE //
        if(x) c->texture = c->texture_G;
        else c->texture = c->texture_G2;

        c->location.x-=2;

        if(c->location.x < 0)   // SI TOUT A GAUCHE //
            c->location.x = 0;
        if(ecran->x > 0 && c->location.x < max_arriere) // FAIRE RECULER LE MONDE //
            {
                ecran->x-=2;
                c->location.x = max_arriere;
            }


    }
    //  DROITE  //
    if(direction == 'D')
    {
        c->marcheD = 1; c->marcheG = 0;

        SDL_Delay(1);

        // ANIMATION LORSQU'IL SE DEPLACE //
        if(x) c->texture = c->texture_D;
        else c->texture = c->texture_D2;

        if(c->location.x <= max_avant) //SI MARIO ESY DANS LE PREMIER TIERS DE L'ECRAN //
        {
            c->location.x+=2;
        }
        else
        {
            // SI ON ARRIVE SUR LA DERNIERE PARTIE DU MONDE A AFFICHER MARIO PEUT MARCHER JUSQU'AU DRAPEAU  //
            if(c->location_fictive.x >= 3093 && c->location_fictive.x <= 3168 )
            {
                c->location.x+=2;
            }
            if( ecran->x <= taille_image - taille_ecran ) //  SI PLUS PETIT QUE LE COIN LE PLUS A DROITE DE L'ECRAN //
            {
                    ecran->x+=2;
            }

        }
    }
    }
}
//  SAUT MARIO  //
void saut(character *c,SDL_Renderer *r)
{
    //  SAUT REGARD A DROITE    //
    if(c->marcheD)
    {
        c->img = IMG_Load("./img/mario_saut.png");
    }
    //  SAUT REGARDE A GAUCHE   //
    if(c->marcheG)
    {
        c->img = IMG_Load("./img/mario_sautG.png");
    }
    c->location.y -= 32;
    c->texture = SDL_CreateTextureFromSurface(r,c->img);

}
// REPOSITIONEMENT MARIO AU SOL //
void reset_after_jump(character *c)
{
    if(c->location.y< 338) //  SI MARIO PLUS HAUT QUE LE SOL (taille fenetre - sol - mario) //
            c->location.y = y_max - 48 - 64;

}
// TEST SI BLOC EN DESSOUS, RETOURNE 1 SI IL Y EN A UN, 0 SINON   //
int bloc_en_dessous(character *c)
{
     if(((c->location_fictive.x >= 1104) && (c->location_fictive.x <= 1136 - 16))        //  1 ER TROU    //
        ||((c->location_fictive.x >= 1377) && (c->location_fictive.x <= 1424 - 16))     //  2 EME TROU  //
        ||((c->location_fictive.x >= 2448) && (c->location_fictive.x <= 2480 - 16)))
        {
            return 0;
        }
        else
        {
            return 1;
        }
}
//  RENDU DES TEXTURE //
void render_SDL(SDL_Renderer *r,SDL_Rect e,character *c,monde *m,t_texte *s)
{
    SDL_RenderClear(r);
    SDL_RenderCopy(r,m->texture,&e,&m->location);
    SDL_RenderCopy(r,c->texture,NULL,&c->location);
    SDL_RenderCopy(r, s->texture, NULL, &s->location);
    SDL_RenderPresent(r);
}
//  ON LIBERE LA MEMOIRE DE CHAQUE TEXTURE ET SURFACE    //
void free_SDL(character *c,police *t,monde *m,t_texte *s,SDL_Renderer *r,SDL_Window *f,screen *p,screen *g)
{
    SDL_DestroyTexture(c->texture_D);
    SDL_FreeSurface(c->img_D);

    SDL_DestroyTexture(c->texture_G);
    SDL_FreeSurface(c->img_G);

    SDL_DestroyTexture(c->texture_immo);
    SDL_FreeSurface(c->img_immo);

    SDL_FreeSurface(m->img);
    SDL_DestroyTexture(m->texture);

    SDL_FreeSurface(s->surface);
    SDL_DestroyTexture(s->texture);

    SDL_FreeSurface(p->img);
    SDL_DestroyTexture(p->texture);

    SDL_FreeSurface(g->img);
    SDL_DestroyTexture(g->texture);

    TTF_CloseFont(t->police);

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(f);

    TTF_Quit();
    SDL_Quit();
    IMG_Quit();
}

