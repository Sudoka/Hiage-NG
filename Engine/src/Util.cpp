#include <fstream>
#include <string>
#include "Util.h"

using namespace std;
using namespace Hiage;

string ReadFileToEnd(string file)
{
    string output;
    ifstream stream(file);

    stream.seekg(0, ios::end);
    output.resize(stream.tellg());
    stream.seekg(0, ios::beg);

    stream.read(&output[0], output.size());

    return output;
}
