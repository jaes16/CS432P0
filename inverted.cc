#include <iostream>
#include <fstream>
#include <string>

using namespace std;


#define STRSIZE 1024
#define NUMFILES 512
#define MAXSIZE 2048


struct wordRef {
  string str;
  int indexList[NUMFILES];
  int sizeIndexList = 0;
};


wordRef archive[MAXSIZE];
int size = 0;

void printArchive(){
  for (int i = 0; i < size; i++){
    cout << archive[i].str << ":";
    for (int j = 0; j < archive[i].sizeIndexList; j++){
      cout << " " << archive[i].indexList[j];
    }
    cout << "\n";
  }
}


int sameWordRef(wordRef ref1, string ref2){
  return ref1.str.compare(ref2);
}



void saveWord(string key, int filenum){
  //cout << "gothere3";
  // archive is empty
  if(size == 0){
    wordRef newRef;
    newRef.str = key;
    newRef.indexList[0] = filenum;
    newRef.sizeIndexList = 1;
    archive[0] = newRef;
    size++;
  } else {

    // search 
    for(int i = 0; i < size; i++){
      // if word is already in archive, add the file number
      if(sameWordRef(archive[i], key) == 0){
	// check if word is was already in the current file
	for(int j = 0; j < archive[i].sizeIndexList; j++){
	  if(archive[i].indexList[j] == filenum){
	    return;
	  }
	}
	archive[i].indexList[archive[i].sizeIndexList] = filenum;
	archive[i].sizeIndexList++;
	return;
      }
    }
    // else add new word
    wordRef newRef;
    newRef.str = key;
    newRef.indexList[0] = filenum;
    newRef.sizeIndexList = 1;
    archive[size] = newRef;
    size++;

  }
  //printArchive();
  //cout << "----------------------------\n";
}


void onlyAlpha(string line, int filenum){
  //cout << "gothere2\n";
  int l = line.length();
  string str;
  for(int i = 0; i < l+1; i++){
    //cout << str << ", ";
    if(isalpha(line[i])){
      str.push_back(line[i]);
      if(i == l){
	saveWord(str, filenum);
      }
    } else if (!str.empty()) {
      saveWord(str, filenum);
      //cout << str;
      str = "";
    } 
  }


}

void readinFile(string name, int filenum){
  //cout << "got here1\n";
  string s;
  ifstream input;
  input.open(name);
  while (input >> s){
    //cout << s << "\n";    
    onlyAlpha(s, filenum);
  }
  input.close();
  input.clear();


}


void takeFiles(string masterFile){
  string fileNames[NUMFILES];
  int curNum = 0;
  bool repeat = false;

  ifstream input;
  input.open(masterFile);
  string s;
  int numFile = 0;

  while (input >> s){
    ifstream test;
    test.open(s);
    if((s.find(".txt") != string::npos) && test.good()){
      for(int i = 0; i < curNum; i++){
	if(s.compare(fileNames[i]) == 0){
	  repeat = true;
	}
      }
      if (!repeat){
	// update filenames
	fileNames[curNum] = s;
	curNum++;

	// finally add
	//cout << s << "\n";    
	readinFile(s, numFile);
	numFile++;
      }
      repeat = false;
    }
  }
  input.close();
  input.clear();


}






void merge(wordRef a[], int m, int s)
{
  int i, j, k;
  int n =  s - m;

  wordRef temp1[m];

  for (i = 0; i < m; i++) {
    temp1[i] = a[i];
  }
  i = 0;
  j = 0; 
  k = 0; 
  while (i < m && j < n) {
    if (temp1[i].str.compare(a[m+j].str) <= 0) {
      a[k++] = temp1[i++];
    } else {
      a[k++] = a[m+j++];
    }
  }

  while (i < m) {
    a[k++] = temp1[i++];
  }
  while (j < n) {
    a[k++] = a[m+j++];
  }

}

void mergeSort(wordRef a[], int s)  {
  if (s != 1) {
    int m = s/2;

    mergeSort(&a[0], m);
    mergeSort(&a[m], (s-m));

    merge(a, m, s);
  }
}





int main(int argc, char **argv){
  //cout << argv[1];
  takeFiles(argv[1]);
  //  printArchive();
  //  cout<<"------------------------------\n-----------------------------\n";
  mergeSort(archive, size);
  printArchive();
  return 1;
}
