// BallInfo.java

package objectP;

import genericP.*;
import java.awt.geom.*;

/********************************************************/
/*         BallInfo     ボール情報                      */
/********************************************************/

public class BallInfo extends ObjectInfo{

/***************************************************************************/

  public BallInfo(){
    super(0,0);
    setDecay(Generic.BALL_DECAY);
    setCertainty(0.0);
    setRand(Generic.BALL_RAND);
    conf_decay = 0.8;
  }

/***************************************************************************/

  public BallInfo(double distance,double direction){
    super(distance, direction);
    setDecay(Generic.BALL_DECAY);
    setRand(Generic.BALL_RAND);conf_decay = 0.8;
  }

/***************************************************************************/

  public BallInfo(double distance,double direction,
		  double distanceChange,double directionChange){
    super(distance, direction, distanceChange, directionChange);
    setDecay(Generic.BALL_DECAY);
    setRand(Generic.BALL_RAND);conf_decay = 0.8;
  }

/***************************************************************************/

  public String toStringBallInfo(){  // sprintf
    StringBuffer strBuff = new StringBuffer("(NIT_ball ");
    strBuff.append(getTime()).append(" ").append(getX()).append(" ")
       .append(getY()).append(" ").append(getVX()).append(" ")
       .append(getVY()).append(" ").append(getAX()).append(" ")
       .append(getAY()).append(")");
    return strBuff.toString();
  }

/***************************************************************************/

  // コンストラクタでしか情報登録が出来ないのはマズイので
  public void setBallInfo(int Time , double Dist , double Dir){
    controlTime = Time;
    seenTime = Time;
    dist = Dist;
    dir = Dir;
    
    /* Unknown を入れておくべきだと思う
    distChange = DistChng;
    dirChange = DirChng; 
    */

    setCertainty(1.0);
    setRand(Generic.BALL_RAND);
    setDecay(Generic.BALL_DECAY);
    conf_decay = 0.8;
  }

/***************************************************************************/

  // コンストラクタでしか情報登録が出来ないのはマズイので
  public void setBallInfo(int Time , double Dist , double Dir , double DistChng , double DirChng){
    controlTime = Time;
    seenTime = Time;
    dist = Dist;
    dir = Dir;
    distChange = DistChng;
    dirChange = DirChng;

    setCertainty(1.0);
    setRand(Generic.BALL_RAND);
    setDecay(Generic.BALL_DECAY);
    conf_decay = 0.8;
  }

/***************************************************************************/

  public double getPredX(int t){     // 時間 t 後のX座標を予測
    double predX = pos.getX();
    double predVX = vel.getX();
    
    for(int i = 0; i < t; i++){
	predX += predVX;
	predVX *= decay;
    }
    
    return predX;
  }

/***************************************************************************/

  public double getPredY(int t){     // 時間 t 後のY座標を予測
    double predY = pos.getY();
    double predVY = vel.getY();
    
    for(int i = 0; i < t; i++){
	predY += predVY;
	predVY *= decay;
    }
    
    return predY;
  }

/***************************************************************************/

  public Point2D getPredPos(int t){   // 時間 t 後の座標を予測
    Point2D temp = new Point2D.Double(getPredX(t) , getPredY(t));

    return temp;
  }

/***************************************************************************/

  // 情報の進行
  public void advanceBallInfo(int filedTime){
    if (filedTime == controlTime){
      // 進行する必要なし
      return;
    }

    // ノイズ取りの確証が得られたら使用
    //RemoveNoise();
	
    double ux,uy; 
    // ここが実行される時は必ず 1 秒のずれは生じているので
    if (getCertainty2()){
      ux = vel.getX() + acc.getX();
      uy = vel.getY() + acc.getY();

      pos.setLocation(pos.getX()+ux , pos.getY()+uy);
      
      vel.setLocation(ux * decay , uy * decay);

      acc.setLocation(0.0 , 0.0);

      certainty *= conf_decay;
      controlTime++;
    }
    
    // 基本的に 2 秒以上離れないはずだが念の為
    while((filedTime - controlTime) != 0){
      // 2 秒以上のずれの場合はここで修正
      if (getCertainty2()){
	// 絶対に加速度は不明であるから
	ux = vel.getX();
	uy = vel.getY();
	
	pos.setLocation(pos.getX()+ux , pos.getY()+uy);
	
	vel.setLocation(ux * decay , uy * decay);
	
	certainty *= conf_decay;	
      }
      controlTime++;
    }
  }

/***************************************************************************/

  // データをクリアする．信頼性が落ちた時にゴミが残らないようにする為．
  private void ClearBallInfo(){
    setPos(0.0,0.0);
    setV(0.0,0.0);
    setA(0.0,0.0);
    setCertainty(0.0);
   
    // Unknown 識別子を入れた方が厳密だと思う
    dir = 0;
    dist = 0;
    distChange = 0;
    dirChange = 0; 
  }
  
/***************************************************************************/  

  // ボールに関してがデータの消去の必要があるので,こっちを使う
  public boolean getCertainty2(){
    if (certainty < min_conf){
      ClearBallInfo();
      return false;
    }
    return true;
  }

/***************************************************************************/

  // see と sense の同期を取るため
  public void adjustBallDir(double turnDir){
    dir -= turnDir;
  }
}









