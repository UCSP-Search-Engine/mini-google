#include<iostream>
#include <map>
#include<vector>
#include<string>
#include <fstream>
using namespace std;


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

	//For printing
	string toString()
	{
		return (word + " " + to_string(index));
	}

	//Getting the words. Each word has their index according its position in the text.
	/*vector<Word> getWords(string text)
	{
		vector<Word> words;

		int posSpace;
		string word;
		int wordStartPos = 0;

		while (text.size() >= 1)
		{
			posSpace = text.find(" ");

			if (posSpace == -1)
			{
				words.push_back(Word(text, wordStartPos));
				break;
			}

			word = text.substr(0, posSpace);

			if (word.size() > 0)
				words.push_back(Word(word, wordStartPos));

			text = text.substr(posSpace + 1);

			wordStartPos += posSpace + 1;
		}
		return words;
	}*/

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
	/*vector<Word> getSuffix(string word, int index)
	{
		vector<Word> suffix;

		int i = word.size() - 1;

		for (; i >= 0; i--)
		{
			suffix.push_back(Word(word.substr(i), index + i));
		}
		return suffix;
	}*/

	vector<string> getSuffix(string word)
	{
		vector<string> suffix;

		int i = word.size() - 1;

		for (; i >= 0; i--)
		{
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



	//Start point to add the words inside the tree. First we can read the text and separate each word, 
	//after that for each word we can extract the suffix in order to add them in the tree. Also we must have
    //like parameter the index (dbindex) of the text.
	void execution(string text, int index)
	{
		Word w;
		vector<string> words = w.getWords(text);
		int i = 0;
		for (; i<words.size(); i++)
		{
			//cout<<"----------------"<<endl;
			//cout<<"-"<<words[i].word<<"-"<<words[i].index<<"-"<<endl;

			vector<string> suffix = w.getSuffix(words[i]);

			for (int j = 0; j<suffix.size(); j++)
			{
				//cout<<"-"<<suffix[j].word<<"-"<<suffix[j].index<<"-"<<endl;
				addSuffix(suffix[j], index);
				//print();
				//return;
			}
		}
	}

	//This function search return the correct node where we can insert the word suffix.
	//Also it returns the edge position where the suffix has a coincidence (if it is -1 there is not a coincidence).
	//On the other side, it returns the coincidence string with respect to the position to insert.
	Node * search(string &suffix, string &parteCoinciden, Node *node, int &edgePos)
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
					return search(suffix, parteCoinciden, node->edges[i].endNode, edgePos);
				}

				return node;
			}

		}

		return node;
	}

	// Recursive search: it searches the correct node from to root.
	Node * search(string &suffix, string &parteCoinciden, int &edgePos)
	{
		return search(suffix, parteCoinciden, root, edgePos);
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
		Node *node = search(suffix, parteCoinciden, edgePos);

		//================================ START PRINTING =====================================
		/*cout << "-----------edPos: " << edgePos << "\t NodeEdgesSize:";
		if (node != NULL)
			cout << node->edges.size();
		cout << endl;*/
		//==================================== END PRINTING ===================================

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
		}


		//print();
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

			//cout << aux << "suffix: -" << node->edges[i].suffixString << "- \t indexs: " << indexs << endl;
			if (node->edges[i].endNode != NULL)
			{
				print(node->edges[i].endNode, profundidad + 1);
			}
		}
	}

	void print()
	{
		cout << "--------- Impresion del Arbol ---------" << endl;
		print(root, 0);
		cout << endl;
	}

	friend class MiniGoogle;
};


class MiniGoogle
{
private:
	int cantTree; // Number of trees to use
	vector<SuffixTree> trees; // Vector that store all the trees
    //map<int, string> documentTitles; // Map that stores index, titles.
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
        //documentTitles[dbindex] = title;
	}

    void putDocument(int id, int dbindex, string title, string content)
    {
        putDocument(idTree, id, dbindex, title, content);
        idTree++;
        if(cantTree <= idTree)
            idTree = 0;
    }

	void buscarIndicesHojas(Edge edge, map<int, int> &result)
	{
		if (edge.endNode == NULL)//Significa q llego a una arista hoja
		{
			for (int j = 0; j < edge.indexes.size(); j++)
			{
                if (result.find(edge.indexes[j]) != result.end()) //si lo contiene
                {
				    result[edge.indexes[j]] = result.find(edge.indexes[j])->second + edge.coincidences[j];
                    //result[edge.indexes[j]] = result[edge.indexes[j]] + edge.coincidences[j];
                }
                else
                {
                    result[edge.indexes[j]] = edge.coincidences[j];
                }
			}
		}
		else
		{
			for (int j = 0; j < edge.endNode->edges.size(); j++)
			{
				buscarIndicesHojas(edge.endNode->edges[j], result);
			}
		}
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

	void printResult(map<int, int> result, string texto)//, vector<string> resultTitles)
	{
		cout << "\n\n================================ Resultado de busqueda ("<< texto << ")================================\n" << endl;
		if(result.size()==0)
            cout << "No hay coicidencia" << endl;

        int i=0;
		for (map<int, int>::const_iterator it = result.begin(); it != result.end(); ++it)
		{
			//std::cout <<"dbIndex: "<< it->first << " \t titulo: " << resultTitles[i] << "\t coincidencias: " << it->second << "\n";
			printf("[%2d] dbIndex: %8d | cnds: %8d | ", i+1, it->first, it->second);
			cout<<endl;
			//cout<<"titulo: " << resultTitles[i]<<endl;
            i++;
		}
	}


    //Function that return a map with the text index and the number of the coincidence
    bool find (string texto)
    {

        map<int, int> result = search(texto);
        vector<string> resultTitles;

        printResult(result, texto);
        //for (map<int, int>::const_iterator it = result.begin(); it != result.end(); ++it)
		//{
            
        //    resultTitles.push_back(documentTitles[it->first] );
        //}

        //printResult(result, texto, resultTitles);
    }



	friend class Node;
	friend class SuffixTree;

};

string recorrer(string s,int  pos){
    string resultado;
   // cout<<"pos "<<pos<<endl;
    s=s.substr(pos+1);int i;
    for(i=1;i<s.length();i++){
        if(s[i] == '|'){
            break;
        } 
    }   
    resultado=s.substr(0,i);
    //cout<<"i "<<i<<endl;
    //cout<<"resultado recorrer "<<resultado<<endl;
    //system("PAUSE()");
    return resultado;
}

int main()
{
    /*
    //string text = "HELLO WORLD";
    string text = "HELLO HILO";
    //string text = "HELLO LLOA";
    //string text = "HELLO WORLD LLOA LLOAE";
    //string text = "HELLO AE";
    SuffixTree *tree = new SuffixTree();
    tree->execution(text, 1);

    tree->print();
    cout << "hello" << endl;
    system("pause");*/

    MiniGoogle *mg = new MiniGoogle(10);
    //mg->putDocument(0, 1, 3, "T3", "O");
    //mg->putDocument(0, 1, 4, "T4", "O OA");

    /*mg->putDocument(0, 1, 1, "T1", "HELLO LO");//En que arbol, id(nose usa), index, titulo, contenido
    mg->putDocument(0, 1, 2, "T2", "HILO");
    mg->putDocument(1, 1, 3, "T3", "HELLO LO");
    mg->putDocument(1, 1, 4, "T4", "HELLO WORLD LLOA LLOAE");
    mg->print();

    
    cout<<"Busqueda"<<endl;
    //mg->find("Lo");
    */

    
    //--------------Lectura de data------------------------
    string frase;
    int i=0;
    //vector <string> contenido(999999);
    //vector <int> id(999999);
    //vector <int> dbindex(999999);
    //vector <string> title(999999);
    //vector <string> nonfiltered(999999);
    //vector <string> textInFile(999999);

    string textInFile;
    int dbindex;
    string title;
    string contenido;

    int posend,posid,postittle,posnfil,posdbindex;
    string ssid,ssttitle,ssnfil,ssdbindex,sscontenido;

    ifstream ficheroEntrada;
    ficheroEntrada.open ("parse/raw.txt");
    if (ficheroEntrada.is_open())
    { 
        cout << "Lectura correcta\n" << endl;
        cout << "================\n\n" << endl;

        while(!ficheroEntrada.eof())
        {  
            //READ
            getline(ficheroEntrada, frase);
            if(frase.length() == 0)
            	continue;

            //======================Number of text to read============================================
            //if(i > 40){
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
            contenido	= textInFile.substr(posend);

            /*	
            textInFile[i] = textInFile[i] + frase;
            string sstart="|";
		        
		    posdbindex      =   textInFile[i].find(sstart);
		    dbindex[i]      =   stoi(recorrer(textInFile[i],posdbindex));

		    postittle       =   textInFile[i].find(sstart,posdbindex+1);
		    title[i]        =   recorrer(textInFile[i],postittle);

		    posend          =   textInFile[i].find(sstart,postittle+1);
		    contenido[i]    =   textInFile[i].substr(posend);
			
			*/
                // system("PAUSE()");
                //EJECUCIOON DE SUFIX TREE
                // SuffixTree *tree = new SuffixTree(textInFile[i]);
                //tree->execution();    
                //tree->print();
            //mg->putDocument(id[i], dbindex[i], title[i], contenido[i]);
            //mg->putDocument(0, dbindex[i], title[i], contenido[i]);

            //cout<<"|"<<dbindex<<"|"<<title<<"|"<<endl;
            mg->putDocument(0, dbindex, title, contenido);
            textInFile = "";
            i++;

            printf("\rSuffix-Tree <- %3.2f\%", 100*(float)i/400000.0);
        }
    }
    else
    {
        cout << "Â¡Error! El archivo no pudo ser abierto." << endl;
    }
    ficheroEntrada.close();
    
    cout<<"\n\nBusqueda:\n"<<endl;
    cout<<"=========\n"<<endl;
    string word;

    while (true){
	    cout<<"Query: ";
	    cin>>word;
	    mg->find(word);
	    cout<<"\n================================ Resultado de busqueda ("<< word << ")================================\n"<<endl;   	
    }

    
    //system("pause"); 

}