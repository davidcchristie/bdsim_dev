/* common header  for the lexer and the parser 
to share 
Symbol table for numeric variables and functions
*/

#include <list>

#define _INT 0
#define _DOUBLE 1
#define _ARRAY 2


using namespace std;
 
#define NSYMS 1024
 
struct symtab {
  int type;
  char *name;
  double (*funcptr)(double);
  double value;
  list<double> array;
};
 
struct symtab *symlook();

