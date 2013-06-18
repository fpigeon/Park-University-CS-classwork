/* ----------------------------------------------------------------------------
Author: Frank Pigeon
Class: CS362
Assignment: Week 6
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
             ASSIGNMENT = "Week 6";
const string GARBAGE_FILENAME = "GARBAGE.BIN";  //binary file 
enum car_type {COMPACT, MIDSIZE, FULLSIZE, SUV, NONE}; // listing car type options
const int MAX_CARS = 5; // maximum cars on a level
const int CAR_TYPES = 4; //types of cars
const int LEVELS = 6; //number of levels in the garage
const int EMPTY_CARS = 0;
const int ERROR_1 = 1; // unique error message
const int ERROR_2 = 2; // unique error message
const int ERROR_3 = 3; // unique error message

          
//prototypes
void showDescription (string DESCRIPTION);
//void readListings(housingRec housingList[], int& num);
//void loadExistingData (housingRec housingList[], int& listingCount);
//void displayListings (housingRec housingList[], int count);
//string convertStatusToString (status currentStatus);
//char checkMenuChoice (char& menuChoice);
//char displayMenu ();
//void menuAction (char menuChoice, housingRec housingList[], int& listingCount, bool& quit);
//void addListings (housingRec housingList[], int& count);
//void displayMLS (housingRec housingList[], int count);
//void deleteItem (int itemToDel, int& listingCount, housingRec housingList[], bool& deleted);
//void writeListings(housingRec housingList[], int count);
//int MLSinput ();
//double priceInput ();
//int statusInput () ;
//string zipInput ();
//string realtorInput ();
void printArray (int array[][LEVELS]);
void fullGarage (int array[][LEVELS]);
void emptyGarage (int array[][LEVELS]);
/******************************************************************************
//  FUNCTION:	  main
//  DESCRIPTION:  Calls other functions and intializes array of records
//  INPUT: File:  Reads the LISTINGS.TXT if user desires
//  CALLS TO:	  showDescription, loadExistingData, displayMenu, checkMenuChoice, menuAction
******************************************************************************/
int main()
{
    //local variables
	car_type carList; // enumerated type carList 
	int garage [CAR_TYPES] [LEVELS]; // create a two-dimensional array for the garage
    int temp_level, //temporary integer variable to hold input
		temp_row,
		temp_car;
	//char menuChoice;
	//bool quit = false;
	fstream binFile;              // in&output file stream (binary file)
	binFile.open (GARBAGE_FILENAME, ios::binary | ios::in | ios::out| ios::app);
	if (!binFile){
			cout << "Error opening data file" << endl;
			cout << "Creating bin file" << endl;
			binFile.open (GARBAGE_FILENAME, ios::binary | ios::in | ios::out| ios::trunc);
			fullGarage (garage);//fill array with max values (initialize)
			system ("PAUSE");
	} //end of if
	else {
	emptyGarage (garage);//fill array with zero values (initialize)
	// priming read for first data item
	binFile.read( reinterpret_cast<char*> (&temp_level), sizeof(int) );
	while (binFile){ //number successfully read
		for (int carsCount = 0; carsCount < CAR_TYPES; carsCount++) {      
			for (int levelCount = 0; levelCount < LEVELS; levelCount++){
				//array[car_type][level] = MAX_CARS;
				binFile.read( reinterpret_cast<char*> (&temp_level), sizeof(int) );
				binFile.read( reinterpret_cast<char*> (&temp_row), sizeof(int) );
				binFile.read( reinterpret_cast<char*> (&temp_car), sizeof(int) );
				garage[temp_level][temp_row] = temp_car;
			} //end inner for
		}    // end outer for
	} // end of while	
	binFile.close();
	cout << "All file data has been read." << endl;
     
		 
    

}  //end of else

    showDescription (DESCRIPTION);  // oupt program desciption to screen	
	printArray (garage);//display array output to screen 
	/*loadExistingData (housingList, listingCount);    
	do {
		menuChoice = displayMenu ();
		checkMenuChoice (menuChoice);	 	
		menuAction (menuChoice, housingList, listingCount, quit);
	} while (quit == false);*/
	system ("PAUSE");
    return 0;
}  // end main
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
	  setw(10) << "Total" << endl << endl;
  
  for (int car_type = 0; car_type < CAR_TYPES; car_type++) {	  
      cout << CAR_STRINGS[car_type];
	  lineCount = 0; //clear line counter
      for (int level = 0; level < LEVELS; level++){
        cout << setw(6) << array[car_type][level];
		lineCount += array[car_type][level];
	  } //end inner loop
	  cout << setw(6) << lineCount << endl;
	  totalCount += lineCount;

	  
  }    // end outer for

  cout << endl << endl;
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
