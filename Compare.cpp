#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include "Utilities/Scanner.h"
#include "Utilities/ScanLine.h"
#include "Utilities/Utils.h"

using namespace std;

//holder for occurences of words
typedef struct word
{
  string token;
  int count;
  word(){}
  word(string& tkn){token = tkn;}
  friend bool operator>(word a, word b)
  {
    return a.token > b.token;
  }
  friend bool operator<(word a, word b)
  {
    return a.token < b.token;
  }
  friend bool operator<(word a, string b)
  {
    return a.token < b;
  }
  friend bool operator>(word a, string b)
  {
    return a.token > b;
  }
  friend bool operator==(word a, word b)
  {
    return a.token == b.token;
  }
  friend bool operator==(word a, string b)
  {
    return a.token == b;
  }
} WORD;


//declaration of used functions
double compare(set<WORD>, set<WORD>);

int main(int argc, char** varg)
{
  if(argc < 4)//this is how to use this
  {
    cerr << "usage: cmd file1 file2 output" << endl;
    return 1;
  }

  //these are for holding each files occurances of strings, or lack thereof
  set<WORD> file1, file2;
  
  //Java like Scanner object
  Scanner scan;
  //open the file given as argument 1
  scan.openFile(varg[1]);
  //to the first file 'file1' we will add each string that is in there
  //for the first occurance we set count at 1, and increment as needed
  //to the second file 'file2' we will add each string from 'file1'
  //and set the count to 0
  while(scan.hasMoreData())
  {
    ScanLine line;
    line.openString(scan.nextLine());
    while(line.hasMoreData())
    {
      string nxt_token = line.next();
      nxt_token = Utils::trimBlanks(nxt_token);

      //add elements to 'file2' once with count 0
      if(file2.count(nxt_token) == 0)
      {
	WORD f_two;
	f_two.token = nxt_token;
	f_two.count = 0;
        file2.insert(f_two);
      }
      //add first occurance to file1 with count 1
      if(file1.count(nxt_token) == 0)
      {
	WORD f_one;
	f_one.token = nxt_token;
	f_one.count = 1;
        file1.insert(f_one);
      }
      else
      {
	//replace the word with a word of equal token and +1 count
	set<WORD>::iterator it = file1.find(WORD(nxt_token));
	int ct = it->count;
	ct++;
	file1.erase(it);
	WORD replace;
	replace.token = nxt_token;
	replace.count = ct;
	file1.insert(replace);
      }
    }
  }
  scan.close();//close file1

  cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

  scan.openFile(varg[2]);//open file2

  //to the second file 'file2' we will add each word we find, or
  //increment as needed if it occurred in the first file
  //if a string occurs in 'file2' that doesn't exist in 'file1'
  //we will add any token to 'file1' list from 'file2' list and set count to 0
  while(scan.hasMoreData())
  {
    ScanLine line;
    line.openString(scan.nextLine());
    while(line.hasMoreData())
    {
      string nxt_token = line.next();
      nxt_token = Utils::trimBlanks(nxt_token);
      
      //if the word doesn't already exist in 'file1' add it with count 0
      if(file1.count(nxt_token) == 0)
      {
	WORD f_two;
	f_two.token = nxt_token;
	f_two.count = 0;
        file1.insert(f_two);
      }
      //add the first occurance of a word to 'file2' with count 1
      if(file2.count(nxt_token) == 0)
      {
	WORD f_one;
	f_one.token = nxt_token;
	f_one.count = 1;
        file2.insert(f_one);
      }
      else
      {
	//replace the word with a word of equal token and +1 count	
	set<WORD>::iterator it = file2.find(WORD(nxt_token));
	int ct = it->count;
	ct++;
	file2.erase(it);
	WORD replace;
	replace.token = nxt_token;
	replace.count = ct;
	file2.insert(replace);
      }
    }
  }
  scan.close();

  ofstream outStream;
  Utils::FileOpen(outStream, varg[3]);//open the output file
  //write to it...
  outStream << Utils::Format(compare(file1, file2), 2, 6) << endl;
  Utils::FileClose(outStream);//close it
}

/* Compare two files using cosine similarity measure
 * @param the files to compare
 * @return the cosine of the angles between two multidimensional 'vectors'
 */
double compare(const set<WORD> one, const set<WORD> two)
{
  double cosine = 0.0;
  double product = 0.0;
  double absOne = 0.0;
  double absTwo = 0.0;

  if(one.size() !=  two.size())
  {
    cerr << "something went wrong @ read" << endl;
    exit(1);
  }
  {  //reduce the scope of the iterators

  //dot-products 
  //product = one . two
  //absOne = one . one
  //absTwo = two . two
  set<WORD>::iterator itA = one.begin();
  set<WORD>::iterator itB = two.begin();
  while(itA != one.end() && itB != two.end())
  {
    product += static_cast<double>(itA->count)
             * static_cast<double>(itB->count);
    absOne += static_cast<double>(itA->count)
            * static_cast<double>(itA->count);
    absTwo += static_cast<double>(itB->count)
            * static_cast<double>(itB->count);
    itA++;
    itB++;
  }
  }// exit reduced scope

  absOne = sqrt(absOne);//sqrt to find the magnitude
  absTwo = sqrt(absTwo);//for one & two
  //don't divide by zero
  if(absOne * absTwo == 0.0) return cosine;
  //find the cosine similarity
  cosine = product / (absOne * absTwo);

  return cosine;
}
