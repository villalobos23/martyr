#ifndef Propiedad_H_
#define Propiedad_H_
#include "Clase.h"
#include "Evento.h"

class Propiedad{

	private:
		
		string nombre;//Nombre de la propiedad
		int num_clases; //Numero de clases de la propiedad
		int num_padres; //Numero de ancestro de la propiedad
		int num_hijos; //Numero de descendientes de la propiedad
		int tipo; //Tipo de dato
		list<Clase> clases_a_la_que_pertenece; //Clases que poseen la propiedad
		Propiedad *(prop_padres[P]);//Ancestros de la propiedad
		Propiedad *(prop_hijas[P]);//Descendientes de la propiedad
		
	public:
		//Constructor
		Propiedad(string n, Clase *c, int t);
		
		//Destructor		
		~Propiedad();
	
		//Consultar
		int get_num_padres();
		int get_num_hijos();
		string get_nombre();
		Propiedad* get_padre(int n);
		Propiedad* get_hijo(int n);
		bool existe_hijo(string h);
		bool existe_padre(string h);
		int get_tipo();

		//Agregar
		void agregar_padre(Propiedad *nuevo_padre);
		void agregar_hijo(Propiedad *nuevo_hijo);

};
 
 	Propiedad::Propiedad(string n, Clase *c, int t){
		nombre = n;
		num_clases = 1;
		num_padres = 0;
		num_hijos = 0;
		tipo = t;
		clases_a_la_que_pertenece.push_back( *c );//Agregar la clase original
				
		//Agregar la propiedad a todas las clases descendientes de la clase original
		int n_hijos_clase = c->get_num_hijos();
		for(int i = 0; i < n_hijos_clase ; i++){
			clases_a_la_que_pertenece.push_back( *(c->get_hijo(i)) );
			num_clases+=1;
		}
	}

	//Destructor
	Propiedad::~Propiedad(){
	}
	
	//Consultar numero de ancestros
	int Propiedad::get_num_padres(){
		return num_padres;
	}

	//Consultar numero de descendientes
	int Propiedad::get_num_hijos(){
		return num_hijos;
	}

	//Consultar nombre de la propiedad
	string Propiedad::get_nombre(){
		return nombre;
	}
	
	//Consultar ancestro de la propiedad
	Propiedad* Propiedad::get_padre(int n){
		return prop_padres[n];
	}

	//Consultar descendiente de la propiedad
	Propiedad* Propiedad::get_hijo(int n){
		return prop_hijas[n];
	}

	//Consultar tipo de dato de la propiedad
	int Propiedad::get_tipo(){
		return tipo;
	}
	
	bool Propiedad::existe_hijo(string h){
		int j;
		bool band = true;
		j = 0;
		while(j < num_hijos && band){
			if( prop_hijas[j]->get_nombre() == h ){
				band = false;
			}
			j++;
		}
		return band;//Si retornar verdad, no existe el hijo
	}

	bool Propiedad::existe_padre(string h){
		int j;
		bool band = true;
		j = 0;
		while(j < num_padres && band){
			if( prop_padres[j]->get_nombre() == h ){
				band = false;
			}
			j++;
		}
		return band;//Si retornar verdad, no existe el hijo
	}

	//Agregar ancestro a la propiedad
	void Propiedad::agregar_padre(Propiedad *nuevo_padre){
	//Ver primero si el padre a conectar existe 
		if( existe_padre( nuevo_padre->get_nombre() ) ){		
			prop_padres[num_padres] = nuevo_padre;
			num_padres+=1;
		}
	}

	//Agregar descendiente a la propiedad
	void Propiedad::agregar_hijo(Propiedad *nuevo_hijo){
	//Ver primero si el padre a conectar existe 
		if( existe_hijo( nuevo_hijo->get_nombre() ) ){
			prop_hijas[num_hijos] = nuevo_hijo;
			num_hijos+=1;
		}
	}

# endif
