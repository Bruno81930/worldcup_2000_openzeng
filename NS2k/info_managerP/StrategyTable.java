// StrategyTable.java

package info_managerP;

import java.util.*;
import genericP.*;
import objectP.*;
import stokenizerP.*;
import parserP.*;

/********************************************************/
/*         StrategyTable      協調戦略の管理,加工       */
/********************************************************/

public class StrategyTable{

  //  協調戦略の情報
  //  背番号が分かっている奴はIndex + 1 が背番号になるように
  //  分からない奴は順番につめる
  StrategyInfo[] strategyWithUNum = new StrategyInfo[11];
  StrategyInfo[] strategyNonUNum = new StrategyInfo[11];

  PlayerTable playerTable;

  int teammateCount;

/***************************************************************************/

  public StrategyTable(PlayerTable pTable){

    formatAll();           //  StrategyInfoテーブルの初期化 
    teammateCount = 0;

    playerTable = pTable;  //  PlayerTableの共有化(InfoManagerと)

  }

/***************************************************************************/
    
  public void printInfo(){
    for(int i = 0; i < 11; i++){
      if(this.strategyWithUNum[i].getCertainty() > 0.1){
	System.out.println("UniformNun : " + (i + 1));
	System.out.println("Strategy : " + strategyWithUNum[i].getStrategy());
        System.out.println("Type : " + strategyWithUNum[i].getType());
	System.out.println("(X,Y) = (" + this.strategyWithUNum[i].getX()
                             + "," + strategyWithUNum[i].getY() + ")");
	System.out.println("Certainty : " 
                                 + strategyWithUNum[i].getCertainty());
	}
    }

    for(int i = 0; i < this.teammateCount; i++){
      if(this.strategyNonUNum[i].getCertainty() > 0.1){
	System.out.println("UniformNun : Unknown");
	System.out.println("Strategy : " + strategyNonUNum[i].getStrategy());
        System.out.println("Type : " + strategyWithUNum[i].getType());
	System.out.println("(X,Y) = (" + this.strategyNonUNum[i].getX()
                             + "," + strategyNonUNum[i].getY() + ")");
	System.out.println("Certainty : " 
                                 + strategyNonUNum[i].getCertainty());
	}
    }

  }

/***************************************************************************/

  public void formatAll(){
    for(int i = 0; i < 11; i++){
      strategyWithUNum[i] = new StrategyInfo();
      strategyNonUNum[i] = new StrategyInfo();
    }
  }

/***************************************************************************/

  public void refresh(){
    for(int i = 0; i < 11; i++){
      strategyWithUNum[i].setStrategy("unknown","norm");
      strategyWithUNum[i].setCertainty(0.0);
      strategyNonUNum[i].setStrategy("unknown","norm");
      strategyNonUNum[i].setCertainty(0.0);
    }
  }

/***************************************************************************/

  public boolean hasPlayerSInfo(int unum){
    if(this.strategyWithUNum[unum - 1].getCertainty() > 0.1)
	return Generic.NS_TRUE;
    else
	return Generic.NS_FALSE;
  }

/***************************************************************************/

  public void updateStrategyTable(){
    for(int i = 0; i < 11; i++)
      strategyWithUNum[i].calcNextState();
  }

/***************************************************************************/

  public void setPlayerStrategy(int unum,String S,String tp){
    strategyWithUNum[unum - 1].setStrategy(S,tp);
  }

/***************************************************************************/

  public void setPlayerSPos(int unum,double X, double Y){
    strategyWithUNum[unum - 1].setPos(X,Y);
  }

/***************************************************************************/

  public String getPlayerStrategy(int unum){ 
    return strategyWithUNum[unum - 1].getStrategy();
  }

/***************************************************************************/

  public double getReceivePartner(String S,String tp,double dir,double dirDiff,
                                  double dist,double X,double Y,Point recXY){
    double minValue = 1000;

    for(int i = 0; i < 11; i++){
      if(this.strategyWithUNum[i].getStrategy().equals(S) && 
              this.strategyWithUNum[i].getType().equals(tp)){
        PlayerInfo  partner = this.playerTable.getTeammateInfo(i + 1);
        if(partner.getCertainty() > 0.1){
	  double tx = partner.getX();
	  double ty = partner.getY();
	  double tdir = Generic.RADIAN * Math.atan2(ty - Y,tx - X);
	  double tdist = Generic.DIST(X,Y,tx,ty);
	    
	  if(tdist <= dist && 
                           Math.abs(Generic.normDir(tdir - dir)) <= dirDiff){
	    double value = this.playerTable.evalLine(X,Y,tx,ty);
	    if(value < minValue){
	      recXY.setPointX(tx);
	      recXY.setPointY(ty);
	      minValue = value;
	    }
	  }
	}
      }
    }

    return minValue;

    }

/***************************************************************************/

  public int getStrategyPlanner(String S){
    int uNum = 0;
    double C = 0;
    double tmpC;
    for(int i = 0; i < 11; i++){
      if(this.strategyWithUNum[i].getStrategy().equals(S) && 
              this.strategyWithUNum[i].getType().equals("plan")){
        tmpC = this.strategyWithUNum[i].getCertainty();
        if(tmpC > C){
          C = tmpC;
          uNum = i + 1;
	}
      }
    }
    return uNum;
  }

/***************************************************************************/

  public int getStrategyPartner(String S){
    int uNum = 0;
    double C = 0;
    double tmpC;
    for(int i = 0; i < 11; i++){
      if(this.strategyWithUNum[i].getStrategy().equals(S)){
        tmpC = this.strategyWithUNum[i].getCertainty();
        if(tmpC > C){
          C = tmpC;
          uNum = i + 1;
	}
      }
    }
    return uNum;
  }


}




