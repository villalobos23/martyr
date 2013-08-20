#ifndef Clase_H_
#define Clase_H_
#include "evaluate.h"
#include "Tabla.h"

/*
Clase.h especifica la estructura y funciones que puede tener una Clase
*/

class Clase{
	private:
		string nombre; //Nombre de la clase
		int num_padres; //Numero de ancestros de la clase
		int num_hijos; //Numero de descendientes de la clase
		int num_propiedades; //Numero de propiedades de la clase
		int num_instancias; //Numero de instancias de la clase
		list<Clase> padres; //Lista de ancestros de la clase
		Clase *( hijos[C] );//Lista de descendientes de la clase
		Cuadro *propi; //Tabla de Hash con las propiedades de la clase, Nombre y Posicion en el arreglo principal de OA.h
		Cuadro *instancias; //Tabla de Hash con las instancias que perteneces a la clase, con Nombre y Posicion en el arreglo principal de OA.h
		struct vartable *vt;//Tabla de propiedades que pertenecen a la clase, se tiene para poder comprobar expresiones, las propiedades en esta tabla no tienen valor alguno, solo se usa para comprobar errores de sintaxis en las expresiones 
		struct val x; //Al añadir una variable a la tabla de variables se necesita obligatoriamente un valor, como solo nos interesa comprobar la expresion, siempre mandamos el valor x...
		
	public:
		//Constructor
		Clase();
		Clase(string nom); 
		
		//Destructor
		~Clase();
		
		//Consultores
		string get_nombre();
		Clase* get_padre(int n);
		Clase* get_hijo(int n);
		Cuadro* get_propiedades();//Tiene el nombre de la propiedad y la posicion en la tabla general en la clase OA menos 1
		Cuadro* get_instancias();//Tiene el nombre de la instancia (sin el "_" + nom_clase) y la posicion en la tabla general en la clase OA menos 1
		int get_num_padres();
		int get_num_hijos();
		int get_num_propiedades();
		int get_num_instancias();

		//Comprobar existencia
		bool existe_propiedad(string p);
		bool existe_instancia(string i);
		bool existe_hijo(string h);
		int comprobar_expresion(string expresion, string variables[V], int *cant_var);

		//Adiciones
		void agregar_padre(Clase *nuevo_padre);
		void agregar_hijo(Clase *nuevo_hijo);
		void agregar_propiedad(string n_p, int posicion);
		void agregar_instancia(string n_i, int posicion);
};

	//Constructor basico
 	Clase::Clase()
	{
	}

	//Constructor
 	Clase::Clase(string nom){
		nombre = nom;
		num_padres = 0;
		num_hijos = 0;
		num_propiedades = 0;
		num_instancias = 0;
		propi = new Cuadro();
		instancias = new Cuadro();
		vt = create_vartable();
		x.type = T_INT; x.ival = 0; //Al agregar una propiedad se debe proporcionar una direccion obligatoria con un valor, para eso se usa la variable x, debido a que no necesitamos valores reales siempre usaremos x, ya que solo necesitamos saber si la sintaxis de la expresion es correcta.
	}

	Clase::~Clase(){
		//Destructor
	}

	//Consultar nombre de la clase
	string Clase::get_nombre(){
		return nombre;
	}

	//Consultar un ancestro de la clase
	Clase* Clase::get_padre(int n){
		int i = 0;
		list<Clase>::iterator pos;
		pos = padres.begin();
		while( i < n && pos != padres.end())
		{
			pos++;
			i++;
		}
		return &(*pos);
	}

	//Consulto un descendiente de la clase
	Clase* Clase::get_hijo(int n){
		return hijos[n];
	}

	//Consultar la tabla de Hash con las propiedades de la clase
	map<string, int>* Clase::get_propiedades(){
		return propi;
	}

	//Consultar la tabla de Hash con las instancias de la clase
	map<string, int>* Clase::get_instancias(){
		return instancias;
	}

	//Consultar numero de ancestros de la clase
	int Clase::get_num_padres(){
		return num_padres;
	}

	//Consultar numero de descendientes de la clase
	int Clase::get_num_hijos(){
		return num_hijos;
	}

	//Consultar numero de propiedades de la clase
	int Clase::get_num_propiedades(){
		return num_propiedades;
	}

	int Clase::get_num_instancias(){
		return num_instancias;
	}

	//Saber existencia de una propiedad dentro de la clase
	bool Clase::existe_propiedad(string p){
		int nume = (*propi).find(p)->second ;
		return (  (nume-1) != -1 );
	}

	//Saber existencia de una instancia dentro de la clase
	bool Clase::existe_instancia(string i){
		int nume = (*instancias).find(i)->second ;
		return (  (nume-1) != -1 );
	}

	bool Clase::existe_hijo(string h){
		int j;
		bool band = true;
		j = 0;
		while(j < num_hijos && band){
			if( hijos[j]->get_nombre() == h ){
				band = false;
			}
			j++;
		}
		return band;//Si retornar verdad, no existe el hijo
	}

	//Comprobar la sintaxis de una expresion de un evento
	int Clase::comprobar_expresion(string expresion, string variables[V], int *cant_var){
		struct val result;
		return evaluate(StringAChar(expresion), &result, vt, variables, cant_var);
	}

	//Agregar un ancestro a la clase
	void Clase::agregar_padre(Clase *nuevo_padre){
	//Ver primero si el padre a conectar existe 
	
		padres.push_back( *nuevo_padre ); 
		num_padres+=1;
	

	}

	//Agregar un descendiente a la clase
	void Clase::agregar_hijo(Clase *nuevo_hijo){
	//Ver primero si el padre a conectar existe 
	
		if( existe_hijo(nuevo_hijo->get_nombre()) ){
			hijos[num_hijos] = nuevo_hijo;
			num_hijos+=1;
		}	
	}
	
	void Clase::agregar_propiedad(string n_p, int posicion){
		
		put_var(vt, StringAChar(n_p), &x);
		propi->insert(Cuadro::value_type(n_p, posicion + 1) );//le sumo 1, porque cuando la posicion es 0, y la propiedad no exista, devolvera 0 igual, y se presta para confusiones
		num_propiedades+=1;
	
	}
	
	void Clase::agregar_instancia(string n_i, int posicion){
		instancias->insert( Cuadro::value_type(n_i, posicion + 1) );//le sumo 1, porque cuando la posicion es 0, y la propiedad no exista, devolvera 0 igual, y se presta para confusiones
		num_instancias+=1;
	}

# endif
