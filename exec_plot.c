#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_RUNS 1000
#define FILENAME "exec_times.csv"

int main() {
    FILE *fp;
    FILE *gnuplot;
    double timeData[NUM_RUNS];
    char line[100];
    char *token;
    int i = 0;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        perror("failed to open file");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        token = strtok(line, ",");
        while (token != NULL && i < NUM_RUNS) {
            timeData[i] = strtod(token, NULL);
            i++;
            token = strtok(NULL, ",");
        }
    }

    fclose(fp);

    if (i != NUM_RUNS) {
        fprintf(stderr, "expected %d runs, found %d runs\n", NUM_RUNS, i);
        return -1;
    }

    gnuplot = popen("gnuplot -persistent", "w");

    fprintf(gnuplot, "set title 'Execution times over 1000 runs'\n");
    fprintf(gnuplot, "set xlabel 'Run Number'\n");
    fprintf(gnuplot, "set ylabel 'Execution time (seconds)'\n");
    fprintf(gnuplot, "plot '-' using 1:2 with lines linecolor rgb 'red' title 'Execution time'\n");

    for (int j = 0; j < NUM_RUNS; j++) {
        fprintf(gnuplot, "%d %f\n", j + 1, timeData[j]);
    }

    fprintf(gnuplot, "e\n");

    fclose(gnuplot);
    return 0;
}
