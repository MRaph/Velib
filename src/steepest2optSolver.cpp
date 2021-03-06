#include "solver.hpp"

Steepest2optSolver::Steepest2optSolver(Instance* inst) : Solver::Solver(inst) {
    name = "Steepest2optSolver";
    desc = "Résolution par acceptation systématique d'un voisinage\n";
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }

    this->testsol = new Solution(inst);
    testsol->solve_stupid();
    this->cursol = new Solution(this->testsol);
    this->bestsol = new Solution(this->testsol);
}

Steepest2optSolver::~Steepest2optSolver()  {

}


// 1) Construction solution initiale par glouton stupide :----------------------------------------------------------------------------
// Pareil que StupidSolver::solve() mais retourne une sol et non pas un bool :
Solution* Steepest2optSolver::Stupid_solver(Solution* sol) {
    if (log3()) {
        cout << "\n---StupidSolver::solve START instance: "
             << inst->name << " de taille "
             << inst->stations->size() << "\n";
    }
    sol->clear();
    // Principe : On prend les stations dans l'ordre de l'instance, puis
    // on les affecte a chaque remorque � tour de r�le.

    int remorque_id = -1;
    for (unsigned j = 0; j < inst->stations->size(); j++) {
        // on extrait la prochaine station � visiter
        Station* station = inst->stations->at(j);
        // s�lection des remorques � tour de r�le
        remorque_id = (remorque_id + 1) % inst->remorques->size();
        // on extrait le circuit associ� � la remorque s�lectionn�e
        Circuit* circuit = sol->circuits->at(remorque_id);
        // on ajoute la station en fin de ce circuit
        if (log5()) {
            logn5("StupidSolver::solve: ajout de la station " + station->name +
            " à la remorque " + circuit->remorque->name);
        }
        circuit->stations->push_back(station);
        // update inutile ici car n'a pas (encore) besoin de mesurer la
        // stupidit� de cette insertion !
    }
    if (log4()) {
        logn4("StupidSolver::solve: avant appel � sol->update");
    }
    sol->update();
    if (log3()) {
        logn3("StupidSolver::solve: END");
    }
    return sol;
};


// Construction solution initiale par glouton aleatoire :----------------------------------------------------------------------------
// On reprend la fonction apply_one_greedy du CarloSolver

// Construire la solution en param�tre par un glouton.
// Principe : On prend les stations dans l'ordre de l'instance, puis
// on les affecte � chaque remorque.
// - la remorque est s�lectionn�e selon l'option remorque_chooser
// - la station est tir�e au hazard selon l'option station_chooser
// - le mode d'insertion d'une station dans le circuit courant de la
//   remorque est s�lectionn� selon l'option station_inserter
//
// La solution pass�e en param�tre est vid�e puis recontruite mais
// l'objet reste le m�me : il est simplement modifi�.
// L'objet Solution pass� en param�tre est donc enti�rement g�r�e par la
// m�thode appelante (construction puis destruction).
//

Solution* Steepest2optSolver::glouton_aleatoire(Solution* sol) {
    if (log4()) {
        logn4("Steepest2optSolver::apply_one_greedy BEGIN");
    }
    sol->clear();
    Station* station;
    Circuit* circuit;

    //auto stations = new vector<Station*>(*inst->stations);
    if (Options::args->station_chooser == "RAND") {
        // On m�lange le vector par la lib standard c++
        random_shuffle(inst->stations->begin(), inst->stations->end());
    };

    int remorque_id = -1; // s�lection des remorques � tour de r�le
    for (unsigned j = 0; j < inst->stations->size(); j++) {
        station = inst->stations->at(j);

        auto rchooser = Options::args->remorque_chooser;
        if (rchooser == "ALT") {
            remorque_id = (remorque_id + 1) % inst->remorques->size();
        } else if (rchooser == "RAND") {
            remorque_id = rand() % inst->remorques->size();
        } else {
            U::die("remorque_chooser inconnu : " + U::to_s(rchooser));
        }
        circuit = sol->circuits->at(remorque_id);
        circuit->insert_from_option(station);

    }
    if (log5()) {
        logn5("Steepest2optSolver::apply_one_greedy: avant appel à sol->update");
    }
    sol->update();
    if (log4()) {
        logn4("Steepest2optSolver::apply_one_greedy END");
    }
    return sol;
};


// 2)Methode stupid de ce solver :----------------------------------------------------------------------------
// "Effectue une seule descente à partir de la solution (deterministe) issue du glouton StupidSolver"
bool Steepest2optSolver::solve_stupid() {
    this->cursol = new Solution(inst);
    this->cursol = Stupid_solver(this->cursol);
    mutate(this->cursol);
    if(this->cursol->get_cost() < this->bestsol->get_cost()) {
      this->bestsol->copy(this->cursol);
    }
    this->found = true;
    return found;
};




// 3)Methode principale de ce solver :----------------------------------------------------------------------------
// S'enchaine un nombre parametrable de fois : on utilise la classe Option !
bool Steepest2optSolver::solve() {
    Options* args = Options::args;
    int itermax = args->itermax;
    this->bestsol->copy(this->testsol);
    this->cursol->copy(this->testsol);

    for(int i=1; i<itermax; i++){
        if (log7()) {
            logn7("Iteration " + std::to_string(i) + " du steepestSolver");
        }
        // this->cursol = new Solution(inst);
        this->cursol->copy(glouton_aleatoire(this->cursol));
        mutate(this->cursol);

        if(this->cursol->get_cost() < this->bestsol->get_cost()) {
            this->bestsol->copy(this->cursol);
            this->bestsol->update();
        }
    }
    this->found = true;
    return found;
};


// 4) Effectue une mutation sur la solution en parametre :----------------------------------------------------------------------------
void Steepest2optSolver::mutate(Solution* sol) {
    logn4("Steepest2optSolver::mutate BEGIN");

    for(int i=0;i<sol->circuits->size();i++){
        if (log8()) {
            logn8("-- Circuit numéro "+ std::to_string(i));
        }
        sol->circuits->at(i) = sol->circuits->at(i)->mutate_2opt_steepest(100000);
    }

    sol->update();
    logn4("Steepest2optSolver::mutate END");
};
