// Point.java

package genericP;

/********************************************************/
/*         Point フィールドの座標                       */
/********************************************************/

public class Point extends java.awt.geom.Point2D.Double{

/***************************************************************************/

  public Point(){

      super();

  }

/***************************************************************************/

  public Point(double tx, double ty){

      super(tx, ty);

  }

/***************************************************************************/

  public void setPoint(double tx, double ty){
 
      setLocation(tx, ty);

  }

/***************************************************************************/

  public void setPointX(double tx){

      setLocation(tx, y);

  }

/***************************************************************************/

  public void setPointY(double ty){

      setLocation(x, ty);

  }

/***************************************************************************/

  public double getPointX(){
    return getX();
  }

/***************************************************************************/

  public double getPointY(){
    return getY();
  }

}
