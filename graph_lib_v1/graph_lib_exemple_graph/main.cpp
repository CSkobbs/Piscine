#include "grman/grman.h"
#include <iostream>
#include "menu.h"
#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;

    //g.make_example();
    // g.remplissage_vertex("sommet.txt");

    g.make_graphe("sommet.txt","edge.txt");

    g.ecriture_edge("edgetest.txt");



    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        menu(g);
        //g.update();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }


    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


