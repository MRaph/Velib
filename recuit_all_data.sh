cd build/
echo "Temperature initiale : " . $1
echo "Nombre d'itérations pour chaque température : " . $2
echo "Start running all data files"
echo "File 0"
./run --descent ../data/v0.dat --temperature $1 --explore recuit --itermax $2 --level $3
echo "File 1"
./run --descent ../data/v1.dat --temperature $1 --explore recuit --itermax $2 --level $3
echo "File 2"
./run --descent ../data/v2.dat --temperature $1 --explore recuit --itermax $2 --level $3
echo "File 3"
./run --descent ../data/v3.dat --temperature $1 --explore recuit --itermax $2 --level $3
echo "File 4"
./run --descent ../data/v4.dat --temperature $1 --explore recuit --itermax $2 --level $3
echo "File 5"
./run --descent ../data/v5.dat --temperature $1 --explore recuit --itermax $2 --level $3
echo "File 6"
./run --descent ../data/v6.dat --temperature $1 --explore recuit --itermax $2 --level $3
echo "File 7"
./run --descent ../data/v7.dat --temperature $1 --explore recuit --itermax $2 --level $3
echo "File 8"
./run --descent ../data/v8.dat --temperature $1 --explore recuit --itermax $2 --level $3
echo "File 9"
./run --descent ../data/v9.dat --temperature $1 --explore recuit --itermax $2 --level $3
cd ..
