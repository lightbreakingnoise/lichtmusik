#include "init.h"
#include "song.h"
#include "synth.h"
#include "tune.h"
#include "m2s.h"

int main() {
	tune_create();

	PSTATUS stat = sbar_create();
	PSONG song = song_create(125.0);
	PSYNTH syn = synth_create(1.7);
	syn->inc = saw_TABLE[24];

	int c = 0;

	while (c < 1024) {
		if (song_walk(song) == 1) {
			c = song->c;

			if (c % 8 == 0) {
				syn->inc = saw_TABLE[24];
				syn->vol = 1.0;
			}
			if (c % 8 == 3) {
				syn->inc = saw_TABLE[21];
				syn->vol = 1.0;
			}
			if (c % 8 == 6) {
				syn->inc = saw_TABLE[19];
				syn->vol = 1.0;
			}

			sbar_show(stat, c, 1024);
		}

		m2s(synth_walk(syn));
		syn->vol *= 0.9998;

		fwrite(&m2s_lft, 2, 1, stdout);
		fwrite(&m2s_rgt 2, 1, stdout);
		fflush(stdout);
	}

	return 0;
}
