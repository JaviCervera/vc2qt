#include "projectinfo.h"
#include "str.h"
#include <iostream>

using namespace std;

// This is really dirty... should move to exception handling soon
namespace rapidxml {
  void parse_error_handler(const char *what, void *where) {
    cout << "Parse error: " << what << endl;
    system("pause");
    abort();
  }
}

int main(int argc, char* argv[]) {
  // Check number of arguments
  if ( argc < 2 || argc > 3 ) {
    cout << "Usage: vc2qt vcprojfile [profile]" << endl;
    return -1;
  }

  // Get input and output filenames
  string vcfile = argv[1];
  string qtfile;
  if ( argc == 3 ) {
    qtfile = argv[2];
  } else {
    qtfile = str::strip_ext(argv[1]) + ".pro";
  }
  if ( vcfile == qtfile ) {
    cout << "Error: VC and Qt filenames cannot be the same" << endl;
    return -2;
  }

  // Parse VC project
  cout << "Parsing VC project..." << endl;
  ProjectInfo project(str::read(vcfile));
  cout << "Parsed" << endl;

  // Export Qt project
  cout << "Generating Qt project..." << endl;
  str::write(project.GetAsQtProject(), qtfile, false);
  cout << "Generated" << endl;

  return 0;
}
