#include <mpd/client.h>
#include <stdio.h>

struct mpd_connection *mpd_connect() {
    struct mpd_connection *conn;
    conn = mpd_connection_new("raspberrypi.local", 6600, 30000);

    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
        perror("ERROR: cannot connect mpd!");
        return NULL;
    }

    return conn;
}
