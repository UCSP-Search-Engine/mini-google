#include <iostream>
#include <fstream>
#include <string>
#include "Suffix-Tree.hpp"

using namespace std;

int main(){
	MiniGoogle *mg = new MiniGoogle(1);
	
	MakeSuffixTree(mg);

	while (true){
		string query_val;
		cout<<"\rQuery: ";
		getline(cin, query_val);
		if (query_val.size()==0 or query_val == ""){
			continue;
		}
		query_val = Lower2(query_val);
		mg->findQuery(query_val, true, false);
		cout<<"\n================================== Final de busqueda ("<< query_val << ")================================\n"<<endl;
	}

	return 0;
}