#include "menu.h"


void menu(Graph G)
{
    BITMAP * buffer;

    int page_actuelle =1;
    int etat = 0;

    /* Declaration des variables */
    BITMAP * menu_page1[3];
    BITMAP * menu_page2[4];
    BITMAP * fond;

    fond = load_bitmap("Menu Decors/fond1.bmp", NULL);
    buffer = create_bitmap(SCREEN_W, SCREEN_H);

    /* chargement image page_1 */

    menu_page1[0]=load_bitmap("Menu Decors/page1-neutre.bmp",NULL);
    menu_page1[1]=load_bitmap("Menu Decors/page1-2.bmp", NULL);
    menu_page1[2]=load_bitmap("Menu Decors/page1-1.bmp", NULL);

    /* chargement image page_2 */

    menu_page2[0]=load_bitmap("Menu Decors/page2-neutre.bmp",NULL);
    menu_page2[1]=load_bitmap("Menu Decors/page2-1.bmp",NULL);
    menu_page2[2]=load_bitmap("Menu Decors/page2-2.bmp",NULL);
    menu_page2[3]=load_bitmap("Menu Decors/page2-3.bmp",NULL);


    while(!key[KEY_P])
    {
        if(page_actuelle == 1)
        {
            blit(fond, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
            draw_sprite(buffer, menu_page1[etat],0,0);

            if((key[KEY_DOWN]))
            {
                if(etat < 2)
                    etat++;
                else
                    etat = 1;
            }
            else if(key[KEY_UP])
            {
                if((etat < 2) || (etat == 0))
                    etat = 2;
                else
                    etat--;
            }

            if ((key[KEY_ENTER]) && (etat==1))
            {
                page_actuelle = 2;
                etat = 0;
            }

            else if((key[KEY_ENTER]) && (etat==2))
            {
                exit(1);
            }

            draw_sprite(screen, buffer,0,0);
        }

        else if(page_actuelle == 2)
        {
            blit(fond, buffer,0, 0, 0, 0, SCREEN_W, SCREEN_H);
            draw_sprite(buffer, menu_page2[etat],0,0);
            if((key[KEY_DOWN]))
            {
                if(etat < 3)
                    etat++;
                else
                    etat = 1;
            }
            else if(key[KEY_UP])
            {
                if((etat < 2) || (etat == 0))
                    etat = 3;
                else
                    etat--;
            }






            if ((key[KEY_ENTER])&& (etat==1))
            {

                while(!key[KEY_F])
                {

                    G.update();
                    grman::mettre_a_jour();

               }

            }

            if ((key[KEY_ENTER])&& (etat==2))
            {


                while(!key[KEY_F])
                {

                    G.update();
                    grman::mettre_a_jour();

               }
            }

            if ((key[KEY_ENTER])&& (etat==3))
            {


                while(!key[KEY_F])
                {

                    G.update();
                    grman::mettre_a_jour();

               }
            }


            draw_sprite(screen, buffer,0,0);

            if (key[KEY_BACKSPACE])
            {
                page_actuelle=1;
                etat = 0;
            }
        }
        rest(150);
    }
}

