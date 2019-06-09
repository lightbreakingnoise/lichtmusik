typedef struct {
	double phase;
	double inc;
	double lfo_phase;
	double lfo_inc;
	double lo;
	int c;
} ISYNTH, *PSYNTH;

PSYNTH synth_create(int tune, double lfo_hz) {
	PYSNTH syn;
	syn = (PSYNTH) malloc(sizeof(ISYNTH));

	double f;
	f = 27.5;

	int i;
	for (i = -3; i < tune; i++)
		f /= HN;

	syn->phase = 0.0;
	syn->inc = (2.0 / SR) * f;
	syn->lfo_phase = 0.0;
	syn->lfo_inc = (DP / SR) * lfo_hz;
	syn->lo = 0.0;
	syn->c = 0;

	return syn;
}

double synth_walk(PSYNTH syn) {
	double out;

	out = syn->phase - 1.0;

	syn->phase += syn->inc + (0.01 * sin(syn->lfo_phase) * syn->inc);
	if (syn->phase > 1.0)
		syn->phase -= 2.0;

	syn->lfo_phase += syn->lfo_inc;
	if (syn->lfo_phase > DP)
		syn->lfo_phase -= DP;

	double q;
	q = 0.01;

	if (syn->c < 8)
		q = 0.1;

	syn->lo -= (syn->lo - out) * q;
	out = syn->lo;

	syn->c = (syn->c + 1) % 12;

	return out;
}
