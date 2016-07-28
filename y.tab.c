/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "trabalho.y" /* yacc.c:339  */

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

struct Tipo {
  string nome;  // nome na nossa linguagem
  string decl;  // declaração correspondente em c-assembly
  string fmt;   // formato para "printf"
  int dim;
};

Tipo Integer =	{ "numerosemponto", "int", "d" };
Tipo Float =	{ "numerocomponto", "float", "f" };
Tipo Double =	{ "numerograndecomponto", "double", "lf" };
Tipo Boolean =	{ "vouf", "int", "d" };
Tipo String =	{ "palavra", "char", "s" };
Tipo Char = 	{ "simbolo", "char", "c" };
Tipo Void = 	{"vazio", "void"};

struct Atributo {
  string v, c;
  Tipo t;
  vector<string> lst;
}; 

#define YYSTYPE Atributo

int yylex();
int yyparse();
void yyerror(const char *);
void erro( string );

vector< map<string,Tipo> > ts;
map<string, vector<Tipo> > tf; 

map< string, map< string, Tipo > > tiporesultado;

map< string, int > temp_global;
map< string, int > temp_local;
map< string, int > nlabel;
bool escopo_local = false;

struct Sw{
	Atributo var;
	vector<string> casos;
	vector<string> cod_casos;
};

Sw swvar;

int nsw = 0;

string toString( int n ) {
  char buf[256] = "";
  
  sprintf( buf, "%d", n );
  
  return buf;
}

int toInt( string valor ) {
  int aux = 0;
  
  sscanf( valor.c_str(), "%d", &aux );
  
  return aux;
}

void insereFuncao(string nome, Tipo retorno){

	vector<Tipo> v;

	if ( tf.find( nome ) != tf.end() ){
		
		v = tf[nome];
		for (int i = 0; i < v.size(); i++){
			if ( v[i].nome == retorno.nome){
				erro("A função de nome '"+ nome +"' com retorno '"+ retorno.nome +"' já existe!");			
			}
		}

		v.push_back(retorno);
	}
	else{

		v.push_back(retorno);
		tf[nome] = v;
	}

}

void empilha_nova_tabela_de_simbolos() {
  ts.push_back( map< string, Tipo >() );
}

void desempilha_tabela_de_simbolos() {
  ts.pop_back();
}


string gera_nome_var( Tipo t ) {
  return "t_" + t.nome + "_" + 
   toString( ++(escopo_local ? temp_local : temp_global)[t.nome] );
}

string gera_nome_label( string cmd ) {
  return "L_" + cmd + "_" + toString( ++nlabel[cmd] );
}

ostream& operator << ( ostream& o, const vector<string>& st ) {
  cout << "[ ";
  for( vector<string>::const_iterator itr = st.begin();
       itr != st.end(); ++itr )
    cout << *itr << " "; 
       
  cout << "]";
  return o;     
}


string trata_dimensoes_decl_var( Tipo t ) {
  string aux;
  
  if (t.dim != 0)
  		aux += toString(t.dim);
           
  return aux;
}

// 'Atributo&': o '&' significa passar por referência (modifica).
void declara_variavel( Atributo& ss, 
                       vector<string> lst, 
                       Tipo tipo,
					   string dimensaov) {
  ss.c = "";

  string dimensao;
  for( int i = 0; i < lst.size(); i++ ) {
    if( ts[ts.size()-1].find( lst[i] ) != ts[ts.size()-1].end() ) 
      erro( "Variável já declarada: " + lst[i] );
    else {

		if (tipo.nome == "palavra"){
			dimensao = trata_dimensoes_decl_var( tipo );
		}else{
			dimensao = dimensaov;
		}

		if (toInt(dimensaov) > 0 && toInt(trata_dimensoes_decl_var( tipo )) > 0 )
			dimensao = toString(toInt(trata_dimensoes_decl_var( tipo )) * toInt(dimensaov));

		if (toInt(dimensao) > 0)  dimensao = '[' + dimensao +']';
		else	dimensao = "";

		ts[ts.size()-1][ lst[i] ] = tipo; 
		ss.c += tipo.decl + " " + lst[i] +  dimensao + ";\n"; 
    }  
  }
}

void declara_variavel( Atributo& ss, string nome, Tipo tipo ) {
  vector<string> lst;
  lst.push_back( nome );
  declara_variavel( ss, lst, tipo, "");
}
	

void gera_codigo_atribuicao( Atributo& ss, 
                             const Atributo& s1,
							 const Atributo& s2,
                             const Atributo& s3 ) {
  
  if( s1.t.nome == s3.t.nome &&  s1.t.nome == "palavra" ) {
    ss.c = s1.c + s3.c + "  " 
           + "strncpy( " + s1.v + ", " + s3.v + ", " + 
           toString( s3.t.dim ) + " );\n";
  }else if( s1.t.nome == s3.t.nome ){
    ss.c = s1.c + s3.c + "  " + s1.v + s2.c + " = " + s3.v + ";\n";
  }

}

void gera_codigo_atribuicao_funcao( Atributo& ss, 
                             const Atributo& s1,
						     const Atributo& s2, 
                             const Atributo& s3 ) {

	vector<Tipo> v = tf[s3.v];

	bool existe = false;

	for (int i = 0; i < v.size(); i++){
		if ( v[i].nome == s1.t.nome){
			existe = true;
			break;	
		}
	}

	if (!existe)
		erro("A função '"+ s3.v +"' com retorno '"+ s1.t.nome +"' não existe!");

	if( s1.t.nome == "palavra" ) {
		ss.c = s1.c + s2.c + s3.c + "  " 
		       + "strncpy( " + s1.v + ", " + s3.v + ", " + 
		       toString( s1.t.dim) + " );\n";
  	}else{
    	ss.c = "  " + s1.v + s2.c + " =" + s3.c + "\n";
	}
}

void busca_tipo_da_variavel( Atributo& ss, const Atributo& s1 ) {

	bool encontrada = false;

	for (int i = ts.size()-1; i >= 0 ; i--){
		if( ts[i].find( s1.v ) != ts[i].end() ){
			ss.t = ts[i][ s1.v ];
			ss.v = s1.v;
			encontrada = true;
			break;
		}
	}

	if (!encontrada) 
		erro( "Variável não declarada: " + s1.v );
}

string par( Tipo a, Tipo b ) {
  return a.nome + "," + b.nome;  
}

void gera_codigo_operador( Atributo& ss,  const Atributo& s1, const Atributo& s2, const Atributo& s3 ) {

  if( tiporesultado.find( s2.v ) != tiporesultado.end() ) {

    if( tiporesultado[s2.v].find( par( s1.t, s3.t ) ) != tiporesultado[s2.v].end() ) {
     
      ss.t =  tiporesultado[s2.v][par( s1.t, s3.t )];
    
	  if (s1.t.nome == String.nome && s3.t.nome == String.nome){
			ss.c  = s1.c + s3.c + "  strncat("+ s1.v + ", " + s3.v + ", " + toString(s3.t.dim) +");\n";
	  }
	  else{
		ss.v = gera_nome_var( ss.t );
      	ss.c = s1.c + s3.c + "  " + ss.v + " = " + s1.v + s2.v + s3.v + ";\n";
	  }    
	}
    else
      erro( "O operador '" + s2.v + "' não está definido para os tipos " + s1.t.nome + " e " + s3.t.nome + "." );
 
  }
  else
    erro( "Operador '" + s2.v + "' não definido." );
}

void gera_codigo_operador_unario( Atributo& ss, const Atributo& op, const Atributo& value){
	
	if( (op.v == "!" && value.t.nome == "vouf") ||  
		((op.v == "+" || op.v == "-") && ( value.t.nome == "numerosemponto" 
											|| value.t.nome == "numerocomponto"
											|| value.t.nome == "numerograndecomponto" )) ){
		string temp1 = gera_nome_var(value.t);
		ss.c = value.c + "  " +  temp1 + " = " + op.v + value.v + ";\n";
		ss.v = temp1;
		ss.t = value.t;
	}
	else{
		erro("Não é possível utilizar operador unário: "+op.v+" com: "+value.t.nome);
	}

}


string declara_nvar_temp( Tipo t, int qtde ) {
  string aux = "";
   
  for( int i = 1; i <= qtde; i++ )
    aux += t.decl + " t_" + t.nome + "_" + toString( i ) + ";\n";
    
  return aux;  
}

string declara_var_temp( map< string, int >& temp ) {
  string decls = 
    declara_nvar_temp( Integer, temp[Integer.nome] ) +
    declara_nvar_temp( Float, temp[Float.nome] ) +
    declara_nvar_temp( Double, temp[Double.nome] ) +
    declara_nvar_temp( String, temp[String.nome] ) +
    declara_nvar_temp( Char, temp[Char.nome] ) +
    declara_nvar_temp( Boolean, temp[Boolean.nome] ) +
    "\n";
  
  temp.clear();
  
  return decls;
}

void gera_cmd_se(Atributo& ss, const Atributo& exp, const Atributo& cmd_ehverdade, string cmd_ehmentira){

  string lbl_ehverdade = gera_nome_label( "ehverdade" );
  string lbl_fim_se = gera_nome_label( "fim_se" );
  
  if( exp.t.nome != Boolean.nome )
    erro( "A expressão do SE deve ser booleana!" );

  ss.c = exp.c + 
         "\nif( " + exp.v + " ) goto " + lbl_ehverdade + ";\n" +
         cmd_ehmentira + "  goto " + lbl_fim_se + ";\n\n" +
         lbl_ehverdade + ":;\n" + 
         cmd_ehverdade.c + "\n" +
         lbl_fim_se + ":;\n";

}

void gera_cmd_repita(Atributo& ss, const Atributo& exp, const Atributo& cmd){

	string lbl_repita = gera_nome_label("repita");
	string lbl_fim_repita = gera_nome_label("fim_repita");

	if( exp.t.nome != Boolean.nome )
    	erro( "A expressão do REPITA deve ser booleana!" );

	ss.c = "\n" + lbl_repita + ":;\n" +
			exp.c +
          	"  "+ exp.v + "= !" + exp.v + ";\n" +
			"if ( "+ exp.v + " ) goto "+ lbl_fim_repita  + ";\n" +
			cmd.c +
			"  goto " + lbl_repita + ";\n\n"+
			lbl_fim_repita + ":;\n\n";	

}

void gera_cmd_execute_repita(Atributo& ss, const Atributo& exp, const Atributo& cmd){

	string lbl_execute = gera_nome_label("execute");

	if( exp.t.nome != Boolean.nome )
    	erro( "A expressão do EXECUTE/REPITA deve ser booleana!" );

	ss.c = "\n" + lbl_execute + ":;\n" +
			cmd.c + 
			exp.c +
			"if ( "+ exp.v + " ) goto "+ lbl_execute + ";\n";

}

void gera_cmd_com(Atributo& ss, const Atributo& atr1, const Atributo& atr2, const Atributo& exp, const Atributo& cmd){

	string lbl_fim_com = gera_nome_label("fim_com");
	string lbl_inicio_com = gera_nome_label("inicio_com");

	if( exp.t.nome != Boolean.nome )
    	erro( "A expressão do COM/FAÇA deve ser booleana!" );

	ss.c = atr1.c
			+ "\n" + lbl_inicio_com + ":;\n" +
			exp.c +
			"  "+ exp.v + "= !" + exp.v + ";\n" +
			"\nif ( "+ exp.v + " ) goto " + lbl_fim_com + ";\n"+
			cmd.c+
			atr2.c+
			"  goto " + lbl_inicio_com + ";\n\n"+
			lbl_fim_com + ":;\n\n";

}

void gera_labels(std::vector<string>* label, string tipo){

	string label_parte1 = "sw_" + toString(nsw) + "_";

	for (int i = 0; i < swvar.casos.size()-1 ; i++){
		
		if (tipo == "simbolo"){
			(*label).push_back( label_parte1 + swvar.casos[i].at(1) );
		}
		else{
			(*label).push_back( label_parte1 + swvar.casos[i] );
		} 
	}

	(*label).push_back( label_parte1 + "default" +":;\n");

}

void gera_cmd_switch(Atributo& ss){

	if (swvar.var.t.nome != "numerosemponto" && swvar.var.t.nome != "simbolo"){
		erro("ESCOLHA recebe numerosemponto e simbolo com parâmetros!\n");
	}
	
	vector<string> labels; 
	gera_labels(&labels, swvar.var.t.nome);

	string var_comparacao = "";
	
	for (int i = 0; i < swvar.casos.size()-1 ; i++){

		if (swvar.var.t.nome != swvar.var.t.nome) 
			erro ("Caso do ESCOLHA com tipo diferente de "+  swvar.var.t.nome);
		var_comparacao = gera_nome_var(Boolean);
		ss.c = ss.c + 
				var_comparacao + " = " + swvar.var.v + " == " + swvar.casos[i] + ";\n"+			
				"  if(" + var_comparacao + ") goto " + labels[i] + ";\n";
	}

	int indice = swvar.casos.size()-1;

	ss.c = "\n" + ss.c + labels[indice] + swvar.cod_casos[indice] + "  goto fim_sw_" + toString(nsw) + ";\n";

	for (int i = 0; i < swvar.casos.size()-1 ; i++){
		ss.c = ss.c + labels[i] + ":;\n" + swvar.cod_casos[i] + "  goto fim_sw_" + toString(nsw) + ";\n";
	}

	ss.c = ss.c + "fim_sw_" + toString(nsw) + ":;\n\n";

}

string ajeita_parametros_funcao(string params){

	if (params=="")
		return "";

	string s;
	
	while (params.find(";") != std::string::npos){
		s = (params.replace(params.find(";"), 2, ", "));
	}
	
	s = s.erase(s.size()-2);

	return s;
}


void gera_codigo_funcao_com_retorno( Atributo& ss,
						             string nome,
									 const Atributo& retorno,
									 string var_retorno,
						             string params,
						             const Atributo& vars,
						             const Atributo& codigo ) {

	string tipo_retorno = retorno.t.decl;
	string codigo_retorno = retorno.c;

	if (retorno.t.nome == String.nome){
		tipo_retorno = "char*";
		codigo_retorno = tipo_retorno + var_retorno + ";\n";
	}

	string new_params = ajeita_parametros_funcao(params);

	ss.c = tipo_retorno + " " + nome + "( " + new_params + " ) {\n  " +
			codigo_retorno +
			declara_var_temp( temp_local ) + "  " +
			vars.c +
			codigo.c +
			"  return " + var_retorno +";\n}\n\n";
}

void gera_codigo_funcao_sem_retorno( Atributo& ss,
						             string nome, 
						             string params,
						             const Atributo& vars,
						             const Atributo& codigo ){

	string new_params = ajeita_parametros_funcao(params);

	ss.c = "void " + nome + "( " + new_params + " ) {\n" +
		 declara_var_temp( temp_local ) + "  " + 
		 vars.c +
		 codigo.c +
		 "\n}\n\n";
} 


string criachamadafuncao( Atributo& ss, Atributo& s1, Atributo& s3){

	if ( tf.find( s1.v ) == tf.end() ){
		erro("Funcao " + s1.v + " não existe!");	
	}
	
	return "  " + s1.v + "(" + s3.v + ");\n";
}

void checa_tipo_exp(string tipo1, string tipo2){

	if (tipo1 != Integer.nome && tipo2 != Integer.nome)
		erro("Indices devem ser do tipo numerocomponto");
}


#line 566 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _ID = 258,
    _TUDAO = 259,
    _USANDOISSO = 260,
    _EXECUTEISSO = 261,
    _SE = 262,
    _EHVERDADE = 263,
    _EHMENTIRA = 264,
    _MOSTRE = 265,
    _VALE = 266,
    _COM = 267,
    _FACA = 268,
    _ENQUANTO = 269,
    _REPITA = 270,
    _EXECUTE = 271,
    _LE = 272,
    _NUMEROSEMPONTO = 273,
    _PALAVRA = 274,
    _NUMEROCOMPONTO = 275,
    _NUMEROGRANDECOMPONTO = 276,
    _SIMBOLO = 277,
    _VOUF = 278,
    _ESCOLHA = 279,
    _SEFOR = 280,
    _OK = 281,
    _CASOCONTRARIO = 282,
    _FUNCAO = 283,
    _RECEBE = 284,
    _RETORNA = 285,
    _NADA = 286,
    _CTE_PALAVRA = 287,
    _CTE_NUMEROSEMPONTO = 288,
    _CTE_NUMEROCOMPONTO = 289,
    _CTE_NUMEROGRANDECOMPONTO = 290,
    _CTE_SIMBOLO = 291,
    _RESTO = 292,
    _SOBRE = 293,
    _X = 294,
    _OU = 295,
    _E = 296,
    _NAO = 297
  };
#endif
/* Tokens.  */
#define _ID 258
#define _TUDAO 259
#define _USANDOISSO 260
#define _EXECUTEISSO 261
#define _SE 262
#define _EHVERDADE 263
#define _EHMENTIRA 264
#define _MOSTRE 265
#define _VALE 266
#define _COM 267
#define _FACA 268
#define _ENQUANTO 269
#define _REPITA 270
#define _EXECUTE 271
#define _LE 272
#define _NUMEROSEMPONTO 273
#define _PALAVRA 274
#define _NUMEROCOMPONTO 275
#define _NUMEROGRANDECOMPONTO 276
#define _SIMBOLO 277
#define _VOUF 278
#define _ESCOLHA 279
#define _SEFOR 280
#define _OK 281
#define _CASOCONTRARIO 282
#define _FUNCAO 283
#define _RECEBE 284
#define _RETORNA 285
#define _NADA 286
#define _CTE_PALAVRA 287
#define _CTE_NUMEROSEMPONTO 288
#define _CTE_NUMEROCOMPONTO 289
#define _CTE_NUMEROGRANDECOMPONTO 290
#define _CTE_SIMBOLO 291
#define _RESTO 292
#define _SOBRE 293
#define _X 294
#define _OU 295
#define _E 296
#define _NAO 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 698 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   414

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  95
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  223

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    60,     2,
      55,    56,    51,    49,    58,    50,     2,    52,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    57,    59,
      44,    45,    43,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    61,     2,    62,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    53,     2,    54,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    46,    47,
      48
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   525,   525,   528,   538,   539,   542,   545,   550,   548,
     557,   565,   572,   574,   577,   580,   582,   585,   586,   589,
     592,   594,   597,   599,   600,   603,   604,   605,   606,   607,
     608,   611,   613,   617,   620,   623,   624,   627,   633,   636,
     639,   642,   646,   649,   652,   653,   654,   655,   656,   657,
     658,   659,   662,   671,   677,   678,   681,   687,   691,   694,
     698,   700,   704,   708,   711,   714,   718,   721,   723,   727,
     731,   735,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   750,   751,   752,   753,   754,   755,   758,
     759,   760,   761,   762,   763,   764
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_ID", "_TUDAO", "_USANDOISSO",
  "_EXECUTEISSO", "_SE", "_EHVERDADE", "_EHMENTIRA", "_MOSTRE", "_VALE",
  "_COM", "_FACA", "_ENQUANTO", "_REPITA", "_EXECUTE", "_LE",
  "_NUMEROSEMPONTO", "_PALAVRA", "_NUMEROCOMPONTO",
  "_NUMEROGRANDECOMPONTO", "_SIMBOLO", "_VOUF", "_ESCOLHA", "_SEFOR",
  "_OK", "_CASOCONTRARIO", "_FUNCAO", "_RECEBE", "_RETORNA", "_NADA",
  "_CTE_PALAVRA", "_CTE_NUMEROSEMPONTO", "_CTE_NUMEROCOMPONTO",
  "_CTE_NUMEROGRANDECOMPONTO", "_CTE_SIMBOLO", "_RESTO", "_SOBRE", "_X",
  "_OU", "_E", "_NAO", "'>'", "'<'", "'='", "\">=\"", "\"<=\"", "\"!=\"",
  "'+'", "'-'", "'*'", "'/'", "'{'", "'}'", "'('", "')'", "':'", "','",
  "';'", "'&'", "'['", "']'", "$accept", "S", "TUDAO", "USANDOISSO",
  "FUNCTIONDECLS", "FUNCTIONDECL", "$@1", "NOME_FUNCAO", "PARAMETROS",
  "EXECUTEISSO", "DECLS", "DECL", "TAMANHOARRAY", "IDS", "TIPO",
  "TAM_PALAVRA", "MIOLOS", "MIOLO", "CHAMADAFUNCAO", "PARAM_CHAMADA", "ES",
  "CMDS", "CMD", "CMD_ESCOLHA", "ESCOLHAS", "CASOS", "CASO",
  "CASOCONTRARIO", "MOSTRE", "LE", "CMD_ATRIB", "CMD_ATRIB_SPV", "IDATR",
  "INDICE", "CMD_SE", "CMD_FOR", "CMD_WHILE", "CMD_DOWHILE", "E", "F", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,    62,    60,    61,   298,   299,   300,    43,
      45,    42,    47,   123,   125,    40,    41,    58,    44,    59,
      38,    91,    93
};
# endif

#define YYPACT_NINF -124

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-124)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      15,   -32,    24,  -124,    17,  -124,   -25,    -1,    26,    28,
      27,    -1,   -17,  -124,   -22,   -20,   -40,    11,   -11,   -10,
    -124,    26,  -124,    26,    -9,    40,  -124,   140,  -124,    -6,
     -12,  -124,  -124,   -16,  -124,  -124,  -124,  -124,    14,  -124,
      -4,    -3,    59,    -2,    53,     8,    60,    22,    29,   140,
    -124,  -124,  -124,  -124,  -124,  -124,    20,  -124,  -124,  -124,
    -124,   -23,    26,    45,  -124,    43,  -124,    59,    59,  -124,
    -124,  -124,  -124,  -124,  -124,    31,    59,    59,    59,    59,
     180,  -124,    60,    32,   198,  -124,    25,    35,  -124,  -124,
      59,    78,    42,    17,  -124,    36,    67,    46,  -124,   214,
     230,    59,   352,   -36,   -36,   244,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,  -124,    47,
      20,    59,    50,   198,  -124,    81,   127,    96,   104,    27,
    -124,  -124,    51,    59,   105,   317,  -124,   342,   352,   362,
     362,   362,   362,   362,   362,   -36,   -36,  -124,  -124,    99,
     106,   258,   101,  -124,    22,    64,    93,    81,    61,    -4,
    -124,   197,    66,    70,  -124,  -124,    68,    59,    71,    59,
      83,   130,    77,  -124,    82,  -124,  -124,    59,  -124,    -9,
    -124,   198,   272,    60,   330,   198,    89,   140,   140,   147,
      92,    95,  -124,    98,   109,    59,   132,  -124,  -124,  -124,
     150,   146,  -124,   286,  -124,   112,   113,   125,   122,    17,
     198,    59,  -124,    27,   128,   300,   129,  -124,   131,  -124,
     198,   148,  -124
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     5,     1,     0,     7,     0,     0,
      16,     7,     0,    24,     0,     0,     0,     0,     0,     0,
       6,    14,     4,    18,     0,     0,    11,    34,     3,     0,
      13,    17,    25,    32,    26,    27,    29,    30,    22,    23,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    34,
      35,    36,    51,    44,    45,    47,    66,    46,    48,    49,
      50,     0,    14,     0,    28,    21,    19,    39,     0,    94,
      89,    90,    91,    92,    93,     0,     0,     0,     0,     0,
       0,    88,     0,     0,    43,    63,     0,     0,    15,    33,
       0,     0,     0,     5,    12,     0,     0,     0,    38,    41,
       0,     0,    87,    85,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      66,     0,     0,    43,    59,    55,     0,     0,     0,    16,
      31,    20,     0,     0,     0,     0,    95,    82,    81,    76,
      77,    84,    79,    78,    80,    72,    73,    74,    75,     0,
       0,     0,     0,    42,     0,     0,     0,    55,    65,    94,
      61,     0,     0,     0,    37,    40,     0,     0,     0,     0,
       0,     0,     0,    52,     0,    53,    54,     0,    60,     0,
      10,    43,     0,     0,    62,    43,     0,    34,    34,     0,
       0,     0,    83,     0,     0,     0,     0,    57,    64,     8,
      67,     0,    70,     0,    56,     0,     0,     0,     0,     5,
      43,     0,    71,    16,     0,     0,     0,    68,     0,     9,
      43,     0,    69
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -124,  -124,  -124,   -90,   174,  -124,  -124,  -124,   124,  -123,
     177,   -13,  -124,  -124,    33,  -124,   -47,  -124,    76,  -124,
      73,  -105,   -26,  -124,  -124,    54,  -124,  -124,  -124,  -124,
    -124,    21,   -41,    87,  -124,  -124,  -124,  -124,   -42,   -43
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     7,    10,    11,   205,    12,    29,    19,
      14,    15,    66,    16,    38,    64,    48,    49,    50,    97,
      98,   122,   123,    52,   155,   156,   157,   175,    53,    54,
      55,   119,    56,    91,    57,    58,    59,    60,    99,    81
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      80,    51,    89,   129,    87,    86,   163,    92,    30,    32,
      33,    34,    35,    36,    37,   116,   117,    24,   153,     1,
      25,     4,     6,    51,     5,    69,   100,     9,     8,    13,
      93,    17,    22,    18,   102,   103,   104,   105,    21,    23,
      26,   120,    27,    39,    28,    63,    62,    65,   126,    30,
      61,    67,    68,    82,    70,    71,    72,    73,    74,   135,
      83,    84,    69,    85,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   191,    79,    95,   151,
     194,    90,    96,    88,   124,   161,   101,   121,   125,   127,
     216,    70,    71,    72,    73,    74,    75,   128,   130,   159,
     131,    76,   132,   149,   152,   214,   154,   162,    77,    78,
     164,   172,   168,   166,    79,   221,   171,   169,   173,   213,
     174,   181,   177,   179,   180,   182,   183,   184,    70,    71,
      72,    73,    74,    75,   187,   189,   185,   186,    76,   188,
     196,   197,   120,    40,   195,    77,    78,    41,   199,   200,
      42,    79,    43,   203,   201,    44,    45,    46,   204,   206,
     207,    51,    51,   202,    47,   209,   210,   106,   107,   215,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     211,   212,   217,   219,   220,    20,    94,   106,   107,   158,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      31,    85,   222,   160,   193,    41,   165,   150,    42,   198,
      43,   176,   190,    44,    45,    46,     0,     0,     0,     0,
     106,   107,    47,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,     0,     0,     0,     0,   106,   107,   118,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
       0,     0,     0,     0,   106,   107,   178,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,     0,     0,     0,
     106,   107,   133,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,     0,   106,   107,   134,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,     0,   106,   107,
     136,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,     0,   106,   107,   170,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,     0,   106,   107,   192,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,     0,
     106,   107,   208,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,     0,     0,   167,   218,   106,   107,     0,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     106,   107,     0,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   107,     0,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117
};

static const yytype_int16 yycheck[] =
{
      42,    27,    49,    93,    47,    46,   129,    30,    21,    18,
      19,    20,    21,    22,    23,    51,    52,    57,   123,     4,
      60,    53,     5,    49,     0,     3,    68,    28,    53,     3,
      53,     3,    54,     6,    76,    77,    78,    79,    55,    59,
      29,    82,    53,     3,    54,    61,    58,    33,    90,    62,
      56,    55,    55,    55,    32,    33,    34,    35,    36,   101,
       7,    53,     3,     3,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   181,    55,    33,   121,
     185,    61,    39,    54,    59,   127,    55,    55,    53,    11,
     213,    32,    33,    34,    35,    36,    37,    55,    62,     3,
      33,    42,    56,    56,    54,   210,    25,     3,    49,    50,
      59,   154,    13,     8,    55,   220,    15,    11,    54,   209,
      27,    53,    61,    57,    54,   167,    55,   169,    32,    33,
      34,    35,    36,    37,    57,   177,    53,     7,    42,    57,
     187,   188,   183,     3,    55,    49,    50,     7,    56,    54,
      10,    55,    12,   195,    56,    15,    16,    17,    26,     9,
      14,   187,   188,    54,    24,    53,    53,    40,    41,   211,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      55,    59,    54,    54,    53,    11,    62,    40,    41,    62,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      23,     3,    54,   127,   183,     7,   133,   120,    10,    62,
      12,   157,   179,    15,    16,    17,    -1,    -1,    -1,    -1,
      40,    41,    24,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    -1,    -1,    -1,    -1,    40,    41,    59,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      -1,    -1,    -1,    -1,    40,    41,    59,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    -1,    -1,    -1,
      40,    41,    58,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    -1,    40,    41,    56,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    -1,    40,    41,
      56,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    -1,    40,    41,    56,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    -1,    40,    41,    56,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    -1,
      40,    41,    56,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    -1,    -1,    38,    56,    40,    41,    -1,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      40,    41,    -1,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    41,    -1,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    64,    65,    53,     0,     5,    66,    53,    28,
      67,    68,    70,     3,    73,    74,    76,     3,     6,    72,
      67,    55,    54,    59,    57,    60,    29,    53,    54,    71,
      74,    73,    18,    19,    20,    21,    22,    23,    77,     3,
       3,     7,    10,    12,    15,    16,    17,    24,    79,    80,
      81,    85,    86,    91,    92,    93,    95,    97,    98,    99,
     100,    56,    58,    61,    78,    33,    75,    55,    55,     3,
      32,    33,    34,    35,    36,    37,    42,    49,    50,    55,
     101,   102,    55,     7,    53,     3,    95,   102,    54,    79,
      61,    96,    30,    53,    71,    33,    39,    82,    83,   101,
     101,    55,   101,   101,   101,   101,    40,    41,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    59,    94,
      95,    55,    84,    85,    59,    53,   101,    11,    55,    66,
      62,    33,    56,    58,    56,   101,    56,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,    56,
      96,   101,    54,    84,    25,    87,    88,    89,    62,     3,
      81,   101,     3,    72,    59,    83,     8,    38,    13,    11,
      56,    15,   102,    54,    27,    90,    88,    61,    59,    57,
      54,    53,   101,    55,   101,    53,     7,    57,    57,   101,
      77,    84,    56,    94,    84,    55,    79,    79,    62,    56,
      54,    56,    54,   101,    26,    69,     9,    14,    56,    53,
      53,    55,    59,    66,    84,   101,    72,    54,    56,    54,
      53,    84,    54
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    63,    64,    65,    66,    66,    67,    67,    69,    68,
      68,    70,    71,    71,    71,    72,    72,    73,    73,    74,
      75,    75,    75,    76,    76,    77,    77,    77,    77,    77,
      77,    78,    78,    79,    79,    80,    80,    81,    82,    82,
      83,    83,    84,    84,    85,    85,    85,    85,    85,    85,
      85,    85,    86,    87,    88,    88,    89,    90,    91,    92,
      93,    93,    94,    95,    96,    96,    96,    97,    97,    98,
      99,   100,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   102,
     102,   102,   102,   102,   102,   102
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     6,     4,     0,     2,     0,     0,    15,
       8,     3,     3,     1,     0,     4,     0,     3,     2,     4,
       3,     1,     0,     3,     1,     1,     1,     1,     2,     1,
       1,     3,     0,     2,     0,     1,     1,     5,     1,     0,
       3,     1,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     5,     2,     2,     0,     5,     3,     3,     3,
       5,     4,     4,     1,     6,     3,     0,     8,    12,    15,
       8,    10,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     6,     3,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 525 "trabalho.y" /* yacc.c:1646  */
    { cout << (yyvsp[0]).c << endl; }
#line 1970 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 529 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "#include <stdlib.h>\n"
                    "#include <string.h>\n"
                    "#include <stdio.h>\n\n" + 
                    declara_var_temp( temp_global ) +
                    (yyvsp[-3]).c + "\n" + (yyvsp[-2]).c +
                    "int main() {\n" + (yyvsp[-1]).c + "}\n";
       }
#line 1982 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 538 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c; }
#line 1988 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 539 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 1994 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 543 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c; }
#line 2000 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 545 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2006 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 550 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyvsp[-1]), (yyvsp[-3]).v, (yyvsp[-1]).t ); 
					insereFuncao((yyvsp[-9]).v,(yyvsp[-1]).t); }
#line 2013 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 553 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_funcao_com_retorno( (yyval), (yyvsp[-14]).v, (yyvsp[-6]), (yyvsp[-8]).v, (yyvsp[-12]).c, (yyvsp[-2]), (yyvsp[-1]) ); 
					escopo_local = false;
            		desempilha_tabela_de_simbolos(); }
#line 2021 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 559 "trabalho.y" /* yacc.c:1646  */
    { insereFuncao((yyvsp[-7]).v, Void);
					gera_codigo_funcao_sem_retorno( (yyval), (yyvsp[-7]).v, (yyvsp[-5]).c, (yyvsp[-2]), (yyvsp[-1]) ); 
					escopo_local = false;
					desempilha_tabela_de_simbolos(); }
#line 2030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 566 "trabalho.y" /* yacc.c:1646  */
    { escopo_local = true; 
					empilha_nova_tabela_de_simbolos();
					(yyval).v = (yyvsp[-1]).v;}
#line 2038 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 573 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + (yyvsp[0]).c; }
#line 2044 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 575 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[0]).c; }
#line 2050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 577 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2056 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 581 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[-1]).c; }
#line 2062 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 582 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = "";}
#line 2068 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 585 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + (yyvsp[0]).c; }
#line 2074 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 589 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyval), (yyvsp[-3]).lst, (yyvsp[-1]).t, (yyvsp[0]).v ); }
#line 2080 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 593 "trabalho.y" /* yacc.c:1646  */
    { (yyval).v = toString(toInt((yyvsp[-2]).v) * toInt((yyvsp[0]).v));}
#line 2086 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 595 "trabalho.y" /* yacc.c:1646  */
    { (yyval).v = toString(toInt((yyvsp[0]).v)); }
#line 2092 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 597 "trabalho.y" /* yacc.c:1646  */
    { (yyval).v = "";}
#line 2098 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 599 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst = (yyvsp[-2]).lst; (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 2104 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 600 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 2110 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 603 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Integer; }
#line 2116 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 604 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Float; }
#line 2122 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 605 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Double; }
#line 2128 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 606 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = (yyvsp[0]).t; }
#line 2134 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 607 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Char; }
#line 2140 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 608 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Boolean; }
#line 2146 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 612 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; (yyval).t.dim = toInt( (yyvsp[-1]).v ); }
#line 2152 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 613 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; }
#line 2158 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 618 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c; }
#line 2164 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 620 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2170 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 628 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + criachamadafuncao((yyval), (yyvsp[-4]), (yyvsp[-2]));
				  (yyval).v = (yyvsp[-4]).v;}
#line 2177 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 634 "trabalho.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]); }
#line 2183 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 636 "trabalho.y" /* yacc.c:1646  */
    {(yyval).v = (yyval).c = ""; }
#line 2189 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 640 "trabalho.y" /* yacc.c:1646  */
    {  	(yyval).c = (yyvsp[-2]).c + (yyvsp[0]).c;
		(yyval).v = (yyvsp[-2]).v + ',' + (yyvsp[0]).v; }
#line 2196 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 643 "trabalho.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]); }
#line 2202 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 647 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c; }
#line 2208 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 649 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2214 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 663 "trabalho.y" /* yacc.c:1646  */
    { swvar.var = (yyvsp[-3]) ; 
			   gera_cmd_switch((yyval));
			   nsw++;
			   Sw clear_sw;
			   swvar = clear_sw;
			 }
#line 2225 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 672 "trabalho.y" /* yacc.c:1646  */
    { swvar.casos.push_back("default");
			  swvar.cod_casos.push_back((yyvsp[0]).c);
			}
#line 2233 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 682 "trabalho.y" /* yacc.c:1646  */
    { swvar.casos.push_back((yyvsp[-3]).v);
		  swvar.cod_casos.push_back((yyvsp[-1]).c);
		}
#line 2241 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 688 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[0]).c;}
#line 2247 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 691 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  printf( \"%"+ (yyvsp[-1]).t.fmt + "\\n\", " + (yyvsp[-1]).v + " );\n"; }
#line 2253 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 695 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  scanf( \"%"+ (yyvsp[-1]).t.fmt + "\", &"+ (yyvsp[-1]).v + " );\n"; }
#line 2259 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 699 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_atribuicao( (yyval), (yyvsp[-4]), (yyvsp[-3]), (yyvsp[-1])); }
#line 2265 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 701 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_atribuicao_funcao((yyval), (yyvsp[-3]), (yyvsp[-2]), (yyvsp[0])); }
#line 2271 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 705 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_atribuicao( (yyval), (yyvsp[-3]), (yyvsp[-2]), (yyvsp[0])); }
#line 2277 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 708 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) ); }
#line 2283 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 712 "trabalho.y" /* yacc.c:1646  */
    { checa_tipo_exp((yyvsp[-5]).t.nome , (yyvsp[-4]).t.nome); 
		  (yyval).c = "[" + toString( toInt((yyvsp[-4]).v) * toInt((yyvsp[-1]).v) ) + "]" ;}
#line 2290 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 715 "trabalho.y" /* yacc.c:1646  */
    { checa_tipo_exp((yyvsp[-2]).t.nome, Integer.nome); 
			(yyval).c = "[" + (yyvsp[-1]).v + "]";}
#line 2297 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 718 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2303 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 722 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_se( (yyval), (yyvsp[-5]), (yyvsp[-1]), ""); }
#line 2309 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 724 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_se( (yyval), (yyvsp[-9]), (yyvsp[-5]), (yyvsp[-1]).c ); }
#line 2315 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 728 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_com( (yyval), (yyvsp[-12]), (yyvsp[-8]), (yyvsp[-4]), (yyvsp[-1]) ); }
#line 2321 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 732 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_repita( (yyval), (yyvsp[-4]), (yyvsp[-1]) ); }
#line 2327 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 736 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_execute_repita( (yyval), (yyvsp[-2]), (yyvsp[-7]) ); }
#line 2333 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 739 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2339 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 740 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2345 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 741 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2351 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 742 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2357 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 743 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2363 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 744 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2369 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 745 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2375 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 746 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2381 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 747 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2387 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 748 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0])); }
#line 2393 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 749 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0])); }
#line 2399 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 750 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-3]), (yyvsp[-5]), (yyvsp[-1]) ); }
#line 2405 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 751 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2411 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 752 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador_unario( (yyval), (yyvsp[-1]), (yyvsp[0]) );}
#line 2417 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 753 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador_unario( (yyval), (yyvsp[-1]), (yyvsp[0]) );}
#line 2423 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 754 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador_unario( (yyval), (yyvsp[-1]), (yyvsp[0]) );}
#line 2429 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 758 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = String; }
#line 2435 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 759 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Integer; }
#line 2441 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 760 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Float; }
#line 2447 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 761 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Double; }
#line 2453 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 762 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Char; }
#line 2459 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 763 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) ); }
#line 2465 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 764 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2471 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2475 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 767 "trabalho.y" /* yacc.c:1906  */


#include "lex.yy.c"

void erro( string st ) {
  yyerror( st.c_str() );
  exit( 1 );
}

void yyerror( const char* st )
{
   if( strlen( yytext ) == 0 )
     fprintf( stderr, "%s\nNo final do arquivo\n", st );
   else  
     fprintf( stderr, "%s\nProximo a: %s\nlinha/coluna: %d/%d\n", st, 
              yytext, yylineno, yyrowno - (int) strlen( yytext ) );
}

void inicializa_tabela_de_resultado_de_operacoes() {
  
    map< string, Tipo > r;

	r[par(Integer, Integer)] = Integer;    
	tiporesultado[ "%" ] = r; 
    
    r[par(Integer, Float)] = Float;    
    r[par(Integer, Double)] = Double;    
    r[par(Float, Integer)] = Float;    
    r[par(Float, Float)] = Float;    
    r[par(Float, Double)] = Double;    
    r[par(Double, Integer)] = Double;    
    r[par(Double, Float)] = Double;    
    r[par(Double, Double)] = Double;    

    tiporesultado[ "-" ] = r; 
    tiporesultado[ "*" ] = r; 
    tiporesultado[ "/" ] = r; 

    r[par(Char, Char)] = String;      
    r[par(String, Char)] = String;      
    r[par(Char, String)] = String;    
    r[par(String, String)] = String;    
    tiporesultado[ "+" ] = r;

	r.clear();
	r[par(Integer, Integer)] = Boolean; 
	r[par(Float, Float)] = Boolean;    
	r[par(Float, Double)] = Boolean;    
	r[par(Double, Float)] = Boolean;    
	r[par(Double, Double)] = Boolean;    
	r[par(Char, Char)] = Boolean;      
	r[par(String, Char)] = Boolean;      
	r[par(Char, String)] = Boolean;    
	r[par(String, String)] = Boolean;    
	r[par(Boolean, Boolean)] = Boolean;    
	tiporesultado["=="] = r;
	tiporesultado["!="] = r;
	tiporesultado[">="] = r;
	tiporesultado[">"] = r;
	tiporesultado["<"] = r;
	tiporesultado["<="] = r;

	r.clear();
	r[par(Boolean, Boolean)] = Boolean;
	tiporesultado["&&"] = r;
	tiporesultado["||"] = r;

}

void inicializa_tamanho_String() {
  
	int tamanho = 256;  
  	String.dim = tamanho;
}


int main( int argc, char* argv[] )
{
	inicializa_tamanho_String();
  	inicializa_tabela_de_resultado_de_operacoes();
  	empilha_nova_tabela_de_simbolos();
  	yyparse();
}

