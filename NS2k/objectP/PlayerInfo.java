// PlayerInfo.java

package objectP;
import genericP.*;
import java.awt.geom.*;

/********************************************************/
/*         PlayerInfo プレイヤ情報                      */
/********************************************************/

public class PlayerInfo extends ObjectInfo{
  String teamname;
  int uniformNum;

  double bodyAbsDir;     // 絶対座標での身体の向き
  double headAbsDir;     // 絶対座標での首の向き
  double headRelDir;     // 身体の向きからの首の向き

/***************************************************************************/

  public PlayerInfo(){
    super(0,0);
    teamname = "";
    uniformNum = 0;
    setCertainty(0.0);   // つまりこんなプレイヤーは知らない
    setRand(Generic.PLAYER_RAND);
  }

/***************************************************************************/

  public PlayerInfo(String team , int uNum){
    super(0,0);
    teamname = team;
    uniformNum = uNum;
    setCertainty(0.0);   // つまりこんなプレイヤーは知らない
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

  // チーム名と背番号は後付けにする
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

  // コンストラクタでしか情報登録が出来ないのはマズイので
  public void setPlayerInfo(int Time , double Dist , double Dir){
    controlTime = Time;
    seenTime = Time;
    dist = Dist;
    dir = Dir;

    setRand(Generic.PLAYER_RAND);
    setCertainty(1.0);
  }

/***************************************************************************/ 

  // コンストラクタでしか情報登録が出来ないのはマズイので
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
  
  // コンストラクタでしか情報登録が出来ないのはマズイので
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

  // 情報の進行
  public void advancePlayerInfo(int filedTime){
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

  public Point2D getPredPos(int t){     // 時間 t 後のY座標を予測
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





