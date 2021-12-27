#include "crow/app.h"
#include "mpd.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <mpd/client.h>

struct free_delete {
    void operator()(void *x) { free(x); }
};
template <typename T> using unique_c_ptr = std::unique_ptr<T, free_delete>;

unique_c_ptr<char[]> get_song_title() {
    struct mpd_connection *conn;
    conn = mpd_connect();

    mpd_send_current_song(conn);

    struct mpd_song *song;
    song = mpd_recv_song(conn);

    const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
    printf("%s\n", title);
    unique_c_ptr<char[]> ret = unique_c_ptr<char[]>(strdup(title));

    mpd_song_free(song);
    mpd_connection_free(conn);

    return ret;
}

int main(int argc, char **argv) {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]() {
        auto title = get_song_title();
        return title.get();
    });

    app.port(8080).multithreaded().run();

    return 0;
}
