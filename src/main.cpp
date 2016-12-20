#include "common.hpp"

#include "instance.hpp"
#include "solver.hpp"

#include "options.hpp"
#include "test.hpp"

// je dois définir cette variable statique en dehors de la classe Options
Options* Options::args = NULL;

void test_action(Options* args) {
    // logn1("s=0 => Test construction d'instance new_velib_mini\n");
    // Instance* inst = Instance::new_velib_mini();
    // cout << inst->to_s();
    // return 0;

    test_initial_load();
    //test_circuit_reverse_branch();
    // test_circuit_move_branch_to();
    //test_util_rand();
    // test_circuit_move();
    // test_circuit_move_to();
}

void instance_action(Options* args) {
    logn1("instance_action BEGIN : lecture, affichage d'une instance");
    Instance* inst = new Instance(args->filename, args);
    inst->print_instance();
    inst->print_instance("mini_rand.dat");
    delete inst;
    logn1("instance_action END");
}

void stupid_solver_action(Options* args) {
    logn1("stupid_solver_action BEGIN");
    Instance* inst = new Instance(args->filename, args);
    StupidSolver* solver = new StupidSolver(inst);
    solver->solve();
    if (solver->found) {
        Solution::save(solver->get_solution(), args);
    } else {
        cout << "StupidSolver : pas de solution" << endl;
    }
    delete solver;
    delete inst;
    logn1("stupid_solver_action END");
}

void carlo_solver_action(Options* args) {
    logn1("carlo_solver_action BEGIN");
    Instance* inst = new Instance(args->filename, args);
    CarloSolver* solver = new CarloSolver(inst);
    solver->solve();
    if (solver->found) {
        Solution::save(solver->get_solution(), args);
    } else {
        cout << "Solver : pas de solution "  << endl;
    }
    delete solver;
    delete inst;
    logn1("carlo_solver_action END");
}

void steepest2opt_solver_action(Options* args) {
    logn1("steepest2opt_solver_action BEGIN");
    Instance* inst = new Instance(args->filename, args);
    Steepest2optSolver* solver = new Steepest2optSolver(inst);
    solver->solve_stupid();
    if (solver->found) {
        Solution::save(solver->get_solution(), args);
    } else {
        cout << "Solver : pas de solution "  << endl;
    }
    delete solver;
    delete inst;
    logn1("steepest2opt_solver_action END");
}

void descent_solver_action(Options* args) {
    logn1("descent_solver_action BEGIN");
    Instance* inst = new Instance(args->filename, args);
    DescentSolver* solver = new DescentSolver(inst);
    solver->solve();
    if (solver->found) {
        Solution::save(solver->get_solution(), args);
    } else {
        cout << "Solver : pas de solution "  << endl;
    }
    delete solver;
    delete inst;
    logn1("descent_solver_action END");
}

void annealing_solver_action(Options* args) {
    logn1("annealing_solver_action END");
    Instance* inst = new Instance(args->filename, args);
    AnnealingSolver* solver = new AnnealingSolver(inst);

    // Largeur du palier à température constante
    // step_size = 10;

    // // température initiale (-1 pour automatique)
    // temp_init = -1.0;

    // // taux d'acceptation cible pour un calcul auto de temp_init
    // temp_init_rate = 0.8;

    // // tempétature minimale (critère d'arêt)
    // temp_mini = 1.0e-6;

    // // coeff de décroissance géométrique
    // temp_coef = 0.999950;

    // // nb maxi de refus consécutif de mouvement (améliorant ou pas)
    // nb_cons_reject_max = 1e9; // infini

    // // nb maxi de tests consécutifs non améliorants
    // nb_cons_no_improv_max = 1000;

    solver->solve();
    if (solver->found) {
        Solution::save(solver->get_solution(), args);
    } else {
        cout << "Solver : pas de solution "  << endl;
    }
    delete solver;
    delete inst;
    logn1("annealing_solver_action END");
}

void taboo_solver_action(Options* args) {
    logn1("taboo_solver_action BEGIN");
    Instance* inst = new Instance(args->filename, args);
    TabooSolver* solver = new TabooSolver(inst);
    solver->solve();
    if (solver->found) {
        Solution::save(solver->get_solution(), args);
    } else {
        cout << "Solver : pas de solution "  << endl;
    }
    delete solver;
    delete inst;
    logn1("taboo_solver_action END");
}

// void test_action(Options* args) {
//     Instance* inst = new Instance(args->filename, args);
// }

// TODO : Remplacer classe Solver par un interface purement abstraite
//        et réorganiser ce fichier
//
int main(int argc, char *argv[]) {

    // Analyse et extraction des arguments de la ligne de commande
    // La variable statique Options::args sera disponible dans toutes les classes
    // Options* args = new Options(argc, argv);
    // Options::args = args;
    Options::args = new Options(argc, argv);
    Options* args =  Options::args; // une variable locale

    // Exemple d'affichage de quelques options
    cout << "args->action=" << args->action << endl;
    cout << "args->seed=" << args->seed << endl;
    cout << "args->filename=" << args->filename << endl;
    cout << "args->outfilename=" << args->outfilename << endl;
    // exit(0);

    // Démarrage d'un chronomètre
    time_t start_time;
    time(&start_time);

    // Dans la suite, le comportement de la résolution dépend du choix du
    // solveur passé en paramètre.
    if (args->action == "test") {;
        test_action(args);
    } else if (args->action == "instance") {
        instance_action(args);
    } else if (args->action == "stupid") {
        stupid_solver_action(args);
    } else if (args->action == "carlo") {;
        carlo_solver_action(args);
    } else if (args->action == "steepest2opt") {;
        steepest2opt_solver_action(args);
    } else if (args->action == "descent") {;
        descent_solver_action(args);
    } else if (args->action == "annealing") {;
        annealing_solver_action(args);
    } else if (args->action == "taboo") {;
        taboo_solver_action(args);
    } else {
        cerr << "Action non reconnue : " << args->action << "\n";
        exit(1);
    }

    // Arrêt et exploitation du chronomètre
    time_t end_time;
    time(&end_time);
    double diff = difftime(end_time, start_time);
    printf("Temps de calcul:\t %.1fs\n", diff);
    // cout << "Temps de calcul:\t" << diff << "s" << endl << endl;

    delete args;
    return 0;
}
//./
