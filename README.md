# Velib

Lancement du Steepest2Opt:
Aucune option n'est � ajouter. La ligne suivante suffit :
./run --steepest2opt ../data/v0.dat


Lancement du DescentSolver :
Le DescentSolver prend en argument s'il doit consid�rer uniquement les solutions
am�liorantes ou toutes les solutions. Ci-dessous, deux exemples repr�sentant les deux
cas de figure.
Acceptation syst�matique : 
./run --descent ../data/v0.dat --explore all
Acceptation am�liorante uniquement :
./run --descent ../data/v0.dat --explore strict


Lancement du recuit :
Le recuit prend deux arguments suppl�mentaires qui sont la temp�rature initiale
et le nombre d'it�rations par palier. La ligne ci-dessous lance un recuit de
temp�rature initiale 5 millions et effectuant jusqu'a 40000 it�rations par palier.
./run --annealing ../data/v0.dat --temperature 5000000 --itermax 40000