// GoalInfo.java

package objectP;

import genericP.*;

/********************************************************/
/*         GoalInfo     ÉSÅ[ÉãèÓïÒ                      */
/********************************************************/

public class GoalInfo extends ObjectInfo{
  String side;

/***************************************************************************/

  public GoalInfo(double distance, double direction, String gs, String s){
    super(distance, direction);
    side = gs;

    if(side.equals(s))
      setX(- Generic.FIELD_X_WIDTH * 0.5);
    else
      setX(Generic.FIELD_X_WIDTH * 0.5);

    setY(0);
  }

/***************************************************************************/

  public String getSide(){
    return side;
  }

}
