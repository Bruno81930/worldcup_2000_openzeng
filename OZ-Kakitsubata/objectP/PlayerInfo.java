// PlayerInfo.java

package objectP;
import genericP.*;
import java.awt.geom.*;

/********************************************************/
/*         PlayerInfo �ץ쥤�����                      */
/********************************************************/

public class PlayerInfo extends ObjectInfo{
  String teamname;
  int uniformNum;

  double bodyAbsDir;     // ���к�ɸ�Ǥο��Τθ���
  double headAbsDir;     // ���к�ɸ�Ǥμ�θ���
  double headRelDir;     // ���Τθ�������μ�θ���

/***************************************************************************/

  public PlayerInfo(){
    super(0,0);
    teamname = "";
    uniformNum = 0;
    setCertainty(0.0);   // �Ĥޤꤳ��ʥץ쥤�䡼���Τ�ʤ�
    setRand(Generic.PLAYER_RAND);
  }

/***************************************************************************/

  public PlayerInfo(String team , int uNum){
    super(0,0);
    teamname = team;
    uniformNum = uNum;
    setCertainty(0.0);   // �Ĥޤꤳ��ʥץ쥤�䡼���Τ�ʤ�
    setRand(Generic.PLAYER_RAND);
  }

/***************************************************************************/

  public PlayerInfo(double distance,double direction){
    super(distance, direction);
    teamname = "";
    uniformNum = 0;
  }

/***************************************************************************/

  public PlayerInfo(double distance,double direction,
             double distanceChange,double directionChange){
    super(distance, direction, distanceChange, directionChange);
    teamname = "";
    uniformNum = 0;
  }

/***************************************************************************/

  public PlayerInfo(double distance,double direction,
             double distanceChange,double directionChange,
             double bodyFaceDirection,double headFaceDirection){
    super(distance, direction, distanceChange, directionChange);
    bodyAbsDir = bodyFaceDirection;
    headAbsDir = headFaceDirection;

    teamname = "";
    uniformNum = 0;
  }

/***************************************************************************/

  // ������̾�����ֹ�ϸ��դ��ˤ���
  public void setTeamname(String tname){
    teamname = tname;
  }

/***************************************************************************/

  public String getTeamname(){
    return teamname;
  }

/***************************************************************************/

  public void setUniformNum(int uNum){
    uniformNum = uNum;
  }

/***************************************************************************/

  public int getUniformNum(){
    return uniformNum;
  }

/***************************************************************************/

  // ���󥹥ȥ饯���Ǥ���������Ͽ������ʤ��Τϥޥ����Τ�
  public void setPlayerInfo(int Time , double Dist , double Dir){
    controlTime = Time;
    seenTime = Time;
    dist = Dist;
    dir = Dir;

    setRand(Generic.PLAYER_RAND);
    setCertainty(1.0);
  }

/***************************************************************************/ 

  // ���󥹥ȥ饯���Ǥ���������Ͽ������ʤ��Τϥޥ����Τ�
  public void setPlayerInfo(int Time , double Dist , double Dir , double DistChng , double DirChng){
    controlTime = Time;
    seenTime = Time;
    dist = Dist;
    dir = Dir;
    distChange = DistChng;
    dirChange = DirChng;

    setRand(Generic.PLAYER_RAND);
    setCertainty(1.0);
  }

/***************************************************************************/
  
  // ���󥹥ȥ饯���Ǥ���������Ͽ������ʤ��Τϥޥ����Τ�
  public void setPlayerInfo(int Time , double Dist , double Dir , double DistChng , double DirChng , double bodyDir , double headDir){
    controlTime = Time;
    seenTime = Time;
    dist = Dist;
    dir = Dir;
    distChange = DistChng;
    dirChange = DirChng;   
    bodyAbsDir = bodyDir;
    headAbsDir = headDir;

    setRand(Generic.PLAYER_RAND);
    setCertainty(1.0);
  }

/***************************************************************************/

  // ����οʹ�
  public void advancePlayerInfo(int filedTime){
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

  public Point2D getPredPos(int t){     // ���� t ���Y��ɸ��ͽ¬
    return new Point2D.Double(getPredX(t),getPredY(t));
  }

/***************************************************************************/
  
  public void setHeadAbsDir(double had){
    headAbsDir = had;    
  }

  public void setHeadRelDir(double hrd){
    headRelDir = hrd;
  }

  public void setBodyAbsDir(double bad){
    bodyAbsDir = bad;    
  }

  public double getHeadAbsDir(){
    return headAbsDir;    
  }

  public double getBodyAbsDir(){
    return bodyAbsDir;    
  }

  public double getHeadRelDir(){
    return headRelDir;    
  }
}





