// LineInfo.java

package objectP;

/********************************************************/
/*         LineInfo     �饤�����                      */
/********************************************************/

public class LineInfo extends ObjectInfo{
  String location;

/***************************************************************************/

  public LineInfo(double distance,double direction, String loc){
    super(distance, direction);
    setLocation(loc);
  }

/***************************************************************************/

  public void setLocation(String loc){
    location = loc;
  }

/***************************************************************************/

  public String getLocation(){
    return location;
  }

  public void setLineInfo(int Time , String loc , double Dist , double Dir){
    controlTime = Time;
    seenTime = Time;
    dist = Dist;
    dir = Dir;

    setLocation(loc);
  }
}










