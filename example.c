#include "init.h"
#include "song.h"
#include "synth.h"
#include "tune.h"
#include "m2s.h"
#include "statusbar.h"
#include "fx.h"
#include "drums.h"

#define FIN 1280

int main() {
	tune_create();

	PSTATUS stat = sbar_create();
	PSONG song = song_create(125.0);
	PTRIFX tri = trifx_create();
	PSYNTH syn = synth_create(1.7, 3);
	syn->inc = saw_TABLE[24];
	PSYNTH bass = synth_create(8.0, 1);
	bass->inc = saw_TABLE[4];
	PSYNTH lead = synth_create(1.0, 5);
	lead->inc = saw_TABLE[48];
	PECHO echo = echo_create((int)(song->step * 3.0));
	PBDRUM bd = bdrum_create();
	PHAT hat = hihat_create();
	hat->vol = 0.0;

	double sam;
	double htsam;
	double lo = 0.0;
	double q = 0.02;
	double qd = 0.3;
	double vmul = 0.9998;
	int okt = 0;
	int c = 0;

	while (c < FIN) {
		if (song_walk(song) == 1) {
			c = song->c;

			if (c >= 256 && c % 64 == 0) okt = 12;
			if (c >= 256 && c % 64 == 32) okt = 0;

			if (c % 8 == 0) {
				syn->inc = saw_TABLE[24 + okt];
				syn->vol = 0.2;
				qd = 0.1;
				vmul = 0.9998;
			}
			if (c % 8 == 1) qd = 0.02;

			if (c % 8 == 3) {
				syn->inc = saw_TABLE[21 + okt];
				syn->vol = 0.2;
				qd = 0.2;
				vmul = 0.99999;
			}
			if (c % 8 == 5) qd = 0.02;

			if (c % 8 == 6) {
				syn->inc = saw_TABLE[19 + okt];
				syn->vol = 0.2;
				qd = 0.3;
				vmul = 0.9997;
			}
			if (c % 8 == 7) qd = 0.02;

			if (c % 8 == 0)
				bass->vol = 0.6;
			if (c % 8 == 2)
				bass->vol = 0.9;
			if (c % 8 == 4)
				bass->vol = 0.6;
			if (c % 8 == 7)
				bass->vol = 0.9;

			if (c % 64 == 0)
				bass->inc = saw_TABLE[4];
			if (c % 64 == 58)
				bass->inc = saw_TABLE[9];

			if (c % 32 < 8)
				lead->vol = 0.3;

			if (c % 3 == 0)
				lead->inc = saw_TABLE[48];
			if (c % 3 == 1)
				lead->inc = saw_TABLE[50];
			if (c % 3 == 2)
				lead->inc = saw_TABLE[53];

			if (c % 4 == 0)
				bdrum_set(bd);

			if (c % 4 == 2) {
				hat->atk = 0.2;
				hat->vol = 0.3;
			}
			if (c % 4 == 3)
				hat->vol = 0.0;

			sbar_show(stat, c, FIN);
		}

		sam = synth_walk(syn);

		if (c >= 192) sam += synth_walk(bass);
		if (c >= 384) sam += synth_walk(lead);
		if (c >= 512) htsam = hihat_walk(hat);
		else htsam = 0.0;

		lo -= (lo - sam) * q;
		sam = lo;

		sam = trifx_walk(tri, sam);
		sam += (htsam * 0.2);
		sam = echo_walk(echo, sam);
		sam += (htsam * 0.8);
		sam = bdrum_walk(bd, sam);

		m2s(sam);

		syn->vol *= vmul;
		q = ((511.0 * q) + qd) / 512.0;
		bass->vol *= 0.9999;
		lead->vol *= 0.9997;

		fwrite(&m2s_lft, 2, 1, stdout);
		fwrite(&m2s_rgt, 2, 1, stdout);
		fflush(stdout);
	}

	fwrite("\n", 1, 1, stderr);

	return 0;
}
