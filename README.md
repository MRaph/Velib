# Velib

Lancement du Steepest2Opt:
Aucune option n'est à ajouter. La ligne suivante suffit :
./run --steepest2opt ../data/v0.dat


Lancement du DescentSolver :
Le DescentSolver prend en argument s'il doit considérer uniquement les solutions
améliorantes ou toutes les solutions. Ci-dessous, deux exemples représentant les deux
cas de figure.
Acceptation systématique : 
./run --descent ../data/v0.dat --explore all
Acceptation améliorante uniquement :
./run --descent ../data/v0.dat --explore strict


Lancement du recuit :
Le recuit prend deux arguments supplémentaires qui sont la température initiale
et le nombre d'itérations par palier. La ligne ci-dessous lance un recuit de
température initiale 5 millions et effectuant jusqu'a 40000 itérations par palier.
./run --annealing ../data/v0.dat --temperature 5000000 --itermax 40000