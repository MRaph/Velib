#include "common.hpp"

#include "instance.hpp"
#include "solver.hpp"

#include "options.hpp"
#include "test.hpp"
#include <time.h>

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

int main(int argc, char *argv[]) {
    Options::args = new Options(argc, argv);
    Options* args =  Options::args;

    // Exemple d'affichage de quelques options
    cout << "args->action=" << args->action << endl;
    cout << "args->seed=" << args->seed << endl;
    cout << "args->filename=" << args->filename << endl;
    cout << "args->outfilename=" << args->outfilename << endl;

    // Démarrage d'un chronomètre
    clock_t time_start, time_stop;
    time_start = clock();

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
    }

    // Arrêt et exploitation du chronomètre
    time_stop = clock();
    float diff = ((float)time_stop - (float)time_start);
    float seconds = diff/CLOCKS_PER_SEC;
    printf("Time of execution: %f\n", seconds);

    delete args;
    return 0;
}
//./
