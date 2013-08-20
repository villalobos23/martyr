#ifndef OA_H_
#define OA_H_
#include "Instancia.h"
#include "Propiedad.h"


using namespace kyotocabinet;

class OA{

	private:
		int num_clases;//Cantidad de clases
		int num_instancias;//Cantidad de instancias
		int num_propiedades;//Cantidad de propiedades
		int num_eventos; //Cantidad de eventos
		Clase *(clases[C]); //Todas las clases existentes en la BD
		Instancia *(instancias[I]);//Todas las instancias en la BD
		Propiedad *(propiedades[P]);//Todas las propiedades de la BD
		Evento *(eventos[E]); //Todas los eventos de la BD
		StashDB *cla, *ins, *prop, *event;//Indices de las clases, instancias, propiedades y eventos
				
	public:
		//Constructor 
		OA();

		//Destructor
		~OA();

		//Crear e inicializar
		bool crear_clase(string nom_clase);
		bool inicializar_instancia(int tipo_propiedad, string nom_clase, string nom_instancia, string nom_propiedad);
		bool crear_instancia(string nom_instancia, string nom_clase);
		bool crear_propiedad(string nom_clase, string nom_propiedad, int tipo_propiedad);
		bool crear_evento(string nom_clase, string nom_evento, string nom_propActiva, int valor_futuro, string expresion);
		bool crear_evento(string nom_clase, string nom_evento, string nom_propActiva, double valor_futuro, string expresion);
		bool crear_evento(string nom_clase, string nom_evento, string nom_propActiva, string valor_futuro, string expresion);
		
		//Consultas
		Clase* get_clase(int pos); //La uso solo para pruebas
		Clase* get_clase(string nombre);//Retorna la clase con proporcionar su nombre
		Instancia* get_instancia(int pos);//La uso solo para pruebas
		Instancia* get_instancia(string clase, string nombre);
		Propiedad* get_propiedad(int pos);//La uso solo para pruebas
		Propiedad* get_propiedad(string nombre);//Retorna la propiedad con proporcionar su nombre
		int get_pos_propiedad(string nombre);
		Evento* get_evento(string nombre);//Retorna el evento con proporcionar su nombre
		int get_num_clases();//Numero de clases existentes en la base de datos
		int get_num_instancias();//Numero de instancias existentes en la base de datos
		int get_num_propiedades();//Numero de propiedades existentes en la base de datos
		int get_num_eventos();
		string consultar_propiedad_instancia(string nom_clase, string nom_instancia, string nom_propiedad);

		//Agregar
		bool agregar_subclase(string nom_clase, string nom_padre); //n_clase sera subclase de n_padre
		bool agregar_subpropiedad(string nom_propiedad, string nom_padre);
		bool agregar_subevento(string nom_evento, string nom_padre);
		
		//Verificar
		bool es_subclase_de(string nom_clase, string nom_clase_padre); //Ver si n_clase es subclase de n_clase_padre
		bool es_subpropiedad_de(string nom_propiedad, string nom_propiedad_padre);
		bool es_subevento_de(string nom_evento, string nom_evento_padre);

		//Dar valor
		bool agregar_valorApropiedad(string nom_clase, string nom_instancia, string nom_propiedad, int valor);
		bool agregar_valorApropiedad(string nom_clase, string nom_instancia, string nom_propiedad, double valor);
		bool agregar_valorApropiedad(string nom_clase, string nom_instancia, string nom_propiedad, string valor);
			
		//Otros
		bool activar_eventos(string nom_clase, string nom_evento);
		bool existencia_consulta_reactiva(string events[V], int n, string clase);
		bool pertenece_a(string nom_instancia, string nom_clase);
		bool organizar_eventos(string events[V], int n, string clase);
};
  
	
	OA::OA(){
		num_clases = 0;
		num_instancias = 0;
		num_propiedades = 0;
		num_eventos = 0;
		
		//Crear las base de datos con kyotocabinet
		cla = new StashDB();
		ins = new StashDB();
		prop = new StashDB();
		event = new StashDB();
					
		if( !cla->open("cofre.kch", StashDB::OWRITER | StashDB::OCREATE) ){
			cout << "Error al crear el cofre: " << cla->error().name() << endl;
		}
		if(!ins->open("instancias.kch", StashDB::OWRITER | StashDB::OCREATE)){
			cout << "Error al crear el cofre: " << ins->error().name() << endl;
		}
		if(!prop->open("propiedades.kch", StashDB::OWRITER | StashDB::OCREATE)){
			cout << "Error al crear el cofre: " << prop->error().name() << endl;
		}
		if(!event->open("eventos.kch", StashDB::OWRITER | StashDB::OCREATE)){
			cout << "Error al crear el cofre: " << event->error().name() << endl;
		}
	}

	OA::~OA(){
		//Destructor
	}

	//Crear una Clase en la BD
	bool OA::crear_clase(string nombre){
		bool se_creo = false;
		//Se verifica que no exista la clase aun, y que no exceda el limite de clases
		if( get_clase(nombre) == NULL && num_clases < C ){		
			
			//Se verifica que se guarde el nombre de la clase y la posicion en la BD
			if( !(cla->set(nombre, EnteroAString( num_clases ) )) ){
				cout << "Error creando clase: " << cla->error().name() << endl;
			}
			else{
				//Se crea la clase
				clases[num_clases] = new Clase(nombre);
				num_clases+=1;
				se_creo = true;
			}
		}
		return se_creo;
	}
	
	//Se inicializa el valor de la propiedad de una instancia
	bool OA::inicializar_instancia(int tipo_propiedad, string nom_clase, string nom_instancia, string nom_propiedad){
		bool band = false;
		
			if(tipo_propiedad == ENTERO){	
				agregar_valorApropiedad(nom_clase, nom_instancia, nom_propiedad, INT_MIN);
				band = true;
			}
			else if(tipo_propiedad == REAL){
				agregar_valorApropiedad(nom_clase, nom_instancia, nom_propiedad, DBL_MIN);
				band = true;
			}
			else if(tipo_propiedad == CADENA){
				agregar_valorApropiedad(nom_clase, nom_instancia, nom_propiedad, "");
				band = true;
			}
			return band;
		}
	
	//Crear una instancia en una clase dada
	bool OA::crear_instancia(string nom_instancia, string nom_clase){
		bool se_creo = false;
		Clase *cl = get_clase(nom_clase);
		int tipo_propiedad;

		//Si existe la clase y no existe la instancia, crear		
		if( cl != NULL && get_instancia(nom_clase, nom_instancia) == NULL && num_instancias < I ){

			//Se guarda el nombre de la instancia junto con el nombre de la clase
			//Para que se diferencie con otras instancias del mismo nombres pero de
			//otras clases
			if( !ins->set(nom_instancia + "_" + nom_clase, EnteroAString(num_instancias) ) ){
				cout << "Error creando instancia: " << ins->error().name() << endl;
			}
			else{
				instancias[num_instancias] = new Instancia(nom_instancia, cl );
			
				//Agregar las propiedades de la clase a la instancia
				Cuadro::const_iterator it; //Iterador
				Cuadro *portMap; //Apuntador a hash
				portMap = cl->get_propiedades(); //consulto tabla hash de las propiedade de la clase
				for(it = portMap->begin(); it != portMap->end(); ++it){//Iterando
					//Voy agregando las propiedades del padre al hijo
					if( get_propiedad( (string)it->first ) !=NULL ){ 
						tipo_propiedad = get_propiedad( (string)it->first )->get_tipo();
						inicializar_instancia(tipo_propiedad, nom_clase, nom_instancia, (string)it->first);
					}
				}
				cl->agregar_instancia(nom_instancia, num_instancias);
				num_instancias+=1;
				se_creo = true;
			}
	
		}
		return se_creo;
	}
	
	//Crear una propiedad en la BD
	bool OA::crear_propiedad(string nom_clase, string nom_propiedad, int tipo_propiedad){
		bool band = false;
		int i;
		Clase *cl = get_clase(nom_clase);
		
		//Si existe la clase y si la propiedad no existe -> agregar 
		if(cl != NULL && get_propiedad(nom_propiedad) == NULL && num_propiedades < P){
			
			if( !prop->set(nom_propiedad, EnteroAString(num_propiedades) ) ){
				cout << "Error creando propiedad: " << prop->error().name() << endl;
			}
			else{
				//Creo la propiedad
				propiedades[num_propiedades] = new Propiedad( nom_propiedad, cl, tipo_propiedad );
				//Agrego la propiedad a la clase
				cl->agregar_propiedad(nom_propiedad, num_propiedades);
				
				//Inicializar nueva propiedad a las instancias de la clase
				Cuadro::const_iterator it; //Iterador
				Cuadro *portMap; //apuntador a hash
				portMap = cl->get_instancias(); //consulto tabla hash de las instancias
				for(it = portMap->begin(); it != portMap->end(); ++it){//Iterando
					inicializar_instancia(tipo_propiedad, nom_clase, (string)it->first, nom_propiedad);
				
				}
				
				int num_hijos = cl->get_num_hijos();
				if(num_hijos > 0){
					for(i = 0; i < num_hijos; i++){
						Clase *p = cl->get_hijo(i);
						p->agregar_propiedad(nom_propiedad, num_propiedades);
						//Inicializar nueva propiedad a las instancias de la clase
						portMap = p->get_instancias(); //consulto tabla hash de las propiedades del padre
						for(it = portMap->begin(); it != portMap->end(); ++it){//Iterando
							inicializar_instancia(tipo_propiedad, p->get_nombre(), (string)it->first, nom_propiedad);
						}
					}
				}
				num_propiedades = num_propiedades + 1;
				band = true;
			}
		}
		return band;
	}
	
	//Crear un evento con propiedad activa de tipo ENTERO
	bool OA::crear_evento(string nom_clase, string nom_evento, string nom_propActiva, int valor_futuro, string expresion){
		bool band = false;
		int c_v = 0;
		string variables[V]; //Aqui se guardan el nombre de las variables de la expresion.

		Clase *cl = get_clase(nom_clase);
		
		if(cl != NULL){		
					
			if( cl->existe_propiedad(nom_propActiva) && get_evento(nom_evento) == NULL && get_propiedad(nom_propActiva) != NULL && num_eventos < E){

				if( !event->set(nom_evento, EnteroAString(num_eventos) ) ){
					cout << "Error creando clase: " << event->error().name() << endl;
				}
				else{
					//validar que la expresion sea correcta							
					if( cl->comprobar_expresion(expresion, variables, &c_v)==0 ){
						eventos[num_eventos] = new Evento(nom_evento, nom_propActiva, valor_futuro, expresion);
						//Descomponer expresion y ver que variables tiene para agregar el evento a esas propiedades
						num_eventos = num_eventos + 1;
						band = true;
					}
				}
			}
		}
		return band;
	}

	//Crear un evento con propiedad activa de tipo REAL
	bool OA::crear_evento(string nom_clase, string nom_evento, string nom_propActiva, double valor_futuro, string expresion){
		bool band = false;
		int c_v = 0;
		string variables[V]; 

		Clase *cl = get_clase(nom_clase);
		
		//Si el evento ya existe y la prop_Activa es la misma, agregar la expresion al evento existente
		if(cl != NULL){	
			if( cl->existe_propiedad(nom_propActiva) && get_evento(nom_evento) == NULL && get_propiedad(nom_propActiva) != NULL && num_eventos < E){
				if( !event->set(nom_evento, EnteroAString(num_eventos) ) ){
					cout << "Error creando clase: " << event->error().name() << endl;
				}
				else{
					//validar que la expresion sea correcta							
					if( cl->comprobar_expresion(expresion, variables, &c_v)==0 ){
						eventos[num_eventos] = new Evento(nom_evento, nom_propActiva, valor_futuro, expresion);
						//Descomponer expresion y ver que variables tiene para agregar el evento a esas propiedades
						num_eventos = num_eventos + 1;
						band = true;
					}
				}
	
			}
		}
		return band;
	}
	
	//Crear un evento con propiedad activa de tipo CADENA
	bool OA::crear_evento(string nom_clase, string nom_evento, string nom_propActiva, string valor_futuro, string expresion){
		bool band = false;
		int c_v = 0;
		string variables[V]; 

		Clase *cl = get_clase(nom_clase);
		
		//Si el evento ya existe y la prop_Activa es la misma, agregar la expresion al evento existente
		//cout<<"compro clase no nula";
		if(cl != NULL){			
			//cout<<"clase si existe, comprobando si la prop existe y no es nula, el evento existe"<<endl;
			if( cl->existe_propiedad(nom_propActiva) && get_evento(nom_evento) == NULL && get_propiedad(nom_propActiva) != NULL && num_eventos < E){
				//cout<<"todobien"<<endl;
				if( !event->set(nom_evento, EnteroAString(num_eventos) ) ){
					cout << "Error creando clase: " << event->error().name() << endl;
				}
				else{
					//validar que la expresion sea correcta
					//cout<<"comprobacion de expresion"<<endl;
					if( cl->comprobar_expresion(expresion, variables, &c_v)==0 ){
						//cout<<"hace comprobacion"<<endl;
						eventos[num_eventos] = new Evento(nom_evento, nom_propActiva, valor_futuro, expresion);
						//Descomponer expresion y ver que variables tiene para agregar el evento a esas propiedades
						num_eventos = num_eventos + 1;
						band = true;
					}
					else{
						cout << "La expresion no es correcta, puede que tenga alguna propiedad que no exista en la clase o la expresion este mal formulada" << endl;
					}
				}
			}else if(get_evento(nom_evento)!= NULL){//en caso de que el evento ya exista se tiene que asignar un nuevo valor y condicion
				//cout<<"El evento es distinto de NULL, ya fue asignado"<<endl;
				if(cl->comprobar_expresion(expresion, variables, &c_v)==0 ){
					get_evento(nom_evento)->agregar_expresion(valor_futuro,expresion);
					band = true;
				}
			}

		}
		return band;
	}
	
	//Consultar una clase dada su posicion
	Clase* OA::get_clase(int pos){
		if( pos < num_clases ){
			return clases[pos];
		}
		return NULL;
	}
	
	//Consultar una clase dado su nombre
	Clase* OA::get_clase(string nombre){
		Clase *r = NULL;
		string valor;
		if( cla->get(nombre, &valor) ){
			int n ;
			istringstream ( valor ) >> n;//Transformo el string en int
			r = clases[n];
		}
		
		return r;
	}

	//Consultar instancia dada su posicion
	Instancia* OA::get_instancia(int pos){
		if( pos < num_instancias ){
			return instancias[pos];
		}
		return NULL;
	}

	//Consultar instancia dada su clase y el nombre de la instancia
	Instancia* OA::get_instancia(string clase, string nombre){
		Instancia *i = NULL;
		
		string valor;
		
		if( ins->get(nombre + "_" + clase, &valor) ){
			int n;
			istringstream ( valor ) >> n;//Transformo el string en int
			i = instancias[n];
		}
		
		return i;
	}

	//Consultar propiedad dada su posicion
	Propiedad* OA::get_propiedad(int pos){
		if( pos < num_propiedades ){
			return propiedades[pos];
		}
		return NULL;
	}

	//Consultar propiedad dado su nombre
	Propiedad* OA::get_propiedad(string nombre){
		Propiedad *i = NULL;
		string valor;
		if( prop->get(nombre, &valor) ){
			int n;
			istringstream ( valor ) >> n;//Transformo el string en int
			i = propiedades[n];
		}
		return i;
	}
	
	//Consultar posicion de una propiedad en el arreglo principal
	int OA::get_pos_propiedad(string nombre){
		int n = -1;
		string valor;
		if( prop->get(nombre, &valor) ){
			istringstream ( valor ) >> n;//Transformo el string en int
		}
		return n;
	}

	//Consultar evento dado su nombre
	Evento* OA::get_evento(string nombre){
		Evento *i = NULL;
		string valor;
		if( event->get(nombre, &valor) ){
			int n;
			istringstream ( valor ) >> n;//Transformo el string en int
			i = eventos[n];
		}
		return i;
	}

	//Consultar numero de clases en la BD
	int OA::get_num_clases(){
		return num_clases;
	}

	//Consultar numero de instancias en la BD
	int OA::get_num_instancias(){
		return num_instancias;
	}

	//Consultar numero de propiedades en la BD
	int OA::get_num_propiedades(){
		return num_propiedades;
	}

	//Consultar numero de eventos en la BD
	int OA::get_num_eventos(){
		return num_eventos;
	}

	//Consultar el valor de una propiedad de una instancia, se retorna en string
	string OA::consultar_propiedad_instancia(string nom_clase, string nom_instancia, string nom_propiedad){
		Instancia *i;
		string v = "";
		i = get_instancia(nom_clase, nom_instancia);

		//Retornar los valores de las propiedad que se consulten, pero en formato string
		if( i !=NULL ){	
			if( i->existe_propiedad(nom_propiedad) ){
				struct var *valor;
				valor = i->get_valor_propiedad(nom_propiedad);
			
				if(valor->val.type == ENTERO){
					v = EnteroAString( valor->val.ival  );
				}
				if(valor->val.type == REAL){
					v = DoubleAString( valor->val.rval );
				}
				if(valor->val.type == CADENA){
					v = string( valor->val.cval );
				}
			}
			else{
				cout << "No se encontro la propiedad "<< nom_propiedad << " en la clase "<< nom_clase << endl;
				}
		}
		return v;
	}


	//Si devuelve verdadero se agrego el padre, falso en caso contrario
	bool OA::agregar_subclase(string nom_clase, string nom_padre){
		int tipo_propiedad;
		bool todo_bien = false;
		Clase *hijo = get_clase(nom_clase);
		Clase *padre = get_clase(nom_padre);
		if( hijo!=NULL && padre!=NULL && !es_subclase_de(nom_clase, nom_padre) &&  !es_subclase_de(nom_padre, nom_clase) ){	
			hijo->agregar_padre(padre); //Agregar padre al hijo
			int num_padres = padre->get_num_padres(); //Consultar ancestros del padre
			if(num_padres > 0){
				for(int i = 0; i < num_padres; i++){//Agregar los ancestros del padre al hijo
					Clase *p = get_clase( padre->get_padre(i)->get_nombre() );
					hijo->agregar_padre( p );
					p->agregar_hijo( hijo );
				}
			}
			padre->agregar_hijo(hijo);//Agregar hijo al padre
			int num_hijos = hijo->get_num_hijos();//Consultar descendientes del hijo
			if(num_hijos > 0){
				for(int i = 0; i < num_hijos; i++){//Agregar los descendientes del hijo al padre
					Clase *p = get_clase( hijo->get_hijo(i)->get_nombre() );
					padre->agregar_hijo( p );
					p->agregar_padre( padre );
				}
			}
						
			//Agregar las propiedades de la superclase a la clase
			Cuadro::const_iterator it; //Iterador
			Cuadro *portMap; //apuntador a hash
			portMap = padre->get_propiedades(); //consulto tabla hash de las propiedades del padre
			for(it = portMap->begin(); it != portMap->end(); ++it){//Iterando
					//Voy agregando las propiedades del padre al hijo
	    			hijo->agregar_propiedad((string)it->first, (int)it->second);//Agrego propiedad al hijo, y abajo a sus descendientes
	    			tipo_propiedad = get_propiedad( (string)it->first )->get_tipo();
	    			
	    			//Inicializar nueva propiedad a las instancias de la clase
				Cuadro::const_iterator ins; //Iterador
				Cuadro *portMap_ins; //apuntador a hash
				portMap_ins = hijo->get_instancias(); //consulto tabla hash de las instancias
				for(ins = portMap_ins->begin(); ins != portMap_ins->end(); ++ins){//Iterando
					inicializar_instancia(  tipo_propiedad  , nom_clase, (string)ins->first, (string)it->first);
				}
    			
    			//Tambien agrego las propiedades a los descendientes del hijo
				for(int i = 0; i < num_hijos; i++){//Agregar propiedades del padre a los descendientes del hijo
					Clase *p = get_clase( hijo->get_hijo(i)->get_nombre() );
					p->agregar_propiedad((string)it->first, (int)it->second);//Clave y Valor
				
					 //Inicializar nueva propiedad a las instancias de la clase
					portMap_ins = p->get_instancias(); //consulto tabla hash de las instancias
					for(ins = portMap_ins->begin(); ins != portMap_ins->end(); ++ins){//Iterando
						inicializar_instancia(  tipo_propiedad  , p->get_nombre(), (string)ins->first, (string)it->first);
					}
				}
			}
			todo_bien = true;
		}
		return todo_bien;
	}

	//Agregar sub_propiedad a una propiedad
	bool OA::agregar_subpropiedad(string nom_propiedad, string nom_padre){
		bool todo_bien = false;
		Propiedad *hijo = get_propiedad(nom_propiedad);
		Propiedad *padre = get_propiedad(nom_padre);
		
		if( hijo!=NULL && padre!=NULL && !es_subpropiedad_de(nom_propiedad, nom_padre) &&  !es_subpropiedad_de(nom_padre, nom_propiedad) ){	
			hijo->agregar_padre(padre);
			int num_padres = padre->get_num_padres();
			for(int i = 0; i < num_padres; i++){//Agregar los ancestros del padre al hijo
				Propiedad *p = get_propiedad( (padre->get_padre(i))->get_nombre() );
				hijo->agregar_padre( p );
				p->agregar_hijo( hijo );
			}
			
			padre->agregar_hijo(hijo);
			int num_hijos = hijo->get_num_hijos();//Consultar descendientes del hijo
			for(int i = 0; i < num_hijos; i++){//Agregar los descendientes del hijo al padre
				Propiedad *p = get_propiedad( (hijo->get_hijo(i))->get_nombre() );
				padre->agregar_hijo( p );
				p->agregar_padre( padre );
			}
						
			todo_bien = true;			
		}
		return todo_bien;
	}

	//Agregar sub_evento al evento
	bool OA::agregar_subevento(string nom_evento, string nom_padre){
		bool todo_bien = false;
		Evento *hijo = get_evento(nom_evento);
		Evento *padre = get_evento(nom_padre);
		Evento *h, *p;
		
		if( hijo!=NULL && padre!=NULL && !es_subevento_de(nom_evento, nom_padre) &&  !es_subevento_de(nom_padre, nom_evento) ){	
			hijo->agregar_padre(padre);
			int num_padres = padre->get_num_padres();
			
			for(int i = 0; i < num_padres; i++){//Agregar los ancestros del padre al hijo
				p = get_evento( (padre->get_padre(i))->get_nombre() );
				hijo->agregar_padre( p );				
				p->agregar_hijo( hijo );				
			
			}
		
			padre->agregar_hijo(hijo);
			int num_hijos = hijo->get_num_hijos();//Consultar descendientes del hijo
			
			for(int i = 0; i < num_hijos; i++){//Agregar los descendientes del hijo al padre
				h = get_evento( (hijo->get_hijo(i))->get_nombre() );
				h->agregar_padre( padre );				
				padre->agregar_hijo( h );
				
			}
			todo_bien = true;			
		}
		return todo_bien;
	}

	//Verificar si una clase es subclase de otra
	bool OA::es_subclase_de(string nom_clase, string nom_clase_padre){
		bool es_subclase = false;
		Clase *clase = get_clase( nom_clase );
		Clase *padre = get_clase( nom_clase_padre );
		
		if( clase!=NULL && padre!=NULL ){//Si existen ambas clases, procedemos a comprobar si nom_clase es subclase de nom_clase_padre
			int num_padres = clase->get_num_padres();
			int i = 0;
			while(i < num_padres && !es_subclase){
				if( (clase->get_padre(i))->get_nombre() == nom_clase_padre ){
					es_subclase = true;
				}
				i+=1;
			}
		}
		return es_subclase;
	}

	//Verificar si una propiedad es subpropiedad de otra
	bool OA::es_subpropiedad_de(string nom_propiedad, string nom_propiedad_padre){
		bool es_subpropiedad = false;
		
		Propiedad *propiedad = get_propiedad( nom_propiedad );
		Propiedad *padre = get_propiedad( nom_propiedad_padre );
		
		if( propiedad!=NULL && padre!=NULL ){
			
			int num_padres = propiedad->get_num_padres();
			int i = 0;
			while(i < num_padres && !es_subpropiedad){
				if( (propiedad->get_padre(i))->get_nombre() == nom_propiedad_padre ){
					es_subpropiedad = true;
				}
				i+=1;
			}
		}
		return es_subpropiedad;
	}

	//Verificar si un evento es subevento de otro
	bool OA::es_subevento_de(string nom_evento, string nom_evento_padre){
		bool es_subevento = false;
		
		Evento *evento = get_evento( nom_evento );
		Evento *padre = get_evento( nom_evento_padre );
		
		if( evento!=NULL && padre!=NULL ){
			int num_padres = evento->get_num_padres();
			int i = 0;
			while(i < num_padres && !es_subevento){
				if( (evento->get_padre(i))->get_nombre() == nom_evento_padre ){
					es_subevento = true;
				}
				i+=1;
			}
		}
		return es_subevento;
	}

	//Agregar valor a una propiedad de tipo ENTERO
	bool OA::agregar_valorApropiedad(string nom_clase, string nom_instancia, string nom_propiedad, int valor){
		Instancia *i;
		Clase *c;
		bool band = false, existe_propiedad_en_la_clase, entrar = false;
		int j;
		Propiedad *p,*p_p;
		
		c = get_clase(nom_clase); //Obtengo apuntador a la clase	
		
		if( c != NULL){ //Si existe la clase
			existe_propiedad_en_la_clase = c->existe_propiedad(nom_propiedad);
			if( existe_propiedad_en_la_clase ){ //Si existe la propiedad en esa clase
				i = get_instancia(nom_clase, nom_instancia);
				if( i != NULL){ //Si existe la instancia en esa clase
					entrar = true;
				}
				else{
					cout << "No existe la instancia " << nom_instancia << endl;
				}
			}
			else{
				cout << "No existe la propiedad " << nom_propiedad << endl;
			}
		}
		else{
			cout << "No existe la clase " << nom_clase << endl;
		}

		p = get_propiedad(nom_propiedad);
		if( entrar && p != NULL){ //Si existe la instancia, la propiedad y la clase
			band = i->agregar_valor_propiedad(nom_propiedad, p->get_tipo() , valor);//Si se agrega el valor correctamente
			
			int prop_padres = p->get_num_padres();
			for(j = 0; j < prop_padres; j++){
				p_p = p->get_padre(j);
				i->agregar_valor_propiedad(p_p->get_nombre(), p_p->get_tipo() , valor);
			}
		}
		
		return band;
	}

	//Agregar valor a una propiedad de tipo REAL
	bool OA::agregar_valorApropiedad(string nom_clase, string nom_instancia, string nom_propiedad, double valor){
		Instancia *i;
		Clase *c;
		bool band = false, existe_propiedad_en_la_clase, entrar = false;
		int j;
		Propiedad *p,*p_p;
		
		c = get_clase(nom_clase); //Obtengo apuntador a la clase	
		
		if( c != NULL){ //Si existe la clase
			existe_propiedad_en_la_clase = c->existe_propiedad(nom_propiedad);
			if( existe_propiedad_en_la_clase ){ //Si existe la propiedad en esa clase
				i = get_instancia(nom_clase, nom_instancia);
				if( i != NULL){ //Si existe la instancia en esa clase
					entrar = true;
				}
				else{
					cout << "No existe la instancia " << nom_instancia << endl;
				}
			}
			else{
				cout << "No existe la propiedad " << nom_propiedad << endl;
			}
		}
		else{
			cout << "No existe la clase " << nom_clase << endl;
		}

		p = get_propiedad(nom_propiedad);
		if( entrar && p != NULL){ //Si existe la instancia, la propiedad y la clase
			band = i->agregar_valor_propiedad(nom_propiedad, p->get_tipo() , valor);//Si se agrega el valor correctamente

			int prop_padres = p->get_num_padres();
			for(j = 0; j < prop_padres; j++){
				p_p = p->get_padre(j);
				i->agregar_valor_propiedad(p_p->get_nombre(), p_p->get_tipo() , valor);
			}
		}
		
		return band;
	}

	//Agregar valor a una propiedad de tipo ENTERO
	bool OA::agregar_valorApropiedad(string nom_clase, string nom_instancia, string nom_propiedad, string valor){
		Instancia *i;
		Clase *c;
		bool band = false, existe_propiedad_en_la_clase, entrar = false;
		int j;
		Propiedad *p,*p_p;
		
		c = get_clase(nom_clase); //Obtengo apuntador a la clase	
		
		if( c != NULL){ //Si existe la clase
			existe_propiedad_en_la_clase = c->existe_propiedad(nom_propiedad);
			if( existe_propiedad_en_la_clase ){ //Si existe la propiedad en esa clase
				i = get_instancia(nom_clase, nom_instancia);
				if( i != NULL){ //Si existe la instancia en esa clase
					entrar = true;
				}
				else{
					cout << "No existe la instancia " << nom_instancia << endl;
				}
			}
			else{
				cout << "No existe la propiedad " << nom_propiedad << endl;
			}
		}
		else{
			cout << "No existe la clase " << nom_clase << endl;
		}

		p = get_propiedad(nom_propiedad);
		if( entrar && p != NULL ){ //Si existe la instancia, la propiedad y la clase
			band = i->agregar_valor_propiedad(nom_propiedad, p->get_tipo() , valor);//Si se agrega el valor correctamente
			
			int prop_padres = p->get_num_padres();
			for(j = 0; j < prop_padres; j++){
				p_p = p->get_padre(j);
				i->agregar_valor_propiedad(p_p->get_nombre(), p_p->get_tipo() , valor);
			}
		}
		
		return band;
	}

	//Activar eventos
	bool OA::activar_eventos(string nom_clase, string nom_evento){
	
		Clase *cl,*cl_padre;
		Instancia *i;
		Evento *e, *evento;
		valor_r *va;
		int k, m, num_clases, j;
		bool cumple = false, cumple_una_expresion;
		string arreglo_evento[V];
				
		cl = get_clase(nom_clase); //Obtengo el apuntador a la clase donde se activarán los eventos
		cl_padre = cl;		
		evento = get_evento(nom_evento); //Obtengo el apuntador al evento que se activará
		//cout<<"evento pasado"<<nom_evento<<endl;
		if( cl !=NULL && evento !=NULL ){
			j = 0;
			num_clases = cl_padre->get_num_hijos();

			do{
			
			Cuadro::const_iterator it; //Iterador
			Cuadro *portMap; //apuntador a hash
			portMap = cl->get_instancias(); //consulto las instancias de esa clase
			

			for(it = portMap->begin(); it != portMap->end(); ++it){//Iterando por todas las instancias de la clase
				//Validar que el evento padre pertenezca a la misma clase
				
				i = get_instancia(nom_clase, (string) it->first); //Obtengo el apuntador a la instancia en donde se investigara si existe el evento
				//cout << i->get_nombre() << endl;
				//cout << "Instancia: " << it->first << ", Posicion: " << instancias[(int)it->second - 1]->get_nombre() << endl;//Clave y Valor
		    	int eventos_padres = evento->get_num_padres(); //Numero de padres del evento principal
		    	int e_padres = 0;
		    	e = get_evento(nom_evento); //El evento principal se activa primero
		
				//Obtener los string de los eventos padres
				m = 0;
				while( m <  eventos_padres ){
					arreglo_evento[m] = (evento->get_padre(m))->get_nombre();
					m+=1;		
				}

				//Organizar los eventos padres 
				bool activar_ancestros = organizar_eventos( arreglo_evento, eventos_padres, nom_clase );
			
		    	cumple = false;
		    	do{ //Recorre los eventos padres
				
		    		if( e != NULL && i != NULL & i->existe_propiedad(e->get_nombre_propiedad())  ){
							
						cumple_una_expresion = false;
						k = 0;
						//Ciclar todas las expresiones del evento que se podrian cumplir
						while( k < e->get_num_expr_y_prop_activ() && !cumple_una_expresion){//Mientras haya expresiones del mismo evento y ninguna ha cumplido
						//cout<<e->get_expresion(k)<<endl;

							if( i->validar_expresion( e->get_expresion(k) ) ) {
								va = e->get_valor_nuevo(k);
						
								if(va->tipo == ENTERO){
									agregar_valorApropiedad(nom_clase, i->get_nombre_puro(),  e->get_nombre_propiedad(), va->ival);
									cout<<"Agrega propiedad entera"<<endl;
								}
								else if(va->tipo == REAL){
									agregar_valorApropiedad(nom_clase, i->get_nombre_puro(),  e->get_nombre_propiedad(), va->rval);
									cout<<"Agrega propiedad float"<<endl;
								}
								else if(va->tipo == CADENA){
									agregar_valorApropiedad(nom_clase, i->get_nombre_puro(),  e->get_nombre_propiedad(), va->cval);
									cout<<"Agrega propiedad string"<<endl;
								}
								cumple_una_expresion = true; //Ya no se deben revisar las demas expresiones del evento actual
								cumple = true;//Se cumple el evento principal, por lo tanto se puede seguir con el do-while y activar los eventos padres
								//cout << "Cumple con la condicion" << endl << endl;
								cout<<"la instancia "<<i->get_nombre_puro()<<" cambio su propiedad "<<e->get_nombre_propiedad()<<endl;
							}
							else{
								cout << "NO Cumple con la condicion" << endl << endl;
								
							}
									k+=1;
						}
					}
					else{
						cout << "El evento es NULL o La instancia es NULL" << endl;		
					}
			
					e = get_evento( arreglo_evento[e_padres]);//Continuo con los eventos padres del evento principal
					e_padres+=1;
			
				}while( e_padres <= eventos_padres && cumple && activar_ancestros);
			}
				
				if(j < num_clases){
					cl = cl_padre->get_hijo(j);
					nom_clase = cl->get_nombre();
				}
				j = j + 1;
			}while(j <= num_clases );
		}
		else{
			cout << "La clase " <<  nom_clase << " o el evento dado " <<  nom_evento << " no existe" << endl;
		}

		return cumple;
	}
	
	bool OA::existencia_consulta_reactiva(string events[V], int n, string cl){
		bool band = true;
		int i;
		
		if(get_clase(cl)== NULL){
			band = false;
			cout << "La clase especificada no existe" << endl;		
		}
		
		i = 0;
		while( i < n ){ //Recorro todos los eventos nombrados en la consulta reactiva
			if( get_evento( events[i] ) == NULL ){//Si el evento es NULL no existe
				band = false;
				cout <<"El evento " << events[i] << " no existe" << endl;
			}
			i++;
		}
		cout << endl;
		return band;
	}

	bool OA::pertenece_a(string nom_instancia, string nom_clase){
		bool pertenece = true;
		Clase *cl, *c_i;
		
		if( get_instancia(nom_clase, nom_instancia) == NULL ){
			pertenece = false;
			cl = get_clase(nom_clase);
			int i, num_clases_hijas;
			
			if(cl != NULL){
				i = 0;
				num_clases_hijas = cl->get_num_hijos();
			
				while(i < num_clases_hijas && !pertenece){
				
					c_i = cl->get_hijo(i);
				
					Cuadro::const_iterator it; //Iterador
					Cuadro *portMap; //apuntador a hash
					
					portMap = c_i->get_instancias(); //consulto las instancias de esa clase
					it = portMap->begin();
				
					while( it != portMap->end() && !pertenece ){//Iterando por todas las instancias de la clase
						
						if( (string)it->first == nom_instancia ){
							pertenece = true;
							}
						++it;
					}
				
				i+=1;
				}
			}
		}
				
		return pertenece;
	}

	bool OA::organizar_eventos(string events[V], int n, string clase){
		
		bool no_hay_ciclos = true, band;
		Clase *cl;
		Evento *ev,*ev2;
		string variables[V], variables2[V], aux;
		
		int c_v, c_v2, j, k, i, m;
		cl = get_clase( clase );

		j = 0;
		while(j < n && no_hay_ciclos ){ 
			ev = get_evento( events[j] );
			cl->comprobar_expresion(ev->get_expresion(0), variables, &c_v);//Revisar todas las expresiones :(
			
			k = 0;
			while(k < n && no_hay_ciclos ){		
				ev2 = get_evento( events[k] );
				if( k != j && k > j ){
					
					i = 0;
					band = true;
					while(i < c_v && band && no_hay_ciclos ){
						if( ev2->get_nombre_propiedad() == variables[i]){

							cl->comprobar_expresion(ev2->get_expresion(0), variables2, &c_v2);
							m = 0;
							while(m < c_v2 && band && no_hay_ciclos ){
								if( ev->get_nombre_propiedad() == variables2[m] )
									no_hay_ciclos = false;
								m+=1;							
							}
							aux = events[k];
							events[k] = events[j];
							events[j] = aux;
							band = false;
							//cout << "intercambio " << events[k] << " y " << events[j] << endl;
							ev = get_evento( events[j] );
							cl->comprobar_expresion(ev->get_expresion(0), variables, &c_v);
							k = -1;
						}
						i+=1;
					}				
					//cout << "La propiedad " << variables[i] << " esta en la condicion" << endl;
				}
				k+=1;
			}

			j++;
		}
		
		if( !no_hay_ciclos  )
			cout << "HAY CICLOS!" << endl;
		else		
		for(i = 0; i < n; i++){
			//cout << events[i] << endl;
		}
		return (no_hay_ciclos);

	}

# endif
