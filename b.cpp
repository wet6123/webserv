#include <fstream>
#include <iostream>

using namespace std;

int main(void) {
  fstream file("test", ios::in);

  int cnt = 0;
  while (true) {
    if (file.eof())
      break;
    char c = file.get();
    if (c == 'O')
      cnt++;
  }
  cout << cnt << endl;
}