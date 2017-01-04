cd ..
cd build/
echo "=== Mini-guide ==="
echo "Variable 1 : Nom de l'algorithme"
echo "Variable 2 : Température initiale"
echo "Variable 3 : Nombre d'itérations à chaque palier"
echo "Variable 4 : Niveau de verbosité"
echo "------------------"
echo "Example : sh run_algorithm_on_all_data --annealing 6000 300 2"
echo "Temperature initiale : " . $2
echo "Nombre d'itérations pour chaque température : " . $3
echo "Start running all data files"
echo "File 0"
./run $1 ../data/v0.dat --temperature $2 --explore recuit --itermax $3 --level $4
echo "File 1"
./run $1 ../data/v1.dat --temperature $2 --explore recuit --itermax $3 --level $4
echo "File 2"
./run $1 ../data/v2.dat --temperature $2 --explore recuit --itermax $3 --level $4
echo "File 3"
./run $1 ../data/v3.dat --temperature $2 --explore recuit --itermax $3 --level $4
echo "File 4"
./run $1 ../data/v4.dat --temperature $2 --explore recuit --itermax $3 --level $4
echo "File 5"
./run $1 ../data/v5.dat --temperature $2 --explore recuit --itermax $3 --level $4
echo "File 6"
./run $1 ../data/v6.dat --temperature $2 --explore recuit --itermax $3 --level $4
echo "File 7"
./run $1 ../data/v7.dat --temperature $2 --explore recuit --itermax $3 --level $4
echo "File 8"
./run $1 ../data/v8.dat --temperature $2 --explore recuit --itermax $3 --level $4
echo "File 9"
./run $1 ../data/v9.dat --temperature $2 --explore recuit --itermax $3 --level $4
cd ..
