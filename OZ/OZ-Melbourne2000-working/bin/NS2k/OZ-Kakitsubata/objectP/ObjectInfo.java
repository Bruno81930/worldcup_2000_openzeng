// ObjectInfo.java

package objectP;

import genericP.*;
import java.awt.geom.*;

/********************************************************/
/*         ObjectInfo ����Ū�ʥ��֥������Ⱦ���          */
/********************************************************/

public class ObjectInfo {
  final int UNKNOWN = -1;
  final double min_conf = 0.5;
  double conf_decay = 0.9;

  int controlTime;            // ���Υ��֥������Ȥ����椵�줿����
  int seenTime;               // ���Υ��֥������Ȥ�����줿����

  double dist;                // ��Υ
  double dir;                 // ����

  double distChange;          // ��Υ���Ѳ���
  double dirChange;           // �������Ѳ���
  boolean change;

  Point2D pos;                // ����
  Point2D vel;                // ����®��
  double relVV,relVH;         // ����®�� �ץ쥤�䡼���Ф��ƿ�ľ(V),��ʿ(H)
  double relVX,relVY;         // ����®�� �ե�����ɺ�ɸ��

  Point2D acc;                // ��®��

  Point2D velCorrect;         // �Υ���������
  double randp;               // �Υ�������

  double decay;

  double certainty;   // �ο���


/***************************************************************************/

  public ObjectInfo(double distance,double direction){
    dist = distance;
    dir = direction;
    change = Generic.NS_FALSE;

    pos = new Point2D.Double();
    vel = new Point2D.Double();
    acc = new Point2D.Double();
    velCorrect = new Point2D.Double();
    certainty = 1.0;
  }

/***************************************************************************/

  public ObjectInfo(double distance,double direction,
                    double distanceChange,double directionChange){
    dist = distance;
    dir = direction;
    distChange = distanceChange;
    dirChange = directionChange;
    change = Generic.NS_TRUE;

    // ��������
    pos = new Point2D.Double();
    vel = new Point2D.Double();
    acc = new Point2D.Double();
    velCorrect = new Point2D.Double();

    acc.setLocation(0,0);

    certainty = 1.0;
  }

/***************************************************************************/

  public void calcPos(double playerX, double playerY, double faceDir){
    double x,y;

    x = dist * Math.cos(Generic.digToRad(faceDir+dir)) + playerX;
    y = dist * Math.sin(Generic.digToRad(faceDir+dir)) + playerY;

    pos.setLocation(x , y);
  }

/***************************************************************************/

  public double getRelX(double faceDir){   // calc ��̵��
    return dist * Math.cos(Generic.digToRad(faceDir+dir));
  }

/***************************************************************************/

  public double getRelY(double faceDir){  // calc ��̵��
    return dist * Math.sin(Generic.digToRad(faceDir+dir));
  }

/***************************************************************************/

  public void calcRelVVH(){          // ����®�� VH ��ɸ
    double sinDir = Math.sin(Generic.digToRad(dir));
    double cosDir = Math.cos(Generic.digToRad(dir));

    relVV = distChange * cosDir  + (Math.PI/180.0) * dirChange * dist * sinDir;
    relVH = distChange * sinDir  + (Math.PI/180.0) * dirChange * dist * cosDir;

    //systemcout << "VV : " << relVV << endl;
    //systemcout << "VH : " << relVH << endl;
  }

/***************************************************************************/

  public void calcRelVXY(double faceDir){ // ����®�� XY ��ɸ
    double sinDir = Math.sin(Generic.digToRad(faceDir));
    double cosDir = Math.cos(Generic.digToRad(faceDir));

    relVX = relVV * cosDir - relVH * sinDir;
    relVY = relVV * sinDir + relVH * cosDir;
  }

/***************************************************************************/

  public void calcV(double playerVX,double playerVY){ // ����®�� XY ��ɸ
    vel.setLocation(relVX + playerVX , relVY + playerVY);
  }

/***************************************************************************/

  public void setTime(int t){
    seenTime = t;
  }

/***************************************************************************/

  public void setControlTime(int t){
    controlTime = t;
  }

/***************************************************************************/

  public void setPos(double X , double Y){
    pos.setLocation(X , Y);
  }

/***************************************************************************/

  public void setX(double X){
    pos.setLocation(X , pos.getY());
  }

/***************************************************************************/

  public void setY(double Y){
    pos.setLocation(pos.getX() , Y);
  }

/***************************************************************************/

  public void setVX(double VX){
    vel.setLocation(VX , vel.getY());
  }

/***************************************************************************/

  public void setVY(double VY){
    vel.setLocation(vel.getX() , VY);
  }

/***************************************************************************/

  public void setV(double VX , double VY){
    vel.setLocation(VX , VY);
  }

/***************************************************************************/

  public void setAX(double AX){      // ��®�٤򥻥å�
    acc.setLocation(AX , acc.getY());
  }

/***************************************************************************/

  public void setAY(double AY){
    acc.setLocation(acc.getX() , AY);
  }

/***************************************************************************/
  // �ܡ���򽳤ä��Ȥ��Ȥ��Ϥ�����Ȥ�
  public void setA(double AX , double AY){
    acc.setLocation(AX , AY);
  }

/***************************************************************************/

  public void setDecay(double d){
    decay = d;
  }

/***************************************************************************/

  public void setCertainty(double c){
    certainty = c;
    if(certainty < 0)
         certainty = 0.0;
  }

/***************************************************************************/

  public void setRand(double r){
    randp = r;
  }

/***************************************************************************/
  public double getCertainty(){
    return certainty;
  }

/***************************************************************************/

  public int getTime(){
    return seenTime;
  }

/***************************************************************************/

  public double getX(){
    return pos.getX();
  }

/***************************************************************************/

  public double getY(){
    return pos.getY();
  }

/***************************************************************************/

  public Point2D getPoint(){
    return pos;
  }

/***************************************************************************/

  public double getDist(){
    return dist;
  }

/***************************************************************************/

  public double getDir(){
    return dir;
  }

/***************************************************************************/

  public double getVX(){
    return vel.getX();
  }

/***************************************************************************/

  public double getVY(){
    return vel.getY();
  }

/***************************************************************************/

  public double getV(){
    return vel.distance(0,0);
  }

/***************************************************************************/

  public double getAX(){
    return acc.getX();
  }

/***************************************************************************/

  public double getAY(){
    return acc.getY();
  }

/***************************************************************************/

  public double getPredX(int t){
    double predX = pos.getX();
    double predVelX = vel.getX();

    for(int i = 0; i < t; i++){
      predVelX *= decay;
      predX += predVelX;
    }
    
    return predX;
  }

/***************************************************************************/

  public double getPredY(int t){
    double predY = pos.getY();
    double predVelY = vel.getY();

    for(int i = 0; i < t; i++){
      predVelY *= decay;
      predY += predVelY;
    }

    return predY;
  }

/***************************************************************************/

  public double getPredVX(int t){
    double predVX = vel.getX();

    for(int i = 0; i < t; i++){
			predVX *= decay;
    }

    return predVX;
  }

/***************************************************************************/

  public double getPredVY(int t){
    double predVY = vel.getY();

    for(int i = 0; i < t; i++){
			predVY *= decay;
    }

    return predVY;
  }

/***************************************************************************/

  public double getDistFrom(ObjectInfo obj){ // �Ϥ��줿 ObjectInfo �Ȥε�Υ
    return Math.sqrt(Math.pow(getX() - obj.getX(),2)
                      + Math.pow(getY() - obj.getY(),2));
  }

/***************************************************************************/

  public boolean hasChangeInfo(){
    return change;
  }

/***************************************************************************/

  public void calcNextState(){       // ���ξ��֤�׻�
    double ux,uy;  // �ޥ˥奢��Ǥ� ux^(t+1),uy^(t+1)

    ux = vel.getX() + acc.getX();
    uy = vel.getY() + acc.getY();

    pos.setLocation(pos.getX()+ux , pos.getY()+uy);

    vel.setLocation(ux * decay , uy * decay);

    acc.setLocation(0,0);

    certainty -= 0.1;

    if(certainty < 0)
	certainty = 0;

    seenTime++;
    controlTime++;  //  <- ���λȤ�ʬ�������ա�
  }

/***************************************************************************/


  public boolean isInSight(double X,double Y,double headFaceDir,
                                              String angleWidth){
    double absDir = Generic.RADIAN * Math.atan2(pos.getY() - Y,pos.getX() - X);
    double width;
    if(!angleWidth.equals("wide")){
	width = 180;
    } else if(angleWidth.equals("normal")){
	width = 90;
    } else {
	width = 45;
    }

    double dir1 = Generic.normDir(headFaceDir + width / 2);
    double dir2 = Generic.normDir(headFaceDir - width / 2);

    if(dir1 >= dir2){
	if(dir1 >= absDir && absDir >= dir2){
	    return Generic.NS_TRUE;
	}
    } else {
	if(dir1 >= absDir || absDir >= dir2){
	    return Generic.NS_TRUE;
	}
    }

    return Generic.NS_FALSE;
  }

/***************************************************************************/

  // 6/14 �ɲá����֥������Ȥ�����®�٤��֤���XY��ɸ��
  public double getRelV(){
    return Math.sqrt(relVX * relVX + relVY * relVY);
  }

/***************************************************************************/

  public int getSeenTime(){
    return seenTime;
  }

/***************************************************************************/
  // �Υ����������
  public void RemoveNoise(){
    double x1 , x2;
    double y1 , y2;
    Point2D v1 , v2 , v3 , v4;
    Point2D max , min;

    x1 = vel.getX() + velCorrect.getX();
    x2 = vel.getX() - velCorrect.getX();
    y1 = vel.getY() + velCorrect.getY();
    y2 = vel.getY() - velCorrect.getY();

    v1 = new Point2D.Double(x1 , y1);
    v2 = new Point2D.Double(x1 , y2);
    v3 = new Point2D.Double(x2 , y1);
    v4 = new Point2D.Double(x2 , y2);

    max = new Point2D.Double(Math.max(x1 + v1.distance(0,0) * randp ,
				      x1 + v2.distance(0,0) * randp) ,
			     Math.min(y1 + v1.distance(0,0) * randp ,
				      y1 + v3.distance(0,0) * randp));

    min = new Point2D.Double(Math.max(x2 - v3.distance(0,0) * randp ,
				      x2 - v4.distance(0,0) * randp) ,
			     Math.min(y2 - v2.distance(0,0) * randp ,
				      y2 - v4.distance(0,0) * randp));

    vel.setLocation((max.getX() + min.getX()) / 2.0 ,
		    (max.getY() + min.getY()) / 2.0);

    velCorrect.setLocation((max.getX() - min.getX()) / 2.0 ,
			   (max.getY() - min.getY()) / 2.0);
  }

/***************************************************************************/

  public boolean getCertainty2(){
    if (certainty < min_conf){
      return false;
    }
    return true;
  }

/***************************************************************************/

  public double getCertainty1(){
    return certainty;
  }

/***************************************************************************/

  // ������֤��֤�
  public int getControlTime(){
    return controlTime;
  }
}









