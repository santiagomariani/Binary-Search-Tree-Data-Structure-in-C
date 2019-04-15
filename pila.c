#include "pila.h"
#include <stdlib.h>

struct pila {
    void** datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};
#define MISMO 0
#define CAP_INICIAL 5
#define FACTOR_AUMENTO 2
#define FACTOR_REDUCCION 2
#define FACTOR_MINIMO 4

bool pila_redimencion(pila_t* pila, size_t tam_nuevo) {
		void* datos_nuevo = realloc(pila->datos, sizeof(void*) * tam_nuevo);
		if(datos_nuevo == NULL) return false;
		pila->datos = datos_nuevo;
		return true;
}

bool pila_ver_redimencion(pila_t* pila) {

	size_t tam_nuevo = MISMO;
	if(pila->cantidad == pila->capacidad) {
		tam_nuevo = FACTOR_AUMENTO * pila->capacidad;
	}
	else if(pila->cantidad <= (pila->capacidad / FACTOR_MINIMO)){
		tam_nuevo = pila->capacidad / FACTOR_REDUCCION;
	}

	if(tam_nuevo != MISMO){
		if(!pila_redimencion(pila, tam_nuevo)) return false;
		pila->capacidad = tam_nuevo;
	}
	return true;
}


pila_t* pila_crear(void) {

	pila_t* pila = malloc(sizeof(pila_t));
	if(pila == NULL) return NULL;

    pila->datos = malloc(sizeof(void*) * CAP_INICIAL);
    if(pila->datos == NULL) return NULL;

    pila->cantidad = 0;
    pila->capacidad = CAP_INICIAL;
    return pila;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
	return (pila->cantidad == 0);
}

bool pila_apilar(pila_t *pila, void* valor) {

	if(!pila_ver_redimencion(pila)) return false;
	size_t pos = pila->cantidad;
	pila->datos[pos] = valor;
	pila->cantidad++;
	return true;
}

void* pila_ver_tope(const pila_t *pila) {
	
	if(pila->cantidad == 0) return NULL;
	size_t pos_tope = pila->cantidad - 1;
	void* tope = pila->datos[pos_tope];
	return tope;
}

void* pila_desapilar(pila_t *pila) {

	void* tope_anterior = pila_ver_tope(pila);
	if(tope_anterior == NULL && pila->cantidad == 0) return NULL;
	if(!pila_ver_redimencion(pila)) return NULL;
	pila->cantidad--;
	return tope_anterior;
}


