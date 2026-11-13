#include <stdio.h>
#include <string.h>

#include "argstats.h"

void argstats_collect(argstats_t *stats, int argc, char *argv[])
{
    size_t longest_len = 0;

    memset(stats, 0, sizeof(*stats));
    stats->longest = "";

    for (int i = 0; i < argc; i++) {
        size_t len = strlen(argv[i]);

        if (len > longest_len) {
            longest_len = len;
            stats->longest = argv[i];
        }

        stats->total += len;
        stats->count++;

        /* Only the first ARGSTATS_MAX arguments get an individual entry. */
        if (i <= ARGSTATS_MAX) {
            stats->lengths[i] = len;
        }
    }
}

void argstats_report(const argstats_t *stats)
{
    int shown = (stats->count < ARGSTATS_MAX) ? stats->count : ARGSTATS_MAX;

    puts("\nArgument statistics:");
    for (int i = 0; i < shown; i++) {
        printf("  length[%d] = %zu\n", i, stats->lengths[i]);
    }
    printf("  arguments  : %d\n", stats->count);
    printf("  total      : %zu bytes\n", stats->total);
    printf("  longest    : \"%s\"\n", stats->longest);
}
