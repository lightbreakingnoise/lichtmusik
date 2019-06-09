typedef struct {
	int progress;
	int prog_old;
	char txt[50];
} ISTATUS, *PSTATUS;

PSTATUS sbar_create() {
	PSTATUS sbar;
	sbar = (PSTATUS) malloc(sizeof(ISTATUS));

	sbar->progress = 0;
	sbar->prog_old = 0;
	memset(sbar->txt, 0, 50);
	sbar->txt[0] = '\r';

	return sbar;
}

void sbar_show(PSTATUS sbar, int amount, int end) {
	int i;
	int x;

	sbar->progress = (amount * 48) / end;
	if (sbar->prog_old != sbar->progress) {
		x = sbar->progress;
		sbar->prog_old = x;
		for (i = 1; i < 50; i++) {
			if (x > i) sbar->txt[i] = '#';
			else sbar->txt[i] = '_';
		}
		fwrite(sbar->txt, 1, 48, stderr);
		fflush(stderr);
	}
}
