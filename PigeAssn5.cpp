/* ----------------------------------------------------------------------------
Author: Frank Pigeon
Class: CS362
Assignment: Week 5
Program helps keep track of the realtor's association homes for sale.
Processing: Uses array of records to store housing listings.
Output: Outputs the records in the file to screen and at the end asks
user if they want to commit changes to LISTINGS.txt
FUNCTIONS:	 main - calls other functions and initializes array of records			 
------------------------------------------------------------------------------*/

//Header File
#include <iostream> 
#include <iomanip> 
#include <cmath>  
#include <cctype> 
#include <fstream> 
#include <string>

using namespace std;

//global constants
const string DESCRIPTION = "This program keeps track of rental cars.",
             DEVELOPER = "Frank Pigeon",
             CLASSNUM = "CS362",
             ASSIGNMENT = "Week 5";
const string GARBAGE_FILENAME = "GARBAGE.BIN";  //binary file 
enum carType {COMPACT, MIDSIZE, FULLSIZE, SUV, NONE}; // listing car type options
const int MAX_CARS = 5; // maximum cars on a level
const int CAR_TYPES = 4; //types of cars
const int LEVELS = 6; //number of levels in the garage
const int EMPTY_CARS = 0;
const int ERROR_1 = 1; // unique error message
const int ERROR_2 = 2; // unique error message
const int ERROR_3 = 3; // unique error message
const int ALL_LVL_FULL = 30; // level is filled with cars
const int ALL_LVL_EMPTY  = 0; // level is empty        

//prototypes
void showDescription (string DESCRIPTION);
void fullGarage (int array[][LEVELS]);
void emptyGarage (int array[][LEVELS]);
char displayMenu ();
char checkMenuChoice (char& menuChoice);
void openBinFile (int garage[][LEVELS]);
char subMenu ();
int checkSubChoice (char& menuChoice);
void menuAction (char menuChoice, int array[][LEVELS], bool& quit);
string convertCarToString (int carList);
int parkWhere ();
int checkPark (int car_type, int& menuChoice, int array[][LEVELS]);
void printArray (int array[][LEVELS]);
void displayCars (int car_type, int array[][LEVELS], int& lineCount, string& carString);
int emptySpot (int car_type, int array[][LEVELS]);
void closeBinFile (int garage[][LEVELS]);

/******************************************************************************
//  FUNCTION:	  main
//  DESCRIPTION:  Calls other functions and intializes array of records
//  INPUT: File:  Reads the LISTINGS.TXT if user desires
//  CALLS TO:	  showDescription, loadExistingData, displayMenu, checkMenuChoice, menuAction
******************************************************************************/
int main()
{
    //local variables	
	int garage [CAR_TYPES] [LEVELS]; // create a two-dimensional array for the garage    
	char menuChoice;
	bool quit = false;
	
	showDescription (DESCRIPTION);  // oupt program desciption to screen	
	openBinFile (garage);    //open binary file and read existing data
	printArray (garage); //display array output to screen 
	do { //loop until EXIT is selected
		menuChoice = displayMenu ();  //prompt user for input from main menu
		checkMenuChoice (menuChoice); //error check user input
		menuAction (menuChoice, garage, quit); // take action based on user choice
	} while (quit == false); 	//exit only if flag is set to false
    return 0;
}  // end main

/*************************************************************************
  FUNCTION:	    menuAction
  DESCRIPTION:  calls functions based on user selected menu choice
  INPUT:		Parameters:	menuChoice -user input from menu choices
							housingList - array of records that stores listings
							listingCount - count of listings in the array of records
							quit - used to flag the end of the program
  OUTPUT: 	    Parameters:	listingCount - count of listings in the array of records
							quit - used to flag the end of the program
*************************************************************************/
void menuAction (char menuChoice, int array[][LEVELS], bool& quit)
{
 char commitChange, // user input to commit changes
	  subChoice; // user input from sub menu
 int carChoice = 4, //converted int from char
	 levelTotal = 0, // total for car type
     parkLevel; //what level the car will be parked at
 string carString;
 
	switch (menuChoice) {
		case 'R':  //Rental Car				
			subChoice = subMenu();
			carChoice = checkSubChoice (subChoice);			
			if (carChoice != 4){ // user chose to return to menu
				displayCars (carChoice, array, levelTotal, carString);
				if (levelTotal == ALL_LVL_EMPTY) // no parking spots avail
					cout <<  "Error - There are no cars available of that type" << endl;
				else {
					parkLevel = emptySpot (carChoice, array); //find a parking spot
					array[carChoice][parkLevel] = array[carChoice][parkLevel] - 1;//subtract a car to the spot
					levelTotal = levelTotal - 1; //subtract total car avail					06
					cout << "Pick up car from  " << carString << " row of level " << parkLevel <<  endl;
					cout << levelTotal  << " " << carString <<  " cars will now be available " <<  endl;
					system ("PAUSE");
					printArray (array);
				} //end else
			} //end else			
			break;
		case 'T':  //Turn in Car					
			subChoice = subMenu();
			carChoice = checkSubChoice (subChoice);			
			if (carChoice != 4){ // user chose to return to menu
				displayCars (carChoice, array, levelTotal, carString);				
				if (levelTotal == ALL_LVL_FULL)
					cout <<  "Error - Car is being returned to the wrong company" << endl;
				else {				
					parkLevel = parkWhere (); //promt user to select where to park
					checkPark(carChoice, parkLevel, array); //error check					
					array[carChoice][parkLevel] = array[carChoice][parkLevel] +1;//add a car to the spot
					levelTotal = levelTotal + 1; //aggregate total car avail					
					cout << "Okay to park car in " << carString << " row of level " << parkLevel <<  endl;
					cout << levelTotal  << " " << carString <<  " cars will now be available " <<  endl;
					system ("PAUSE");
					printArray (array);

				} //end else
			} //end if
			break;
		case 'E': // Exit					
			cout << "Do you want to commit changes to file (Y/N)?: ";
			cin >> commitChange;
			commitChange = toupper (commitChange);
			if (commitChange == 'Y')
			closeBinFile (array);
			quit = true;
			break;
	} //end switch

} // end of menuAction

/*************************************************************************
  FUNCTION:	    showDescription
  DESCRIPTION:  displays program description to screen
  INPUT:		Parameters:	DESCRIPTION - description of what the program does

*************************************************************************/
void showDescription (string DESCRIPTION){
    cout << endl;
    cout << "------------------------------------------------------" << endl;
    cout << DESCRIPTION << endl;
    cout << "Developed by " << DEVELOPER << endl;
    cout << "Class: " << CLASSNUM << endl;
    cout << "Assn:  " << ASSIGNMENT << endl;
    cout << "------------------------------------------------------" << endl;
    cout << endl << endl;
} // end of showDescription



//********************************************************************* 
// Function:         printArray 
// Description:      Displays values in one matrix, neatly by row
// INPUT:  
//       Parameter:  array - matrix to dispaly
//********************************************************************* 
void printArray (int array[][LEVELS])		
{
  int lineCount = 0,
	  totalCount = 0;
  const string CAR_STRINGS[] = {"Compact  ", "Mid-Size ", "Full Size",
                  "SUV      "};
  //Output header
  cout << "Level:   " << setw(6) << "0" << setw(6) << "1" << setw(6) << "2" <<
	  setw(6) << "3" << setw(6) << "4" << setw(6) << "5" << 
	  setw(9) << "Total" << endl << endl;
  
  // display array data per line & track line total &total total
  for (int car_type = 0; car_type < CAR_TYPES; car_type++) {	  
      cout << CAR_STRINGS[car_type];
	  lineCount = 0; //clear line counter
      for (int level = 0; level < LEVELS; level++){
        cout << setw(6) << array[car_type][level];
		lineCount += array[car_type][level]; // track car total for line
	  } //end inner loop
	  	  
	  cout << setw(6) << lineCount << endl;
	  totalCount += lineCount; // total values for all avaiavle cars
	  
  }    // end outer for
  //Display Totals for Loop
  cout << setw(51) << "-----"<< endl;
  cout << "Total Cars Available for Rental" << setw(20) << totalCount << endl;

  cout << endl << endl;
  return;
}
//********************************************************************* 
// Function:         displayCars
// Description:      Displays values in one matrix, neatly by row
// INPUT:  
//       Parameter:  array - matrix to dispaly
//********************************************************************* 
void displayCars (int car_type, int array[][LEVELS], int& lineCount, string& carString)		
{
  //string carString;
  if (car_type != 4) {//only display if not none
	  //carType carList; // enumerated type carList 
	  lineCount = 0; //initialize var
	  const string CAR_STRINGS[] = {"Compact  ", "Mid-Size ", "Full Size",
					  "SUV      "};
	  
	  //Output header  	  
	  carString = convertCarToString (car_type);
	  cout << "Number of " << carString << " Cars Parked on each Level" << endl;

	  cout << "Level:   " << setw(6) << "0" << setw(6) << "1" << setw(6) << "2" <<
		  setw(6) << "3" << setw(6) << "4" << setw(6) << "5" << 
		  setw(9) << "Total" << endl << endl;
  
	  // display array data per line & track line total &total total	  
		cout << CAR_STRINGS[car_type];
		for (int level = 0; level < LEVELS; level++){
		cout << setw(6) << array[car_type][level];
		lineCount += array[car_type][level]; // track car total for line
		} //end inner loop
	  	
		//Display total for the ar type
		cout << setw(6) << lineCount << endl;	   

	  //spacing
	  cout << endl << endl;
  } // end of if
  
  return;
}

//********************************************************************* 
// Function:         fullGarage 
// Description:      Fills garage with the maximum cars 5 per level
// INPUT:  
//       Parameter:  array - matrix to initialize values
//********************************************************************* 
void fullGarage (int array[][LEVELS])		
{
  for (int car_type = 0; car_type < CAR_TYPES; car_type++) {
      
      for (int level = 0; level < LEVELS; level++)
          array[car_type][level] = MAX_CARS;	  
      
  }    // end outer for

  return;
}  //end of fullGarage

//********************************************************************* 
// Function:         emptyGarage 
// Description:      Fills garage with the maximum cars 5 per level
// INPUT:  
//       Parameter:  array - matrix to initialize values
//********************************************************************* 
void emptyGarage (int array[][LEVELS])		
{
  for (int car_type = 0; car_type < CAR_TYPES; car_type++) {
      
      for (int level = 0; level < LEVELS; level++)
          array[car_type][level] = EMPTY_CARS;      
  }    // end outer for

  return;
}  //end of emptyGarage

// **************************************************************************
// FUNCTION:     displayMenu 
// DESCRIPTION:  Displays the menu choices
// OUTPUT:       menuChoice - user input from the menu choices
// **************************************************************************
char displayMenu ()
{
//local variables
char  menuChoice;

	cout << "---------------------------------------" << endl;
	cout << "RENTAL CAR ACTION MENU" << endl;
	cout << "R - Rent a Car" << endl;
	cout << "T - Turn in a car" << endl;	
	cout << "E - Exit the Program" << endl;
	cout << "---------------------------------------" << endl << endl;
	cout << "Choose an option from the menu: ";
	cin >> menuChoice;
	menuChoice = toupper (menuChoice); //uppercase user input
return menuChoice;
} // end of actionMenu

// **************************************************************************
// FUNCTION:     checkMenuChoice 
// DESCRIPTION:  Validates user input menu choice re-prompting until correct
// INPUT:        Parameter:  menuChoice -user input from menu choices
// OUTPUT:       Parameter:  menuChoice  - validated menu choice
// **************************************************************************
char checkMenuChoice (char& menuChoice)
{	
 //loop until valid input 	
	while( (menuChoice != 'R') && (menuChoice != 'T')
		&& (menuChoice != 'E') ) {
		if ( (menuChoice != 'R') && (menuChoice != 'T') && (menuChoice != 'E') ){				
			cout << "Invalid Input- Please select R, T, or E" << endl;
			menuChoice = displayMenu();
		} // end if
	}    // end while 

return menuChoice;
} // end of checkMenuChoice

// **************************************************************************
// FUNCTION:     checkSubChoice 
// DESCRIPTION:  Validates user input menu choice re-prompting until correct
// INPUT:        Parameter:  menuChoice -user input from menu choices
// OUTPUT:       Parameter:  menuChoice  - validated menu choice
// **************************************************************************
int checkSubChoice (char& menuChoice)
{
  int intMenuChoice; // converted char to int to work with enumerated type carType
 //loop until valid input 	
	while( (menuChoice != 'C') && (menuChoice != 'M')
		&& (menuChoice != 'F') && (menuChoice != 'S')
		&& (menuChoice != 'R') ) {

			if ( (menuChoice != 'C') && (menuChoice != 'M')
			&& (menuChoice != 'F') && (menuChoice != 'S')
			&& (menuChoice != 'R') ) {
				cout << "Invalid Input- Please select C, M, F, S, or R: " << endl;
				menuChoice = subMenu();
			} //end if
	}    // end while

	//convert to int for enumerated type
	switch (menuChoice) {
		case 'C':  //Compact
			intMenuChoice = 0;
			break;
		case 'M':  //midsize
			intMenuChoice = 1;
			break;
		case 'F':  //fullsize
			intMenuChoice = 2;
			break;
		case 'S':  //SUV
			intMenuChoice = 3;
			break;
		case 'R':  //None
			intMenuChoice = 4;
			break;
	} //end switch

return intMenuChoice;
} // end of checkMenuChoice


// **************************************************************************
// FUNCTION:     subMenu 
// DESCRIPTION:  Displays the menu choices
// OUTPUT:       menuChoice - user input from the menu choices
// **************************************************************************
char subMenu ()
{
//local variables
char  menuChoice;

	cout << "---------------------------------------" << endl;
	cout << "RENTAL CAR CAR TYPES" << endl;
	cout << "C - Compact" << endl;
	cout << "M - Mid-size" << endl;	
	cout << "F - Full-size" << endl;
	cout << "S - SUV" << endl;
	cout << "R - Return to Main Menu" << endl;
	cout << "---------------------------------------" << endl << endl;
	cout << "Choose an option from the menu: ";
	cin >> menuChoice;
	menuChoice = toupper (menuChoice); //uppercase user input
return menuChoice;
} // end of actionMenu

// **************************************************************************
// FUNCTION:     convertCarToString 
// DESCRIPTION:  turn enum data into string for output purposes
// INPUT:        Parameter:  currentStatus - enum data type for listing status
// OUTPUT:       Parameter:  statusString  - converted enum type to string
// **************************************************************************
string convertCarToString (int carList)
{
	string carString;
	switch (carList) {
		case 0: carString = "Compact";
			break;
		case 1: carString = "Mid-size";
			break;
		case 2: carString = "Full size";
			break;
		case 3: carString = "SUV";
			break;
		case 4: carString = "None";
			break;
	} //end switch
	return carString;
} // end of convertCarToString

// **************************************************************************
// FUNCTION:     parkWhere 
// DESCRIPTION:  Displays the menu choices
// OUTPUT:       menuChoice - user input from the menu choices
// **************************************************************************
int parkWhere ()
{
//local variables
int  menuChoice;

	cout << endl;
	cout << "Your car can be parked on any level with less than " << MAX_CARS << " cars." << endl;
	cout << "Which level will the car be parked on? ";
	cin >> menuChoice;
	
return menuChoice;
} // end of actionMenu

// **************************************************************************
// FUNCTION:     checkPark 
// DESCRIPTION:  Validates user input menu choice re-prompting until correct
// INPUT:        Parameter:  menuChoice -user input from menu choices
// OUTPUT:       Parameter:  menuChoice  - validated menu choice
// **************************************************************************
int checkPark (int car_type, int& menuChoice, int array[][LEVELS])
{	
 //loop until valid input 	
	while( ( array [car_type] [menuChoice] >= MAX_CARS) || (menuChoice < 0) || (menuChoice > MAX_CARS) ) {
		//cout << "array value is " << array [car_type] [menuChoice] << endl;
		if ( (menuChoice < 0) || (menuChoice > MAX_CARS) )
			cout << menuChoice << " is out of bounds" << endl;
		
		else if (array [car_type] [menuChoice] >= MAX_CARS) 				
			cout << "Error - Level " << menuChoice << " already contains " << MAX_CARS <<  endl;
		
		menuChoice = parkWhere(); //re promt for user input		
	}    // end while	
return menuChoice;
} // end of checkMenuChoice

//********************************************************************* 
// Function:         emptySpot 
// Description:      Fills garage with the maximum cars 5 per level
// INPUT:  
//       Parameter:  array - matrix to initialize values
//********************************************************************* 
int emptySpot (int car_type, int array[][LEVELS])
{
 
int spot = 10;      //inialize out of range
int level = 5; //start at the top level
  
	do {		
		//cout << "level = " << level << endl;
		//cout << "value in array" << array[car_type][level] << endl;
		 if (array[car_type][level] > 0)
			spot = level;
		 else 
			 level--;
	} while ( (spot == 10) && (level > 0 ) );

  return spot;
}  //end of fullGarage

// **************************************************************************
// FUNCTION:     openBinFile 
// DESCRIPTION:  Opens Binary File
// OUTPUT:       None
// **************************************************************************
void openBinFile (int garage[][LEVELS])
{	
	int row, //temporary integer variable to hold input
		col,
		cars;
	ifstream binFile;  // in&output file stream (binary file)
	
	binFile.open (GARBAGE_FILENAME.c_str(), ios::binary | ios::in );

	if (!binFile)			
		fullGarage (garage); //fill array with max values (initialize)				
	else {
		emptyGarage (garage); //fill array with zero values (initialize)
	    // priming read for first data item
		binFile.read( reinterpret_cast<char*> (&row), sizeof(int) );
		while (binFile){ //number successfully read				
			binFile.read( reinterpret_cast<char*> (&col), sizeof(int) );
			binFile.read( reinterpret_cast<char*> (&cars), sizeof(int) );			
			garage[row][col] = cars;
			binFile.read( reinterpret_cast<char*> (&row), sizeof(int) );
		} // end of while		
	    cout << "All file data has been read." << endl << endl;
	}  //end of else
	
	binFile.close();
	 		    
} //end of openBinFile

// **************************************************************************
// FUNCTION:     closeBinFile 
// DESCRIPTION:  Opens Binary File
// OUTPUT:       None
// **************************************************************************
void closeBinFile (int garage[][LEVELS])
{	
	int row, //temporary integer variable to hold input
		col,
		cars,		
		writeCount = 0; //count how many cars were written
	ofstream binFile;  // in&output file stream (binary file)
	
	binFile.open (GARBAGE_FILENAME.c_str(), ios::binary | ios::out | ios::trunc );		
		
	// priming read for first data item
	for (int car_type = 0; car_type < CAR_TYPES; car_type++) {
      
      for (int level = 0; level < LEVELS; level++) {
		  binFile.write( reinterpret_cast<char*> (&row), sizeof(int) );
		  binFile.write( reinterpret_cast<char*> (&col), sizeof(int) );
		  binFile.write( reinterpret_cast<char*> (&cars), sizeof(int) );
		  writeCount++;
	  }  // end inner for        	  
    }    // end outer for
	
	cout << writeCount << " files has been written." << endl << endl;	
	binFile.close();
	system("PAUSE");	
	return;
} //end of openBinFile
