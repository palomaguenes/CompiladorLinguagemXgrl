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

void generateLabels(std::vector<string>* label, string tipo){

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

void gera_switch(Atributo& ss){

	if (swvar.var.t.nome != "numerosemponto" && swvar.var.t.nome != "simbolo"){
		erro("ESCOLHA recebe numerosemponto e simbolo com parâmetros!\n");
	}
	
	vector<string> labels; 
	generateLabels(&labels, swvar.var.t.nome);
	
	for (int i = 0; i < swvar.casos.size()-1 ; i++){
		ss.c = ss.c + "  if(" + swvar.var.v + " == " + swvar.casos[i] + ") goto " + labels[i] + ";\n";
	}

	int indice = swvar.casos.size()-1;

	ss.c = "\n" + ss.c + labels[indice] + swvar.cod_casos[indice] + "  goto fim_sw_" + toString(nsw) + ";\n";

	for (int i = 0; i < swvar.casos.size()-1 ; i++){
		ss.c = ss.c + labels[i] + ":;\n" + swvar.cod_casos[i] + "  goto fim_sw_" + toString(nsw) + ";\n";
	}

	ss.c = ss.c + "fim_sw_" + toString(nsw) + ":;\n\n";

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
  		aux += "[" + toString(t.dim) + "]";
           
  return aux;
}

// 'Atributo&': o '&' significa passar por referência (modifica).
void declara_variavel( Atributo& ss, 
                       vector<string> lst, 
                       Tipo tipo ) {
  ss.c = "";
  for( int i = 0; i < lst.size(); i++ ) {
    if( ts[ts.size()-1].find( lst[i] ) != ts[ts.size()-1].end() ) 
      erro( "Variável já declarada: " + lst[i] );
    else {
      ts[ts.size()-1][ lst[i] ] = tipo; 
      ss.c += tipo.decl + " " + lst[i] 
              + trata_dimensoes_decl_var( tipo ) + ";\n"; 
    }  
  }
}

void declara_variavel( Atributo& ss, string nome, Tipo tipo ) {
  vector<string> lst;
  lst.push_back( nome );
  declara_variavel( ss, lst, tipo );
}
	

void gera_codigo_atribuicao( Atributo& ss, 
                             const Atributo& s1, 
                             const Atributo& s3 ) {
  
  if( s1.t.nome == s3.t.nome &&  s1.t.nome == "palavra" ) {
    ss.c = s1.c + s3.c + "  " 
           + "strncpy( " + s1.v + ", " + s3.v + ", " + 
           toString( s1.t.dim) + " );\n";
  }else if( s1.t.nome == s3.t.nome ){
    ss.c = s1.c + s3.c + "  " + s1.v + " = " + s3.v + ";\n";
  }

}

void gera_codigo_atribuicao_funcao( Atributo& ss, 
                             const Atributo& s1, 
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
		ss.c = s1.c + s3.c + "  " 
		       + "strncpy( " + s1.v + ", " + s3.v + ", " + 
		       toString( s1.t.dim) + " );\n";
  	}else{
    	ss.c = "  " +s1.v + " =" + s3.c + "\n";
	}
}

void busca_tipo_da_variavel( Atributo& ss, const Atributo& s1 ) {
  if( ts[ts.size()-1].find( s1.v ) == ts[ts.size()-1].end() )
        erro( "Variável não declarada: " + s1.v );
  else {
    ss.t = ts[ts.size()-1][ s1.v ];
    ss.v = s1.v;
  }
}

string par( Tipo a, Tipo b ) {
  return a.nome + "," + b.nome;  
}

void gera_codigo_operador( Atributo& ss,  const Atributo& s1, const Atributo& s2, const Atributo& s3 ) {

  if( tiporesultado.find( s2.v ) != tiporesultado.end() ) {

    if( tiporesultado[s2.v].find( par( s1.t, s3.t ) ) != tiporesultado[s2.v].end() ) {
     
      ss.t =  tiporesultado[s2.v][par( s1.t, s3.t )];
      ss.v = gera_nome_var( ss.t );
      ss.c = s1.c + s3.c + "  " + ss.v + " = " + s1.v + s2.v + s3.v + ";\n";
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


string ajeita_parametros_funcao(string params){

	if (params=="")
		return "";

	string s;
	
	while (params.find(";") != std::string::npos){
		s = (params.replace(params.find(";"), 2, ", "));
	}

	while (params.find("*") != std::string::npos){
		s = (params.replace(params.find("*"), 5, "[256]"));
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


#line 526 "y.tab.c" /* yacc.c:339  */

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
    _OU = 294,
    _E = 295,
    _NAO = 296
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
#define _OU 294
#define _E 295
#define _NAO 296

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 656 "y.tab.c" /* yacc.c:358  */

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
#define YYLAST   381

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  92
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  218

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   298

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    58,     2,
      53,    54,    49,    47,    56,    48,     2,    50,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    55,    57,
      43,    44,    42,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    59,     2,    60,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    51,     2,    52,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    45,    46
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   485,   485,   488,   498,   499,   502,   505,   510,   508,
     517,   525,   532,   534,   537,   540,   542,   545,   546,   549,
     552,   553,   556,   557,   558,   559,   560,   561,   564,   566,
     570,   573,   576,   577,   580,   586,   589,   592,   595,   599,
     602,   605,   606,   607,   608,   609,   610,   611,   612,   615,
     622,   628,   629,   632,   638,   642,   645,   649,   651,   655,
     659,   662,   663,   666,   667,   670,   672,   676,   680,   684,
     688,   689,   690,   691,   692,   693,   694,   695,   696,   697,
     698,   699,   700,   701,   702,   703,   706,   707,   708,   709,
     710,   711,   712
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
  "_CTE_NUMEROGRANDECOMPONTO", "_CTE_SIMBOLO", "_RESTO", "_SOBRE", "_OU",
  "_E", "_NAO", "'>'", "'<'", "'='", "\">=\"", "\"<=\"", "'+'", "'-'",
  "'*'", "'/'", "'{'", "'}'", "'('", "')'", "':'", "','", "';'", "'&'",
  "'['", "']'", "$accept", "S", "TUDAO", "USANDOISSO", "FUNCTIONDECLS",
  "FUNCTIONDECL", "$@1", "NOME_FUNCAO", "PARAMETROS", "EXECUTEISSO",
  "DECLS", "DECL", "IDS", "TIPO", "TAM_PALAVRA", "MIOLOS", "MIOLO",
  "CHAMADAFUNCAO", "PARAM_CHAMADA", "ES", "CMDS", "CMD", "CMD_ESCOLHA",
  "ESCOLHAS", "CASOS", "CASO", "CASOCONTRARIO", "MOSTRE", "LE",
  "CMD_ATRIB", "CMD_ATRIB_SPV", "IDATR", "INDICE", "EXPS", "CMD_SE",
  "CMD_FOR", "CMD_WHILE", "CMD_DOWHILE", "E", "F", YY_NULLPTR
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
     295,   296,    62,    60,    61,   297,   298,    43,    45,    42,
      47,   123,   125,    40,    41,    58,    44,    59,    38,    91,
      93
};
# endif

#define YYPACT_NINF -120

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-120)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      32,   -40,    43,  -120,    46,  -120,     3,    28,    54,    56,
      65,    28,    30,  -120,    33,    27,   -11,    57,    36,    38,
    -120,    54,  -120,    54,     7,    85,  -120,   145,  -120,    37,
      40,  -120,  -120,    34,  -120,  -120,  -120,  -120,  -120,  -120,
      39,    41,     5,    44,    93,    50,     5,    45,    51,   145,
    -120,  -120,  -120,  -120,  -120,  -120,    47,  -120,  -120,  -120,
    -120,   -20,    54,    69,  -120,     5,     5,  -120,  -120,  -120,
    -120,  -120,  -120,    58,     5,     5,     5,     5,   143,  -120,
     101,    60,   160,   159,    59,  -120,  -120,     5,    96,    61,
      46,  -120,    55,    62,  -120,   191,   221,     5,   -28,    24,
      24,   234,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,  -120,  -120,    64,    47,     5,    67,   160,
    -120,    95,    70,   206,    92,   119,    65,  -120,    74,     5,
     124,   299,  -120,   322,   -28,   331,   331,   331,   331,   331,
      24,    24,  -120,  -120,   121,   125,   247,   120,  -120,    45,
      89,   115,    95,    47,     5,    39,  -120,   175,    88,    94,
    -120,  -120,    99,     5,    91,     5,   100,   140,    98,  -120,
     110,  -120,  -120,  -120,  -120,  -120,     7,  -120,   160,   260,
     101,   311,   160,   103,   145,   145,   112,   102,  -120,   114,
     122,     5,   147,  -120,  -120,   162,   164,  -120,   273,  -120,
     128,   129,   141,   138,    46,   160,     5,  -120,    65,   144,
     286,   158,  -120,   130,  -120,   160,   161,  -120
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     5,     1,     0,     7,     0,     0,
      16,     7,     0,    21,     0,     0,     0,     0,     0,     0,
       6,    14,     4,    18,     0,     0,    11,    31,     3,     0,
      13,    17,    22,    29,    23,    24,    26,    27,    19,    20,
      60,     0,     0,     0,     0,     0,     0,     0,     0,    31,
      32,    33,    48,    41,    42,    44,    62,    43,    45,    46,
      47,     0,    14,     0,    25,    36,     0,    91,    86,    87,
      88,    89,    90,     0,     0,     0,     0,     0,     0,    85,
       0,     0,    40,     0,     0,    15,    30,     0,     0,     0,
       5,    12,     0,     0,    35,    38,     0,     0,    84,    82,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    55,    60,     0,    62,     0,     0,    40,
      56,    52,     0,    64,     0,     0,    16,    28,     0,     0,
       0,     0,    92,    79,    78,    74,    75,    81,    77,    76,
      70,    71,    72,    73,     0,     0,     0,     0,    39,     0,
       0,     0,    52,    62,     0,    91,    58,     0,     0,     0,
      34,    37,     0,     0,     0,     0,     0,     0,     0,    49,
       0,    50,    51,    61,    63,    57,     0,    10,    40,     0,
       0,    59,    40,     0,    31,    31,     0,     0,    80,     0,
       0,     0,     0,    54,     8,    65,     0,    68,     0,    53,
       0,     0,     0,     0,     5,    40,     0,    69,    16,     0,
       0,     0,    66,     0,     9,    40,     0,    67
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -120,  -120,  -120,   -87,   186,  -120,  -120,  -120,   149,  -119,
     189,   -12,  -120,    52,  -120,   -47,  -120,   105,  -120,    97,
    -106,   -26,  -120,  -120,    75,  -120,  -120,  -120,  -120,  -120,
      63,   -75,  -104,    90,  -120,  -120,  -120,  -120,   -42,   -41
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     7,    10,    11,   200,    12,    29,    19,
      14,    15,    16,    38,    64,    48,    49,    50,    93,    94,
     118,   119,    52,   150,   151,   152,   171,    53,    54,    55,
     115,    56,    88,   122,    57,    58,    59,    60,    95,    79
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      78,    51,    86,   126,    83,   116,    84,   159,    67,    30,
      89,     4,   145,   148,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    51,    96,    32,    33,    34,    35,    36,
      37,    90,    98,    99,   100,   101,     1,    68,    69,    70,
      71,    72,    73,     5,    24,   123,    74,    25,    67,   173,
      30,     6,    75,    76,     8,   131,     9,    13,    77,    17,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,    18,   187,   111,   112,   146,   190,    68,    69,    70,
      71,    72,   157,    21,    23,    22,    26,    27,    39,   211,
      28,    61,    65,    63,    66,   155,    62,    80,    77,   209,
      81,    82,    92,    85,   114,   116,    87,   124,   168,   216,
     121,    97,   123,   117,   125,   127,   128,   208,   144,   147,
     149,   179,   158,   181,    68,    69,    70,    71,    72,    73,
     153,   160,   162,    74,   164,   167,   165,   192,   193,    75,
      76,   169,   170,   176,   180,    77,   177,   183,    40,   198,
     178,   182,    41,   184,   195,    42,   191,    43,    51,    51,
      44,    45,    46,   114,   210,   185,   194,    41,   196,    47,
      42,   201,    43,   199,   197,    44,    45,    46,   202,   204,
     205,   215,   102,   103,    47,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   206,   207,   212,    20,   102,   103,
     113,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     214,    91,    31,   217,   102,   103,   120,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   161,   172,   186,   156,
     102,   103,   175,   104,   105,   106,   107,   108,   109,   110,
     111,   112,     0,   189,   174,   102,   103,   129,   104,   105,
     106,   107,   108,   109,   110,   111,   112,     0,     0,     0,
     102,   103,   154,   104,   105,   106,   107,   108,   109,   110,
     111,   112,     0,   102,   103,   130,   104,   105,   106,   107,
     108,   109,   110,   111,   112,     0,   102,   103,   132,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     0,   102,
     103,   166,   104,   105,   106,   107,   108,   109,   110,   111,
     112,     0,   102,   103,   188,   104,   105,   106,   107,   108,
     109,   110,   111,   112,     0,   102,   103,   203,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   163,   102,   103,
     213,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     102,   103,     0,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   103,     0,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,    -1,    -1,    -1,    -1,   109,   110,
     111,   112
};

static const yytype_int16 yycheck[] =
{
      42,    27,    49,    90,    46,    80,    47,   126,     3,    21,
      30,    51,   116,   119,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    49,    66,    18,    19,    20,    21,    22,
      23,    51,    74,    75,    76,    77,     4,    32,    33,    34,
      35,    36,    37,     0,    55,    87,    41,    58,     3,   153,
      62,     5,    47,    48,    51,    97,    28,     3,    53,     3,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,     6,   178,    49,    50,   117,   182,    32,    33,    34,
      35,    36,   124,    53,    57,    52,    29,    51,     3,   208,
      52,    54,    53,    59,    53,     3,    56,    53,    53,   205,
       7,    51,    33,    52,     3,   180,    59,    11,   149,   215,
      51,    53,   154,    53,    53,    60,    54,   204,    54,    52,
      25,   163,     3,   165,    32,    33,    34,    35,    36,    37,
      60,    57,     8,    41,    13,    15,    11,   184,   185,    47,
      48,    52,    27,    55,    53,    53,    52,     7,     3,   191,
      51,    51,     7,    55,    52,    10,    53,    12,   184,   185,
      15,    16,    17,     3,   206,    55,    54,     7,    54,    24,
      10,     9,    12,    26,    52,    15,    16,    17,    14,    51,
      51,    51,    39,    40,    24,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    53,    57,    52,    11,    39,    40,
      57,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      52,    62,    23,    52,    39,    40,    57,    42,    43,    44,
      45,    46,    47,    48,    49,    50,   129,   152,   176,   124,
      39,    40,    57,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,   180,   154,    39,    40,    56,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,    -1,    -1,
      39,    40,    56,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    39,    40,    54,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    39,    40,    54,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    39,
      40,    54,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,    39,    40,    54,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    39,    40,    54,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    38,    39,    40,
      54,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      39,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    42,    43,    44,    45,    46,    47,    48,
      49,    50
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    62,    63,    51,     0,     5,    64,    51,    28,
      65,    66,    68,     3,    71,    72,    73,     3,     6,    70,
      65,    53,    52,    57,    55,    58,    29,    51,    52,    69,
      72,    71,    18,    19,    20,    21,    22,    23,    74,     3,
       3,     7,    10,    12,    15,    16,    17,    24,    76,    77,
      78,    82,    83,    88,    89,    90,    92,    95,    96,    97,
      98,    54,    56,    59,    75,    53,    53,     3,    32,    33,
      34,    35,    36,    37,    41,    47,    48,    53,    99,   100,
      53,     7,    51,    99,   100,    52,    76,    59,    93,    30,
      51,    69,    33,    79,    80,    99,    99,    53,    99,    99,
      99,    99,    39,    40,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    57,     3,    91,    92,    53,    81,    82,
      57,    51,    94,    99,    11,    53,    64,    60,    54,    56,
      54,    99,    54,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    54,    93,    99,    52,    81,    25,
      84,    85,    86,    60,    56,     3,    78,    99,     3,    70,
      57,    80,     8,    38,    13,    11,    54,    15,   100,    52,
      27,    87,    85,    93,    94,    57,    55,    52,    51,    99,
      53,    99,    51,     7,    55,    55,    74,    81,    54,    91,
      81,    53,    76,    76,    54,    52,    54,    52,    99,    26,
      67,     9,    14,    54,    51,    51,    53,    57,    64,    81,
      99,    70,    52,    54,    52,    51,    81,    52
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    61,    62,    63,    64,    64,    65,    65,    67,    66,
      66,    68,    69,    69,    69,    70,    70,    71,    71,    72,
      73,    73,    74,    74,    74,    74,    74,    74,    75,    75,
      76,    76,    77,    77,    78,    79,    79,    80,    80,    81,
      81,    82,    82,    82,    82,    82,    82,    82,    82,    83,
      84,    85,    85,    86,    87,    88,    89,    90,    90,    91,
      92,    93,    93,    94,    94,    95,    95,    96,    97,    98,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,   100,   100,   100,   100,
     100,   100,   100
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     6,     4,     0,     2,     0,     0,    15,
       8,     3,     3,     1,     0,     4,     0,     3,     2,     3,
       3,     1,     1,     1,     1,     2,     1,     1,     3,     0,
       2,     0,     1,     1,     5,     1,     0,     3,     1,     2,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     5,
       2,     2,     0,     5,     3,     3,     3,     5,     4,     4,
       1,     4,     0,     3,     1,     8,    12,    15,     8,    10,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       6,     3,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     3
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
#line 485 "trabalho.y" /* yacc.c:1646  */
    { cout << (yyvsp[0]).c << endl; }
#line 1918 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 489 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "#include <stdlib.h>\n"
                    "#include <string.h>\n"
                    "#include <stdio.h>\n\n" + 
                    declara_var_temp( temp_global ) +
                    (yyvsp[-3]).c + "\n" + (yyvsp[-2]).c +
                    "int main() {\n" + (yyvsp[-1]).c + "}\n";
       }
#line 1930 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 498 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c; }
#line 1936 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 499 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 1942 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 503 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c; }
#line 1948 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 505 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 1954 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 510 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyvsp[-1]), (yyvsp[-3]).v, (yyvsp[-1]).t ); 
					insereFuncao((yyvsp[-9]).v,(yyvsp[-1]).t); }
#line 1961 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 513 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_funcao_com_retorno( (yyval), (yyvsp[-14]).v, (yyvsp[-6]), (yyvsp[-8]).v, (yyvsp[-12]).c, (yyvsp[-2]), (yyvsp[-1]) ); 
					escopo_local = false;
            		desempilha_tabela_de_simbolos(); }
#line 1969 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 519 "trabalho.y" /* yacc.c:1646  */
    { insereFuncao((yyvsp[-7]).v, Void);
					gera_codigo_funcao_sem_retorno( (yyval), (yyvsp[-7]).v, (yyvsp[-5]).c, (yyvsp[-2]), (yyvsp[-1]) ); 
					escopo_local = false;
					desempilha_tabela_de_simbolos(); }
#line 1978 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 526 "trabalho.y" /* yacc.c:1646  */
    { escopo_local = true; 
					empilha_nova_tabela_de_simbolos();
					(yyval).v = (yyvsp[-1]).v;}
#line 1986 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 533 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + (yyvsp[0]).c; }
#line 1992 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 535 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[0]).c; }
#line 1998 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 537 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2004 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 541 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[-1]).c; }
#line 2010 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 542 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = "";}
#line 2016 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 545 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + (yyvsp[0]).c; }
#line 2022 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 549 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyval), (yyvsp[-2]).lst, (yyvsp[0]).t ); }
#line 2028 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 552 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst = (yyvsp[-2]).lst; (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 2034 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 553 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 2040 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 556 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Integer; }
#line 2046 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 557 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Float; }
#line 2052 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 558 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Double; }
#line 2058 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 559 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = (yyvsp[0]).t; }
#line 2064 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 560 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Char; }
#line 2070 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 561 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Boolean; }
#line 2076 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 565 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; (yyval).t.dim = toInt( (yyvsp[-1]).v ); }
#line 2082 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 566 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; }
#line 2088 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 571 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c; }
#line 2094 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 573 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2100 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 581 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + criachamadafuncao((yyval), (yyvsp[-4]), (yyvsp[-2]));
					(yyval).v = (yyvsp[-4]).v;}
#line 2107 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 587 "trabalho.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]); }
#line 2113 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 589 "trabalho.y" /* yacc.c:1646  */
    {(yyval).v = (yyval).c = ""; }
#line 2119 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 593 "trabalho.y" /* yacc.c:1646  */
    {   (yyval).c = (yyvsp[-2]).c + (yyvsp[0]).c;
		(yyval).v = (yyvsp[-2]).v + ',' + (yyvsp[0]).v; }
#line 2126 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 596 "trabalho.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]); }
#line 2132 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 600 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c; }
#line 2138 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 602 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2144 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 616 "trabalho.y" /* yacc.c:1646  */
    { swvar.var = (yyvsp[-3]) ; 
			   gera_switch((yyval));
			   nsw++;
			 }
#line 2153 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 623 "trabalho.y" /* yacc.c:1646  */
    { swvar.casos.push_back("default");
			  swvar.cod_casos.push_back((yyvsp[0]).c);
			}
#line 2161 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 633 "trabalho.y" /* yacc.c:1646  */
    { swvar.casos.push_back((yyvsp[-3]).v);
		  swvar.cod_casos.push_back((yyvsp[-1]).c);
		}
#line 2169 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 639 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[0]).c;}
#line 2175 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 642 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  printf( \"%"+ (yyvsp[-1]).t.fmt + "\\n\", " + (yyvsp[-1]).v + " );\n"; }
#line 2181 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 646 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  scanf( \"%"+ (yyvsp[-1]).t.fmt + "\", &"+ (yyvsp[-1]).v + " );\n"; }
#line 2187 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 650 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_atribuicao( (yyval), (yyvsp[-4]), (yyvsp[-1])); }
#line 2193 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 652 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_atribuicao_funcao((yyval), (yyvsp[-3]), (yyvsp[0])); }
#line 2199 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 656 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_atribuicao( (yyval), (yyvsp[-3]), (yyvsp[0])); }
#line 2205 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 659 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) ); }
#line 2211 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 671 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_se( (yyval), (yyvsp[-5]), (yyvsp[-1]), ""); }
#line 2217 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 673 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_se( (yyval), (yyvsp[-9]), (yyvsp[-5]), (yyvsp[-1]).c ); }
#line 2223 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 677 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_com( (yyval), (yyvsp[-12]), (yyvsp[-8]), (yyvsp[-4]), (yyvsp[-1]) ); }
#line 2229 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 681 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_repita( (yyval), (yyvsp[-4]), (yyvsp[-1]) ); }
#line 2235 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 685 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_execute_repita( (yyval), (yyvsp[-2]), (yyvsp[-7]) ); }
#line 2241 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 688 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2247 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 689 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2253 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 690 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2259 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 691 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2265 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 692 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2271 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 693 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2277 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 694 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2283 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 695 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2289 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 696 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0])); }
#line 2295 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 697 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0])); }
#line 2301 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 698 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-3]), (yyvsp[-5]), (yyvsp[-1]) ); }
#line 2307 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 699 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2313 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 700 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador_unario( (yyval), (yyvsp[-1]), (yyvsp[0]) );}
#line 2319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 701 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador_unario( (yyval), (yyvsp[-1]), (yyvsp[0]) );}
#line 2325 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 702 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador_unario( (yyval), (yyvsp[-1]), (yyvsp[0]) );}
#line 2331 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 706 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = String; }
#line 2337 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 707 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Integer; }
#line 2343 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 708 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Float; }
#line 2349 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 709 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Double; }
#line 2355 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 710 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Char; }
#line 2361 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 711 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) ); }
#line 2367 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 712 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2373 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2377 "y.tab.c" /* yacc.c:1646  */
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
#line 715 "trabalho.y" /* yacc.c:1906  */


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

