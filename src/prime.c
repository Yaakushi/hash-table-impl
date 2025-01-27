#include "prime.h"
#include <math.h>

/*
 * Returns whether or not x is a prime.
 *
 * Return values:
 *   1 - prime
 *   0 - not prime
 *   -1 - undefined (x < 2)
 *
 */
int is_prime(const int x) {
  if (x < 2)
    return -1;
  if (x < 4)
    return 1;
  if (x % 2 == 0)
    return 0;

  for (int i = 3; i < floor(sqrt(x)); i += 2) {
    if (x % i == 0)
      return 0;
  }

  return 1;
}

// Returns next prime after x, or x if it's prime.
int next_prime(int x) {
  while (is_prime(x) != 1)
    x++;

  return x;
}
