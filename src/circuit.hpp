#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP

#include "common.hpp"
#include "instance.hpp"

// Un circuit représente le résultat d'une tournée associée à une remorque donnée.
// Les principaux attributs sont :
//   - inst : l'instance propriétaire
// Attributs primaires (indispensables pour construire une solution)
//   - remorque : la remorque correspondant à ce circuit,
//   - charge_init : charge initiale de la remorque
//   - tournees_data : tableau de paires [station, depot]
//     (n'est PAS mémorisé en attribut)
// Attributs dérivés (calculés à partir des attributs primaires)
//   - stations : la liste des stations gérées par cette remorque
//   - depots[Station] : la Hash des dépots des stations visitées
//   - arcs : liste des arcs représentant le chemin (plus pratique à exploiter
//     que la liste des stations visitées)
//
// Pour l'instant, un Circuit n'est pas intelligent : tout est piloté par la
// classe Solution (qui doit donc avoir un accès complet à tous les attributs)
//
// idée : créer une classe Service pour représenter un couple
//  <Station*, int depos>
class Circuit  {
public:

    // L'instance du problème
    Instance* inst;

    // La remorque associée à ce circuit
    Remorque* remorque;

    // charge initiale de la remorque
    int charge_init=0;

    // charge en cours
    int current_load=0;

    // les stations visitées
    list<Station*>* stations;

    // Une map (Hash en Ruby ou Dict en python) contenant le dépos associé
    // à chaque station servie
    // depots = new map<Station*,int>()
    map<Station*,int>* depots;

    // charge de la remorque **après** passage à une station
    // (peut-être utilisé pour vérifier la remorque ou optimiser une insertion)
    map<Station*,int>* charges;

    // Désequilibre total (toujours positif ou nul !)
    int desequilibre=0;

    // longueur de ce circuit
    int length;

    // la liste des arcs parcourus par la remorque
    // vector<Arc*> arcs;

    // Construction d'un circuit sans stations
    Circuit(Instance* inst, Remorque* remorque);

    // Construction d'un circuit à partir d'une liste de stations
    Circuit(Instance* inst, Remorque* remorque, list<Station*>* stations);

    // Attention : ceci n'est pas le constructeur par copie car il prend un
    // **pointeur** en parametre
    Circuit(const Circuit* circ);

    virtual ~Circuit();

    void copy(const Circuit* other);

    // vide la liste des stations
    void clear();

    // Ne vide que les attributs dérivés à partir de la liste des stations
    // desservies (en vue de regénérer des attributs dérivés valides)
    void partial_clear();

    // Calul le dépos optimum en chaque staion, la charge initiale de la remorque
    // et les autres attributs dérivés
    // Pourrait s'appeler equilibate equilibrate, mais update est plus générique
    void update();

    // retourne le coût mélangeant déséquilibre et distance totale (mesure
    // permettant la comparaison simple de deux solutions)
    inline int get_cost() {
        // return 1000*this->desequilibre + this->length;
        return 1000000*this->desequilibre + this->length;
    }
    // retourne une chaine de la forme  "1-1234"  séparant la valeur du déséquilibre
    // et la distance totale (pour affichage des messages ou dans le nom des
    // fichiers de solution)
    inline string get_cost_string() {
        return U::to_s(this->desequilibre) + "-" + U::to_s(this->length);
    }

    // Return the initial load that minimizes the score on the circuit
    int optimal_initial_load();

    // procède à la mise à jour des attributs charge_init, charges et depots
    // de façon à equilibrer au mieux les stations gérées par la remorque de ce
    // circuit.
    void equilibrate_dummy(); // version bidon pour le proto élèves
    void equilibrate_circuit(); //Premiere brique
    void equilibrate();       // version à créer

    void insert(Station* s, int pos=0);
    void insert_rand(Station* s);

    // Insère la station dans le circuit en fonction de l'option
    // Options::args->station_inserter
    void insert_from_option(Station* s);

    // Supprime et retourne la station en position pos
    Station* erase(int pos);

    // void move(int pos1, int pos2);
    // void move_to(int pos1, Circuit* other, int pos2);

    // Return the circuit where stations i and j are mutated
    Circuit* mutate_2opt(int i, int j);

    // Return the best circuit using mutations
    Circuit* mutate_2opt_best();

    // Based on the actual circuit, do a steepest descent to find a local minimum nearby
    Circuit* mutate_2opt_steepest(int nmax);


    void reverse_branch(int pos1, int pos2);

    // Suppression d'une branche de ce circuit de pos1 (inclu) à pos2 (exclue) et
    // insertion dans le circuit other devant la position pos3.
    // Si reverse vaut true, alors la branche est inversée avant insertion
    // Domaines des paramètres :
    // - pos1 : [0,size1[ (size1 exclu)
    // - pos2 : [1,size1] (size1 inclu)
    // - pos3 : [0,size2] (size2 inclu)
    // - pos1 <= pos2
    // Si |pos2-pos1| == 1 alors on déplace l'élément [pos1]
    // Si pos1 == pos2     alors on ne fait rien
    // Si pos1 >= pos2+1   alors l'insertion se fait en sens inverse
    //
    // - EFFETS DE BORD : exit() si bornes incorrectes ou si this==other
    // - LA SOLUTION N'EST PAS MISE À JOUR ! (pas de update)
    //
    // void move_branch_to(int pos1, int pos2, Circuit* other, int pos3, bool reverse=false);

    string to_s();
    string to_s_long();

};
#endif
//./
