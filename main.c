#include <stdio.h>
#include <inttypes.h>
#include <conio.h>
#include <stdbool.h>

void doJob() {
	uint32_t a32 = 0;
	uint32_t b32 = 0;

	scanf("%" SCNu32 "%" SCNu32, &a32, &b32);

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
	uint16_t b[2] = { 0 };
	uint16_t c[4] = { 0 };

	uint16_t pow16 = 65535;
	uint16_t pow15 = 32768;

	// cast to uint16

	a[0] = (uint16_t)(a32 / (pow16 + 1)); // >> 16
	a[1] = (uint16_t)(a32 - a[0] * (pow16 + 1)); // & 0000FFFF

	c[2] = (uint16_t)(b32 / (pow16 + 1)); // >> 16
	c[3] = (uint16_t)(b32 - c[2] * (pow16 + 1)); // & 0000FFFF

	bool remainder = false;
	for (int i = 0; i < 32; i++) {
		if (c[3] % 2 != 0) { // if (x & 000F == 0001)
			if (pow16 - c[0] >= a[0]) {
				c[0] += a[0];
			}
			else { // overflow make addition, then add 1 to the next digit (c[0])
				c[0] = a[0] - (pow16 - c[0] + 1); // (x + y) & FFFF
				remainder = true;
			}
			if (pow16 - c[1] >= a[1]) c[1] += a[1];
			else { // overflow make addition, then add 1 to the next digit (c[0])
				c[1] = a[1] - (pow16 - c[1] + 1); // (x + y) & FFFF
				c[0]++;
			}
		}

		bool odd = false;
		bool lastOdd = false;
		for (int j = 0; j < 4; j++) {
			if (c[j] % 2 != 0) odd = true; // if 0001
			else odd = false;
			c[j] /= 2; // c[j] >> 1

			if (lastOdd) c[j] = c[j] + pow15; // | F000
			lastOdd = odd;
		}

		if (remainder) {
			c[0] += pow15;
			remainder = false;
		}
	}

	printf("%" PRIu64, c[0] * 281474976710656 + c[1] * 4294967296 + c[2] * 65536 + c[3]);
}

int main()
{
	doJob();

	_getch();
	return 0;
}
