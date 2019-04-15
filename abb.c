#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "abb.h"
#include "pila.h"

typedef struct abb_nodo{
	char* clave;
	void* dato;
	struct abb_nodo* izq;
	struct abb_nodo* der;
}abb_nodo_t;

struct abb{
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	abb_nodo_t* raiz;
	size_t cant;
};

struct abb_iter {
	pila_t* pila_iter;
};


char *strdup (const char *s) { 
    char *d = malloc (strlen(s) + 1);
    if (d == NULL) return NULL;          
    strcpy (d,s);                        
    return d;                            
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){

	abb_t* arbol = malloc(sizeof(abb_t));
	if(!arbol) return NULL;

	arbol->cmp = cmp;
	arbol->destruir_dato = destruir_dato;
	arbol->raiz = NULL;
	arbol->cant = 0;

	return arbol;
}

abb_nodo_t* abb_nodo_crear(const char* clave,void* dato){
	abb_nodo_t* abb_nodo = malloc(sizeof(abb_nodo_t));
	if(!abb_nodo) return NULL;

	char* copia_clave = strdup(clave);
	if(!copia_clave){
		free(abb_nodo);
		return NULL;
	}

	abb_nodo->clave = copia_clave;
	abb_nodo->dato = dato;
	abb_nodo->izq = NULL;
	abb_nodo->der = NULL;

	return abb_nodo;
}

abb_nodo_t* abb_nodo_buscar(abb_nodo_t* nodo,const char* clave,abb_comparar_clave_t cmp){

	if(cmp(nodo->clave,clave) == 0){
		return nodo;
	}else if(cmp(nodo->clave,clave) < 0){
		if(nodo->der){
			return abb_nodo_buscar(nodo->der,clave,cmp);
		}
		return NULL;	
	}else{
		if(nodo->izq){
			return abb_nodo_buscar(nodo->izq,clave,cmp);
		}
		return NULL;
	}
}

abb_nodo_t* abb_nodo_buscar_padre(abb_nodo_t* nodo,const char* clave,abb_comparar_clave_t cmp){

	if(cmp(nodo->clave,clave) == 0){
		return nodo;
	}else if(cmp(nodo->clave,clave) < 0){
		if(nodo->der){
			if(cmp(nodo->der->clave,clave) == 0){
				return nodo;
			}
			return abb_nodo_buscar_padre(nodo->der,clave,cmp);
		}
	}else{
		if(nodo->izq){
			if(cmp(nodo->izq->clave,clave) == 0){
				return nodo;
			}
			return abb_nodo_buscar_padre(nodo->izq,clave,cmp);
		}
	}
	return NULL;
}

bool abb_nodo_insertar(abb_nodo_t* nodo,const char* clave,void* dato,abb_t *arbol){

	if(arbol->cmp(nodo->clave,clave) == 0){
		if(arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
		nodo->dato = dato;
		return true;
	}else if(arbol->cmp(nodo->clave,clave) < 0){
		if(nodo->der){
			return abb_nodo_insertar(nodo->der,clave,dato,arbol);
		}
		abb_nodo_t* nodo_nuevo = abb_nodo_crear(clave,dato);
		if(!nodo_nuevo) return false;

		nodo->der = nodo_nuevo;
		arbol->cant++;
		return true;
	}else{
		if(nodo->izq){
			return abb_nodo_insertar(nodo->izq,clave,dato,arbol);
		}
		abb_nodo_t* nodo_nuevo = abb_nodo_crear(clave,dato);
		if(!nodo_nuevo) return false;

		nodo->izq = nodo_nuevo;
		arbol->cant++;
		return true;
	}
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){

	if(!arbol->raiz){
		abb_nodo_t* nodo_nuevo = abb_nodo_crear(clave,dato);
		if(!nodo_nuevo) return false;

		arbol->raiz = nodo_nuevo;
		arbol->cant++;
		return true;
	}
	return abb_nodo_insertar(arbol->raiz,clave,dato,arbol);
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if(!arbol->raiz) return false;
	return abb_nodo_buscar(arbol->raiz,clave,arbol->cmp);
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cant;
}

bool abb_nodo_hoja(abb_nodo_t* nodo){
	return (!nodo->izq && !nodo->der);
}

bool abb_nodo_1_hijo(abb_nodo_t* nodo){
	return (nodo->izq && !nodo->der) || (!nodo->izq && nodo->der);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	if(!arbol->raiz) return NULL;

	abb_nodo_t* nodo = abb_nodo_buscar(arbol->raiz,clave,arbol->cmp);
	if(!nodo) return NULL;

	return nodo->dato;
}

bool abb_nodo_hijo_der(abb_nodo_t* nodo_padre,abb_nodo_t* nodo_hijo,abb_comparar_clave_t cmp){
	if(!nodo_padre->der) return false;
	return (cmp(nodo_padre->der->clave,nodo_hijo->clave) == 0);
}

bool abb_nodo_hijo_izq(abb_nodo_t* nodo_padre,abb_nodo_t* nodo_hijo,abb_comparar_clave_t cmp){
	if(!nodo_padre->izq) return false;
	return (cmp(nodo_padre->izq->clave,nodo_hijo->clave) == 0);
}

bool abb_nodo_raiz(abb_t *arbol, abb_nodo_t* nodo){
	return arbol->raiz == nodo;
}

abb_nodo_t* abb_nodo_minimo(abb_nodo_t* nodo){
	if(!nodo->izq){
		return nodo;
	}
	return abb_nodo_minimo(nodo->izq);
}

void abb_nodo_swap(abb_nodo_t* nodo1,abb_nodo_t* nodo2){
	char* aux_clave1 = nodo1->clave;
	void* aux_dato1 = nodo1->dato;

	nodo1->clave = nodo2->clave;
	nodo1->dato = nodo2->dato;

	nodo2->clave = aux_clave1;
	nodo2->dato = aux_dato1;

	return;
}

void* abb_borrar_aux(abb_nodo_t* nodo_raiz,const char* clave,abb_comparar_clave_t cmp){

	void* dato;
	abb_nodo_t* nodo;

	if(cmp(nodo_raiz->der->clave,clave) == 0){
		nodo = nodo_raiz->der;
		dato = nodo->dato; 

		if(abb_nodo_hoja(nodo)){
			nodo_raiz->der = NULL;			
		}else{
			nodo_raiz->der = nodo->der;
		}
		free(nodo->clave);
		free(nodo);
		return dato;
	}

	nodo = abb_nodo_buscar(nodo_raiz->der,clave,cmp);
	if(!nodo) return NULL; // No se encuentra nodo.

	dato = nodo->dato;

	if(abb_nodo_hoja(nodo)){	
		abb_nodo_t* nodo_padre = abb_nodo_buscar_padre(nodo_raiz->der,clave,cmp);

		if(abb_nodo_hijo_der(nodo_padre,nodo,cmp)){
			nodo_padre->der = NULL;
		}else if(abb_nodo_hijo_izq(nodo_padre,nodo,cmp)){
			nodo_padre->izq = NULL;
		}
	}else if(abb_nodo_1_hijo(nodo)){
		
		abb_nodo_t* nodo_hijo;
		if(nodo->izq) nodo_hijo = nodo->izq;
		else if(nodo->der) nodo_hijo = nodo->der;
		
		abb_nodo_t* nodo_padre = abb_nodo_buscar_padre(nodo_raiz->der,clave,cmp);

		if(abb_nodo_hijo_der(nodo_padre,nodo,cmp)){
			nodo_padre->der = nodo_hijo;
		}else if(abb_nodo_hijo_izq(nodo_padre,nodo,cmp)){
			nodo_padre->izq = nodo_hijo;
		}
	}
	free(nodo->clave);
	free(nodo);
	return dato;
}


void *abb_borrar(abb_t *arbol, const char *clave){

	if(!arbol->raiz) return NULL;

	void* dato;

	abb_nodo_t* nodo = abb_nodo_buscar(arbol->raiz,clave,arbol->cmp);
	if(!nodo) return NULL; // No se encuentra nodo.

	dato = nodo->dato;

	// SI NO TIENE HIJOS.
	if(abb_nodo_hoja(nodo)){
		if(!abb_nodo_raiz(arbol,nodo)){
			abb_nodo_t* nodo_padre = abb_nodo_buscar_padre(arbol->raiz,clave,arbol->cmp);


			if(abb_nodo_hijo_der(nodo_padre,nodo,arbol->cmp)){
				nodo_padre->der = NULL;
			}else if(abb_nodo_hijo_izq(nodo_padre,nodo,arbol->cmp)){
				nodo_padre->izq = NULL;
			}
		}
		else{
			arbol->raiz = NULL;
		}

	// SI TIENE UN HIJO.
	}else if(abb_nodo_1_hijo(nodo)){
		abb_nodo_t* nodo_hijo;
		if(nodo->izq) nodo_hijo = nodo->izq;
		else if(nodo->der) nodo_hijo = nodo->der;
		
		if(!abb_nodo_raiz(arbol,nodo)){
			abb_nodo_t* nodo_padre = abb_nodo_buscar_padre(arbol->raiz,clave,arbol->cmp);

			if(abb_nodo_hijo_der(nodo_padre,nodo,arbol->cmp)){
				nodo_padre->der = nodo_hijo;
			}else if(abb_nodo_hijo_izq(nodo_padre,nodo,arbol->cmp)){
				nodo_padre->izq = nodo_hijo;
			}
		}
		else{
			arbol->raiz = nodo_hijo;
		}
	}else{ // SI TIENE 2 HIJOS.
		abb_nodo_t* sucesor = abb_nodo_minimo(nodo->der);
		abb_nodo_swap(nodo,sucesor);

		dato = abb_borrar_aux(nodo,clave,arbol->cmp);
		if(!dato) return NULL;
		arbol->cant--;
		return dato;
	}
	arbol->cant--;
	free(nodo->clave);
	free(nodo);
	return dato;
}

void abb_nodo_destruir(abb_nodo_t* nodo,abb_destruir_dato_t destruir_dato){

	if(!nodo) return;

	abb_nodo_t* hijo_izq = nodo->izq;
	abb_nodo_t* hijo_der = nodo->der;

	if(destruir_dato) destruir_dato(nodo->dato);
	free(nodo->clave);
	free(nodo);
	abb_nodo_destruir(hijo_izq, destruir_dato);
	abb_nodo_destruir(hijo_der, destruir_dato);
}

void abb_destruir(abb_t *arbol){

	abb_nodo_destruir(arbol->raiz,arbol->destruir_dato);
	free(arbol);
	return;
}

bool abb_es_nil(abb_nodo_t* arbol) {
	return (!arbol->clave);
}

// ITERADOR INTERNO //

bool iterar(abb_nodo_t* nodo, bool (*visitar)(const char *, void *, void *), void *extra) {
	
	if(!nodo) return true;

	if(!iterar(nodo->izq, visitar, extra)){
		return false;
	}
	if(visitar(nodo->clave, nodo->dato, extra)) {
		if(!iterar(nodo->der, visitar, extra)){
			return false;
		}   
		return true; 
	}
	return false;
}

void abb_in_order(abb_t *arbol, bool (*visitar)(const char *, void *, void *), void *extra){
	if(arbol){
		iterar(arbol->raiz, visitar, extra);	
	}
}	

// ITERADOR EXTERNO //

void apilar_izquierda(pila_t* pila,abb_nodo_t* nodo){
	if(nodo){
		pila_apilar(pila,nodo);
		apilar_izquierda(pila,nodo->izq);
	}
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if (!iter) return NULL;
	
	pila_t* pila = pila_crear();
	if (!pila) {
		free(iter);
		return NULL;
	}
	if (arbol->raiz) {
		pila_apilar(pila, arbol->raiz);
		apilar_izquierda(pila, arbol->raiz->izq);
	}

	iter->pila_iter = pila;
	return iter;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	abb_nodo_t* actual = pila_ver_tope(iter->pila_iter);
	return actual ? actual->clave : NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return (abb_iter_in_ver_actual(iter) == NULL);
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (abb_iter_in_al_final(iter)) return false;

	abb_nodo_t* nodo = pila_desapilar(iter->pila_iter);
	if (nodo->der != NULL) {
		pila_apilar(iter->pila_iter,nodo->der);
		apilar_izquierda(iter->pila_iter, nodo->der->izq);
	}
	return true;
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila_iter);
	free(iter);
}

