#include "log.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

const char* logFile;

// Initializes the log file
void initLogging(const char* file) {
  logFile = file;
  FILE* f = fopen(logFile, "w");
  fclose(f);
}

// Records a message to the log file
bool vRecordLog(char* message, va_list args) {
  FILE* f = fopen(logFile, "a");
  if (f) {
    vfprintf(f, message, args);
    fprintf(f, "\n");
    fclose(f);
    return true;
  }

  else {
    fprintf(stderr, "ERROR: Could not write to the log file at %s\n\n", logFile);
    fprintf(stderr, "Message intended to be logged:\n");
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    return false;
  }
}

// Wrapper around vRecordLog
bool recordLog(char* message, ...) {
  va_list args;
  va_start(args, message);
  
  bool retVal = vRecordLog(message, args);
  
  va_end(args);
  return retVal;
}
