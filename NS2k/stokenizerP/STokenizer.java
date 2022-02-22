// STokenizer.java

package stokenizerP;

import java.util.*;

/********************************************************/
/*         Stokenizer  S$B<0$r%H!<%/%s$KJ,2r(B              */
/********************************************************/

public class STokenizer {

  public static final int MAX_TOKEN_SIZE = 128;

  String[] s = new String[MAX_TOKEN_SIZE];
  int nest = 0;
  int end;
  int count = 0;

/***************************************************************************/

  public STokenizer(String inputString){
    StringTokenizer stz = new StringTokenizer(inputString,"( )",true);
    StringBuffer stb = new StringBuffer();
    boolean flag = false;
    int index = 0;

    this.s[index] = stz.nextToken();
    // $B:G=i$N(B'('$B$r$O$:$9(B
    if((this.s[index] == "(") || (this.s[index] == " ")){
      this.s[index] = stz.nextToken();
    };

    // $B%H!<%/%s$,$J$/$J$k$^$G%k!<%W(B
    while(stz.hasMoreTokens()){
      this.s[index] = stz.nextToken();
      // $B%M%9%H$,$"$C$?$i(Bindex$B$r$H$a$F(BStringBuffer$B$K%H!<%/%s$r$?$a$k(B
      while(nest > 0){
        flag = true;
        if(this.s[index].equals("("))
          nest++;
        else{
          if(this.s[index].equals(")"))
            nest--;
          else{
            if((this.s[index].equals(" ")) && (nest == 0))
              this.s[index] = stz.nextToken();
	  };
	};
        stb.append(this.s[index]);
        this.s[index] = stz.nextToken();
      };
      if(flag){  // $B%M%9%H$,=*N;$7$?$i(BStringBuffer$B$NCf?H$r=PNO(B
        flag = false;
        this.s[index] = stb.toString();
      };
      if(this.s[index].equals("(")){
        nest++;
        stb = new StringBuffer("(");
      }else{
         if(!this.s[index].equals(" "))
           index++;
      };
    };
    if(index != 0 && this.s[index -1].equals(")"))
      index--;  // $B:G8e$N(B')'$B$N8!=PMQ(B?
    this.end = index;
  }

/***************************************************************************/

  public String nextSToken(){  // nextToken
    return this.s[this.count++];
  }

/***************************************************************************/

  public boolean hasMoreSTokens(){  // hasMoreTokens
    if(this.count < this.end)
      return true;
    else
      return false;
  }
  
/***************************************************************************/

  public void printStokens(){
    int i;
    for(i = 0; i <= this.end; i++){
      System.out.println(">" + this.s[i]);
    };
  }

}
