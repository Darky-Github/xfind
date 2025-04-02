#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#define MAX_PATH 1024
#define MAX_THREADS 4

// Structure to pass arguments to threaded search
typedef struct {
    char directory[MAX_PATH];
    char query[MAX_PATH];
    int case_insensitive;
    int search_text;
} SearchArgs;

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 32;
        }
    }
}

// Function to check if a file contains the search term
int file_contains_text(const char *filepath, const char *text) {
    FILE *file = fopen(filepath, "r");
    if (!file) return 0;
    char line[MAX_PATH];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, text)) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void *search_directory(void *args) {
    SearchArgs *searchArgs = (SearchArgs *)args;
    struct dirent *entry;
    DIR *dir = opendir(searchArgs->directory);
    if (!dir) return NULL;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char path[MAX_PATH];
        snprintf(path, sizeof(path), "%s/%s", searchArgs->directory, entry->d_name);
        struct stat statbuf;
        if (stat(path, &statbuf) == 0) {
            char filename[MAX_PATH];
            strcpy(filename, entry->d_name);
            if (searchArgs->case_insensitive) to_lowercase(filename);
            if (strstr(filename, searchArgs->query) != NULL || 
                (searchArgs->search_text && S_ISREG(statbuf.st_mode) && file_contains_text(path, searchArgs->query))) {
                printf("%s\n", path);
            }
            if (S_ISDIR(statbuf.st_mode)) {
                SearchArgs newArgs;
                strcpy(newArgs.directory, path);
                strcpy(newArgs.query, searchArgs->query);
                newArgs.case_insensitive = searchArgs->case_insensitive;
                newArgs.search_text = searchArgs->search_text;
                search_directory(&newArgs);
            }
        }
    }
    closedir(dir);
    return NULL;
}

void print_help() {
    printf("Usage: xfind [options] <query>\n");
    printf("Options:\n");
    printf("  -i          Case insensitive search\n");
    printf("  -text       Search inside file contents\n");
    printf("  -h          Show this help message\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    int case_insensitive = 0;
    int search_text = 0;
    char *query = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            case_insensitive = 1;
        } else if (strcmp(argv[i], "-text") == 0) {
            search_text = 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            print_help();
            return 0;
        } else {
            query = argv[i];
        }
    }

    if (!query) {
        printf("Error: No search query provided.\n");
        return 1;
    }

    SearchArgs args;
    getcwd(args.directory, MAX_PATH);
    strcpy(args.query, query);
    args.case_insensitive = case_insensitive;
    args.search_text = search_text;

    pthread_t threads[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&threads[i], NULL, search_directory, &args);
    }
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
