#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string readQuery();

int main(){
    string OldQuery="$";
    string Query;

    while (true){
        Query = readQuery();
        if (Query != OldQuery && Query.length()!=0){
            cout<<"Query: "<< Query;
            cout<<", length: "<< Query.length()<<endl;
        }
        OldQuery = Query;
    }

    return 0;
}

string readQuery(){
    string      line;
    ifstream    myfile("comm/log.txt");

    if (myfile.is_open()){
        getline(myfile, line);
        //cout<< line << '\n';
        myfile.close();
        return line;
    }
    else {
        cout<< "Unable to open file!";
        return "$";
    }
}
