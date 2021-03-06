#ifndef OPTIONS_H
#define OPTIONS_H

using namespace std;
#include "optionparser.hpp"
#include "logger.hpp"

class Options  {
    public:

    // Cet attribut statique servira de "variable globale" pour faciliter l'accès
    // aux options de l'application par ses différentes classes.
    static Options* args;

    OptionParser* parser;

    string action;
    int itermax;
    double temp_init;

    string station_inserter;
    string station_chooser;
    string remorque_chooser;

    string explore;

    // long seed;
    int seed;
    bool force;

    string filename;
    string outfilename;

    bool print_exemple;


    // Liste des paramètres simples (par exemple la listes des fichiers à traiter).
    // Elle est déjà accessible dans parser, mais on peut souhaiter supprimer
    // l'objet parser après l'analyse (si on veut économiser la RAM).
    vector<string> params;

    Options(int argc, char *argv[]) {

        parser = new OptionParser(argc, argv);

        this->init_options();

        // Personnalisation éventuelle du comportement : que faire si l'utilisateur
        // passe une option incorrecte ?  Les valeurs possibles sont :
        //      exit (par défaut), raise, warn, ignore
        // Ici, on veut afficher les clés erronées tout en continuant l'analyse.
        // parser->on_error("warn");
        // Ici, on veut arrêter le programme en cas d'option invalide.
        parser->on_error("exit");  // inutile car c'est "exit" par défaut

        // Permet des post-traitements (vérification de cohérence d'options,...)
        pre_process_options();

        // On peut lancer l'analyse ici ou bien le faire depuis le main() de la
        // du programme principal.
        parser->parse();

        // Permet des post-traitements (vérification de cohérence entre options,...)
        post_process_options();

        // Les attributs options étant affectés, on peut supprimer le parser
        // sauf si on souhaite exploiter quelques-unes de ses méthodes ou attributs
        // e.g parser->print_values() ou parser->params
        //  delete parser;
        //  parser = NULL; // Utile ssi on veut pouvoir tester l'existence de parser !
    }
    ~Options() {
    }

    string get_exemples() {
        stringstream buf;
        buf <<
        "Exemples : \n"
        "./run -a test\n"
        "./run --test\n"
        "   test de contruction de l'instance mini et de son affichage\n"
        ""
        "Exemples : \n"
        "./run -a instance ../data/v0.dat\n"
        "   lit et affiche une instance\n"
        ""
        "Exemples : \n"
        "./run  --action stupid ../data/v0.dat\n"
        "./run  -a stupid       ../data/v0.dat\n"
        "./run  --stupid        ../data/v0.dat\n"
        "./run  -S              ../data/v0.dat\n"
        "   lit et résoud une instance avec le solver StupidSolver\n"
        "   avec affichage standard d'informations (par -L 2)\n"
        "   la solution est affiché sur la sortie standard et enregistrée\n"
        "   dans un fichier sous la forme vN-D-LLL.sol (e.g. v0-4-406.sol)\n"
        ""
        "Exemples : \n"
        "./run  -S ../data/v0.dat -L 5\n"
        "   même chose mais affiche plus d'informations (par -L 5)\n"
        ""
        "Exemples : \n"
        "./run  --action carlo ../data/v0.dat \n"
        "./run  --carlo ../data/v0.dat \n"
        "./run  -C ../data/v0.dat \n"
        "   Méthod de Monte Carlo appliquée à l'instance v0\n"
        ""
        "Exemples : \n"
        "./run  --carlo ../data/v0.dat --itermax 1000\n"
        "./run  -C      ../data/v0.dat -N 1000\n"
        "   Méthod de Monte Carlo avec 1000 itérations\n"
        ""
        "Exemples : \n"
        "./run  --descent ../data/v0.dat -N 1000\n"
        "./run  -D ../data/v0.dat -N 1000\n"
        "   mode de descente pure avec 1000 itérations\n"
        ""
        "Exemples : \n"
        "./run  --descent -E ../data/v0.dat -N 1000\n"
        "   mode exploration (car -E option de l'action descente)\n"
        ""
        "Exemples : \n"
        "./run  --annealing ../data/v0.dat \n"
        "./run  --recuit    ../data/v0.dat\n"
        "./run  -R          ../data/v0.dat \n"
        "   Méthode de recuit simulé **À FAIRE**\n"
        ""
        ;
        return buf.str();
    }
    void pre_process_options() {
        // On peut ici faire un prétraitemenet des option, par exemple pour
        // extraire un premier paramètre (e.g une clé pour pour cvs ou git)

        // Ici on impose de passer au moins un paramètre (e.g nom du fichier à
        // traiter, ...
        if (parser->argc <= 1) {
            cout << parser->abstract;
            exit(1);
        }
    }

    void post_process_options() {
        // On peut faire des tests plus approfondis sur les options, par exemple
        // vérifier que la couleurs est seulement une de celles autorisée, ...
        // if ( !(this->color=="blue" || this->color=="white" || this->color=="red" ) )  {
        //     cerr << "Erreur valeur de color incorrecte : " << this->color << endl;
        //     cerr << parser->get("--color")->get_help();
        //     exit(1);
        // }
        if (this->print_exemple) {
            cout << this->get_exemples();
            exit(0);
        }

        // Validation des types énumérés, ...
        if ( !(this->station_inserter=="NONE" ||
               this->station_inserter=="FRONT" ||
               this->station_inserter=="BACK" ||
               this->station_inserter=="RAND" ||
               this->station_inserter=="BEST" )) {
            cerr << "Erreur valeur de station_inserter incorrecte : "
                 << this->station_inserter << endl;
            cerr << parser->get("--station-inserter")->get_help();
            exit(1);
        }
        if ( !(this->station_chooser=="INST" ||
               this->station_chooser=="RAND" )) {
            cerr << "Erreur valeur de station_chooser incorrecte : "
                 << this->station_chooser << endl;
            cerr << parser->get("--station-chooser")->get_help();
            exit(1);
        }
        if ( !(this->remorque_chooser=="ALT" ||
               this->remorque_chooser=="RAND" )) {
            cerr << "Erreur valeur de remorque_chooser incorrecte : "
                 << this->remorque_chooser << endl;
            cerr << parser->get("--remorque-chooser")->get_help();
            exit(1);
        }

        // Lecture des paramètres supplémentaires (sans clé associée)
        // Paramètre 1 : fichier d'entrée
        //  this->filename = "";
        //  parser->add_string_option("--infile", this->filename)
        //        ->set_desc("Fichier d'instance (à traité ou à généré).")
        //        ->add_alias("-i");

        this->filename = "";
        if (parser->params.size() >= 1) {
            this->filename = parser->params[0];
        }

        // Lecture des paramètres supplémentaires (sans clé associée)
        // Paramètre 2 : fichier de sortie
        //  this->outfilename = "_AUTO_";
        //  buf.str(""); // pour raz buf
        //  buf << "Fichier de la solution à enregistrer.\n"
        //      << "    - défaut : \"<infile>.sol\"\n"
        //      << "    - passer \"\" pour éviter la création d'un fichier de sortie.";
        //  parser->add_string_option("--outfile", this->outfilename)
        //        ->set_desc(buf.str())
        //        ->add_alias("-o");
        this->outfilename = "_AUTO_";
        if (parser->params.size() >= 2) {
            this->outfilename = parser->params[1];
        }

        // Traitement des arguments résiduels, et vérif. de la cohérence de
        // certaines options reçues

        if (this->filename == "" && this->action != "test") {
            cout << "\n\nManque le fichier d'entrée.\n\n";;
            exit(1);
        }

        // On pourrait vérifier ici l'existance du fichier
        if (Log::level >= 2) {
            cout << "\nNouvelles valeurs des options :\n";
            parser->print_values(cout);
        }

        // traitemenet du générateur aléatoire
        //
        if (this->seed == 0) {
            this->seed = time(0);  // TODO : TESTER NECESSITE => OUI CA L'EST !!
        }
        cout << "seed=" << this->seed << endl;
        srand(this->seed);

    }

    // Retourne l'aide de l'application.
    // On se contente ici d'appeler l'aide automatique des options déclarées
    ostream& print_help(ostream& os = cout) {
        return parser->print_syntaxe();
    }

    void init_options() {

        //--------------
        stringstream buf;
        buf << "Syntaxe :\n";
        buf << "  "<< parser->exename << " [options...] [--] [fichier_instance [fichier_solution]]\n";
        buf << "  "<< parser->exename << " -h pour en savoir plus ; -H pour exemples\n";
        parser->abstract = buf.str();

        //--------------
        parser->add_doc("\nOptions principales de résolution\n");


        //--------------
        this->action = "carlo";
        buf.str("");
        buf << "   Choix de l'action (ou d'une méthode de résolution).\n"
            << "       -a test    : n'appelle aucun solver (mode test)\n"
            << "       -a instance: lit, construit et ré-affiche un fichier d'instance\n"
            << "       -a stupid  : heuristique stupide pour tester Solution (StupidSolver)\n"
            << "       -a carlo   : méthode de Monte-Carlo (CarloSolver)\n"
            // << "       -a greedy  : glouton (GreedySolver)\n"
            << "       -a steepest2opt : méthode de meilleure descente (Steepest2optSolver)*"
            << "       -a descent : méthode de descente (DescentSolver)\n"
            << "       -a annealing : recuit simulé (AnnealingSolver)*\n"
            // << "       -a taboo   : méthode tabou (TabooSolver)\n"
            ;
        parser->add_string_option("--action", this->action)
              ->set_desc(buf.str())
              ->add_alias("-a")
              ->add_abbrev("--test",  "test")
              ->add_abbrev("--stupid",  "stupid")      ->add_abbrev("-S", "stupid")
              ->add_abbrev("--carlo",   "carlo")       ->add_abbrev("-C", "carlo")
              // ->add_abbrev("--greedy", "greedy")      ->add_abbrev("-G", "greedy")
              ->add_abbrev("--steepest2opt", "steepest2opt")  ->add_abbrev("-P", "steepest2opt")
              ->add_abbrev("--descent", "descent")     ->add_abbrev("-D", "descent")
              ->add_abbrev("--annealing", "annealing") ->add_abbrev("-A", "annealing")
              ->add_abbrev("--recuit",    "annealing") ->add_abbrev("-R", "annealing")
              // ->add_abbrev("--taboo",   "taboo")       ->add_abbrev("-T", "taboo")
              ;

        //--------------
        this->itermax = 100;
        parser->add_int_option("--itermax", this->itermax)
              ->set_desc("Nombre maxi d'itérations (ou autre selon le solveur).")
              ->add_alias("-N");

        this->temp_init = 2000.0;
        parser->add_double_option("--temperature", this->temp_init)
              ->set_desc("Temperature initiale pour le recuit simulé")
              ->add_alias("-T");

        //--------------
        parser->add_doc("\nOptions liées aux actions glouton ou carlo\n");

        this->station_inserter = "RAND";
        parser->add_string_option("--station-inserter", this->station_inserter)
              ->set_desc("Mode d'insertion d'une station dans un circuit"
                         "(NONE|FRONT|BACK|BEST|RAND)")
              ->add_alias("--sinserter");

        this->station_chooser = "RAND";
        parser->add_string_option("--station-chooser", this->station_chooser)
              ->set_desc("Mode de choix de la station (INST|RAND)")
              ->add_alias("--schooser");

        this->remorque_chooser = "RAND";
        parser->add_string_option("--remorque-chooser", this->remorque_chooser)
              ->set_desc("Mode de choix de la remorque (ALT|RAND)")
              ->add_alias("--rchooser");


        //--------------
        parser->add_doc("\nOptions liées à l'action descent\n");

        //--------------
        this->explore = "";
        parser->add_string_option("--explore", this->explore)
              ->set_desc("mode exploration : acceptation systématique du voisin.\n")
              ->add_alias("-E");


        //--------------
        parser->add_doc("\nOptions communes indépendantes de l'application\n");

        //--------------
        Log::level = 2;
        parser->add_int_option("--level", Log::level)
              ->set_desc("Niveau de verbosité (0: silence, 9: tres verbeux).")
              ->add_alias("-L")
              ->add_alias("--verbose-level")
              ->add_abbrev("--debug", 5)
              ->add_abbrev("-d", 5);

        //--------------
        this->print_exemple = false;
        parser->add_switch_option("--exemples", this->print_exemple)
              ->set_desc("Affiche quelques exemples d'utilisation).")
              ->add_abbrev("-H", 1);


        //--------------
        this->seed = 0;
        parser->add_int_option("--seed", this->seed)
              ->set_desc("Graine pour le générateur aléatoire.");


        //--------------
        this->force = false;
        parser->add_switch_option("--force", this->force)
              ->set_desc("Autorise l'écrasement d'un fichier existant.\n"
                         "    (e.g. fichier de sortie, ...)");

        parser->set_params_vector(this->params);
    };
};
// L'attribut statique args ne peut pas défini ici, dans le même fichier que sa
// déclaration. Cette décaration sera donc faite dans le fichier main.cpp par :
//    Options* Options::args = NULL;

#endif
