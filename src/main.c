#include <complex.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define KEYWORD_LIMIT 255

char* GrepCMD(const char *fileName, const char *pattern);
extern void free(void *);
extern void *malloc(unsigned long);
char* RemoveQuotes(char* content, char* name);

int main() {
    char *result = GrepCMD("/etc/os-release", "PRETTY_NAME=");
    char *stripped = RemoveQuotes(result, "PRETTY_NAME=");
    printf("%s\n", stripped);
    return 0;
}

char* RemoveQuotes(char* content, char* name) {
    char *strippedResult = content + strlen(name) + 1;
    char *endQuote = strchr(strippedResult, '"');
    if (endQuote) {
        *endQuote = '\0';
    }
    return strippedResult;
}

char* GrepCMD(const char *fileName, const char *pattern) {
    // Construct the command to be executed
    char command[BUFFER_SIZE];
    snprintf(command, sizeof(command), "grep '%s' %s", pattern, fileName);

    // Open pipe
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to open pipe!");
        return NULL;
    }

    // Allocate memory for the result
    char *result = malloc(BUFFER_SIZE);
    if (result == NULL) {
        perror("Failed to allocate memory");
        pclose(fp);
        return NULL;
    }

    // Read grep output
    if (fgets(result, BUFFER_SIZE, fp) == NULL) {
        free(result);
        result = NULL;
    }

    // Close the pipe
    if (pclose(fp) == -1) {
        perror("Error closing pipe");
    }

    return result;
} 