typedef struct {
	double phase[5];
	double inc;
	int numph;
	double vol;
	double lfo_phase;
	double lfo_inc;
	double lo;
	int c;
} ISYNTH, *PSYNTH;

PSYNTH synth_create(double lfo_hz, int n) {
	PSYNTH syn;
	syn = (PSYNTH) malloc(sizeof(ISYNTH));

	int i;
	for (i = 0; i < 5; i++)
		syn->phase[i] = 0.0;
	syn->inc = 0.0;
	syn->vol = 1.0;
	syn->lfo_phase = 0.0;
	syn->lfo_inc = (DP / SR) * lfo_hz;
	syn->lo = 0.0;
	syn->c = 0;
	syn->numph = n;

	return syn;
}

double synth_walk(PSYNTH syn) {
	double out;
	double inc;
	int i;
	int n;
	n = syn->numph;

	out = 0.0;
	inc = syn->inc;
	for (i = 0; i < n; i++) {
		out += (syn->phase[i] - 1.0);
		syn->phase[i] += inc + (0.01 * sin(syn->lfo_phase) * inc);
		inc *= 1.002;
		if (syn->phase[i] > 2.0)
			syn->phase[i] -= 2.0;
	}
	out /= 5.0;

	syn->lfo_phase += syn->lfo_inc;
	if (syn->lfo_phase > DP)
		syn->lfo_phase -= DP;

	double q;
	q = 0.05;

	if (syn->c < 24)
		q = 0.25;

	syn->lo -= (syn->lo - out) * q;
	out = syn->lo * syn->vol;

	syn->c = (syn->c + 1) % 32;

	return out;
}
