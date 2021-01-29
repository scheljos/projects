// Battleboats by Joe Schelske
// schel351
// CSCI1933

import java.util.Scanner;

public class Main {

  private static boolean skip = false;

  public static void main(String args[]) {
    System.out.println("■■■ ■■■    ■■■"); // fun graphic
    System.out.println("---Battleboats Console---"); // Summary and instructions follow:
    System.out.println("This game is like Battleship. Your goal is to sink all the ships. Each turn, enter which location you want to attack when prompted.");
    System.out.println("One difference: you can call a drone to reveal the squares in the same row and the same column as the square you choose, in a T pattern.");
    System.out.println("Use your drone capabilities wisely, as it will cost you four turns.");
    System.out.println("All boats have a length of three.");
    System.out.println("Your goal is to beat the game in as few turns as possible. Good luck!");
    System.out.println("Battleboats will set up a board for you with your specified dimensions.");
    System.out.println("Acceptable board sizes range from 3 x 3 to 12 x 12.");
    Scanner scanner = new Scanner(System.in); // Scanner object for user inputs
    System.out.println("Please enter the number of rows you want on the board: "); // rows for BattleboatsBoard constructor
    int mRows = scanner.nextInt();
    System.out.println("Next, please enter the number of columns: "); // columns for constructor
    int nColumns = scanner.nextInt();
    BattleboatsBoard board = new BattleboatsBoard(mRows, nColumns); // make a new BattleboatsBoard object
    board.fillBattleboatsBoard(); // Fill the board with boats
    board.fillShownBoard(); // Fill the board for display purposes during the game (the one the user sees)
    System.out.println("Debug mode? Y/N: ");
    String yesNo = scanner.next();
    boolean debug = false;
    if (yesNo.equals("Y") || yesNo.equals("y")) { // See if the program is to run in debug mode, in which case the board filled with boats is printed instead of the regular display game board.
      debug = true;
    }
    System.out.println("Number of 3-unit boats in the game: " + board.getHowManyBoats());
    System.out.print("\n");
    while (board.checkBoard() > 0) { // runs checkBoard function which checks to see if there are still any boats in the real battleboard.
      System.out.println("Turn count: " + board.getTurnCount()); // counts turns each loop
      System.out.print("\n");
      if (!(debug)) {
        board.printShownBoard(); // if not debug, print regular display board
      } else {
        board.printRealBoard(); // if debug, print actual game board with boat locations visible
      }
      if (skip == true) { // if a penalty, skips a turn of the loop
        skip = false;
        continue;
      }
      skip = board.getSkipTurn();
      if (!(skip)) { // if there's no penalty, run the game as usual
        System.out.println("Would you like a drone to reveal an area? Y/N: ");
        String noYes = scanner.next();
        boolean drone = false;
        if (noYes.equals("Y") || noYes.equals("y")) {
          drone = true; // ask for drone, set boolean appropriate for the user's input
        }
        System.out.println("Enter the x (horizontal) position you want to attack: "); // spot they want to attack, X coordinate
        int attackX = scanner.nextInt();
        System.out.println("Enter the y (vertical) position you want to attack: "); // spot they want to attack, Y coordinate
        int attackY = scanner.nextInt();
        if ((attackX <= nColumns - 1 && attackX >= 0) && (attackY <= mRows - 1 && attackY >= 0)) { // if the scanner input is within bounds...
          if (drone) {
            System.out.println("Drone searching..."); // calls drone function which reveals boats, putting them on the display board if the boolean was made true
            board.drone(attackX, attackY);
          } else {
            System.out.println("Cannon fire!"); // regular cannon fire. Calls hitOrMiss function
            board.hitOrMiss(attackX, attackY);
          }
        } else { // ... otherwise give a penalty
          System.out.println("Penalty! Out of bounds.");
          skip = true;
        }
      }
    }
    board.printShownBoard(); // prints out the final game board and says goodbye
    System.out.println("\n");
    System.out.println("Final boat sunk!");
    System.out.println("Final turn count: " + board.getTurnCount());
    System.out.println("Thanks for playing!");
    scanner.close();
  }
}
