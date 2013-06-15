#include <stdio.h>
#include <stdlib.h>
#include "arbol.h"
#define BLUE "\e[34m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define WHITE "\e[m"
#define CYAN "\e[36m"
#define MAGENTA "\e[35m"

void agregarHijo(Nodo_t* padre, Nodo_t* hijo){
  if(padre->listaHijos->primerHijo==NULL)
		padre->listaHijos->primerHijo=hijo;
	else{
		Nodo_t* nodoTmp= padre->listaHijos->primerHijo;
		while(nodoTmp->siguiente!=NULL){
			nodoTmp=nodoTmp->siguiente;
		}
		nodoTmp->siguiente=hijo;
	}
}
//Metodo que crea nuevos elementos del arbol sintactico
//Recibe el valor del nodo, y el padre del nodo.
Nodo_t* crearNodo(char* nombreNodo,Nodo_t* padre){
	Nodo_t* nuevoNodo = malloc(sizeof(Nodo_t));
	nuevoNodo->listaAtributos= malloc(sizeof(ListaAtributos_t));
	nuevoNodo->listaTextos= malloc(sizeof(ListaTextos_t));
	nuevoNodo->nombreNodo=nombreNodo;
	nuevoNodo->padre=padre;
	nuevoNodo->siguiente=NULL;//Al no tener hermanos en este momento nodo siguiente se inicia en NULL
	nuevoNodo->listaHijos=malloc(sizeof(struct ListaHijos));
	if(padre!=NULL){
		agregarHijo(padre,nuevoNodo);
	}
	return nuevoNodo;
}

//Metodo que agrega un hijo a un nodo padre
//Recibe un nodo_t padre y un nodo_t hijo


Atributo_t* crearAtributo(char* nombreAtributo, char* valorAtributo){
	Atributo_t* nuevoAtributo = malloc(sizeof(Atributo_t));
	
	nuevoAtributo->nombreAtributo=nombreAtributo;
	nuevoAtributo->valorAtributo=valorAtributo;
	nuevoAtributo->siguiente=NULL;//Al no tener hermanos en este momento nodo siguiente se inicia en NULL
	return nuevoAtributo;
}
//Se agrega elatributo a una lista de atributos
void agregarAtributo(ListaAtributos_t* listaAtributos, Atributo_t* atributo){
	if(listaAtributos->primerAtributo==NULL)
		listaAtributos->primerAtributo=atributo;
	else{
		Atributo_t* atributoTmp= listaAtributos->primerAtributo;
		while(atributoTmp->siguiente!=NULL){
			atributoTmp=atributoTmp->siguiente;
		}
		atributoTmp->siguiente=atributo;
	}
}

void agregarListaAtributos(Nodo_t* nodo, ListaAtributos_t* listaAtributos){
	nodo->listaAtributos= listaAtributos;
}
ListaAtributos_t* crearListaAtributos(){
	ListaAtributos_t* listaAtributos = malloc(sizeof(ListaAtributos_t));
	return listaAtributos;
}

Texto_t* crearTexto(char* valorTexto){
	Texto_t* nuevoTexto = malloc(sizeof(Texto_t));
	
	nuevoTexto->texto= valorTexto;
	nuevoTexto->siguiente=NULL;//Al no tener hermanos en este momento nodo siguiente se inicia en NULL
	return nuevoTexto;
}
//Se agrega el texto a la lista de textos
void agregarTexto(ListaTextos_t* listaTextos, Texto_t* texto){
	if(listaTextos->primerTexto==NULL)
		listaTextos->primerTexto=texto;
	else{
		Texto_t* textoTmp= listaTextos->primerTexto;
		while(textoTmp->siguiente!=NULL){
			textoTmp=textoTmp->siguiente;
		}
		textoTmp->siguiente=texto;
	}
}


void agregarListaTextos(Nodo_t* nodo, ListaTextos_t* listaTextos){
	/*sto es por se se da la condicion
	 * <body>
	 * sjaklldsak<p>djskfldsjf</p> dsfjisipjfds
	 * </body>*/
	Texto_t* tmpTexto = nodo->listaTextos->primerTexto;
	if(tmpTexto!=NULL){
		while(tmpTexto->siguiente!=NULL){
			tmpTexto= tmpTexto->siguiente;
		}
		tmpTexto->siguiente= listaTextos->primerTexto;
	}
	else{
		nodo->listaTextos= listaTextos;
	}
}
ListaTextos_t* crearListaTextos(){
	ListaTextos_t* listaTextos = malloc(sizeof(ListaTextos_t));
	return listaTextos;
}

//IMpresión del arbol, el indice me indica en que nivel se encuentra
void printArregloEntero(int numero[30], int profundidad ){
	int i=0;
	while(profundidad--){
		printf(BLUE "%i."WHITE,numero[i]);
		i++;
	}
}
void printTabs(int cantidad){
	while(cantidad--){
		printf("    ");
	}
}
void imprimirDoctype(DocType_t* doctype){	
	printf("     "BLUE"1." RED" Doctype"GREEN " valor 1="YELLOW "%s "GREEN"valor 2 ="YELLOW " %s\n ",doctype->valor1, doctype->valor2);
}
void preImprimirArbol(Arbol_t* arbol){
	imprimirDoctype(arbol->doctype);
	int numero[30];
	numero[0]= 2;
	imprimirArbol(arbol->raizHtml->listaHijos->primerHijo,1,1, numero);
}



void imprimirArbol(Nodo_t* raiz, int iProfundidad, int iAnchura, int numero[30]){
	if(raiz!=NULL){
		printTabs(iProfundidad);
		printArregloEntero(numero, iProfundidad);
		printf(RED" %s "WHITE, raiz-> nombreNodo);
		Atributo_t* atributoTmp= raiz->listaAtributos->primerAtributo;
		if(atributoTmp!=NULL){
			while(atributoTmp!=NULL){
				printf(GREEN " %s"WHITE "=" YELLOW" %s"WHITE,atributoTmp->nombreAtributo,atributoTmp->valorAtributo);
				atributoTmp= atributoTmp->siguiente;
			}
		}
		Texto_t* textoTmp = raiz->listaTextos->primerTexto;
		printf(CYAN" texto = "WHITE);
		if(textoTmp!=NULL){
			while(textoTmp!=NULL){
				printf(MAGENTA "%s "WHITE,textoTmp->texto);
				textoTmp= textoTmp->siguiente;
			}
		}
		printf("%s","\n");
		Nodo_t* nodoTmp= raiz->listaHijos->primerHijo;
		iAnchura=1;
		while(nodoTmp!=NULL){
			numero[iProfundidad]= iAnchura;
			imprimirArbol(nodoTmp, iProfundidad+1, iAnchura, numero);
			nodoTmp=nodoTmp->siguiente;
			iAnchura++;
		}
	}
}

DocType_t* crearDocType(char* valor1, char* valor2){
	DocType_t* nuevoDocType= malloc(sizeof(DocType_t));
	nuevoDocType->valor1=valor1;
	nuevoDocType->valor2=valor2;
	return nuevoDocType;
	
}
