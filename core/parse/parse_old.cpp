#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <string>
#include <locale.h>
#include <windows.h>

using namespace std;
 


string DelCharIndeseados(string s)
{
    const char Caracteresdeseados[] = { 
    'a', 'b', 'c', 'd', 'e','f','g','h','i','j',
    'k','l','m','n','o','p','q','r','s','t',
    'v','u','w','x','y','z','0','1','2','3',
    '4','5','6','7','8','9',' '};
    int Switch;
    for (int i = 0; i < s.length(); i++)
    {
    	//cout << "ok: " << s[i] << endl;
    	Switch = 1;
    	//system("PAUSE()"); 
        for (int j = 0; j < strlen(Caracteresdeseados); j++){
        		//cout << "uno a uno  " << s[i] << endl;
        		//cout << "uno a uno x 2 " << Caracteresdeseados[j] << endl;
            if (s[i] == Caracteresdeseados[j]){
            		Switch=0;
            		//cout << "aceptado:  " << s[i] << endl;
            		//cout << Caracteresdeseados[j] << endl;
            		//cout << j << endl;
            		//system("PAUSE()"); 
            } 
        }

       if(Switch==1){
         	s.push_back(s[i]);
         	//cout << "no es deseado: " << s[i] << endl;
            //		system("PAUSE()"); 
        }  
    }
    return s;
}

string Lower(string s){
	int len=s.length();

	for (int i = 0; i < len; ++i)
	{
		//TOlower--
		if(s[i]>='A' && s[i] <='Z'){
			s[i]=(s[i] + 32);
		}
		//Quitar tildes
		//cout << (int)s[i]<< endl;system("PAUSE()");
		if( (int)s[i] == -31 or (int)s[i]== -63) s[i] = 'a'; 
		if(	(int)s[i] == -55 or (int)s[i] == -23){ s[i] = 'e'; }
		if( (int)s[i] == -19 or (int)s[i]== -51) s[i] = 'i'; 
		if( (int)s[i] == -13 or (int)s[i]== -45 ) s[i] = 'o'; 
		if( (int)s[i] == -38 or (int)s[i]== -6 ) s[i] = 'u'; 
	}
	return s;
}

string recorrer(string s,int  pos){
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
	return resultado;
}

int main () {

 setlocale(LC_ALL, "spanish");	
 setlocale(LC_CTYPE, "Spanish");
 string url; int cont=10000;

//-------------------lectura de data 
 string frase;
 int i=0;
 vector <string> textInFile(999999);
 ifstream ficheroEntrada;

for(int k = 0;cont < 15000;k++){
	url="raw/spanishText_"+to_string(cont)+'_'+to_string(cont+5000);
	cout<<k<<"url: "<<url<<endl;
	cont=cont+5000;

 	ficheroEntrada.open (url);
 	if(ficheroEntrada.fail()){
		cout<<"no se pudo abrir el archivo";
		continue;
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
 		continue;		
 	}
 }
 //system("PAUSE()");
  ficheroEntrada.close();
}
 

//----------------------------------------------------------------
	ofstream archivo;
	archivo.open("raw.txt",ios::out);//abriendo el archivo
	if(archivo.fail()){
		cout<<"no se pudo abrir el archivo";
		exit(1);
	}
 //___________________________________________________________
int posinit,posend,posid,postittle,posnfil,posproce,posdbindex;
cout<<"i: "<<i <<endl;
int a=i;
for (int i = 0; i < a; ++i)
 {
 	string sstart,ssend,ssid,ssttitle,ssnfil,ssprocesed,ssdbindex,mensaje;
 	sstart='<doc',ssend='">',ssid='id=',ssttitle='title=',
 	ssnfil='nonfiltered=',ssprocesed='=',ssdbindex='dbindex=';

 	//textInFile[i]=DelCharIndeseados(Lower(textInFile[i]));

 	posinit=textInFile[i].find(sstart);

 	posid=textInFile[i].find(ssid,posinit);
 		ssid=recorrer(textInFile[i],posid+2);

 	postittle=textInFile[i].find(ssttitle,posid+2);
 		ssttitle=recorrer(textInFile[i],postittle+2);	

 	posnfil=textInFile[i].find(ssnfil,postittle+2);
 		ssnfil=recorrer(textInFile[i],posnfil+2);

 	posproce=textInFile[i].find(ssprocesed,posnfil+2);
 		ssprocesed=recorrer(textInFile[i],posproce+2);

 	posdbindex=textInFile[i].find(ssdbindex,posproce+2);
 		ssdbindex=recorrer(textInFile[i],posdbindex+2);	

 	posend=textInFile[i].find(ssend);

 	mensaje="|"+DelCharIndeseados(Lower(ssid))+"|"+DelCharIndeseados(Lower(ssttitle))+"|"+
 	DelCharIndeseados(Lower(ssnfil))+"|"+DelCharIndeseados(Lower(ssdbindex))+"|"+
 	DelCharIndeseados(Lower(textInFile[i].substr(posend+1)))+"\n";
	
	archivo<<"</doc>\n";	
 	archivo<<mensaje;
 	archivo<<"ENDOFARTICLE.\n";
 }
	archivo.close();
//---------------------------------------------------------------- 

 //Lectura de data	


 return 0;
}