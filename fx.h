typedef struct {
	double lo;
	double hi;
	double q_lo;
	double q_hi;
} IBAND, *PBAND;

typedef struct {
	int hold;
	double mul;
	double maxi;
} ICOMP, *PCOMP;

typedef struct {
	PBAND low;
	PBAND mid;
	PBAND hih;
	PCOMP comp[4];
} ITRIFX, *PTRIFX;

typedef struct {
	double lo;
	double hi;
	double *buf;
	int pos;
	int len;
} IECHO, *PECHO;

PBAND band_create(double cutoff_lo, double cutoff_hi) {
	PBAND band;
	band = (PBAND) malloc(sizeof(IBAND));

	band->lo = 0.0;
	band->hi = 0.0;
	band->q_lo = (cutoff_lo / HSR);
	band->q_hi = (cutoff_hi / HSR);

	return band;
}

PCOMP comp_create() {
	PCOMP comp;
	comp = (PCOMP) malloc(sizeof(ICOMP));

	comp->hold = 0;
	comp->mul = 1.0;
	comp->maxi = 0.5;

	return comp;
}

double band_walk(PBAND band, double in) {
	double out;
	out = in;

	band->lo -= (band->lo - out) * band->q_lo;
	out = band->lo;

	band->hi += (out - band->hi) * band->q_hi;
	out -= band->hi;

	return out;
}

double comp_walk(PCOMP comp, double in) {
	double out;
	out = in;

	double v;
	v = out;
	if (v < 0.0) v = -v;
	if (v > comp->maxi) {
		comp->maxi += 0.00001;
		comp->hold = 480;
	}

	if (comp->hold <= 0) {
		if (v < comp->maxi && comp->maxi > 0.35) comp->maxi -= 0.00001;
	} else {
		comp->hold--;
	}

	v = 0.6 / comp->maxi;
	comp->mul = v;
	out *= v;

	return out;
}

PTRIFX trifx_create() {
	PTRIFX fx;
	fx = (PTRIFX) malloc(sizeof(ITRIFX));

	int i;
	for (i = 0; i < 4; i++)
		fx->comp[i] = comp_create();

	fx->low = band_create(400.0, 20.0);
	fx->mid = band_create(6000.0, 400.0);
	fx->hih = band_create(20000.0, 6000.0);

	return fx;
}

double trifx_walk(PTRIFX fx, double in) {
	double a, b, c;
	double out;

	a = comp_walk(fx->comp[0], band_walk(fx->low, in));
	b = comp_walk(fx->comp[1], band_walk(fx->mid, in));
	c = comp_walk(fx->comp[2], band_walk(fx->hih, in));

	out = comp_walk(fx->comp[3], (a+b+c) / 3.0);

	return out;
}

PECHO echo_create(int len) {
	PECHO echo;
	echo = (PECHO) malloc(sizeof(IECHO));
	echo->buf = (double*) malloc(sizeof(double) * len);

	echo->lo = 0.0;
	echo->hi = 0.0;
	memset(echo->buf, 0, sizeof(double) * len);
	echo->pos = 0;
	echo->len = len;

	return echo;
}

double echo_walk(PECHO echo, double in) {
	double out;
	double sam;
	double *buf;
	buf = echo->buf;
	int pos;
	pos = echo->pos;

	echo->pos = (echo->pos + 1) % echo->len;

	out = buf[pos];
	sam = out + (in * 0.7);

	echo->lo -= (echo->lo - sam) * 0.35;
	sam = echo->lo;
	echo->hi += (sam - echo->hi) * 0.15;
	sam -= echo->hi;
	buf[pos] = sam;

	out = in + buf[echo->pos];

	return out;
}
