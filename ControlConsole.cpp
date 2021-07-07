//#include "Time.cpp"

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
//#define SLEEP 100

using namespace std;

struct ControlConsole
{
 vector<string> text;

 ControlConsole();

 void Puts(const vector<string>&);
 void RewriteLine(const int&, const string&);
 void RewriteDisplay(const vector<string>&);

 void MoveCursorBeginning();
 void MoveCursorEnd();
 void MoveCursorPosition(int n=1, int m=1);
 void MoveUpCursor(int n=1);
 void MoveDownCursor(int n=1);
 void MoveRightCursor(int n=1);
 void MoveLeftCursor(int n=1);
 void EraseDisplayBelow();
 void EraseDisplayAbove();
 void EraseDisplayEntire();
 void EraseLineBelow();
 void EraseLineAbove();
 void EraseLineEntire();
};

ControlConsole::ControlConsole()
{
 //EraseDisplayEntire();
}

void ControlConsole::Puts(const vector<string>& S)
{
 int line;
 line = S.size();
 text = S;
 for (int i = 0; i < line; ++i)
 {
  printf("%s", S[i].c_str());
  if (i < line-1) printf("\n");
 }
}

void ControlConsole::RewriteLine(const int& n, const string& s)
{
 MoveCursorPosition(n, 0);
 EraseLineEntire();
 printf("%s", s.c_str());
}

void ControlConsole::RewriteDisplay(const vector<string>& S)
{
 int line = text.size();
 for (int i = 0; i < line; ++i)
 {
  if (i >= S.size())
  {
   MoveCursorEnd();
   MoveLeftCursor(0);
   EraseLineEntire();
   text.pop_back();
  }
  else if (text[i] != S[i])
  {
   RewriteLine(i+1, S[i]);
   text[i] = S[i];
  }
 }
 for (int i = line; i < S.size(); ++i)
 {
  MoveCursorEnd();
  text.emplace_back(S[i]);
  printf("%s", S[i].c_str());
 }
}

void ControlConsole::MoveCursorBeginning()
{
 MoveCursorPosition(0, 0);
}

void ControlConsole::MoveCursorEnd()
{
 MoveCursorPosition(text.size(), 0);
}

void ControlConsole::MoveCursorPosition(int n, int m)
{
 printf("\033[%d;%dH", n, m);
}

void ControlConsole::MoveUpCursor(int n)
{
 printf("\033[%dA", n);
}

void ControlConsole::MoveDownCursor(int n)
{
 printf("\033[%dB", n);
}

void ControlConsole::MoveRightCursor(int n)
{
 printf("\033[%dC", n);
}

void ControlConsole::MoveLeftCursor(int n)
{
 printf("\033[%dD", n);
}

void ControlConsole::EraseDisplayBelow()
{
 printf("\033[0J");
}

void ControlConsole::EraseDisplayAbove()
{
 printf("\033[1J");
}

void ControlConsole::EraseDisplayEntire()
{
 printf("\033[2J");
}
void ControlConsole::EraseLineBelow()
{
 printf("\033[0K");
}

void ControlConsole::EraseLineAbove()
{
 printf("\033[1K");
}

void ControlConsole::EraseLineEntire()
{
 printf("\033[2K");
}

/*
int main()
{
 ControlConsole C;
 vector<string> S = {"123456789",\
                     "2abcdefgh",\
                     "3kijklmno",\
                     "4pqrstuvw"};
 C.Puts(S);
 Sleep(300);
 vector<string> T = {"123456789",\
                     "2ab0defgh",\
                     "3kijk0mno",\
                     "4pqrstuvw",\
                     "eee",\
                     "ttt"};

 vector<string> U = {"123456789",\
                     "2ab0defgh",\
                     "3kijk0mno"};

// C.MoveLeftCursor(3);
// C.MoveUpCursor(2);
 C.RewriteDisplay(T);
 Sleep(300);


// C.EraseDisplayEntire();
 C.RewriteDisplay(U);
 Sleep(300);

 return 0;
}
*/