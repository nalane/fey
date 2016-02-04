#include "log.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

// Records a message to the log file
bool vRecordLog(char* message, va_list args) {
  static const char LOG_FILE[] = "log.txt";
  
  FILE* f = fopen(LOG_FILE, "w");
  if (f) {
    vfprintf(f, message, args);
    fprintf(f, "\n");
    fclose(f);
    return true;
  }

  else {
    fprintf(stderr, "ERROR: Could not write to the log file at %s\n\n", LOG_FILE);
    fprintf(stderr, "Message intended to be logged:\n");
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    return false;
  }
}

bool recordLog(char* message, ...) {
  va_list args;
  va_start(args, message);
  
  bool retVal = vRecordLog(message, args);
  
  va_end(args);
  return retVal;
}
