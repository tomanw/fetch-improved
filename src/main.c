//* Title: Fetch-Improved
//* Description: An improved version of my original fetch clone
//* Author: @tomanw


#include <bits/pthreadtypes.h>
#include <complex.h>
#include <linux/sysinfo.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/sysinfo.h>
#include "main.h"
#define BUFFER_SIZE 1024
#define KEYWORD_LIMIT 255
#define us unsigned

char* getenv(const char *name);
extern void free(void *);
extern void *malloc(unsigned long);

int main() {
   char* OS = GetOS();
   char* FULL_NAME = GetName();
   char* UPTIME = GetUptime();
   char* SHELL = GetShell();

   printf("╭─ %s\n", OS);
   printf("├─ %s\n", SHELL);
   printf("├─ %s\n", FULL_NAME); 
   printf("╰─󰄉 %s\n", UPTIME);
   return 0;
}


// DESCRIPTION: Gets the OS from 'PRETTY_NAME'
char* GetOS() {
    char* OS = GrepCMD("/etc/os-release", "PRETTY_NAME");
    char* strippedOS = RemoveQuotes(OS, "PRETTY_NAME=");
    return strippedOS;
}

// DESCRIPTION: Gets the username + hostname in the format of 'username@hostname'
char* GetName() {
    char hostname[1024];
    char username[1024];
    char* uptime = malloc(BUFFER_SIZE);
    
    char *full_name = NULL;
    //* get hostname
    if (gethostname(hostname, sizeof(hostname)) < 0) {
        perror("gethostname");
        return NULL;
    }
    //* get username
    if (getlogin_r(username, sizeof(username)) < 0) {
        perror("getlogin_r");
        return NULL;
    }
    //* allocate memory for full_name
    full_name = malloc(sizeof(char) * (strlen(username) + strlen(hostname) + 2)); // +2 for '@' and null terminator
    if (full_name == NULL) {
        perror("malloc");
        return NULL;
    }
    //* combine them together with an @
    snprintf(full_name, strlen(username) + strlen(hostname) + 2, "%s@%s", username, hostname);
    return full_name;
}

// NOTE: for info about buffers check /.notes/c-lang/buffers.txt
// DESCRIPTION: Returns the uptime in the format of '[days] Day('s), [hours] Hour('s), [minutes] Minute('s)'
char* GetUptime() {
   char* buffer = malloc(BUFFER_SIZE);
    struct sysinfo info;
    sysinfo(&info);
    unsigned long UpTime       = info.uptime;
    unsigned long SecPDay      = 86400;
    unsigned long SecPHour     = 3600;
    unsigned long SecPMinute   = 60;

    unsigned long days         = UpTime / SecPDay;
    unsigned long hours        = ( UpTime % SecPDay ) / SecPHour;
    unsigned long minutes      = ( UpTime % SecPHour ) / SecPMinute;
//   unsigned long seconds      = UpTime % SecPMinute;
	


    // buffer parts
    if ( buffer == NULL ) {
	    perror("can't allocate memory");
    }

    snprintf(buffer, BUFFER_SIZE,
		    "%lu day%s, %lu hour%s, %lu minute%s",
		    days, (days == 1) ? "": "s",
		    hours, (hours == 1) ? "": "s",
		    minutes, (minutes == 1) ? "": "s");
//		    seconds, (seconds == 1) ? "": "s");

    return buffer;
}

// DESCRIPTION: Returns the shell being used
char* GetShell() {
   return getenv("SHELL");
}


//! Stupid Shit


// DESCRIPTION: Removes quotes and name from the result of GrepCMD();
char* RemoveQuotes(char* content, char* name) {
    char *strippedResult = content + strlen(name) + 1; // removes "PRETTY_NAME="
    char *endQuote = strchr(strippedResult, '"'); // gets the end quote
    if (endQuote) {
        *endQuote = '\0'; // sets the end quote to a null character
    }
    return strippedResult;
}

// DESCRIPTION: Returns the result of the "grep" command in bash
char* GrepCMD(const char *fileName, const char *pattern) {
    // construct the command to be executed
    char command[BUFFER_SIZE];
    snprintf(command, sizeof(command), "grep '%s' %s", pattern, fileName);

    // open pipe
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to open pipe!");
        return NULL;
    }

    // allocate memory for the result
    char *result = malloc(BUFFER_SIZE);
    if (result == NULL) {
        perror("Failed to allocate memory");
        pclose(fp);
        return NULL;
    }

    // read grep output
    if (fgets(result, BUFFER_SIZE, fp) == NULL) {
        free(result);
        result = NULL;
    }

    // close the pipe
    if (pclose(fp) == -1) {
        perror("Error closing pipe");
    }

    return result;
} 
