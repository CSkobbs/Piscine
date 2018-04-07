#include "grman/grman.h"
#include <iostream>
#include "menu.h"
#include "graph.h"


/// VARIABLE GLOBALE
// variable temporelle vitale pour la dynamique des populations
// la dynamique est r�gie par des �quations diff�rentielles
// elles sont donc d�pendantes du temps on a donc cmp = temps
// cette variable est globale car tous les sous-programmes de 
// calcul de populations ont besoin du temps pour g�rer la population au temps t+1
// extern int temps_dynamique_population;

int main()
{
    // temps_dynamique_population = 0;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g,h,b;

    //g.make_example();
    // g.remplissage_vertex("sommet.txt");

    g.make_graphe("graphe_1_sommet.txt","graphe_1_arete.txt");
    g.ecriture_edge("graphe_1_sommet_test.txt");
    g.ecriture_vertex("graphe_1_arete_test.txt");

    h.make_graphe("graphe_1_sommet.txt","graphe_1_arete.txt");
    h.ecriture_edge("graphe_1_sommet_test.txt");
    h.ecriture_vertex("graphe_1_arete_test.txt");

    b.make_graphe("graphe_3_sommet.txt","graphe_3_arete.txt");
    b.ecriture_edge("graphe_3_sommet_test.txt");
    b.ecriture_vertex("graphe_3_arete_test.txt");


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        menu(g,h,b);
        //g.update();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }


    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


