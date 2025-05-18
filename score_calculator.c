#include "customs.h"

#define CHUNK_SIZE 10
#define BUFFER_SIZE 256

typedef struct {
    char user[20];
    int score;
} UserScore;

void write_line(const char *line) {
    write(STDOUT_FILENO, line, strlen(line));
}

int main(int argc, char **argv) {
    if (argc != 2) {
        const char *usage = "Usage: score_calculator <hunt_id>\n";
        write(STDERR_FILENO, usage, strlen(usage));
        return EXIT_FAILURE;
    }

    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s.dat", argv[1], argv[1]);

    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        const char *err = "Failed to open .dat file\n";
        write(STDERR_FILENO, err, strlen(err));
        return EXIT_FAILURE;
    }

    UserScore *scores = NULL;
    int capacity = 0;
    int count = 0;

    Treasure_t t;
    ssize_t n;
    while ((n = read(fd, &t, sizeof(Treasure_t))) > 0) {
        if (n != sizeof(Treasure_t)) {
            const char *err = "Incomplete treasure record.\n";
            write(STDERR_FILENO, err, strlen(err));
            free(scores);
            close(fd);
            return EXIT_FAILURE;
        }

        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(scores[i].user, t.user_name) == 0) {
                scores[i].score += t.value;
                found = 1;
                break;
            }
        }

        if (!found) {
            if (count == capacity) {
                capacity += CHUNK_SIZE;
                scores = realloc(scores, capacity * sizeof(UserScore));
                if (!scores) {
                    const char *err = "Memory allocation failed.\n";
                    write(STDERR_FILENO, err, strlen(err));
                    close(fd);
                    return EXIT_FAILURE;
                }
            }
            strncpy(scores[count].user, t.user_name, sizeof(scores[count].user) - 1);
            scores[count].user[sizeof(scores[count].user) - 1] = '\0';
            scores[count].score = t.value;
            count++;
        }
    }

    close(fd);

    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "\nHunt: %s\n", argv[1]);
    write_line(buffer);

    for (int i = 0; i < count; i++) {
        snprintf(buffer, sizeof(buffer), "%s: %d\n", scores[i].user, scores[i].score);
        write_line(buffer);
    }

    free(scores);
    return EXIT_SUCCESS;
}
