#include<iostream>
#include<vector>
#include<string>
using namespace std;

class Edge;

class Node
{
    private:
        vector<Edge> edges;
        
    friend class Edge;
    friend class SuffixTree;
};

class Edge 
{
    private:
        string suffixString;
        Node *startNode;
        Node *endNode;
        int index;
    
    public:
        Edge(Node *startNode, string suffixString, int index)
        {
            this->suffixString = suffixString;
            this->startNode = startNode;
            this->index = index;
            this->endNode = NULL;
        }
    
    friend class Node;
    friend class SuffixTree;

};

class Word
{
    private:
        string word;
        int index;
    public:
        Word(){}
        Word(string word, int index)
        {
            this->word = word;
            this->index = index;
        }
        string toString()
        {
            return (word + " " + to_string(index));
        }
    
    //Getting the words
    vector<Word> getWords(string text)
    {
        vector<Word> words;

        int posSpace;
        string word;
        int wordStartPos = 0;

        while(text.size() >= 1)
        {
            posSpace = text.find(" ");

            if(posSpace == -1)
            {
                words.push_back(Word(text, wordStartPos) );
                break;
            }

            word = text.substr(0, posSpace);

            if(word.size() > 0)
                words.push_back( Word(word, wordStartPos) );
            
            text = text.substr(posSpace+1);
            
            wordStartPos +=posSpace+1;
        }
        return words;
    }
    
    //Getting the suffix
    vector<Word> getSuffix(string word, int index)
    {
        vector<Word> suffix;

        int i=word.size()-1;

        for(; i>=0; i--)
        {
            suffix.push_back(Word(word.substr(i), index+i) );
        }
        return suffix;
    }
    
    friend class SuffixTree;
};

class SuffixTree
{
    private:
        Node *root;
        string text;
    
    public:
        SuffixTree(string text)
        {
            this->root = new Node();
            this->text = text;
        }

    

    

    void execution()
    {
        Word w;
        vector<Word> words = w.getWords(text);
        int i=0;
        for(; i<words.size(); i++)
        {
            //cout<<"----------------"<<endl;
            //cout<<"-"<<words[i].word<<"-"<<words[i].index<<"-"<<endl;
            
            vector<Word> suffix = w.getSuffix(words[i].word, words[i].index);
            
            for(int j=0; j<suffix.size(); j++)
            {
                //cout<<"-"<<suffix[j].word<<"-"<<suffix[j].index<<"-"<<endl;
                addSuffix(suffix[j].word, suffix[j].index);
                //print();
                //return;
            }
        }
    }

    Node * search(string &suffix, string &parteCoinciden, Node *node, int &edgePos)
    {
        if(node->edges.size() == 0)
            return node;
        
        edgePos = -1;
        parteCoinciden = "";
        
        string substring1, substring2;
        for(int i=0; i<node->edges.size(); i++)
        {
            substring1 = suffix.substr(0,1);
            substring2 = node->edges[i].suffixString.substr(0,1);
            
            if(substring1==substring2)
            {
                parteCoinciden += substring1;
                suffix = suffix.substr(1);
                edgePos = i;
                
                int j=0, k=1;
                for(; j<suffix.size()  && k<node->edges[i].suffixString.size(); j++, k++)
                {
                    substring1 = suffix.substr(j,1);
                    substring2 = node->edges[i].suffixString.substr(k,1);
                   
                   
                    edgePos = i;
                    
                    if(substring1 != substring2)
                    {
                        break;
                    }
                    parteCoinciden += substring1;
                    
                }
                suffix = suffix.substr(j);
                
                if( (node->edges[i].endNode) != NULL)
                {
                    return search( suffix, parteCoinciden , node->edges[i].endNode, edgePos);
                }
                
                return node;
            }

        }
        
        return node;
    }

    Node * search(string &suffix, string &parteCoinciden, int &edgePos)
    {
        return search(suffix, parteCoinciden, root, edgePos);   
    }



    void addSuffix(string suffix, int index)
    {
        int edgePos = -1; //Posición de la arista
        string parteCoinciden = "";
        Node *node = search(suffix, parteCoinciden, edgePos);
        
        cout<<"-----------edPos: "<<edgePos << "\t NodeEdgesSize:";
        if(node!=NULL)
            cout<<node->edges.size();
        cout<<endl;
        
        
        if(edgePos == -1) // si no encontro arista adecuada, agregar un nuevo elemento al nodo
        {
            node->edges.push_back( Edge(node, suffix, index) );
        }
        else
        {
            string coinciden = parteCoinciden;
            string noCoinciden = suffix;
            string noCoincidenEdge = node->edges[edgePos].suffixString.substr( coinciden.size() );
            cout<<"coincide:"<<coinciden<<"- restoSuf:"<<noCoinciden<<"- resto edge:"<<noCoincidenEdge<<"-"<<endl;

            node->edges[edgePos].suffixString= coinciden;

            if(node->edges[edgePos].endNode==NULL && (noCoinciden.size()>0 || noCoincidenEdge.size()>0) )
            {
                Node * newNode = new Node();
                node->edges[edgePos].endNode = newNode;

                if(noCoinciden.size()>0)
                    newNode->edges.push_back( Edge(newNode, noCoinciden, index) );

                if(noCoincidenEdge.size()>0)
                    newNode->edges.push_back( Edge(newNode, noCoincidenEdge, node->edges[edgePos].index) );
            }
            else
            {
                if(noCoinciden.size()>0)
                    node->edges.push_back( Edge(node, noCoinciden, index) );
            }
            
        }
        
        
        print();
        string a;
        cin>>a;
    }

    void print(Node *node, int profundidad)
    {
        string aux = "";
        for(int i=0; i<profundidad; i++)
            aux += "\t" ;
        for(int i=0; i<node->edges.size(); i++)
        {
            cout<<aux<<"suffix: -"<<node->edges[i].suffixString<<"- \t index: "<<node->edges[i].index<<endl;
            if(node->edges[i].endNode != NULL)
            {
                print(node->edges[i].endNode, profundidad+1);
            }
        }
    }
    
    void print()
    {
        cout<<"----------------------Arbol ----------------------"<<endl;
        print(root, 0);
    }
};


int main()
{
    string text = "HELLO WORLD";
    //string text = "HELLO HILO";
    //string text = "HELLO LLOA";
    //string text = "HELLO WORLD LLOA LLOAE";
    //string text = "HELLO AE";
    SuffixTree *tree = new SuffixTree(text);
    tree->execution();
    
    tree->print();
    cout<<"hello"<<endl;

    
    /*Word w;
    vector<Word> su = w.getSuffix("hola como te va mi amor", 5);
    for(Word s:su)
        cout<<"--"<<s.toString()<<"--"<<endl;*/
    
}