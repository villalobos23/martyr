#ifndef Instancia_H_
#define Instancia_H_
#include "Clase.h"

/**
En esta clase se define la estructura de una instancia de una clase

**/

class Instancia{

	private:
		string nombre;//Nombre de la clase junto con el de la instancia, ejemplo: NombreInstancia_NombreClase
		string nombre_puro; //Nombre de la instancia, ejemplo: NombreInstancia
		Clase *clase; //Apuntador a la clase a la que pertenece la instancia
		struct vartable *vt; //Tabla de propiedades de la instancia, aqui se comprueba la expresion 
		struct val *(valores[P]);//Tabla de los valores de cada propiedad de la instancia
		int num_var;//Numero de propiedades con valor asignado por el usuario 
				
	public:
		//Constructor
		Instancia(string nombre_instancia, Clase *c);

		//Destructor		
		~Instancia();
		
		//Consultores
		string get_nombre();
		string get_nombre_puro();
		Clase* get_clase();
		struct var * get_valor_propiedad(string nom_propiedad);

		//Existencia
		bool existe_propiedad(string nom_propiedad);
		
		//Validacion		
		bool validar_expresion(string exp);
		
		//Agregar
		bool agregar_valor_propiedad(string propiedad, int tipo, int valor);
		bool agregar_valor_propiedad(string propiedad, int tipo, double valor);
		bool agregar_valor_propiedad(string propiedad, int tipo, string valor);
};
 
	//Constructor
 	Instancia::Instancia(string nombre_instancia, Clase *c){
 		num_var = 0;
		nombre = nombre_instancia + "_" + c->get_nombre();
 		nombre_puro = nombre_instancia;
		clase = c;
		vt = create_vartable();
	}

	//Destructor
	Instancia::~Instancia(){
	}

	//Consultar nombre de la instancia junto con el de la clase
	string Instancia::get_nombre(){
		return nombre;
	}

	//Consultar nombre de la instancia
	string Instancia::get_nombre_puro(){
		return nombre_puro;
	}

	//Consultar clase a la cual pertenece la instancia
	Clase* Instancia::get_clase(){
		return clase;
	}

	//Consultar valor de una propiedad de la instancia
	struct var * Instancia::get_valor_propiedad(string nom_propiedad){
		struct var *v;
		v = get_var(vt, StringAChar(nom_propiedad) );
	   	return v;
   	}

	//Averiguar si la instancia posee alguna propiedad dada
	bool Instancia::existe_propiedad(string nom_propiedad){
		struct var *v;
		if ((v = get_var(vt, StringAChar(nom_propiedad) ))) {
			return true;
		}
		return false;
	}

	//Saber si la expresion de una evento se cumple con los valores de las propiedades de la instancia
	bool Instancia::validar_expresion(string exp){
		bool band = false;
		struct val result;
		
		if( evaluate(StringAChar(exp), &result, vt)==0 ){//Si, no hubo errores de sintaxis
			if(result.ival == 1){//Si la expresion devuelve verdadero
				band = true;
			}
		}
		return band;
	}

	//Agrega valor a una propiedad de tipo ENTERO
	bool Instancia::agregar_valor_propiedad(string propiedad, int tipo, int valor){
		struct var *v;
		bool band = false;
				
		if( get_var(vt, StringAChar(propiedad) )  ){//Si la propiedad ya existe (ya ha obtenido su primer valor) actualizo su valor
			v = get_var(vt, StringAChar(propiedad));
 			v->val.ival = valor;		   		
			band = true;
		}
		else{
			if( clase->existe_propiedad(propiedad) ){ //Si la propiedad pertenece a la clase, se agrega valor por primera vez
				valores[num_var] = new val(); //Se crea el registro del valor
				valores[num_var]->type = tipo; //Se asigna el tipo de dato
				valores[num_var]->ival = valor; //Se guarda el valor
				put_var(vt, StringAChar(propiedad), valores[num_var]);//Se introduce el registro en la tabla de valores
				num_var = num_var + 1; //Se incrementa el contador de valores
				band = true;
			}
			else{
				cout << "La propiedad " << propiedad << " no pertenece a la clase de la instancia" << endl;
			}
		}
		return band;
	}

	//Agrega valor a una propiedad de tipo DOUBLE
	bool Instancia::agregar_valor_propiedad(string propiedad, int tipo, double valor){
		struct var *v;
		bool band = false;
				
		if( get_var(vt, StringAChar(propiedad) )  ){//Si la propiedad ya existe (ya ha obtenido su primer valor) actualizo su valor
			v = get_var(vt, StringAChar(propiedad)); //Se consulta la propiedad
 			v->val.rval = valor;//Se cambia su valor
			band = true;
		}
		else{
			if( clase->existe_propiedad(propiedad) ){ //Si la propiedad pertenece a la clase, se agrega valor por primera vez
				valores[num_var] = new val(); //Se crea el registro del valor
				valores[num_var]->type = tipo; //Se asigna el tipo de dato
				valores[num_var]->rval = valor; //Se guarda el valor
				put_var(vt, StringAChar(propiedad), valores[num_var]);//Se introduce el registro en la tabla de valores
				num_var = num_var + 1; //Se incrementa el contador de valores
				band = true;
			}
			else{
				cout << "La propiedad " << propiedad << " no pertenece a la clase de la instancia" << endl;
			}
		}
		return band;
	}
	
	//Agrega valor a una propiedad de tipo STRING
	bool Instancia::agregar_valor_propiedad(string propiedad, int tipo, string valor){
		struct var *v;
		bool band = false;

		if( get_var(vt, StringAChar(propiedad) )  ){//Si la propiedad ya existe (ya ha obtenido su primer valor) actualizo su valor
			v = get_var(vt, StringAChar(propiedad));//Se consulta la propiedad
 			(v->val).cval = StringAChar(valor);//Se cambia su valor
 			band = true;
		}
		else{
			if( clase->existe_propiedad(propiedad) ){ //Si la propiedad pertenece a la clase, se agrega valor por primera vez
				valores[num_var] = new val(); //Se crea el registro del valor
				valores[num_var]->type = tipo; //Se asigna el tipo de dato
				(valores[num_var])->cval = StringAChar(valor); //Se guarda el valor
				put_var(vt, StringAChar(propiedad), valores[num_var]);//Se introduce el registro en la tabla de valores
				num_var = num_var + 1; //Se incrementa el contador de valores
				band = true;
			}
			else{
				cout << "La propiedad " << propiedad << " no pertenece a la clase de la instancia" << endl;
			}
		}
		return band;
	}	

#endif
