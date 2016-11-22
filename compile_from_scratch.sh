echo "Removing build directory"
rm -r build/
echo "Creating new build directory"
mkdir build/
echo "Moving to build/"
cd build/
echo "cmake .."
cmake .. > tmp.log
echo "make"
make
echo "Running project"
if [ $# -eq 0 -o $# -eq 1]
then
  echo "Missing algorithm name or data file path. Using stupid and v0.dat"
  ./run --stupid ../data/v0.dat
  echo "Selecting solution file"
  solution_file=$(ls *.sol -t | head -n1)
  echo "Testing solution"
  velib -d ../data/v0.dat -s $solution_file
else
  ./run $1 $2
  echo "Selecting solution file"
  solution_file=$(ls *.sol -t | head -n1)
  echo "Testing solution"
  velib -d $2 -s $solution_file
fi
rm tmp.log
