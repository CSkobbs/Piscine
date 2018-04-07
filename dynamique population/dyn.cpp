#include <vector>
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




