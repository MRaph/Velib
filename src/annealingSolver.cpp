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
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }
    solve();
    return;
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
    double criteria_stop = 1;

    Solution* solution_current;

    int nb_iterations_ameliorations=0;
    int diff;
    double r;

    // We iterate over the temperature, which are updated as 0.99*previous_temperature
    while (temperature_current > criteria_stop) {
        if (log2()) {
            printf("%f \t %d \t %d\n", temperature_current, this->cursol->get_cost(), this->bestsol->get_cost());
        }
        nb_iterations_ameliorations = 0;
        // We apply N random searches at each step of temperature
        while (nb_iterations_ameliorations < nb_iterations_temperature) {
            // We look for a neighbour of the current solution
            solution_current = new Solution(this->cursol);
            mutate(solution_current);
            // We compuet the difference of score
            diff = solution_current->get_cost() - this->cursol->get_cost();
            if (diff <= 0) {
                // It improves, we remember the mutation as the new best solution
                if (log3()) {
                    logn3("New solution with lower cost has been found : " + std::to_string(solution_current->get_cost()));
                }
                if (solution_current->get_cost() < this->bestsol->get_cost()) {
                    this->bestsol->copy(solution_current);
                }
                this->cursol->copy(solution_current);
            } else {
                // It doesn't improve, we remember the solution if rand() < exp(-diff/T_init)
                // We pick a random number between 0 and 1
                r = ((double) rand() / (RAND_MAX));
                // We compare it to exp(-diff/T0)
                if (r < exp(-diff/temperature_current)) {
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
        logn4("AnnealingSolver::mutate BEGIN");
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
                    this->mutate_2opt(circuit_1, min(k,l), max(k,l));
                } else {
                    // if k is the last position, we switch the last two stations.
                    l = k+1;
                    this->mutate_2opt(circuit_1, k, l);
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
        logn4("AnnealingSolver::mutate END");
    }
};

//.Effectue une mutation basée sur le désequilibre actuel de tous les circuits
void AnnealingSolver::mutate_based_on_desequilibre(Solution* sol) {
    if (log4()) {
        logn4("AnnealingSolver::mutate_based_on_desequilibre BEGIN");
    }

    bool are_different_circuits;
    int circuit_1_int, circuit_2_int, length_circuit_1, length_circuit_2, nb_circuits;
    int remove_position, add_position;
    Station* station_circuit_1;
    Station* station_circuit_2;
    Station* station_to_move;

    nb_circuits = sol->circuits->size();
    vector<float> probabilities = convert_desequilibre_to_probability_vector(sol);
    circuit_1_int = select_circuit_according_to_probabilities(probabilities);
    circuit_2_int = rand() % nb_circuits;

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
    } else if (is_circuit_1_empty && !is_circuit_2_empty) {
        if (log3()) {
            logn3("AnnealingSolver::mutate - Circuit 1 is empty.");
        }
        // Move station of circuit 2 to 1
        remove_position = rand() % length_circuit_2;
        station_circuit_2 = circuit_2->erase(remove_position);
        circuit_1->insert(station_circuit_2, 0);
    } else if (!is_circuit_1_empty && is_circuit_2_empty) {
        if (log3()) {
            logn3("AnnealingSolver::mutate - Circuit 2 is empty.");
        }
        // Move station of circuit 1 to 2
        remove_position = rand() % length_circuit_1;
        station_circuit_1 = circuit_1->erase(remove_position);
        circuit_2->insert(station_circuit_1, 0);
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
    // delete station_circuit_1;
    // delete station_circuit_2;
    // delete station_to_move;
    // delete circuit_1;
    // delete circuit_2;
    if (log4()) {
        logn4("AnnealingSolver::mutate_based_on_desequilibre END");
    }

}


vector<float> AnnealingSolver::convert_desequilibre_to_probability_vector(Solution* sol) {
    int nombre_circuits = sol->circuits->size();
    std::vector<float> desequilibres(nombre_circuits);
    std::vector<float> probabilities(nombre_circuits);
    int i;
    int desequilibre_max=1;
    int desequilibre=0;
    Circuit* circuit;

    // Initializing desequilibres in a vector, finding the largest one
    for (i = 0; i < nombre_circuits; i++) {
        circuit = sol->circuits->at(i);
        desequilibre = circuit->desequilibre;
        desequilibres.at(i) = desequilibre;
        probabilities.at(i) = desequilibre;
        if (desequilibre > desequilibre_max) {
            desequilibre_max = desequilibre;
        }
    }

    // Normalizing desequilibres according to desequilibre_max
    if (desequilibre_max > 0) {
        for (i=0; i < nombre_circuits; i++) {
            probabilities.at(i) /= desequilibre_max;
        }
    } else {
        for (i=0; i < nombre_circuits; i++) {
            probabilities.at(i) = 1/nombre_circuits;
        }
    }

    return probabilities;
}

int AnnealingSolver::select_circuit_according_to_probabilities(vector<float> probabilities) {
    int position=0;
    int i=0;
    int size = probabilities.size();
    float sum = 0;
    float random_value = ((float) rand()/(RAND_MAX));

    while (sum<random_value && i<size) {
        sum += probabilities.at(i);
        i += 1;
    }
    position = i;

    return position;
}
