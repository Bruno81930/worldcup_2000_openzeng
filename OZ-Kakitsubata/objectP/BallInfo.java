// BallInfo.java

package objectP;

import genericP.*;
import java.awt.geom.*;

/********************************************************/
/*         BallInfo     �ܡ������                      */
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

  // ���󥹥ȥ饯���Ǥ���������Ͽ������ʤ��Τϥޥ����Τ�
  public void setBallInfo(int Time , double Dist , double Dir){
    controlTime = Time;
    seenTime = Time;
    dist = Dist;
    dir = Dir;
    
    /* Unknown ������Ƥ����٤����Ȼפ�
    distChange = DistChng;
    dirChange = DirChng; 
    */

    setCertainty(1.0);
    setRand(Generic.BALL_RAND);
    setDecay(Generic.BALL_DECAY);
    conf_decay = 0.8;
  }

/***************************************************************************/

  // ���󥹥ȥ饯���Ǥ���������Ͽ������ʤ��Τϥޥ����Τ�
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

  public double getPredX(int t){     // ���� t ���X��ɸ��ͽ¬
    double predX = pos.getX();
    double predVX = vel.getX();
    
    for(int i = 0; i < t; i++){
	predX += predVX;
	predVX *= decay;
    }
    
    return predX;
  }

/***************************************************************************/

  public double getPredY(int t){     // ���� t ���Y��ɸ��ͽ¬
    double predY = pos.getY();
    double predVY = vel.getY();
    
    for(int i = 0; i < t; i++){
	predY += predVY;
	predVY *= decay;
    }
    
    return predY;
  }

/***************************************************************************/

  public Point2D getPredPos(int t){   // ���� t ��κ�ɸ��ͽ¬
    Point2D temp = new Point2D.Double(getPredX(t) , getPredY(t));

    return temp;
  }

/***************************************************************************/

  // ����οʹ�
  public void advanceBallInfo(int filedTime){
    if (filedTime == controlTime){
      // �ʹԤ���ɬ�פʤ�
      return;
    }

    // �Υ������γξڤ�����줿�����
    //RemoveNoise();
	
    double ux,uy; 
    // �������¹Ԥ�������ɬ�� 1 �äΤ���������Ƥ���Τ�
    if (getCertainty2()){
      ux = vel.getX() + acc.getX();
      uy = vel.getY() + acc.getY();

      pos.setLocation(pos.getX()+ux , pos.getY()+uy);
      
      vel.setLocation(ux * decay , uy * decay);

      acc.setLocation(0.0 , 0.0);

      certainty *= conf_decay;
      controlTime++;
    }
    
    // ����Ū�� 2 �ðʾ�Υ��ʤ��Ϥ�����ǰ�ΰ�
    while((filedTime - controlTime) != 0){
      // 2 �ðʾ�Τ���ξ��Ϥ����ǽ���
      if (getCertainty2()){
	// ���Ф˲�®�٤������Ǥ��뤫��
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

  // �ǡ����򥯥ꥢ���롥����������������˥��ߤ��Ĥ�ʤ��褦�ˤ���١�
  private void ClearBallInfo(){
    setPos(0.0,0.0);
    setV(0.0,0.0);
    setA(0.0,0.0);
    setCertainty(0.0);
   
    // Unknown ���̻Ҥ����줿������̩���Ȼפ�
    dir = 0;
    dist = 0;
    distChange = 0;
    dirChange = 0; 
  }
  
/***************************************************************************/  

  // �ܡ���˴ؤ��Ƥ��ǡ����ξõ��ɬ�פ�����Τ�,���ä���Ȥ�
  public boolean getCertainty2(){
    if (certainty < min_conf){
      ClearBallInfo();
      return false;
    }
    return true;
  }

/***************************************************************************/

  // see �� sense ��Ʊ�����뤿��
  public void adjustBallDir(double turnDir){
    dir -= turnDir;
  }
}









