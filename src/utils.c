#include "utils.h"

uint32_t next_power_of_2(uint32_t n) {
	if (n == 0)
		return 1;
	// If already a power of 2, return n
	if ((n & (n - 1)) == 0)
		return n;

	// Else find the next power of 2
	uint32_t power = 1;
	while (power < n) {
		power <<= 1; // Same as power *= 2
	}
	return power;
}

uint32_t previous_power_of_2(uint32_t n) {
	if (n == 0)
		return 0;

	uint32_t power = 1;
	while (power << 1 <= n) {
		power <<= 1;
	}
	return power;
}
