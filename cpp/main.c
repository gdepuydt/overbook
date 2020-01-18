#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 256 * 1024

struct Mistake {
    // Human readable description of the mistake
    char *message;
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
            struct Mistake *m = malloc(sizeof(Mistake));
            m->message = "commas are forbidden!\n";
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

    // check for mistakes, and wite in result
    check_mistakes(buf, len, result);
    
    return result;
}

int main() {
    char buf[BUF_SIZE];

    struct CheckResult *result = check("sample.txt", buf);
    if(result->mistake != NULL) {
        printf("Commas are forbidden!\n");
        return 0;
    }

    return 0;
}