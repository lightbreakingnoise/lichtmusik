typedef struct {
	double lo[6];
	double hi[6];
	double vol;
	double atk;
} IHAT, *PHAT;

typedef struct {
	int pos;
	double phase;
	double inc;
	double decer;
	double mul;
	double vol;
} IBDRUM, *PBDRUM;

PHAT hihat_create() {
	PHAT hat;
	hat = (PHAT) malloc(sizeof(IHAT));

	int i;
	for (i = 0; i < 6; i++) {
		hat->lo[i] = 0.0;
		hat->hi[i] = 0.0;
	}
	hat->vol = 1.0;
	hat->atk = 0.2;

	return hat;
}

double hihat_walk(PHAT hat) {
	double out;

	out = (double)(rand()%1024) / 512.0;
	out -= 1.0;
	out *= hat->vol;

	int i;
	for (i = 0; i < 6; i++) {
		hat->lo[i] -= (hat->lo[i] - out) * (0.4 + hat->atk);
		out = hat->lo[i] * 1.5;
		hat->hi[i] += (out - hat->hi[i]) * (0.3 - hat->atk);
		out -= hat->hi[i];
	}
	hat->atk *= 0.998;

	return out;
}

PBDRUM bdrum_create() {
	PBDRUM bd;
	bd = (PBDRUM) malloc(sizeof(IBDRUM));

	bd->pos = 0;
	bd->phase = 0.0;
	bd->inc = (DP / SR) * 12000.0;
	bd->decer = 0.0;
	bd->mul = 0.7;
	bd->vol = 0.8;

	return bd;
}

void bdrum_set(PBDRUM bd) {
	bd->pos = 0;
	bd->inc = (DP / SR) * 12000.0;
	bd->mul = 0.7;
	bd->decer = (bd->inc * bd->mul) / 8.0;
	bd->vol = 0.8;
}

double bdrum_walk(PBDRUM bd, double in) {
	double out;
	out = in;

	if (bd->pos >= 28000)
		return out;

	double val;
	val = 1.0 - (bd->vol * 0.3);

	double sam;
	sam = 0.5 * bd->vol * sin(bd->phase);

	if (bd->pos > 24000)
		bd->vol *= 0.99885;
	else
		bd->vol *= 0.99997;

	bd->phase += bd->inc;
	if (bd->phase > DP)
		bd->phase -= DP;

	double wavlen;
	wavlen = DP / bd->inc;
	bd->decer = (bd->inc * bd->mul) / wavlen;
	if (bd->pos < 4000) {
		bd->mul -= (0.68 / 4000.0);
		bd->vol += (0.6 / 5000.0);
	}

	bd->inc -= bd->decer;
	bd->pos++;

	out *= val;
	out += sam;

	return out;
}
