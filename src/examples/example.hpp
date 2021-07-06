#include <iostream>
#include <fstream>
#include <string>

inline std::string 
read_from_file(char const* infile)
{
   std::ifstream instream(infile);
   if (!instream.is_open()) {
      std::cerr << "Could not open file: " << infile << std::endl;
      exit(-1);
   }
   instream.unsetf(std::ios::skipws); // no white space skipping!
   return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
                      std::istreambuf_iterator<char>());
}

