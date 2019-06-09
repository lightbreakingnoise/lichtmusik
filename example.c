#include "init.h"
#include "song.h"
#include "synth.h"
#include "tune.h"

int main() {
	tune_create();

	PSONG song = song_create(125.0);
	PSYNTH syn = synth_create(1.7);

	double sam;
	short pcm;
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
		}

		sam = synth_walk(syn);

		if (sam < -1.0) sam = -1.0;
		if (sam > 1.0) sam = 1.0;
		pcm = (short)(sam * 32767.0);
		fwrite(&pcm, 2, 1, stdout);
		fwrite(&pcm, 2, 1, stdout);
		fflush(stdout);
	}

	return 0;
}
