tabu_search
===========

Compile

```
    $ make cpp 
```

To run the script supply one of the villian teams without the budget restriction:

```
    $ ./tabu data/instances/V2_763.txt 0
```

To run the script supply one of the villian teams with the budget restriction:

```
    $ ./tabu data/instances/V2_763.txt 1
```

To run for all the instances with and without budget (results will be written on our_results.txt):
```
   $ ./benchmark
