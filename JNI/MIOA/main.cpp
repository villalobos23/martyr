#include <regex.h>
#include <vector>
#include <cctype>
#include "OA.h"

#include <new>
using std::set_new_handler;
#include <cstdlib>
using std::abort;

void mensaje_error(){
	cerr << "Error de memoria, bad_alloc";
	abort();
}

//Funcion que recibe un string, y elimina los espacios en blanco del mismo
string Trim(string cadena)
{
	string::const_iterator i = cadena.begin(); 
	string::const_iterator f = cadena.end();
	
     string::const_iterator it;
     string buff;
     for (it = i; it != f; ++it)
     {
         if (*it != ' ') buff += *it;
     }

     return buff;    
}

string StringToUpper(string myString)
{
  const int length = myString.length();
  for(int i=0; i!=length ; ++i)
  {
    myString[i] = toupper(myString[i]);
  }
  return myString;
}

int main(){


	set_new_handler( mensaje_error ); //Llama al metodo cuando hay una sobrecarga de memoria, mensaje personalizado

	typedef int T;
	int i, j, n, cont;
	string c, in, h1, h2;
	Clase *x;
	Instancia *is;
	string buffer;
	 
	OA *oa_bd = new OA();



	cout << "oa_bd->crear_clase( huracan )" << endl;
		cout << oa_bd->crear_clase( "huracan" ) << endl<< endl;
	cout << "oa_bd->crear_clase( fenMeteorologico )" << endl;
		cout << oa_bd->crear_clase( "fenMeteorologico" )<< endl<< endl;
	cout << "oa_bd->crear_clase( fenClimatico )" << endl;
		cout << oa_bd->crear_clase( "fenClimatico" )<< endl<< endl;
	

	cout << "oa_bd->crear_propiedad(fenClimatico, NumMuertos, ENTERO)" << endl;
		cout << oa_bd->crear_propiedad("fenClimatico", "NumMuertos", ENTERO)<< endl<< endl;	
	cout << "oa_bd->crear_propiedad(fenClimatico, HuboPerdidaHumana, CADENA)" << endl;	
		cout << oa_bd->crear_propiedad("fenClimatico", "HuboPerdidaHumana", CADENA)<< endl<< endl;
	cout << "oa_bd->crear_propiedad(huracan, intensidad, ENTERO)" << endl;
		cout << oa_bd->crear_propiedad( "huracan", "intensidad", ENTERO )<< endl<< endl;
	cout << "oa_bd->crear_propiedad(huracan, categoria, ENTERO)" << endl;
		cout << oa_bd->crear_propiedad( "huracan", "categoria", ENTERO )<< endl<< endl; //Propiedad Padre
	
	cout << "oa_bd->agregar_subpropiedad(intensidad, categoria)" << endl;
		cout << oa_bd->agregar_subpropiedad("intensidad", "categoria") << endl<< endl;

	cout << "oa_bd->agregar_subclase(huracan, fenMeteorologico)" << endl;
		cout << oa_bd->agregar_subclase("huracan", "fenMeteorologico") << endl<< endl;
	cout << "oa_bd->agregar_subclase(huracan, fenClimatico)" << endl;
		cout << oa_bd->agregar_subclase("huracan", "fenClimatico") << endl<< endl;
	

	cout << "oa_bd->crear_instancia( ike, huracan )" << endl;
		cout << oa_bd->crear_instancia( "ike", "huracan" ) << endl<< endl;
	cout << "oa_bd->crear_instancia( desastreVargas, fenClimatico )" << endl;	
		cout << oa_bd->crear_instancia( "desastreVargas", "fenClimatico" ) << endl<< endl;
	

	cout << "oa_bd->crear_evento(fenClimatico, HuboMuerte, HuboPerdidaHumana, Si,NumMuertos > 0)" << endl;	
		cout << oa_bd->crear_evento("fenClimatico", "HuboMuerte", "HuboPerdidaHumana", "Si","NumMuertos > 0") << endl<< endl;

	
	cout << "oa_bd->consultar_propiedad_instancia(huracan, ike, NumMuertos)" << endl;
		cout << oa_bd->consultar_propiedad_instancia("huracan", "ike", "NumMuertos") << endl<< endl;
	cout << "oa_bd->consultar_propiedad_instancia(huracan, ike, HuboPerdidaHumana)" << endl;
		cout << oa_bd->consultar_propiedad_instancia("huracan", "ike", "HuboPerdidaHumana") << endl<< endl;
	cout << "oa_bd->consultar_propiedad_instancia(huracan, ike, intensidad)" << endl;
		cout << oa_bd->consultar_propiedad_instancia("huracan", "ike", "intensidad") << endl<< endl;
	cout << "oa_bd->consultar_propiedad_instancia(huracan, ike, categoria)" << endl;
		cout << oa_bd->consultar_propiedad_instancia("huracan", "ike", "categoria") << endl<< endl;

	cout << "oa_bd->agregar_valorApropiedad(huracan, ike, NumMuertos, 125)" << endl;
		cout << oa_bd->agregar_valorApropiedad("huracan", "ike", "NumMuertos", 125)<< endl<< endl;
	cout << "oa_bd->agregar_valorApropiedad(huracan, ike, categoria, 4)" << endl;
		cout << oa_bd->agregar_valorApropiedad("huracan", "ike", "intensidad", 4)<< endl<< endl;
	
	cout << "oa_bd->activar_eventos(huracan, HuboMuerte)" << endl;
		cout << oa_bd->activar_eventos("huracan", "HuboMuerte") << endl;


	cout << "oa_bd->consultar_propiedad_instancia(huracan, ike, NumMuertos)" << endl;
		cout << oa_bd->consultar_propiedad_instancia("huracan", "ike", "NumMuertos") << endl<< endl;
	cout << "oa_bd->consultar_propiedad_instancia(huracan, ike, HuboPerdidaHumana)" << endl;
		cout << oa_bd->consultar_propiedad_instancia("huracan", "ike", "HuboPerdidaHumana") << endl<< endl;
	cout << "oa_bd->consultar_propiedad_instancia(huracan, ike, intensidad)" << endl;
		cout << oa_bd->consultar_propiedad_instancia("huracan", "ike", "intensidad") << endl<< endl;
	cout << "oa_bd->consultar_propiedad_instancia(huracan, ike, categoria)" << endl;
		cout << oa_bd->consultar_propiedad_instancia("huracan", "ike", "categoria") << endl<< endl;
	
	cout << "oa_bd->es_subpropiedad_de(intensidad, categoria)" << endl;
		cout << oa_bd->es_subpropiedad_de("intensidad", "categoria") << endl<< endl;


	cout << "oa_bd->pertenece_a(ike, huracan)" << endl;
		cout << oa_bd->pertenece_a("ike", "huracan") << endl<< endl;
	cout << "oa_bd->pertenece_a(ike, fenNatural)" << endl;
		cout << oa_bd->pertenece_a("ike", "fenNatural") << endl<< endl;

		





		/**  Se hace la lectura por archivos de las consultas reactivas:
		 *  Se verifica la correctitud de la sintaxis
		 *  No deben haber eventos repetidos
		 *  Se comprueba la existencia de la clase donde se activara el o los eventos
		 *  
		*/

			ifstream entrada("consultas_reactivas.in");
			int reti;
			regex_t regex; //Variable para la expresion regular
			string exp;
			char * pattern ="^\\((S|s|p|P),\\[([A-Za-z0-9])+(([,])([A-Za-z0-9])+)*\\],([A-Za-z0-9])+\\)$"; //Patron de la expresion regular

			/**
			^ Indica el inicio de la frase
			\\(   Agrega el parentesis
			(S|s|p|P) debe haber cualquiera de las letras que esta entre parentesis
			, Debe haber siempre una coma
			\\[ Agrega el corchete
			([A-Za-z0-9])+ 1 o mas palabras que contengan letras y numeros
			(([,])([A-Za-z0-9])+)* expresion formada por una coma seguido y 1 palabra que contenga letras y numeros (Puede haber 0 o mas expresiones)
			\\] Agrega corchete
			, Debe haber siempre una coma
			([A-Za-z0-9])+ 1 o mas palabras que contengas letras y numeros
			\\) Agrega el parentesis
			$ indica el final de la frase
			*/

				reti = regcomp (&regex, pattern, REG_EXTENDED);//Compilo el patrón
				while( getline (entrada,exp) ){//Leo una linea del archivo
					exp = Trim(exp);//Quitar espacios de la expresion
					cout << exp << " " << endl;
					reti = regexec (&regex, &exp[0], (size_t)0, NULL, 0); //Se verifica que la consulta reactiva cumpla con el patrón
					if( reti == REG_NOMATCH ){ //Error de sintaxis en la consulta reactiva
						puts("Error de sintaxis");
					}
					else{ //Si es 0, entonces la consulta reactiva cumple con el patrón
		
						// Se obtienen los eventos dentro del corchete
						int    status;
						regex_t    re;
						regmatch_t rm;
						char *patter = "\\[([A-Za-z0-9])+(([,])([A-Za-z0-9])+)*\\]";
						string eventos, tipo, tabla;
						int num_eventos;
						map<string,string> *eventosHash;//Tabla Hash con los eventos de la consulta-reactiva
						string eventoArreglo[100];						
						string eve;
						bool band;
			
						eventosHash = new map<string, string>();
						
						//Se compila la expresión regular y se verifica si tiene algún error
						if (regcomp(&re, patter, REG_EXTENDED) != 0) {
							cout << "Patrón erróneo";
						}
		
						status = regexec(&re, &exp[0], 1, &rm, 0); //Se verifica que el grupo de eventos de la consulta reactiva cumpla con el patrón
						regfree(&re); //Libera la memoria asignada por regcomp()
		
						if (status != 0) { //Si el estado no devuelve 0, entonces no cumple el patrón
							cout << "No coincide";
							cout << status << endl;
						}
						else{
							// Extraigo el rango de los eventos
							eventos = exp.substr( (int)rm.rm_so, (int)rm.rm_eo - 3);
							tipo = exp.substr(1, (int)rm.rm_so - 2); //Tipo de ejecucion de ventos
							tabla = exp.substr((int)rm.rm_eo + 1 , exp.size() ); //Clase donde se ejecutaran los eventos
							tabla.erase(tabla.size() - 1 , 2);
							eventos = eventos.substr( 1, eventos.size());
							eventos[ eventos.size()-1 ] = ',';
						}
			
						cout << "Modo de activacion: "<< tipo<< ", Clase: " << tabla << endl;
						// Se obtienen los nombres de los eventos y se verifica que no hayan repetidos
						num_eventos = 0;
						band = true;
			
						if( StringToUpper( tipo ) == "P"){ //Se verifica si es paralelo para no permitir eventos repetidos
							do{
								int f;
								f = eventos.find(","); //Busco la posicion de la coma mas cercana
								eve = eventos.substr(0, f); //Extraigo un evento de la lista
				
								if( (*eventosHash)[eve] != ""  ){//Reviso si es evento es repetido
									band = false;
								}
								else{
									(*eventosHash)[eve] = eve; //Guardo el evento en la tabla hash
									eventoArreglo[num_eventos] = eve;
									num_eventos+=1; //Contador de eventos
									eventos = eventos.substr( f+1, eventos.size()-1);//Elimino el evento que se agrego de la cadena original
								}
							}while( eventos.size() > 1 && band );
						}
						if(!band){
							cout << "Hay uno o más eventos repetidos" << endl;
						}
						else{
							//Verificar que existen los eventos y la clase en donde se activaran		
							if( oa_bd->existencia_consulta_reactiva(eventoArreglo, num_eventos, tabla) &&
								oa_bd->organizar_eventos(eventoArreglo, num_eventos, tabla) ){
								cout << "Se activa los eventos! " << endl;
								oa_bd->activar_eventos(tabla,eve);
							}
						}
			
					}
					cout << endl;		 
				}
				regfree (&regex);
				entrada.close();
			
	return EXIT_SUCCESS;
}
