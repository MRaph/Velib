#include "circuit.hpp"

Circuit::Circuit(Instance* inst, Remorque* remorque) {
    // logn1("Circuit::Circuit: START " + remorque->name);
    this->inst = inst;
    this->remorque = remorque;
    this->charge_init = 0;
    this->desequilibre = 0;
    this->length = 0;

    this->stations = new list<Station*>();
    this->depots = new map<Station*,int>();
    this->charges = new map<Station*,int>();
    // this->depots = new vector<int>();
    // this->charges = new vector<int>();
}
Circuit::Circuit(Instance* inst, Remorque* remorque, list<Station*>* stations)
        : Circuit(inst, remorque) {
    this->stations->assign(stations->begin(), stations->end());
}

// Attention : ceci n'est pas le constructeur par copie car il prend un
// **pointeur** en paramètre
Circuit::Circuit(const Circuit* other) {

    // CECI FONCTIONNE AUSSI (avec copie des list et map le 22/2/2016)
    this->stations = new list<Station*>(*other->stations);  // ATTENTION aux *
    this->depots = new map<Station*,int>(*other->depots);  // ATTENTION aux *
    this->charges = new map<Station*,int>(*other->charges);  // ATTENTION aux *

    this->inst = other->inst;
    this->remorque = other->remorque;
    this->charge_init = other->charge_init;
    this->desequilibre = other->desequilibre;
    this->length = other->length;
}

Circuit::~Circuit() {
    delete this->stations;
    delete this->depots;
    delete this->charges;
}

void Circuit::copy(const Circuit* other) {
    this->inst = other->inst;
    this->remorque = other->remorque;
    this->charge_init = other->charge_init;
    this->desequilibre = other->desequilibre;
    this->length = other->length;

    this->stations->assign(other->stations->begin(), other->stations->end());
    *(this->depots) = *(other->depots);
    *(this->charges) = *(other->charges);
}

void Circuit::partial_clear() {
    this->charge_init = 0;
    this->desequilibre = 0;
    this->length = 0;
    this->depots->clear();
    this->charges->clear();
}

void Circuit::clear() {
    this->partial_clear();
    this->stations->clear();
}

// Recalcule l'équilibrage et met à jour les attributs dérivés
void Circuit::update() {
    this->partial_clear();
    this->equilibrate();

    // Mise à jour distance parcourue totale et déséquilibre global
    this->length = inst->get_dist(this->stations, this->remorque);
}


int Circuit::optimal_initial_load() {
    int capacity_remorque = this->remorque->getCapa();
    int kmax = capacity_remorque;
    int kmin = 0;
    int ksup = kmax;
    int kinf = kmin;
    int lmin = kinf;
    int lmax = ksup;
    int deficit, lackOfSpace, lackOfBikes;
    for (auto it = this->stations->begin(); it != this->stations->end(); ++it) {
        Station* station = *it;
        logn6(station->to_s_long());
        deficit = station->deficit();
        if (deficit < 0) {
            //Bikes from station to remorque
            lackOfSpace = abs(abs(deficit)-(capacity_remorque-lmax));
            if (abs(deficit)>capacity_remorque-lmax) {
                //Not enough space on the remorque for all bikes
                ksup -= lackOfSpace;
                if (ksup <= kinf) {
                    return kinf;
                }
            }
            lmin = min(kmax, lmin+abs(deficit));
            lmax = min(kmax, lmax+abs(deficit));
        } else if (deficit > 0) {
            //Bikes from remorque to station
            if (abs(deficit) <= lmin) {
                //Enough bikes on the remorque
                lmin -= abs(deficit);
                lmax -= abs(deficit);
            } else {
                //Not enough bikes on the remorque
                lackOfBikes = abs(lmin-abs(deficit));
                kinf += lackOfBikes;
                if (kinf >= ksup) {
                    return ksup;
                }
                lmin = max(kmin, lmin-abs(deficit));
                lmax = max(kmin, lmax-abs(deficit));
            }
        }
    }
    return kmin;
}

// Méthode d'équilibrage d'un circuit
void Circuit::equilibrate_dummy() {}

// Méthode d'équilibrage d'un circuit
void Circuit::equilibrate_circuit() {
    logn6("Circuit::equilibrate BEGIN");
    int deficit_station, newdeficit_station = 0, capacity_remorque_left;
    int initial_load = this->optimal_initial_load();
    this->charge_init = initial_load;
    this->current_load = initial_load;
    int capacity_remorque = remorque->getCapa();
    printf("Remorque %i with load %i\n", remorque->id, this->current_load);


    for (auto it = this->stations->begin(); it != this->stations->end(); ++it) {
        printf("---\n");
        Station* station = *it;
        deficit_station = station->deficit();
        capacity_remorque_left = capacity_remorque-this->current_load;

        logn7(station->to_s_long());
        logn7("Circuit::equilibrate: avant maj depots");

        if (deficit_station > 0) {
            //On prend des vélos de la remorque pour les mettre dans la station
            if (deficit_station < this->current_load) {
                printf("Cas Depot 1\n");
                printf("Current load %i\n", this->current_load);
                printf("Max %i\n", this->remorque->getCapa());
                printf("Desequilibre %i\n", abs(deficit_station));
                printf("Depot de %i vélos\n", deficit_station);
                printf("Vélos dans la station : %i\n", station->getNbvp());
                (*this->depots)[station] = deficit_station;
                printf("Après dépot : %i\n", station->getNbvp()+deficit_station);
                printf("Vélos sur la remorque : %i\n", this->current_load);
                this->current_load -= deficit_station;
                printf("Après dépot : %i\n", this->current_load);
                (*this->charges)[station] = this->current_load;
                newdeficit_station = 0;
            } else {
                printf("Cas Depot 1\n");
                printf("Current load %i\n", this->current_load);
                printf("Max %i\n", this->remorque->getCapa());
                printf("Desequilibre %i\n", abs(deficit_station));
                printf("Depot de %i vélos\n", this->current_load);
                printf("Vélos dans la station : %i\n", station->getNbvp());
                (*this->depots)[station] = this->current_load;
                printf("Après dépot : %i\n", station->getNbvp()+this->current_load);
                printf("Vélos sur la remorque : %i\n", this->current_load);
                newdeficit_station = abs(station->getIdeal()-(station->getNbvp() + this->current_load));
                this->current_load -= this->current_load;
                (*this->charges)[station] = this->current_load;
                printf("Après dépot : %i\n", this->current_load);
            }
        } else if (deficit_station < 0) {
            //On met des vélos de la station sur la remorque
            if (capacity_remorque_left > abs(deficit_station)) {
                printf("Cas Charge 1\n");
                printf("Current load %i\n", this->current_load);
                printf("Max %i\n", this->remorque->getCapa());
                printf("Desequilibre %i\n", abs(deficit_station));
                printf("Charge de %i vélos\n", abs(deficit_station));
                //On a la place nécessaire pour mettre tous les vélos et se ramener à l'idéal
                (*this->depots)[station] = deficit_station;
                printf("Vélos dans la station : %i\n", station->getNbvp());
                printf("Après charge : %i\n", station->getNbvp()-abs(deficit_station));
                printf("Vélos sur la remorque : %i\n", this->current_load);
                this->current_load += abs(deficit_station);
                printf("Après charge : %i\n", this->current_load);
                (*this->charges)[station] = this->current_load;
                newdeficit_station = 0;
            } else {
                printf("Cas Charge 2\n");
                printf("Current load %i\n", this->current_load);
                printf("Max %i\n", this->remorque->getCapa());
                printf("Desequilibre %i\n", abs(deficit_station));
                printf("Charge de %i vélos\n", capacity_remorque_left);
                //On met le maximum de vélos possibles, le déséquilibre sera non nul
                (*this->depots)[station] = -capacity_remorque_left;
                printf("Vélos dans la station : %i\n", station->getNbvp());
                printf("Après charge : %i\n", station->getNbvp() - capacity_remorque_left);
                printf("Vélos sur la remorque : %i\n", this->current_load);
                this->current_load += capacity_remorque_left;
                printf("Après charge : %i\n", this->current_load);
                (*this->charges)[station] = this->current_load;
                newdeficit_station = abs(abs(deficit_station)-capacity_remorque_left);
            }
        } else {
            (*this->depots)[station] = 0;
            (*this->charges)[station] = this->current_load;
            newdeficit_station = 0;
        }

        // incrémentation du desequilibre du circuit
        logn7("Circuit::equilibrate: avant maj desequilibre");
        this->desequilibre += abs(newdeficit_station);
    }

    printf("---\n");
    printf("Desequilibre final : %i\n", this->desequilibre);
    logn6("Circuit::equilibrate END");
}

//Method to equilibrate the deliveries on a circuit
void Circuit::equilibrate() {
    Circuit::equilibrate_circuit();
}

// Insertion d'une station dans un circuit à la position indiquée.
// - pos est un entier >= -1
// - si pos vaut -1 : insère à la fin
// - si pos vaut 0 (ou est absent) : insère au début
// - si pos vaut stations->size() : insère à la fin
// - si pos vaut plus que stations->size(), alors pos est forcé pour être
//   entre 0 et stations->size() (inclu !)
//
// L'appel à update est à la charge de l'appelant
//
void Circuit::insert(Station* station, int pos) {
    logn5("Circuit::insert BEGIN " + station->name + " pos=" + U::to_s(pos));
    if (pos == -1) {
        this->stations->push_back(station);
    } else if (pos == 0) {
        this->stations->push_front(station);
    } else {
        // On impose à pos d'être entre 0 et size (inclue)
        //// if (pos < 0) pos = 0;
        //// if (pos > this->stations->size()) pos = this->stations->size();
        pos = pos % (1 + this->stations->size());

        auto it = this->stations->begin();
        // on avance l'itérateur jusqu'à la position pos
        std::advance(it, pos); // version STL
        // for (int i = 0; i < pos; ++i) {
        //     it++; // version manuelle
        // }

        // on procède à l'insertion
        this->stations->insert(it, station);
    }
    logn5("Circuit::insert END");
}
// Insertion d'une station dans un circuit à une position est aléatoire.
//
// L'appel à update est à la charge de l'appelant
//
void Circuit::insert_rand(Station* station) {
    int pos = 0;
    if (this->stations->size() != 0) {
        pos = rand() % this->stations->size()+1;
    }
    logn5("Circuit::insert_rand BEGIN " + this->remorque->name + ": " + station->name + " pos=" + U::to_s(pos));
    auto it = this->stations->begin();
    // on avance l'itérateur jusqu'à la position pos
    std::advance(it, pos);

    // on procède à l'insertion
    this->stations->insert(it, station);
    logn5("Circuit::insert_rand END");
}


// Insert une station à un endroit spécifié par les options de la ligne de commande.
void Circuit::insert_from_option(Station* s) {
    // Options* args = Options::args;
    const string sinserter = Options::args->station_inserter;
    if (sinserter == "FRONT") {
        this->insert(s);
    } else if (sinserter == "BACK") {
        this->insert(s, -1);
    } else if (sinserter == "RAND") {
        this->insert_rand(s);
    } else {
        if (sinserter == "NONE") {
            cout << "La valeur par défaut NONE doit être modifée par le solveur.\n";
        }
        U::die("station_inserter inconnu : " + U::to_s(sinserter));
    }
}

// Supprime et retourne la station en position pos ;  pos doit être
// entre 0 et stations->size()-1
//
Station* Circuit::erase(int pos) {
    logn5("Circuit::erase BEGIN pos=" + U::to_s(pos));
    if (pos < 0 || pos > this->stations->size()-1 ) {
        cout << "Circuit::erase ERREUR pos=" << pos << " hors borne" << endl;
        exit(1);
    }
    auto it = this->stations->begin();
    // on avance l'itérateur jusqu'à la position pos
    std::advance(it, pos);
    Station* station = *it;
    this->stations->erase(it);
    logn5("Circuit::erase END");
    return station;
}

// inversion d'un chemin dans le circuit (2opt)
// inverse le chemin de parcours des stations comprises entre pos1 (inclue) et
// pos2 (exclue)
// Principe :
// - on passe par une liste temporaire de stations,
// - on déplace le chemin à inverser depuis le circuit vers la liste temporaire,
// - on redéplace la liste temporaire inversée dans le circuit,
// - 0 représente la première station du circuit,
// - stations->size() représente ce qui suit la dernière stattion, donc la
//   remorque elle-même,
// - EFFETS DE BORD : exit() si bornes incorrectes
// - LA SOLUTION N'EST PAS MISE À JOUR ! (pas de update)
//
void Circuit::reverse_branch(int pos1, int pos2) {
    U::die("Circuit::reverse_branch: méthode non implémentée !");
    //
    // 1. vérification des paramèters pos1 et pos2 (on assure pos1 < pos2)
    //
    // ...

    //
    // 2. déclaration et positionnement itérateurs associés aux positions
    //
    // ...

    //
    // 3. inversion de la branche (avec méthode splice de la STL)
    //
    // ...
}


// void Circuit::reverse_branch_test() {
//
// }
string Circuit::to_s() {
    stringstream buf;
    buf << "# Circuit associé à la remorque " <<  remorque->name <<
           " de capa " << remorque->capa << endl;
    buf << "#       id, charge_init, desequ, longueur\n";
    buf << "circuit " << remorque->name
        << "        " << this->charge_init
        << "        " << this->desequilibre
        << "       "  << this->length
        << endl;
    for (auto it = this->stations->begin(); it != this->stations->end(); it++) {
        Station* s = *it;
        // Différentes possibilités pour accéder à l'élément i
        // buf << "  " << s->name << " " << (*this->depots)[s]  << endl;
        buf << "  " << s->name << " " << this->depots->at(s) << endl;
    }
    buf << "end" << endl;
    return buf.str();
}
// Affichage détaillé d'une solution (format non standard mais plus détaillé) !
string Circuit::to_s_long() {
    stringstream buf;
    buf << "# Circuit détaillé associé à la remorque " <<  remorque->name <<
           " de capa "          << remorque->capa << endl;
    buf << "#   charge_init="   << this->charge_init <<endl;
    buf << "#   desequilibre="  << this->desequilibre <<endl;
    buf << "#   distance="      << this->length <<endl;

    Site* src = this->remorque;
    for (auto it = this->stations->begin(); it != this->stations->end(); ++it) {
        Station* dst = *it;
        Arc* arc = inst->get_arc(src, dst);
        int depot = this->depots->at((Station*)arc->dst);
        int charge = this->charges->at((Station*)arc->dst);
        buf << "   " <<  arc->to_s_long()
            << " depot de "  << depot
            << " => charge = " << charge << endl;
        src = dst;
    }
    if (stations->size() != 0) {
        buf << "   " <<  inst->get_arc(stations->back(), remorque)->to_s_long();
    }
    return buf.str();
}
//./
