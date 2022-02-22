// Kakitsubata2000.java

import java.net.*;
import genericP.*;
import info_managerP.*;
import playerP.*;
import brainP.*;

/********************************************************/
/*         Kakitsubata2000   サッカーエージェント       */
/********************************************************/

public class Kakitsubata{

    static int formation = -1;
    static double dashPower = 1.0;
    static double kickPower = 1.0;
    static PositionTable positionTable = new PositionTable();

    static Player player;
    static Brain brain;
    static int formationSet = 4;

/***************************************************************************/

  public static void main(String args[]){
    String teamname;
    String hostname;
  
    teamname = "Kakitsubata";
    hostname = "localhost";

    int port = 6000;
    int uniformNum = 0;
    boolean goalie = false;
    boolean reconnect = false;
    boolean learning = false;
    boolean testAgent = false;
    String tablename = "";
    float version = (float)6.06;           // デフォルトはver6.00

    int i = 0;

    String buff = "";
    String seeInfo;
    String bodyInfo;
    String hearInfo;

    boolean typeSet = false;
    int typeID = 0;

    boolean setPlayer = false;
    double startX = 0,startY = 0,stdX = 0,stdY = 0;

    while(i < args.length){
      if(args[i].equals("-g")){
        goalie = true;
        i += 1;
      }
      else if(args[i].equals("-OZ")){
       // -OZ for RoboCup2000
       teamname = "OZ";
       setPlayer = true;
       typeSet = true;
       typeID = 3;         // MF
       startX = -12;
       startY = -15;
       stdX = -5;
       stdY = -12;

       i += 1;
      }

      if(i + 1 < args.length){
	if(args[i].equals("-t")){
	  teamname = args[i + 1];
	  i += 2;
	}
	else if(args[i].equals("-h")){
	  hostname = args[i + 1];
	  i += 2;
	}
	else if(args[i].equals("-p") &&
                            Character.isDigit(args[i+1].charAt(0))){
	  port = Integer.valueOf(args[i + 1]).intValue();
	  i += 2;
	}
	else if(args[i].equals("-v") &&
                            Character.isDigit(args[i+1].charAt(0))){
	  version = Float.valueOf(args[i + 1]).floatValue();
	  i += 2;
	}
	else if(args[i].equals("-r") &&
                            Character.isDigit(args[i+1].charAt(0))){
	  reconnect = true;
	  uniformNum = Integer.valueOf(args[i + 1]).intValue();
	  i += 2;
	}
        else if(args[i].equals("-s") && 
                                Character.isDigit(args[i+1].charAt(0))){
	// プレイヤータイプをオプションで決定
	  typeSet = true;
	  typeID = Integer.parseInt(args[i+1]);
	  if(typeID < 1 || typeID > 5){
	    System.out.println("s option's argument is not available !!");
	    System.exit(1);
	  };
	  i += 2;
        }
        else if(args[i].equals("-f") &&
                                Character.isDigit(args[i+1].charAt(0))){
	  typeSet = true;
	  formation = Integer.parseInt(args[i+1]);
	  System.out.println("Formation : " + formation);
	  i += 2;
        }
        else if(args[i].equals("-d") && 
                                Character.isDigit(args[i+1].charAt(0))){
    	  dashPower = Double.parseDouble(args[i+1]);
	  System.out.println("dashPower : " + dashPower);
	  i += 2;
        }
        else if(args[i].equals("-k") && 
                                Character.isDigit(args[i+1].charAt(0))){
  	  kickPower = Double.parseDouble(args[i+1]);
	  System.out.println("kickPower : " + kickPower);
	  i += 2;
        }
        else if(args[i].equals("-x") && 
                                Character.isDigit(args[i+1].charAt(0))){
	  // -x [type] [startX] [startY] [stdX] [stdY]
	  setPlayer = true;
	  typeSet = true;
	  typeID = Integer.parseInt(args[i+1]);
          startX = Double.parseDouble(args[i+1]);
          startY = Double.parseDouble(args[i+1]);
          stdX = Double.parseDouble(args[i+1]);
          stdY = Double.parseDouble(args[i+1]);
	  if(typeID < 1 || typeID > 5){
	    System.out.println("s option's argument is not available !!");
	    System.exit(1);
	  };
	  i += 6;
        }
      }
    }

    if(reconnect)
      player = new Player(teamname,uniformNum,hostname,port);
    else
      player = new Player(teamname,hostname,port,version,goalie);

    uniformNum = player.getUniformNum();

    if(dashPower != 1.0){
      player.setDashPowerRate(dashPower);
    }
    if(kickPower != 1.0){
      player.setKickPowerRate(kickPower);
    }

    if(!typeSet){
      if(uniformNum == 1){
	selectPlayerType(1);
      }
      else if(2 <= uniformNum && uniformNum <= 5){
        selectPlayerType(2);
      }
      else if(6 <= uniformNum && uniformNum <= 8){
	selectPlayerType(3);
      }
      else if(9 <= uniformNum && uniformNum <= 9){
	selectPlayerType(4);
      }
      else if(10 <= uniformNum && uniformNum <= 11){
	selectPlayerType(5);
      }
      else{
	brain = new Brain(player);
	System.out.println("Too Much Player !!");
	System.exit(1);
      }
    }
    else if(setPlayer){
      selectPlayerType(typeID);
      brain.setPosition(startX,startY,stdX,stdY);
    }
    else if(formation >-1 && formation < formationSet){ // Formation Section!!
      if(positionTable.role[formation][uniformNum-1].equals("GK")){
	selectPlayerType(1);
	brain.setPosition(formation,uniformNum);
      }
      else if(positionTable.role[formation][uniformNum-1].equals("DF")){
	selectPlayerType(2);
	brain.setPosition(formation,uniformNum);
      }
      else if(positionTable.role[formation][uniformNum-1].equals("MF")){
	selectPlayerType(3);
	brain.setPosition(formation,uniformNum);
      }
      else if(positionTable.role[formation][uniformNum-1].equals("OH")){
	selectPlayerType(4);
	brain.setPosition(formation,uniformNum);
      }
      else if(positionTable.role[formation][uniformNum-1].equals("FW")){
	selectPlayerType(5);
	brain.setPosition(formation,uniformNum);
      }
      else{
	brain = new Brain(player);
	System.out.println("Too Much Player !!");
	System.exit(1);
      }      
    }
    else{
      selectPlayerType(typeID+1);
    }
 

    for(;;){	
      buff = player.receiveMessage();

      if(buff.startsWith("(see")){
        seeInfo = buff;

        player.updateSeeInfo(seeInfo);

      }
      else if(buff.startsWith("(sense")){
        bodyInfo = buff;

	player.updateEtc();

        player.updateBodyInfo(bodyInfo);

        brain.decideCommand();

        player.sendCommand();

        // player.updateEtc();
      }
      else if(buff.startsWith("(hear")){

        hearInfo = buff;

        /*
        if(uniformNum == 10)
          System.out.println(buff);
	*/

        player.updateHearInfo(hearInfo);
      }

    }
  }

/***************************************************************************/
  // プレイヤータイプの割り当て

  static void selectPlayerType(int typeID){
    if(typeID == 1){
      brain = new Defender_GSRwithC(player);
    }
    else if(typeID == 2){
      brain = new DefenderSR(player);
    }
    else if(typeID == 3){
      brain = new IntercepterSR(player);
    }
    else if(typeID == 4){
      brain = new OffensiveHalfSR(player);
    }
    else if(typeID == 5){
      brain = new BomberSR(player);
    }
    else{
      brain = new Brain(player);
      System.out.println("typeID Illigal !! " + typeID);
      System.exit(1);
    }
  }

/***************************************************************************/

  static void printUsage(){
    System.out.println("Usage: player [options]");
    System.out.println("options: -p port");
    System.out.println("         -h hostname");
    System.out.println("         -t teamname");
    System.out.println("         -v version");
    System.out.println("         -r n (reconnect as nth player)");
    System.out.println("         -g   (connect as a goalie)");
  }

}
