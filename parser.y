/*******************************************************************************************
 * Tecnologico de Costa Rica                                                               *
 * Ingieneria en Computación                                                               *
 * Compiladores e Interpretes                                                              *
 * Analizador Semantico xhtml                                                              *
 * Tarea Programada 3                                                                      *
 * parser.y                                                                                *
 * Archivo que implementa un parser para xhtml                                             *
 *                                                                                         *
 * Estudiantes: Joseph Araya Rojas                                                         *
 *				Luis Prado Rodríguez                                                       *
 *				Jean Carlo Cervantes                                                       *
 *                                                                                         *
 * Profesor: Andrei Fuentes                                                                *
 * *****************************************************************************************/
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "arbol.h"
#include "semantico.h"
#define BLUE "\e[34m" 
#define RED "\e[31m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define WHITE "\e[m"
#define CYAN "\e[36m"
#define MAGENTA "\e[35m"

#define YYERROR_VERBOSE 1



//Variables usadas en el programa

char* etiquetaLeida;//Ultima etiqueta html leida;
char* nombreAtributoLeido;
char* valorAtributoLeido;
int docTypeCreado=0;
int filaAtributo=0;
int filaValor=0;
int columnaAtributo=0;
int columnaValor=0;
int errorSemantico=0;//Bandera que indica si ocurrio un error semantico, si ocurriera no se imprime el arbol sintactico



//Metodo que crea nuevos elementos del arbol sintactico
//Recibe el valor del nodo, y el padre del nodo.

void accionCabezaLeida_Arbl(char* nombreTag);
void accionColaLeida_Arbl();
 
void analizarSemantico(Nodo_t* raiz);

void yyerror(const char* s);

%} 

%union {
             
             char* str;
         }
%token T_dl T_html T_script T_dt T_img T_span
	T_a T_dd T_input T_strong T_b T_em T_li
	T_style T_blockquote T_embed T_link T_table
	T_body T_footer T_meta T_td T_br T_form 
	T_object T_th T_button T_h1 T_h2 T_h3
	T_h4 T_h5 T_h6 T_ol T_tr T_caption T_head
	T_option T_textarea T_code T_header T_p T_title
	T_div T_hr T_pre T_ul T_Atributtes T_Values T_Text 
	T_XML T_PublicIdentifier T_DOCTYPE
%locations

%%


xhtml : doctype html_tag
{
	analizarSemantico(arbol->raizHtml);
	if(!errorSemantico){
		preImprimirArbol(arbol);
	}
}
;
doctype : '<''!'dctype html pblid values values'>'
{
	docTypeCreado=1;// Me indica que el docktype ya fue creado
	docTypeDocumento= crearDocType(listaTextosDocType->primerTexto->texto,listaTextosDocType->primerTexto->siguiente->texto);
	arbol->doctype=docTypeDocumento;
	listaTextosDocType= crearListaTextos();
}
;
html_tag : html_h head_h body html_t 
;
html_h : '<'html atributos '>' {
	nodoActual= crearNodo("html",raizDocumento); 
	ListaAtributos_t* listaTmp =listaAtributosActual;
	agregarListaAtributos(nodoActual,listaTmp);
	listaAtributosActual=crearListaAtributos();
}//Agregado a la gramatica
;
html_t : '<' '/' html '>' //Agregado a la gramtica 
;
head_h : head_h2 head_tag 
;
head_h2 : '<' head atributos '>' //Agregado a la gramatica
{
	nodoActual= crearNodo("head",nodoActual); 
	ListaAtributos_t* listaTmp =listaAtributosActual;
	agregarListaAtributos(nodoActual,listaTmp);
	listaAtributosActual=crearListaAtributos();
}//Agregado a la gramatica
;
head_tag :  infoh head_t 
;
head_t : '<' '/' head '>' {
	nodoActual= nodoActual->padre;
}
;
infoh : meta title_h
| title_h
;
meta : '<' T_meta atributos '/' '>' 
{
	nodoActual= crearNodo("meta",nodoActual); 
	ListaAtributos_t* listaTmp =listaAtributosActual;
	agregarListaAtributos(nodoActual,listaTmp);
	listaAtributosActual=crearListaAtributos();
	nodoActual= nodoActual->padre;
}//Agregado a la gramatica
;
body : body_h body_tag
;
body_h: '<' T_body atributos '>' {
	nodoActual= crearNodo("body",nodoActual); 
	ListaAtributos_t* listaTmp =listaAtributosActual;
	agregarListaAtributos(nodoActual,listaTmp);
	listaAtributosActual=crearListaAtributos();
}//Agregado a la gramatica
;
body_tag :  tags body_tag
| body_t
;
body_t : '<' '/' T_body '>' 
{
	nodoActual= nodoActual->padre;
}
;
tags : a_h 
| dt_h 
| img_h 
| span_h 
| dl_h
| dd_h
| input_h
| strong_h
| b_h
| em_h
| li_h
| style_h
| blockquote_h
| embed_h
| link_h
| table_h
| footer_h
| td_h
| br_h
| form_h
| object_h
| th_h
| button_h
| h1_h
| h2_h
| h3_h
| h4_h
| h5_h
| h6_h
| ol_h
| tr_h
| caption_h
| option_h
| textarea
| code_h
| header_h
| p_h 
| title_h
| div_h
| hr_h
| pre_h
| ul_h
;
a_h : a_h2 a_tag //Modificacion gramtica
;
a_h2 : '<' T_a atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("a");
}
;
em_h : em_h2 em_tag
;
em_h2 : '<' T_em atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("em");
}
;
dt_h : dt_h2 dt_tag
;
dt_h2 : '<' T_dt atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("dt");
}
;
img_h : '<' T_img atributos '/''>'{
	nodoActual= crearNodo("img",nodoActual); 
	ListaAtributos_t* listaTmp =listaAtributosActual;
	agregarListaAtributos(nodoActual,listaTmp);
	listaAtributosActual=crearListaAtributos();
	nodoActual= nodoActual->padre;
}//Modificado borre img_tag !!!!!!!!!!!!!!!!!!!!!!!!!PREGUNTAR SI ES IMPORTANTE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
span_h : span_h2 span_tag
;
span_h2 : '<' T_span atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("span");
}
;
dl_h : dl_h2 dl_tag
;
dl_h2 : '<' T_dl atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("dl");
}
;
dd_h : dd_h2 dd_tag
;
dd_h2 : '<' T_dd atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("dd");
}
;
b_h : b_h2 b_tag
;
b_h2 : '<' T_b atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("b");
}
;
li_h : li_h2 li_tag
;
li_h2 : '<' T_li atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("li");
}
;
strong_h: strong_h2 strong_tag
;
strong_h2 : '<' T_strong atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("strong");
}
;
style_h : style_h2 style_tag
;
style_h2 : '<' T_style atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("style");
}
;
blockquote_h : blockquote_h2 blockquote_tag
;
blockquote_h2 : '<' T_blockquote atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("blockquote");
}
;
embed_h : embed_h2 embed_tag
;
embed_h2 : '<' T_embed atributos '>'//Agregado a la gramtica
{
	accionCabezaLeida_Arbl("embed");
}
;
link_h : link_h2 link_tag
;
link_h2 : '<' T_link atributos '>'
{
	accionCabezaLeida_Arbl("link");
}
;
table_h : table_h2 table_tag
;
table_h2 : '<' T_table atributos '>'
{
	accionCabezaLeida_Arbl("table");
}
;
footer_h : footer_h2 footer_tag
;
footer_h2 : '<' T_footer atributos '>'
{
	accionCabezaLeida_Arbl("footer");
}
;
td_h : td_h2 td_tag
;
td_h2 : '<' T_td atributos '>'
{
	accionCabezaLeida_Arbl("td");
}
;
br_h : br_h2 br_tag
;
br_h2 : '<' T_br atributos '>'
{
	accionCabezaLeida_Arbl("br");
}
;
form_h : form_h2 form_tag
;
form_h2 : '<' T_form atributos '>'
{
	accionCabezaLeida_Arbl("form");
}
;
object_h : object_h2 object_tag//Modificacion gramtica
;
object_h2: '<' T_object atributos '>'
{
	accionCabezaLeida_Arbl("object");
}
;
th_h : th_h2 th_tag
;
th_h2 : '<' T_th atributos '>'
{
	accionCabezaLeida_Arbl("th");
}
;
button_h : button_h2 button_tag
;
button_h2 : '<' T_button atributos '>'
{
	accionCabezaLeida_Arbl("button");
}
;
h1_h : h1_h2 h1_tag
;
h1_h2 : '<' T_h1 atributos '>'
{
	accionCabezaLeida_Arbl("h1");
}
;
h2_h : h2_h2 h2_tag
;
h2_h2 : '<' T_h2 atributos '>'
{
	accionCabezaLeida_Arbl("h2");
}
;
h3_h : h3_h2 h3_tag
;
h3_h2 : '<' T_h3 atributos '>'
{
	accionCabezaLeida_Arbl("h3");
}
;
h4_h : h4_h2 h4_tag
;
h4_h2 : '<' T_h4 atributos '>'
{
	accionCabezaLeida_Arbl("h4");
}
;
h5_h : h5_h2 h5_tag
;
h5_h2 : '<' T_h5 atributos '>'
{
	accionCabezaLeida_Arbl("h5");
}
;
h6_h : h6_h2 h6_tag
;
h6_h2 : '<' T_h6 atributos '>'
{
	accionCabezaLeida_Arbl("h6");
}
;
ol_h : ol_h2 ol_tag
;
ol_h2 : '<' T_ol atributos '>'
{
	accionCabezaLeida_Arbl("ol");
}
;
tr_h : tr_h2 tr_tag
;
tr_h2 : '<' T_tr atributos '>'
{
	accionCabezaLeida_Arbl("tr");
}
;
caption_h : caption_h2 caption_tag
;
caption_h2 : '<' T_caption atributos '>'
{
	accionCabezaLeida_Arbl("caption");
}
;
option_h : option_h2 option_tag
;
option_h2: '<' T_option atributos '>'
{
	accionCabezaLeida_Arbl("option");
}
;
textarea : textarea_h2 textarea_tag
;
textarea_h2: '<' T_textarea atributos '>'
{
	accionCabezaLeida_Arbl("textarea");
}
;
code_h : code_h2 code_tag
; 
code_h2: '<' T_code atributos '>'
{
	accionCabezaLeida_Arbl("code");
}
;
header_h : header_h2 header_tag
;
header_h2: '<' T_header atributos '>'
{
	accionCabezaLeida_Arbl("header");
}
;
p_h : p_h2 p_tag//Modificacion gramtica
;
p_h2: '<' T_p atributos '>' 
{
	accionCabezaLeida_Arbl("p");
}
//Agregado a la gramatica
;
title_h: title_h2 title_tag
;
title_h2: '<' T_title atributos '>' 
{
	//Si hay algun texto en la lista pertenecen al padre de title
	accionCabezaLeida_Arbl("title");
}
;
div_h : div_h2 div_tag
;
div_h2: '<' T_div atributos '>'
{
	accionCabezaLeida_Arbl("div");
}
;
hr_h : hr_h2 hr_tag
;
hr_h2: '<' T_hr atributos '>'
{
	accionCabezaLeida_Arbl("hr");
}
;
pre_h : pre_h2 pre_tag
;
pre_h2: '<' T_pre atributos '>'
{
	accionCabezaLeida_Arbl("pre");
}
;
ul_h : ul_h2 ul_tag
;
ul_h2: '<' T_ul atributos '>'
{
	accionCabezaLeida_Arbl("ul");
}
;
input_h : input_h2 input_tag 
;
input_h2: '<' T_input atributos '>'
{
	accionCabezaLeida_Arbl("input");
}
;
a_tag : text a_tag
| tags a_tag 
| a_t 
;
a_t : a_t2  
;
a_t2 :  '<' '/' T_a '>' 
{
	accionColaLeida_Arbl();
}
;
em_tag : text em_tag
| tags em_tag
| em_t
;
em_t : em_t2 
;
em_t2 :  '<' '/' T_em '>' 
{
	accionColaLeida_Arbl();
}
;
dt_tag : text dt_tag
| tags dt_tag
| dt_t
;
dt_t : dt_t2 
;
dt_t2 :  '<' '/' T_dt '>' 
{
	accionColaLeida_Arbl();
}
;
span_tag : text span_tag
| tags span_tag
| span_t
;
span_t : span_t2 
;
span_t2 :  '<' '/' T_span '>' 
{
	accionColaLeida_Arbl();
}
;
dl_tag : text dl_tag
| tags dl_tag
| dl_t
;
dl_t : dl_t2 
;
dl_t2 :  '<' '/' T_dl '>' 
{
	accionColaLeida_Arbl();
}
;
dd_tag : text dd_tag
| tags dd_tag
| dd_t
;
dd_t : dd_t2 
;
dd_t2 :  '<' '/' T_dd '>' 
{
	accionColaLeida_Arbl();
}
;
b_tag : text b_tag
| tags b_tag
| b_t
;
b_t : b_t2 
;
b_t2 :  '<' '/' T_b '>' 
{
	accionColaLeida_Arbl();
}
;
li_tag : text li_tag
| tags li_tag
| li_t
;
li_t : li_t2
;
li_t2 :  '<' '/' T_li '>' 
{
	accionColaLeida_Arbl();
}
;
strong_tag : text strong_tag
| tags strong_tag
| strong_t
;
strong_t : strong_t2 
;
strong_t2 :  '<' '/' T_strong '>' 
{
	accionColaLeida_Arbl();
}
;
style_tag : text style_tag
| tags style_tag
| style_t
;
style_t : style_t2 
;
style_t2 :  '<' '/' T_style '>' 
{
	accionColaLeida_Arbl();
}
;
blockquote_tag : text blockquote_tag
| tags blockquote_tag
| blockquote_t
;
blockquote_t : blockquote_t2 
;
blockquote_t2 :  '<' '/' T_blockquote '>' 
{
	accionColaLeida_Arbl();
}
;
embed_tag : text embed_tag
| tags embed_tag
| embed_t
;
embed_t : embed_t2 
;
embed_t2 :  '<' '/' T_embed '>' 
{
	accionColaLeida_Arbl();
}
;
link_tag : text link_tag
| tags link_tag
| link_t
;
link_t : link_t2 
;
link_t2 :  '<' '/' T_link '>' 
{
	accionColaLeida_Arbl();
}
;
table_tag : text table_tag
| tags table_tag
| table_t
;
table_t : table_t2 
;
table_t2 :  '<' '/' T_table '>' 
{
	accionColaLeida_Arbl();
}
;
footer_tag : text footer_tag
| tags footer_tag
| footer_t
;
footer_t : footer_t2 
;
footer_t2 :  '<' '/' T_footer '>' 
{
	accionColaLeida_Arbl();
}
;
td_tag : text td_tag
| tags td_tag
| td_t
;
td_t : td_t2 
;
td_t2 :  '<' '/' T_td '>' 
{
	accionColaLeida_Arbl();
}
;
br_tag : text br_tag
| tags br_tag
| br_t
;
br_t : br_t2 
;
br_t2 :  '<' '/' T_br '>' 
{
	accionColaLeida_Arbl();
}
;
form_tag : text form_tag
| tags form_tag
| form_t
;
form_t : form_t2 
;
form_t2 :  '<' '/' T_form '>' 
{
	accionColaLeida_Arbl();
}
;
object_tag : text object_tag
| tags object_tag
| object_t
;
object_t : object_t2 //Modificacion gramatica
;
object_t2: '<' '/' T_object '>'
{
	accionColaLeida_Arbl();
}
//Agregado a la gramatica
;
th_tag : text th_tag
| tags th_tag
| th_t
;
th_t : th_t2 
;
th_t2 :  '<' '/' T_th '>' 
{
	accionColaLeida_Arbl();
}
;
button_tag : text button_tag
| tags button_tag
| button_t
;
button_t : button_t2 
;
button_t2 :  '<' '/' T_button '>' 
{
	accionColaLeida_Arbl();
}
;
h1_tag : text h1_tag
| tags h1_tag
| h1_t
;
h1_t : h1_t2 
;
h1_t2 :  '<' '/' T_h1 '>'
{
	accionColaLeida_Arbl();
} 
;
h2_tag : text h2_tag
| tags h2_tag
| h2_t
;
h2_t : h2_t2 
;
h2_t2 :  '<' '/' T_h2 '>' 
{
	accionColaLeida_Arbl();
}
;
h3_tag : text h3_tag
| tags h3_tag
| h3_t
;
h3_t : h3_t2 
;
h3_t2 :  '<' '/' T_h3 '>' 
{
	accionColaLeida_Arbl();
}
;
h4_tag : text h4_tag
| tags h4_tag
| h4_t
;
h4_t : h4_t2 
;
h4_t2 :  '<' '/' T_h4 '>' 
{
	accionColaLeida_Arbl();
}
;
h5_tag : text h5_tag
| tags h5_tag
| h5_t
;
h5_t : h5_t2 
;
h5_t2 :  '<' '/' T_h5 '>' 
{
	accionColaLeida_Arbl();
}
;
h6_tag : text h6_tag
| tags h6_tag
| h6_t
;
h6_t : h6_t2 
;
h6_t2 :  '<' '/' T_h6 '>' 
{
	accionColaLeida_Arbl();
}
;
ol_tag : text ol_tag
| tags ol_tag
| ol_t
;
ol_t : ol_t2 
;
ol_t2 :  '<' '/' T_ol '>' 
{
	accionColaLeida_Arbl();
}
;
tr_tag : text tr_tag
| tags tr_tag
| tr_t
;
tr_t : tr_t2 
;
tr_t2 :  '<' '/' T_tr '>' 
{
	accionColaLeida_Arbl();
}
;
caption_tag : text caption_tag
| tags caption_tag
| caption_t
;
caption_t : caption_t2 
;
caption_t2 :  '<' '/' T_caption '>' 
{
	accionColaLeida_Arbl();
}
;
option_tag : text option_tag
| tags option_tag
| option_t
;
option_t : option_t2 
;
option_t2 :  '<' '/' T_option '>' 
{
	accionColaLeida_Arbl();
}
;
textarea_tag : text textarea_tag
| tags textarea_tag
| textarea_t
;
textarea_t : textarea_t2 
;
textarea_t2 :  '<' '/' T_textarea '>' 
{
	accionColaLeida_Arbl();
}
;
code_tag : text code_tag
| tags code_tag
| code_t
;
code_t : code_t2 
; 
code_t2 :  '<' '/' T_code '>' 
{
	accionColaLeida_Arbl();
}
;
header_tag : text header_tag
| tags header_tag
| header_t
;
header_t : header_t2 
;
header_t2 :  '<' '/' T_header '>' 
{
	accionColaLeida_Arbl();
}
;
p_tag : text p_tag
| tags p_tag
| p_t
;
p_t : p_t2  //Modificacion a la gramatica
; 
p_t2 :  '<' '/' T_p '>' 
{
	accionColaLeida_Arbl();
}
//Agregado a la gramatica
;
title_tag : text title_tag
| title_t
;
title_t : '<' '/' T_title '>' //PORQUE TITLE NO TIENE 
{
	accionColaLeida_Arbl();
}
;
div_tag : text div_tag
| tags div_tag
| div_t
;
div_t : div_t2 
;
div_t2 :  '<' '/' T_div '>' 
{
	accionColaLeida_Arbl();
}
;
hr_tag : text hr_tag
| tags hr_tag
| hr_t
;
hr_t : hr_t2 
;
hr_t2 :  '<' '/' T_hr '>' 
{
	accionColaLeida_Arbl();
}
;
pre_tag : text pre_tag
| tags pre_tag
| pre_t
;
pre_t : pre_t2 
;
pre_t2 :  '<' '/' T_pre '>' 
{
	accionColaLeida_Arbl();
}
;
ul_tag : text ul_tag
| tags ul_tag
| ul_t
;
ul_t : ul_t2 
;
ul_t2 :  '<' '/' T_ul '>' 
{
	accionColaLeida_Arbl();
}
;
input_tag : text input_tag
| tags input_tag
| input_t
;
input_t : input_t2 
;
input_t2 :  '<' '/' T_input '>' 
{
	accionColaLeida_Arbl();
}
;
atributos : atributo atributos
|
;
atributo : atributte'='values { //Agregado a la gramatica

	Atributo_t* nuevoAtributo = crearAtributo(nombreAtributoLeido, valorAtributoLeido, filaAtributo, columnaAtributo, filaValor, columnaValor);
	agregarAtributo(listaAtributosActual,nuevoAtributo);
}
	
;
text :T_Text{
	Texto_t* nuevoTexto = crearTexto(yylval.str, yylloc.first_line, yylloc.first_column);
	agregarTexto(listaTextosActual,nuevoTexto);
}
| T_Atributtes {
	Texto_t* nuevoTexto = crearTexto(yylval.str, yylloc.first_line, yylloc.first_column);
	agregarTexto(listaTextosActual,nuevoTexto);
}
| T_dl {agregarTexto(listaTextosActual,crearTexto("dl", yylloc.first_line, yylloc.first_column));}
| T_html {agregarTexto(listaTextosActual,crearTexto("html", yylloc.first_line, yylloc.first_column));}
| T_script {agregarTexto(listaTextosActual,crearTexto("script", yylloc.first_line, yylloc.first_column));}
| T_dt {agregarTexto(listaTextosActual,crearTexto("dt", yylloc.first_line, yylloc.first_column));}
| T_img {agregarTexto(listaTextosActual,crearTexto("img", yylloc.first_line, yylloc.first_column));}
| T_span {agregarTexto(listaTextosActual,crearTexto("span", yylloc.first_line, yylloc.first_column));}
| T_a {agregarTexto(listaTextosActual,crearTexto("a", yylloc.first_line, yylloc.first_column));}
| T_dd {agregarTexto(listaTextosActual,crearTexto("dd", yylloc.first_line, yylloc.first_column));}
| T_input {agregarTexto(listaTextosActual,crearTexto("input", yylloc.first_line, yylloc.first_column));} 
| T_strong {agregarTexto(listaTextosActual,crearTexto("strong", yylloc.first_line, yylloc.first_column));} 
| T_b {agregarTexto(listaTextosActual,crearTexto("b", yylloc.first_line, yylloc.first_column));}
| T_em {agregarTexto(listaTextosActual,crearTexto("em", yylloc.first_line, yylloc.first_column));} 
| T_li {agregarTexto(listaTextosActual,crearTexto("li", yylloc.first_line, yylloc.first_column));}
| T_style {agregarTexto(listaTextosActual,crearTexto("style", yylloc.first_line, yylloc.first_column));}
| T_blockquote {agregarTexto(listaTextosActual,crearTexto("blockquote", yylloc.first_line, yylloc.first_column));}
| T_embed {agregarTexto(listaTextosActual,crearTexto("embed", yylloc.first_line, yylloc.first_column));}
| T_link {agregarTexto(listaTextosActual,crearTexto("link", yylloc.first_line, yylloc.first_column));}
| T_table {agregarTexto(listaTextosActual,crearTexto("table", yylloc.first_line, yylloc.first_column));}
| T_body {agregarTexto(listaTextosActual,crearTexto("body", yylloc.first_line, yylloc.first_column));}
| T_footer {agregarTexto(listaTextosActual,crearTexto("footer", yylloc.first_line, yylloc.first_column));}
| T_meta {agregarTexto(listaTextosActual,crearTexto("meta", yylloc.first_line, yylloc.first_column));}
| T_td {agregarTexto(listaTextosActual,crearTexto("td", yylloc.first_line, yylloc.first_column));}
| T_br {agregarTexto(listaTextosActual,crearTexto("br", yylloc.first_line, yylloc.first_column));}
| T_form {agregarTexto(listaTextosActual,crearTexto("form", yylloc.first_line, yylloc.first_column));}
| T_object {agregarTexto(listaTextosActual,crearTexto("object", yylloc.first_line, yylloc.first_column));}
| T_th {agregarTexto(listaTextosActual,crearTexto("th", yylloc.first_line, yylloc.first_column));}
| T_button {agregarTexto(listaTextosActual,crearTexto("button", yylloc.first_line, yylloc.first_column));}
| T_h1 {agregarTexto(listaTextosActual,crearTexto("h1", yylloc.first_line, yylloc.first_column));}
| T_h2 {agregarTexto(listaTextosActual,crearTexto("h2", yylloc.first_line, yylloc.first_column));}
| T_h3 {agregarTexto(listaTextosActual,crearTexto("h3", yylloc.first_line, yylloc.first_column));}
| T_h4 {agregarTexto(listaTextosActual,crearTexto("h4", yylloc.first_line, yylloc.first_column));}
| T_h5 {agregarTexto(listaTextosActual,crearTexto("h5", yylloc.first_line, yylloc.first_column));}
| T_h6 {agregarTexto(listaTextosActual,crearTexto("h6", yylloc.first_line, yylloc.first_column));}
| T_ol {agregarTexto(listaTextosActual,crearTexto("ol", yylloc.first_line, yylloc.first_column));}
| T_tr {agregarTexto(listaTextosActual,crearTexto("tr", yylloc.first_line, yylloc.first_column));}
| T_caption {agregarTexto(listaTextosActual,crearTexto("caption", yylloc.first_line, yylloc.first_column));}
| T_head {agregarTexto(listaTextosActual,crearTexto("head", yylloc.first_line, yylloc.first_column));}
| T_option {agregarTexto(listaTextosActual,crearTexto("option", yylloc.first_line, yylloc.first_column));}
| T_textarea {agregarTexto(listaTextosActual,crearTexto("textarea", yylloc.first_line, yylloc.first_column));}
| T_code {agregarTexto(listaTextosActual,crearTexto("code", yylloc.first_line, yylloc.first_column));}
| T_header {agregarTexto(listaTextosActual,crearTexto("header", yylloc.first_line, yylloc.first_column));}
| T_p {agregarTexto(listaTextosActual,crearTexto("p", yylloc.first_line, yylloc.first_column));}
| T_title {agregarTexto(listaTextosActual,crearTexto("title", yylloc.first_line, yylloc.first_column));} 
| T_div {agregarTexto(listaTextosActual,crearTexto("div", yylloc.first_line, yylloc.first_column));}
| T_hr {agregarTexto(listaTextosActual,crearTexto("hr", yylloc.first_line, yylloc.first_column));}
| T_pre {agregarTexto(listaTextosActual,crearTexto("pre", yylloc.first_line, yylloc.first_column));}
| T_ul {agregarTexto(listaTextosActual,crearTexto("ul", yylloc.first_line, yylloc.first_column));}
;
html : T_html;
dctype : T_DOCTYPE
;
pblid : T_PublicIdentifier
;
values : T_Values {
	 if(docTypeCreado==0){
	 	int lenAtributo = strlen(yylval.str);
		char *nuevo = (char*)malloc(sizeof(char) * lenAtributo-1);
		strncpy(nuevo, yylval.str+1, lenAtributo-2);
		Texto_t* nuevoTexto = crearTexto(nuevo, yylloc.first_line, yylloc.first_column);
		agregarTexto(listaTextosDocType,nuevoTexto);
	 }
	 else{
	 	int lenAtributo = strlen(yylval.str);
		char *nuevo = (char*)malloc(sizeof(char) * lenAtributo-1);
		strncpy(nuevo, yylval.str+1, lenAtributo-2);
		valorAtributoLeido= nuevo;
		filaValor= yylloc.first_line;
		columnaValor= yylloc.first_column;
	} //Modificacion gramatica
}
;
head : T_head
;
atributte : T_Atributtes  {
	nombreAtributoLeido=  yylval.str;
	filaAtributo= yylloc.first_line;
	columnaAtributo= yylloc.first_column;
}
;

%%

int main() {
	docTypeCreado=0;
	raizDocumento= crearNodo("arbol",NULL);
	listaAtributosActual= crearListaAtributos();
	listaTextosActual= crearListaTextos();
	listaTextosDocType = crearListaTextos();
	arbol = malloc(sizeof(Arbol_t));
	arbol->raizHtml= raizDocumento;
	return yyparse();
}

void yyerror(const char* s)
{
 fprintf(stderr,RED"Error Sintactico "BLUE"Linea %d, Columna %d : "WHITE"%s\n", yylloc.first_line, yylloc.first_column, s);
}



//Accion que se ejecuta cuando se lee una cabeza de un elemento html
void accionCabezaLeida_Arbl(char* nombreTag){
	agregarListaTextos(nodoActual,listaTextosActual);
	listaTextosActual= crearListaTextos();
	nodoActual= crearNodo(nombreTag,nodoActual); 
	ListaAtributos_t* listaTmp =listaAtributosActual;
	agregarListaAtributos(nodoActual,listaTmp);
	listaAtributosActual=crearListaAtributos();
}

//Accion que se ejecuta cuando se lee una cola de un elemento html
void accionColaLeida_Arbl(){
	agregarListaTextos(nodoActual,listaTextosActual);
	listaTextosActual= crearListaTextos();
	nodoActual= nodoActual->padre;
}

void analizarSemantico(Nodo_t* raiz){
	if(raiz!=NULL){
		char* elemento = raiz-> nombreNodo;
		Atributo_t* atributoTmp= raiz->listaAtributos->primerAtributo;
		if(atributoTmp!=NULL){
			while(atributoTmp!=NULL){
				char* atributo = atributoTmp->nombreAtributo;
				char* valorAtributo = atributoTmp->valorAtributo;
				int filaA= atributoTmp->filaAtributo;
				int columnaA = atributoTmp->columnaAtributo;
				int filaV= atributoTmp->filaValor;
				int columnaV= atributoTmp->columnaValor;
				int resultAtributo = atributoValido(elemento, atributo,valorAtributo, filaA, columnaA, filaV, columnaV);
				if((resultAtributo== -1 || resultAtributo==-2) && errorSemantico==0){
					errorSemantico=1;
				}
				atributoTmp= atributoTmp->siguiente;
			}
		}
		Nodo_t* nodoTmp= raiz->listaHijos->primerHijo;
		while(nodoTmp!=NULL){
			analizarSemantico(nodoTmp);
			nodoTmp=nodoTmp->siguiente;
		}
	}
}
