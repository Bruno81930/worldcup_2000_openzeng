// PlayerTable.java

package info_managerP;

import java.util.*;
import genericP.*;
import objectP.*;
import stokenizerP.*;
import parserP.*;

/********************************************************/
/*         PlayerTable  $B%W%l%$%d!<>pJs$N4IM}(B,$B2C9)(B       */
/********************************************************/

public class PlayerTable{
    int time,shadowsTime;
    
    //  $B8=:_$N>pJs(B
    //  $BGXHV9f$,J,$+$C$F$$$kE[$O(BIndex + 1 $B$,GXHV9f$K$J$k$h$&$K(B
    //  $BJ,$+$i$J$$E[$O=gHV$K$D$a$k(B
    PlayerInfo[] teammatesWithUNum = new PlayerInfo[11];
    PlayerInfo[] opponentsWithUNum = new PlayerInfo[11];
    PlayerInfo[] teammates = new PlayerInfo[11];
    PlayerInfo[] opponents = new PlayerInfo[11];
    PlayerInfo[] unknownPlayers = new PlayerInfo[21];

    int teammateCount;
    int opponentCount;
    int unknownPlayerCount;

    //  $B2a5n$N>pJs(B
    PlayerInfo[] teammateShadowsWithUNum = new PlayerInfo[11];
    PlayerInfo[] opponentShadowsWithUNum = new PlayerInfo[11];
    PlayerInfo[] teammateShadows = new PlayerInfo[11];
    PlayerInfo[] opponentShadows = new PlayerInfo[11];

    int teammateShadowCount;
    int opponentShadowCount;

/***************************************************************************/

  public PlayerTable(){

    formatAll();           // PlayerInfo$B%F!<%V%k$N=i4|2=(B 
    teammateCount = 0;
    opponentCount = 0;
    unknownPlayerCount = 0;

  }

/***************************************************************************/
  // $B$=$&$$$($P$3$l!"2?$G(B uNum $B$r0z?t$K$H$C$F$k$N!)(B
  // PlayerInfo $B$+$iH4$-=P$;$P$$$$$@$1$8$c$J$$$N$+!)(B
  public void setTeammateWithUNum(PlayerInfo player, int uNum){
    
    this.teammatesWithUNum[uNum - 1] = player;

    //teammateShadowsWithUNum[uNum - 1].setCertainty(0);
    // $B;DA|$N>pJs$r;H$($J$/$7$F$*$/(B
    // $B;DA|$H<BA|$H$N(B1$BBP(B1$B4X78$rJ]$D$?$a(B
  }

/***************************************************************************/

  public void setOpponentWithUNum(PlayerInfo player, int uNum){
    this.opponentsWithUNum[uNum - 1] = player;

    //opponentShadowsWithUNum[uNum - 1].setCertainty(0);
    // $B;DA|$N>pJs$r;H$($J$/$7$F$*$/(B
    // $B;DA|$H<BA|$H$N(B1$BBP(B1$B4X78$rJ]$D$?$a(B
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

  // $B;DA|$r:n$k!D8=:_$N>pJs$,2a5n$N>pJs$K$J$k$@$1(B
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
      // $B;DA|$K<BA|$H40A4$K0lCW$9$k$b$N$,$"$l$PN><T$r8uJd$+$i30$9(B
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

    // $B=E$$>l9g$K$O30$7$F$b(BOK $B:#2s$O$O$:$9!#(B
    // **$B$3$3$+$i(B** 
    /*
    // teammates $B$H(B teammateShadowsWithUNum $B$rHf3S(B
    comparePlayerWithShadow(margin,teammatesCandFlag,this.teammates,
                            this.teammateCount,teammateShadowsWithUNumCandFlag,
			    this.teammateShadowsWithUNum,11);

    // opponents $B$H(B opponentShadowsWithUNum $B$rHf3S(B
    comparePlayerWithShadow(margin,opponentsCandFlag,this.opponents,
                            this.opponentCount,opponentShadowsWithUNumCandFlag,
			    this.opponentShadowsWithUNum,11);


    // teammates $B$H(B teammatesShadow $B$rHf3S(B
    comparePlayerWithShadow(margin,teammatesCandFlag,this.teammates,
                            this.teammateCount,teammateShadowsCandFlag,
                            this.teammateShadows,this.teammateShadowCount);

    // opponents $B$H(B opponentsShadow $B$rHf3S(B
    comparePlayerWithShadow(margin,opponentsCandFlag,this.opponents,
                            this.opponentCount,opponentShadowsCandFlag,
                            this.opponentShadows,this.opponentShadowCount);

    // teammatesWithUNum $B$H(B teammateShadows $B$rHf3S(B
    comparePlayerWithShadow(margin,teammatesWithUNumCandFlag,
                            this.teammatesWithUNum,11,teammateShadowsCandFlag,
                            this.teammateShadows,this.teammateShadowCount);

    // opponentsWithUNum $B$H(B opponentsShadows $B$rHf3S(B
    comparePlayerWithShadow(margin,opponentsWithUNumCandFlag,
                            this.opponentsWithUNum,11,opponentShadowsCandFlag,
                            this.opponentShadows,
			    this.opponentShadowCount);
    */
    // **$B$3$3$^$G(B**
    // $B$=$&$$$($P(B addTeammate,addOpponent $B$,$"$C$?!D8e$GD>$=$&(B
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

  public void getPosOfOpponentsForward(Point oppXY){ // Point$B%*%V%8%'%/%H(B!!
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
	  oppXY.setPointY(this.opponents[i].getY());  // NS99$B$N%P%0!)(B
	}
    }
  }
    
/***************************************************************************/

    // $B<+J,$OL#J}$NCf$G(B (posX,posY) $B$+$i2?HVL\$K6a$$$N$+(B
  public int getOrderFrom(double posX,double posY,double myX,double myY){
    int order = 0;
    double dist = Math.sqrt((posX - myX) * (posX - myX) 
		       + (posY - myY) * (posY - myY));

    // golie$B$O=|$/!*(B
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
    double a = (ey - sy) / (ex - sx);    // 2$BE@$r7k$V@~J,$N79$-$r5a$a$k(B
    double b = sy - a * sx;
    double ia,sb,eb;
    double value = 0;

    PlayerInfo[] opps = new PlayerInfo[22];
    
    int oppCount;
    
    // $BLLE]$J$N$G(B opps $B$H$$$&G[Ns$KE(%W%l!<%d$r5M$a$F$*$/(B
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
	ia = - 1.0 / a;  // $B6-3&@~$N79$-$r5a$a$k(B
      }
	
      sb = sy - ia * sx;  // $B6-3&@~$N@ZJR$r5a$a$k(B
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

    //value *= (DIST(sx,ey,ex,ey) / 20.0); // $B1s$/$J$l$P$J$k$[$ICM$,Bg$-$/$J$k(B

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
                                                         // Point$B%*%V%8%'%/%H(B!!
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
                                                        // Point$B%*%V%8%'%/%H(B!!
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
    
/***************************$B0J2<DI2C%a%=%C%I(B********************************/

  //  $B%\!<%k$,$"$k%5%$%I$KE($,=8Cf$7$F$$$k$+!)(B
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

  //  $BA0J}$K$$$k(BFW($BL#J}(B)$B$N?M?t$rJV$9(B
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
                                                         // Point$B%*%V%8%'%/%H(B!!
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

  // $B%U%j!<$NH=Dj!#;k3&Fb(B($B5wN%;XDj(B)$B!\(BNeighborhood -0.5m$BFb$KE($,$$$k$+$I$&$+(B
  // 1$B%5%$%/%k8e$r%A%'%C%/$9$k$h$&$KJQ99!#(B00/6/15
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

