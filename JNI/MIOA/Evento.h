#ifndef Evento_H_
#define Evento_H_
#include "Tabla.h"
#define MAX_EXP_VAL 50 //Maxima cantidad de expresiones en un mismo evento, tambien corresponde a la cantidad maxima de valores que puede alcanzar la propiedad activa, ya que cada expresion conlleva un valor.

struct valor_r {
	int ival;
	double rval;
	char *cval;
	int tipo;
};

using namespace std;

/**
La clase Evento.h contiene la estructura y funciones de un evento
**/

class Evento{

	private:
		string nombre; //Nombre del evento
		int num_expr_y_prop_activ; //Cantidad de expresiones y de valores que puede alcanzar la propiedad activa
		string prop_activa; //Nombre de la propiedad activa
		int pos_prop_activa; //Posicion de la propiedad activa en el arreglo principal de la Clase OA.h -> Propiedad *(propiedades[P])
		struct valor_r *(valores[MAX_EXP_VAL]);//Tabla de valores que puede tener la propiedad activa
		string *(exp[MAX_EXP_VAL]); //Arreglo de expresiones posibles del evento
		list<Evento> padres; //Lista de ancestros del evento
		list<Evento> hijos; //Lista de descendientes del evento
		int num_padres; //Cantidad de ancestros del evento
		int num_hijos; //Cantidad de descendientes del evento
		int tipo; //Nos dice el tipo de dato de la propiedad activa, para que cuando se agreguen nuevas expresiones y valores futuros, se verifique que sean del mismo tipo
		
		
	public:
		//Constructores
		Evento(string nom, string pA, int NuevoValor, string expresion);
		Evento(string nom, string pA, double NuevoValor, string expresion);
		Evento(string nom, string pA, string NuevoValor, string expresion);
		//Destructor		
		~Evento();
		
		//Agregar expresiones al evento
		void agregar_expresion(int NuevoValor, string expresion);
		void agregar_expresion(double NuevoValor, string expresion);
		void agregar_expresion(string NuevoValor, string expresion);
		
		//Consultas
		int get_num_padres();
		int get_num_hijos();
		int get_num_expr_y_prop_activ();
		string get_expresion(int n);
		string get_nombre();
		Evento* get_padre(int n);
		Evento* get_hijo(int n);
		string get_nombre_propiedad();
		valor_r* get_valor_nuevo(int pos);
		
		//Agregar ancestros o descendientes nuevos
		void agregar_padre(Evento *nuevo_padre);
		void agregar_hijo(Evento *nuevo_hijo);
};
 
	//Se crea el evento con valor futuro tipo ENTERO
 	Evento::Evento(string nom, string pA, int NuevoValor, string expresion){
		num_padres = 0;
		num_hijos = 0;
		num_expr_y_prop_activ = 1;
		nombre = nom;
		exp[0] = new string(expresion);
		prop_activa = pA;
		valores[0] = new valor_r();
		valores[0]->tipo = ENTERO;
		valores[0]->ival = NuevoValor;
		tipo = ENTERO;
		
	}

	//Se crea el evento con valor futuro tipo REAL
 	Evento::Evento(string nom, string pA, double NuevoValor, string expresion){
		num_padres = 0;
		num_hijos = 0;
		num_expr_y_prop_activ = 1;		
		nombre = nom;
		exp[0] = new string(expresion);
		prop_activa = pA;
		valores[0] = new valor_r();
		valores[0]->tipo = REAL;
		valores[0]->rval = NuevoValor;
		tipo = REAL;
	}
	
	//Se crea el evento con valor futuro tipo CADENA
	Evento::Evento(string nom, string pA, string NuevoValor, string expresion){
		num_padres = 0;
		num_hijos = 0;
		num_expr_y_prop_activ = 1;
		nombre = nom;
		exp[0] = new string(expresion);
		prop_activa = pA;
		valores[0] = new valor_r();
		valores[0]->tipo = CADENA;
		valores[0]->cval = StringAChar(NuevoValor);
		tipo = CADENA;
	}

	Evento::~Evento(){
		//Destructor
	}

	//Se agrega una expresion con la propiedad activa tipo ENTERO
	void Evento::agregar_expresion(int NuevoValor, string expresion){
		if( num_expr_y_prop_activ < MAX_EXP_VAL && tipo == ENTERO){
			valores[num_expr_y_prop_activ] = new valor_r();
			valores[num_expr_y_prop_activ]->tipo = ENTERO;
			valores[num_expr_y_prop_activ]->ival = NuevoValor;
			exp[num_expr_y_prop_activ] = new string(expresion);	
			num_expr_y_prop_activ+=1;
		}
		else{
			cout << "El valor futuro para la propiedad activa no coincide con el tipo de dato inicial o se excedieron el numero de expresiones para el Evento " << nombre << endl;
		}
	}

	//Se agrega una expresion con la propiedad activa tipo REAL
	void Evento::agregar_expresion(double NuevoValor, string expresion){
		if( num_expr_y_prop_activ < MAX_EXP_VAL && tipo == REAL){
			valores[num_expr_y_prop_activ] = new valor_r();
			valores[num_expr_y_prop_activ]->tipo = REAL;
			valores[num_expr_y_prop_activ]->rval = NuevoValor;
			exp[num_expr_y_prop_activ] = new string(expresion);	
			num_expr_y_prop_activ+=1;
		}
		else{
			cout << "El valor futuro para la propiedad activa no coincide con el tipo de dato inicial o se excedieron el numero de expresiones para el Evento " << nombre << endl;
		}	
	}

	//Se agrega una expresion con la propiedad activa tipo CADENA
	void Evento::agregar_expresion(string NuevoValor, string expresion){
		if( num_expr_y_prop_activ < MAX_EXP_VAL && tipo == CADENA ){
			valores[num_expr_y_prop_activ] = new valor_r();
			valores[num_expr_y_prop_activ]->tipo = CADENA;
			valores[num_expr_y_prop_activ]->cval = StringAChar(NuevoValor);
			exp[num_expr_y_prop_activ] = new string(expresion);	
			num_expr_y_prop_activ+=1;
		}
		else{
			cout << "El valor futuro para la propiedad activa no coincide con el tipo de dato inicial o se excedieron el numero de expresiones para el Evento " << nombre << endl;
		}
	}

	//Consultar numero de ancestros del evento
	int Evento::get_num_padres(){
		return num_padres;
	}

	//Consultar numero de descendientes del evento
	int Evento::get_num_hijos(){
		return num_hijos;
	}

	//Consultar numero de expresiones (o posibles valores de la propiedad activa) del evento
	int Evento::get_num_expr_y_prop_activ(){
		return num_expr_y_prop_activ;
	}
	
	//Consultar expresion en la posicion n
	string Evento::get_expresion(int n){
		if( n <  num_expr_y_prop_activ)
			return *(exp[n]);
		else
			return "";
	}
	
	//Consultar nombre del evento
	string Evento::get_nombre(){
		return nombre;
	}

	//Consultar un evento ancestro del actual
	Evento* Evento::get_padre(int n){
		int i = 0;
		list<Evento>::iterator pos;
		pos = padres.begin();
		while( i < n && pos != padres.end())
		{
			pos++;
			i+=1;
		}
		return &(*pos);
	}

	//Consultar un evento descendiente del actual
	Evento* Evento::get_hijo(int n){
		int i = 0;
		list<Evento>::iterator pos;
		pos = hijos.begin();
		while( i < n && pos != hijos.end())
		{
			pos++;
			i+=1;
		}
		return &(*pos);
	}

	//Consultar nombre de la propiedad activa del evento
	string Evento::get_nombre_propiedad(){
		return prop_activa;
	}
	
	//Consultar uno de los valores que puede tener la propiedad activa
	valor_r* Evento::get_valor_nuevo(int pos){
		return valores[pos];
	
	}
	
	void Evento::agregar_padre(Evento *nuevo_padre){
	//Ver primero si el padre a conectar existe 
		padres.push_back( *nuevo_padre ); 		
		num_padres = num_padres + 1;
	}

	void Evento::agregar_hijo(Evento *nuevo_hijo){
	//Ver primero si el padre a conectar existe 
		hijos.push_back( *nuevo_hijo ); 	
		num_hijos = num_hijos + 1;
	}


# endif
