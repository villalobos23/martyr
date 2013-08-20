/*
 * martyr_struct_Ontology.cpp
 *
 *  Created on: 14/12/2012
 *      Author: Luis Villalobos
 */


//Librerias minimas para usar JNI
#include <iostream>
#include <sstream>
#include <jni.h>
#include "martyr_struct_Ontology.h"

//Librerias y cabeceras para usar MIOA tal como lo uso Yanina
#include "MIOA/OA.h"

using namespace std;


#define JNI_FALSE  0
#define JNI_TRUE   1
OA *oa_bd = new OA();//creamos la ontología activa
map<string,string> *eventosHash = new map<string,string>();
string eventsArray[100];
int num_eventos=0;

/**
 * Insercion de subclase
 */
JNIEXPORT jboolean JNICALL Java_martyr_struct_Ontology_mioatize_1class
  (JNIEnv *env, jobject thisObj, jstring classname){
	const char *cstr = env->GetStringUTFChars(classname, NULL);
	bool creation = oa_bd->crear_clase(cstr);
	env->ReleaseStringUTFChars(classname, cstr);
	return creation;
}
/**
 * Insercion de subclase
 */
JNIEXPORT jboolean JNICALL Java_martyr_struct_Ontology_mioatize_1subclass
  (JNIEnv *env, jobject thisObj, jstring classname, jstring superclass){
	const char *cstr = env->GetStringUTFChars(classname, NULL);
	const char *ccstr = env->GetStringUTFChars(superclass,NULL);
	bool creation = oa_bd->agregar_subclase(cstr,ccstr);
	//cout<<"clase "<<cstr<<" superclase "<<ccstr<<endl;
	env->ReleaseStringUTFChars(classname, cstr);env->ReleaseStringUTFChars(superclass, ccstr);
	return creation;
}
/*
 * cantidades de las distintas partes de mioa
 * */

JNIEXPORT void JNICALL Java_martyr_struct_Ontology_test(JNIEnv *env, jobject thisObj){
	cout<<"cantidad de clases"<<oa_bd->get_num_clases()<<endl;
	cout<<"cantidad de propiedades"<<oa_bd->get_num_propiedades()<<endl;
	cout<<"cantidad de eventos"<<oa_bd->get_num_eventos()<<endl;
	cout<<"cantidad de instancias"<<oa_bd->get_num_instancias()<<endl;
	return;
}

JNIEXPORT jboolean JNICALL Java_martyr_struct_Ontology_mioatize_1create
  (JNIEnv *env, jobject thisObj, jstring inst, jstring classname){
	const char *cinst = env->GetStringUTFChars(inst, NULL);
	const char *cclassname = env->GetStringUTFChars(classname, NULL);
	bool success = oa_bd->crear_instancia(cinst,cclassname);
	env->ReleaseStringUTFChars(classname, cclassname);
	env->ReleaseStringUTFChars(inst,cinst);
	return success;
}

JNIEXPORT jboolean JNICALL Java_martyr_struct_Ontology_mioatize_1values
  (JNIEnv *env, jobject thisObj, jstring instance, jstring classname, jstring propname, jstring valor){
	const char *cinst = env->GetStringUTFChars(instance, NULL);
	const char *cclassname = env->GetStringUTFChars(classname, NULL);
	const char *cprop = env->GetStringUTFChars(propname,NULL);
	const char *cval = env->GetStringUTFChars(valor,NULL);
	stringstream ss;
	bool success = false;
	//cout<<cprop<<" -> "<<endl;
	int tipo = oa_bd->get_propiedad(cprop)->get_tipo();
	int val;
	double dval;
	//cout <<" es de tipo: "<< tipo<<endl;
	if(tipo == 0){//Entero
		ss << cval;
		ss >> val;
		//cout<<"valor trans "<<val<<endl;
		success = oa_bd->agregar_valorApropiedad(cclassname,cinst,cprop,val);
	}else if(tipo == 1){//REAL?
		ss << cval;
		ss >> dval;
		success = oa_bd->agregar_valorApropiedad(cclassname,cinst,cprop,dval);
	}else{//Cadena
		success = oa_bd->agregar_valorApropiedad(cclassname,cinst,cprop,cval);
	}
	env->ReleaseStringUTFChars(classname, cclassname);
	env->ReleaseStringUTFChars(instance,cinst);
	env->ReleaseStringUTFChars(propname, cprop);
	env->ReleaseStringUTFChars(valor, cval);
	return success;
}

/**
 * Inserción subclase
 */
JNIEXPORT jboolean JNICALL Java_martyr_struct_Ontology_mioatize_1subevent
  (JNIEnv *env, jobject thisObj, jstring eventname, jstring superevent){
	const char *ceve = env->GetStringUTFChars(eventname,NULL);
	const char *cceve = env->GetStringUTFChars(superevent,NULL);
	//cout<<"evento: "<<ceve<<" superevento: "<<cceve<<endl;
	bool creation = oa_bd->agregar_subevento(ceve,cceve);
	env->ReleaseStringUTFChars(eventname, ceve);env->ReleaseStringUTFChars(superevent,cceve);
	return false;
}

/**
 * Insercion propiedad
 * */

JNIEXPORT jboolean JNICALL Java_martyr_struct_Ontology_mioatize_1property
  (JNIEnv *env, jobject thisObj, jstring classname, jstring propname, jint type){
	const char *cstr = env->GetStringUTFChars(classname, NULL);
	const char *ccstr = env->GetStringUTFChars(propname,NULL);
	bool creation;
	cout<<"clase: "<<cstr<<" prop: "<<ccstr<<" tipo: "<<type<<endl;
	if (type == 0){
		creation = oa_bd->crear_propiedad(cstr,ccstr,ENTERO);
	}else if(type == 1){
		creation = oa_bd->crear_propiedad(cstr,ccstr,CADENA);
	}else if(type == 2){
		creation = oa_bd->crear_propiedad(cstr,ccstr,REAL);
	}
	env->ReleaseStringUTFChars(classname, cstr);env->ReleaseStringUTFChars(propname, ccstr);
	return creation;
}

/*
 * Insercion evento+propiedad activa, la funcion de MIOA toma una propiedad estatica previamente creada la pasa a ser dinamica
 * y crea un evento con todos estos parametros
 *
 * */
JNIEXPORT jboolean JNICALL Java_martyr_struct_Ontology_mioatize_1activeproperty
  (JNIEnv *env, jobject thisObj, jstring classname, jstring eventname, jstring propname, jstring reactvalue, jstring condition){
	const char *cclassname = env->GetStringUTFChars(classname,NULL);
	const char *ceventname = env->GetStringUTFChars(eventname, NULL);
	const char *cpropname = env->GetStringUTFChars(propname,NULL);
	const char *creactvalue = env->GetStringUTFChars(reactvalue,NULL);
	const char *ccond = env->GetStringUTFChars(condition,NULL);
	int test;
	//cin>>test;
	bool creation = oa_bd->crear_evento(cclassname,ceventname,cpropname,creactvalue,ccond);
	//cout<<"clase: "<<cclassname<<" evento: "<<ceventname<<" propname: "<<cpropname<<" reactvalue: "<<creactvalue<<" cond: "<<ccond<<endl;
	env->ReleaseStringUTFChars(classname,cclassname);env->ReleaseStringUTFChars(eventname,ceventname);
	env->ReleaseStringUTFChars(propname,cpropname);env->ReleaseStringUTFChars(reactvalue,creactvalue);
	env->ReleaseStringUTFChars(condition,ccond);
	return creation;
}

JNIEXPORT jboolean JNICALL Java_martyr_struct_Ontology_mioatize_1objective
  (JNIEnv *env, jobject thisObj, jstring event){
	const char *ceventname = env->GetStringUTFChars(event,NULL);

	//el status se saca directamente desde el stat no conversion needed

		//cout<<"Evento:"<<ceventname<<endl;
		if((*eventosHash)[ceventname]!=""){
			return false;
		}else{
			eventsArray[num_eventos]=ceventname;
			cout<<"encolando evento:"<<ceventname<<endl;
			num_eventos+=1;
		}

	env->ReleaseStringUTFChars(event,ceventname);

	return true;
}

JNIEXPORT jboolean JNICALL Java_martyr_struct_Ontology_mioatize_1execution
  (JNIEnv *env, jobject thisObj,jstring classname, jstring prop, jint stat){
	const char *cclassname = env->GetStringUTFChars(classname,NULL);
	const char *cprop = env->GetStringUTFChars(prop,NULL);

	if(oa_bd->existencia_consulta_reactiva(eventsArray,num_eventos,cclassname)){
		if(stat == 1){//serial
			cout <<"Serial n"<<endl;

		}else if(stat == 2 &&
				oa_bd->organizar_eventos(eventsArray,num_eventos,cclassname)){//paralelo
			cout<<"Paralelo n"<<endl;
		}else{
			return false;
		}
		//Cual evento activar ?
		cout<<"antes: "<<oa_bd->consultar_propiedad_instancia("huracan", "ike", "intensidad")<<endl;
		for(int i = 0; i < num_eventos; i++){
			oa_bd->activar_eventos(cclassname,eventsArray[i]);
		}
		cout<<"despues: "<<oa_bd->consultar_propiedad_instancia("huracan", "ike", "intensidad")<<endl;
	}
	num_eventos= 0;
	env->ReleaseStringUTFChars(classname,cclassname);
	env->ReleaseStringUTFChars(prop,cprop);
	return true;
}

JNIEXPORT jstring JNICALL Java_martyr_struct_Ontology_mioatize_1getInsValue
  (JNIEnv *env, jobject thisObj, jstring clase, jstring inst, jstring prop){
	const char *cclassname = env->GetStringUTFChars(clase,NULL);
	const char *cinst = env->GetStringUTFChars(inst,NULL);
	const char *cprop = env->GetStringUTFChars(prop,NULL);
	string value = oa_bd->consultar_propiedad_instancia(cclassname,cinst,cprop);
	env->ReleaseStringUTFChars(clase,cclassname);
	env->ReleaseStringUTFChars(inst,cinst);
	env->ReleaseStringUTFChars(prop,cprop);
	return env->NewStringUTF(value.c_str());

}

JNIEXPORT void JNICALL Java_martyr_struct_Ontology_cleanMIOA
  (JNIEnv *env, jobject thisObj){
	oa_bd = new OA();// Solucion Provisional hasta que se pueda mejorar la limpieza de las listas
}
