// SeeInfoParser.java

package parserP;

import java.util.*;
import genericP.*;
import objectP.*;
import stokenizerP.*;

/********************************************************/
/*         SeeInfoParser  $B;k3P>pJs$N2r@O(B                */
/********************************************************/

public class SeeInfoParser {
  String seeInfo;

  // $BK\Ev$O%5%$%I$N>pJs$O;}$?$;$?$/$J$$(B
  // $B$G$b$3$&$7$J$$$H(B InfoManager $B$,HQ;($K$J$k$N$G!D(B Flag $B$G$M(B
  String side;

  int time;

  int playerCount;  // $B%W%l%$%d?t%+%&%s%?(B
  int goalCount; // $B%4!<%k?t%+%&%s%?(B
  int flagCount; // $B%U%i%0?t%+%&%s%?(B
  int lineCount; // $B%i%$%s?t%+%&%s%?(B
    
  PlayerInfo[] players = new PlayerInfo[21];
  BallInfo ball = new BallInfo();
  GoalInfo[] goals = new GoalInfo[2];
  FlagInfo[] flags = new FlagInfo[3];
  LineInfo[] lines = new LineInfo[4];

  double[] flagdist = new double[3]; // $B5wN%$,6a$$=g$K(B 3 $B$D!%$b$&>/$7$$$$J}K!$O$J$$$+$J(B

/***************************************************************************/

  public SeeInfoParser(){

    /*
    if(Generic.DEBUG){
      System.out.println("Creating BodyInfoParser.");
    }
    */

  }

/***************************************************************************/
  // String si : $B;k3P>pJs(B
  // String s  : $B%5%$%I(B
  public SeeInfoParser(String si,String s){

    /*
    if(Generic.DEBUG){
      System.out.println("Creating BodyInfoParser.");
    }
    */

    setInfo(si);
    setSide(s);

    parseInfo();
  }

/***************************************************************************/

  public void setInfo(String si){
    seeInfo = si;
  }

/***************************************************************************/

  public void setSide(String s){
    side = s;
  }

/***************************************************************************/

  public void parseInfo(){
    // cleanAll();  //  $BA]=|$rK:$l$J$$(B!
    ball = null;    

    this.playerCount = 0;
    this.goalCount = 0;
    this.flagCount = 0;
    this.lineCount = 0;

    STokenizer tokenizer = new STokenizer(seeInfo);
    // $B@Z$j=P$7$?%H!<%/%s$rF~$l$kH"!#;H$$2s$9$3$HB?$7(B
    String token;

    token = tokenizer.nextSToken();

    flagdist[0] = 160;
    flagdist[1] = 160;
    flagdist[2] = 160;
    flags[0] = null;
    flags[1] = null;
    flags[2] = null;

    for(int i = 0; i < 3; i++)                      // $B%@%_!<%U%i%0=i4|2=(B
      this.flags[i] = new FlagInfo(1000,0);


    token = tokenizer.nextSToken();
    time = Integer.valueOf(token).intValue();

    while(tokenizer.hasMoreSTokens()){
      token = tokenizer.nextSToken();

      if(token.startsWith("((p") || 
	 token.startsWith("((P")){
        players[playerCount] = parsePlayerInfo(token);
        playerCount++;
      }
      else if(token.startsWith("((f") || 
	      token.startsWith("((F")){
	parseFlagInfo(token,side);
      }
      else if(token.startsWith("((g") || 
	      token.startsWith("((G")){
        goals[goalCount] = parseGoalInfo(token,side);
        goalCount++;
      }
      else if(token.startsWith("((l") || 
	      token.startsWith("((L")){
        lines[lineCount] = parseLineInfo(token);
        lineCount++;
      }
     else if(token.startsWith("((b") || 
             token.startsWith("((B")){
        ball = parseBallInfo(token);
        ball.setTime(time);  // $B;~4V>pJs$r$D$1$F$*$/(B
     }
    }
  }

/***************************************************************************/

  PlayerInfo parsePlayerInfo(String info){
    String token;
    String teamname;
    int uniformNum = 0;
    double dist, dir, distChange, dirChange, bodyFaceDir, headFaceDir;
    teamname = "";
    PlayerInfo playerInfo;

    STokenizer tokenizer = new STokenizer(info);
    token = tokenizer.nextSToken(); 
    STokenizer typeTokenizer = new STokenizer(token);
    token = typeTokenizer.nextSToken();

    /*
    if(Generic.DEBUG){
      if(!token.equals("player") && !token.equals("Player")){
        System.out.println("SeeInfoParser:2:parsePlayerInfo()Illegal Format!");
        return null;
      }
    }
    */

    if(typeTokenizer.hasMoreSTokens()){
      teamname = typeTokenizer.nextSToken();

      // ver6.00$BMQ(B ver6.00$B$+$i$O%A!<%`L>$,(B"teamname"$B$GAw$i$l$F$/$k$N$G!#(B
      if(teamname.startsWith("\"")) 
        teamname = teamname.substring(1,teamname.length()-1);

      if(typeTokenizer.hasMoreSTokens())
        uniformNum = Integer.valueOf(typeTokenizer.nextSToken()).intValue();
    }
    
    dist = Double.valueOf(tokenizer.nextSToken()).doubleValue();
    dir  = Double.valueOf(tokenizer.nextSToken()).doubleValue();

    if(tokenizer.hasMoreSTokens()){
      distChange = Double.valueOf(tokenizer.nextSToken()).doubleValue();
      dirChange = Double.valueOf(tokenizer.nextSToken()).doubleValue();

      if(tokenizer.hasMoreSTokens()){
        bodyFaceDir = Double.valueOf(tokenizer.nextSToken()).doubleValue();
        headFaceDir = Double.valueOf(tokenizer.nextSToken()).doubleValue();

        playerInfo = new PlayerInfo(dist,dir,distChange,
                                    dirChange,bodyFaceDir,headFaceDir);
      } else {
          playerInfo = new PlayerInfo(dist,dir,distChange,dirChange);
      }
    } else {
        playerInfo = new PlayerInfo(dist,dir);
    }

    playerInfo.setTeamname(teamname);
    playerInfo.setUniformNum(uniformNum);

    // $B1~5^A<CV(B
    // playerInfo.setTime(time);
    // playerInfo.setControlTime(time);

    return playerInfo;
  }

/***************************************************************************/
// $B%U%i%0>pJs$N2r@O(B  $B$+$J$j8+$K$/$$(B
  void parseFlagInfo(String info,String side){
    String token;

    double dist,dir;
    STokenizer tokenizer = new STokenizer(info);
    token = tokenizer.nextSToken();
    STokenizer typeTokenizer = new STokenizer(token);
    token = typeTokenizer.nextSToken();

    // FlagInfo flagInfo;

    int count = 2;                         // Flag$B%*%V%8%'%/%HHf3SMQ%+%&%s%?(B

    dist = Double.valueOf(tokenizer.nextSToken()).doubleValue();

    // $B$&$^$/$$$+$J$$!#(B
    // Flag$B$O<+J,$+$i6a$$(B3$BK\<j$K$O$$$l$PNI$$!#$/$I$$$+$J$!!#(B
    if(dist > flags[count].getDist()){
      // $B$b$78=:_J];}$7$F$$$k(BFlag$B$h$j1s$+$C$?$i(B,$BJ];}$7$J$$(B
      return;
    }
    else{ 
      // 3$BK\L\$h$j6a$$(B -> 3$BHVL\$K%*%V%8%'%/%H@8@.(B 
      if(dist < flags[count -1].getDist()){
        // 2$BK\L\$h$j6a$$(B -> 2$BHVL\$N%*%V%8%'%/%H$r%7%U%H(B,2$BHVL\$K@8@.(B
	flags[count] = flags[--count];
        if(dist < flags[count -1].getDist()){
          // 1$BK\L\$h$j6a$$(B -> 1$BHVL\$N%*%V%8%'%/%H$r%7%U%H(B,1$BHVL\$K@8@.(B
	  flags[count] = flags[--count];
	}
      }
    }

    if(flagCount < 3)
      flagCount++;

    dir  = Double.valueOf(tokenizer.nextSToken()).doubleValue();

    token = typeTokenizer.nextSToken();

    flags[count] = null;

    if(token.equals("c")){
      if(typeTokenizer.hasMoreSTokens()){
        token = typeTokenizer.nextSToken();
        flags[count] = new FlagInfo(dist,dir,"c",token,side);
      }
      else {
        flags[count] = new FlagInfo(dist,dir);
      }
    }
    else if(token.equals("p") || token.equals("g")){
      String loc1,loc2;

      loc1 = typeTokenizer.nextSToken();
      loc2 = typeTokenizer.nextSToken();

      flags[count] = new FlagInfo(dist,dir,token,loc1,loc2,side);
    }
    else if(token.equals("l") || token.equals("r")){
      String loc1,loc2;
      loc1 = token;
      loc2 = typeTokenizer.nextSToken();

      if(loc2.equals("0")){
        flags[count] = new FlagInfo(dist,dir,loc1,loc2,side);
      }
      else if(typeTokenizer.hasMoreSTokens()){
        int pitch = Integer.valueOf(typeTokenizer.nextSToken()).intValue();
        flags[count] = new FlagInfo(dist,dir,loc1,loc2,pitch,side);
      }
      else {
        flags[count] = new FlagInfo(dist,dir,loc1,loc2,side);
      }
    }
    else if(token.equals("t") || token.equals("b")){
      String loc1,loc2;
      loc1 = token;
      loc2 = typeTokenizer.nextSToken();

      if(loc2.equals("0")){
        flags[count] = new FlagInfo(dist,dir,loc1,side);
      }
      else {
        int pitch = Integer.valueOf(typeTokenizer.nextSToken()).intValue();
        flags[count] = new FlagInfo(dist,dir,loc1,loc2,pitch,side);
      }
    } 
    else {
      flags[count] = new FlagInfo(dist,dir);
    }
  }

/***************************************************************************/
// $B%4!<%k>pJs$N2r@O(B
  GoalInfo parseGoalInfo(String info,String side){
    String token;
    GoalInfo goalInfo;

    double dist,dir;
    STokenizer tokenizer = new STokenizer(info);
    token = tokenizer.nextSToken();
    STokenizer typeTokenizer = new STokenizer(token);
    token = typeTokenizer.nextSToken();

    /*
    if(Generic.DEBUG) {
      if(!token.equals("goal")){
        System.out.println("SeeInfoParser:4:parseGoalInfo() Illegal Format!");
        return null;
      }
    }
    */

    dist = Double.valueOf(tokenizer.nextSToken()).doubleValue();
    dir  = Double.valueOf(tokenizer.nextSToken()).doubleValue();

    token = typeTokenizer.nextSToken();

    goalInfo = new GoalInfo(dist,dir,token,side);

    return goalInfo;
  }

/***************************************************************************/
// $B%i%$%s>pJs$N2r@O(B
  LineInfo parseLineInfo(String info){
    String token;
    LineInfo lineInfo;

    double dist,dir;
    STokenizer tokenizer = new STokenizer(info);
    token = tokenizer.nextSToken();
    STokenizer typeTokenizer = new STokenizer(token);
    token = typeTokenizer.nextSToken();

    /*
    if(Generic.DEBUG) {
      if(!token.equals("line")){
        System.out.println("token? : " + token);
        System.out.println("SeeInfoParser:5:parseLineInfo() Illegal Format!");
        return null;
      }
    }
    */

    dist = Double.valueOf(tokenizer.nextSToken()).doubleValue();
    dir  = Double.valueOf(tokenizer.nextSToken()).doubleValue();

    token = typeTokenizer.nextSToken();

    lineInfo = new LineInfo(dist,dir,token);

    return lineInfo;

  }

/***************************************************************************/
// $B%\!<%k>pJs$N2r@O(B
  BallInfo parseBallInfo(String info){
    String token;
    BallInfo ballInfo;

    double dist,dir,distChange,dirChange;
    STokenizer tokenizer = new STokenizer(info);
    token = tokenizer.nextSToken();
    STokenizer typeTokenizer = new STokenizer(token);
    token = typeTokenizer.nextSToken();

    /*
    if(Generic.DEBUG){
      if(!token.equals("ball") && !token.equals("Ball")){
        System.out.println("SeeBallParser:6:parseLineInfo() Illegal Format!");
        return null;
      }
    }
    */

    dist = Double.valueOf(tokenizer.nextSToken()).doubleValue();
    dir  = Double.valueOf(tokenizer.nextSToken()).doubleValue();

    if(tokenizer.hasMoreSTokens()){
      distChange = Double.valueOf(tokenizer.nextSToken()).doubleValue();
      dirChange  = Double.valueOf(tokenizer.nextSToken()).doubleValue();

      ballInfo = new BallInfo(dist,dir,distChange,dirChange);
    } else {
        ballInfo = new BallInfo(dist,dir);
    }

    // $B1~5^A<CV(B
    // ballInfo.setTime(time);
    // ballInfo.setControlTime(time);

    return ballInfo;
  }
/***************************************************************************/

  public int getTime(){
    return this.time;
  }

/***************************************************************************/

  public int getPlayerNum(){
    return this.playerCount;
  }

/***************************************************************************/

  public int getGoalNum(){
    return this.goalCount;
  }

/***************************************************************************/

  public int getFlagNum(){
    return this.flagCount;
  }

/***************************************************************************/

  public int getLineNum(){
    return this.lineCount;
  }

/***************************************************************************/

  public boolean hasBallInfo(){
    if(ball != null)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }

/***************************************************************************/

    // n$BHVL\$N$=$l$>$l$N%*%V%8%'%/%H$rJV$9(B
    // $BFs=E(B delete $B$,I]$$$N$G%$%s%9%?%s%9$rJV$7$F$^$9(B
    // Info $B7O$N%a%s%PJQ?t$K$O%]%$%s%?;H$C$F$J$$$N$GB?J,$3$l$G(B OK
    // $B30B&$G%]%$%s%?$K$$$l$?>l9g$K$O$-$A$s$H(B delete $B$9$k$3$H(B
    // $B$d$C$Q%,%Y!<%8%3%l%/%7%g%s$,L5$$8@8l$O%*%V%8%'%/%H$r(B delete $B$9$k(B
    // $B>l=j$r>o$K0U<1$7$D$D=q$+$M$P(B
  public PlayerInfo getPlayerInfo(int n){
    return this.players[n];
    // $BCMEO$7$G$"$k$3$H$KCm0U(B
    // return $B;~$K%$%s%9%?%s%9$,J#@=$5$l$F$^$9(B
  }

/***************************************************************************/

  public GoalInfo getGoalInfo(int n){
    return this.goals[n];
    // $BCMEO$7$G$"$k$3$H$KCm0U(B
    // return $B;~$K%$%s%9%?%s%9$,J#@=$5$l$F$^$9(B
  }

/***************************************************************************/

  FlagInfo getFlagInfo(int n){
    return this.flags[n];
    // $BCMEO$7$G$"$k$3$H$KCm0U(B
    // return $B;~$K%$%s%9%?%s%9$,J#@=$5$l$F$^$9(B
  }

/***************************************************************************/

  public LineInfo getLineInfo(int n){
    return this.lines[n];
    // $BCMEO$7$G$"$k$3$H$KCm0U(B
    // return $B;~$K%$%s%9%?%s%9$,J#@=$5$l$F$^$9(B
  }

/***************************************************************************/

  public BallInfo getBallInfo(){
    return this.ball;
    // $BCMEO$7$G$"$k$3$H$KCm0U(B
    // return $B;~$K%$%s%9%?%s%9$,J#@=$5$l$F$^$9(B
  }

/***************************************************************************/

    // $B%*%V%8%'%/%H$r3JG<$7$F$$$kG[Ns$=$N$b$N$X$N%]%$%s%?$rJV$9(B
    // $B@dBP$K30B&$G(B delete $B$7$J$$$3$H(B
    // $B$3$$$D$r(B delete $B$9$k$N$OLLE]$J$N$GIaDL$7$J$$$H;W$&$,!D(B
    // $B$3$NFs<oN`$O?t$,B?$$$?$a$K4X?t8F$S=P$7$N%*!<%P%X%C%I$r(B
    // $B5$$K$7$FFCJL$KMQ0U$7$?$@$1(B
  public PlayerInfo[] getPlayers(){
    return this.players;
  }

/***************************************************************************/

  public FlagInfo[] getFlags(){
    return this.flags;
  }
 
}









