#include "log.hpp"

#include <iostream>
#include <fstream>

using namespace std;

string logFile;
bool debug;

// Initializes the log file
void initLogging(string file, bool debugFlag) {
  debug = debugFlag;
  logFile = file;
  ofstream fout(logFile.c_str());

  if (!fout.is_open())
    cout << "Could not open the logging file." << endl;

  else 
    fout.close();
}

// Records a message to the log file
bool recordLog(string message) {
  if (debug)
    cout << message << endl;
  
  ofstream fout(logFile.c_str(), ostream::app);
  if (fout.is_open()) {
    fout << message << endl;
    fout.close();
    return true;
  }

  else {
    cerr << "ERROR: Could not write to the log file at " << logFile << endl;
    cerr << "Message intended to be logged:" << endl;
    cerr << message << endl << endl;
    return false;
  }
}
