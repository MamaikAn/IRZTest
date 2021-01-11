#include <stdio.h>
#include <inttypes.h>
#include <conio.h>
#include <stdbool.h>

void doJob() {
	uint32_t a32 = 0;
	uint32_t b32 = 0;

	scanf_s("%" SCNu32 "%" SCNu32, &a32, &b32);

	// if a || b is zero or one

	if (a32 == 0 || b32 == 0) {
		printf("0");
		return;
	}
	else if (a32 == 1) {
		printf("%" PRIu32, b32);
		return;
	}
	else if (b32 == 1) {
		printf("%" PRIu32, a32);
		return;
	}

	uint16_t a[2] = { 0 };
	uint16_t c[4] = { 0 };

	uint16_t pow15 = 32768; // 1000
	uint16_t pow16 = UINT16_MAX;

	// cast to uint16

	a[0] = (uint16_t)(a32 / (UINT16_MAX + 1)); // >> 16 (left register)
	a[1] = (uint16_t)(a32 - a[0] * (UINT16_MAX + 1)); // & 0000FFFF (right register)

	c[2] = (uint16_t)(b32 / (UINT16_MAX + 1)); // >> 16 (left register)
	c[3] = (uint16_t)(b32 - c[2] * (UINT16_MAX + 1)); // & 0000FFFF (right register)

	bool remainder = false; // if true => c[0] overflowed
	for (int i = 0; i < 32; i++) { // binary multiplier

		// first we do addition, if last digit of 'b' is 1 (or if last register contains odd number).
		if (c[3] % 2 != 0) { // check if (x & 000F == 0001) (odd number)
			if (UINT16_MAX - c[0] >= a[0]) { // check if we can do addition without overflow (left register)
				c[0] += a[0];
			}
			else { // overflow, so we need to make an addition (like without overflow) and set remainder to true
				c[0] = a[0] - (UINT16_MAX - c[0] + 1); // (x + y) & FFFF
				remainder = true;
			}
			if (UINT16_MAX - c[1] >= a[1]) c[1] += a[1]; //  check if we can do addition without overflow (right register)
			else { // overflow, so we need to make an addition (like without overflow) and add 1 to the left digit
				c[1] = a[1] - (UINT16_MAX - c[1] + 1); // (x + y) & FFFF
				c[0]++;
			}
		}

		// next we do offset

		bool odd = false; // if true => current digits is odd
		bool lastOdd = false; // if true => previous degits is odd
		for (int j = 0; j < 4; j++) {
			if (c[j] % 2 != 0) odd = true; // if 0001 (odd)
			else odd = false;
			c[j] /= 2; // c[j] >> 1 (offset)

			if (lastOdd) c[j] = c[j] + pow15; // | F000 (if previous digits is odd, set big endian to 1
			lastOdd = odd;
		}

		if (remainder) { // if c[0] was overflowed (before offset), we set c[0] big endian to 1
			c[0] += pow15;
			remainder = false;
		}
	}

	printf("%" PRIu64, c[0] * 281474976710656 + c[1] * 4294967296 + (uint64_t) c[2] * 65536 + c[3]);
}

int main()
{
	doJob();

	_getch();
	return 0;
}
