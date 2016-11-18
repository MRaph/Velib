#include "solver.hpp"

CarloSolver::CarloSolver(Instance* inst) : Solver::Solver(inst) {
    name = "CarloSolver";
    desc = "Résolution par algorithme de Monte-Carlo\n"
            ;
    logn1(name + ": " + desc + " inst: " + inst->name);
    this->solution = new Solution(inst);

    logn1("Apres  CarloSolver::CarloSolver pour instance " + inst->name);
}
CarloSolver::~CarloSolver()  {
    // delete this->solution;
}

// Méthode principale de ce solver, principe :
//
bool CarloSolver::solve() {
    if (log2()) {
        cout << "\n---CarloSolver::solve START size="
             << inst->stations->size() << "---\n";
    }
    Options* args = Options::args;
    int itermax = args->itermax;
    // Par défaut (-1) on ne fait qu'une seule itération
    itermax =  itermax == -1 ? 1 : itermax;

    Solution* currentsol = new Solution(inst);
    for (int iter = 0; iter < itermax; ++iter) {
        this->apply_one_greedy(currentsol);
        // La nouvelle solution est-elle meilleure que la solution courante ?
        if (currentsol->get_cost() < this->solution->get_cost()) {
            // On doit vérifier que chaque remorque visite au moins une
            // station
            bool is_valid = true;
            for (auto it = currentsol->circuits->begin();
                      it != currentsol->circuits->end();
                      ++it) {
                if ( (*it)->length == 0 ) {
                    is_valid = false;
                }
            }
            if (is_valid) {
                // RECORD BATTU
                this->solution->copy(currentsol);
                if (log1()) {
                    // ATTENTION FAQ : bufférisation et cout.flush() ou << flush
                    // L'utilisation de endl au lieu de "\n" est inutile ici car
                    // on veut afficher la chaine qui suite ce endl. Il faut
                    // donc ajouter un flush du flux pour garantir l'affichage
                    // immédiat.
                    cout << "\n" << iter << ": " << currentsol->get_cost() << flush;
                }
                // On enregistre cette solution dans un fichier temporaire
                string tmpname = this->solution->get_tmp_filename();
                U::write_file(tmpname, this->solution->to_s());
            } else {
                // Solution invalide
                // log2("x"); // afficherait "L2: x" or on veut seulement "x"
                if (log2()) { cout << "x" << flush; }
            }
        } else {
            // solution non intéressante
            // log2("."); // afficherait "L2." or on veut seulement "."
            if (log2()) { cout << "." << flush; }
        }
    }
    if (log1()) { cout << endl << flush; } // on ne veut pas voir le prefix "L1:"
    logn2("CarloSolver::solve: END");
    this->found = true;
    // cout << this->solution->to_s_long();
    return found;
}
// Construire la solution en paramètre par un glouton.
// Principe : On prend les stations dans l'ordre de l'instance, puis
// on les affecte à chaque remorque.
// - la remorque est sélectionnée selon l'option remorque_chooser
// - la station est tirée au hazard selon l'option station_chooser
// - le mode d'insertion d'une station dans le circuit courant de la
//   remorque est sélectionné selon l'option station_inserter
//
// La solution passée en paramètre est vidée puis recontruite mais
// l'objet reste le même : il est simplement modifié.
// L'objet Solution passé en paramètre est donc entièrement gérée par la
// méthode appelante (construction puis destruction).
//
Solution* CarloSolver::apply_one_greedy(Solution* sol) {
    // U::die("CarloSolver::apply_one_greedy: non implémenté !");
    logn4("CarloSolver::apply_one_greedy BEGIN");
    sol->clear();

    auto stations = new vector<Station*>(*inst->stations);
    if (Options::args->station_chooser == "RAND") {
        // On mélange le vector par la lib standard c++
        random_shuffle(stations->begin(), stations->end());
    };

    int remorque_id = -1; // sélection des remorques à tour de rôle
    for (unsigned j = 0; j < stations->size(); j++) {
        Station* station = stations->at(j);

        auto rchooser = Options::args->remorque_chooser;
        if (rchooser == "ALT") {
            remorque_id = (remorque_id + 1) % inst->remorques->size();
        } else if (rchooser == "RAND") {
            remorque_id = rand() % inst->remorques->size();
        } else {
            U::die("remorque_chooser inconnu : " + U::to_s(rchooser));
        }
        Circuit* circuit = sol->circuits->at(remorque_id);

        // if (circuit->remorque->name == "r2" && station->name == "s8") {
        //     Log::level += 7;
        // }
        circuit->insert_from_option(station);
    }
    logn5("CarloSolver::apply_one_greedy: avant appel à sol->update");
    sol->update();

    logn4("CarloSolver::apply_one_greedy END");
    return sol;
};

//./
