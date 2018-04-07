#include <vector>
#include "graph.h"
// CODE A IMPLEMENTER DANS LE PROJET AVEC LES CLASSES EDGES ET VERTEX

#ifndef PAS
#define PAS 0.01
#endif

/// Var globale de la reproduction proies

#ifndef PREDATION
#define PREDATION 0.5
#endif

#ifndef MORTALITE

#define MORTALITE 2.5
#endif

#ifndef EFFICATITE
#define EFFICATITE 0.7
#endif

#ifndef N_PROIE
#define N_PROIE 20
#endif

#ifndef P_PREDA
#define P_PREDA 5
#endif

// Capacité de portage de l'environnement : facteur K dans le cdc
#ifndef PORTAGE
#define PORTAGE 100
#endif


/// A INCLURE DANS LA FONCTION UPDATE DONC PAS DE VERTEX PROIE
/// On peut acceder directement aux champs m_from du sommet
#ifndef REPRO
#define REPRO 10
#endif

/************************************************
				Première approche : Vertex

	- la dynamique se fait au niveau des vertex
**************************************************/


// Méthode pour rechercher toute les predateurs du sommet selectionné
void Vertex::Recherchepreda(std::vector<int> & coeff)
{
	for (int i = 0; i < edges.size(); ++i)
	{
		for (int j = 0; j < m_from.size(); ++j)
				{
					if (edges[i].m_to == m_from[j])
						{
							coeff.push_back(edges[i].m_weight);
						}					
				}		
	}
}

void Vertex::Dynamique_pop()
{
	// Calcul du coeff de prédation
	int Predation;
	std::vector<int> coeff_preda;

	Recherchepreda(coeff_preda,vertexes[i]);

	for (int i = 0; i < coeff_preda.size(); ++i)
	{
		// recherche du coeff de prédation 
		// somme des differents coeff d'efficatité des prédateurs * nombre de prédateurs
		// attention récuperer les tailles de populations des autres prédateurs
		Predation += coeff_preda[i]*m_value;
	}

	// d'apres la formule du cours :
	m_value = m_value + REPRO * (1 - m_value/PORTAGE) - Predation;
}

/************************************************
				Deuxième approche : Graph

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
	for (int i = 0; i < edges.size(); ++i)
	{
		for (int j = 0; j < proie.m_from.size(); ++j)
				{
					if (edges[i].m_to == proie.m_from[j])
						{
							

							// Condition pour savoir si prédésseur

							// Ajout des coefficients de prédation des prédateurs
							coeff.push_back(edges[i].m_weight);
							// Ajout de la valeur du sommet prédésseurs
							pop.push_back(vertexes[i].m_value);

						}					
				}		
	}
	// Recherche des populations de prédateurs : valeur des prédécesseurs
	
}

void Graph::Dynamique_pop(Vertex Proie)
{
	// Calcul du coeff de prédation
	int Predation;
	std::vector<int> coeff_preda,pop_preda;

	Recherchepreda(Proie,coeff_preda,pop_preda);


	for (int i = 0; i < coeff_preda.size(); ++i)
	{
		// recherche du coeff de prédation 
		// somme des differents coeff d'efficatité des prédateurs * nombre de prédateurs
		// attention récuperer les tailles de populations des autres prédateurs
		Predation += coeff_preda[i]*pop_preda[i];
	}

	// d'apres la formule du cours :
	Proie.m_value = Proie.m_value + REPRO * (1 - Proie.m_value/PORTAGE) - Predation;
}




