// STokenizer.java

package stokenizerP;

import java.util.*;

/********************************************************/
/*         Stokenizer  S式をトークンに分解              */
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
    // 最初の'('をはずす
    if((this.s[index] == "(") || (this.s[index] == " ")){
      this.s[index] = stz.nextToken();
    };

    // トークンがなくなるまでループ
    while(stz.hasMoreTokens()){
      this.s[index] = stz.nextToken();
      // ネストがあったらindexをとめてStringBufferにトークンをためる
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
      if(flag){  // ネストが終了したらStringBufferの中身を出力
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
      index--;  // 最後の')'の検出用?
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
