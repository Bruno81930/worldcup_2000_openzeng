// StrategyInfo.java

package objectP;

/********************************************************/
/*         StrategyInfo  協調戦略情報                   */
/********************************************************/

public class StrategyInfo{
  final double conf_decay = 0.05;          //  協調戦略の減衰度
  final double min_conf = 0.1;

  int controlTime;                        // このオブジェクトが制御された時間

  String strategy;
  String type;
  int uniformNum; 
  double certainty;                        // 情報の信頼度

  // 新バージョンでは削除予定
  public static final double S_less = 0.05;     //  協調戦略の減衰度
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

  // コンストラクタでしか情報登録が出来ないのはマズイので
  public void setStrategy(int Time , String S_name, String tp){
    controlTime = Time;
    strategy = S_name;
    type = tp;
    certainty = 1.0;
  }

/***************************************************************************/

  // コンストラクタでしか情報登録が出来ないのはマズイので
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

  // 情報の進行
  public void advanceStrategyInfo(int filedTime){
    if (filedTime == controlTime){
      // 進行する必要なし
      return;
    }

    // ここが実行される時は必ず 1 秒のずれは生じているので
    calcNextState(); 
    
    // 基本的に 2 秒以上離れないはずだが念の為
    while((filedTime - controlTime) != 0){
      calcNextState();
    }
  }
	
/***************************************************************************/

  public void calcNextState(){       // 次の状態を計算
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

  // 制御時間を返す
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
/*                        新バージョンでは削除予定                         */
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

