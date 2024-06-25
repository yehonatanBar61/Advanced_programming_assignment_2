./randomGenerator 10 10000000 > random_output.txt

valgrind --tool=massif --massif-out-file=massif.out.prime ./myPrimeCounter < random_output.txt

ms_print massif.out.prime