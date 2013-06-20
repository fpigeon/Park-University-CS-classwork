/* ----------------------------------------------------------------------------
Author: Frank Pigeon
Class: CS362
Assignment: Week 5
Program helps keep track of the realtor's association homes for sale.
Processing: Uses array of records to store housing listings.
Output: Outputs the records in the file to screen and at the end asks
user if they want to commit changes to LISTINGS.txt
FUNCTIONS:	 main - calls other functions and initializes array of records
			 showDescription - displays program description to screen
			 readListings - reads input file and stores them in the array of records
             loadExistingData - prompts user if they want to load existing data from
			    the input file or start a new
			 displayListings - displays to screen contents of the array of records
			 convertStatusToString - works with the enumerated type for status
			 checkMenuChoice - validates menu choice
			 displayMenu - displays menu choices to the screen
			 menuAction - calls other functions based on menu choice
			 addListings - add new listings to the array of records
			 displayMLS - displays to screen all MLS fields for user to select one
			    to delete
			deleteItem - removes a record from the array of records
			writeListings - outputs array of records to output file
			MLSinput - promt, read, and validate MLS input
			priceInput - promt, read, and validate price input
			statusInput - promt, read, and validate status input
			zipInput - promt, read, and validate zip code input
			realtorInput - promt, read, and validate realtor input
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
          
//prototypes
void showDescription (string DESCRIPTION);
void printArray (int array[][LEVELS]);
void fullGarage (int array[][LEVELS]);
void emptyGarage (int array[][LEVELS]);
char displayMenu ();
char checkMenuChoice (char& menuChoice);
void openBinFile (int garage[][LEVELS]);
char subMenu ();
int checkSubChoice (char& menuChoice);
void menuAction (char menuChoice, int array[][LEVELS], bool& quit);
string convertCarToString (int carList);
void displayCars (int car_type, int array[][LEVELS], int& lineCount);
int parkWhere ();
int checkPark (int car_type, int& menuChoice, int array[][LEVELS]);

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
	} while (quit == false); 	
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
// int mlsDelete; //user input to delete an MLS Listing
// bool deleted; // index to delete
 char commitChange, // user input to commit changes
	  subChoice; // user input from sub menu
 int carChoice = 4, //converted int from char
	 levelTotal = 0, // total for car type
     parkLevel; //what level the car will be parked at
	switch (menuChoice) {
		case 'R':  //Rental Car	
			cout << "You chose R" << endl;
			subChoice = subMenu();
			carChoice = checkSubChoice (subChoice);
			cout << "You chose " << subChoice << endl;
			displayCars (carChoice, array, levelTotal);				
			break;
		case 'T':  //Turn in Car		
			cout << "You chose T" << endl;
			subChoice = subMenu();
			carChoice = checkSubChoice (subChoice);
			cout << "You chose " << subChoice << endl;
			if (carChoice != 4){
				displayCars (carChoice, array, levelTotal);				
				if (levelTotal == ALL_LVL_FULL)
					cout <<  "Error - Car is being returned to the wrong company" << endl;
				else {				
					parkLevel = parkWhere ();
					checkPark(carChoice, parkLevel, array);
				} //end else
			} //end if
			break;
		case 'E': // Exit		
			cout << "You chose E" << endl;
			cout << "Do you want to commit changes to file (Y/N)?: ";
			cin >> commitChange;
			commitChange = toupper (commitChange);
			//if (commitChange == 'Y')
			//	writeListings(housingList, listingCount);
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
void displayCars (int car_type, int array[][LEVELS], int& lineCount)		
{
  string carString;
  if (car_type != 4) {//only display if not none
	  //carType carList; // enumerated type carList 
	  lineCount = 0; //initialize var
	  const string CAR_STRINGS[] = {"Compact  ", "Mid-Size ", "Full Size",
					  "SUV      "};
	  //const string CARS[] = {"Compact", "Mid-Size", "Full Size",
	  //                "SUV"};
	  //Output header  
	  //cout << "Number of " << CARS [car_type]<< " Cars Parked on each Level" << endl;
	  carString = convertCarToString (car_type);
	  cout << "Number of " << carString << " Cars Parked on each Level" << endl;

	  cout << "Level:   " << setw(6) << "0" << setw(6) << "1" << setw(6) << "2" <<
		  setw(6) << "3" << setw(6) << "4" << setw(6) << "5" << 
		  setw(9) << "Total" << endl << endl;
  
	  // display array data per line & track line total &total total
	  //for (int car_type = 0; car_type < CAR_TYPES; car_type++) {
	  //    lineCount = 0; //clear line counter
		cout << CAR_STRINGS[car_type];
		for (int level = 0; level < LEVELS; level++){
		cout << setw(6) << array[car_type][level];
		lineCount += array[car_type][level]; // track car total for line
		} //end inner loop
	  	  
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
// FUNCTION:     openBinFile 
// DESCRIPTION:  Opens Binary File
// OUTPUT:       None
// **************************************************************************
void openBinFile (int garage[][LEVELS])
{	
	int row, //temporary integer variable to hold input
		col,
		cars;
	fstream binFile;  // in&output file stream (binary file)
	
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
	while( ( array [car_type] [menuChoice] > MAX_CARS) || (menuChoice < 0) || (menuChoice > MAX_CARS) ) {
		cout << "array value is " << array [car_type] [menuChoice] << endl;
		if ( (menuChoice < 0) || (menuChoice > MAX_CARS) )
			cout << menuChoice << " is out of bounds" << endl;
		
		else if (array [car_type] [menuChoice] > MAX_CARS) 				
			cout << "Error - Level " << menuChoice << " already contains " << MAX_CARS <<  endl;
		
		menuChoice = parkWhere(); //re promt for user input		
	}    // end while
	cout << "valid input" << endl;
return menuChoice;
} // end of checkMenuChoice
