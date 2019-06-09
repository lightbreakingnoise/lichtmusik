typedef struct {
	double pos;
	double next;
	double step;
	int c;
} ISONG, *PSONG;

PSONG song_create(double BPM) {
	PSONG song;
	song = (PSONG) malloc(sizeof(ISONG));

	double step;
	step = (15.0 / BPM) * SR;
	song->pos = 0.0;
	song->next = step;
	song->step = step;
	song->c = 0;

	return song;
}

int song_walk(PSONG song) {
	song->pos += 1.0;

	if (song->pos >= song->next) {
		song->next += song->step;
		song->c++;
		return 1;
	}
	return 0;
}
