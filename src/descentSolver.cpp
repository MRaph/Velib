#include "solver.hpp"
#include <math.h>

DescentSolver::DescentSolver(Instance* inst) : Solver::Solver(inst) {
    name = "DescentSolver";
    desc = "Résolution par acceptation systématique d'un voisinage\n";
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }

    this->testsol = new Solution(inst);
    testsol->solve_stupid();
    this->cursol = new Solution(this->testsol);
    this->bestsol = new Solution(this->testsol);

    cerr << "DescentSolver non implémenté" << endl;
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }
    exit(1);
}
DescentSolver::~DescentSolver()  {
}

// Méthode principale de ce solver, principe :
// On part d'un circuit fixé C0, d'un température T0
// On effectue deux types d'itérations :
// La première correspond à une baisse de la température du système
// La deuxième correspond à une tentative d'amélioration de circuits
// Dans l'amélioration du circuit :
// - Si le poids est inférieur, on accepte le nouveau circuit
// - Si le poids est supérieur, on accepte si Uniforme(0,1) < exp(-(cost(new)-cost(old))/T0)
// On accepte beaucoup de circuit au départ, puis au fur et à mesure, de moins en moins
// car la température du système est updaté suivant : T(n+1) = temperature_update * T(n)
bool DescentSolver::solve() {
    if (log4()) {
        logn4("DescentSolver::solve BEGIN");
    }
    if (Options::args->explore == "recuit") {
        this->found = solve_recuit_simule();
    } else if (Options::args->explore == "all") {
        this->found = solve_explore_everything();
    } else if (Options::args->explore == "strict"){
        this->found = solve_pure_descent();
    } else {
        printf("Argument explore has to be set to recuit, all or strict");
    }

    if (log4()) {
        logn4("End solve DescentSolver");
    }
    this->found = true;
    return found;
}

// On accepte systématiquement chaque voisin
bool DescentSolver::solve_explore_everything() {
    if (log4()) {
        logn4("DescentSolver::solve_explore_everything BEGIN");
    }
    Options* args = Options::args;
    int nb_iter = args->itermax, i, diff;
	Solution* solution_current = new Solution(this->cursol);

    for (i=0; i<nb_iter; i++) {
        mutate(solution_current);
        diff = solution_current->get_cost() - this->bestsol->get_cost();
        if (diff < 0) {
            this->bestsol->copy(solution_current);
        }
    }

    if (log4()) {
        logn4("DescentSolver::solve_explore_everything END");
    }
    return true;
}

// On accepte que les voisins améliorants
bool DescentSolver::solve_pure_descent() {
    if (log4()) {
        logn4("DescentSolver::solve_pure_descent BEGIN");
    }
    Options* args = Options::args;
    int nb_iter = args->itermax, i, diff;
	Solution* solution_current = new Solution(this->cursol);

    for (i=0; i<nb_iter; i++) {
        mutate(solution_current);
        diff = solution_current->get_cost() - this->bestsol->get_cost();
        if(diff < 0) {
            // On met à jour la meilleure solution
            this->bestsol->copy(solution_current);
		} else {
			// On recommence avec la meilleure sol
            solution_current->copy(this->bestsol);
        }
    }
    if (log4()) {
        logn4("DescentSolver::solve_pure_descent END");
    }
    return true;
}

// Algorithme du recuit simulé pour une temperature initiale
// non-nulle et non-infinie
bool DescentSolver::solve_recuit_simule() {
    if (log4()) {
        logn4("DescentSolver::solve_recuit_simule BEGIN");
    }
    Options* args = Options::args;
    double temperature_init = args->temp_init;
    double temperature_update = 0.99;
    int nb_iterations_temperature = 30;
    double temperature_current = temperature_init;
    double criteria_stop = 0.000001*temperature_init;

    Solution* solution_current;

    int nb_iterations_ameliorations;
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
                this->cursol->copy(solution_current);
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
                }
            }
            nb_iterations_ameliorations += 1;
        }
        temperature_current = temperature_current*temperature_update;
    }
    if (log4()) {
        logn4("DescentSolver::solve_pure_descent END");
    }
    return true;
}

// Effectue une mutation sur la solution en paramètre et renvoie un voisin.
void DescentSolver::mutate(Solution* sol) {
    // Deux possibilités pour trouver un voisin :
    // - changer la position de deux stations au sein d'un meme circuit (2opt)
    // - retirer une station d'un circuit pour l'ajouter à un autre
    // Le choix de l'un ou l'autre est fait avec une probabilité 50-50
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
        // Circuits chosen are the same, we have to mutate two stations in the same circuit
        if(circuit_1->stations->size()>=2){
            int k = rand() % (circuit_1->stations->size()-1);
            int l = (rand() % (circuit_1->stations->size()-k-1)) + 2 + k;
            if (k!=l) {
                circuit_1->mutate_2opt(min(k,l), max(k,l));
            } else {
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
