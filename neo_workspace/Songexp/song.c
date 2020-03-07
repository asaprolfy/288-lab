#include "song.h"

void load_the_wake() {

    int song_number = 0;
    int num_notes = 6;
    unsigned char notes[] = {50, 62, 55, 62, 60, 50};
    unsigned char duration[] = {80, 80, 80, 64, 16, 80};

    oi_loadSong(song_number, num_notes, notes, duration);
}

void play_the_wake() {

    oi_play_song(0);
}
