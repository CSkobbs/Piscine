#include "graph.h"
#include <fstream>

/*************************************************
        CONSTANTES DYNAMIQUES DE POPULATIONS
***************************************************/
#ifndef REPRO
#define REPRO 5
#endif

// Capacité de portage de l'environnement : facteur K dans le cdc
#ifndef PORTAGE
#define PORTAGE 50
#endif

#ifndef PAS
#define PAS 0.05
#endif

// Efficacité des prédateurs
#ifndef PREDATION
#define PREDATION 0.5
#endif

// Variable globale de temps
extern int temps_dynamique_population;



/***************************************************
                    VERTEX
****************************************************/



/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;
    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value));
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}


/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1100,1100);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(1100,1100);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    /// Le cadre d'ensemble de l'interface; x y coin sup. gauche, largeur, hauteur
    /// Le fond sera gris clair et on pourra le bouger à la souris (drag & drop)
    m_top_box.set_frame(0, 0, 1025,800);
    m_top_box.set_bg_color(GRISCLAIR);

    /// Les liens sont ajoutés en 1er pour que les autres éléments soient affichés au dessus
    m_top_box.add_child( m_boite_boutons );
    m_boite_boutons.set_frame(0,700,1025,100);
    m_boite_boutons.set_bg_color(BLANC);
    //Bouton 1
    m_boite_boutons.add_child(m_bouton1);
    m_bouton1.set_frame(0,0,255,100);
    m_bouton1.set_bg_color(ROSE);
    m_bouton1.add_child(m_bouton1_label);
    m_bouton1_label.set_message("Sauver");
    //Bouton 2
    m_boite_boutons.add_child(m_bouton2);
    m_bouton2.set_frame(255,0,255,100);
    m_bouton2.set_bg_color(ROSE);
    m_bouton2.add_child(m_bouton2_label);
    m_bouton2_label.set_message("Changer");
    //Bouton 3
    m_boite_boutons.add_child(m_bouton3);
    m_bouton3.set_frame(510,0,255,100);
    m_bouton3.set_bg_color(ROSE);
    m_bouton3.add_child(m_bouton3_label);
    m_bouton3_label.set_message("Ajouter");
    //Bouton 4
    m_boite_boutons.add_child(m_bouton4);
    m_bouton4.set_frame(765,0,255,100);
    m_bouton4.set_bg_color(ROSE);
    m_bouton4.add_child(m_bouton4_label);
    m_bouton4_label.set_message("Supprimer");
}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
/*void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(0, 0, 800, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 400, 450, "carriere.jpg");
    add_interfaced_vertex(1, 60.0, 200, 350, "carbonate.jpg");
    add_interfaced_vertex(2,  50.0, 300, 350, "sable.jpg");
    add_interfaced_vertex(3,  0.0, 500, 350, "calcaire.jpg");
    add_interfaced_vertex(4,  100.0, 700, 350, "granulat.jpg");
    add_interfaced_vertex(5,  0.0, 100, 275, "calcin.jpg");
    add_interfaced_vertex(6,  0.0, 300, 200, "verre.jpg");
    add_interfaced_vertex(7,  0.0, 500, 200, "ciment.jpg");
    add_interfaced_vertex(8,  10.0, 700, 200, "goudron.jpg");
    add_interfaced_vertex(9, 30.0, 100, 100, "erosion.jpg");
    add_interfaced_vertex(10, 60.0, 250, 50, "recyclage.jpg");
    add_interfaced_vertex(11,  50.0, 500, 75, "batiment.jpg");
    add_interfaced_vertex(12,  0.0, 700, 75, "route.jpg");




    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 0, 1, 50.0);
    add_interfaced_edge(1, 0, 2, 50.0);
    add_interfaced_edge(2, 0, 3, 50.0);
    add_interfaced_edge(3, 0, 4, 75.0);

    add_interfaced_edge(4, 1, 6, 25.0);

    add_interfaced_edge(5, 2, 6, 25.0);
    add_interfaced_edge(6, 2, 7, 25.0);

    add_interfaced_edge(7, 3, 6, 0.0);
    add_interfaced_edge(8, 3, 7, 100.0);

    add_interfaced_edge(9, 4, 8, 20.0);

    add_interfaced_edge(10, 5, 6, 80.0);

    add_interfaced_edge(11, 6, 10, 50.0);
    add_interfaced_edge(12, 6, 11, 50.0);

    add_interfaced_edge(13, 7, 11, 50.0);

    add_interfaced_edge(14, 8, 12, 75.0);

    add_interfaced_edge(15, 9, 0, 25.0);
    add_interfaced_edge(16, 9, 11, 25.0);

    add_interfaced_edge(17, 10, 4, 25.0);
    add_interfaced_edge(18, 10, 5, 0.0);

    add_interfaced_edge(19, 11, 10, 100.0);
}*/

// Sous programme de construction des graphes
void Graph::make_graphe(const std::string& vertex, const std::string& edge)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    remplissage_vertex(vertex);
    remplissage_edge(edge);

}

// Construction du graphe par lecture de fichier
// Par m�thode chargement de fichier : remplissage_vertex
void Graph::remplissage_vertex(const std::string& nom_fichier)
{
    // variables temp pour le remplissage de vertex
    int ordre, indice, posx, posy;
    float valeur;
    std::string  nom_image;

    std::ifstream fic(nom_fichier.c_str());
    if ( !fic.is_open() )
        throw "Probleme ouverture fichier !";
    /// Construction du vecteur d'aretes
    if ( fic.good())
    {
        fic >> ordre;
        std::cout << "ordre : " << ordre << std::endl;
        for (int i = 0; i < ordre; ++i)
        {
            // Initialisation des variables locales � chaque tour de boucle pour �viter de construire le mauvais sommet
            indice = 0, valeur = 0.0, posx = 0, posy = 0, nom_image = "";
            fic >> indice >> valeur >> posx >> posy >> nom_image ;
            add_interfaced_vertex(indice,valeur,posx,posy,nom_image);
        }
    }
    std::cout << "crea fichier : " << m_vertices.size() << std::endl;

}

// Construction des aretes par lecture de fichier

void Graph::remplissage_edge(const std::string& nom_fichier)
{

    // variables temp pour le remplissage de edges
    int ordre, indice, sommet1, sommet2;
    float poids;

    std::ifstream fic(nom_fichier.c_str());
    if ( !fic.is_open() )
        throw "Probleme ouverture fichier !";
    /// Construction du vecteur d'aretes
    if ( fic.good())
    {
        fic >> ordre;
        for (int i = 0; i < ordre; ++i)
        {
            // Initialisation des variables locales � chaque tour de boucle pour �viter de construire la mauvaise aretes
            indice = 0, sommet1 = 0, sommet2 = 0, poids = 0.0 ;
            fic >> indice >> sommet1 >> sommet2 >> poids ;
            add_interfaced_edge(indice,sommet1,sommet2,poids);
        }
    }

}

// M�thode d'�criture des aretes dans le fichier de destination
// la m�thode capture les valeurs des aretes lors de la fin de la boucle
void Graph::ecriture_edge(const std::string & nom_fichier)
{

    // variables temp pour le remplissage de edges
    // int ordre, indice, sommet1, sommet2, cmp = 0;
    // float poids;
    int cmp = 0;

    std::ofstream fic(nom_fichier.c_str());
    if ( !fic.is_open() )
        throw "Probleme ouverture fichier !";
    /// Construction du vecteur d'aretes
    if ( fic.is_open())
    {
        fic << m_edges.size() << std::endl;
        for (auto it = m_edges.begin(); it!=m_edges.end(); ++it)
        {
            // r�cup�ration des valeurs du sommet n�cessaires � la construction de l'interface
            // et � la sauvegarde des positions lors de la fin du jeu
            // voir si probl�me h�ritage lors de la r�cup�ration des coordonn�es
            fic << cmp << " " << it->second.m_from << " " << it->second.m_to << " " << it->second.m_weight << std::endl;
            cmp++;
        }
    }
    fic.close();
}

// M�thode d'�criture des sommets(vertex) dans le fichier de destination
// la m�thode capture les valeurs des sommets lors de la fin de la boucle
void Graph::ecriture_vertex(const std::string& nom_fichier)
{

    // variables temp pour le remplissage de edges
    // int ordre, indice, sommet1, sommet2, cmp = 0;
    // float poids;
    int cmp = 0;

    std::ofstream fic(nom_fichier.c_str());
    if ( !fic.is_open() )
        throw "Probleme ouverture fichier !";
    /// Construction du vecteur d'aretes
    if ( fic.is_open())
    {
        fic << m_vertices.size() << std::endl;
        std::cout << m_vertices.size() << std::endl;
        for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {
            // r�cup�ration des valeurs du sommet n�cessaires � la construction de l'interface
            // et � la sauvegarde des positions lors de la fin du jeu
            // voir si probl�me h�ritage lors de la r�cup�ration des coordonn�es
            fic << cmp << " " << it->second.m_value <<" "<< it->second.m_interface->m_top_box.get_posx() << " " << it->second.m_interface->m_top_box.get_posx() << " " << it->second.m_interface->m_img.get_pic_name() << std::endl;
            cmp++;
        }
    }
    fic.close();
}



/************************************************
        Deuxième approche DYNAMIQUE : Graph

    - la dynamique se fait au niveau du graph
    - pour un accès direct aux valeurs des prédécesseurs
    - du sommet à traiter
**************************************************/
// Les deux vecteurs sont symétriques et de meme taille
// un ajout d'un prédateur entraine l'ajout de sa population à l'instant t
// Pour que le modèle reste valide il faut que la dynamique soit
// rafraichit tous les tours de boucles et doit être gérer par un pas
// suivant l'évolution de la population

void Graph::Recherchepreda(Vertex proie,std::vector<int> & coeff,std::vector<int> & pop)
{

    // Recherche des coefficient des prédateurs : poids des aretes des prédecesseurs
    for (int i = 0; i < m_edges.size(); ++i)
    {
        for (int j = 0; j < proie.m_in.size(); ++j)
                {
                    if (m_edges[i].m_to == proie.m_in[j])
                        {

                            // Condition pour savoir si prédésseur

                            // Ajout des coefficients de prédation des prédateurs
                            coeff.push_back(m_edges[i].m_weight);
                            // Ajout de la valeur du sommet prédésseurs
                            pop.push_back(m_vertices[i].m_value);

                        }
                }
    }
    // Recherche des populations de prédateurs : valeur des prédécesseurs

}

void Graph::Dynamique_pop(Vertex & Proie)
{
    // Calcul du coeff de prédation
    int Predation = 0;
    std::vector<int> coeff_preda,pop_preda;

    // Recherchepreda(Proie,coeff_preda,pop_preda);


    for (int i = 0; i < coeff_preda.size(); ++i)
    {
        // recherche du coeff de prédation
        // somme des differents coeff d'efficatité des prédateurs * nombre de prédateurs
        // attention récuperer les tailles de populations des autres prédateurs
        Predation += coeff_preda[i]*pop_preda[i]*PREDATION;
    }
    if (Predation > 100)
    {
        Predation = Predation/100;
    }


    // for (int i = 0; i < coeff_preda.size(); ++i)
    // {
    //     std::cout << "coeff : " << coeff_preda[i] << std::endl;

    // }

    // std::cout << "Predation : " << Predation << std::endl;

    // std::cout << "coeff.size : " << coeff_preda.size() << std::endl;
    // std::cout << "pop_preda.size : " << pop_preda.size() << std::endl;

    // std::cout << "Temps : " << temps_dynamique_population << std::endl;

    // d'apres la formule du cours :
    // ajouter le pas avec i dans les arguments de la fonction
    // i compteur de boucle de jeu et non compteur lors du parcours du vecteur de prédécesseurs

    // Avec Prédation
    Proie.m_value = Proie.m_value + temps_dynamique_population*PAS*(REPRO*(1 - Proie.m_value/PORTAGE) - Predation);
    // Sans prédation
    // Proie.m_value = Proie.m_value + temps_dynamique_population*PAS*(REPRO*(1 - Proie.m_value/PORTAGE));
}


/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    int cmp = 0;
    if (!m_interface)
        return;


    // Dynamique des populations
    for (auto &elt : m_vertices)
    {
        cmp ++;
        // std::cout << "Indice sommet " <<  cmp << std::endl;

        // Dynamique fait beuguer la sauvegarde
        Dynamique_pop(elt.second);

        // std::cout << "Population du sommet [] " << i << " : "<< m_vertices[i].m_value << std::endl;
        // std::cout << "Population du sommet " <<  cmp << " "<< elt.second.m_value << std::endl;
        // std::cout << "--------------------------------------" << std::endl;

    }

    for (auto &elt : m_vertices)
        elt.second.pre_update();



    for (auto &elt : m_edges)
        elt.second.pre_update();



    // for (int i = 0; i < m_vertices.size(); ++i)
    // {

    //     Dynamique_pop(m_vertices[i]);

    // }



    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

    if ( m_interface->m_bouton1.clicked() )
    {
        std::cout << "Sauvegarde du graphe" << std::endl;
        ecriture_edge("last_edges.txt");
        ecriture_vertex("last_vertex.txt");
        //appeler la fonction sauvegarder
    }

    if ( m_interface->m_bouton2.clicked() )
    {
        std::cout << "Chargement" << std::endl;
        //appeler la fonction charger

        // g.make_graphe("graphe_1_sommet.txt","graphe_1_arete.txt");
        // g.ecriture_edge("graphe_1_sommet_test.txt");
        // g.ecriture_vertex("graphe_1_arete_test.txt");

        // h.make_graphe("graphe_1_sommet.txt","graphe_1_arete.txt");
        // h.ecriture_edge("graphe_1_sommet_test.txt");
        // h.ecriture_vertex("graphe_1_arete_test.txt");

        // b.make_graphe("graphe_3_sommet.txt","graphe_3_arete.txt");
        // b.ecriture_edge("graphe_3_sommet_test.txt");
        // b.ecriture_vertex("graphe_3_arete_test.txt");

    }

    if ( m_interface->m_bouton3.clicked() )
    {
        std::cout << "Ajout d'un sommet" << std::endl;
        //appeler la fonction ajouter
    }

    if ( m_interface->m_bouton4.clicked() )
    {
        std::cout << "Suppression d'un sommet" << std::endl;
        //appeler la fonction supprimer
    }

}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);

}



/*void Graph::cliquer()
{
    if ( m_bouton1.clicked() )
    {
        std::cout << "OK1" << std::endl;
        //appeler la fonction sauvegarder
    }

    if ( m_bouton2.clicked() )
    {
        std::cout << "OK2" << std::endl;
        //appeler la fonction charger
    }

    if ( m_bouton3.clicked() )
    {
        std::cout << "OK3" << std::endl;
        //appeler la fonction ajouter
    }

    if ( m_bouton4.clicked() )
    {
        std::cout << "OK4" << std::endl;
        //appeler la fonction supprimer
    }
}

/// Une méthode update de la classe doit être appelée dans la boucle de jeu
/// et cette méthode doit propager l'appel à update sur les widgets contenus...
/// Cette méthode fait le lien entre l'interface, les événements, et les conséquences
void Thing::update()
{

    /// Si tous les widgets dépendants de l'objet sont dans une top box
    /// alors ce seul appel suffit (la propagation d'updates se fait ensuite automatiquement)
    m_top_box.update();

    /// Utilisation d'un bouton pour déclencher un événement
    /// L'accès à clicked() fait un reset : tant que le bouton n'est pas
    /// à nouveau cliqué les futurs accès à clicked seront faux
    /// ( Donc il faut appeler clicked() UNE FOIS ET UNE SEULE par update )


}


/// On a des allocations dynamiques dans m_dynaclowns => à nettoyer dans le destructeur
Thing::~Thing()
{}
*/
