// PlayerTable.java

package info_managerP;

import java.util.*;
import genericP.*;
import objectP.*;
import stokenizerP.*;
import parserP.*;

/********************************************************/
/*         PlayerTable  プレイヤー情報の管理,加工       */
/********************************************************/

public class PlayerTable{
    int time,shadowsTime;
    
    //  現在の情報
    //  背番号が分かっている奴はIndex + 1 が背番号になるように
    //  分からない奴は順番につめる
    PlayerInfo[] teammatesWithUNum = new PlayerInfo[11];
    PlayerInfo[] opponentsWithUNum = new PlayerInfo[11];
    PlayerInfo[] teammates = new PlayerInfo[11];
    PlayerInfo[] opponents = new PlayerInfo[11];
    PlayerInfo[] unknownPlayers = new PlayerInfo[21];

    int teammateCount;
    int opponentCount;
    int unknownPlayerCount;

    //  過去の情報
    PlayerInfo[] teammateShadowsWithUNum = new PlayerInfo[11];
    PlayerInfo[] opponentShadowsWithUNum = new PlayerInfo[11];
    PlayerInfo[] teammateShadows = new PlayerInfo[11];
    PlayerInfo[] opponentShadows = new PlayerInfo[11];

    int teammateShadowCount;
    int opponentShadowCount;

/***************************************************************************/

  public PlayerTable(){

    formatAll();           // PlayerInfoテーブルの初期化 
    teammateCount = 0;
    opponentCount = 0;
    unknownPlayerCount = 0;

  }

/***************************************************************************/
  // そういえばこれ、何で uNum を引数にとってるの？
  // PlayerInfo から抜き出せばいいだけじゃないのか？
  public void setTeammateWithUNum(PlayerInfo player, int uNum){
    
    this.teammatesWithUNum[uNum - 1] = player;

    //teammateShadowsWithUNum[uNum - 1].setCertainty(0);
    // 残像の情報を使えなくしておく
    // 残像と実像との1対1関係を保つため
  }

/***************************************************************************/

  public void setOpponentWithUNum(PlayerInfo player, int uNum){
    this.opponentsWithUNum[uNum - 1] = player;

    //opponentShadowsWithUNum[uNum - 1].setCertainty(0);
    // 残像の情報を使えなくしておく
    // 残像と実像との1対1関係を保つため
  }

/***************************************************************************/

  public void addTeammate(PlayerInfo player){
    this.teammates[this.teammateCount] = player;
    this.teammateCount++;
  }

/***************************************************************************/

  public void addOpponent(PlayerInfo player){
    this.opponents[this.opponentCount] = player;
    this.opponentCount++;
  }

/***************************************************************************/

  public void addUnknownPlayer(PlayerInfo player){
    this.unknownPlayers[this.unknownPlayerCount] = player;
    this.unknownPlayerCount++;
  }

/***************************************************************************/

  // 残像を作る…現在の情報が過去の情報になるだけ
  public void makeShadows(){
    for(int i = 0; i < 11; i++){
      this.teammateShadowsWithUNum[i] = this.teammatesWithUNum[i];
      this.opponentShadowsWithUNum[i] = this.opponentsWithUNum[i];
      this.teammateShadows[i] = this.teammates[i];
      this.opponentShadows[i] = this.opponents[i];
    }

    this.teammateShadowCount = this.teammateCount;
    this.opponentShadowCount = this.opponentCount;
    
    this.teammateCount = 0;
    this.opponentCount = 0;
    this.unknownPlayerCount = 0;

    for(int i = 0; i < 11; i++){
      this.teammatesWithUNum[i].setCertainty(0);
      this.opponentsWithUNum[i].setCertainty(0);
      this.teammates[i].setCertainty(0);
      this.opponents[i].setCertainty(0);
    }

    this.shadowsTime = this.time;
  }

/***************************************************************************/

  public boolean hasTeammateInfo(int unum){
    if(this.teammatesWithUNum[unum - 1].getCertainty() == 1.0)
	return Generic.NS_TRUE;
    else
	return Generic.NS_FALSE;
  }

/***************************************************************************/

  public double getTeammateX(int unum){
    return this.teammatesWithUNum[unum - 1].getX();
  }

/***************************************************************************/

  public double getTeammateCertainty(int unum){
    return this.teammatesWithUNum[unum - 1].getCertainty();
  }

/***************************************************************************/

  public void classifyUnknownPlayers(int blinkTime,double X,double Y,
				double headFaceDir,String angleWidth){
    boolean[] teammatesWithUNumCandFlag = new boolean[11];
    boolean[] opponentsWithUNumCandFlag = new boolean[11];
    boolean[] teammatesCandFlag = new boolean[11];
    boolean[] opponentsCandFlag = new boolean[11];
    boolean[] teammateShadowsWithUNumCandFlag = new boolean[11];
    boolean[] opponentShadowsWithUNumCandFlag = new boolean[11];
    boolean[] teammateShadowsCandFlag = new boolean[11];
    boolean[] opponentShadowsCandFlag = new boolean[11];

    double margin = blinkTime * 1.1;

    initCandFlag(teammatesWithUNumCandFlag,this.teammatesWithUNum,11);
    initCandFlag(opponentsWithUNumCandFlag,this.opponentsWithUNum,11);
    initCandFlag(teammateShadowsWithUNumCandFlag,
                 this.teammateShadowsWithUNum,11);
    initCandFlag(opponentShadowsWithUNumCandFlag,
                 this.opponentShadowsWithUNum,11);
    initCandFlag(teammatesCandFlag,this.teammates,this.teammateCount);
    initCandFlag(opponentsCandFlag,this.opponents,this.opponentCount);
    initCandFlag(teammateShadowsCandFlag,
                 this.teammateShadows,this.teammateShadowCount);
    initCandFlag(opponentShadowsCandFlag,
                 this.opponentShadows,this.opponentShadowCount);

    for(int i = 0; i < 11; i++){
      // 残像に実像と完全に一致するものがあれば両者を候補から外す
      if(teammatesWithUNumCandFlag[i] && 
         teammateShadowsWithUNumCandFlag[i]){
        teammatesWithUNumCandFlag[i] = Generic.NS_FALSE;
        teammateShadowsWithUNumCandFlag[i] = Generic.NS_FALSE;
      }
      if(opponentsWithUNumCandFlag[i] &&
         opponentShadowsWithUNumCandFlag[i]){
        opponentsWithUNumCandFlag[i] = Generic.NS_FALSE;
        opponentShadowsWithUNumCandFlag[i] = Generic.NS_FALSE;
      }
    }

    // 重い場合には外してもOK 今回ははずす。
    // **ここから** 
    /*
    // teammates と teammateShadowsWithUNum を比較
    comparePlayerWithShadow(margin,teammatesCandFlag,this.teammates,
                            this.teammateCount,teammateShadowsWithUNumCandFlag,
			    this.teammateShadowsWithUNum,11);

    // opponents と opponentShadowsWithUNum を比較
    comparePlayerWithShadow(margin,opponentsCandFlag,this.opponents,
                            this.opponentCount,opponentShadowsWithUNumCandFlag,
			    this.opponentShadowsWithUNum,11);


    // teammates と teammatesShadow を比較
    comparePlayerWithShadow(margin,teammatesCandFlag,this.teammates,
                            this.teammateCount,teammateShadowsCandFlag,
                            this.teammateShadows,this.teammateShadowCount);

    // opponents と opponentsShadow を比較
    comparePlayerWithShadow(margin,opponentsCandFlag,this.opponents,
                            this.opponentCount,opponentShadowsCandFlag,
                            this.opponentShadows,this.opponentShadowCount);

    // teammatesWithUNum と teammateShadows を比較
    comparePlayerWithShadow(margin,teammatesWithUNumCandFlag,
                            this.teammatesWithUNum,11,teammateShadowsCandFlag,
                            this.teammateShadows,this.teammateShadowCount);

    // opponentsWithUNum と opponentsShadows を比較
    comparePlayerWithShadow(margin,opponentsWithUNumCandFlag,
                            this.opponentsWithUNum,11,opponentShadowsCandFlag,
                            this.opponentShadows,
			    this.opponentShadowCount);
    */
    // **ここまで**
    // そういえば addTeammate,addOpponent があった…後で直そう
    for(int i = 0; i < 11; i++){
      if(this.teammateCount < 11 && 
         teammateShadowsWithUNumCandFlag[i] &&
         !this.teammateShadowsWithUNum[i].isInSight(X,Y,
						    headFaceDir,angleWidth)){
        this.teammates[this.teammateCount] = this.teammateShadowsWithUNum[i];
	this.teammates[this.teammateCount].setCertainty(
                      this.teammates[this.teammateCount].getCertainty() - 0.1);
        this.teammateCount++;
      }
    }

    for(int i = 0; i < 11; i++){
      if(this.opponentCount < 11 && 
         opponentShadowsWithUNumCandFlag[i] &&
         !this.opponentShadowsWithUNum[i].isInSight(X,Y,
						    headFaceDir,angleWidth)){
        this.opponents[this.opponentCount] = this.opponentShadowsWithUNum[i];
	this.opponents[this.opponentCount].setCertainty(
                      this.opponents[this.opponentCount].getCertainty() - 0.1);
        this.opponentCount++;
      }
    }

    for(int i = 0; i < this.teammateShadowCount; i++){
      if(this.teammateCount < 11 && teammateShadowsCandFlag[i]
         && !this.teammateShadows[i].isInSight(X,Y,headFaceDir,angleWidth)){
        this.teammates[this.teammateCount] = this.teammateShadows[i];
        this.teammates[this.teammateCount].setCertainty(
                      this.teammates[this.teammateCount].getCertainty() - 0.1);
        this.teammateCount++;
      }
    }

    for(int i = 0; i < this.opponentShadowCount; i++){
      if(this.opponentCount < 11 && opponentShadowsCandFlag[i]
         && !this.opponentShadows[i].isInSight(X,Y,headFaceDir,angleWidth)){
        this.opponents[this.opponentCount] = this.opponentShadows[i];
        this.opponents[this.opponentCount].setCertainty(
                      this.opponents[this.opponentCount].getCertainty() - 0.1);
	this.opponentCount++;
      }
    }

  }
 
/***************************************************************************/

  public void initCandFlag(boolean[] flags,PlayerInfo[] players,int n){
    int i;

    for(i = 0; i < n; i++){
      if(players[i].getCertainty() > 0.1){
	flags[i] = Generic.NS_TRUE;
      }
      else {
	flags[i] = Generic.NS_FALSE;
      }
    }
  }

/***************************************************************************/

  public void comparePlayerWithShadow(double margin,
				 boolean[] playerFlags,PlayerInfo[] players,
				 int playerNum,
				 boolean[] shadowFlags,PlayerInfo[] shadows,
				 int shadowNum){
    for(int i = 0; i < shadowNum; i++){
      if(shadowFlags[i]){
        int index = -1;
        double minDist = 1000;
        for(int j = 0; j < playerNum; j++){
           if(playerFlags[j]){
	     double dist = Generic.DIST(shadows[i].getX(),shadows[i].getY(),
	        		        players[j].getX(),players[j].getY());
	     if(dist < margin && dist < minDist){
	       minDist = dist;
	       index = j;
	     }
	   }
	}
	if(index >= 0){
	  shadowFlags[i] = Generic.NS_FALSE;
	  playerFlags[i] = Generic.NS_FALSE;
	}
      }
    }
  }

/***************************************************************************/

  public double getOffsideLine(double ballX){
    double offsideLine = 0;
    double depth1 = 0;
    double depth2 = 0;

    for(int i = 0; i < 11; i++){
      if(this.opponentsWithUNum[i].getCertainty() > 0.1){  // certainty != 0
        double depth = this.opponentsWithUNum[i].getX();
        if(depth1 < depth){
      	  depth2 = depth1;
	  depth1 = depth;
	}
	else if(depth2 < depth){
	  depth2 = depth;
	}
      }
    }
    
    for(int i = 0; i < this.opponentCount; i++){
      double depth = this.opponents[i].getX();
      if(depth1 < depth){
	depth2 = depth1;
	depth1 = depth;
      }
      else if(depth2 < depth){
	depth2 = depth;
      }
    }

    if(depth2 < ballX)
      offsideLine = ballX;
    else
      offsideLine = depth2;
    
    return offsideLine;

  }

/***************************************************************************/

  public void getPosOfOpponentsForward(Point oppXY){ // Pointオブジェクト!!
    double depth = Generic.FIELD_X_WIDTH * 0.5;

    for(int i = 0; i < 11; i++){
      if(this.opponentsWithUNum[i].getCertainty() > 0.1){  // certainty != 0
	double tmp = this.opponentsWithUNum[i].getX();
	if(depth > tmp){
	  depth = tmp;
	  oppXY.setPointX(depth);
	  oppXY.setPointY(this.opponentsWithUNum[i].getY());
	}
      }
    }
    
    for(int i = 0; i < this.opponentCount; i++){
	double tmp = this.opponents[i].getX();
	if(depth > tmp){
	  depth = tmp;
	  oppXY.setPointX(depth);
	  oppXY.setPointY(this.opponents[i].getY());  // NS99のバグ？
	}
    }
  }
    
/***************************************************************************/

    // 自分は味方の中で (posX,posY) から何番目に近いのか
  public int getOrderFrom(double posX,double posY,double myX,double myY){
    int order = 0;
    double dist = Math.sqrt((posX - myX) * (posX - myX) 
		       + (posY - myY) * (posY - myY));

    // golieは除く！
    for(int i = 1; i < 11; i++){
      if(teammatesWithUNum[i].getCertainty() > 0.1)
        if(Math.sqrt(Math.pow(posX - teammatesWithUNum[i].getX(),2) +
           	     Math.pow(posY - teammatesWithUNum[i].getY(),2)) < dist)
	  order++;
    }
    
    for(int i = 0; i < teammateCount; i++){
      if(Math.sqrt(Math.pow(posX - teammates[i].getX(),2) + 
       	           Math.pow(posY - teammates[i].getY(),2)) < dist)
        order++;
    }

    order++;
    
    return order;
  }

/***************************************************************************/

  public double evalLine(double sx,double sy,double ex,double ey){
    double a = (ey - sy) / (ex - sx);    // 2点を結ぶ線分の傾きを求める
    double b = sy - a * sx;
    double ia,sb,eb;
    double value = 0;

    PlayerInfo[] opps = new PlayerInfo[22];
    
    int oppCount;
    
    // 面倒なので opps という配列に敵プレーヤを詰めておく
    for(int i = 0; i < this.opponentCount; i++){
      opps[i] = this.opponents[i];
    }
    oppCount = this.opponentCount;
    for(int i = 0; i < 11; i++){
      if(this.opponentsWithUNum[i].getCertainty() > 0.1){
	opps[oppCount] = this.opponentsWithUNum[i];
	oppCount++;
      }
    }

    if(Math.abs(a) > 0.05){
      if(Math.abs(ex - sx) < 0.001){
	ia = 0;
      }
      else {
	ia = - 1.0 / a;  // 境界線の傾きを求める
      }
	
      sb = sy - ia * sx;  // 境界線の切片を求める
      eb = ey - ia * ex;

      if((sx <= ex && ia < 0) || (sx > ex && ia > 0)){
	for(int i = 0; i < oppCount; i++){
	  double ox = opps[i].getX();
	  double oy = opps[i].getY();
	  if(oy - ia * ox - sb >= 0 && oy -ia * ox - eb <= 0){
	    value += (opps[i].getCertainty() * 
	             (1.0 / (Generic.getDistPointLine(ox,oy,a,-1,b)
                          / 5.0 + 0.7)));
	  }
	  value += (opps[i].getCertainty() *
		   (1.0 / (Math.sqrt((ox - ex) * (ox - ex) + 
                                     (oy - ey) * (oy - ey)) / 5.0 + 1.0)));
	}
      }
      else {
	for(int i = 0; i < oppCount; i++){
	  double ox = opps[i].getX();
	  double oy = opps[i].getY();
          if(oy - ia * ox - sb <= 0 && oy -ia * ox - eb >= 0){
            value += (opps[i].getCertainty() *
                     (1.0 /  (Generic.getDistPointLine(ox,oy,a,-1,b)
                          / 5.0 + 0.7)));
          }
          value += (opps[i].getCertainty() *
       	           (1.0 / (Math.sqrt((ox - ex) * (ox - ex) +
	                             (oy - ey) * (oy - ey)) / 5.0 + 1.0)));
	}
      }
    }
    else {
      for(int i = 0; i < oppCount; i++){
	double ox = opps[i].getX();
        double oy = opps[i].getY();
        if((sx <= ox && ox <= ex) || (ex <= ox && ox <= sx)){
          value += (opps[i].getCertainty() *
       	      (1.0 / (Generic.getDistPointLine(ox,oy,a,-1,b) / 5.0 + 0.7)));
	}
        value += (opps[i].getCertainty() *
       	         (1.0 / (Math.sqrt((ox - ex) * (ox - ex)
                      + (oy - ey) * (oy - ey)) / 5.0 + 1.0)));
      }
    }

    //value *= (DIST(sx,ey,ex,ey) / 20.0); // 遠くなればなるほど値が大きくなる

    //cout << value << endl;

    return value;
  }

/***************************************************************************/

  public double evalPlayer(PlayerInfo player,double X,double Y){
    return evalLine(X,Y,player.getX(),player.getY());
  }

/***************************************************************************/

  public double getPosOfReceiver(double dir,double dirDiff,double dist,
			    double X,double Y,Point recXY){
                                                         // Pointオブジェクト!!
    double minValue = 1000;
    
    for(int i = 0; i < 11; i++){
      if(this.teammatesWithUNum[i].getCertainty() > 0.1){
	double tx = this.teammatesWithUNum[i].getX();
	double ty = this.teammatesWithUNum[i].getY();
	double tdir = Generic.RADIAN * Math.atan2(ty - Y,tx - X);
	double tdist = Generic.DIST(X,Y,tx,ty);
	    
	if(tdist <= dist && Math.abs(Generic.normDir(tdir - dir)) <= dirDiff){
	  double value = evalLine(X,Y,tx,ty);
	  if(value < minValue){
	    recXY.setPointX(tx);
	    recXY.setPointY(ty);
	    minValue = value;
	  }
	}
      }
    }

    for(int i = 0; i < this.teammateCount; i++){
      double tx = this.teammates[i].getX();
      double ty = this.teammates[i].getY();
      double tdir = Generic.RADIAN * Math.atan2(ty - Y,tx - X);
      double tdist = Generic.DIST(X,Y,tx,ty);
      if(tdist <= dist && Math.abs(Generic.normDir(tdir - dir)) <= dirDiff){
	double value = evalLine(X,Y,tx,ty);
	if(value < minValue){
	  recXY.setPointX(tx);
	  recXY.setPointY(ty);
	  minValue = value;
	}
      }
    }
    
    //cout << minValue << endl;

    return minValue;

    }

/***************************************************************************/


  public double getPosOfNearestOpponent(double dir,double dirDiff,double dist,
				   double X,double Y,Point oppXY){
                                                        // Pointオブジェクト!!
    double minDist = 1000;
    
    for(int i = 0; i < 11; i++){
      if(this.opponentsWithUNum[i].getCertainty() > 0.1){
	double ox = this.opponentsWithUNum[i].getX();
        double oy = this.opponentsWithUNum[i].getY();
	double odir = Generic.RADIAN * Math.atan2(oy - Y,ox - X);
	double odist = Generic.DIST(X,Y,ox,oy);
	if(odist <= dist && Math.abs(Generic.normDir(odir - dir)) <= dirDiff
                                                          && odist < minDist){
	  minDist = odist;
	  oppXY.setPointX(ox);
	  oppXY.setPointY(oy);
	}
      }
    }

    for(int i = 0; i < this.opponentCount; i++){
      double ox = this.opponents[i].getX();
      double oy = this.opponents[i].getY();
      double odir = Generic.RADIAN * Math.atan2(oy - Y,ox - X);
      double odist = Generic.DIST(X,Y,ox,oy);
      if(odist <= dist && Math.abs(Generic.normDir(odir - dir)) <= dirDiff
	                                                && odist < minDist){
        minDist = odist;
	oppXY.setPointX(ox);
	oppXY.setPointY(oy);
      }
    }
    
    return minDist;
  }

/***************************************************************************/
    
  public void printInfo(){
    for(int i = 0; i < 11; i++){
      if(this.teammatesWithUNum[i].getCertainty() > 0.1){
	System.out.println("Teammate : " + (i + 1));
	System.out.println("(X,Y) = (" + this.teammatesWithUNum[i].getX()
                             + "," + teammatesWithUNum[i].getY() + ")");
	System.out.println("Certainty : " 
                                 + teammatesWithUNum[i].getCertainty());
	}
    }

    for(int i = 0; i < this.teammateCount; i++){
	if(this.teammates[i].getCertainty() > 0.1){
	  System.out.println("Teammate : Unknown");
	  System.out.println("(X,Y) = (" + this.teammates[i].getX() + ","
		 + this.teammates[i].getY() + ")");
	  System.out.println("Certainty : "
                                   + this.teammates[i].getCertainty());
	}
    }

    for(int i = 0; i < 11; i++){
	if(this.opponentsWithUNum[i].getCertainty() > 0.1){
	  System.out.println("Opponent : " + (i + 1));
	  System.out.println("(X,Y) = (" + this.opponentsWithUNum[i].getX()
                                + "," + opponentsWithUNum[i].getY() + ")");
	  System.out.println("Certainty : "
                                   + this.opponentsWithUNum[i].getCertainty());
	}
    }

    for(int i = 0; i < this.opponentCount; i++){
	if(this.opponents[i].getCertainty() > 0.1){
	  System.out.println("Opponent : Unknown");
	  System.out.println("(X,Y) = (" + this.opponents[i].getX() + ","
		                + opponents[i].getY() + ")");
	  System.out.println("Certainty : " 
                                   + this.opponents[i].getCertainty());
	}
    }
  }
    
/***************************以下追加メソッド********************************/

  //  ボールがあるサイドに敵が集中しているか？
  public boolean parseSide(double ballY){
    int count = 0;
    int opponents = 0;
    for(int i = 0; i < 11; i++){
      if(opponentsWithUNum[i].getCertainty() > 0.1){
        opponents++;
        if(opponentsWithUNum[i].getY() * ballY >= 0)
	  count++;
      }
    }
    
    for(int i = 0; i < opponentCount; i++){
      opponents++;
      if(opponentsWithUNum[i].getY() * ballY >= 0)
	count++;
    }

    if(count * 2 > opponents)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }

/***************************************************************************/

  //  前方にいるFW(味方)の人数を返す
  public int forwardFW(double dir,double dirDiff,double dist,
                                                 double X,double Y){ 
    int count = 0;   
    for(int i = 0; i < 11; i++){
      if(this.teammatesWithUNum[i].getCertainty() > 0.1){
	double tx = this.teammatesWithUNum[i].getX();
	double ty = this.teammatesWithUNum[i].getY();
        double tdir = Generic.RADIAN * Math.atan2(ty - Y,tx - X);	    

	if((tx > X + dist) && 
                            Math.abs(Generic.normDir(tdir - dir)) <= dirDiff)
          count++;
      }
    }

    for(int i = 0; i < this.teammateCount; i++){
      double tx = this.teammates[i].getX();
      double ty = this.teammates[i].getY();
      double tdir = Generic.RADIAN * Math.atan2(ty - Y,tx - X);

      if((tx > X + dist) &&
                            Math.abs(Generic.normDir(tdir - dir)) <= dirDiff)
        count++;
    }

    return count;
  }

/***************************************************************************/

  public double getSideChangeReceiver(double dir,double dirDiff,double dist,
			    double X,double Y,Point recXY){
                                                         // Pointオブジェクト!!
    double minValue = 1000;
    
    for(int i = 0; i < 11; i++){
      if(this.teammatesWithUNum[i].getCertainty() > 0.1){
	double tx = this.teammatesWithUNum[i].getX();
	double ty = this.teammatesWithUNum[i].getY();
        if((Y >= 0 && ty < Y) || (Y < 0 && ty > Y)){
	  double tdir = Generic.RADIAN * Math.atan2(ty - Y,tx - X);
	  double tdist = Generic.DIST(X,Y,tx,ty);
	    
	  if(tdist <= dist && Math.abs(Generic.normDir(tdir - dir))
                                                           <= dirDiff){
	    double value = evalLine(X,Y,tx,ty);
	    if(value < minValue){
	      recXY.setPointX(tx);
	      recXY.setPointY(ty);
	      minValue = value;
	    }
	  }
	}
      }
    }

    for(int i = 0; i < this.teammateCount; i++){
      double tx = this.teammates[i].getX();
      double ty = this.teammates[i].getY();
      if((Y >= 0 && ty < Y) || (Y < 0 && ty > Y)){
        double tdir = Generic.RADIAN * Math.atan2(ty - Y,tx - X);
        double tdist = Generic.DIST(X,Y,tx,ty);
        if(tdist <= dist && Math.abs(Generic.normDir(tdir - dir)) <= dirDiff){
	  double value = evalLine(X,Y,tx,ty);
	  if(value < minValue){
	    recXY.setPointX(tx);
	    recXY.setPointY(ty);
	    minValue = value;
	  }
        }
      }
    }
    
    //cout << minValue << endl;

    return minValue;

    }

/***************************************************************************/

  public PlayerInfo getTeammateInfo(int unum){
    return this.teammatesWithUNum[unum - 1];
  }

/***************************************************************************/

  public PlayerInfo getOpponetsPlayer(int select){
    return opponentsWithUNum[select];

  }

/***************************************************************************/

  public PlayerInfo getTeammate(int select){
    return teammatesWithUNum[select];
  }

/***************************************************************************/

  // フリーの判定。視界内(距離指定)＋Neighborhood -0.5m内に敵がいるかどうか
  // 1サイクル後をチェックするように変更。00/6/15
  public boolean checkMarkedOpponent(double dir,double dirDiff,
                                     double dist, double X,double Y){
                                                       
    boolean freeFlag = Generic.NS_FALSE;
    int i = 0;
    
    while(!freeFlag && i < 11){
      if(this.opponentsWithUNum[i].getCertainty() > 0.1){
	double ox = this.opponentsWithUNum[i].getPredX(1);
        double oy = this.opponentsWithUNum[i].getPredY(1);
	double odir = Generic.RADIAN * Math.atan2(oy - Y,ox - X);
	double odist = Generic.DIST(X,Y,ox,oy);

	if((odist <= Generic.NEIGHBORHOOD - 0.5) ||
          (odist <= dist && Math.abs(Generic.normDir(odir - dir)) <= dirDiff)){
          freeFlag = Generic.NS_TRUE;
	}
      }
    i++;
    }

    i = 0;

    while(!freeFlag && i < this.opponentCount){
      double ox = this.opponents[i].getPredX(1);
      double oy = this.opponents[i].getPredY(1);
      double odir = Generic.RADIAN * Math.atan2(oy - Y,ox - X);
      double odist = Generic.DIST(X,Y,ox,oy);

      if((odist <= Generic.NEIGHBORHOOD - 0.5) ||
        (odist <= dist && Math.abs(Generic.normDir(odir - dir)) <= dirDiff)){
        freeFlag = Generic.NS_TRUE;
      }
    i++;
    }
    
    return freeFlag;
  }

/***************************************************************************/

  public void formatAll(){
    for(int i = 0; i < 11; i++){
      teammatesWithUNum[i] = new PlayerInfo();
      opponentsWithUNum[i] = new PlayerInfo();
      teammates[i] = new PlayerInfo();
      opponents[i] = new PlayerInfo();
      teammateShadowsWithUNum[i] = new PlayerInfo();
      opponentShadowsWithUNum[i] = new PlayerInfo();
      teammateShadows[i] = new PlayerInfo();
      opponentShadows[i] = new PlayerInfo();
      unknownPlayers[i] = new PlayerInfo();
    }
    for(int i = 11; i < 21; i++){
      unknownPlayers[i] = new PlayerInfo();
    }
  }

/***************************************************************************/

  public double getPosOfReceiver(double dir,double dirDiff,double dist,
			    double X,double Y,Point recXY,int point){

    double minValue = 1000;
    
    for(int i = 0; i < 11; i++){
      if(this.teammatesWithUNum[i].getCertainty() > 0.1){
	double tx = this.teammatesWithUNum[i].getX();
	double ty = this.teammatesWithUNum[i].getY();
	double tdir = Generic.RADIAN * Math.atan2(ty - Y,tx - X);
	double tdist = Generic.DIST(X,Y,tx,ty);
	    
	if(tdist <= dist && Math.abs(Generic.normDir(tdir - dir)) <= dirDiff && (tx > X+point)){
	  double value = evalLine(X,Y,tx,ty);
	  if(value < minValue){
	    recXY.setPointX(tx);
	    recXY.setPointY(ty);
	    minValue = value;
	  }
	}
      }
    }

    for(int i = 0; i < this.teammateCount; i++){
      double tx = this.teammates[i].getX();
      double ty = this.teammates[i].getY();
      double tdir = Generic.RADIAN * Math.atan2(ty - Y,tx - X);
      double tdist = Generic.DIST(X,Y,tx,ty);
      if(tdist <= dist && Math.abs(Generic.normDir(tdir - dir)) <= dirDiff){
	double value = evalLine(X,Y,tx,ty);
	if(value < minValue){
	  recXY.setPointX(tx);
	  recXY.setPointY(ty);
	  minValue = value;
	}
      }
    }
    
    //cout << minValue << endl;

    return minValue;

  }

/***************************************************************************/

  public void updatePlayerTable(){
    for(int i = 0; i < 11; i++){
      teammatesWithUNum[i].calcNextState();
      opponentsWithUNum[i].calcNextState();

    }
    for(int i = 0; i < teammateCount; i++){
      teammates[i].calcNextState();
    }
    for(int i = 0; i < opponentCount; i++){
      opponentsWithUNum[i].calcNextState();
    }
  }

}

