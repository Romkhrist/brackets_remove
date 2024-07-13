#include <iostream>
#include <fstream>

int main(int argc, char*argv[])
{
     std::ifstream fin;
     std::ofstream fout;
     
     enum class state
     {
          DEFAULT,             // Basic state
          DEFAULT_SLASH,       // State after '/' in basic state
          SLASHSLASH,          // Such comment as "//this is a C++-style comment"
          SLASHSTAR,           // Such comment as "/*this is a C-style comment*/"
          SLASHSTAR_STAR,      // State after '*' in SLASH_STAR state   
          SINGLEQUOTES,        // State after "'" in DEFAULT state
          SINGLEQUOTES_SLASH,  // State after "/" in SINGLEQUOTES state
          DOUBLEQUOTES,        // State after '"' in DEFAULT state
          BRACKETS,            // State after "["
          PARENTHESES,         // State after "("
     };
          
     if(argc == 1)
     {
          printf(" ->ERROR: input file wasn't specified!\n");
          return 1;
     }
     else if(argc == 2)
     {
          std::cout << " ->ERROR: output file wasn't specified!\n";
          return 1;
     }
     else if(argc > 3)
     {
          std::cout << " ->ERROR: too many parameters!\n";
          return 1;
     }
     
     fin.open(argv[1], std::ios_base::in);
     if(!fin)
     {
         std::cout << " ->ERROR: input file doesn't exist!\n";
          return 1;
     }
     
     fout.open(argv[2]);
     if(!fout)
     {
         std::cout << " ->ERROR: output file doesn't exist!\n";
          fin.close();
          return 1;
     }
     
     state s = state::DEFAULT;
     
     while(true)
     {
          char c;
          fin.get(c);
          
          if(fin.eof())
          {
              break;
          }
         
          switch(s)
          {
               case state::DEFAULT:
                   switch(c)
                   {
                       case '/':
                           s = state::DEFAULT_SLASH;
                           //fout.put(c);
                           break;
                       case '\'':
                           s = state::SINGLEQUOTES;
                           fout.put(c);
                           break; 
                       case '"':
                           s = state::DOUBLEQUOTES;
                           fout.put(c);
                           break;
                       case '[':
                           s = state::BRACKETS;
                           fout.put(c);
                           break;
                       case '(':
                           s = state::PARENTHESES;
                           fout.put(c);
                           break;
                       case ')':
                       case ']':
                           break;
                       default:
                           fout.put(c);
                   }      
                   break;
               case state::DEFAULT_SLASH:
                   switch(c)
                   {
                       case '/':
                           s = state::SLASHSLASH;
                       default:
                           fout.put(c);
                   }
            case state::SLASHSLASH:
                  switch(c)
                  {
                      case '\n':
                          s = state::DEFAULT;
                      default:
                          fout.put(c);
                  }
                  break;
              case state::SLASHSTAR:
                  switch(c)
                  {
                      case '*':
                          s = state::SLASHSTAR_STAR;
                      default:
                          fout.put(c);
                  }
                  break;
              case state::SLASHSTAR_STAR:
                  switch(c)
                  {
                      case '/':
                          s = state::DEFAULT;
                      default:
                          fout.put(c);
                  }
                  break;
              case state::SINGLEQUOTES:
                  switch(c)
                  {
                      case '\'':
                      case '\n':
                          s = state::DEFAULT;
                          fout.put(c);
                          break;
                      case '/':
                          s = state::SINGLEQUOTES_SLASH;
                          break;
                      default:
                          fout.put(c);
                  }
                  break;
              case state::SINGLEQUOTES_SLASH:
                  switch(c)
                  {
                      case '\'':
                      case '\n':
                          s = state::DEFAULT;
                          fout.put('/');
                          fout.put(c);
                          break;
                      case '/':
                          s = state::SLASHSLASH;
                          break;
                      case '*':
                          s = state::SLASHSTAR;
                          break;
                      default:
                          fout.put('/');
                          fout.put(c);
                  }
                  break;                 
              case state::DOUBLEQUOTES:
                  switch(c)
                  {
                      case '"':
                      case '\n':
                          s = state::DEFAULT;
                          fout.put(c);
                          break;
                      default:
                          fout.put(c);
                  }
                  break;
              case state::BRACKETS:
                  switch(c)
                  {
                      case ']':
                          s = state::DEFAULT;
                          fout.put(c);
                          break;
                  }
                  break;
              case state::PARENTHESES:
                  switch(c)
                  {
                      case ')':
                          s = state::DEFAULT;
                          fout.put(c);
                          break;
                  }
          }     
     }
     
     fin.close();
     fout.close();
     return 0;
}
