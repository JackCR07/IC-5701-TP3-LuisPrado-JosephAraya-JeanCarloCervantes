/* 
 * Instituto Tecnológico de Costa Rica
 * Escuela de Computación
 * Curso de Compiladores e Interpretes
 * Profesor Andrei Fuentes
 * Estudiantes: Jean Carlo Cervantes
 * 	        Luis Prado Rodríguez
 		Joseph Araya Rojas
 * Tarea Programada #1
 * Archivo: flex
 */
 
%{
	#include "y.tab.h"
        FILE * ErrorFile;
	int chars = 1;
	int lines = 1;
	void CloseErrorFile();
	void OpenErrorFile();

	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = chars; yylloc.last_column = chars + yyleng - 1; \
    yylval.str = strdup(yytext);

%}


%option yylineno

 /* Definiciones */

Elemento	"acronym"|"address"|"basefont"|"blockquote"|"caption"|"colgroup"|"fieldset"|"frameset"|"isindex"|"noframes"|"noscript"|"optgroup"|"textarea"|"applet"|"button"|"center"|"iframe"|"legend"|"object"|"option"|"script"|"select"|"strike"|"strong"|"frame"|"input"|"label"|"param"|"style"|"table"|"tbody"|"tfoot"|"thead"|"title"|"abbr"|"area"|"base"|"body"|"cite"|"code"|"font"|"form"|"head"|"html"|"link"|"menu"|"meta"|"samp"|"small"|"span"|"bdo"|"big"|"col"|"del"|"dfn"|"dir"|"div"|"img"|"ins"|"kbd"|"map"|"pre"|"sub"|"sup"|"var"|"br"|"dd"|"dl"|"dt"|"em"|"h1"|"h2"|"h3"|"h4"|"h5"|"h6"|"hr"|"li"|"ol"|"td"|"th"|"tr"|"tt"|"ul"|"a"|"b"|"i"|"p"|"q"|"s"|"u"
Atributo	"accept-charset"|"xmlns"|"accept"|"accesskey"|"action"|"align"|"alink"|"archive"|"background"|"bgcolor"|"border"|"cellpadding"|"charoff"|"charset"|"checked"|"classid"|"clear"|"codebase"|"codetype"|"color"|"colspan"|"compact"|"content"|"coords"|"datetime"|"declare"|"defer"|"disabled"|"enctype"|"encoding"|"frameborder"|"headers"|"height"|"hreflang"|"hspace"|"http-equiv"|"ismap"|"label"|"language"|"longdesc"|"marginheight"|"marginwidth"|"maxlength"|"media"|"method"|"multiple"|"nohref"|"noresize"|"noshade"|"nowrap"|"object"|"onblur"|"onchange"|"onclick"|"ondblclick"|"onfocus"|"onkeydown"|"onkeypress"|"onkeyup"|"onload"|"onmousedown"|"onmousemove"|"onmouseout"|"onmouseover"|"onmouseup"|"onreset"|"onselect"|"onsubmit"|"onunload"|"profile"|"prompt"|"readonly"|"rowspan"|"rules"|"scheme"|"scope"|"scrolling"|"selected"|"shape"|"standby"|"start"|"style"|"summary"|"tabindex"|"target"|"title"|"type"|"usemap"|"valign"|"value"|"valuetype"|"version"|"vlink"|"vspace"|"width"|"frame"|"class"|"abbr"|"axis"|"char"|"cite"|"code"|"cols"|"data"|"face"|"href"|"lang"|"link"|"name"|"rows"|"size"|"span"|"text"|"src"|"alt"|"dir"|"for"|"rel"|"rev"|"id" 

/* Definicion de inicio de condiciones para el procesamiento de comentarios */
%x comment
%x single_line_comment

/* Reglas */
%%

"<"						{chars+=strlen(yytext); return yytext[0];}
">"						{chars+=strlen(yytext); return yytext[0];}
"-"						{chars+=strlen(yytext); return yytext[0];}
"="						{chars+=strlen(yytext); return yytext[0];}
":"						{chars+=strlen(yytext); return yytext[0];}
"/"						{chars+=strlen(yytext); return yytext[0];}
"?"						{chars+=strlen(yytext); return yytext[0];}
"!"						{chars+=strlen(yytext); return yytext[0];}
"PUBLIC"					{chars+=strlen(yytext); return T_PublicIdentifier;}
"DOCTYPE"					{chars+=strlen(yytext); return T_DOCTYPE;}
"dl"						{chars+=strlen(yytext); return T_dl;}
"html"						{chars+=strlen(yytext); return T_html;}
"script"					{chars+=strlen(yytext); return T_script;}
"dt"						{chars+=strlen(yytext); return T_dt;}
"img"						{chars+=strlen(yytext); return T_img;}
"span"						{chars+=strlen(yytext); return T_span;}
"a"						{chars+=strlen(yytext); return T_a;}
"dd"						{chars+=strlen(yytext); return T_dd;}
"input"						{chars+=strlen(yytext); return T_input;}
"strong"					{chars+=strlen(yytext); return T_strong;}
"b"						{chars+=strlen(yytext); return T_b;}
"em"						{chars+=strlen(yytext); return T_em;}
"li"						{chars+=strlen(yytext); return T_li;}
"style"						{chars+=strlen(yytext); return T_style;}
"blockquote"					{chars+=strlen(yytext); return T_blockquote;}
"embed"						{chars+=strlen(yytext); return T_embed;}
"link"						{chars+=strlen(yytext); return T_link;}
"table"						{chars+=strlen(yytext); return T_table;}
"body"						{chars+=strlen(yytext); return T_body;}
"footer"					{chars+=strlen(yytext); return T_footer;}
"meta"						{chars+=strlen(yytext); return T_meta;}
"td"						{chars+=strlen(yytext); return T_td;}
"br"						{chars+=strlen(yytext); return T_br;}
"form"						{chars+=strlen(yytext); return T_form;}
"object"					{chars+=strlen(yytext); return T_object;}
"th"						{chars+=strlen(yytext); return T_th;}
"button"					{chars+=strlen(yytext); return T_button;}
"h1"						{chars+=strlen(yytext); return T_h1;}
"h2"						{chars+=strlen(yytext); return T_h2;}
"h3"						{chars+=strlen(yytext); return T_h3;}
"h4"						{chars+=strlen(yytext); return T_h4;}
"h5"						{chars+=strlen(yytext); return T_h5;}
"h6"						{chars+=strlen(yytext); return T_h6;}
"ol"						{chars+=strlen(yytext); return T_ol;}
"tr"						{chars+=strlen(yytext); return T_tr;}
"caption"					{chars+=strlen(yytext); return T_caption;}
"head"						{chars+=strlen(yytext); return T_head;}
"option"					{chars+=strlen(yytext); return T_option;}
"textarea"					{chars+=strlen(yytext); return T_textarea;}
"code"						{chars+=strlen(yytext); return T_code;}
"header"					{chars+=strlen(yytext); return T_header;}
"p"						{chars+=strlen(yytext); return T_p;}
"title"						{chars+=strlen(yytext); return T_title;}
"div"						{chars+=strlen(yytext); return T_div;}
"hr"						{chars+=strlen(yytext); return T_hr;}
"pre"						{chars+=strlen(yytext); return T_pre;}
"ul"						{chars+=strlen(yytext); return T_ul;}
{Atributo}					{chars+=strlen(yytext); yylval.str=strdup(yytext);return T_Atributtes;}
"xml"						{chars+=strlen(yytext); return T_XML;}
\"(\\.|[^\\"])*\" 				{ chars+=strlen(yytext); yylval.str=strdup(yytext);return T_Values; }
"<!--"            				BEGIN(comment);
"//"            				BEGIN(single_line_comment);
[a-zA-Z0-9]+[a-zA-Z0-9]*    			{chars+=strlen(yytext); yylval.str=strdup(yytext);return T_Text;}
[+\-*\%;,.(){}\[\]_]				{chars+=strlen(yytext); yylval.str=strdup(yytext);return T_Text;}
<comment>[^-\n]*         		{} 
<comment>"-"+[^->\n]*    		{} 
<comment>\n              		{++lines;}
<comment>"--"+">"         		{BEGIN(INITIAL);}
<comment><<EOF>>        		{fprintf (stderr,"Error lexico: Unterminated comment ROW: %d", --lines); yyterminate();return T_Text;}
<single_line_comment>.   		{}
<single_line_comment>\n  		{++lines;BEGIN(INITIAL);}
[ \t\v\f]      					{ chars += strlen(yytext); }
[\n]       						{ lines++; chars = 1; }
. 								{ fprintf (stderr,"Error lexico caracter no valido: ROW: %d, COLUMN:%d\n", lines, chars); chars++; return T_Text;}

%%

void OpenErrorFile(){
 
	ErrorFile = fopen ("Errorfile.txt","w");
}

void CloseErrorFile(){
fclose(ErrorFile);
}
