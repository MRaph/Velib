#include "solver.hpp"

// Constructeur de l'algorithme de recuit simulé
AnnealingSolver::AnnealingSolver(Instance* inst) : Solver::Solver(inst) {
    name = "AnnealingSolver";
    desc = "Solver par recuit simulé";
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }
    // Instanciation of new basic solution
    this->testsol = new Solution(inst);
    testsol->solve_stupid(); // Create basic solution
    this->cursol = new Solution(this->testsol);
    this->bestsol = new Solution(this->testsol);
    // We apply the recuit simule algorithm
    solve();
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }
    exit(1);
}

// Destructeur
AnnealingSolver::~AnnealingSolver()  {
    // If testsol exists, we delete it.
    if (this->testsol) {
        delete this->testsol;
    }
    // If bestsol exists, we delete it.
    if (this->bestsol) {
        delete this->bestsol;
    }
    // If cursol exists, we delete it.
    if (this->cursol) {
        delete this->cursol;
    }
}

// Méthode principale de ce solver
bool AnnealingSolver::solve() {
    if (log4()) {
        logn4("DescentSolver::solve_recuit_simule BEGIN");
    }
    // We get the arguments from the optionparser
    Options* args = Options::args;
    double temperature_init = args->temp_init;
    double temperature_update = 0.99;
    int nb_iterations_temperature = args->itermax;
    double temperature_current = temperature_init;
    double criteria_stop = 1.0;

    Solution* solution_current;

    int nb_iterations_ameliorations=0;
    int diff;
    double r;
    int count = 0;

    // We iterate over the temperature, which are updated as 0.99*previous_temperature
    while (temperature_current > criteria_stop) {
        if ((count % 15 == 0) && log2()) {
            logn2("Temperature actuelle : " + std::to_string(temperature_current) + " - score - " + std::to_string(this->bestsol->get_cost()));
        }
        nb_iterations_ameliorations = 0;
        // We apply N random searches at each step of temperature
        while (nb_iterations_ameliorations < nb_iterations_temperature) {
            // We look for a neighbour of the current solution
            solution_current = new Solution(this->cursol);
            mutate(solution_current);
            // We compuet the difference of score
            diff = solution_current->get_cost() - this->bestsol->get_cost();
            if (diff <= 0) {
                // It improves, we remember the mutation as the new best solution
                if (log2()) {
                    logn2("New solution with lower cost has been found : " + std::to_string(solution_current->get_cost()));
                }
                this->bestsol->copy(solution_current);
                this->cursol->copy(solution_current);
            } else {
                // It doesn't improve, we remember the solution if rand() < exp(-diff/T_init)
                // We pick a random number between 0 and 1
                r = ((double) rand() / (RAND_MAX));
                // We compare it to exp(-diff/T0)
                if (r < exp(-diff/temperature_init)) {
                    // We remember the mutation as the current solution
                    if (log3()) {
                        logn3("New solution with higher cost has been found : " + std::to_string(solution_current->get_cost()) + "/" + std::to_string(this->bestsol->get_cost()));
                    }
                    this->cursol->copy(solution_current);
                }
            }
            nb_iterations_ameliorations += 1;
        }
        // We update the temperature of the system.
        temperature_current = temperature_current*temperature_update;
    }
    if (log2()) {
        logn2("Final score : " +  std::to_string(this->bestsol->get_cost()));
    }
    if (log4()) {
        logn4("DescentSolver::solve_pure_descent END");
    }

    Solution::save(this->bestsol, args);
    return true;
}


// Effectue une mutation sur la solution en paramètre et renvoie un voisin.
void AnnealingSolver::mutate(Solution* sol) {
    // Deux possibilités pour trouver un voisin :
    // - changer la position de deux stations au sein d'un meme circuit (2opt)
    // - retirer une station d'un circuit pour l'ajouter à un autre
    // Le choix de l'un ou l'autre est fait avec une probabilité 50-50
    if (log4()) {
        logn4("DescentSolver::mutate BEGIN");
    }

    bool are_different_circuits;
    int circuit_1_int, circuit_2_int, length_circuit_1, length_circuit_2, nb_circuits;
    int remove_position, add_position;
    Station* station_circuit_2;
    Station* station_circuit_1;
    Station* station_to_move;

    // We pick two random circuits
    nb_circuits = sol->circuits->size();
    circuit_1_int = rand() % nb_circuits;
    circuit_2_int = rand() % nb_circuits;
    // We test to see if the circuits are different
    are_different_circuits = (circuit_1_int == circuit_2_int);
    // We pick two random stations in each circuit
    Circuit* circuit_1 = sol->circuits->at(circuit_1_int);
    Circuit* circuit_2 = sol->circuits->at(circuit_2_int);
    length_circuit_1 = circuit_1->stations->size();
    length_circuit_2 = circuit_2->stations->size();
    bool is_circuit_1_empty = (length_circuit_1 == 0);
    bool is_circuit_2_empty = (length_circuit_2 == 0);

    if (is_circuit_2_empty && is_circuit_1_empty) {
        if (log3()) {
            logn3("AnnealingSolver::mutate - Both circuits are empty.");
        }
        // Do nothing
        return;
    } else if (is_circuit_1_empty && !is_circuit_2_empty) {
        if (log3()) {
            logn3("AnnealingSolver::mutate - Circuit 1 is empty.");
        }
        // Move station of circuit 2 to 1
        remove_position = rand() % length_circuit_2;
        station_circuit_2 = circuit_2->erase(remove_position);
        circuit_1->insert(station_circuit_2, 0);
        return;
    } else if (!is_circuit_1_empty && is_circuit_2_empty) {
        if (log3()) {
            logn3("AnnealingSolver::mutate - Circuit 2 is empty.");
        }
        // Move station of circuit 1 to 2
        remove_position = rand() % length_circuit_1;
        station_circuit_1 = circuit_1->erase(remove_position);
        circuit_2->insert(station_circuit_1, 0);
        return;
    } else {
        // Both circuits aren't empty
        if (are_different_circuits) {
            // Circuits are different and non-empty
            if (log3()) {
                logn3("AnnealingSolver::mutate - Switching stations");
            }
            // We remove a station from circuit 1 and add it to circuit 2
            remove_position = rand() % length_circuit_1;
            add_position = rand() % length_circuit_2+1; // +1 to insert at the end
            station_to_move = circuit_1->erase(remove_position);
            circuit_2->insert(station_to_move, add_position);
            return;
        } else {
            // Same circuit, non-empty
            if (circuit_1->stations->size()>1) {
                if (log3()) {
                    logn3("AnnealingSolver::mutate - Intern mutation accepted.");
                }
                // Pick two random positions
                int k = rand() % (circuit_1->stations->size()-1);
                int l = rand() % (circuit_1->stations->size()-1);
                if (k != l) {
                    // mutate_2opt implies that first argument is lower than second one.
                    circuit_1->mutate_2opt(min(k,l), max(k,l));
                } else {
                    // if k is the last position, we switch the last two stations.
                    l = k+1;
                    circuit_1->mutate_2opt(k, l);
                }
            } else {
                if (log3()) {
                    logn3("AnnealingSolver::mutate - Intern mutation refused : Circuit is too short.");
                }
            }
        }
    }

    sol->update();
    if (log4()) {
        logn4("DescentSolver::mutate END");
    }
};

//./
