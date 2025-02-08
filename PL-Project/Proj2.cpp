# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <string>
# include <string.h>
# include <vector>
# include <sstream>

 //  for uTestNum
# define CALTEST true
# define NEED_FIRST_PRETTYPRINT false
# define DEBUG true
# define DEBUG_Print false 
# define DEBUG_Print_H false
# define DEBUG_PRINT_EVAL false  
# define DEBUG_PRINT_EVAL_P2 false  

// # define ぃ璶Τ line comment
// project 2 problem1
// project 2 problem2

# define FunctionCount 74

using namespace std;

int gLine = 1;
int gColumn = 0;
int gM = 0;
bool gerrorMsg = false; // if 竒 Error Msg => true
bool geofcheck = false;
bool gendIsNotEnter = false;
int gLevel = 0; // evaluate璸衡function材碭level
                // define ﹚璶材0糷



string gFunctionList[FunctionCount] = { // quote 矪瞶
  "cons", "list", "define", "car", "cdr", 
  "atom?", "pair?", "list?", "null?", "integer?", "real?", "number?", "string?", "boolean?", "symbol?", 
  "+", "-", "*", "/", "not", "and", "or", ">", ">=", "<", "<=", "=", 
  "string-append", "string>?", "string<?", "string=?", "eqv?", "equal?", 
  "begin", "if", "cond", "clean-environment", "exit", "#<procedure cons>", 
  "#<procedure list>",  
  "#<procedure define>", 
  "#<procedure car>", 
  "#<procedure cdr>", 
  "#<procedure atom?>", 
  "#<procedure pair?>", 
  "#<procedure list?>", 
  "#<procedure null?>", 
  "#<procedure integer?>", 
  "#<procedure real?>", 
  "#<procedure number?>", 
  "#<procedure string?>", 
  "#<procedure boolean?>", 
  "#<procedure symbol?>", 
  "#<procedure +>", 
  "#<procedure ->", 
  "#<procedure *>", 
  "#<procedure />", 
  "#<procedure not>", 
  "#<procedure and>", 
  "#<procedure or>", 
  "#<procedure >>", 
  "#<procedure >=>", 
  "#<procedure <>", 
  "#<procedure <=>", 
  "#<procedure =>", 
  "#<procedure string-append>", 
  "#<procedure string>?>", 
  "#<procedure string<?>", 
  "#<procedure string=?>", 
  "#<procedure eqv?>", 
  "#<procedure equal?>", 
  "#<procedure begin>", 
  "#<procedure if>", 
  "#<procedure cond>"
                                      };





enum TokenType {
  LEFT_PAREN = 0,  // '('
  RIGHT_PAREN = 1, // ')'
  INT = 2,         // '123', '+123', '-123'
  STRING = 3,      // "Hello, world!"
  DOT = 4,         // '.'
  FLOAT = 5,       // '123.567', '123.', '.567', '+123.4', '-.123'
  NIL = 6,         // 'nil', or '#f'
  T = 7,           // 't' or '#t'
  QUOTE = 8,       // '
  SYMBOL = 9,
  UNKNOWN = 10
};

void DP( bool printIt, string str ) {  // Debug Print
  if ( printIt )
    cout << str << endl;
} // DP()

bool IsWhiteSpace( char temp ) ;

char GetAChar() ;

void SkipWhiteSpace( char & temp ) ;

bool IsSeparator( char temp ) ;

bool IsInt( string & str ) ;

bool IsFloat( string & str ) ;

float StrToFloat( string str ) ;

class Token { // 魁token戈癟
public:
  int mline, mcolumn;
  int mtype;    // token type (int, symbol...)
  
  string mname; // token name
  Token *mtoken; // token
  Token *mleft;
  Token *mright;
  Token * mCurToken;
  Token * mNextToken;

  /*
  Token() {
    IsBeReplacedOnce = false ;

  } // Token()
  */


  Token * WhatKindOfToken( string str, int line, int column ) {
    Token *t = new Token();
    if ( str.size() == 0 ) return t;
    t->mname = str;
    t->mline = line;
    t->mcolumn = column;
    if ( str == "(" )
      t->mtype = LEFT_PAREN;
    else if ( str == ")" )
      t->mtype = RIGHT_PAREN;
    else if ( str == "." )
      t->mtype = DOT;
    else if ( str == "nil" || str == "#f" || str == "()" )
      t->mtype = NIL;
    else if ( str == "t" || str == "#t" )
      t->mtype = T;
    else if ( str == "'" )
      t->mtype = QUOTE;
    else if ( IsInt( str ) ) {
      t->mtype = INT;
      t->mname = str;
    } // else if
    else if ( IsFloat( str ) ) {
      t->mtype = FLOAT;
      t->mname = str;
    } // else if
    else if ( str[0] == '"' && str[str.size() - 1] == '"' )
      t->mtype = STRING;
    /*
    else if ( strcmp("quote", str.c_str() ) == 0 ) { // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! эquote!!!!!!!!!!!!!
      if ( DEBUG_PRINT_EVAL ) cout << "is quote!!" << endl ;
      t->mtype = QUOTE;
    } // else if
    */
    else
      t->mtype = SYMBOL;

    return t;
  } // WhatKindOfToken()

  Token * GetAToken() { // Get next token from input (Scanner)
    string str = "";
    char chnext = '\0', temp = '\0';
    mCurToken = mNextToken;
    if ( gendIsNotEnter == true ) {
      gendIsNotEnter = false;
      chnext = cin.peek();
      while ( chnext == ' ' || chnext == '\t' ) {
        temp = GetAChar();
        chnext = cin.peek();
      } // while

      if ( chnext == '\n' ) {
        temp = GetAChar();
        gLine = 1;
        gColumn = 0;
        SkipWhiteSpace( chnext );
        if ( chnext == EOF ) {
          temp = GetAChar();
          geofcheck = true;
          return mCurToken;
        } // if
      } // if
      else if ( chnext == ';' ) {
        temp = GetAChar();
        while ( temp != '\n' && temp != EOF )
          temp = GetAChar();
        if ( temp == EOF ) {
          geofcheck = true;
          return mCurToken;
        } // if

        gLine = 1;
        gColumn = 0;
        SkipWhiteSpace( chnext );
        if ( chnext == EOF ) {
          temp = GetAChar();
          geofcheck = true;
          return mCurToken;
        } // if
      } // if
      else if ( chnext == EOF ) {
        temp = GetAChar();
        geofcheck = true;
        return mCurToken;
      } // else if
    } // if 
    else {
      SkipWhiteSpace( chnext );
      if ( chnext == EOF ) {
        temp = GetAChar();
        geofcheck = true;
        return mCurToken;
      } // if
    } // else
      
    while ( IsSeparator( chnext ) == false && chnext != EOF ) { // if read separator, stop and check
      temp = GetAChar();
      str = str + temp;
      chnext = cin.peek();
    } // while temp != separator, EOF

    if ( str == "" ) { // return separator, else return str for the loop
      if ( chnext == ';' ) {
        temp = GetAChar();
        // comment
        while ( temp != '\n' && temp != EOF )
          temp = GetAChar();
        if ( temp != EOF ) return GetAToken();
      } // if
      else if ( chnext == '"' ) {
        temp = GetAChar();
        // string
        str = str + temp;
        chnext = cin.peek();
        while ( chnext != '"' && chnext != EOF && chnext != '\n' ) {
          temp = GetAChar();
          str = str + temp;
          if ( temp == '\\' ) {
            chnext = cin.peek();
            if ( chnext == '"' || chnext == '\\' ) {
              temp = GetAChar();
              str = str + temp;
            } // if
          } // if

          chnext = cin.peek();
        } // while

        if ( chnext == EOF || chnext == '\n' ) {
          cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << gLine << " Column "
               << gColumn+1 << endl;
          gerrorMsg = true;
          while ( gColumn != 0 )
            temp = GetAChar();
          return mCurToken;
        } // if
        else {
          temp = GetAChar();
          str = str + temp;
        } // else

        chnext = cin.peek();
      } // else if 
      else if ( chnext == '(' || chnext == ')' || chnext == '\'' ) {
        temp = GetAChar();
        str = str + temp;
        if ( temp == '(' ) {
          chnext = cin.peek();
          while ( IsWhiteSpace( chnext ) ) {
            temp = GetAChar();
            chnext = cin.peek();
          } // while

          if ( chnext == ')' ) { // ()
            temp = GetAChar();
            str = str + temp;
          } // if
        } // if

      } // else if temp = ( , ) , '
    } // if

    mNextToken = WhatKindOfToken( str, gLine, gColumn );
    return mCurToken;
  } // GetAToken()

  Token * PeekAToken() {
    return mNextToken;
  } // PeekAToken()

  vector<Token> GetToken() { // Parser
    int parenCount = 0, tmpcolumn = 0;
    char temp = '\0', chnext = '\0';
    vector<Token> expr;
    GetAToken();
    
    if ( geofcheck == true || gerrorMsg == true ) {
      expr.clear();
      return expr;
    } // if
    else if ( IsSexp( expr, false, true ) ) {
      chnext = cin.peek();
      if ( chnext != '\n' && chnext != EOF ) gendIsNotEnter = true;
      else if ( chnext == '\n' ) {
        gendIsNotEnter = false;
        temp = GetAChar();
      } // else if

      return expr;
    } // else if

    expr.clear();
    return expr;
  } // GetToken()

  bool IsExit( vector<Token> expr ) {
    if ( expr.size() == 3 && expr[0].mname == "(" && expr[1].mname == "exit" 
         && expr[2].mname == ")" ) //  ( exit )
      return true;
    else if ( expr.size() == 5 && expr[0].mname == "(" && expr[1].mname == "exit" && expr[2].mname == "." 
              && expr[3].mname == "nil" && expr[4].mname == ")" ) //  ( exit . nil)
      return true;
    else if ( expr.size() == 6 && expr[0].mname == "(" && expr[1].mname == "exit" && expr[2].mname == "." 
              && expr[3].mname == "(" && expr[4].mname == ")" && expr[5].mname == ")" ) //  ( exit . () )
      return true;
    return false;
  } // IsExit()

  bool IsATOM( Token * token ) {
    if ( token->mtype == SYMBOL || token->mtype == INT || token->mtype == FLOAT || token->mtype == STRING ||
         token->mtype == NIL || token->mtype == T )
      return true;
    return false;
  } // IsATOM()

  bool IsSexp( vector<Token> & expr, bool option, bool start ) {
    
    if ( IsATOM( PeekAToken() ) ) {
      expr.push_back( *PeekAToken() );
      if ( !start ) GetAToken();
      return true;
    } // if
    else if ( PeekAToken()->mtype == LEFT_PAREN ) {
      expr.push_back( *PeekAToken() );
      GetAToken();
      if ( geofcheck ) return false;
      if ( IsSexp( expr, false, false ) ) { // 12
        if ( geofcheck ) return false;
        while ( IsSexp( expr, true, false ) ) {
          // if 竒 Error Msg || 笿EOF 碞ぃ膥尿暗钡return
          if ( gerrorMsg == true || geofcheck == true ) return false; 
        } // while
        
        if ( gerrorMsg == true || geofcheck == true ) return false; 
        if ( PeekAToken()->mtype == DOT ) {
          expr.push_back( *PeekAToken() );
          GetAToken();
          if ( geofcheck ) return false;
          if ( IsSexp( expr, false, false ) ) {
            if ( geofcheck ) return false;
            if ( PeekAToken()->mtype == RIGHT_PAREN ) {
              expr.push_back( *PeekAToken() );
              if ( !start ) GetAToken();
              return true;
            } // if
            else if ( gerrorMsg == false ) {
              cout << "ERROR (unexpected token) : ')' expected when token at Line " << PeekAToken()->mline
                   << " Column " << PeekAToken()->mcolumn << " is >>" << PeekAToken()->mname << "<<\n";
              gerrorMsg = true;
              while ( gColumn != 0 )
                GetAChar();
            } // else if
          } // if
        } // if
        else if ( PeekAToken()->mtype == RIGHT_PAREN ) {
          expr.push_back( *PeekAToken() );
          if ( !start ) GetAToken();
          return true;
        } // else if 
        else { // ERROR for expected )
          if ( gerrorMsg == false ) {
            cout << "ERROR (unexpected token) : ')' expected when token at Line " << PeekAToken()->mline
                 << " Column " << PeekAToken()->mcolumn << " is >>" << PeekAToken()->mname << "<<\n";
            gerrorMsg = true;
            while ( gColumn != 0 )
              GetAChar();
          } // if

          return false;
        } // else
      } // if
      else { // ERROR for expected ATOM, (
        if ( gerrorMsg == false ) {
          cout << "ERROR (unexpected token) : atom or '(' expected when token at Line " 
               << PeekAToken()->mline << " Column " << PeekAToken()->mcolumn << " is >>"
               << PeekAToken()->mname << "<<\n";
          gerrorMsg = true;
          while ( gColumn != 0 )
            GetAChar();
        } // if

        return false;
      } // else
    } // else if
    else if ( PeekAToken()->mtype == QUOTE ) {
      expr.push_back( *PeekAToken() );
      GetAToken();
      if ( geofcheck ) return false;
      if ( IsSexp( expr, false, start ) )
        return true;
    } // else if 
    else {
      if ( option == true ) {
        return false;
      } // if
      else { // ERROR for expected ATOM, (
        if ( gerrorMsg == false ) {
          cout << "ERROR (unexpected token) : atom or '(' expected when token at Line " 
               << PeekAToken()->mline << " Column " << PeekAToken()->mcolumn << " is >>" 
               << PeekAToken()->mname << "<<\n";
          gerrorMsg = true;
          while ( gColumn != 0 && geofcheck == false )
            GetAChar();
        } // if

        return false;
      } // else
    } // else

    return false;
  } // IsSexp()

  int FindMatchingParenIndex( vector<Token> expr, int start, int end, string findLorR ) {
    int count = 0;
    if ( findLorR == "findR" ) {            // 笿'('璶т癸莱')'
      for ( int i = start; i < end ; i++ ) { // start < end
        if ( expr[i].mname == "(" )
          count++;
        else if ( expr[i].mname == ")" )
          count--;
        if ( count == 0 )
          return i;
      } // for
    } // if
    else if ( findLorR == "findL" ) {       // 笿')'璶т癸莱'('
      for ( int i = start; i > end ; i-- ) { // start > end
        if ( expr[i].mname == "(" )
          count++;
        else if ( expr[i].mname == ")" )
          count--;
        if ( count == 0 )
          return i;
      } // for
    } // else if

    return -1;
  } // FindMatchingParenIndex()

  void AddToken( vector<Token> & expr, int index, int type, string name ) {
    Token *t = new Token();
    t->mname = name;
    t->mtype = type;
    expr.insert( expr.begin() + index, *t );
  } // AddToken()

  void Translate( vector<Token> & expr ) {
  /*
    Rules of translation :
      1. ( A  B  C  D . E ) is directly translated into
        ( A . ( B . ( C . ( D . E ) )) )

      2. ( A  B  C  D ) is first translated into ( A  B  C  D . nil ),
        and then translated into ( A . ( B . ( C . ( D . nil ) )) )

    1.  .nil and 笿quote 璶暗矪瞶
    2. 埃 .()   惠璶魁(, )竚
    3.  .()
  */
    int j = 0, k = 0;
    // Step 1
    for ( int i = 0; i < expr.size() ; i++ ) {
      if ( expr[i].mtype == QUOTE ) {
        if ( expr[i + 1].mname == "(" || expr[i + 1].mname == "'" ) {  // '(S-exp) || ''(S-exp)
          k = i + 1;
          while ( expr[k].mname != "(" ) k++;
          j = FindMatchingParenIndex( expr, k, expr.size(), "findR" );
          AddToken( expr, j, RIGHT_PAREN, ")" );
          AddToken( expr, i, LEFT_PAREN, "(" );
        } // if
        else {  // 'ATOM
          AddToken( expr, i + 2, RIGHT_PAREN, ")" );
          AddToken( expr, i, LEFT_PAREN, "(" );
        } // else 

        i++;
      } // if
    } // for
    
    for ( int i = 0; i < expr.size() ; i++ ) {
      if ( expr[i].mname == "(" ) { // check point or add .nil
        j = FindMatchingParenIndex( expr, i, expr.size(), "findR" );
        if ( expr[j - 1].mname == ")" ) { // ')' 玡琌 (S-exp) => т (S-exp) 玡Τ⊿Τ '.'
          k = FindMatchingParenIndex( expr, j - 1, i, "findL" );
          if ( expr[k - 1].mname != "." ) { // (S-exp) 玡⊿Τ '.' => expr[j].mname == ")" 玡璶 .nil
            AddToken( expr, j, DOT, "." );
            AddToken( expr, j + 1, NIL, "nil" );
            i = 0;
          } // if
        } // if
        else if ( expr[j - 2].mname != "." ) { // ')' 玡ぃ琌 (S-exp)⊿Τ .nil =>  .nil
          AddToken( expr, j, DOT, "." );
          AddToken( expr, j + 1, NIL, "nil" );
          i = 0;
        } // if
      } // if
      else if ( expr[i].mname == "()" || expr[i].mname == "#f" )
        expr[i].mname = "nil";
      else if ( expr[i].mname == "t" )
        expr[i].mname = "#t";

    } // for ( i < expr.size)

    DP( DEBUG, "Translate Step1 succeeded!" );

    // Step 2
    for ( int i = 0; i < expr.size() ; i++ ) {
      if ( expr[i].mname == "." && expr[i + 1].mname == "(" ) {
        j = FindMatchingParenIndex( expr, i + 1, expr.size(), "findR" );
        expr.erase( expr.begin() + j );
        expr.erase( expr.begin() + i + 1 );
        expr.erase( expr.begin() + i );
        i = 0;
      } // if
    }   // for i < expr.size

    DP( DEBUG, "Translate Step2 succeeded!" );

    // Step 3
    for ( int i = 0; i < expr.size() ; i++ ) { // . 玡玡ぃ琌'(' => 璶.()
      if ( expr[i].mname == "." ) {
        if ( expr[i - 1].mname == ")" ) { // . 玡琌 (S-exp)  ')'
          j = FindMatchingParenIndex( expr, i - 1, 0, "findL" );
          if ( expr[j - 1].mname != "(" ) {   //  .()
            if ( expr[i + 1].mname == "(" ) { // . 琌 (S-exp)  '('
              k = FindMatchingParenIndex( expr, i + 1, expr.size(), "findR" );
              AddToken( expr, k + 1, RIGHT_PAREN, ")" );
              AddToken( expr, j, DOT, "." );
              AddToken( expr, j + 1, LEFT_PAREN, "(" );
            } // if
            else { // . 琌 ATOM
              AddToken( expr, i + 2, RIGHT_PAREN, ")" );
              AddToken( expr, j, DOT, "." );
              AddToken( expr, j + 1, LEFT_PAREN, "(" );
            } // else

            i = 0;
          } // if
        } // if
        else if ( expr[i - 2].mname != "(" ) { // . 玡琌 ATOM
          if ( expr[i + 1].mname == "(" ) {    // . 琌 (S-exp)  '('
            j = FindMatchingParenIndex( expr, i + 1, expr.size(), "findR" );
            AddToken( expr, j + 1, RIGHT_PAREN, ")" );
            AddToken( expr, i - 1, DOT, "." );
            AddToken( expr, i, LEFT_PAREN, "(" );
          } // if
          else { // . 琌 ATOM
            AddToken( expr, i + 2, RIGHT_PAREN, ")" );
            AddToken( expr, i - 1, DOT, "." );
            AddToken( expr, i, LEFT_PAREN, "(" );
          } // else

          i = 0;
        } // else if

      } // if
    } // for i < expr.size

    DP( DEBUG, "Translate Step3 succeeded!" );
  } // Translate()

  vector<Token> Slicing( vector<Token> expr, int start, int end ) { // slice vector
    vector<Token> vec ;
    for ( int i = start; i < end ; i++ ) {
      Token *t = new Token();
      vec.push_back( *t ) ;
      vec[vec.size()-1] = expr[i] ;
    } // for
    
    return vec;
  } // Slicing() 

  void StringPrint( string str ) {
    for ( int i = 0; i < str.size() ; i++ ) {
      if ( str[i] == '\\' && i+1 < str.size() ) {
        if ( str[i+1] == 'n' ) {
          cout << endl ;
          i++ ;
        } // if
        else if ( str[i+1] == '"' ) {
          cout << '"' ;
          i++ ;
        } // if
        else if ( str[i+1] == 't' ) {
          cout << '\t' ;
          i++ ;
        } // if
        else if ( str[i+1] == '\\' ) {
          cout << '\\' ;
          i++ ;
        } // if
        else 
          cout << str[i] ;
      } // if
      else 
        cout << str[i] ;
    } // for

    cout << endl ;
  } // StringPrint()

  void PrettyPrint( Token *temp ) {
    /*
    Rules for printing an S-expression s
      if s is an atom
        then print s with no leading white space and with one trailing '\n'
            note : For 'nil', '()' and '#f', always print 'nil'.
            note : For '#t' and 't', always print '#t'.

      else { // s is of the form : '(' s1 s2 ... sn [ '.' snn ] ')'
        Example: (((1 . 2) (3 4) 5 . 6) 7 . 8)
        let gM be the number of characters that are already
            printed on the current line

        print '(', print one space, print s1
        print M+2 spaces, print s2
        ...
        print M+2 spaces, print sn
        if there are '.' and snn following sn
          print M+2 spaces, print '.', print '\n'
          print M+2 spaces, print snn
        print M spaces, print ')', print '\n'
    } // else s is of the form : '(' s1 s2 ... sn [ '.' snn ] ')'
    */

    if ( temp->mtype == QUOTE || IsATOM( temp ) ) { // S-expressions only an ATOM
      if ( DEBUG_Print_H ) cout << "# root only ATOM #" << endl;

      if ( temp->mtype == FLOAT )
        printf( "%.3f\n", StrToFloat( temp->mname ) );
      else if ( temp->mtype == STRING )
        StringPrint( temp->mname );
      else if ( temp->mtype == QUOTE )
        cout << "quote" << endl;
      else
        cout << temp->mname << endl;
    } // if
    else if ( temp->mleft->mtype == QUOTE || IsATOM( temp->mleft ) ) {

      if ( DEBUG_Print_H ) cout << "# left NOT only ATOM #" << endl;

      if ( temp->mright->mtype == NIL ) {

        if ( DEBUG_Print_H ) cout << "# left NOT only ATOM -> right==NIL #" << endl;

        if ( temp->mleft->mtype == FLOAT )
          printf( "%.3f\n", StrToFloat( temp->mleft->mname ) );
        else if ( temp->mleft->mtype == STRING )
          StringPrint( temp->mleft->mname );
        else if ( temp->mleft->mtype == QUOTE )
          cout << "quote" << endl;
        else
          cout << temp->mleft->mname << endl;
        gM = gM - 2;
        for ( int i = 0; i < gM ; i++ )
          cout << " ";
        cout << ")" << endl;

        if ( DEBUG_Print_H ) cout << "\\\\left\\\\" << endl;
      } // if
      else if ( temp->mright->mtype == QUOTE || IsATOM( temp->mright ) ) {

        if ( DEBUG_Print_H ) cout << "# left NOT only ATOM -> right=' || right is atom #" << endl;

        if ( temp->mleft->mtype == FLOAT )
          printf( "%.3f\n", StrToFloat( temp->mleft->mname ) );
        else if ( temp->mleft->mtype == STRING )
          StringPrint( temp->mleft->mname );
        else if ( temp->mleft->mtype == QUOTE )
          cout << "quote" << endl;
        else
          cout << temp->mleft->mname << endl;
        for ( int i = 0; i < gM ; i++ )
          cout << " ";
        cout << "." << endl;
        for ( int i = 0; i < gM ; i++ )
          cout << " ";
        if ( temp->mright->mtype == FLOAT )
          printf( "%.3f\n", StrToFloat( temp->mright->mname ) );
        else if ( temp->mright->mtype == STRING )
          StringPrint( temp->mright->mname );
        else if ( temp->mright->mtype == QUOTE )
          cout << "quote" << endl;
        else
          cout << temp->mright->mname << endl;
        gM = gM - 2;
        for ( int i = 0; i < gM ; i++ )
          cout << " ";
        cout << ")" << endl;
      } // else if
      else {
        if ( DEBUG_Print_H ) cout << "# left NOT only ATOM -> right is node #" << endl;

        if ( DEBUG_PRINT_EVAL_P2 ) cout << "# left NOT only ATOM -> right is node #" << endl;
        if ( temp->mleft->mtype == FLOAT )
          printf( "%.3f\n", StrToFloat( temp->mleft->mname ) );
        else if ( temp->mleft->mtype == STRING )
          StringPrint( temp->mleft->mname );
        else if ( temp->mleft->mtype == QUOTE )
          cout << "quote" << endl;
        else
          cout << temp->mleft->mname << endl;
        for ( int i = 0; i < gM ; i++ )
          cout << " ";

        if ( DEBUG_Print_H ) cout << "# NOT only ATOM -> else goto right #" << endl;
        PrettyPrint( temp->mright );
        if ( DEBUG_Print_H ) cout << "# NOT only ATOM -> else goto right  return#" << endl;
        
      } // else
    } // else if
    else {
      if ( DEBUG_Print_H ) cout << "# left is nothing #" << endl;
      cout << "( ";
      
      temp->mleft->mtype = LEFT_PAREN; // !!!!!!!!!!!!!!!!!!!!!!!!! test !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      temp->mleft->mname = "(" ;

      if ( DEBUG_Print_H ) cout << "type: " << temp->mleft->mtype << endl ; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      gM = gM + 2;

      if ( DEBUG_Print_H ) cout << "# left is nothing -> goto left #" << endl;
      PrettyPrint( temp->mleft );
      if ( DEBUG_Print_H ) cout << "# left is nothing -> goto left return #" << endl;
      
      if ( temp->mright->mtype == NIL ) {
        if ( DEBUG_Print_H ) cout << "# left is nothing -> right==NIL #" << endl;

        gM = gM - 2;
        for ( int i = 0; i < gM ; i++ )
          cout << " ";
        cout << ")" << endl;
      } // if
      else if ( temp->mright->mtype == QUOTE || IsATOM( temp->mright ) ) {

        if ( DEBUG_Print_H ) cout << "# left is nothing -> right==QUOTE || ATOM #" << endl;

        for ( int i = 0; i < gM ; i++ )
          cout << " ";
        cout << "." << endl;
        for ( int i = 0; i < gM ; i++ )
          cout << " ";
        PrettyPrint( temp->mright );
        gM = gM - 2;
        for ( int i = 0; i < gM ; i++ )
          cout << " ";
        cout << ")" << endl;
      } // else if
      else {
        if ( DEBUG_Print_H ) cout << "# left is nothing -> right == nothing #" << endl;

        for ( int i = 0; i < gM ; i++ )
          cout << " ";

        if ( DEBUG_Print_H ) cout << "# left is nothing -> right == nothing -> goto right #" << endl;
        PrettyPrint( temp->mright );

      } // else
    } // else

  } // PrettyPrint()


  void RetrivalTree( Token *temp ) {
    if ( temp->mtype == QUOTE || IsATOM( temp ) ) ;
    else if ( temp->mleft->mtype == QUOTE || IsATOM( temp->mleft ) ) {
      if ( temp->mright->mtype == NIL ) ;
      else if ( temp->mright->mtype == QUOTE || IsATOM( temp->mright ) ) ;
      else RetrivalTree( temp->mright );
    } // else if
    else {
      temp->mleft->mtype = LEFT_PAREN; // !!!!!!!!!!!!!!!!!!!!!!!!! test !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      temp->mleft->mname = "(" ;

      RetrivalTree( temp->mleft );

      if ( temp->mright->mtype == NIL ) ;
      else if ( temp->mright->mtype == QUOTE || IsATOM( temp->mright ) ) RetrivalTree( temp->mright );
      else RetrivalTree( temp->mright );
    } // else

  } // RetrivalTree()


  Token* BuildCons( vector<Token> expr, Token *head ) {
    Translate( expr );

    if ( DEBUG_Print ) cout << "translate over\ni, Column, name" << endl;
    if ( DEBUG_Print )
      for ( int i = 0; i < expr.size() ; i++ )
        cout << i << ", " << expr[i].mcolumn << ", " << expr[i].mname << endl;

    // build cons
    head = AddNode( expr );
    DP( DEBUG, "AddNode succeeded!" );

    
    // pretty print
    gM = 2;
    if ( NEED_FIRST_PRETTYPRINT ) {
      if (  !IsATOM( head ) ) { // S-expressions not an ATOM
        cout << "( ";
        head->mtype = LEFT_PAREN; // !!!!!!!!!!!!!!!!!!!!!!!!! test !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      } // if

      PrettyPrint( head );
      DP( DEBUG, "PrettyPrint succeeded!" );
    } // if
    else {
      gM = 2;
      if ( !IsATOM( head ) ) { // S-expressions not an ATOM
        head->mtype = LEFT_PAREN; // !!!!!!!!!!!!!!!!!!!!!!!!! test !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      } // if

      RetrivalTree( head ) ; 
    } // else 
    

    return head ; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  } // BuildCons()

  Token *AddNode( vector<Token> expr ) { // build tree
    if ( expr.size() == 1 ) { // ATOM
      Token *t = new Token();
      t->mname = expr[0].mname;
      t->mtype = expr[0].mtype;
      t->mleft = NULL;
      t->mright = NULL;
      return t;
    } // if
    else {
      int parenCount = 0;
      int start = 0, end = 0, dotindex = -1;
      if ( expr[0].mname == "(" )
        expr.erase( expr.begin() );
      if ( expr[expr.size() - 1].mname == ")" )
        expr.pop_back();
      for ( int i = 0; i < expr.size() && dotindex == -1 ; i++ ) {
        if ( expr[i].mname == "(" )
          parenCount++;
        else if ( expr[i].mname == ")" )
          parenCount--;
        if ( parenCount == 0 && expr[i].mname == "." )
          dotindex = i;
      } // for

      vector<Token> tokensL = Slicing( expr, 0, dotindex );
      vector<Token> tokensR = Slicing( expr, dotindex + 1, expr.size() );

      Token *t = new Token();
      t->mtype = DOT;          // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      t->mleft = AddNode( tokensL );
      t->mright = AddNode( tokensR );
      return t;
    } // else
  } // AddNode()
}; // class Token

struct BindingSymbol{
  string name; // token name
  Token * bindingValue;
};

vector<BindingSymbol> gSymbolTable;

bool IsAFunction( Token* temp ) ;
void CheckAndGetList( Token * root, vector<Token *> &vTemp ) ;
Token *EvaluateSExp( Token * root ) ;
Token * Cons(  vector<Token *> args ) ;
Token * List( vector<Token *> args ) ;
Token * Define( vector<Token *> args ) ;
bool IsAlreadyDefined( string symbol, int &originalIndex ) ;
Token * Car( vector<Token *> args ) ;
Token * Cdr( vector<Token *> args ) ;
Token * IsAtom( vector<Token *> args ) ;
Token * IsPair( vector<Token *> args ) ;
Token * IsList( vector<Token *> args ) ;
Token * IsNull( vector<Token *> args ) ;
Token * IsInt( vector<Token *> args ) ;
Token * IsReal( vector<Token *> args ) ;
Token * IsNumber( vector<Token *> args ) ;
Token * IsString( vector<Token *> args ) ;
Token * IsBool( vector<Token *> args ) ;
Token * IsSymbol( vector<Token *> args ) ;
Token * Add( vector<Token *> args ) ;
Token * Minus( vector<Token *> args ) ;
Token * Mul( vector<Token *> args ) ;
Token * Div( vector<Token *> args ) ;
Token * Not( vector<Token *> args ) ;
Token * And( vector<Token *> args ) ;
Token * Or( vector<Token *> args ) ;
Token * BiggerThan( vector<Token *> args ) ;
Token * BiggerThanOrEqu( vector<Token *> args ) ;
Token * SmallerThan( vector<Token *> args ) ;
Token * SmallerThanOrEqu( vector<Token *> args ) ;
Token * EqualTo( vector<Token *> args ) ;
Token * StringAppend( vector<Token *> args ) ;
Token * IsStringBiggerThan( vector<Token *> args ) ;
Token * IsStringSmallerThan( vector<Token *> args ) ;
Token * IsStringEquTo( vector<Token *> args ) ;
Token * IsEeqv( vector<Token *> args ) ;
Token * IsEqual( vector<Token *> args ) ;
Token * Begin( vector<Token *> args ) ;
Token * If( Token * root, vector<Token *> args ) ;
Token * Cond( Token * root, vector<Token *> args ) ;
Token * CleanEnvironment( vector<Token *> args ) ;


int main() {
  int uTestNum = 0;
  char ch = '\0';

  if ( CALTEST )
    cin >> uTestNum;

  /*
  if ( uTestNum == 2 && CALTEST == true ) 
    exit( 0 ) ;
  */

  cout << "Welcome to OurScheme!" << endl << endl << "> ";
  char temp, remainCh = '\0';
  bool exit = false;
  vector<Token> expr; // expr is made up of tokens
  vector<Token> tokens;
  Token *head = NULL;
  Token t;
  

  // t.StartGetToken();
  while ( exit == false && geofcheck == false ) {
    // GetToken
    expr.clear();
    expr = t.GetToken();

    DP( DEBUG, "GetToken succeeded!" );
    if ( DEBUG_Print ) cout << "GetToken succeeded!\ni, Column, name" << endl;
    if ( DEBUG_Print )
      for ( int i = 0; i < expr.size() ; i++ )
        cout << i << ", " << expr[i].mcolumn << ", " << expr[i].mname << endl;

    if ( t.IsExit( expr ) == true )
      exit = true;
    else if ( gerrorMsg == true ) {
      gerrorMsg = false;
      /*
      if ( geofcheck == false ) 
        cout << endl << "> ";
      */
      cout << endl << "> ";
      gLine = 1;
      gColumn = 0;
    } // else if
    else if ( expr.size() != 0 ) { // get a token﹃

      // 浪琩ゅ猭
      tokens = expr;
      head = t.BuildCons( expr, head ); // !!!!!!!!!!!!!!!!!!!!!!! return head pointer セ磅︽Ч硂︽head is NULL
      DP( DEBUG, "PrettyPrint succeeded!" );

      // do something like calculate 3+5 = 8
      /*
      if ( head->mtype == LEFT_PAREN ) {
        
        head = EvaluateSExp( head ) ;
      } // if
      else if ( 0 ) // is ATOM
        ; // 钡
      */
      
      try {
        head = EvaluateSExp( head ) ;

        if ( head != NULL ) { // defined  head == NULL
          gM = 2 ;
          if ( !t.IsATOM( head ) ) { // S-expressions not an ATOM
            cout << "( ";
            t.PrettyPrint( head ) ;
          } // if
          else  
            t.PrettyPrint( head ) ;
        } // if
      } // try
      catch ( const char* str ) {
        cout << str;
        if ( strcmp( str, "ERROR (non-list) : " ) == 0 || strcmp( str, "ERROR (DEFINE format) : " ) == 0 || 
             strcmp( str, "ERROR (DEFINE format) : " ) == 0 ) {
          gM = 2 ;
          if ( !t.IsATOM( head ) ) { // S-expressions not an ATOM
            cout << "( ";
            t.PrettyPrint( head ) ;
          } // if
          else  
            t.PrettyPrint( head ) ;
        } // if
      } // catch

      gLevel = 0;
      gerrorMsg = false;
      cout << endl << "> ";
      gLine = 1;
      gColumn = 0;
    } // else if

  } // while()
  
  if ( geofcheck == true )
    cout << "ERROR (no more input) : END-OF-FILE encountered";
  cout << endl << "Thanks for using OurScheme!";
  return 0;
} // main()

bool IsWhiteSpace( char temp ) {
  if ( temp == '\n' || temp == '\t' || temp == ' ' )
    return true;
  return false;
} // IsWhiteSpace()

char GetAChar() { // get a character and count gLine, gColumn
  char ch = '\0';
  ch = getchar();
  if ( ch == EOF ) {
    geofcheck = true;
    gColumn = 0;
  } // if
  else if ( ch == '\n' ) {
    gLine++;
    gColumn = 0;
  } // if
  else
    gColumn++;

  return ch;
} // GetAChar()

void SkipWhiteSpace( char & temp ) {
  temp = cin.peek();
  while ( IsWhiteSpace( temp ) ) {
    temp = GetAChar();
    temp = cin.peek();
  } // while
} // SkipWhiteSpace()

bool IsSeparator( char temp ) {
  if ( IsWhiteSpace( temp ) || temp == '(' || temp == ')' || temp == '\'' || temp == '"' || temp == ';' )
    return true;
  return false;
} // IsSeparator()

bool IsInt( string & str ) {
  int num = 0;
  for ( int i = 0; i < str.size() ; i++ ) {
    if ( i == 0 && ( str[i] == '+' || str[i] == '-' ) )
      ;
    else if ( str[i] >= '0' && str[i] <= '9' )
      num++ ;
    else
      return false;
  } // for

  if ( num == 0 )
    return false;

  if ( str[0] == '+' )
    str.erase( 0, 1 );

  return true;
} // IsInt()

bool IsFloat( string & str ) {
  int check = 0, num = 0;
  for ( int i = 0; i < str.size() ; i++ ) {
    if ( i == 0 && ( str[i] == '+' || str[i] == '-' ) )
      ;
    else if ( str[i] >= '0' && str[i] <= '9' )
      num++ ;
    else if ( str[i] == '.' )
      check++;
    else
      return false;

    if ( check > 1 )
      return false;
  } // for

  if ( num == 0 )
    return false;

  if ( str[0] == '+' )
    str.erase( 0, 1 );

  return true;
} // IsFloat()

float StrToFloat( string str ) {
  float result = 0.0;
  bool dot = false;
  int index = 1;

  for ( int i = 0; i < str.size() ; i++ ) {
    if ( str[i] >= '0' && str[i] <= '9' ) {
      result = result * 10 + ( str[i] - '0' );
      if ( dot ) index = index * 10;
    } // if
    else if ( str[i] == '.' ) dot = true;
  } // for

  if ( str[0] == '-' ) return -result / index;

  return result / index;
} // StrToFloat()



void CheckAndGetList( Token * root, vector<Token *> &vTemp ) {
  
  Token t;
  for ( Token * walk = root->mright ; walk != NULL ; walk = walk->mright ) { 
    // walk  "." ênode
    if ( walk->mtype == NIL && walk->mright == NULL )
      return ; // 竒┏  磷耞Αerror
    else if ( walk->mright == NULL && walk->mtype != NIL ) { // non list 
      cout << "ERROR (non-list) : ";
      gM = 2 ;
      if ( !t.IsATOM( root ) ) { // S-expressions not an ATOM
        cout << "( ";
        t.PrettyPrint( root ) ;
      } // if
      else  
        t.PrettyPrint( root ) ;
      throw "";
      // any ERRORs?????????????????????????
    } // if
    else if ( walk->mright->mright == NULL && walk->mright->mtype != NIL ) { // non list 
      cout << "ERROR (non-list) : ";
      gM = 2 ;
      if ( !t.IsATOM( root ) ) { // S-expressions not an ATOM
        cout << "( ";
        t.PrettyPrint( root ) ;
      } // if
      else  
        t.PrettyPrint( root ) ;
      throw "";
      // any ERRORs?????????????????????????
    } // if
    else if ( walk->mtype != NIL ) {
      vTemp.push_back( walk->mleft );
      if ( DEBUG_PRINT_EVAL ) cout << "mname: " << walk->mleft->mname << endl ;
      if ( DEBUG_PRINT_EVAL ) cout << "mtype: " << walk->mleft->mtype << endl ;
    } // else if 
    
  } // for


  DP( DEBUG, "GetList succed!!" );
} // CheckAndGetList()


/*
bool IsIncludeSubString( string origiStr, string targetSubString ) {
  if ( origiStr.size() < targetSubString.size() ) { // ﹃璶т﹃
    return false ;

  } // if
  else { 
    for ( int i = 0 ; i < origiStr.size() ; i++ ) {
      for ( int j = 0 ; j < targetSubString.size() ; j++ ) {
        if ( origiStr.at( i + j ) != targetSubString.at( j ) ) {
          return false ;
        }

      } // for
    } // for

  } // else

} // IsIncludeSubString()

*/

Token * EvaluateSExp( Token * root ) {
  int index = 0 ;
  bool isBeReplacedOnce = false  ;
  if ( root->mtype == LEFT_PAREN ) { // left is a symbol to eval
    gLevel++;
    vector<Token *> args;
    CheckAndGetList( root, args );

    DP( DEBUG, "CheckAndGetList succed!!" );

    if ( DEBUG_PRINT_EVAL ) cout << "RootLeft_mname: " << root->mleft->mname << endl ;
    if ( DEBUG_PRINT_EVAL ) cout << "RootLeft_mtype: " << root->mleft->mtype << endl ;
    // if ( DEBUG_PRINT_EVAL ) cout << "args: " << args.at( 0 )->mname << endl ;
    // if ( DEBUG_PRINT_EVAL ) cout << "args: " << args.at(0)->mleft->mname << endl ;

    if ( IsAlreadyDefined( root->mleft->mname, index )  ) {  // check this function name is binding or not
      isBeReplacedOnce = true ;
      root->mleft->mname.assign( gSymbolTable.at( index ).bindingValue->mname ) ; // !!!!!!!! string assign
    } // if

    /*
    if ( IsAlreadyDefined( root->mleft->mname, index )  ) { // check this function name is binding or not

      root->mleft->mname.assign( gSymbolTable.at( index ).bindingValue->mname ) ; // !!!!!!!! string assign
    
      if ( root->mleft->mname.size() > 11 ) {
        if ( strstr( root->mleft->mname.c_str(), "#<procedure" ) != NULL ) {
          root->mleft->mname.erase( 0, 12 );
          root->mleft->mname.erase( root->mleft->mname.end() - 1 );

          if ( DEBUG_PRINT_EVAL_P2 ) 
            cout << "already str #<procedure" << endl << root->mleft->mname << endl  ;

        } // if
      } // if

    } // if
    else if (  root->mleft->mname.size() > 11 ) {
      if ( strstr( root->mleft->mname.c_str(), "#<procedure" ) != NULL ) {
        root->mleft->mname.erase( 0, 12 );
        root->mleft->mname.erase( root->mleft->mname.end() - 1 );

        if ( DEBUG_PRINT_EVAL_P2 ) 
          cout << "str #<procedure" << endl << root->mleft->mname << endl  ;

      } // if
    } // else if
    */

    if ( IsAFunction( root->mleft ) ) {
      if ( DEBUG_PRINT_EVAL ) cout << "IS A Function! " << endl ;
      if ( strcmp( root->mleft->mname.c_str(), "cons" ) == 0  
           || strcmp( root->mleft->mname.c_str(), "#<procedure cons>" ) == 0 ) 
        root = Cons( args ) ;
      else if ( strcmp( root->mleft->mname.c_str(), "list" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure list>" ) == 0 ) 
        root = List( args ) ;
      else if ( strcmp( root->mleft->mname.c_str(), "car" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure car>" ) == 0 )
        root = Car( args ) ;
      else if ( strcmp( root->mleft->mname.c_str(), "cdr" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure cdr>" ) == 0 ) 
        root = Cdr( args ) ;
      else if ( strcmp( root->mleft->mname.c_str(), "define" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure define>" ) == 0 ) 
        root = Define( args ) ;
      else if ( strcmp( root->mleft->mname.c_str(), "clean-environment" ) == 0 ) 
        root = CleanEnvironment( args );
      
      else if ( strcmp( root->mleft->mname.c_str(), "atom?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure atom?>" ) == 0 ) 
        root = IsAtom( args );
      else if ( strcmp( root->mleft->mname.c_str(), "pair?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure pair?>" ) == 0 ) 
        root = IsPair( args );
      else if ( strcmp( root->mleft->mname.c_str(), "list?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure list?>" ) == 0 ) 
        root = IsList( args );
      else if ( strcmp( root->mleft->mname.c_str(), "null?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure null?>" ) == 0 ) 
        root = IsNull( args );
      else if ( strcmp( root->mleft->mname.c_str(), "integer?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure integer?>" ) == 0 ) 
        root = IsInt( args );
      else if ( strcmp( root->mleft->mname.c_str(), "real?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure real?>" ) == 0 ) 
        root = IsReal( args );
      else if ( strcmp( root->mleft->mname.c_str(), "number?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure number?>" ) == 0 ) 
        root = IsNumber( args );
      else if ( strcmp( root->mleft->mname.c_str(), "string?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure string?>" ) == 0 ) 
        root = IsString( args );
      else if ( strcmp( root->mleft->mname.c_str(), "boolean?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure boolean?>" ) == 0 ) 
        root = IsBool( args );
      else if ( strcmp( root->mleft->mname.c_str(), "symbol?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure symbol?>" ) == 0 ) 
        root = IsSymbol( args );

      else if ( strcmp( root->mleft->mname.c_str(), "+" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure +>" ) == 0 ) 
        root = Add( args );
      else if ( strcmp( root->mleft->mname.c_str(), "-" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure ->" ) == 0 ) 
        root = Minus( args );
      else if ( strcmp( root->mleft->mname.c_str(), "*" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure *>" ) == 0 ) 
        root = Mul( args );
      else if ( strcmp( root->mleft->mname.c_str(), "/" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure />" ) == 0 ) 
        root = Div( args );

      else if ( strcmp( root->mleft->mname.c_str(), "not" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure not>" ) == 0 ) 
        root = Not( args );
      else if ( strcmp( root->mleft->mname.c_str(), "and" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure and>" ) == 0 ) 
        root = And( args );
      else if ( strcmp( root->mleft->mname.c_str(), "or" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure or>" ) == 0 ) 
        root = Or( args );

      else if ( strcmp( root->mleft->mname.c_str(), ">" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure >>" ) == 0 ) 
        root = BiggerThan( args );
      else if ( strcmp( root->mleft->mname.c_str(), ">=" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure >=>" ) == 0 ) 
        root = BiggerThanOrEqu( args );
      else if ( strcmp( root->mleft->mname.c_str(), "<" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure <>" ) == 0 ) 
        root = SmallerThan( args );
      else if ( strcmp( root->mleft->mname.c_str(), "<=" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure <=>" ) == 0 ) 
        root = SmallerThanOrEqu( args );
      else if ( strcmp( root->mleft->mname.c_str(), "=" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure =>" ) == 0 ) 
        root = EqualTo( args );

      else if ( strcmp( root->mleft->mname.c_str(), "string-append" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure string-append>" ) == 0 ) 
        root = StringAppend( args );
      else if ( strcmp( root->mleft->mname.c_str(), "string>?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure string>?>" ) == 0 ) 
        root = IsStringBiggerThan( args );
      else if ( strcmp( root->mleft->mname.c_str(), "string<?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure string<?>" ) == 0 ) 
        root = IsStringSmallerThan( args );
      else if ( strcmp( root->mleft->mname.c_str(), "string=?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure string=?>" ) == 0 ) 
        root = IsStringEquTo( args );
      else if ( strcmp( root->mleft->mname.c_str(), "eqv?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure eqv?>" ) == 0 ) 
        root = IsEeqv( args );
      else if ( strcmp( root->mleft->mname.c_str(), "equal?" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure equal?>" ) == 0 ) 
        root = IsEqual( args );

      else if ( strcmp( root->mleft->mname.c_str(), "begin" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure begin>" ) == 0 ) 
        root = Begin( args );
      else if ( strcmp( root->mleft->mname.c_str(), "if" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure if>" ) == 0 ) 
        root = If( root, args );
      else if ( strcmp( root->mleft->mname.c_str(), "cond" ) == 0 
                || strcmp( root->mleft->mname.c_str(), "#<procedure cond>" ) == 0 ) 
        root = Cond( root, args );

      else if ( strcmp( root->mleft->mname.c_str(), "exit" ) == 0 ) {
        if ( gLevel != 1 ) 
          throw "ERROR (level of EXIT)\n";
        if ( root->mright != NULL )
          throw "ERROR (incorrect number of arguments) : exit\n";
      } // else if

    } // if isAFunction
    
    else if ( root->mleft->mtype == QUOTE || ( strcmp( root->mleft->mname.c_str(), "quote" ) == 0 ) ) {
      if ( DEBUG_PRINT_EVAL ) cout << "left is quote" << endl ;
      if ( DEBUG_PRINT_EVAL ) cout << "quote 1st element: " <<  root->mright ->mleft->mleft->mname << endl ;

      return root->mright->mleft ; // 硂妓叉奔纯()
                                   // root->mright 穦Τ糷 ()
    } // else if

    // else if ( strcmp( root->mleft->mname.c_str(), "(" ) == 0 && root->mIsBeEvaluate == false ) { 
    else if ( strcmp( root->mleft->mname.c_str(), "(" ) == 0 ) {
      // No Name Function
      if ( DEBUG_PRINT_EVAL_P2 ) cout << "No Name Function" << endl ;
      if ( DEBUG_PRINT_EVAL_P2 ) cout << "function Name: " <<  root->mleft->mname << endl ;
      root->mleft = EvaluateSExp( root->mleft ) ;

      if ( DEBUG_PRINT_EVAL_P2 ) cout << "No Name Fun return "  << endl ;

      return EvaluateSExp( root ) ;
    } // else if
    else {
      Token t;
      // ERROR

      
      if ( ( t.IsATOM( root->mleft ) == true && root->mleft->mtype != SYMBOL ) || isBeReplacedOnce ) {
        // (a 7 ) 硂贺 竒砆竚传筁Ω狥狥 (眖gSymbolTable 筁 )

        cout << "ERROR (attempt to apply non-function) : " << root->mleft->mname << endl;
        throw "";
      } // if

      else if ( strcmp( root->mleft->mname.c_str(), "(" ) == 0 || root->mleft->mtype == LEFT_PAREN ) { 
        // No Name Function
        if ( DEBUG_PRINT_EVAL_P2 ) cout << "No Name Function attempt to apply non-function" ;

        cout << "ERROR (attempt to apply non-function) : " << root->mname;
        gM = 2 ;
        if ( !t.IsATOM( root->mleft ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( root->mleft  ) ;
        } // if
        else  
          t.PrettyPrint( root->mleft  ) ;
        throw "";
      } // else if
      else {

        if ( DEBUG_PRINT_EVAL_P2 ) cout << " else: " << root->mleft->mname << endl ;

        // ERROR
        cout << "ERROR (unbound symbol) : " << root->mleft->mname << endl;
        // cout << "ERROR (unbound symbol) : " << root->mleft->mname << endl;
        throw "";
      } // else

    } // else

  } // if
  
  else if ( root->mtype == SYMBOL ) { // check this symbol is binding or not
    for ( int i = 0 ; i < gSymbolTable.size() ; i ++ ) {
      if ( strcmp( root->mname.c_str(), gSymbolTable.at( i ).name.c_str() ) == 0 ) {
        
        return gSymbolTable.at( i ).bindingValue ;
      } // if

    } // for()

    if ( IsAFunction( root ) == true ) {
      root->mname = "#<procedure " + root->mname + ">";

      return root;
    } // if

    if ( DEBUG_PRINT_EVAL_P2 ) cout << "symbol unbound: " << root->mleft->mname << endl ;
    // ERROR
    cout << "ERROR (unbound symbol) : " << root->mname << endl;
    throw "";
    // cout << "ERROR: Unbound value!" << endl ;
    
  } // else if 

  return root;

} // EvaluateSExp()

bool IsAFunction( Token* temp ) { // quote 矪瞶
  for ( int i = 0; i < FunctionCount ; i++  ) {
    if ( ( strcmp( temp->mname.c_str(), gFunctionList[i].c_str() ) == 0 ) && ( temp->mtype != QUOTE ) ) {
      // temp->mtype ; // 璶ぃ璶ノtype ㄓ癘魁硂function琌或function(ノindexfunction ぇ摸 )
      if ( DEBUG_PRINT_EVAL ) cout << "Func Name: " << gFunctionList[i].c_str() << endl ;
      return true ;
    } // if
  } // for

  return false ;
} // IsAFunction()

Token * Cons( vector<Token *> args ) {
  if ( args.size() != 2 ) {
    throw "ERROR (incorrect number of arguments) : cons\n";
    // cout << "ERROR" << endl ; // ERROR
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      // if ( strcmp( args.at( i )->mname.c_str(), "(" ) == 0 )
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
    
    } // for

    Token * temp = new Token() ;
    temp->mleft = args.at( 0 );
    temp->mright = args.at( 1 );

    return temp ;
  } // else 

  return NULL ;
} // Cons()

Token * List( vector<Token *> args ) {

  if ( args.empty() ) { // garbage
    /*
    Token * nilnode = new Token();
    nilnode->mleft =  new Token();
    nilnode->mleft->mname = "nil";
    nilnode->mleft->mtype = NIL ;
    nilnode->mright = new Token() ; // 琧攫ネ港nil竊翴
    nilnode->mright->mname = "nil";
    nilnode->mright->mtype = NIL ;
    */
    Token * nilnode = new Token();
    nilnode->mname = "nil";
    nilnode->mtype = NIL ;
    nilnode->mright = NULL;
    nilnode->mleft = NULL ;

    
    if ( DEBUG_PRINT_EVAL ) cout << "no args!!" <<  endl ; // ERROR
    return nilnode; 
  } // if
  else {
    Token * tempNode = new Token();
    Token * walk = NULL;
    tempNode->mname = "(";
    tempNode->mtype = LEFT_PAREN;
    tempNode->mleft = EvaluateSExp( args.at( 0 ) ) ; // 穝evaluate 

    walk = tempNode;
    for ( int i = 1 ; i < args.size() ; i ++ ) {
      walk->mright = new Token();
      walk = walk->mright;
      walk ->mleft = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 

    } // for()
    
    walk->mright = new Token(); // 钡Чぇ 程node竊翴模ネ港nil
    walk = walk->mright;
    walk->mname = "nil";
    walk->mtype = NIL ;

    return tempNode;

  } // else

} // List()

Token * Define( vector<Token *> args ) {
  int symbolIndex = -1;


  if ( gLevel != 1 ) {
    // ERROR 
    throw "ERROR (level of DEFINE)\n";
  } // if

  else if ( args.size() != 2 ) {
    // ERROR 
    throw "ERROR (DEFINE format) : ";
  } // if

  else {
    args.at( 1 ) = EvaluateSExp( args.at( 1 ) ) ; // eval 2nd argument

    if ( args.at( 0 )->mtype != SYMBOL )
      throw "ERROR (DEFINE format) : "; // ERROR// ERROR
    else if ( IsAFunction( args.at( 0 ) ) ) {
      // ERROR 
      throw "ERROR (DEFINE format) : ";
    } // if
    else if ( IsAlreadyDefined( args.at( 0 )->mname,  symbolIndex ) ) { // symbol already binding, replace it
      gSymbolTable.at( symbolIndex ).bindingValue = args.at( 1 );
      cout << args.at( 0 )->mname << " defined" << endl ;
    } // else if
    else {
      BindingSymbol tempBS;
      tempBS.name = args.at( 0 )->mname;
      tempBS.bindingValue = args.at( 1 );

      gSymbolTable.push_back( tempBS );
      cout << args.at( 0 )->mname << " defined" << endl ;
    } // else 
    
  } // else

  return NULL ;

} // Define()

bool IsAlreadyDefined( string symbol, int &originalIndex ) {
  for ( int i = 0 ; i < gSymbolTable.size() ; i ++ ) {
    if ( strcmp( symbol.c_str(), gSymbolTable.at( i ).name.c_str() ) == 0 ) {
      originalIndex = i ;
      return true ;
    } // if

  } // for()

  originalIndex = -1 ; // didn't binding
  return false;

} // IsAlreadyDefined()

Token * Car( vector<Token *> args ) { // return 1st element
  if ( args.size() != 1 ) {
    // ERROR
    throw "ERROR (incorrect number of arguments) : car\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );

    Token t;
    if ( t.IsATOM( args.at( 0 ) ) ) {
      cout << "ERROR (car with incorrect argument type) : ";
      t.PrettyPrint( args.at( 0 ) );
      throw "";
    } // if


    return args.at( 0 )->mleft;

  } // else

  return NULL ;
} // Car()

Token * Cdr( vector<Token *> args ) { // CDR 羆琌 return 埃奔材じ

  if ( args.size() != 1 ) {
    // ERROR
    throw "ERROR (incorrect number of arguments) : cdr\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );

    Token t;
    if ( t.IsATOM( args.at( 0 ) ) ) {
      cout << "ERROR (cdr with incorrect argument type) : " << args.at( 0 )->mname << endl;
      throw "";
    } // if
    
    return args.at( 0 )->mright; // ぃ穦糷

  } // else

  return NULL ;

} // Cdr()

Token * IsAtom( vector<Token *> args ) { // atom?

  if ( args.size() != 1 ) {
    // ERROR
    if ( DEBUG_PRINT_EVAL_P2 ) cout << "IsAtom args size : " << args.size() << endl ;
    throw "ERROR (incorrect number of arguments) : atom?\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );
    Token * returnBoolNode = new Token() ;

    if ( args.at( 0 )->mtype == LEFT_PAREN ) {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
      
    } // if
    else {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsAtom()   

Token * IsPair( vector<Token *> args ) { // pair?

  if ( args.size() != 1 ) {
    throw "ERROR (incorrect number of arguments) : pair?\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );
    Token * returnBoolNode = new Token() ;

    if ( args.at( 0 )->mtype == LEFT_PAREN ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
      
    } // if
    else {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsPair()

Token * IsList( vector<Token *> args ) { // list?
  bool isList = true ;
  if ( args.size() != 1 ) {
    throw "ERROR (incorrect number of arguments) : list?\n";
  } // if
  else {

    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );
    Token * returnBoolNode = new Token() ;

    if ( args.at( 0 )->mtype == SYMBOL || args.at( 0 )->mtype == INT || args.at( 0 )->mtype == FLOAT ||
         args.at( 0 )->mtype == STRING || args.at( 0 )->mtype == T ) // nil is list???????
      isList = false;
    else {      
      for ( Token * walk = args.at( 0 ) ; walk != NULL ; walk = walk->mright ) { // ﹍ぃノ琧攫
        // walk  "." ênode
        if ( DEBUG_PRINT_EVAL_P2 ) cout << " walk->mtype: " << walk->mtype << endl ;
        if ( DEBUG_PRINT_EVAL_P2 && ( walk->mright == NULL ) ) cout << " walk->mright = NULL" << endl ;

        if ( walk->mtype == NIL && walk->mright == NULL ) 
          walk->mright = NULL; // 竒┏  磷耞Αerror // The use of 'break' is not allowed here.
        
        else if ( walk->mright->mright == NULL && walk->mright->mtype != NIL ) { // non list 
          if ( DEBUG_PRINT_EVAL_P2 ) cout << "non-list" << endl ;
          isList = false;
          walk->mright = NULL; // important!!!!
          // The use of 'break' is not allowed here.
        } // if
        
      } // for

    } // else

    if ( isList ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // if
    else {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsList() 

Token * IsNull( vector<Token *> args ) { // null?
  // is it the empty list

  if ( args.size() != 1 ) {
    throw "ERROR (incorrect number of arguments) : null?\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );
    Token * returnBoolNode = new Token() ;

    if ( args.at( 0 )->mtype == NIL ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // if
    else {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsNull() 

Token * IsInt( vector<Token *> args ) { // integer?

  if ( args.size() != 1 ) {
    throw "ERROR (incorrect number of arguments) : integer?\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );
    Token * returnBoolNode = new Token() ;

    if ( args.at( 0 )->mtype == INT ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // if
    else {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsInt() 

Token * IsReal( vector<Token *> args ) { // real?

  if ( args.size() != 1 ) {
    throw "ERROR (incorrect number of arguments) : real?\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );
    Token * returnBoolNode = new Token() ;

    if ( args.at( 0 )->mtype == INT || args.at( 0 )->mtype == FLOAT ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // if
    else {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsReal()

Token * IsNumber( vector<Token *> args ) { // number?
  // this case this condition just in OurScheme
  // Lisp a number may not be real
  if ( args.size() != 1 ) {
    throw "ERROR (incorrect number of arguments) : number?\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );
    Token * returnBoolNode = new Token() ;

    if ( args.at( 0 )->mtype == INT || args.at( 0 )->mtype == FLOAT ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // if
    else {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsNumber()

Token * IsString( vector<Token *> args ) { // string?

  if ( args.size() != 1 ) {
    throw "ERROR (incorrect number of arguments) : string?\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );
    Token * returnBoolNode = new Token() ;

    if ( args.at( 0 )->mtype == STRING ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // if
    else {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsString()

Token * IsBool( vector<Token *> args ) { // boolean?

  if ( args.size() != 1 ) {
    throw "ERROR (incorrect number of arguments) : boolean?\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );
    Token * returnBoolNode = new Token() ;

    if ( args.at( 0 )->mtype == T || args.at( 0 )->mtype == NIL ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // if
    else {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsBool()

Token * IsSymbol( vector<Token *> args ) { // symbol?

  if ( args.size() != 1 ) {
    throw "ERROR (incorrect number of arguments) : symbol?\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );
    Token * returnBoolNode = new Token() ;

    if ( args.at( 0 )->mtype == SYMBOL ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // if
    else {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsSymbol()


int StrToInt( string strInt ) {
  // create a stringstream object, to input/output strings
  stringstream ss; 
  
  // a variable named str, that is of string data type
    
  // a variable named num, that is of int data type
  int num;
  
  // extract the string from the str variable (input the string in the stream)
  ss << strInt;
  
  // place the converted value to the int variable
  ss >> num;

  return num;

} // StrToInt()

string ToString( float num ) { //         overridding!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  stringstream ss;
  ss << num; // appending the float value to the streamclass

  return ss.str(); // converting the float value to string
} // ToString()

string ToString( int num ) {
  stringstream ss;
  ss << num; // appending the float value to the streamclass

  return ss.str(); // converting the float value to string
} // ToString()

Token * Add( vector<Token *> args ) { // +
  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  Token * numResult = new Token() ;
  float tempResult = 0.0;
  int intResult = 0;
  bool isFloat = false;
  Token t;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : +\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == INT || args.at( i )->mtype == FLOAT ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (+ with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if
      else if ( args.at( i )->mtype == FLOAT )
        isFloat = true;
        
      tempArgs.clear() ;
    } // for

    if ( isFloat ) {
      for ( int i = 0 ; i < args.size() ; i++ ) 
        tempResult += StrToFloat( args.at( i )->mname );
      
      numResult->mtype = FLOAT ;
      numResult->mname = ToString( tempResult );
    } // if
    else {
      for ( int i = 0 ; i < args.size() ; i++ ) 
        intResult += atoi( args.at( i )->mname.c_str() );
        
      numResult->mtype = INT ;
      numResult->mname = ToString( intResult );
    } // else

    return numResult ;
  } // else

  return NULL;
} // Add()

Token * Minus( vector<Token *> args ) { // -
  Token t;
  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  Token * numResult = new Token() ;
  float tempResult = 0.0;
  int intResult = 0;
  bool isFloat = false;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : -\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == INT || args.at( i )->mtype == FLOAT ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (- with incorrect argument type) : " ;
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if
      else if ( args.at( i )->mtype == FLOAT ) 
        isFloat = true;

      tempArgs.clear() ;
    } // for

    if ( isFloat ) {
      tempResult = StrToFloat( args.at( 0 )->mname );
      for ( int i = 1 ; i < args.size() ; i++ )
        tempResult -= StrToFloat( args.at( i )->mname );

      numResult->mtype = FLOAT ;
      numResult->mname = ToString( tempResult );
    } // if
    else {
      intResult = atoi( args.at( 0 )->mname.c_str() );
      for ( int i = 1 ; i < args.size() ; i++ )
        intResult -= atoi( args.at( i )->mname.c_str() );
      
      numResult->mtype = INT ;
      numResult->mname = ToString( intResult );
    } // else

    return numResult ;
  } // else

  return NULL;
} // Minus()

Token * Mul( vector<Token *> args ) { // "*"

  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  Token * numResult = new Token() ;
  float tempResult = 1.0; // 
  int intResult = 1;
  bool isFloat = false;
  Token t;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : *\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == INT || args.at( i )->mtype == FLOAT ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (* with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if
      else if ( args.at( i )->mtype == FLOAT )
        isFloat = true;

      tempArgs.clear() ;
    } // for

    if ( isFloat ) {
      for ( int i = 0 ; i < args.size() ; i++ ) 
        tempResult *= StrToFloat( args.at( i )->mname );
      
      numResult->mtype = FLOAT ;
      numResult->mname = ToString( tempResult );
    } // if
    else {
      for ( int i = 0 ; i < args.size() ; i++ ) 
        intResult *= atoi( args.at( i )->mname.c_str() );
        
      numResult->mtype = INT ;
      numResult->mname = ToString( intResult );
    } // else

    return numResult ;
  } // else

  return NULL;
} // Mul() 

Token * Div( vector<Token *> args ) { //  /

  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  Token * numResult = new Token() ;
  float tempFloatResult = 0;
  int tempIntResult = 0 ;
  bool isFloat = false;
  Token t;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : /\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == INT || args.at( i )->mtype == FLOAT ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (/ with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if
      else if ( args.at( i )->mtype == FLOAT ) 
        isFloat = true ;

      tempArgs.clear() ;
    } // for


    // ##############################################
    // ########### 耞埃计ぃ0 #####################
    // ##############################################
    for ( int i = 1 ; i < args.size() ; i++ ) {
      if ( args[i]->mtype == FLOAT && StrToFloat( args.at( i )->mname ) == 0 )
        throw "ERROR (division by zero) : /\n";
      else if ( args[i]->mtype == INT && StrToInt( args.at( i )->mname ) == 0 )
        throw "ERROR (division by zero) : /\n";
    } // for
    
    // 常琌number
    // 盢材把计倒砆埃计
    if ( isFloat ) {
      tempFloatResult = StrToFloat( args.at( 0 )->mname );

      for ( int i = 1 ; i < args.size() ; i++ ) {
        tempFloatResult /= StrToFloat( args.at( i )->mname );
      } // for

      numResult->mtype = FLOAT ;
      numResult->mname = ToString( tempFloatResult );
    } // if
    else {
      tempIntResult = atoi( args.at( 0 )->mname.c_str() );

      for ( int i = 1 ; i < args.size() ; i++ ) {
        tempIntResult /= atoi( args.at( i )->mname.c_str() );
      } // for

      numResult->mtype = INT ;
      numResult->mname = ToString( tempIntResult );
    } // else

    return numResult ;
  } // else

  return NULL;
} // Div()

Token * Not( vector<Token *> args ) { // not

  if ( args.size() != 1 ) {
    throw "ERROR (incorrect number of arguments) : not\n";
  } // if
  else {
    args.at( 0 ) = EvaluateSExp( args.at( 0 ) );
    Token * returnBoolNode = new Token() ;

    if ( args.at( 0 )->mtype == NIL ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // if
    else {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // Not()

Token * And( vector<Token *> args ) {
  // ( and        ; 'and' either returns the evaluated result of
  //   (pair? 3)  ; the first one that is evaluated to nil
  //   a          ; or the evaluated result of the last one
  // )                     

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : and\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate

    
      if ( DEBUG_PRINT_EVAL_P2 ) cout << "total: " << args.size() <<  "and at: " << i << endl ;

      if ( args.at( i )->mtype == NIL ) 
        return args.at( i );
    } // for

    return args.back();
  } // else

  return NULL;
} // And()

Token * Or( vector<Token *> args ) {
  // ( or         ; 'or' either returns the evaluated result of
  //   a          ; the first one that is not evaluated to nil
  //   (null? ()) ; or the evaluated result of the last one
  // )

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : or\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) 
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate

    for ( int i = 0 ; i < args.size() ; i++ ) {
      if ( args.at( i )->mtype != NIL ) 
        return args.at( i );
    } // for

    return args.back();
  } // else

  return NULL;
} // Or()

Token * BiggerThan( vector<Token *> args ) { // >
  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  bool onceSmallerOrEqualTo = false;
  float firstNum = 0;
  Token t;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : >\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == INT || args.at( i )->mtype == FLOAT ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (> with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if

      tempArgs.clear() ;
    } // for

    // all are num
    firstNum = StrToFloat( args.at( 0 )->mname );

    for ( int i = 1 ; i < args.size() ; i++ ) { // compare 
      if ( firstNum <= StrToFloat( args.at( i )->mname ) ) //  a <= b -> a is Not BiggerThan b
        onceSmallerOrEqualTo = true ;
      firstNum = StrToFloat( args.at( i )->mname );
    } // for

    Token * returnBoolNode = new Token() ;

    if ( onceSmallerOrEqualTo ) {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // if
    else {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // BiggerThan()

Token * BiggerThanOrEqu( vector<Token *> args ) { // >=
  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  bool onceSmallerThan = false;
  float firstNum = 0;
  Token t;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : >=\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == INT || args.at( i )->mtype == FLOAT ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (>= with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if

      tempArgs.clear() ;
    } // for

    // all are num
    firstNum = StrToFloat( args.at( 0 )->mname );

    for ( int i = 1 ; i < args.size() ; i++ ) { // compare 
      if ( firstNum < StrToFloat( args.at( i )->mname ) ) //  a < b -> a is Not BiggerThanOrEqu b
        onceSmallerThan = true ;
      firstNum = StrToFloat( args.at( i )->mname );
    } // for

    Token * returnBoolNode = new Token() ;

    if ( onceSmallerThan ) {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // if
    else {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // BiggerThanOrEqu()

Token * SmallerThan( vector<Token *> args ) {  // <
  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  bool onceBiggerOrEqualTo = false;
  float firstNum = 0;
  Token t;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : <\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == INT || args.at( i )->mtype == FLOAT ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (< with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if

      tempArgs.clear() ;
    } // for

    // all are num
    firstNum = StrToFloat( args.at( 0 )->mname );

    for ( int i = 1 ; i < args.size() ; i++ ) { // compare 
      if ( firstNum >= StrToFloat( args.at( i )->mname ) ) //  a >= b -> a is Not SmallerThan b
        onceBiggerOrEqualTo = true ;
      firstNum = StrToFloat( args.at( i )->mname );
    } // for

    Token * returnBoolNode = new Token() ;

    if ( onceBiggerOrEqualTo ) {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // if
    else {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // SmallerThan()

Token * SmallerThanOrEqu( vector<Token *> args ) {  // <=
  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  bool onceBiggerThan = false;
  float firstNum = 0;
  Token t;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : <=\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == INT || args.at( i )->mtype == FLOAT ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (<= with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if

      tempArgs.clear() ;
    } // for

    // all are num
    firstNum = StrToFloat( args.at( 0 )->mname );

    for ( int i = 1 ; i < args.size() ; i++ ) { // compare 
      if ( firstNum > StrToFloat( args.at( i )->mname ) ) //  a > b -> a is Not SmallerThanOrEqu b
        onceBiggerThan = true ;
      firstNum = StrToFloat( args.at( i )->mname );
    } // for

    Token * returnBoolNode = new Token() ;

    if ( onceBiggerThan ) {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // if
    else {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // SmallerThanOrEqu()

Token * EqualTo( vector<Token *> args ) {  // =
  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  bool notEqual = false;
  float firstNum = 0, otherNum = 0;
  Token t;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : =\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == INT || args.at( i )->mtype == FLOAT ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (= with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if

      tempArgs.clear() ;
    } // for

    // all are num
    firstNum = StrToFloat( args.at( 0 )->mname );

    for ( int i = 1 ; i < args.size() ; i++ ) { // compare
      otherNum = StrToFloat( args.at( i )->mname );
      if ( DEBUG_PRINT_EVAL_P2 ) cout << "a - b: " << firstNum - otherNum << endl ;
      if ( ( firstNum - otherNum ) < -0.01 || 0.01 < ( firstNum - otherNum ) ) // 畉0.01碞耞ぃ
        notEqual = true ;
      firstNum = otherNum;
    } // for

    Token * returnBoolNode = new Token() ;

    if ( notEqual ) {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // if
    else {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // EqualTo()

Token * StringAppend( vector<Token *> args ) {  // string-append
  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  string firstStr = "", tempStr ;
  Token t;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : string-append\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == STRING ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (string-append with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if

      tempArgs.clear() ;
    } // for

    // "hi," + " hello" = "hi, hello"
    // NOT = "hi,"" hello"

    // all are STRING
    firstStr = args.at( 0 )->mname;
    firstStr.erase( firstStr.begin() ); // erase 1st "
    firstStr.erase( firstStr.end() - 1 ); // erase last "
    if ( DEBUG_PRINT_EVAL_P2 ) cout << firstStr << endl ;

    for ( int i = 1 ; i < args.size() ; i++ ) { // compare
      tempStr = args.at( i )->mname;
      tempStr.erase( tempStr.begin() ); // erase 1st "
      tempStr.erase( tempStr.end() - 1 ); // erase last "

      firstStr += tempStr;
    } // for

    // 程繷Ю ""
    firstStr.insert( 0, "\"" );
    firstStr.insert( firstStr.size(), "\"" );
    // if ( DEBUG_PRINT_EVAL_P2 ) cout << firstStr << endl ;


    Token * returnNode = new Token() ;

    returnNode->mtype = STRING;
    returnNode->mname = firstStr;

    return returnNode;
  } // else

  return NULL;
} // StringAppend()

Token * IsStringBiggerThan( vector<Token *> args ) {  // string>?
  Token t;
  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  string firstStr = "", tempStr ;
  bool onceSmllerThanOrEqual = false ;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : string>?\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == STRING ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (string>? with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if

      tempArgs.clear() ;
    } // for

    // "hi," + " hello" = "hi, hello"
    // NOT = "hi,"" hello"

    // all are STRING
    firstStr = args.at( 0 )->mname;
    firstStr.erase( firstStr.begin() ); // erase 1st "
    firstStr.erase( firstStr.end() - 1 ); // erase last "

    for ( int i = 1 ; i < args.size() ; i++ ) { // compare
      tempStr = args.at( i )->mname;
      tempStr.erase( tempStr.begin() ); // erase 1st "
      tempStr.erase( tempStr.end() - 1 ); // erase last "

      if ( strcmp( firstStr.c_str(), tempStr.c_str() ) == 0 || 
           strcmp( firstStr.c_str(), tempStr.c_str() ) < 0 ) {
        onceSmllerThanOrEqual = true ;
        if ( DEBUG_PRINT_EVAL_P2 ) cout << "onceSmllerThanOrEqual" << endl ;

      } // if

      firstStr = tempStr;   // 璶ㄌゑ癸
    } // for

    Token * returnBoolNode = new Token() ;

    if ( onceSmllerThanOrEqual ) {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // if
    else {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsStringBiggerThan()

Token * IsStringSmallerThan( vector<Token *> args ) {  // string<?
  Token t;
  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  string firstStr = "", tempStr ;
  bool onceBiggerThanOrEqual = false ;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : string<?\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == STRING ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (string<? with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if

      tempArgs.clear() ;
    } // for

    // "hi," + " hello" = "hi, hello"
    // NOT = "hi,"" hello"

    // all are STRING
    firstStr = args.at( 0 )->mname;
    firstStr.erase( firstStr.begin() ); // erase 1st "
    firstStr.erase( firstStr.end() - 1 ); // erase last "

    for ( int i = 1 ; i < args.size() ; i++ ) { // compare
      tempStr = args.at( i )->mname;
      tempStr.erase( tempStr.begin() ); // erase 1st "
      tempStr.erase( tempStr.end() - 1 ); // erase last "

      if ( strcmp( firstStr.c_str(), tempStr.c_str() ) == 0 || 
           strcmp( firstStr.c_str(), tempStr.c_str() ) > 0 ) {
        onceBiggerThanOrEqual = true ;

      } // if

      firstStr = tempStr ; // 璶ㄌゑ癸
    } // for

    Token * returnBoolNode = new Token() ;

    if ( onceBiggerThanOrEqual ) {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // if
    else {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsStringSmallerThan()

Token * IsStringEquTo( vector<Token *> args ) {  // string=?
  Token t;
  vector<Token *> tempArgs; // ノㄓメ秈 IsNumber function 把计
  string firstStr = "", tempStr ;
  bool notEqual = false ;

  if ( args.size() < 2 ) {
    throw "ERROR (incorrect number of arguments) : string=?\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      args.at( i ) = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 
      tempArgs.push_back( args.at( i ) ) ;

      if ( ! ( args.at( i )->mtype == STRING ) ) { // Not Number 
        // but ぃ㊣ !IsNumber 穦 Evaluate ㄢΩ把计
        cout << "ERROR (string=? with incorrect argument type) : ";
        gM = 2;
        if ( !t.IsATOM( args.at( i ) ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( args.at( i ) ) ;
        } // if
        else  
          t.PrettyPrint( args.at( i ) ) ;
        throw "";
      } // if

      tempArgs.clear() ;
    } // for

    // "hi," + " hello" = "hi, hello"
    // NOT = "hi,"" hello"

    // all are STRING
    firstStr = args.at( 0 )->mname;
    firstStr.erase( firstStr.begin() ); // erase 1st "
    firstStr.erase( firstStr.end() - 1 ); // erase last "

    for ( int i = 1 ; i < args.size() ; i++ ) { // compare // 
      tempStr = args.at( i )->mname;
      tempStr.erase( tempStr.begin() ); // erase 1st "
      tempStr.erase( tempStr.end() - 1 ); // erase last "

      if ( strcmp( firstStr.c_str(), tempStr.c_str() ) != 0 ) {
        notEqual = true ;
      } // if

    } // for

    Token * returnBoolNode = new Token() ;

    if ( notEqual ) {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // if
    else {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // else

    return returnBoolNode;
  } // else

  return NULL;
} // IsStringEquTo()

Token * IsEeqv( vector<Token *> args ) {  // eqv?
  /*
  ; eqv? returns "true" only when the two being compared
  ; objects are atoms (except in the case of strings)
  ; or when the two being compared objects "occupy the same memory space".
  */

  Token *firstToken = NULL; 
  Token *secondToken = NULL;
  Token * returnBoolNode = new Token() ;

  if ( args.size() != 2 ) {
    throw "ERROR (incorrect number of arguments) : eqv?\n";
  } // if
  else {
    firstToken = EvaluateSExp( args.at( 0 ) ) ;
    secondToken = EvaluateSExp( args.at( 1 ) ) ;

    if ( firstToken == secondToken ) { // "occupy the same memory space".
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // if
    else if ( firstToken->mtype == T && secondToken->mtype == T ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // else if
    else if ( firstToken->mtype == NIL && secondToken->mtype == NIL ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // else if
    else if ( ( firstToken->mtype == INT || firstToken->mtype == FLOAT ) && 
              ( secondToken->mtype == INT || secondToken->mtype == FLOAT ) ) { 
      // just compared int and float
      // becaus SYMBOL will be evaluated to something like int or float or list....
      if ( firstToken->mname == secondToken->mname ) {
        returnBoolNode->mtype = T;
        returnBoolNode->mname = "#t" ;
      } // if
      else { // list string
        returnBoolNode->mtype = NIL;
        returnBoolNode->mname = "nil" ;
      } // else 
    } // else if
    else {
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else

    return returnBoolNode ;
  } // else

  return NULL;
} // IsEeqv()


void GetThisToken( Token * temp,  string & strToken ) {
  Token t;

  if ( temp->mtype == QUOTE || t.IsATOM( temp ) ) { // S-expressions only an ATOM

    if ( temp->mtype == QUOTE )
      strToken += "quote" ;
    else
      strToken += temp->mname ;
  } // if
  else if ( temp->mleft->mtype == QUOTE || t.IsATOM( temp->mleft ) ) {

    if ( temp->mright->mtype == NIL ) {
      if ( temp->mtype == QUOTE )
        strToken += "quote" ;
      else
        strToken += temp->mleft->mname ;
      
      strToken += ")" ;
    } // if
    else if ( temp->mright->mtype == QUOTE || t.IsATOM( temp->mright ) ) {

      if ( temp->mtype == QUOTE )
        strToken += "quote" ;
      else
        strToken += temp->mleft->mname ;
      
      strToken += "." ;

      if ( temp->mtype == QUOTE )
        strToken += "quote" ;
      else
        strToken += temp->mleft->mname ;
      
      strToken += ")" ;
    } // else if
    else {
      if ( temp->mtype == QUOTE )
        strToken += "quote" ;
      else
        strToken += temp->mleft->mname ;
      
      GetThisToken( temp->mright, strToken );

    } // else
  } // else if
  else {
    strToken += "(" ;
    
    temp->mleft->mtype = LEFT_PAREN; // !!!!!!!!!!!!!!!!!!!!!!!!! test !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    temp->mleft->mname = "(" ;

    GetThisToken( temp->mleft, strToken );

    if ( temp->mright->mtype == NIL ) {
      strToken += ")" ;
    } // if
    else if ( temp->mright->mtype == QUOTE || t.IsATOM( temp->mright ) ) {
      strToken += "." ;
      GetThisToken( temp->mright, strToken );
      strToken += ")" ;
    } // else if
    else 
      GetThisToken( temp->mright, strToken );
  } // else

} // GetThisToken()

Token * IsEqual( vector<Token *> args ) { // equal?
  /*
  ; equal?, on the other hand, is the usual notion of 
  ; equality comparison
  */
  // フ杠р┮Τ块讽string 璶场妓-> return #t else return nil

  Token t ;
  Token * firstToken = NULL; 
  Token * secondToken = NULL;
  Token * returnBoolNode = new Token() ;
  string firstStr = "", secondStr = "";
  

  if ( args.size() != 2 ) {
    throw "ERROR (incorrect number of arguments) : equal?\n";
  } // if
  else {
    // firstToken = EvaluateSExp( args.at( 0 ) ) ;
    // secondToken = EvaluateSExp( args.at( 1 ) ) ;

    // cout << "H: " << firstToken->mleft->mname << endl ;

    GetThisToken( EvaluateSExp( args.at( 0 ) ), firstStr ) ;
    // t.PrettyPrint( EvaluateSExp( args.at( 0 ) ) ) ;
    GetThisToken( EvaluateSExp( args.at( 1 ) ), secondStr ) ;


    if ( strcmp( firstStr.c_str(), secondStr.c_str() ) == 0 ) {
      returnBoolNode->mtype = T;
      returnBoolNode->mname = "#t" ;
    } // if
    else { // 
      returnBoolNode->mtype = NIL;
      returnBoolNode->mname = "nil" ;
    } // else 

    return returnBoolNode;

  } // else 

  return NULL;
} // IsEqual()

Token * Begin( vector<Token *> args ) {
  // return last element

  Token * lastElement; 
  if ( args.empty() ) {
    throw "ERROR (incorrect number of arguments) : begin\n";
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) 
      lastElement = EvaluateSExp( args.at( i ) ) ; // 穝evaluate 

  } // else

  return lastElement;

} // Begin()

Token * If( Token * root, vector<Token *> args ) {
  // (if #t 30 40 )
  // ( if condition true-returnValue false-returnValue)

  // (if #f 20) when args.size()==2 && condition == nil
  // ERROR (no return value) :

  // condition == #t 惠璶evaluate true block
  // condition == nil 惠璶evaluate false block

  Token t;
  Token * condition;

  if ( args.size() != 2 && args.size() != 3 ) {
    if ( DEBUG_PRINT_EVAL_P2 ) cout << "If args size: " <<  args.size() << endl ;
    throw "ERROR (incorrect number of arguments) : if\n";
  } // if
  else {
    condition = EvaluateSExp( args.at( 0 ) ) ;
      
    if ( condition->mtype == NIL ) { // false
      if ( args.size() == 2 ) { // 
        cout << "ERROR (no return value) : " ; // ?????????????????????????????????????????????????????????
        
        gM = 2 ;
        if ( !t.IsATOM( root ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( root ) ;
        } // if
        else  
          t.PrettyPrint( root ) ;
        
        throw "";
      } // if
      else {
        return EvaluateSExp( args.at( 2 ) ) ;
      } // else
    } // if
    else // true
      return EvaluateSExp( args.at( 1 ) ) ;

  } // else

  return NULL ;
} // If()


void CheckAndGetSubList( Token * root, vector<Token *> &vTemp ) {
  // 

  if ( strcmp( root->mname.c_str(), "(" ) != 0 )
    return ; // not list

  for ( Token * walk = root ; walk != NULL ; walk = walk->mright ) { 
    // walk  "." ênode
    if ( walk->mtype == NIL && walk->mright == NULL )
      return ; // 竒┏  磷耞Αerror
    
    else if ( walk->mright->mright == NULL && walk->mright->mtype != NIL ) { // non list 
      throw "ERROR (non-list) : ";
      // any ERRORs?????????????????????????
    } // if
    else if ( walk->mtype != NIL ) {
      vTemp.push_back( walk->mleft );
      // if ( DEBUG_PRINT_EVAL_P2 ) cout << "mname: " << walk->mleft->mname << endl ;
      // if ( DEBUG_PRINT_EVAL_P2 ) cout << "mtype: " << walk->mleft->mtype << endl ;
    } // else if 
    
  } // for


  DP( DEBUG, "GetList succed!!" );
} // CheckAndGetSubList()

Token * Cond( Token * root, vector<Token *> args ) {
  //  ; 'else' is a keyword (and not a reserve word) in OurScheme 
  // ; (or rather, Scheme) ; 
  // ; according to our textbook (by Sebesta), a keyword has a
  // ; special meaning ONLY WHEN it appears in some special contexts
  // ; (translation: when the word appears in contexts that are not
  // ;               special, the word is just an "ordinary word")
  // ;
  // ; 'else' has a special meaning only when it appear as the first
  // ; element of the last condition of 'cond' ;
  // ; in all other cases, 'else' is considered a normal symbol
  // 

  
  // (cond ((> 3 b) 'bad)
  //       ((> b 3) 'good)
  //       (else "What happened?") ; this 'else' has a special meaning ;
  //                              ; it means "in all other cases" here
  
  /*
  ( cond
    ( ( >
        3
        b
      )
      ( quote
        bad
      )
    )
    ( ( >
        b
        3
      )
      ( quote
        good
      )
    )
    ( else
      "What happened?"
    )
  )
  */

 
  // if()
  // else if()
  // else if()
  // (else)

  // output: good

  vector< vector<Token *> > argsList;
  
  Token t;
  Token * result = new Token() ;


  if ( args.empty() ) {
    cout << "ERROR (COND format) : " ;
    gM = 2 ;
    if ( !t.IsATOM( root ) ) { // S-expressions not an ATOM
      cout << "( ";
      t.PrettyPrint( root ) ;
    } // if
    else  
      t.PrettyPrint( root ) ;

    throw "" ;
  } // if
  else {
    for ( int i = 0 ; i < args.size() ; i++ ) {
      vector<Token *> tempArgs ; //  eg.  ((> 3 b) 'bad) い (> 3 b)'bad

      if ( DEBUG_PRINT_EVAL_P2 ) cout << " before size: " << tempArgs.size()  ;
      CheckAndGetSubList( args.at( i ), tempArgs ) ; 
      argsList.push_back( tempArgs ) ;
      
      if ( DEBUG_PRINT_EVAL_P2 ) cout << "      after size: " << tempArgs.size() << endl ;

      /*
      for ( int j = 0 ; j < tempArgs.size() ; j++ ) {
        // if ( strcmp( tempArgs.at( j )->mname.c_str(), "(" ) != 0 )
        if ( tempArgs.at( j )->mtype != LEFT_PAREN )
          throw "ERROR (COND format) : \n"; // ???????????????????????????????????????????

      } // for
      */

      
      if ( tempArgs.size() == 0 || tempArgs.size() == 1 ) { // ?????????????????????????????????????????????
        if ( DEBUG_PRINT_EVAL_P2 ) cout << "size: " << tempArgs.size() << endl ;

        cout << "ERROR (COND format) : " ;
        gM = 2 ;
        if ( !t.IsATOM( root ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( root ) ;
        } // if
        else  
          t.PrettyPrint( root ) ;

        throw "" ;
        // throw "ERROR (COND format) : \n"; // ???????????????????????????????????????????
      } // if
      
      

    } // for

    // (cond ((> 3 b) 'bad)
    //       ((> b 3) 'good)
    //       (else "What happened?") ; this 'else' has a special meaning ;
    //
    // ArgsList.at( 0 ).at( 0 ) ( tempArgs[0] ) == condition  |  (> 3 b)
    // ArgsList.at( 0 ).at( 1 ) ( tempArgs[1] ) == value      |  'bad

    // ArgsList.at( 1 ).at( 0 ) ( tempArgs[0] ) == condition  |  (> b 3)
    // ArgsList.at( 1 ).at( 1 ) ( tempArgs[1] ) == value      |  'good

    // ArgsList.at( 2 ).at( 0 ) ( tempArgs[0] ) == condition  |  else
    // ArgsList.at( 2 ).at( 1 ) ( tempArgs[1] ) == value      |  "What happened?"

    // BUT??? tempArgs[1] might have more so return last one 

    /*
    if( DEBUG_PRINT_EVAL_P2 ) cout << "1st arg: " << endl ; 
    for( int j = 0 ; j < tempArgs.size() ; j++ ) {
      // t.PrettyPrint( tempArgs.at( j ) );
      cout <<  tempArgs.at( 1 )->mright->mleft->mname  << endl ;
      
    } // for
    */
    int x;
    for ( int i = 0 ; i < argsList.size() ; i++ ) {
      if ( strcmp( args.at( i )->mname.c_str(), "(" ) != 0 ) {
        cout << "ERROR (COND format) : " ;
        gM = 2 ;
        if ( !t.IsATOM( root ) ) { // S-expressions not an ATOM
          cout << "( ";
          t.PrettyPrint( root ) ;
        } // if
        else  
          t.PrettyPrint( root ) ;

        throw "" ;

      } // if




      if ( i == argsList.size() - 1  ) { // 程 condition is else ( else ぃ材)->ヘ玡⊿耞硂
        
        // if ( IsAlreadyDefined( "else", x ) ) ;
        /*
        else {
          try {
            argsList.at( i ).at( 0 ) = EvaluateSExp( argsList.at( i ).at( 0 )  ) ;   
          } // try
          catch ( const char* str ) {
            if ( strcmp( str, "ERROR (unbound symbol) : else" ) ) 
              return EvaluateSExp( argsList.at( i ).at( argsList.at( i ).size() - 1 ) ) ; 
              // return value ( tempArgs[last] )
              // 秈êcondition block  evaluate
            else  
              throw;
          } // catch
        } // else
        */
        // else { // else 临⊿砆define

        if ( strcmp( argsList.at( i ).at( 0 )->mname.c_str(), "else" ) == 0 ) {
          return EvaluateSExp( argsList.at( i ).at( argsList.at( i ).size() - 1 ) ) ; 
          // return value ( tempArgs[last] )
        } // if
        else
          argsList.at( i ).at( 0 ) = EvaluateSExp( argsList.at( i ).at( 0 )  ) ;   
          // 狦程condition琌else 碞ぃノevaluate

        if ( argsList.at( i ).at( 0 )->mtype != NIL )
          return EvaluateSExp( argsList.at( i ).at( argsList.at( i ).size() - 1 ) ) ; 
        else {
          cout << "ERROR (no return value) : "; // ???????????????????????????????????????????;
          gM = 2 ;
          if ( !t.IsATOM( root ) ) { // S-expressions not an ATOM
            cout << "( ";
            t.PrettyPrint( root ) ;
          } // if
          else  
            t.PrettyPrint( root ) ;
        
          throw "";
          
        } // else
        // } // else

      } // if
      else {
        argsList.at( i ).at( 0 ) = EvaluateSExp( argsList.at( i ).at( 0 )  ) ;  
              
        
        



        /*
        if ( i == argsList.size() - 1  ) { // 程 condition is else    ( else ぃ材)->ヘ玡⊿耞硂

          // cout << "HEHE"  <<  endl ;
          // cout << argsList.at( i ).at( 0 )->mname << endl ;
          if ( strcmp( argsList.at( i ).at( 0 )->mname.c_str(), "else" ) == 0 ) 
            return EvaluateSExp( argsList.at( i ).at( argsList.at( i ).size() - 1 ) ) ; 
            // return value ( tempArgs[last] )
          else if ( argsList.at( i ).at( 0 )->mtype != NIL )
            return EvaluateSExp( argsList.at( i ).at( argsList.at( i ).size() - 1 ) ) ; 
            // return value ( tempArgs[last] )
          
        } // if
        */

        if ( DEBUG_PRINT_EVAL_P2 ) 
          cout << "i: " << i << " type: " << argsList.at( i ).at( 0 )->mtype << endl ;


        if ( argsList.at( i ).at( 0 )->mtype != NIL ) { // true // condition ( tempArgs[0] )
          // also need to evaluate all 把计
          for ( int cc = 1 ;  cc <  argsList.at( i ).size() ; cc++ )
            argsList.at( i ).at( cc ) = EvaluateSExp( argsList.at( i ).at( cc )  ) ;  
          
          return argsList.at( i ).at( argsList.at( i ).size() - 1 ) ; 

        } // if

        // return EvaluateSExp(  argsList.at( i ).at( argsList.at( i ).size() - 1 ) ); 
      
        // return value ( tempArgs[last] )
        // 秈êcondition block  evaluate

      } // else

    } // for 

    // 常⊿Τ return 
    

  } // else

  return NULL ;
} // Cond()

Token * CleanEnvironment( vector<Token *> args ) {
  if ( gLevel != 1 ) {
    // ERROR 
    throw "ERROR (level of CLEAN-ENVIRONMENT)\n";
  } // if

  if ( !args.empty() ) {
    cout << "ERROR: arg size!0" << endl ; // ERROR
  } // if
  else {
    gSymbolTable.clear() ;
    cout << "environment cleaned" << endl ;
  } // else

  return NULL ;

} // CleanEnvironment()