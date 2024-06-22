#include <stdio.h>
#include <stdbool.h>
#include <time.h> 

time_t program_start, program_end;

// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    int num;
    int total_counter = 0;

    time(&program_start);

    // Read numbers from stdin until end of file
    while (scanf("%d", &num) != EOF) {   
        if (isPrime(num)) {
            total_counter++;
        }
    }

    time(&program_end);
    
    printf("%d total primes.\n", total_counter);

    double elapsed_time = difftime(program_end, program_start);
    printf("%.2f seconds: total program time\n", elapsed_time);

    return 0;
}