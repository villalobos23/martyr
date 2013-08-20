#ifndef Tabla_H_
#define Tabla_H_
#include <iostream>
#include <kcstashdb.h>
#include <string.h>
#include <map>
#include <fstream>
#include <limits.h>
#include <float.h>
#include <cstdlib>
#include <list>

#define C 300 //Maximo de Clases
#define I 249000  //Maximo de Instancias
#define P 500 //Maximo de Propiedades
#define E 150 //Maximo de Eventos
#define V 100 //Maximo de propiedades dentro de una expresion y maximo de eventos en una consulta reactiva (P,[ev1,eve2,...,eve100),C]

#define ENTERO 0
#define REAL 1
#define CADENA 2

using namespace kyotocabinet;
using namespace std;

typedef std::map< string, int, std::less<string> > Cuadro; //Tipo de dato Tabla de Hash, con Key de string y Value de entero

/** En esta clase se establecen las funciones, librerias 
y variables definidas usadas a lo largo de toda la ontología, 
ademas de establecer las limitaciones **/

class Tabla{

	public:
		Tabla();
		~Tabla();
		string EnteroAString(int a);
		string DoubleAString(double a);
		char* StringAChar(string a);
};
 	//Constructor
 	Tabla::Tabla(){
	}

	//Destructor
 	Tabla::~Tabla(){
	}

	//Pasar un entero a string
	string EnteroAString(int a){
		stringstream ss;
		ss << a;
		return ss.str();
	}

	//Pasar un double a string
	string DoubleAString(double a){
		stringstream ss;
		ss << a;
		return ss.str();
	}
	
	//Pasar string a char
	char* StringAChar(string a){
		char *i,*p = &a[0];
		i = new char[strlen(p)+1];
		strcpy(i, p);
		return i;
	}

# endif
