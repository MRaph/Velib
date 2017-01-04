#include "solver.hpp"

AnnealingSolver::AnnealingSolver(Instance* inst) : Solver::Solver(inst) {
    name = "AnnealingSolver";
    desc = "Solver par recuit simulé";
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }

    this->testsol = new Solution(inst);
    testsol->solve_stupid(); // Create basic solution
    this->cursol = new Solution(this->testsol);
    this->bestsol = new Solution(this->testsol);

    solve();
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }
    exit(1);
}

AnnealingSolver::~AnnealingSolver()  {
    if (this->testsol) {
        delete this->testsol;
    }
    if (this->bestsol) {
        delete this->bestsol;
    }
    if (this->cursol) {
        delete this->cursol;
    }
}

// Méthode principale de ce solver
//
// Algorithme du recuit simulé pour une temperature initiale
// non-nulle et non-infinie
bool AnnealingSolver::solve() {
    if (log4()) {
        logn4("DescentSolver::solve_recuit_simule BEGIN");
    }
    Options* args = Options::args;
    double temperature_init = args->temp_init;
    double temperature_update = 0.99;
    int nb_iterations_temperature = args->itermax;
    double temperature_current = temperature_init;
    double criteria_stop = 0.000001*temperature_init;

    Solution* solution_current;

    int nb_iterations_ameliorations=0;
    int diff;
    double r;
    while (temperature_current > criteria_stop) {
        nb_iterations_ameliorations = 0;
        if (log4()) {
            logn4("Temperature actuelle " + std::to_string(temperature_current));
        }
        while (nb_iterations_ameliorations < nb_iterations_temperature) {
            // On obtient une solution dans un voisinage de la solution actuelle
            solution_current = new Solution(this->cursol);
            mutate(solution_current);
            // On calcule la différence de cout entre les deux solutions
            diff = solution_current->get_cost() - this->bestsol->get_cost();
            // Cas où la solution trouvée est meilleure
            if (diff < 0) {
                if (log7()) {
                    logn7("New solution with lower cost has been found");
                }
                // Mise à jour de la valeur de la solution optimale
                this->bestsol->copy(solution_current);
                this->bestsol->update();
                this->cursol->copy(solution_current);
                this->cursol->update();
            // Cas où la solution trouvée est moins bonne
            } else {
                // On prend un nombre aléatoire entre 0 et 1
                r = ((double) rand() / (RAND_MAX));
                // On le compare à exp(-diff/T0)
                if (r < exp(-diff/temperature_init)) {
                    // Cas où la solution moins bonne est tout de même retenue
                    if (log7()) {
                        logn7("New solution with higher cost has been found.");
                    }
                    this->cursol->copy(solution_current);
                    this->cursol->update();
                }
            }
            nb_iterations_ameliorations += 1;
        }
        temperature_current = temperature_current*temperature_update;
    }
    printf("Final score : %d\n", this->bestsol->get_cost());

    if (log4()) {
        logn4("DescentSolver::solve_pure_descent END");
    }
    return true;
}


// Effectue une mutation sur la solution en paramètre et renvoie un voisin.
void AnnealingSolver::mutate(Solution* sol) {
    // Deux possibilités pour trouver un voisin :
    // - changer la position de deux stations au sein d'un meme circuit (2opt)
    // - retirer une station d'un circuit pour l'ajouter à un autre
    if (log4()) {
        logn4("DescentSolver::mutate BEGIN");
    }

    bool are_different_circuits;
    int circuit_1_int, circuit_2_int, length_circuit_1, length_circuit_2, nb_circuits, single_position;
    int remove_position, add_position;

    // On choisit au hasard deux circuits
    nb_circuits = sol->circuits->size();
    circuit_1_int = rand() % nb_circuits;
    circuit_2_int = rand() % nb_circuits;
    // On teste si la mutation est interne à un circuit ou entre deux circuits
    are_different_circuits = (circuit_1_int == circuit_2_int);
    // On choisit deux stations dans chaque circuit
    Circuit* circuit_1 = sol->circuits->at(circuit_1_int);
    Circuit* circuit_2 = sol->circuits->at(circuit_2_int);
    length_circuit_1 = circuit_1->stations->size();
    length_circuit_2 = circuit_2->stations->size();
    if (are_different_circuits) {
        if (log7()) {
            logn7("Une station a été déplacé d'un circuit à un autre.");
        }
        if (length_circuit_1 < 2 && length_circuit_2 < 2) {
            // Switch remorque between the two circuits
            // Even if it seems useless, remorque capacity can modify the score
            Station* station_circuit_1 = circuit_1->erase(0);
            Station* station_circuit_2 = circuit_2->erase(0);
            circuit_1->insert(station_circuit_2, 0);
            circuit_2->insert(station_circuit_1, 0);
            return;
        } else if (length_circuit_1 == 1 && length_circuit_2 > 0) {
            // Remove circuit 1 and add station of the circuit to the other circuit
            single_position = rand() % length_circuit_2+1; // +1 pour insérer à la fin du circuit
            Station* station_to_move = circuit_1->erase(0);
            circuit_2->insert(station_to_move, single_position);
            return;
        } else if (length_circuit_2 == 1 && length_circuit_1 > 0) {
            // Remove circuit 2 and add station of the circuit to the other circuit
            single_position = rand() % length_circuit_1+1; // +1 pour insérer à la fin du circuit
            Station* station_to_move = circuit_2->erase(0);
            circuit_1->insert(station_to_move, single_position);
            return;
        } else {
            // We remove a station from circuit 1 and add it to circuit 2
            remove_position = rand() % length_circuit_1;
            add_position = rand() % length_circuit_2+1; // +1 because you can add at the end
            Station* station_to_move = circuit_1->erase(remove_position);
            circuit_2->insert(station_to_move, add_position);
            return;
        }
    } else {
        if (log7()) {
            logn7("Deux stations au sein d'un même circuit ont été échangées.");
        }
        // Circuits chosen are the same, we have to mutate two stations in the same circuit
        if (circuit_1->stations->size()>=2) {
            int k = rand() % (circuit_1->stations->size()-1);
            int l = rand() % (circuit_1->stations->size()-1);
            if (k != l) {
                circuit_1->mutate_2opt(min(k,l), max(k,l));
            } else {
                if (k == circuit_1->stations->size()-1) {
                    k -= 1;
                }
                l = k+1;
                circuit_1->mutate_2opt(k, l);
            }
		}
    }

    if (log4()) {
        logn4("DescentSolver::mutate END");
    }
};

//./
