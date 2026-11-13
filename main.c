#include <limits.h>
#include <process.h>
#include <stdio.h>
#include <unistd.h>

#include "argstats.h"

static void print_banner(void)
{
    puts("=============================================");
    puts(" QNX sample application");
    puts(" configure / build / run / debug from CLion");
    puts("=============================================");
}

static void print_executable(void)
{
    /* _cmdname() reports the full path the process was loaded from, which is
     * more useful than argv[0] since a caller is free to set that to anything.
     * Passing NULL returns a pointer to a string we must not modify. */
    const char *path = _cmdname(NULL);

    puts("\nExecutable:");
    printf("  %s\n", (path != NULL) ? path : "<unavailable>");
}

static void print_working_directory(void)
{
    char cwd[PATH_MAX];

    puts("\nWorking directory:");
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("  %s\n", cwd);
    } else {
        perror("  getcwd");
    }
}

static void print_arguments(int argc, char *argv[])
{
    printf("\nArguments (%d):\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d] = %s\n", i, argv[i]);
    }
}

#ifdef SHOW_ENV
static void print_environment(void)
{
    int count = 0;

    puts("\nEnvironment:");
    for (char **var = environ; (var != NULL) && (*var != NULL); var++) {
        printf("  %s\n", *var);
        count++;
    }
    printf("  (%d variable%s)\n", count, (count == 1) ? "" : "s");
}
#else
static void print_environment(void)
{
    puts("\nEnvironment:");
    puts("  (omitted - rebuild with SHOW_ENV=1 to include it)");
}
#endif

static void print_argument_statistics(int argc, char *argv[])
{
    argstats_t stats;

    argstats_collect(&stats, argc, argv);
    argstats_report(&stats);
}

int main(int argc, char *argv[])
{
    /* Line buffering so the output appears as it is produced even when stdout
     * is a pipe. */
    setvbuf(stdout, NULL, _IOLBF, 0);

    print_banner();
    print_executable();
    print_working_directory();
    print_arguments(argc, argv);
    print_argument_statistics(argc, argv);
    print_environment();

    return 0;
}
