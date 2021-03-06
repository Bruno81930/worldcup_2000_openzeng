// HearInfoParser.java

package parserP;

import java.util.*;
import genericP.*;
import objectP.*;
import stokenizerP.*;

/********************************************************/
/*        HearInfoParser          聴覚情報の解析        */
/********************************************************/

public class HearInfoParser implements PlaymodeTable{
  final int FROM_UNKNOWN = 0;
  final int FROM_REFEREE = 1;
  final int FROM_TEAMMATE = 2;
  final int FROM_COACH = 3;

  String hearInfo;
  String side;
  String teamname;

  String message;

  int time;
  int playmode;
  int from;
    
  int ourPoint,opponentsPoint;
    
/***************************************************************************/

  public HearInfoParser(){

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println("Creating HearInfoParser.");
    }
    */

    this.ourPoint = this.opponentsPoint = 0;
  }

/***************************************************************************/

  public HearInfoParser(String hi,String s,String tname){

    /*
    if(Generic.DEBUG){   // デバッグ用
      System.out.println("Creating HearInfoParser.");
    }
    */

    this.ourPoint = this.opponentsPoint = 0;

    setInfo(hi);
    setSide(s);
    setTeamname(tname);
    parseInfo();
  }

/***************************************************************************/

  public void setInfo(String hi){
    this.hearInfo = hi;
  }

/***************************************************************************/

  public void setSide(String si){
    this.side = si;
  }

/***************************************************************************/

  public void setTeamname(String tname){
    this.teamname = tname;
  }
   
/***************************************************************************/

  public void parseInfo(){
    String token;

    STokenizer tokenizer = new STokenizer(this.hearInfo);
    token = tokenizer.nextSToken();

    /* DEBUG用
    if(Generic.DEBUG){ 
      if(!token.equals("hear")){
        System.out.println("HearInfoParser.parseInfo() Illegal Format!");
        return;
      }
    }
    */

    token = tokenizer.nextSToken();
    this.time = Integer.valueOf(token).intValue();

    token = tokenizer.nextSToken();

    if(token.equals("referee")){
      this.from = FROM_REFEREE;
      token = tokenizer.nextSToken();

      if(token.startsWith("before_kick_off")){
        this.playmode = PM_BEFORE_KICK_OFF;
      }
      else if(token.startsWith("kick_off_")){
        if(token.endsWith(this.side))
          this.playmode = PM_OUR_KICK_OFF;
        else
          this.playmode = PM_OPPONENTS_KICK_OFF;
      }
      else if(token.startsWith("kick_in_")){
        if(token.endsWith(this.side))
          this.playmode = PM_OUR_KICK_IN;
	else
	  this.playmode = PM_OPPONENTS_KICK_IN;
      }
      else if(token.startsWith("corner_kick_")){
        if(token.endsWith(this.side))
          this.playmode = PM_OUR_CORNER_KICK;
	else
	  this.playmode = PM_OPPONENTS_CORNER_KICK;
      }
      else if(token.startsWith("goal_kick_")){
        if(token.endsWith(this.side))
          this.playmode = PM_OUR_GOAL_KICK;
	else
	  this.playmode = PM_OPPONENTS_GOAL_KICK;
      }
      else if(token.startsWith("free_kick_")){
        if(token.endsWith(this.side))
          this.playmode = PM_OUR_FREE_KICK;
        else
	  this.playmode = PM_OPPONENTS_FREE_KICK;
      }
      else if(token.startsWith("offside_")){
        if(token.endsWith(this.side))
	  this.playmode = PM_OUR_OFFSIDE;
	else
	  this.playmode = PM_OPPONENTS_OFFSIDE;
      }
      else if(token.startsWith("play_on")){
        this.playmode = PM_PLAY_ON;
      }
      else if(token.startsWith("half_time")){
        this.playmode = PM_HALF_TIME;
      }
      else if(token.startsWith("time_up")){
        this.playmode = PM_TIME_UP;
      }
      else if(token.startsWith("time_extended")){
        this.playmode = PM_EXTEND;
      }
      else if(token.startsWith("foul_")){
        if(token.endsWith(this.side))
          this.playmode = PM_OUR_FOUL;
        else
	  this.playmode = PM_OPPONENTS_FOUL;
      }
      else if(token.startsWith("goal_")){
        if(token.substring(5,6).equals(this.side)){
          this.playmode = PM_OUR_GOAL;
	  this.ourPoint = Integer.valueOf(token.substring(7,8)).intValue();
	}
        else {
	  this.playmode = PM_OPPONENTS_GOAL;
          this.opponentsPoint =
                          Integer.valueOf(token.substring(7,8)).intValue();
	}
      }
    }
    else if(token.equals("self")){
      // とりあえず捨てとけばいいか
    }
    else if(token.startsWith("online_coach_")){
      if(token.startsWith(this.side,13)){
	this.from = FROM_COACH;
	this.message = tokenizer.nextSToken();
      }
    }
    else {
      token = tokenizer.nextSToken();

      if(token.equals(teamname)){
	this.from = FROM_TEAMMATE;
        this.message = tokenizer.nextSToken();
      }
    }
  }

/***************************************************************************/

  public int getTime(){
    return this.time;
  }

/***************************************************************************/

  public int getPlaymode(){
    return this.playmode;
  }

/**************************************************************************/

 public boolean fromCoach(){
    if(this.from == FROM_COACH)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }

/***************************************************************************/

  public boolean fromReferee(){
    if(this.from == FROM_REFEREE)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }

/***************************************************************************/

  public boolean fromTeammate(){
    if(this.from == FROM_TEAMMATE)
      return Generic.NS_TRUE;
    else
      return Generic.NS_FALSE;
  }
/***************************************************************************/

  public String getMessage(){
    return this.message;
  }
    
/***************************************************************************/

  public int getOurPoint(){
    return this.ourPoint;
  }

/***************************************************************************/

  public int getOpponentsPoint(){
    return opponentsPoint;
  }
    
}
