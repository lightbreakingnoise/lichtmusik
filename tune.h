double sin_TABLE[100];
double saw_TABLE[100];

void tune_create() {
	int i;
	double f;
	f = 27.5;

	for (i = 0; i < 3; i++)
		f /= HN;

	for (i = 0; i < 100; i++) {
		sin_TABLE[i] = (DP / SR) * f;
		saw_TABLE[i] = (2.0 / SR) * f;
		f /= HN;
	}
}
