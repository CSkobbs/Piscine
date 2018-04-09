#include "grman/grman.h"
#include <iostream>
#include "menu.h"
#include "graph.h"


/// VARIABLE GLOBALE
// variable temporelle vitale pour la dynamique des populations
// la dynamique est régie par des équations différentielles
// elles sont donc dépendantes du temps on a donc cmp = temps
// cette variable est globale car tous les sous-programmes de 
// calcul de populations ont besoin du temps pour gérer la population au temps t+1
// extern int temps_dynamique_population;

int main()
{
    // temps_dynamique_population = 0;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g,h,b;

    // Construction des 3 graphes
    g.make_graphe("graphe_1_sommet.txt","graphe_1_arete.txt");

    h.make_graphe("graphe_2_sommet.txt","graphe_2_arete.txt");

    b.make_graphe("graphe_3_sommet.txt","graphe_3_arete.txt");

    // Affichage de la connexité des graphes
    std::cout << "---------------------------------"<< std::endl;
    std::cout << " ETUDE CONNEXITE GRAPHE g --> "<< std::endl;
    g.tarjan();

    std::cout << "---------------------------------"<< std::endl;
    std::cout << " ETUDE CONNEXITE GRAPHE h --> "<< std::endl;
    h.tarjan();

    std::cout << "---------------------------------"<< std::endl;
    std::cout << " ETUDE CONNEXITE GRAPHE b --> "<< std::endl;
    b.tarjan();

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


