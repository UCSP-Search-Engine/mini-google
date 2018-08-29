#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <string>
#include <locale.h>
//#include <windows.h>

using namespace std;
 

string DelCharIndeseados(string s)
{
    const char Caracteresdeseados[] = { 'a', 'b', 'c', 'd', 'e','f','g','h','i','j','k','l','m','n'
    ,'o','p','q','r','s','t','v','u','w','x','y','z','0','1','2','3','4','5','6','7','8','9','=','"','<','>'};
    int Switch;
    for (int i = 0; i < s.length(); i++)
    {
    	///cout << "ok: " << s[i] << endl;
    	Switch = 1;
    	//system("PAUSE()"); 
        for (int j = 0; j < sizeof(Caracteresdeseados); j++){

            if (s[i] == Caracteresdeseados[j]){
            		Switch=0;
            } 
        }
       if(Switch==1){
         	s[i]= '\0';
        }  
    }
    return s;
}



string Lower(string s){
	int len=s.length();

	for (int i = 0; i < len; ++i)
	{
		//TOlower
		if(s[i]>='A' && s[i] <='Z'){

			s[i]=(s[i] + 32);
		}
		//Quitar tildes
		//s[i]=Delaccented[i];	
		//cout << (char)s[i]<< endl;system("PAUSE()");
		//cout << (int)s[i]<< endl;system("PAUSE()");
		if( (int)s[i] == -31 or (int)s[i]== -63) s[i] = 'a'; 
		if(	(int)s[i] == -55 or (int)s[i] == -23){ s[i] = 'e'; }
		if( (int)s[i] == -19 or (int)s[i]== -51) s[i] = 'i'; 
		if( (int)s[i] == -13 or (int)s[i]== -45 ) s[i] = 'o'; 
		if( (int)s[i] == -38 or (int)s[i]== -6 ) s[i] = 'u'; 
	}
	return s;
}

int main () {

 setlocale(LC_ALL, "spanish");	
 setlocale(LC_CTYPE, "Spanish");
 ifstream ficheroEntrada;
 string frase;
 int i=0;
 vector <string> textInFile(999999);
 

 ficheroEntrada.open ("../../raw.es/spanishText_10000_15000");
 while(!ficheroEntrada.eof()){  

 	getline(ficheroEntrada, frase);
 	if(frase =="</doc>"){	
 		continue;
 	}
 	if(frase =="ENDOFARTICLE."){
 		i++;	
 		continue;
 	}
 	if(frase !="ENDOFARTICLE." && frase !="</doc>"){
 		textInFile[i] =textInFile[i] + frase;
 		continue;		
 	}
 	
 }
 for (int i = 0; i < 1000; ++i)
 {
 	textInFile[i]=DelCharIndeseados(Lower(textInFile[i]));
 	cout << textInFile[i] << endl;	
 }

 ficheroEntrada.close();
 return 0;
}