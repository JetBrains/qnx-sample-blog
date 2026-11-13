#ifndef ARGSTATS_H_
#define ARGSTATS_H_

#include <stddef.h>

/* Number of arguments we keep an individual length for. Any beyond this
 * still contribute to the totals, they just do not get their own entry. */
#define ARGSTATS_MAX 4

typedef struct {
    size_t      lengths[ARGSTATS_MAX];
    const char *longest;
    size_t      total;
    int         count;
} argstats_t;

/* Summarise argv into stats. */
void argstats_collect(argstats_t *stats, int argc, char *argv[]);

/* Print a summary previously filled in by argstats_collect(). */
void argstats_report(const argstats_t *stats);

#endif /* ARGSTATS_H_ */
