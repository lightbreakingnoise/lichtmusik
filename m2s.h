double m2s_buf[256];
short m2s_lft;
short m2s_rgt;

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

	m2s_lft = clip2short(lft);
	m2s_rgt = clip2short(rgt);
}
