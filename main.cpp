#include <fstream>
#include <iostream>
#include <string>

#include "input_reader.h"
#include "stat_reader.h"

using namespace std;
using namespace catalogue::transcat;
using namespace catalogue::inpreader;
using namespace catalogue::streader;

int main() {
  TransportCatalogue catalogue;
  ifstream in_file("C:\\02transport\\my_transcat\\in.txt"s);

  int base_request_count;
  in_file >> base_request_count >> ws;

  {
    InputReader reader;
    for (int i = 0; i < base_request_count; ++i) {
      string line;
      getline(in_file, line);
      reader.ParseLine(line);
    }

    reader.ApplyCommands(catalogue);
  }

  int stat_request_count;
  in_file >> stat_request_count >> ws;
  ofstream out_file("C:\\02transport\\my_transcat\\out.txt"s);
  for (int i = 0; i < stat_request_count; ++i) {
    string line;
    getline(in_file, line);

    ParseAndPrintStat(catalogue, line, out_file);
  }
}
