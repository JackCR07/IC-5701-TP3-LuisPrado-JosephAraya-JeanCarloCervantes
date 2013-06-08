/**************************************************
 *  
 * Arbol.h  
 * Header que contiene la declaración de
 * variables y funciones necesitadas para el parse tre
 * 
 * 
 * *****************************************/
typedef struct Atributo{//Representa atributos de elementos html 
  char* nombreAtributo;
	char* valorAtributo;
	struct Atributo* siguiente;
}Atributo_t;

typedef struct ListaAtributos{//Representa la lista de atributos que puede tener un elemento html
	Atributo_t* primerAtributo;
}ListaAtributos_t;

typedef struct Texto{
	char* texto;
	struct Texto* siguiente;
}Texto_t;


typedef struct ListaTextos{
	Texto_t* primerTexto;
}ListaTextos_t;


typedef struct DocType{
	char* valor1;
	char* valor2;
}DocType_t;

typedef struct Nodo //Representa cada elemento del arbol sintactico
{
	char* nombreNodo;
	ListaAtributos_t* listaAtributos;
	ListaTextos_t* listaTextos;
	struct Nodo* padre;
	struct Nodo* siguiente;//Siguiente nodo hermano(Es decir se encuentran al mismo nivel)
	struct ListaHijos* listaHijos;
}Nodo_t;


struct ListaHijos{//Representa la lista de hijos que tiene un nodo
	Nodo_t* primerHijo;//Primer hijo
};
typedef struct Arbol{
	Nodo_t* raizHtml;
	DocType_t* doctype;
}Arbol_t;

//Declaración de variables
Nodo_t* raizDocumento;
Nodo_t* nodoActual;
DocType_t* docTypeDocumento;
ListaAtributos_t* listaAtributosActual;
ListaTextos_t * listaTextosActual;
ListaTextos_t * listaTextosDocType;
int docTypeCreado=0;
Arbol_t* arbol;

//Declaración de funciones

Nodo_t* crearNodo(char* nombreNodo, Nodo_t* padre);
void agregarHijo(Nodo_t* padre, Nodo_t* hijo);
Atributo_t* crearAtributo(char* nombreAtributo, char* valorAtributo);
ListaAtributos_t* crearListaAtributos();
void agregarListaAtributos(Nodo_t* nodo, ListaAtributos_t* listaAtributos);
void agregarAtributo(ListaAtributos_t* listaAtributos, Atributo_t* atributo);
Texto_t* crearTexto(char* valorTexto);
void agregarTexto(ListaTextos_t* listaTextos, Texto_t* texto);
void agregarListaTextos(Nodo_t* nodo, ListaTextos_t* listaTextos);
ListaTextos_t* crearListaTextos();
void imprimirArbol(Nodo_t* raiz, int iProfundidad, int iAnchura, int numero[30]);
void printTabs(int cantidad);
void printArregloEntero(int numero[30], int profundidad );
DocType_t* crearDocType(char* valor1, char* valor2);
void preImprimirArbol(Arbol_t* arbol);
void imprimirDoctype(DocType_t* doctype);
