#include <iostream>
#include <fstream>
#include <string>
#include "Suffix-Tree.hpp"

using namespace std;

string readQuery();
MiniGoogle *mg = new MiniGoogle(1);


long get_results(char *word){
    //string word;
    //getline(cin, word);
    string newWord = Lower(string(word));
    long numberResults = mg->find(newWord, false);
    return numberResults;
}

void make_suffix_tree()
{
    MakeSuffixTree(mg);
}

char* get_contenido(char* contenido, int dbIndex)
{
    //string content = mg->get_contenido(dbIndex);
    //cout << ">>>>>>>>>>>>>>>>>>>"<<content << endl;
    contenido = (char*)malloc(sizeof(char) * 100000000);
    return strcat(contenido,mg->get_contenido(dbIndex).c_str());
}

extern "C"
{
    extern float cffi_get_results(char *word)
    {
        return get_results(word);
    }

    extern void cffi_make_suffix_tree()
    {
        return make_suffix_tree();
    }

    extern char* cffi_get_contenido(char* contenido, int dbIndex)
    {
        return get_contenido(contenido, dbIndex);
    }
}

/*string readQuery(){
    string      line;
    ifstream    myfile("core/comm/log.txt");

    if (myfile.is_open()){
        getline(myfile, line);
        myfile.close();
        return line;
    }
    else {
        cout<< "Unable to open file!";
        return "$";
    }
}*/
