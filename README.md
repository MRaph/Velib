# Velib

This project implements various meta-heuristics for an optimization problem.

## Problem

The problem tackles a bike sharing repartition. You want to equilibrate the repartition of bikes in different stations of a city.
As input data, you have the number `M` of trucks with their bike capacity, and the number `N` of stations with their current count of bikes and their ideal number of bikes.  

## Circuit

Each station belongs to only one circuit, and a circuit is assigned to a truck. A circuit begins and ends at the same point. We note the desequilibrium of a circuit as the sum of the differences between the ideal number and the effective number of bikes in each station.
We can modify the association circuit-station, and modify the order in which the stations are explored, as long as every station belongs to one circuit.


## Loss function

The loss function is computed based on the desequilibrium of each circuit at the end of the tour, and the distance of each truck.  
The expression is `loss = 1e6*desequilibria + distances`.


# Run

First algorithm implemented is the steepest descent. It can be launched with:
```cpp
./run --steepest2opt ../data/v0.dat
```


Second algorithm is a descent solver, which can explore only improving solutions, or every solution based on argument. It can be launched with:
```cpp
./run --descent ../data/v0.dat --explore all
./run --descent ../data/v0.dat --explore strict
```

Third algorithm is the annealing solver, which takes as argument the initial temperature, and the number of iterations at each step. It can be launched with:
```cpp
./run --annealing ../data/v0.dat --temperature 5000000 --itermax 40000
```
