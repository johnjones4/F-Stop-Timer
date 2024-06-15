#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../lib/timescale/timescale.h"

void assert_double(double a, double b, const char* message) {
  if (abs(a - b) > 0.0001) {
    printf("Assertion failure: %f != %f (%s)\n", a, b, message);
    abort();
  }
}

#define N_BASE_TIMES 1
#define N_DELTAS 3
#define N_VARIANTS 7
#define VAIRANT_OFFSET -3

void test_generateTime() {
  double test_base_times[N_BASE_TIMES] = {10};
  double test_deltas[N_DELTAS] = {0.25,1.0/3.0,0.5};
  double expected_variants[N_BASE_TIMES][N_DELTAS][N_VARIANTS] = {
    {
      {5.946036,7.071068,8.408964,10,11.892071,14.142137,16.817928},
      {5.000000347,6.299605541,7.937005262,10,12.59921021,15.87400979,19.99999861},
      {3.535533906,5,7.071067812,10,14.14213562,20,28.28427125},
    }
  };

  for (int i = 0; i < N_BASE_TIMES; i++) {
    for (int j = 0; j < N_DELTAS; j++) {
      for (int k = 0; k < N_VARIANTS; k++) {
        double delta = (double)(k + VAIRANT_OFFSET);
        printf("Testing %fs + %f stops\n", test_base_times[i], test_deltas[j] * delta);
        double time = generateTime(test_base_times[i], test_deltas[j], delta);
        assert_double(time, expected_variants[i][j][k], "generateTime");
      }
    }
  }
}

int main() {
  test_generateTime();
  return 0;
}