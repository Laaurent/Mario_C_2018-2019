#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "./fonction.h"

    #define x_max  900
    #define y_max 450
    #define nb_bloc 21
    #define nb_enemie 3

 //    SAUTER POUR COMMENCER LORS DE LA DEUXIEME ET TROISIEME VIE  //

int main(int argc,char* argv[])
{
    //  DECLARATION DES VARIABLES   //
    int repeat = 0,space = 0,left = 0,right = 0,perdu = 0,gagne = 0, x = 0;
    SDL_Event event;

    //  INITIALISATION DES BIBLIOTHEQUES    //
    Init_SDL();

    //  INITIALISATION DE LA FENETRE    //
    SDL_Window *fenetre = SDL_CreateWindow("Super_Mario",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,x_max,y_max,SDL_WINDOW_SHOWN);
    SDL_Renderer *render = SDL_CreateRenderer(fenetre,-1,SDL_RENDERER_ACCELERATED);

    //  INITIALISATION DES ECRANS GAGNE ET PERDU  //
    screen *perdu_screen,*gagne_screen;
    perdu_screen = malloc(sizeof(screen));
    gagne_screen = malloc(sizeof(screen));
    Init_screen(gagne_screen,perdu_screen,render);

    // INITIALISATION DU MONDE 1    //
    monde *monde1;
    monde1 = malloc(sizeof(monde));
    Init_monde1(monde1,render);

    //  INITIALISATION DU MARIO    //
    character* mario;
    mario = malloc(sizeof(character));
    Init_character(mario,render);

    //  STRUCTURE OU ON STOCKE LA POLICE ET LA COULEUR  //
    police* p;
    p = malloc(sizeof(police));
    Init_police(p,render);

    //  INITIALISATION DES VIES    //
    int nb_vie = 3;

    //  INITIALISATION DU SCORE    //
    char chaine[5] = "";
    int int_score = 1000;
    sprintf(chaine,"%d",int_score);

    t_texte* score;
    score = malloc(sizeof(t_texte));
    Init_texte(score,render,800,20,20,70,chaine,p);

    //  INITIALISATION DES BLOCS   //
    bloc tbloc[nb_bloc];
    Init_bloc(tbloc,nb_bloc);

    //  INITIALISATION DE LA ZONE DE FOND A AFFCIHER //
    SDL_Rect ecran;
    ecran.x = 0;
    ecran.y = 0;
    ecran.w = 450;
    ecran.h = 224;

    //  BOUCLE DE JEUX  //
    while(!repeat)
    {
        // ALTERNANCE D'UN NOMBRE EN 0 et 1 POUR GERER L'ANIMATION DE MARIO //
        if(x == 1) x = 0;
        else x = 1;

        //  POSITION DE MARIO SUR LE REFERENCIEL IMAGE  //
        mario->location_fictive.x = ecran.x + mario->location.x/2;

        //  TEST PERDU //
        if(test_reset(mario,&ecran))
        {
            // SI TOMBE ON ENLEVE UNE VIE //
            nb_vie--;
        }
        //  SI VIE = 0 ALORS ON A PERDU //
        if(nb_vie == 0)
        {
            perdu = 1;
        }

        //  MARIO PRENDS UNE TEXTURE DIFFERENTE SI CELUI CI EST IMMOBILE    //
        if(mario->immobile) texture_immobile(mario);

        //  AFFICHAGE DU MONDE SI LE JOUEUR N'A NI PERDU NI GAGNE //
        if(perdu == 0 && gagne == 0)
        {
            render_SDL(render,ecran,mario,monde1,score);
        }
        //  SINON AFFICHAGE ECRAN GAGNE OU PERDU    //
        else
        {
            if(perdu == 1)
            {
                SDL_RenderClear(render);
                SDL_RenderCopy(render,perdu_screen->texture, NULL, NULL);

                //  ON AFFICHE LE SCORE SUR L'ECRAN DE FIN  //
                sprintf(chaine," Votre score etait de : %d",int_score);
                Init_texte(score,render,20,400,20,300,chaine,p);
                SDL_RenderCopy(render, score->texture, NULL, &score->location);

                SDL_RenderPresent(render);
            }
            if(gagne == 1)
            {
                SDL_RenderClear(render);
                SDL_RenderCopy(render, gagne_screen->texture, NULL, NULL);

                //  ON AFFCIHELE SCORE SUR L'ECRAN DE FIN   //
                sprintf(chaine," Votre score etait de : %d",int_score);
                Init_texte(score,render,20,400,20,350,chaine,p);
                SDL_RenderCopy(render, score->texture, NULL, &score->location);

                SDL_RenderPresent(render);
            }
        }

        //  ON REGARDE SI MARIO A UN BLOC EN DESSOUS DE LUI ET SI IL SAUTE //
        if(bloc_en_dessous(mario)== 0 && space == 0 )
        {
            // MARIO TOMBE //
            mario->location.y+=2;
        }
        else
        {
            //  MARIO RETOMBE //
            reset_after_jump(mario);
        }

        // DECLARATION DE L'EVENEMENT //
        SDL_PollEvent(&event);

        switch(event.type)
        {
            //  SI L'UTILISATEUR FERME LA FENETRE   //
            case SDL_QUIT:
                {
                    repeat = 1;
                    break;
                }
            //  TOUCHE ENFONCEE //
            case SDL_KEYDOWN:
                {
                    //  GAUCHE  //
                    if(event.key.keysym.sym == SDLK_LEFT)
                    {
                        left = 1;
                        mario->immobile = 0;
                    }
                    //  DROITE  //
                    if(event.key.keysym.sym == SDLK_RIGHT)
                    {
                        right = 1;
                        mario->immobile = 0;
                    }
                    //  ESPACE  //
                    if(event.key.keysym.sym == SDLK_SPACE)
                    {
                        space = 1;
                    }
                break;
                }
            // TOUCHE RELACHEE  //
            case SDL_KEYUP:
                {
                    //  GAUCHE  //
                    if(event.key.keysym.sym == SDLK_LEFT)
                    {
                        left = 0;
                        mario->immobile = 1;
                    }
                    //  DROITE  //
                    if(event.key.keysym.sym == SDLK_RIGHT)
                    {
                        right = 0;
                        mario->immobile = 1;

                    }
                    //  ESPACE  //
                    if(event.key.keysym.sym == SDLK_SPACE)
                    {
                        space = 0;
                    }
               break;
               }
        }
        //  SI GAUCHE ENFONCEE  //
        if(right)
        {
            deplacement('D',mario,&ecran,x);
            //  SI MARIO EST SUR LE DRAPEAU  //
            if(mario->location_fictive.x == 3168)
                gagne = 1;

        }
        //  SI DROITE ENFONCEE  //
        if(left)
        {
            deplacement('G',mario,&ecran,x);

        }
        //  SI ESPACE ENFONCEE  //
        if(space)
        {
            saut(mario,render);
        }

        //  GESTION DU SCORE    //
        int_score = bloc_au_dessus(mario,tbloc,nb_bloc,int_score,render);
        sprintf(chaine,"%d",int_score);
        Init_texte(score,render,800,20,20,70,chaine,p);

        //  RENDU    //
        SDL_RenderPresent(render);

        //  INITIALISATION DU DELAY //
        SDL_Delay(5);
    }


    //  ON LIBERE LA MEMOIRE DE CHAQUE TEXTURE ET SURFACE   //
    free_SDL(mario,p,monde1,score,render,fenetre,perdu_screen,gagne_screen);

    return 0;
}
