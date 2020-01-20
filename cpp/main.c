#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 256 * 1024
#define MAX_RESULTS 256

struct Mistake {
    // Human readable description of the mistake
    char *message;

    // Points to where the mistake is in the text
    char *location;
} Mistake;

struct CheckResult {
    // Name of the file we checked
    char *path;

    // NULL if manuscript was fine,
    // otherwise describes what's wrong with it
    struct Mistake *mistake;
} CheckResult;

size_t read_file(char *path, char *buf) {
    FILE *f = fopen(path, "r");
    size_t len = fread(buf, 1, BUF_SIZE - 1, f);
    fclose(f);
    buf[len] = '\0'; // null-terminate the string
    return len;
}

void check_mistakes(char *buf, size_t len, struct CheckResult *result) {
    for (size_t i = 0; i < len; i++) {
        if (buf[i] == ',') {
            struct Mistake *m = malloc(sizeof(struct Mistake));
            size_t location_len = len - i;
            if (location_len > 12) {
                location_len = 12;
            }
                     
            m->location = malloc(location_len + 1);
            memcpy(m->location, &buf[i], location_len);
            m->location[location_len] = '\0';
            m->message = "commas are forbidden";
            result->mistake = m;
            break;
        }
    }
}

struct CheckResult* check(char *path, char *buf) {
    struct CheckResult *result = malloc(sizeof(CheckResult));
    result->path = path;
    result->mistake = NULL;

    // read file in buf and return the length of the file 
    size_t len = read_file(path, buf);

    // check for mistakes, and write in result
    check_mistakes(buf, len, result);
    
    return result;
}

void report(struct CheckResult *result) {
    printf("\n");
    printf("~ %s ~\n", result->path);
    if(result->mistake == NULL) {
        printf("No mistakes were found!\n");
    }
    else {
        // TODO (Geert): print mistake text
        printf(
            "mistake: %s: '%.12s'\n",
            result->mistake->message,
            result->mistake->location
        );
    }
}

int main() {
    char buf[BUF_SIZE];

    struct CheckResult *bad_results[MAX_RESULTS];
    int num_results = 0;
    
    char *paths[] = {"sample3.txt", "sample2.txt", "sample.txt", NULL};

    for (int i = 0; paths[i] != NULL; i++) {
        char *path = paths[i];
        struct CheckResult *result = check(path, buf);
        bad_results[num_results++] = result;
    }

    for (int i = 0; i < num_results; i++) {
        report(bad_results[i]);
    }

    for (int i = 0; i < num_results; i++) {
        free(bad_results[i]->mistake->location);
        free(bad_results[i]->mistake);
        free(bad_results[i]);
    }

    return 0;
}