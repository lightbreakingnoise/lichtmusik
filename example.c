#include "init.h"
#include "song.h"
#include "synth.h"
#include "tune.h"
#include "m2s.h"
#include "statusbar.h"
#include "fx.h"

#define FIN 256

int main() {
	tune_create();

	PSTATUS stat = sbar_create();
	PSONG song = song_create(125.0);
	PTRIFX tri = trifx_create();
	PSYNTH syn = synth_create(1.7);
	syn->inc = saw_TABLE[24];

	double sam;
	double lo = 0.0;
	double q = 0.02;
	double qd = 0.3;
	int c = 0;

	while (c < FIN) {
		if (song_walk(song) == 1) {
			c = song->c;

			if (c % 8 == 0) {
				syn->inc = saw_TABLE[24];
				syn->vol = 1.0;
				qd = 0.3;
			}
			if (c % 8 == 1) qd = 0.02;

			if (c % 8 == 3) {
				syn->inc = saw_TABLE[21];
				syn->vol = 1.0;
				qd = 0.3;
			}
			if (c % 8 == 4) qd = 0.02;

			if (c % 8 == 6) {
				syn->inc = saw_TABLE[19];
				syn->vol = 1.0;
				qd = 0.3;
			}
			if (c % 8 == 7) qd = 0.02;

			sbar_show(stat, c, FIN);
		}

		sam = synth_walk(syn);
		sam = trifx_walk(tri, sam);

		lo -= (lo - sam) * q;
		sam = lo;

		m2s(sam);

		syn->vol *= 0.9998;
		q = ((511.0 * q) + qd) / 512.0;

		fwrite(&m2s_lft, 2, 1, stdout);
		fwrite(&m2s_rgt, 2, 1, stdout);
		fflush(stdout);
	}

	fwrite("\n", 1, 1, stderr);

	return 0;
}
