// FlagInfo.java

package objectP;

import genericP.*;

/********************************************************/
/*         FlagInfo     フラグ情報                      */
/********************************************************/

public class FlagInfo extends ObjectInfo{

/***************************************************************************/

  public FlagInfo(double distance,double direction){  // センターマークの場合
    super(distance, direction);
    setX(0);
    setY(0);
  }

/***************************************************************************/

  public FlagInfo(double distance,double direction,
           String loc1,String loc2,String side){
    super(distance, direction);
    String MYDir;

    if(side.equals("l"))
      MYDir = "t";
    else
      MYDir = "b";

    if(loc1.equals("c"))
      setX(0);
    else if(side.equals(loc1))
      setX(- Generic.FIELD_X_WIDTH / 2.0);
    else
      setX(Generic.FIELD_X_WIDTH / 2.0);

    if(MYDir.equals(loc2))
      setY(- Generic.FIELD_Y_WIDTH / 2.0);
    else
      setY(Generic.FIELD_Y_WIDTH / 2.0); 
  }

/***************************************************************************/

  public FlagInfo(double distance,double direction,String PorG,
           String loc1,String loc2,String side){ // ペナルティ or ゴールポスト
    super(distance, direction);
    String MYDir;

    if(side.equals("l"))
      MYDir = "t";
    else
      MYDir = "b";

    if(PorG.equals("p")){  // ペナルティエリアフラグ
      if(side.equals(loc1))
	setX(- Generic.PENALTY_AREA_X_WIDTH * 0.5);
      else
        setX(Generic.PENALTY_AREA_X_WIDTH * 0.5);

      if(loc2.equals("c"))
        setY(0);
      else if(MYDir.equals(loc2))
          setY(- Generic.PENALTY_AREA_Y_WIDTH * 0.5);
      else
          setY(Generic.PENALTY_AREA_Y_WIDTH * 0.5);
    }
    else {                          // ゴールポスト
      if(side.equals(loc1))
        setX(- Generic.FIELD_X_WIDTH * 0.5);
      else
        setX(Generic.FIELD_X_WIDTH * 0.5);

      if(MYDir.equals(loc2))
        setY(- Generic.GOAL_WIDTH * 0.5);
      else
        setY(Generic.GOAL_WIDTH * 0.5);
    }
  }

/***************************************************************************/

  public FlagInfo(double distance,double direction, // 外周フラグ
	   String baseLine,String locOnLine,int pitch,String side){
    super(distance, direction);
    String MYDir; 

    if(side.equals("l"))
	MYDir = "t";
    else
	MYDir = "b";

    if(baseLine.equals("t") || baseLine.equals("b")){
	if(MYDir.equals(baseLine))
	    setY(- (Generic.FIELD_Y_WIDTH * 0.5 + 5.0));
	else
	    setY(Generic.FIELD_Y_WIDTH * 0.5 + 5.0);

	if(side.equals(locOnLine))
	    setX(- pitch);
	else
	    setX(pitch);
    } else {
	if(baseLine.equals(side))
	    setX(- (Generic.FIELD_X_WIDTH * 0.5 + 5.0));
	else
	    setX(Generic.FIELD_X_WIDTH * 0.5 + 5.0);
	
	if(MYDir.equals(locOnLine))
	    setY(- pitch);
	else
	    setY(pitch);
    }
  }

/***************************************************************************/

  public FlagInfo(double distance,double direction,
	   String baseLine,String side){ // pitch 0 の場合
    super(distance, direction);
    String MYDir;
    
    if(side.equals("l"))
	MYDir = "t";
    else
	MYDir = "b";

    if(baseLine.equals("t") || baseLine.equals("b")){
	if(MYDir.equals(baseLine))
	    setY(- (Generic.FIELD_Y_WIDTH * 0.5 + 5.0));
	else
	    setY(Generic.FIELD_Y_WIDTH * 0.5 + 5.0);
	
	setX(0);
    } else {
	if(baseLine.equals(side))
	    setX(- (Generic.FIELD_X_WIDTH * 0.5 + 5.0));
	else
	    setX(Generic.FIELD_X_WIDTH * 0.5 + 5.0);
	
	setY(0);
    }
  }

/***************************************************************************/

  public void printInfo(){
    System.out.println("--------- Flag ---------");
    System.out.println("X : " + getX());
    System.out.println("Y : " + getY());
  }

}



















