#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <functional>
#include "parse/tools.hpp"
#include <omp.h>

using namespace std;

//----> HASH
struct Articulo {
  string titulo;
  string contenido;
} ;
map <int, Articulo> mapContent;


char Change(char letra){
	if( (int)letra == -31 or (int)letra== -63 	or (int)letra==-95 or (int)letra==-127) 	letra = 'a'; 
	if(	(int)letra == -55 or (int)letra== -23 	or (int)letra==-87 or (int)letra==-119) 	letra = 'e'; 
	if( (int)letra == -19 or (int)letra== -51 	or (int)letra==-83 or (int)letra==-115) 	letra = 'i'; 
	if( (int)letra == -13 or (int)letra== -45 	or (int)letra==-77 or (int)letra==-109) 	letra = 'o'; 
	if( (int)letra == -38 or (int)letra== -6  	or (int)letra==-70 or (int)letra==-102) 	letra = 'u';
	if( (int)letra == -79 or (int)letra== -111  or (int)letra==-15)					 		letra = char(-15);
	return letra; 	
}

string ParserData(string s){
    int Switch;
    string resultado="";
    int longitud=s.length();	
    for (int i = 0; i < longitud; i++){
    	char chr = Change(s[i]);
    	resultado += chr;
    }
    return resultado;
}


string recorrerMAP(string s,int  pos){
	string resultado;
	//cout<<"pos recorrer"<<pos<<endl;
	s=s.substr(pos);int i;
	//cout<<"s "<<s<<endl;
	//system("PAUSE()");
	for(i=1;i<s.length();i++){
		if(s[i] == '"'){
			break;
		} 
	}	
	//cout<<"i "<<i<<endl;
	resultado=s.substr(0,i);
	//cout<<"resultado recorrer "<<resultado<<endl;
	//system("PAUSE()");
	resultado = ParserData(resultado);
	return resultado;
}

void cargarTexto(){
	setlocale(LC_ALL, "spanish");	
	setlocale(LC_CTYPE, "Spanish");
	 
	string url;
	int cont=10000;
//-------------------lectura de data 
	string frase;
	int i=0;
	vector <string> textInFile(999999);
	ifstream ficheroEntrada;

	for(int k = 0;cont < 485000;k++){

		//ficheroEntrada.open ("../../raw.es/spanishText_10000_15000");
		url="raw.es/spanishText_"+to_string(cont)+'_'+to_string(cont+5000);
		
		cont=cont+5000;

		ficheroEntrada.open (url);
		if(ficheroEntrada.fail()){
			continue;
		} else {
			printf("[%2d]", k);
			cout<<" File: "<<url<<endl;
		}
		while(!ficheroEntrada.eof()){  

			getline(ficheroEntrada, frase);
			if(frase =="</doc>"){	
				continue;
			}
			if(frase =="ENDOFARTICLE."){
				i++;	
				continue;
			}
		//si no es cabecera ni fin asignarle a una variable.
			if(frase !="ENDOFARTICLE." && frase !="</doc>"){
				textInFile[i] = textInFile[i] + frase;
				if(frase.length() != 0)
					textInFile[i]+= "\n";
				continue;		
			}
		}
		ficheroEntrada.close();
	}
//----------------------------------------------------------------

 //___________________________________________________________
	int posinit,posend,posid,postittle,posnfil,posproce,posdbindex;
	int a=i;
	
	for (int i = 0; i < a; ++i)
	{
		string sstart,ssend,ssid,ssttitle,ssnfil,ssprocesed,ssdbindex,mensaje;
		sstart='<',ssend='>',ssid='=',ssttitle='=',
		ssnfil='=',ssprocesed='=',ssdbindex='=';

		posinit=textInFile[i].find(sstart);

		posid=textInFile[i].find(ssid,posinit);
 		ssid=recorrerMAP(textInFile[i],posid+2);

		postittle=textInFile[i].find(ssttitle,posid+2);
		ssttitle=recorrerMAP(textInFile[i],postittle+2);	

		posnfil=textInFile[i].find(ssnfil,postittle+2);
		ssnfil=recorrerMAP(textInFile[i],posnfil+2);
		
		posproce=textInFile[i].find(ssprocesed,posnfil+2);
		ssprocesed=recorrerMAP(textInFile[i],posproce+2);

		posdbindex=textInFile[i].find(ssdbindex,posproce+2);
		ssdbindex=recorrerMAP(textInFile[i],posdbindex+2);	

		posend=textInFile[i].find(ssend);

		Articulo art;
		art.titulo = ssttitle;
		art.contenido = textInFile[i].substr(posend+2);
		mapContent.insert(pair <int, Articulo> (stoi(ssdbindex),art));

	}
}
//<---- HASH

class Edge;

class Node
{
private:
	vector<Edge> edges;

	friend class Edge;
	friend class SuffixTree;
	friend class MiniGoogle;
};

class Edge
{
private:
	string suffixString;
	Node *startNode;
	Node *endNode;
	vector<int> indexes; // dbindex of each text 
	vector<int> coincidences; // How many coincidences of the suffix for each text

public:
	Edge(Node *startNode, string suffixString, int index)
	{
		this->suffixString = suffixString;
		this->startNode = startNode;
		this->indexes.push_back(index);
		this->coincidences.push_back(1);
		this->endNode = NULL;
	}

    //Constructor in order to create a new node after an split because we need to store all the before coincidences
    Edge(Node *startNode, string suffixString, Edge edge)
	{
		this->suffixString = suffixString;
		this->startNode = startNode;
		this->indexes = edge.indexes;
		this->coincidences = edge.coincidences;
		this->endNode = NULL;
	}

    //Adding a new text index for a given suffix (when the suffix exist in the tree)
	void putOtherIndex(int index)
	{
		this->indexes.push_back(index);
		this->coincidences.push_back(1);
	}

    //Precaution: This part its a trouble to parallelize; because it has a dependence from the last one
    //inserted index.
	void addCantCoicidencia()
	{
		this->coincidences[coincidences.size()-1] += 1;
		
	}
	friend class Node;
	friend class SuffixTree;
	friend class MiniGoogle;
};

class Word
{
private:
	string word;
	int index;
public:
	Word() {}
	Word(string word, int index)
	{
		this->word = word;
		this->index = index;
	}

	vector<string> getWords(string text)
	{
		vector<string> words;
		
		int posSpace;
		string word;

		while (text.size() >= 1)
		{
			posSpace = text.find(" ");

			if (posSpace == -1)
			{
				words.push_back(text);
				break;
			}

			word = text.substr(0, posSpace);

			if (word.size() > 0)
				words.push_back(word);

			text = text.substr(posSpace + 1);
		}
		return words;
		
	}

	//Getting the word suffixes and set up their indexes.
	vector<string> getSuffix(string word)
	{
		vector<string> suffix;

		// ===================== (Sin Sufijos)
		
		suffix.push_back(word);
		return suffix;

		// ===================== (Sin Sufijos)
		int i = word.size() - 1;

		for (; i >= 0; i--)
		{
			if (i <4) {break;}
			suffix.push_back(word.substr(i));
		}
		return suffix;
	}

	friend class SuffixTree;
	friend class MiniGoogle;
};

class SuffixTree
{
	private:
		Node *root;

	public:
		SuffixTree()
		{
			this->root = new Node();
		}


	//{{'a','b'}}
	//Start point to add the words inside the tree. First we can read the text and separate each word, 
	//after that for each word we can extract the suffix in order to add them in the tree. Also we must have
    //like parameter the index (dbindex) of the text.
	void execution(string text, int index)
	{
		Word w;
		vector<string> words = w.getWords(text);
		int i = 0;

		bool FLAG=false;

		for (; i<words.size(); i++)
		{
			//cout<<"----------------"<<endl;
			//cout<<"-"<<words[i].word<<"-"<<words[i].index<<"-"<<endl;
			//if (words.size()>10){
			//}
			
			//cout<<"-->"<<endl;
			if (words.size()>2){
				for (int wi = 0; wi < words[i].size()-1; wi++){
					if (words[i][wi]==words[i][wi+1]){
						FLAG = true;
					}
				}
			}
			if (FLAG == true){continue;}
			//cout<<"<--"<<endl;
			
			vector<string> suffix = w.getSuffix(words[i]);
			
			for (int j = 0; j<suffix.size(); j++)
			{
				//cout<<"-"<<suffix[j].word<<"-"<<suffix[j].index<<"-"<<endl;
				addSuffix(suffix[j], index);
				//print();
				//return;
			}
			suffix.clear();
		}
		//
		words.clear();
	}

	//This function search return the correct node where we can insert the word suffix.
	//Also it returns the edge position where the suffix has a coincidence (if it is -1 there is not a coincidence).
	//On the other side, it returns the coincidence string with respect to the position to insert.
	Node * search(string &suffix, string &parteCoinciden, Node *node, int &edgePos,int index)
	{
		edgePos = -1;
		if (node->edges.size() == 0)
			return node;


		parteCoinciden = "";

		string substring1, substring2;
		for (int i = 0; i<node->edges.size(); i++)
		{
			substring1 = suffix.substr(0, 1);
			substring2 = node->edges[i].suffixString.substr(0, 1);

			if (substring1 == substring2)
			{

				//----------Agregar coicidencias
				//if the suffix is from a different text (push_back)
				if (node->edges[i].indexes[node->edges[i].indexes.size() - 1] != index)
				{
					node->edges[i].putOtherIndex(index);
				}
				else //If the suffix is from the same text, add coincidence(+1).
				{
					node->edges[i].addCantCoicidencia();
				}
				//---------------Fin Agregar coicidencias
				parteCoinciden += substring1;
				suffix = suffix.substr(1);
				edgePos = i;

				int j = 0, k = 1;
				for (; j<suffix.size() && k<node->edges[i].suffixString.size(); j++, k++)
				{
					substring1 = suffix.substr(j, 1);
					substring2 = node->edges[i].suffixString.substr(k, 1);


					edgePos = i;

					if (substring1 != substring2)
					{
						break;
					}
					parteCoinciden += substring1;

				}
				suffix = suffix.substr(j);

				if ((node->edges[i].endNode) != NULL && suffix.size()>0)
				{
					return search(suffix, parteCoinciden, node->edges[i].endNode, edgePos, index);
				}

				return node;
			}

		}

		return node;
	}

	// Recursive search: it searches the correct node from to root.
	Node * search(string &suffix, string &parteCoinciden, int &edgePos, int index)
	{
		return search(suffix, parteCoinciden, root, edgePos,index);
	}


    //Search: the same idea like the search in the tree; however in this case we store all the coincidence characters without
    //remove anything for each recursive call.
    Node * search2(string &suffix, string &parteCoinciden, Node *node, int &edgePos)
	{
		edgePos = -1;
		if (node->edges.size() == 0)
			return node;

		string substring1, substring2;
		for (int i = 0; i<node->edges.size(); i++)
		{
			substring1 = suffix.substr(0, 1);
			substring2 = node->edges[i].suffixString.substr(0, 1);
			if (substring1 == substring2)
			{
				parteCoinciden += substring1;
				suffix = suffix.substr(1);
				edgePos = i;
				int j = 0, k = 1;
				for (; j<suffix.size() && k<node->edges[i].suffixString.size(); j++, k++)
				{
					substring1 = suffix.substr(j, 1);
					substring2 = node->edges[i].suffixString.substr(k, 1);


					edgePos = i;

					if (substring1 != substring2)
					{
						break;
					}
					parteCoinciden += substring1;

				}
				suffix = suffix.substr(j);

				
				if ((node->edges[i].endNode) != NULL && suffix.size()>0)
				{
					return search2(suffix, parteCoinciden, node->edges[i].endNode, edgePos);
				}

				return node;
			}

		}

		return node;
	}

	// Recursive search: it searches the text where a given word exists after the insertion of the data.
	Node * search2(string &suffix, string &parteCoinciden, int &edgePos)
	{		
        parteCoinciden = "";
        return search2(suffix, parteCoinciden, root, edgePos);
	}

	// Function to add all the suffix in the tree.
	void addSuffix(string suffix, int index)
	{
		//In the case we use the $ sign, we must consider the next line
		suffix += "$";
		int edgePos = -1; //Edge position
		string parteCoinciden = "";
		//The search give us inside suffix the remained part to add in the tree.
		Node *node = search(suffix, parteCoinciden, edgePos, index);

		if (edgePos == -1) // If not exist the appropiate edge, we add a new element to the node.
		{
			node->edges.push_back(Edge(node, suffix, index));
		}
		else
		{
			string coinciden = parteCoinciden;
			string noCoinciden = suffix;
			string noCoincidenEdge = node->edges[edgePos].suffixString.substr(coinciden.size());
			//cout << "coincide:" << coinciden <<"- restoSuf:" << noCoinciden << "- resto edge:" << noCoincidenEdge << "-" << endl;

			node->edges[edgePos].suffixString = coinciden;

			//When we need to add a new node and when there is nothing to add from the suffix word.
			if (node->edges[edgePos].endNode == NULL && (noCoinciden.size()>0 || noCoincidenEdge.size()>0))
			{
				Node * newNode = new Node();
				node->edges[edgePos].endNode = newNode;

				if (noCoinciden.size()>0)
					newNode->edges.push_back(Edge(newNode, noCoinciden, index));

				if (noCoincidenEdge.size()>0)
					newNode->edges.push_back(Edge(newNode, noCoincidenEdge, node->edges[edgePos]));
			}
			else
			{
				if (noCoinciden.size()>0)
					node->edges.push_back(Edge(node, noCoinciden, index));
			}
			/*
			//In case all the suffix coincide in the tree (we must not to add again).
			if (noCoinciden.size() == 0)
			{
                //if the suffix is from a different text (push_back)

                if (node->edges[edgePos].indexes[node->edges[edgePos].indexes.size() - 1] != index)
                {
				    node->edges[edgePos].putOtherIndex(index);
                }
                else //If the suffix is from the same text, add coincidence(+1).
                {
                   	node->edges[edgePos].addCantCoicidencia();
                }
			}
			*/
		}		
		//system("pause");
	}

	void print(Node *node, int profundidad)
	{
		string aux = "";
		string indexs;
		for (int i = 0; i<profundidad; i++)
			aux += "\t";
		for (int i = 0; i<node->edges.size(); i++)
		{
			indexs = "";
			for (int j = 0; j <node->edges[i].indexes.size(); j++)
				indexs += "-" + to_string(node->edges[i].indexes[j]);
			indexs += "-";

			cout << aux << "suffix: -" << node->edges[i].suffixString << "- \t indexs: " << indexs << endl;
			if (node->edges[i].endNode != NULL)
			{
				print(node->edges[i].endNode, profundidad + 1);
			}
		}
	}

	void print()
	{
		cout << "--------- Impresion del Arbol ---------" << endl;
		print(root, 2);
		cout << endl;
	}

	friend class MiniGoogle;
};


class MiniGoogle
{
private:
	int cantTree; // Number of trees to use
	vector<SuffixTree> trees; // Vector that store all the trees
	//SuffixTree tress;
    map<int, string> documentTitles; // Map that stores index, titles.
    int idTree;

public:
	MiniGoogle(int cantTree)
	{
		this->cantTree = cantTree;
        this->idTree = 0;
		for (int i = 0; i < cantTree; i++)
		{
			trees.push_back(SuffixTree());
		}

	}

    //Inserting all the documents. posTree: tree which have the document, id: document id, dbindex(using it), title, content
	void putDocument(int posTree, int id, int dbindex, string title, string content)
	{
		trees[posTree].execution(title, dbindex);
		trees[posTree].execution(content, dbindex);
		//cout<<title<<endl;
       	documentTitles[dbindex] = title;
	}

    void putDocument(int id, int dbindex, string title, string content)
    {
        putDocument(idTree, id, dbindex, title, content);
        idTree++;
        if(cantTree <= idTree)
            idTree = 0;
        //print();
    }
	void buscarIndicesHojas(Edge edge, map<int, int> &result)
	{
		map<int, int>::iterator it;
		for (int j = 0; j < edge.indexes.size(); j++)
		{
			result[edge.indexes[j]] = edge.coincidences[j];
				/*
				it = result.find(edge.indexes[j]);
				if (it != result.end()){
					cout<<"+++"<<endl;
					(*it).second = (*it).second + edge.coincidences[j];
				}
				else {
					result[edge.indexes[j]] = edge.coincidences[j];
				}
				*/
		
		}
	}
	template<typename K, typename V>
	std::multimap<V,K> invertMap(std::map<K,V> const &map)
	{
		std::multimap<V,K> multimap;

		for (auto const &pair: map) {
			multimap.insert(std::make_pair(pair.second, pair.first));
		}

		return multimap;
	}

    //Function to return a map with the keys and values; where the key is the index of the text and the value is the
    //coincidence number in the tree.
	map<int, int> search(string texto)
	{
		map<int, int> result;//key, value
		for (int i = 0; i < cantTree; i++)
		{
			int edgePos = -1;
			string parteCoinciden = "";
			string suffix = texto;
			Node *node = trees[i].search2(suffix, parteCoinciden, edgePos);

			
			//cout << "suff:" << suffix << " coicide:" << parteCoinciden << endl;

            if (edgePos != -1 && suffix.size()==0)
			{
				buscarIndicesHojas(node->edges[edgePos], result);
			}
		}

		return result;
	}

	void print()
	{
		cout << "================================ Imprimiendo Arboles ================================" << endl;
		for (int i = 0; i<trees.size(); i++)
		{
			cout << "--------------- Arbol " << to_string(i) << "----------------" << endl;
			trees[i].print();
		}
		cout << endl;
	}

	bool to20blocks(int nblocks){//, ofstream &doc){
		char conf;
		if (nblocks%20==0){
			cout<<"\n\nVer mas(y/n) | Mostrar Documento(D/d)? ";
			cin>>conf;

			if ((conf == 'y') || (conf == 'Y')){
				return true;
			}
			if ((conf == 'd') || (conf == 'D')){
				int docId;
				cout<<"\nDocument(DbIndex) -> ";
				cin>>docId;
				cout<<endl;
				cout<<mapContent.find(docId)->second.titulo<<endl;
				cout<<"=================================================================================================\n"<<endl;
				cout<<mapContent.find(docId)->second.contenido<<endl;
				cout<<"\n\nVer más artículos (Y/N)";
				cin>>conf;
				if ((conf == 'y') || (conf == 'Y')){
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
		else {
			return true;
		}
	}

	void printResult2(map<int, int> result, bool MODE)//, vector<string> resultTitles)
	{
        int i=0;
		std::multimap<int,int>::reverse_iterator it;
        if (MODE == true){
        	printf("\nResultados: %d\n\n", (int)result.size());
			for (it=result.rbegin(); it!=result.rend(); ++it){
				//printf("[%2d] dbIndex: %8d | cnds: %8d | ", i+1, it->first, it->second);
				printf("[DbIndex = %6d] ", it->first);
				cout<<mapContent.find(it->first)->second.titulo<<endl;
				//cout<<documentTitles[it->first]<<endl;
				i++;

				bool contin = to20blocks(i);//, archivo);
				if (contin != true){		
					break;
				}
			}
			char conf2;
			cout<<"\nsalir(y/n)/document(D/d)? ";
			cin>>conf2;

			if ((conf2 == 'd') || (conf2 == 'D')){
				int docId;
				cout<<"\nDocument(DbIndex) -> ";
				cin>>docId;
				cout<<endl;
				cout<<mapContent.find(docId)->second.titulo<<endl;
				cout<<"=================================================================================================\n"<<endl;
				cout<<mapContent.find(docId)->second.contenido<<endl;		
			} 

        } else {
			ofstream archivo;
			archivo.open("core/toapp.txt", ios::binary);
			if(archivo.fail()){
				cout<<"no se pudo abrir el archivo";
				exit(1);
			}

        	for (it=result.rbegin(); it!=result.rend(); ++it){
				//printf("[%2d] dbIndex: %8d | cnds: %8d | ", i+1, it->first, it->second);
				//cout<<"titulo : "<< documentTitles[it->first]<<endl;
				i++;
        		/*archivo <<"[";
        		archivo << i ;
        		archivo <<"] ";
        		archivo << documentTitles[it->first];*/
        		//archivo <<"[";
        		archivo << it->first;
        		archivo <<"|";
        		archivo << mapContent.find(it->first)->second.titulo;
        		//archivo << documentTitles[it->first];
        		archivo <<"\n";

        	}
        	archivo.close();
        }
	}

	void printResult(multimap<int,int> result, bool MODE)//, vector<string> resultTitles)//, vector<string> resultTitles)
	{
		if (MODE == true){
			printf("\nResultados: %d\n\n", (int)result.size());

			if(result.size()==0)
	            cout << "No hay coicidencia" << endl;

	        int i=0;

			std::multimap<int,int>::reverse_iterator it;
			//for (auto const &pair: result) 
			
	  		for (it=result.rbegin(); it!=result.rend(); ++it)
			{
				//resultTitles.push_back(documentTitles[rit->second]);
				//std::cout <<"dbIndex: "<< it->second << "\t coincidencias: " << it->first << "\n";
				//printf("[%2d] dbIndex: %8d | cnds: %8d | ", i+1, it->second, it->first);
				printf("[%6d] | ", it->second);
				cout<<mapContent.find(it->second)->second.titulo<<endl;
				//cout<< documentTitles[it->second]<<endl;
				//std::cout <<"dbIndex: "<< rit->second << " \t titulo: " << resultTitles[i] << "\t coincidencias: " << rit->first << "\n";
	            i++;
	            
				bool contin = to20blocks(i);//, archivo);
				if (contin != true){
					break;
				}

			}

			char conf2;
			cout<<"\nSalir(y/n)/Documento(D/d)? ";
			cin>>conf2;
			
			if ((conf2 == 'd') || (conf2 == 'D')){
				int docId;
				cout<<"\nDocument(DbIndex) -> ";
				cin>>docId;
				cout<<endl;
				cout<<mapContent.find(docId)->second.titulo<<endl;
				cout<<"=================================================================================================\n"<<endl;
				cout<<mapContent.find(docId)->second.contenido<<endl;		
			} 			
		} 
		else {
			printf("\nResultados: %d\n\n", (int)result.size());

			if(result.size()==0)
	            cout << "No hay coicidencia" << endl;

	        int i=0;

			ofstream archivo;
			archivo.open("toapp.txt", ios::out);
			if(archivo.fail()){
				cout<<"no se pudo abrir el archivo";
				exit(1);
			}

			std::multimap<int,int>::reverse_iterator it;
			//for (auto const &pair: result) 
			
	  		for (it=result.rbegin(); it!=result.rend(); ++it)
			{
				//resultTitles.push_back(documentTitles[rit->second]);
				//std::cout <<"dbIndex: "<< it->second << "\t coincidencias: " << it->first << "\n";
				//printf("[%2d] dbIndex: %8d | cnds: %8d | ", i+1, it->second, it->first);
				printf("[%6d] | ", it->second);
				cout<<mapContent.find(it->second)->second.titulo<<endl;
				//cout<< documentTitles[it->second]<<endl;
				//std::cout <<"dbIndex: "<< rit->second << " \t titulo: " << resultTitles[i] << "\t coincidencias: " << rit->first << "\n";
	            i++;


				archivo << documentTitles[it->second];
				archivo <<"\n";
	            
				bool contin = to20blocks(i);//, archivo);
				if (contin != true){
					break;
				}

			}

			char conf2;
			cout<<"\nsalir(S/s)/document(D/d)? ";
			cin>>conf2;
			
			if ((conf2 == 'd') || (conf2 == 'D')){
				int docId;
				cout<<"\nDocument(DbIndex) -> ";
				cin>>docId;
				cout<<endl;
				cout<<mapContent.find(docId)->second.titulo<<endl;
				cout<<"=================================================================================================\n"<<endl;
				cout<<mapContent.find(docId)->second.contenido<<endl;		
			} 				
		}

	}

    //Function that return a map with the text index and the number of the coincidence
    bool find (string text, bool MODE)
    {
		cout << "\n================================ Resultado de búsqueda ( "<< text<< " ) ==============================" << endl;
		clock_t t;
		
		Word word;
		vector<string> words = word.getWords(text);
		//vector<multimap<int,int>> mapsResults;
		
		t = clock();

		if (words.size() == 1){
			map<int, int> result = search(text);
			t = clock() - t;
			vector<string> resultTitles;
			
			printf("\nBúsqueda en %.8f segundos\n", (float)t/CLOCKS_PER_SEC);
			printResult2(result, MODE);//, resultTitles);
		}
		else{

		
			vector<map<int,int>> maps;
			
			for(string t: words)
			{
				maps.push_back(search(t));
			}
			t = clock() - t;
			printf("\nBúsqueda en %.8f segundos\n", (float)t/CLOCKS_PER_SEC);

			int key, value; //key: dbindex, value: nro coincidences
			map<int, int> result;

			for(int i=0; i<maps.size(); i++)
			{
				for (map<int, int>::const_iterator it = maps[i].begin(); it != maps[i].end(); ++it)
				{
					vector<int> coincidences;
					key = it->first;
					value = it->second;
					coincidences.push_back(value); //all the coincidences for the key.

					map<int,int>::iterator itAux;
					for(int k=i+1; k<maps.size(); k++)
					{
						itAux = (maps[k]).find(key);
						
						if (itAux != maps[k].end())//si existe
						{
							value = itAux->second;
							coincidences.push_back(value);
							maps[k].erase (itAux);
						}
						else
						{
							coincidences.push_back(0);
						}
					}
					
					int min = 1000000000;
					for(int j=0; j<coincidences.size(); j++)
					{
						if(coincidences[j]<min)
							min = coincidences[j];
					}
					result[key] = min;
				}

			}

			multimap<int,int> multimap = invertMap(result);
			vector<string> resultTitles;	


			std::multimap<int,int>::reverse_iterator rit;
	  		for (rit=multimap.rbegin(); rit!=multimap.rend(); ++rit)
			{
	    		//std::cout << rit->first << " => " << rit->second << '\n';
				resultTitles.push_back(documentTitles[rit->second]);
			}

			printResult(multimap, MODE);//, resultTitles);
	     	//New <---------------------------------------------------------------
     	}
    }

    string get_contenido(int dbIndex)
    {
    	return mapContent.find(dbIndex)->second.contenido;
    }

	friend class Node;
	friend class SuffixTree;

};

string recorrer(string s,int  pos){
    string resultado;

    s=s.substr(pos+1);int i;
    for(i=1;i<s.length();i++){
        if(s[i] == '|'){
            break;
        } 
    }   
    resultado=s.substr(0,i);

    return resultado;
}


void MakeSuffixTree(MiniGoogle *mg){
  
    //--------------Lectura de data------------------------
    string frase;
    int i=0;
    cargarTexto();

    string textInFile;
    int dbindex;
    string title;
    string contenido;

    int posend,posid,postittle,posnfil,posdbindex;
    string ssid,ssttitle,ssnfil,ssdbindex,sscontenido;

    ifstream ficheroEntrada;
    ficheroEntrada.open ("core/parse/raw.txt");
    if (ficheroEntrada.is_open())
    { 
        cout << "\nLectura correcta" << endl;
        cout << "============================================================\n" << endl;

        while(!ficheroEntrada.eof())
        {  
            //READ
            getline(ficheroEntrada, frase);
            if(frase.length() == 0)
            	continue;

            //======================Number of text to read============================================
            //if(i > 1000){
            //    break;
            //}
            //si no es cabecera ni fin asignarle  variables.

            textInFile 		= textInFile + frase;
            string sstart 	="|";
            
            posdbindex 		= textInFile.find(sstart);
			dbindex 		= stoi(recorrer(textInFile,posdbindex));


            postittle   	= textInFile.find(sstart,posdbindex+1);
            title 			= recorrer(textInFile,postittle);

            posend		 	= textInFile.find(sstart,postittle+1);
            contenido		= recorrer(textInFile,posend+1);

            mg->putDocument(0, dbindex, title, contenido);

            textInFile = "";
            i++;
            if (i%10==0){
            	printf("\r\tSuffix-Tree <- %3.2f %%", 100*(float)i/259409.0);
            }

            
        }
    }
    else
    {
        cout << "¡Error! El archivo no pudo ser abierto." << endl;
    }
    ficheroEntrada.close();
    cout<<endl;	
}