// SeeInfoParser.java

package parserP;

import java.util.*;
import genericP.*;
import objectP.*;
import stokenizerP.*;

/********************************************************/
/*         SeeInfoParser  視覚情報の解析                */
/********************************************************/

public class SeeInfoParser {
  String seeInfo;

  // 本当はサイドの情報は持たせたくない
  // でもこうしないと InfoManager が煩雑になるので… Flag でね
  String side;

  int time;

  int playerCount;  // プレイヤ数カウンタ
  int goalCount; // ゴール数カウンタ
  int flagCount; // フラグ数カウンタ
  int lineCount; // ライン数カウンタ
    
  PlayerInfo[] players = new PlayerInfo[21];
  BallInfo ball = new BallInfo();
  GoalInfo[] goals = new GoalInfo[2];
  FlagInfo[] flags = new FlagInfo[3];
  LineInfo[] lines = new LineInfo[4];

  double[] flagdist = new double[3]; // 距離が近い順に 3 つ．もう少しいい方法はないかな

/***************************************************************************/

  public SeeInfoParser(){

    /*
    if(Generic.DEBUG){
      System.out.println("Creating BodyInfoParser.");
    }
    */

  }

/***************************************************************************/
  // String si : 視覚情報
  // String s  : サイド
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
    // cleanAll();  //  掃除を忘れない!
    ball = null;    

    this.playerCount = 0;
    this.goalCount = 0;
    this.flagCount = 0;
    this.lineCount = 0;

    STokenizer tokenizer = new STokenizer(seeInfo);
    // 切り出したトークンを入れる箱。使い回すこと多し
    String token;

    token = tokenizer.nextSToken();

    flagdist[0] = 160;
    flagdist[1] = 160;
    flagdist[2] = 160;
    flags[0] = null;
    flags[1] = null;
    flags[2] = null;

    for(int i = 0; i < 3; i++)                      // ダミーフラグ初期化
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
        ball.setTime(time);  // 時間情報をつけておく
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

      // ver6.00用 ver6.00からはチーム名が"teamname"で送られてくるので。
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

    // 応急措置
    // playerInfo.setTime(time);
    // playerInfo.setControlTime(time);

    return playerInfo;
  }

/***************************************************************************/
// フラグ情報の解析  かなり見にくい
  void parseFlagInfo(String info,String side){
    String token;

    double dist,dir;
    STokenizer tokenizer = new STokenizer(info);
    token = tokenizer.nextSToken();
    STokenizer typeTokenizer = new STokenizer(token);
    token = typeTokenizer.nextSToken();

    // FlagInfo flagInfo;

    int count = 2;                         // Flagオブジェクト比較用カウンタ

    dist = Double.valueOf(tokenizer.nextSToken()).doubleValue();

    // うまくいかない。
    // Flagは自分から近い3本手にはいれば良い。くどいかなぁ。
    if(dist > flags[count].getDist()){
      // もし現在保持しているFlagより遠かったら,保持しない
      return;
    }
    else{ 
      // 3本目より近い -> 3番目にオブジェクト生成 
      if(dist < flags[count -1].getDist()){
        // 2本目より近い -> 2番目のオブジェクトをシフト,2番目に生成
	flags[count] = flags[--count];
        if(dist < flags[count -1].getDist()){
          // 1本目より近い -> 1番目のオブジェクトをシフト,1番目に生成
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
// ゴール情報の解析
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
// ライン情報の解析
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
// ボール情報の解析
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

    // 応急措置
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

    // n番目のそれぞれのオブジェクトを返す
    // 二重 delete が怖いのでインスタンスを返してます
    // Info 系のメンバ変数にはポインタ使ってないので多分これで OK
    // 外側でポインタにいれた場合にはきちんと delete すること
    // やっぱガベージコレクションが無い言語はオブジェクトを delete する
    // 場所を常に意識しつつ書かねば
  public PlayerInfo getPlayerInfo(int n){
    return this.players[n];
    // 値渡しであることに注意
    // return 時にインスタンスが複製されてます
  }

/***************************************************************************/

  public GoalInfo getGoalInfo(int n){
    return this.goals[n];
    // 値渡しであることに注意
    // return 時にインスタンスが複製されてます
  }

/***************************************************************************/

  FlagInfo getFlagInfo(int n){
    return this.flags[n];
    // 値渡しであることに注意
    // return 時にインスタンスが複製されてます
  }

/***************************************************************************/

  public LineInfo getLineInfo(int n){
    return this.lines[n];
    // 値渡しであることに注意
    // return 時にインスタンスが複製されてます
  }

/***************************************************************************/

  public BallInfo getBallInfo(){
    return this.ball;
    // 値渡しであることに注意
    // return 時にインスタンスが複製されてます
  }

/***************************************************************************/

    // オブジェクトを格納している配列そのものへのポインタを返す
    // 絶対に外側で delete しないこと
    // こいつを delete するのは面倒なので普通しないと思うが…
    // この二種類は数が多いために関数呼び出しのオーバヘッドを
    // 気にして特別に用意しただけ
  public PlayerInfo[] getPlayers(){
    return this.players;
  }

/***************************************************************************/

  public FlagInfo[] getFlags(){
    return this.flags;
  }
 
}









