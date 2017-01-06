#ifndef SOLVER_H
#define SOLVER_H

#include "common.hpp"
#include "instance.hpp"
#include "solution.hpp"

class Solver {
public:

    // nom court du solver e.g "S_1_glouton"
    string name;
    // description en une ligne
    // e.g "Par itération : un simplex + un mip + un seul sous tour"
    string desc;

    // pointeur sur l'instance du problème
    Instance* inst;

    // un boolean parmettent de connaitre l'état de la résolution en fin de calcul
    // TODO : différencier les états "optimum trouvé" de l'état "interrompu avant
    // optimum"
    // true : le solveur à terminé normalement sa fonction qui dépend de la
    //        méthode appelée, en général c'est que l'optimum a été trouvé
    // false : une interruption à été déclenchée avant la fin de la résolution
    // TODO TRANSFORMER EN ATTRIBUT state :
    // - found : solution trouvée fin normale du solver
    // - notfound : solution non trouvée (car interuption avant la fin, ...)
    // - optimum : solution optimale trouvée (version branch and bound ou PPC)
    //
    // enum State {
    //     INIT,
    //     RUNING,
    //     INTERRUPTED,
    //     FOUND,
    //     OPTIMUM
    // }
    // State state = INIT;
    bool found;

    Solution* solution;

    Solver(Instance* Instance);
    virtual ~Solver();

    // La principal méthode à redéfinir par les classes filles
    //
    virtual bool solve();
    // virtual State solve();

    virtual Solution* get_solution()  {return solution ; };

};

////////////////////////////////////////////////////////////////////////
// Déclaration des solveur spécialisés
//
class StupidSolver : public Solver {
public:
    StupidSolver(Instance* Instance);
    virtual ~StupidSolver();
    virtual bool solve();
    Solution* solution;
    virtual Solution* get_solution() {return this->solution;};
};

class CarloSolver : public Solver {
public:
    CarloSolver(Instance* Instance);
    virtual ~CarloSolver();

    string sinserter;
    string schooser;
    string rchooser;

    virtual bool solve();
    virtual Solution* apply_one_greedy(Solution* currentsol);
    virtual Solution* get_solution() {return this->solution;};
};

class DescentSolver : public Solver {
public:

    Solution* cursol = NULL;
    Solution* testsol = NULL;
    Solution* bestsol = NULL;

    DescentSolver(Instance* Instance);
    virtual ~DescentSolver();
    virtual bool solve();
    virtual bool solve_explore_everything();
    virtual bool solve_pure_descent();
    virtual void mutate(Solution* sol);

    virtual Solution* get_solution() {return this->bestsol;};
};


class Steepest2optSolver : public Solver {
public:

    Solution* cursol = NULL;
    Solution* testsol = NULL;
    Solution* bestsol = NULL;

    Steepest2optSolver(Instance* Instance);
    virtual ~Steepest2optSolver();
    virtual bool solve_stupid();
    virtual bool solve();
    Solution* Stupid_solver(Solution* sol);
    Solution* glouton_aleatoire(Solution* sol);
    virtual void mutate(Solution* sol);

    virtual Solution* get_solution() {return this->bestsol;};
};


class AnnealingSolver : public Solver {
public:

    Solution* cursol = NULL;
    Solution* testsol = NULL;
    Solution* bestsol = NULL;

    AnnealingSolver(Instance* Instance);
    virtual ~AnnealingSolver();
    virtual bool solve();

    void mutate(Solution* sol);
    void mutate_based_on_desequilibre(Solution* sol);
    std::vector<float> convert_desequilibre_to_probability_vector(Solution* sol);
    int select_circuit_according_to_probabilities(vector<float> probabilities);
    
    // void update_temp();
    // double guess_temp_init(Solution* sol,
    //                        double taux_cible=0.8,
    //                        int nb_degrad_max=1000);
    // string get_stats();
    // bool finished();


    virtual Solution* get_solution() {return this->bestsol;};
};

class TabooSolver : public Solver {
public:

    Solution* cursol  = NULL;
    Solution* testsol = NULL;
    Solution* bestsol = NULL;

    TabooSolver(Instance* Instance);
    virtual ~TabooSolver();
    virtual bool solve();
    virtual Solution* get_solution() {return this->bestsol;};
};

#endif
