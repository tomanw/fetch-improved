#include <bits/pthreadtypes.h>
#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#define BUFFER_SIZE 1024
#define KEYWORD_LIMIT 255

char* getenv(const char *name);
char* GrepCMD(const char *fileName, const char *pattern);
extern void free(void *);
extern void *malloc(unsigned long);
char* RemoveQuotes(char* content, char* name);
char* GetOS();
char* GetName();

int main() {
    char* OS = GetOS();
    char* FULL_NAME = GetName();
    printf("╭─ %s\n", OS);
    printf("├─ %s\n", FULL_NAME);
    return 0;
}


char* GetOS() {
    char* OS = GrepCMD("/etc/os-release", "PRETTY_NAME");
    char* strippedOS = RemoveQuotes(OS, "PRETTY_NAME=");
    return strippedOS;
}

char* GetName() {
    return getenv("USER");
}   


//! _     _ _                          
//!| |   (_) |__  _ __ __ _ _ __ _   _ 
//!| |   | | '_ \| '__/ _` | '__| | | |
//!| |___| | |_) | | | (_| | |  | |_| |
//!|_____|_|_.__/|_|  \__,_|_|   \__, |
//!                              |___/ 



// removes quotes and name from the result of GrepCMD();
char* RemoveQuotes(char* content, char* name) {
    char *strippedResult = content + strlen(name) + 1; // removes "PRETTY_NAME="
    char *endQuote = strchr(strippedResult, '"'); // gets the end quote
    if (endQuote) {
        *endQuote = '\0'; // sets the end quote to a null character
    }
    return strippedResult;
}

// returns the result of the "grep" command in bash
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