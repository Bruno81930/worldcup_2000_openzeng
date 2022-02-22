// BodyInfoParser.java

package parserP;

import java.util.*;
import genericP.*;
import objectP.*;
import stokenizerP.*;

/********************************************************/
/*         BodyInfoParser  sense_body $B>pJs$N2r@O(B        */
/********************************************************/

public class BodyInfoParser{
    String bodyInfo;

    int time;

    // $B%S%e!<%b!<%I(B
    String angleWidth;
    String quality;
    
    // $B%9%?%_%J(B
    double stamina;
    double effort;
    
    double speed;
    double speed_ang;       // $B%W%l!<%d$N;kLn$K4X$9$k3QEY(B ver6.00$B$h$jDI2C(B
    double headAngle;

    // $B%+%&%s%?(B
    int kickCount;
    int dashCount;
    int turnCount;
    int sayCount;
    int turnNeckCount;

/***************************************************************************/

  public BodyInfoParser(){

    /*
    if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
      System.out.println("Creating BodyInfoParser.");
    }
    */

  }

/***************************************************************************/

  public BodyInfoParser(String bi){

    /*
    if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
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
    if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
      if(!token.equals("sense_body")){
        System.out.println("BodyInfoParser::parseFlagInfo() Illegal Format!");
        System.out.println("bodyInfo");
        return;
      }
    */

    this.time = Integer.valueOf(tokenizer.nextSToken()).intValue();

    // $B0l1~>l9gJ,$1$7$F$k$1$IK\Ev$O7h$a$&$A$G$$$$$O$:(B
    while(tokenizer.hasMoreSTokens()){
	token = tokenizer.nextSToken();
	STokenizer subTokenizer = new STokenizer(token);

	// $B$3$N$"$?$j$OJ8;z$GH=Dj$9$l$P9bB.2==PMh$k$7(B case $B$K=PMh$k(B
	// $B$G$b!"2DFI@-$,Dc$/$J$k$N$G:G8e$N<jCJ$K(B
	// $B4pK\E*$K%3%a%s%H$J$7$G$b%W%m%0%i%`$OFI$a$J$1$l$P%@%a(B

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
      if(Generic.DEBUG){   // $B%G%P%C%0MQ(B
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


