#pragma once

/*
 * Considering the fact that paths given to fopen are relative to cwd,
 * we must give it the full path determined by the path of the program.
 */

typedef char* path;

void setUpProgramPath();
char* strConcatenate(char* s1, char* s2);
path getFullPath(path s);
void freePath(path s);
void cleanupPaths();
