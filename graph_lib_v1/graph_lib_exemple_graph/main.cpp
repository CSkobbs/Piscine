#include "grman/grman.h"
#include <iostream>
#include "menu.h"
#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
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
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        menu(g,h,b);
        //g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }


    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


