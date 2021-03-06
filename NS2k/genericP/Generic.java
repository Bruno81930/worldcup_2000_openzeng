//  Generic.java

package genericP;

import serverconfP.*;

/********************************************************/
/*         Generic                                      */
/********************************************************/

public class Generic implements Serverconf{

  public static final boolean NS_FALSE = false;
  public static final boolean NS_TRUE = true;

  public static final double RADIAN = 180.0 / Math.PI;
  
  public static final double FIELD_X_WIDTH = 105.0;
  public static final double FIELD_Y_WIDTH = 68.0;
  public static final double PENALTY_AREA_X_WIDTH = 72.0;
  public static final double PENALTY_AREA_Y_WIDTH = 40.4;

  public static final double KICKABLE_AREA 
                              = PLAYER_SIZE + BALL_SIZE + KICKABLE_MARGIN;

  public static final boolean DEBUG = false;   //  デバッグ用
  public static final boolean DEBUG2 = false;  //  デバッグ用2

/***************************************************************************/

  public static double digToRad(double dig){
	return dig / 180.0 * Math.PI;
  }

/***************************************************************************/

  public static double DIST(double X1, double Y1, double X2, double Y2){
        return Math.sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1));
  }

/***************************************************************************/

  public static double normDir(double direction){
    if(direction > 180.0)
	return direction - 360.0;
    else if(direction < -180.0)
	return direction + 360.0;
    else
	return direction;
  }

/***************************************************************************/

  public static void getCrossPoint(double r, double x1, double y1, double x2,
				   double y2, Point tmp1, Point tmp2){
                                                                   // Point!!
    if(Math.abs(x1 - x2) < 0.01){
	tmp1.setPointX(x1);
        tmp2.setPointX(x1);
	tmp1.setPointY(Math.sqrt(r * r + tmp1.getPointX() * tmp1.getPointX()));
	tmp2.setPointY(- Math.sqrt(r * r + tmp2.getPointX()
                                                          * tmp2.getPointX()));
    } else {
	double a,b; // 直線の傾きと切片 (y = a * x + b)
	a = (y1 - y2) / (x1 - x2);
	b = y2 - a * x2;

	if((1 + a) < 0.01){
	    tmp1.setPointX((r * r - b * b) * 0.5 * a * b);
            tmp2.setPointX((r * r - b * b) * 0.5 * a * b);
	} else {
	    tmp1.setPointX((-2 * a * b + Math.sqrt(Math.pow(2 * a * b,2)
				       - 4 * (1 + a) * (b * b - r * r)))
		* 0.5 * (1 + a));
	    tmp2.setPointX((-2 * a * b - Math.sqrt(Math.pow(2 * a * b,2)
				       - 4 * (1 + a) * (b * b - r * r)))
		* 0.5 * (1 + a));
	}
	
	tmp1.setPointY(a * tmp1.getPointX() + b);
	tmp2.setPointY(a * tmp2.getPointX() + b);
    }
  }

/***************************************************************************/

  public static double getDistPointLine(double X,double Y,double a,double b,
                                        double c){
    return Math.pow(a * X + b * Y + c, 2) / (a * a + b * b);
  }

/***************************************************************************/

  public static void getDivisionPoint(double sx,double sy,double ex,double ey,
		      double m,double n,Point dxy){
                                                               // Point!!
    dxy.setPointX((n * sx + m * ex) / (m + n));
    dxy.setPointY((n * sy + m * ey) / (m + n));
  }

/***************************************************************************/

  public static void getDivisionPointByDX(double sx,double sy,double ex,
                                          double ey,Point dxy){
                                                               // Point!!
    double m = Math.abs(sx - dxy.getPointX());
    double n = Math.abs(ex - dxy.getPointX());
    
    dxy.setPointY((n * sy + m * ey) / (m + n));
  }

/***************************************************************************/

  // キーパー用 DivisionPoint
  public static void getDivisionPointForGoalie(double ballx,double bally,
					    double ex,Point dxy){
    
    // System.out.println("ballX" + ballx);
    // System.out.println("ballY" + bally);

    double a = bally / (ballx + 52.5); // 傾きを求める
    double b = a * 52.5; // 切片を求める

    double y = a * ex + b;

    if (y > 9.16){
      y = 9.16;
    }
    if (y < -9.16){
      y = -9.16;
    }

    dxy.setPointX(ex);
    dxy.setPointY(y);
    
  }

}
