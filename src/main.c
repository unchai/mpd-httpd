#include "fiobject.h"
#include "subprojects/cJSON-1.7.15/cJSON.h"
#include <fio.h>
#include <fio_cli.h>
#include <http.h>

#include <cjson/cJSON.h>

#include <stdio.h>
#include <sys/queue.h>
#include <syslog.h>

#include "mpd.h"
#include <mpd/client.h>

//#define PROJECT_NAME "mpd-httpd"

TAILQ_HEAD(tailHead, entry) head;

static void on_http_request(http_s *h) {
    printf("%s\n", fiobj_obj2cstr(h->path).data);

    struct mpd_connection *conn;
    conn = mpd_connect();

    if (conn == NULL) {
        http_send_body(h, "error", 5);
        return;
    }

    mpd_send_current_song(conn);

    struct mpd_song *song;
    song = mpd_recv_song(conn);

    const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);

    cJSON *monitor = cJSON_CreateObject();
    cJSON_AddItemToObject(monitor, "song", cJSON_CreateString(title));

    char *res = cJSON_Print(monitor);
    printf("%s\n", res);

    /* set a response and send it (finnish vs. destroy). */
    http_send_body(h, res, strlen(res));

    mpd_song_free(song);
    mpd_connection_free(conn);
}

int main(int argc, char **argv) {
    if (http_listen("8080", "127.0.0.1", .on_request = on_http_request,
                    .max_body_size = fio_cli_get_i("-maxbd"),
                    .public_folder = fio_cli_get("-public"),
                    .log = fio_cli_get_bool("-log"),
                    .timeout = fio_cli_get_i("-keep-alive")) == -1) {
        perror("ERROR: facil.io couldn't initialize HTTP service (already "
               "running?)");
        exit(1);
    }

    fio_start(.threads = fio_cli_get_i("-t"), .workers = fio_cli_get_i("-w"));

    return 0;
}
