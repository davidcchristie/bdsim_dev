/*
   bison grammar for the gmad parser
   Ilya Agapov, 2005
*/


%{

  const int DEBUG = 1; // print debug info like parsing output etc.
  const int ECHO_GRAMMAR = 0; // print grammar rule expamsion (for debugging)
  const int VERBOSE = 1; // print warnings and errors
  const int VERBOSE_EXPAND = 1; // print the process of line expansion 
  const int INTERACTIVE = 1; // print output of commands (like in interactive mode)

#include "parser.h"

  int execute = 1;

%}


%union{
  double dval;
  int ival;
  struct symtab *symp;
  char *str;
  struct Array *array;
}


%left '+' '-'
%left '*' '/'
%left '^' '<' '>' NE LE GE
%nonassoc UMINUS

%token <dval> NUMBER
%token <symp> VARIABLE VECVAR FUNC 
%token <str> STR
%token MARKER ELEMENT DRIFT DIPOLE SBEND QUADRUPOLE SEXTUPOLE OCTUPOLE MULTIPOLE SOLENOID COLLIMATOR LINE SEQUENCE
%token PERIOD APERTURE FILENAME
%token BEAM OPTION PRINT RANGE STOP USE VALUE ECHO PRINTF
%token IF ELSE BEGN END LE GE NE

%type <dval> aexpr
%type <dval> expr
%type <symp> assignment
%type <array> vecexpr;
%type <array> vect;
%type <str> use_parameters;
%type <ival> extension;
%%

input : 
      | input stmt ';'
       { 
	 if(ECHO_GRAMMAR) printf("input -> input stmt ';' \n");
       }
     ;


stmt :          if_clause stmt { if(ECHO_GRAMMAR) printf("stmt -> IF '(' aexpr ')' stmt\n" ); execute = 1;}
              | if_clause stmt ELSE stmt  
                { if(ECHO_GRAMMAR) printf("stmt -> IF '(' bool_expr ')' ELSE stmt \n" ); }
              | atomic_stmt  { if(ECHO_GRAMMAR) printf("stmt -> atomic_stmt \n"); }
              | BEGN input END { if(ECHO_GRAMMAR) printf("stmt -> '{' stmt ';' atomic_stmt '}' \n"); }
;

if_clause: IF '(' aexpr ')' {if( ($3 > 0) && (execute > 0) ) execute = 1; else execute = 0;}
;

atomic_stmt : 
            | expr { if(ECHO_GRAMMAR) printf("atomic_stmt -> expr\n"); }
            | command  { if(ECHO_GRAMMAR) printf("atomic_stmt -> command"); }
            | decl  { if(ECHO_GRAMMAR) printf("atomic_stmt -> decl\n"); }
;



decl : VARIABLE ':' marker
       {
	 if(execute)  {
	   if(ECHO_GRAMMAR) printf("decl -> VARIABLE : marker\n");
	   // check parameters and write into element table
	   write_table(params,$1->name,_MARKER);
	   params.flush();
	 }
       }
     | VARIABLE ':' drift
       {
	 if(execute) {
	   if(ECHO_GRAMMAR) printf("decl -> VARIABLE : drift\n");
	   // check parameters and write into element table
	   write_table(params,$1->name,_DRIFT);
	   params.flush();
	 }
       } 
     | VARIABLE ':' sbend
       {  
	 if(execute) {
	   if(ECHO_GRAMMAR) printf("decl -> VARIABLE : sbend\n");
	   // check parameters and write into element table
	   write_table(params,$1->name,_SBEND);
	   params.flush();
	 }
       }
     | VARIABLE ':' quad
       {
	 if(execute)       
	   {
	     if(ECHO_GRAMMAR) printf("decl -> VARIABLE : quad %s \n",$1->name);
	     // check parameters and write into element table
	     write_table(params,$1->name,_QUAD);
	     params.flush();
	   }
       }
     | VARIABLE ':' sextupole
       {
	 if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("decl -> VARIABLE : sext %s \n",$1->name);
	     // check parameters and write into element table
	     write_table(params,$1->name,_SEXT);
	     params.flush();
	   }
       }
     | VARIABLE ':' octupole
       {
	 if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("VARIABLE : octupole %s \n",$1->name);
	     // check parameters and write into element table
	     write_table(params,$1->name,_OCTUPOLE);
	     params.flush();
	   }
       }
     | VARIABLE ':' multipole
       {
	 if(execute)
	   {	 
	     if(ECHO_GRAMMAR) printf("VARIABLE : multipole %s \n",$1->name);
	     // check parameters and write into element table
	     write_table(params,$1->name,_MULT);
	     params.flush();	 
	   }
       }
     | VARIABLE ':' line 
       {
	 if(execute)
	   {
	     // create entry in the main table and add pointer to the parsed sequence
	     if(ECHO_GRAMMAR) printf("VARIABLE : LINE %s\n",$1->name);
	     write_table(params,$1->name,_LINE,new list<struct Element>(tmp_list));
	     tmp_list.erase(tmp_list.begin(), tmp_list.end());
	   }
       }
     | VARIABLE ':' sequence
       {
	 if(execute)
	   {
	   }
       }
     | VARIABLE ':' extension
       {
	 if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("decl -> VARIABLE : VARIABLE, %s  :  %s\n",$1->name, typestr($3));
	     if($3 != _NONE)
	       {
		 write_table(params,$1->name,$3);
	       }
	     params.flush();
	   }
       }
;

marker : MARKER;

drift : DRIFT parameters
;

sbend : SBEND parameters
;

quad : QUADRUPOLE parameters
;

sextupole : SEXTUPOLE parameters
;

octupole : OCTUPOLE parameters
;

multipole : MULTIPOLE parameters
{
  if(DEBUG) print(params);
}
;

extension : VARIABLE parameters
            {
	      if(execute)
		{	 
		  if(ECHO_GRAMMAR) printf("extension : VARIABLE parameters   -- %s \n",$1->name);
		  list<struct Element>::iterator it = element_lookup($1->name);
		  if(it == NULL)
		    {
		      if(VERBOSE) printf("type %s has not been defined\n",$1->name);
		      $$ = _NONE;
		    }
		  else
		    {
		      // inherit properties from the base type
		      $$ = (*it).type;
		      inherit_properties(*it);
		    }
		  
		}
	    }
;

parameters: 
          | parameters ',' VARIABLE '=' aexpr
            {
	      if(execute)
		{
		  if(DEBUG) printf("parameters, VARIABLE(%s) = aexpr(%.10g)\n",$3->name,$5);
		  if(!strcmp($3->name,"l")) { params.l = $5; params.lset = 1;} 
		  else
		    if(!strcmp($3->name,"k0")) { params.k0 = $5; params.k0set = 1;}
		    else 
		      if(!strcmp($3->name,"k1")) { params.k1 = $5; params.k1set = 1;} 
		      else
			if(!strcmp($3->name,"k2")) { params.k2 = $5; params.k2set = 1;}
			else 	
			  if(VERBOSE) printf("Warning : unknown parameter %s at line %d\n",$3->name,@3.first_line);
		  
		}
	    }
           | parameters ',' VARIABLE '=' vecexpr
             {
	       if(execute) 
		 {
		   if(DEBUG) printf("params,VARIABLE = vecexpr (%d)\n",$5->size);
		   if(!strcmp($3->name,"knl")) 
		     {
		       params.knlset = 1;
		       set_vector(params.knl,$5);
		       delete[] $5->data;
		     } 
		   else
		     if(!strcmp($3->name,"ksl")) 
		       {
			 params.kslset = 1;
			 set_vector(params.ksl,$5);
			 delete[] $5->data;
		       }
		     else 	  
		       if(VERBOSE) printf("unknown parameter %s\n",$3->name);
		 }
	     }         
           | VARIABLE '=' vecexpr
             {
	       if(execute) 
		 {
		   if(DEBUG) printf("VARIABLE = vecexpr\n");
		   if(!strcmp($1->name,"knl")) 
		     {
		       params.knlset = 1;
		       set_vector(params.knl,$3);
		       delete[] $3->data;
		     } 
		   else
		     if(!strcmp($1->name,"ksl")) 
		       {
			 params.kslset = 1;
			 set_vector(params.ksl,$3);
			 delete[] $3->data;
		       }
		     else 	  
		       if(VERBOSE) printf("unknown parameter %s\n",$1->name);
		 }         
	     }
          | VARIABLE '=' aexpr
            {
	      if(execute)
		{
		  if(DEBUG) printf("VARIABLE = aexpr(%.10g)\n",$3);
		}
	    }

line : LINE '=' '(' element_seq ')'           
;

sequence : SEQUENCE;


element_seq : 
            | element_seq ',' VARIABLE 
              {
		if(execute)
		  {
		    if(DEBUG) printf("matched sequence element, %s\n",$3->name);
		    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = $3->name;
		      e.type = _LINE;
		      e.lst = NULL;
		      tmp_list.push_back(e);
		    }
		  }
	      }
            | VARIABLE
              {
		if(execute)
		  {
		    if(DEBUG) printf("matched last sequence element, %s\n",$1->name);
		    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = $1->name;
		      e.type = _LINE;
		      e.lst = NULL;
		      tmp_list.push_back(e);
		    }
		  }
	      }
;


expr : aexpr 
       { // check type ??
	 if(ECHO_GRAMMAR) printf("expr -> aexpr\n");
	 if(execute) 
	   {
	     if(INTERACTIVE) printf ("\t%.10g\n", $1); $$=$1;
	   }
       }
     | vecexpr 
       {
	 if(ECHO_GRAMMAR) printf("expr -> vecexpr\n");
	 if(execute)
	   {
	     if(INTERACTIVE)
	       for(int i=0;i<$1->size;i++)
		 {
		   printf(" %.10g ",$1->data[i]);
		 }
	     $$ = 0;
	   } 
       }
     | assignment 
       { // check type
	 if(ECHO_GRAMMAR) printf("expr -> assignment\n");
	 if(execute)
	   {
	     if(INTERACTIVE) {
	       if($1->type == _ARRAY)
		 {
		   for(list<double>::iterator it = $1->array.begin();
		       it!=$1->array.end();it++)
		     printf ("\t%.10g", (*it));
		   printf("\n");
		 }
	       else
		 printf ("\t%.10g\n", $1->value);
	     } 
	     $$=0;
	   }
       }
;

aexpr :  NUMBER               { $$ = $1;                         }
       | VARIABLE             
         { 
	   //check type ??
	   $$ = $1->value;        
          } 
       | FUNC '(' aexpr ')'   { $$ = (*($1->funcptr))($3);       } 
       | aexpr '+' aexpr      { $$ = $1 + $3;                    }
       | aexpr '-' aexpr      { $$ = $1 - $3;                    }  
       | aexpr '*' aexpr      { $$ = $1 * $3;                    }
       | aexpr '/' aexpr      { $$ = $1 / $3;                    }
       | aexpr '^' aexpr      { $$ = pow($1,$3);                 }
       | '-' aexpr  %prec UMINUS { $$ = -$2; }
       | '(' aexpr ')'         { $$ = $2;                         }
       | '<' vecexpr ',' vecexpr '>' // scalar product
         {
	   if($2->size == $4->size)
	     {
	       $$ = 0;
	       for(int i=0;i<$2->size;i++)
		 $$ += $2->data[i] * $4->data[i];
	     }
	   else
	     {
	       if(VERBOSE) printf("vector dimensions do not match");
	       $$ = _undefined;
	     }
         } 
       // boolean stuff
        | aexpr '<' aexpr { $$ = ($1 < $3 )? 1 : 0; } 
        | aexpr LE aexpr { $$ = ($1 <= $3 )? 1 : 0; } 
        | aexpr '>' aexpr { $$ = ($1 > $3 )? 1 : 0; } 
        | aexpr GE aexpr { $$ = ($1 >= $3 )? 1 : 0; } 
        | aexpr NE aexpr { $$ = ($1 != $3 )? 1 : 0; } 
 ; 

assignment :  VARIABLE '=' aexpr  
              {
		if(execute)
		  {
		    $1->value = $3; $$=$1;       
		  }
	      }
           |  VARIABLE '=' vecexpr
              {
		if(execute)
		  {
		    $1->array.erase($1->array.begin(),$1->array.end());
		    for(int i=0;i<$3->size;i++)
		      $1->array.push_back($3->data[i]);
		    $1->type = _ARRAY;
		    $$ = $1;
		    delete[] $3->data;
		    $3->size = 0;
		  }
              }
           |  VECVAR '=' vecexpr
              {
		if(execute)
		  {
		    $1->array.erase($1->array.begin(),$1->array.end());
		    for(int i=0;i<$3->size;i++)
		      $1->array.push_back($3->data[i]);
		    $$ = $1;
		    delete[] $3->data;
		    $3->size = 0;
		  }
              }
;

vecexpr :  VECVAR  
        {
	  if(execute)
	    {
	      $$ = new struct Array;
	      $$->data = new double[$1->array.size()];
	      $$->size = $1->array.size();
	      //array_list.push_back($$);
	      list<double>::iterator it = 0;
	      int i = 0;
	      for(it=$1->array.begin();it!=$1->array.end();it++)
		{
		  $$->data[i++] = (*it);
		}
	    }
        } 
        | vect
        {
	  if(execute)
	    {
	      $$ = new struct Array;
	      $$->data = new double[$1->size];
	      $$->size = $1->size;
	      //array_list.push_back($$);
	      int i = 0;
	      for(int i=0;i<$1->size;i++)
		{
		  $$->data[i] = $1->data[i];
		}
	      
	      
	      // erase data in vect
	      
	      delete[] $1->data;
	      $1->size = 0;
	    }
	}
       | vecexpr '+' vecexpr
        {
	  if(execute)
	    {
	      $$ = new struct Array;
	      $$->size = ($1->size < $3->size )? $1->size : $3->size;
	      $$->data = new double[$$->size];
	      //array_list.push_back($$);
	      int i = 0;
	      for(int i=0;i<$$->size;i++)
		{
		  $$->data[i] = $1->data[i] + $3->data[i];
		}
	      
	      
	      // erase data in vect
	      
	      delete[] $1->data;
	      delete[] $3->data;
	      $1->size = 0;
	      $3->size = 0;
	    }
        }
      | vecexpr '-' vecexpr
        {
	  if(execute)
	    {
	      $$ = new struct Array;
	      $$->size = ($1->size < $3->size )? $1->size : $3->size;
	      $$->data = new double[$$->size];
	      //array_list.push_back($$);
	      int i = 0;
	      for(int i=0;i<$$->size;i++)
		{
		  $$->data[i] = $1->data[i] - $3->data[i];
		}
	      
	      
	      // erase data in vect
	      
	      delete[] $1->data;
	      delete[] $3->data;
	      $1->size = 0;
	      $3->size = 0;
	    }
	}
       | vecexpr '+' aexpr
        {
	  if(execute)
	    {
	      $$ = new struct Array;
	      $$->size = $1->size;
	      $$->data = new double[$$->size];
	      //array_list.push_back($$);
	      int i = 0;
	      for(int i=0;i<$$->size;i++)
		{
		  $$->data[i] = $1->data[i] + $3;
		}
	      
	      // erase data in vect
	      
	      delete[] $1->data;
	      $1->size = 0;
	    }
	}

      | vecexpr '*' aexpr
        {
	  if(execute)
	    {
	      $$ = new struct Array;
	      $$->size = $1->size;
	      $$->data = new double[$$->size];
	      //array_list.push_back($$);
	      int i = 0;
	      for(int i=0;i<$$->size;i++)
		{
		  $$->data[i] = $1->data[i] * $3;
		}
	      
	      // erase data in vect
	      
	      delete[] $1->data;
	      $1->size = 0;
	    }
	}
      | vecexpr '/' aexpr
        {
	  if(execute)
	    {
	      $$ = new struct Array;
	      $$->size = $1->size;
	      $$->data = new double[$$->size];
	      //array_list.push_back($$);
	      int i = 0;
	      for(int i=0;i<$$->size;i++)
		{
		  $$->data[i] = $1->data[i] / $3;
		}
	      
	      // erase data in vect
	      
	      delete[] $1->data;
	      $1->size = 0;
	    }
	}
       | aexpr '+' vecexpr
        {
	  if(execute)
	    {
	      $$ = new struct Array;
	      $$->size = $3->size;
	      $$->data = new double[$$->size];
	      //array_list.push_back($$);
	      int i = 0;
	      for(int i=0;i<$$->size;i++)
		{
		  $$->data[i] = $3->data[i] + $1;
		}
	      
	      // erase data in vect
	      
	      delete[] $3->data;
	      $3->size = 0;
	    }
	}
       | aexpr '-' vecexpr
        {
	  if(execute)
	    {
	      $$ = new struct Array;
	      $$->size = $3->size;
	      $$->data = new double[$$->size];
	      //array_list.push_back($$);
	      int i = 0;
	      for(int i=0;i<$$->size;i++)
		{
		  $$->data[i] = $1 - $3->data[i];
		}
	      
	      // erase data in vect
	      
	      delete[] $3->data;
	      $3->size = 0;
	    }
	}
      | aexpr '*' vecexpr
        {
	  if(execute)
	    {
	      $$ = new struct Array;
	      $$->size = $3->size;
	      $$->data = new double[$$->size];
	      //array_list.push_back($$);
	      int i = 0;
	      for(int i=0;i<$$->size;i++)
		{
		  $$->data[i] = $1 * $3->data[i];
		}
	      
	      // erase data in vect
	      
	      delete[] $3->data;
	      $3->size = 0;
	    }
	}

;

vect : '{' numbers '}' 
{
  if(execute)
    {
      //printf("matched vector of size %d\n",_tmparray.size());
      $$ = new struct Array;
      $$->data = new double[_tmparray.size()];
      $$->size = _tmparray.size();
      
      //array_list.push_back(a);
      
      list<double>::iterator it;
      
      int i=0;
      for(it=_tmparray.begin();it!=_tmparray.end();it++)
	{
	  $$->data[i++] = (*it);
	}
      
      _tmparray.erase(_tmparray.begin(),_tmparray.end());
    }
}
;

numbers : 
        | numbers ',' aexpr
          {
	    if(execute)
	      _tmparray.push_back($3);
          } 
       | aexpr
         {
	   if(execute)
	     _tmparray.push_back($1);
        }
;

command : STOP             { if(execute) quit(); }
        | BEAM ',' beam_parameters
        | PRINT            { if(execute) print( element_list ); }
        | PRINT ',' LINE   { if(execute) print( beamline_list); }
        | PRINT ',' OPTION { if(execute) print(options); }
        | PRINT ',' VARIABLE 
          {
	    if(execute)
	      {
		printf("\t");
		printf("\t%.10g\n",$3->value);
	      }
	  } 
        | PRINT ',' VECVAR 
          {
	    if(execute)
	      {
		printf("\t");
		
		list<double>::iterator it;
		for(it=$3->array.begin();it!=$3->array.end();it++)
		  {
		    printf("  %.10g ",(*it));
		  }
		
		printf("\n");
	      } 
	  }
        | USE ',' use_parameters { if(execute) expand_line(current_line,current_start, current_end);}
        | OPTION  option_parameters
	| ECHO STR { if(execute) printf("%s\n",$2); }
//| PRINTF '(' fmt ')' { if(execute) printf($3,$5); }
;

use_parameters :  PERIOD '=' VARIABLE
                  {
		    if(execute)
		      {
			$$ = $3->name;
			current_line = $3->name;
			current_start = NULL;
			current_end = NULL;
		      }
                  }
                | PERIOD '=' VARIABLE ',' RANGE '=' VARIABLE '/' VARIABLE
                  {
		    if(execute)
		      {
			$$ = $3->name;
			current_line = $3->name;
			current_start = $7->name;
			current_end = $9->name;
		      }
		  }
;

option_parameters : 
                  | option_parameters ',' VARIABLE '=' NUMBER
                    {
		      if(execute)
			{
			  if(!strcmp($3->name,"echo")) options.echo = (int)$5;
			}
		    }   
;

beam_parameters :
                | beam_parameters ',' VARIABLE '=' NUMBER
                  {
		    if(execute)
		      if(!strcmp($3->name,"nparticles")) options.nparticles = (int)$5;
		  }   
                | VARIABLE '=' NUMBER
                  {
		    if(execute)
		      if(!strcmp($1->name,"nparticles")) options.nparticles = (int)$3;
		  }   
                | beam_parameters ',' VARIABLE '=' STR
                  {
		    if(execute)
		      if(!strcmp($3->name,"particle")) strcpy(options.particle,$5);
		  }   
                | VARIABLE '=' STR
                  {
		    if(execute)
		      if(!strcmp($1->name,"particle")) strcpy(options.particle ,$3);
		  }   
;

%%



int yyerror(char *s)
{
  printf(s);
}

int yywrap()
{
	return 1;
}



