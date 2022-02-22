// StrategyInfo.java

package objectP;

/********************************************************/
/*         StrategyInfo  �����헪���                   */
/********************************************************/

public class StrategyInfo{
  final double conf_decay = 0.05;          //  �����헪�̌����x
  final double min_conf = 0.1;

  int controlTime;                        // ���̃I�u�W�F�N�g�����䂳�ꂽ����

  String strategy;
  String type;
  int uniformNum; 
  double certainty;                        // ���̐M���x

  // �V�o�[�W�����ł͍폜�\��
  public static final double S_less = 0.05;     //  �����헪�̌����x
  double x, y;

/***************************************************************************/

  public StrategyInfo(){
    strategy = "unknown";
    type = "norm";
    uniformNum = 0;
    certainty = 0.0;
  }

/***************************************************************************/

  public StrategyInfo(int uNum, String S_name, String tp){
    strategy = S_name;
    uniformNum = uNum;
    type = tp;
    certainty = 1.0;
  }

/***************************************************************************/

  // �R���X�g���N�^�ł������o�^���o���Ȃ��̂̓}�Y�C�̂�
  public void setStrategy(int Time , String S_name, String tp){
    controlTime = Time;
    strategy = S_name;
    type = tp;
    certainty = 1.0;
  }

/***************************************************************************/

  // �R���X�g���N�^�ł������o�^���o���Ȃ��̂̓}�Y�C�̂�
  public void setStrategy(String S_name, String tp){
    strategy = S_name;
    type = tp;
    certainty = 1.0;
  }

/***************************************************************************/

  public void setStrategyName(String S_name){
    strategy = S_name;
  }

/***************************************************************************/

  public void setStrategyType(String Stp){
    type = Stp;
  }

/***************************************************************************/

  public void setUniformNum(int uNum){
    uniformNum = uNum;
  }

/***************************************************************************/

  public void setCertainty(double c){
    certainty = c;
    if(certainty < 0)
      certainty = 0.0;
  }

/***************************************************************************/

  // ���̐i�s
  public void advanceStrategyInfo(int filedTime){
    if (filedTime == controlTime){
      // �i�s����K�v�Ȃ�
      return;
    }

    // ���������s����鎞�͕K�� 1 �b�̂���͐����Ă���̂�
    calcNextState(); 
    
    // ��{�I�� 2 �b�ȏ㗣��Ȃ��͂������O�̈�
    while((filedTime - controlTime) != 0){
      calcNextState();
    }
  }
	
/***************************************************************************/

  public void calcNextState(){       // ���̏�Ԃ��v�Z
    certainty -= conf_decay;
    controlTime++;
    
    if(certainty < 0){
      certainty = 0.0;
      strategy = "unknown";
      type = "norm";
    }
  }

/***************************************************************************/

  public void refresh(int t){
    controlTime = t;
    strategy = "unknown";
    type = "norm";
    certainty = 0.0;
  }

/***************************************************************************/

  public String getStrategy(){
    return strategy;
  }

/***************************************************************************/

  public int getUniformNum(){
    return uniformNum;
  }

/***************************************************************************/

  public String getType(){
    return type;
  }

/***************************************************************************/

  public double getCertainty1(){
    return certainty;
  }

/***************************************************************************/

  public boolean getCertainty2(){
    if (certainty < min_conf){
      return false;
    }
    return true;
  }

/***************************************************************************/

  // ���䎞�Ԃ�Ԃ�
  public int getControlTime(){
    return controlTime;
  }    

/***************************************************************************/

  public String toStringStrategyInfo(){
    StringBuffer strBuff = new StringBuffer("(NIT_strategy ");
    strBuff.append(strategy).append(" ")
           .append(uniformNum).append(" ").append(type).append(")");
    return strBuff.toString();
  }


/***************************************************************************/
/*                        �V�o�[�W�����ł͍폜�\��                         */
/***************************************************************************/

/***************************************************************************/

  public void setPos(double X, double Y){
    x = X;
    y = Y;
  }

/***************************************************************************/

  public double getX(){
    return x;
  }

/***************************************************************************/

  public double getY(){
    return y;
  }

/***************************************************************************/

  public double getCertainty(){
    return certainty;
  }



}

