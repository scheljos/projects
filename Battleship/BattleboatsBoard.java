// Battleboats by Joe Schelske
// schel351
// CSCI1933

public class BattleboatsBoard {

  private String[][] battleboard; // main board with boats, coordinates, used in debug mode. Printing this board shows everything
  private String[][] gameboard; // used for display purposes to hide the main array from players
  private int boatCount; // how many boats the battleboard should have on it, initialized in the constructor along with the boards.
  private int howManyBoats; // initialized in the constructor -- I used this variable only in main, to print out the number of boats on the board.
  private int row, column; // used to size the boards
  private String globalBoat = "   ■   "; // string variable that is the same as a third of a boat. I chain three squares together in the battleboard array and call that a boat. I check if this gets hit.
  private int turnCount = 0; // counts the number of times the user takes to sink all the boats, + penalties
  private boolean skipTurn = false; // skipTurn is a boolean used to skip a turn in the case of a penalty
  private String globalHit = "   X   "; // used in drone to check if spot has been hit. If it's hit, I don't want to reveal a boat.

  public BattleboatsBoard(int row, int column) {
    // If bounds are appropriate, initialize the array
    if ((row >= 3 && row <= 12) && (column >=3 && column <= 12)) {
      battleboard = new String[row][column];
      this.row = row;
      this.column = column;
    }
    // If not, print error message
    else {
      battleboard = new String[6][6];
      System.out.println("Invalid input; board initialized to 6 x 6");
      row = 6;
      column = 6;
    }
    // Setting up boatCount with the number of boats to use in fill...Board()
    if (row == 3 || column ==3) {
      boatCount = 1;
    } else if ((row > 3 && row <= 6) || (column > 3 && column <= 6)) {
      boatCount = 2;
    } else if ((row > 6 && row <= 8) || (column > 6 && column <= 8)) {
      boatCount = 3;
    } else if ((row > 8 && row <= 10) || (column > 8 && column <=10)) {
      boatCount = 4;
    } else {
      boatCount = 6;
    }
    howManyBoats = boatCount;
    gameboard = new String[row+1][column+1]; // also construct gameboard, used for display purposes in the game. Is +1 in length to accomodate numbers on the left and top.
  } // BattleboatsBoard

  public void fillBattleboatsBoard() {
    String boat = "   ■   "; // Used to add squares to coordinates with boats
    // Sets values in battleboard to the coordinates of the spaces
    for (int i = 0; i < battleboard.length; i++) {
      for (int j = 0; j < battleboard[i].length; j++) {
        battleboard[i][j] = "(" + j + ", " + i + ") ";
      }
    }
    // Places boats on the board (chooses hotizontal, chooses location)
    for (int i = 0; i < boatCount; i++){
      // "Coinflip" for random assignment of vertical or horizontal for the boat
      boolean vert = false;
      boolean horiz = false;
      double verticalHorizontal = Math.random(); // used for coinflip
      int randomX = (int)Math.floor(Math.random() * 10); // random int between 0 and 10 -- what column to put the random boat in
      if (randomX > battleboard[0].length - 1) { // battleboard[0] is the length of the row, or how many columns there are (1 greater than last index)
        randomX = randomX - (randomX - battleboard[0].length + 1); // makes the coordinate within the acceptable bounds of # of columns
      }
      int randomY = (int)Math.floor(Math.random() * 10); // what row to put the random boat in
      if (randomY > battleboard.length - 1) {
        randomY = randomY - (randomY - battleboard.length + 1); // makes coordinate within acceptable bounds of # of rows
      }
      if (verticalHorizontal >= 0.5) { // coinflip that determines horizontal or vertical placement
        vert = true;
      } else {
        horiz = true;
      }
      if (!(battleboard[randomY][randomX].equals(boat))) { //checks for a boat in the first randomly generated spot
        battleboard[randomY][randomX] = boat; // first index of array selects the row, second selects the column. Puts 1/3 of boat in at random spot
      } else {
        boatCount++;
        continue;
      }
         if (vert) {
           if (randomY == battleboard.length - 1) {
             if (!(battleboard[randomY - 1][randomX].equals(boat)) && !(battleboard[randomY - 2][randomX].equals(boat))) {
               battleboard[randomY - 1][randomX] = boat;
               battleboard[randomY - 2][randomX] = boat;
             } else {
               battleboard[randomY][randomX] = "(" + randomX + ", " + randomY + ") "; // resets the space in the array to what it was
               boatCount++; // failed to make a boat because another boat blocked it, add another boat and try again.

             }
           } else if (randomY == 0) {
             if (!(battleboard[randomY + 1][randomX].equals(boat)) && !(battleboard[randomY + 2][randomX].equals(boat))) {
               battleboard[randomY + 1][randomX] = boat;
               battleboard[randomY + 2][randomX] = boat;
             } else {
               battleboard[randomY][randomX] = "(" + randomX + ", " + randomY + ") ";
               boatCount++;
             }
           } else {
             if (!(battleboard[randomY + 1][randomX].equals(boat)) && !(battleboard[randomY - 1][randomX].equals(boat))) {
               battleboard[randomY + 1][randomX] = boat;
               battleboard[randomY - 1][randomX] = boat;
             } else {
               battleboard[randomY][randomX] = "(" + randomX + ", " + randomY + ") ";
               boatCount++;
             }

           }

         } else if (horiz) {
           if (randomX == battleboard[randomY].length - 1) {
             if (!(battleboard[randomY][randomX - 1].equals(boat)) && !(battleboard[randomY][randomX - 2].equals(boat))) {
               battleboard[randomY][randomX - 1] = boat;
               battleboard[randomY][randomX - 2] = boat;
             } else {
               battleboard[randomY][randomX] = "(" + randomX + ", " + randomY + ") ";
               boatCount++;
             }

           } else if (randomX == 0) {
             if (!(battleboard[randomY][randomX + 1].equals(boat)) && !(battleboard[randomY][randomX + 2].equals(boat))) {
               battleboard[randomY][randomX + 1] = boat;
               battleboard[randomY][randomX + 2] = boat;
             } else {
               battleboard[randomY][randomX] = "(" + randomX + ", " + randomY + ") ";
               boatCount++;
             }
           } else {
             if (!(battleboard[randomY][randomX - 1].equals(boat)) && !(battleboard[randomY][randomX + 1].equals(boat))) {
               battleboard[randomY][randomX - 1] = boat;
               battleboard[randomY][randomX + 1] = boat;
             } else {
               battleboard[randomY][randomX] = "(" + randomX + ", " + randomY + ") ";
               boatCount++;
             }
           }
         }
      }

  } //fillBattleboatsBoard

  // Prints out the real board, meaning the board containing boats and coordinates
  // use for debug mode. turnCount incremented when this function is called, which means every turn in debug mode.
  public void printRealBoard() {
    for (int i = 0; i < battleboard.length; i++) {
      if (i >= 1) {
        System.out.println("\n");
      }
      for (int j = 0; j < battleboard[i].length; j++) {
        System.out.print(battleboard[i][j]);
      }
    }
    System.out.print("\n");
    turnCount++;
  } // printRealBoard

  public void fillShownBoard() { // fills the display board with "water", or circles and makes a number grid.
    String water = "   ●   ";
    for (int i = 0; i < gameboard.length; i++) {
      for (int j = 0; j < gameboard[i].length; j++) {
        if (i == 0) {
          gameboard[i][j] = "   " + Integer.toString(j-1) + "   ";
        } else if (j == 0) {
          gameboard[i][j] = "   " + Integer.toString(i-1) + "   ";
        } else {
          gameboard[i][j] = water;
        }
      }
    }
    gameboard[0][0] = "       ";
  } // fillShownBoard

  public void printShownBoard() { // this is the non-debug mode board, this function is called repeatedly throughout the game to print out the current board. Prints board and updates turn count each time
    for (int i = 0; i < gameboard.length; i++) {
      if (i >= 1) {
        System.out.println("\n");
      }
      for (int j = 0; j < gameboard[i].length; j++) {
        System.out.print(gameboard[i][j]);
      }
    }
    System.out.print("\n");
    turnCount++;
  } // printShownBoard

  public void hitOrMiss(int strikeX, int strikeY) { // handles hitting and missing in the game, takes in ints from the scanner, updates the battleboard (true board with boats on it) and the display board (that the player sees)
    String hit = "   X   ";
    if (battleboard[strikeY][strikeX].equals(globalBoat)) {
      System.out.println("Hit!");
      battleboard[strikeY][strikeX] = hit;
      gameboard[strikeY+1][strikeX+1] = hit;
    } else if (battleboard[strikeY][strikeX].equals(hit)) {
      System.out.println("Penalty!");
      turnCount++;
      System.out.println("Punishment: turn count increased by 1.");
    } else {
      System.out.println("Miss!");
      battleboard[strikeY][strikeX] = "~empty~";
      gameboard[strikeY + 1][strikeX + 1] = "~empty~";
    }

  } // hitOrMiss

  public int checkBoard() { // used in main to end the program when there are no boats left in the array.
    int boatSquares = 0;
    for (int i = 0; i < battleboard.length; i++) {
      for (int j = 0; j < battleboard[i].length; j++) {
        if (battleboard[i][j].equals(globalBoat)) {
          boatSquares += 1;
        }
      }
    }
    return boatSquares;
  } // checkBoard

  public void drone(int droneX, int droneY) { //checks if there's boat objects in the row and column chosen by the users input, puts them in the display board.
    for (int i = 0; i <= battleboard.length - 1; i++) {
      if (battleboard[droneY][i].equals(globalBoat)) {
        if (!(gameboard[droneY + 1][i + 1].equals(globalHit))) {
          gameboard[droneY + 1][i + 1] = globalBoat;
        }
      }
    }
    for (int j = 0; j <= battleboard.length - 1; j++) {
      if (battleboard[j][droneX].equals(globalBoat)) {
        if (!(gameboard[j + 1][droneX + 1].equals(globalHit))) {
          gameboard[j + 1][droneX + 1] = globalBoat;
        }
      }
    }
    turnCount = turnCount + 4;
  } // drone


  public int getRow() {
    return this.row;
  } // getRow

  public int getColumn() {
    return this.column;
  } // getColumn

  public String getBoat() {
    return globalBoat;
  } // getBoat

  public int getHowManyBoats() {
    return howManyBoats;
  } // howManyBoats();

  public boolean getSkipTurn() {
    return skipTurn;
  } //skipTurn

  public int getTurnCount() {
    return turnCount;
  } // turnCount
} // class
