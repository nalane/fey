#include "log.hpp"

#include <iostream>
#include <fstream>

using namespace std;

string logFile;
bool debug;
ofstream fout;

// Initializes the log file
void initLogging(const string& file, bool debugFlag) {
  debug = debugFlag;
  logFile = file;
  fout = ofstream(logFile.c_str());

  if (!fout.is_open())
    cout << "Could not open the logging file." << endl;
}

// Records a message to the log file
bool recordLog(const string& message) {
  if (debug)
    cout << message << "\n";
  
  if (fout.is_open()) {
    fout << message << "\n";
    return true;
  }

  else {
    fout = ofstream(logFile.c_str(), ostream::app);
    if (fout.is_open()) {
      fout << message << "\n";
      return true;
    }
    
    cerr << "ERROR: Could not write to the log file at " << logFile << endl;
    cerr << "Message intended to be logged:" << endl;
    cerr << message << endl << endl;
    return false;
  }
}

// Close log file
void endLogging() {
  fout.close();
}
