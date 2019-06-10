double m2s_buf[256];
short m2s_lft;
short m2s_rgt;
double m2s_lft_mul;
double m2s_rgt_mul;

short clip2short(double in) {
	double sam;
	short out;

	sam = in;
	if (sam < -1.0) sam = -1.0;
	if (sam > 1.0) sam = 1.0;

	out = (short)(sam * 32767.0);
	return out;
}

void m2s(double in) {
	double sam;
	double lft;
	double rgt;
	int i;

	sam = in;
	m2s_buf[250] = sam;
	memcpy(m2s_buf, &m2s_buf[1], sizeof(double) * 251);

	lft = 0.0;
	for (i = 30; i < 35; i++) lft += m2s_buf[i];
	lft /= 6.0;
	lft = sam - lft;

	rgt = 0.0;
	for (i = 130; i < 135; i++) rgt += m2s_buf[i];
	rgt /= 6.0;
	rgt = sam - rgt;

	double v;
	double m;

	v = lft;
	m = 1.0 / m2s_lft_mul;
	if (v < 0.0) v = -v;
	if (v > m) m = v;
	if (m > 1.0) m *= 0.99999875;
	m2s_lft_mul = 1.0 / m;
	lft *= m2s_lft_mul;

	v = rgt;
	m = 1.0 / m2s_rgt_mul;
	if (v < 0.0) v = -v;
	if (v > m) m = v;
	if (m > 1.0) m *= 0.99999875;
	m2s_rgt_mul = 1.0 / m;
	rgt *= m2s_rgt_mul;

	m2s_lft = clip2short(lft);
	m2s_rgt = clip2short(rgt);
}
