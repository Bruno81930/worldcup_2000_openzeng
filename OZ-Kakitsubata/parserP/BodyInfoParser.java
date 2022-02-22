// BodyInfoParser.java

package parserP;

import java.util.*;
import genericP.*;
import objectP.*;
import stokenizerP.*;

/********************************************************/
/*         BodyInfoParser  sense_body 情報の解析        */
/********************************************************/

public class BodyInfoParser{
    String bodyInfo;

    int time;

    // ビューモード
    String angleWidth;
    String quality;
    
    // スタミナ
    double stamina;
    double effort;
    
    double speed;
    double speed_ang;       // プレーヤの視野に関する角度 ver6.00より追加
    double headAngle;

    // カウンタ
    int kickCount;
    int dashCount;
    int turnCount;
    int sayCount;
    int turnNeckCount;

/***************************************************************************/

  public BodyInfoParser(){

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println("Creating BodyInfoParser.");
    }
    */

  }

/***************************************************************************/

  public BodyInfoParser(String bi){

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println("Creating BodyInfoParser.");
    }
    */

    setInfo(bi);
    parseInfo();
  }

/***************************************************************************/
    
  public void setInfo(String bi){
    this.bodyInfo = bi;
  }

/***************************************************************************/

  public void parseInfo(){
    STokenizer tokenizer = new STokenizer(this.bodyInfo);
    String token;

    token = tokenizer.nextSToken();

    /*
    if(Generic.DEBUG){   // デバッグ用
      if(!token.equals("sense_body")){
        System.out.println("BodyInfoParser::parseFlagInfo() Illegal Format!");
        System.out.println("bodyInfo");
        return;
      }
    */

    this.time = Integer.valueOf(tokenizer.nextSToken()).intValue();

    // 一応場合分けしてるけど本当は決めうちでいいはず
    while(tokenizer.hasMoreSTokens()){
	token = tokenizer.nextSToken();
	STokenizer subTokenizer = new STokenizer(token);

	// このあたりは文字で判定すれば高速化出来るし case に出来る
	// でも、可読性が低くなるので最後の手段に
	// 基本的にコメントなしでもプログラムは読めなければダメ

    token = subTokenizer.nextSToken();
    if(token.equals("view_mode")){
      this.quality = subTokenizer.nextSToken();
      this.angleWidth = subTokenizer.nextSToken();
    }
    else if(token.equals("stamina")){
      this.stamina = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
      this.effort = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
    }
    else if(token.equals("speed")){
      this.speed = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
      this.speed_ang = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
    }
    else if(token.equals("head_angle")){
      this.headAngle = Double.valueOf(subTokenizer.nextSToken()).doubleValue();
    }
    else if(token.equals("kick")){
      this.kickCount = Integer.valueOf(subTokenizer.nextSToken()).intValue();
    }
    else if(token.equals("dash")){
      this.dashCount = Integer.valueOf(subTokenizer.nextSToken()).intValue();
    }
    else if(token.equals("turn")){
      this.turnCount = Integer.valueOf(subTokenizer.nextSToken()).intValue();
    }
    else if(token.equals("say")){
      this.sayCount = Integer.valueOf(subTokenizer.nextSToken()).intValue();
    }
    else if(token.equals("turn_neck")){
      this.turnNeckCount = Integer.valueOf(subTokenizer.nextSToken()).
                                                                 intValue();
    } else {

      /*
      if(Generic.DEBUG){   // デバッグ用
        System.out.println("BodyInfoParser::parseFlagInfo() Illegal Format!");
        System.out.println(token);
      }
      */

      return;
      }
    }
  }

/***************************************************************************/

  public int getTime(){
    return this.time;
  }

/***************************************************************************/

  public String getAngleWidth(){
    return this.angleWidth;
  }

/***************************************************************************/

  public String getQuality(){
    return this.quality;
  }

/***************************************************************************/

  public double getStamina(){
    return this.stamina;
  }

/***************************************************************************/

  public double getEffort(){
    return this.effort;
  }

/***************************************************************************/

  public double getSpeed(){
    return this.speed;
  }

/***************************************************************************/

  public double getHeadAngle(){
    return this.headAngle;
  }

/***************************************************************************/

  public int getKickCount(){
    return this.kickCount;
  }

/***************************************************************************/

  public int getDashCount(){
    return this.dashCount;
  }

/***************************************************************************/

  public int getTurnCount(){
    return this.turnCount;
  }

/***************************************************************************/

  public int getSayCount(){
    return this.sayCount;
  }

/***************************************************************************/

  public int getTurnNeckCount(){
    return this.turnNeckCount;
  }

/***************************************************************************/

  public void printInfo(){
    System.out.println("************* BodyInfo *************");
    System.out.println("Time : " + this.time);
    System.out.println("AngleWidth : " + this.angleWidth);
    System.out.println("Quality : " + this.quality);
    System.out.println("Stamina : " + this.stamina);
    System.out.println("Effort : " + this.effort);
    System.out.println("Speed : " + this.speed );
    System.out.println("HeadAngle : " + this.headAngle);
  }

}


