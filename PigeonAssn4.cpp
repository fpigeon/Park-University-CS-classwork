/* ----------------------------------------------------------------------------
Author: Frank Pigeon
Class: CS362
Assignment: Week 4
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
const string DESCRIPTION = "This program keeps track of homes for sale in the area.",
             DEVELOPER = "Frank Pigeon",
             CLASSNUM = "CS362",
             ASSIGNMENT = "Week 4";
const string LISTINGS_FILENAME = "LISTINGS.TXT";  //input file 
enum status {AVAILABLE, CONTRACT, SOLD}; // listing status options
const int MAX_LISTINGS = 750, // maximum number of patients
          ZIP_MAX_LENGTH = 10, // 10 chars in the zip code ie 12345-7890
	      MID_POINT = 5, // middle point in the zip code string
		  REALTOR_MAX_LENGTH = 20, // max legnth for realtor string
		  MAX_MAX = 999999; //max MLS and Price
const int ERROR_1 = 1; // unique error message
const int ERROR_2 = 2; // unique error message
const int ERROR_3 = 3; // unique error message

// Defines structure to hold housing listing information
struct  housingRec { //define structure
    int mlsNum; //MLS number
    double price; // double floating point number
    status currentStatus; // enumerated type for listing status
    string zip;  // 5+4 zip code
    string realtyCompany; // up to 20 charaacters any combo of letters and spaces
}; // end housingRec definition    
          
//prototypes
void showDescription (string DESCRIPTION);
void readListings(housingRec housingList[], int& num);
void loadExistingData (housingRec housingList[], int& listingCount);
void displayListings (housingRec housingList[], int count);
string convertStatusToString (status currentStatus);
char checkMenuChoice (char& menuChoice);
char displayMenu ();
void menuAction (char menuChoice, housingRec housingList[], int& listingCount, bool& quit);
void addListings (housingRec housingList[], int& count);
void displayMLS (housingRec housingList[], int count);
void deleteItem (int itemToDel, int& listingCount, housingRec housingList[], bool& deleted);
void writeListings(housingRec housingList[], int count);
int MLSinput ();
double priceInput ();
int statusInput () ;
string zipInput ();
string realtorInput ();

/******************************************************************************
//  FUNCTION:	  main
//  DESCRIPTION:  Calls other functions and intializes array of records
//  INPUT: File:  Reads the LISTINGS.TXT if user desires
//  CALLS TO:	  showDescription, loadExistingData, displayMenu, checkMenuChoice, menuAction
******************************************************************************/
int main()
{
    //local variables	
    housingRec housingList[MAX_LISTINGS]; //array of records storing Listings
    int listingCount;
	char menuChoice;
	bool quit = false;

    showDescription (DESCRIPTION); 
	loadExistingData (housingList, listingCount);    
	do {
		menuChoice = displayMenu ();
		checkMenuChoice (menuChoice);	 	
		menuAction (menuChoice, housingList, listingCount, quit);
	} while (quit == false);
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
    cout << "Assn: " << ASSIGNMENT << endl;
    cout << "------------------------------------------------------" << endl;
    cout << endl << endl;
} // end of showDescription

/*************************************************************************
  FUNCTION:	    readListings
  DESCRIPTION:  attempts to load existing data from input file
  INPUT:		Parameters:	housingList - array of records that stores listings
							num - count of listings in the array of records
  OUTPUT: 	    Parameters:	housingList - array of records that stores listings
							num - count of listings in the array of records
*************************************************************************/
void readListings(housingRec housingList[], int& num)
{
//local variables
  char errorChoice; // user input if error opening file
  ifstream inData;         // input file stream
  ofstream outData;         // output file stream
  int temp;  // temp file to work with enum stype status
  num = 0;   // number of listings read from file

  inData.open (LISTINGS_FILENAME.c_str() );  

  // if file doesn't open present error and ask user if they
  // wish to exit or start with no data    
  if (!inData) { 
    do {        
      cout << "Error opening data file" << endl ;
      cout << "Do you wish to exit or proceed with no data? (Q-QUIT or C-Continue)";
      cin >> errorChoice;
      errorChoice = toupper (errorChoice);
    } while ( (errorChoice == 'Q') && (errorChoice == 'C') );
	if (errorChoice == 'Q')
		return;
	else
		outData.open (LISTINGS_FILENAME.c_str() );		
  } // end if no file 
      

  else {  // file opened successfully     
    inData >> housingList[num].mlsNum;// priming read

	if (num > MAX_LISTINGS) { //array bounds 
			 cout << "Error - too many records in the file" << endl;
			 cout << "All excess daya will be ignored." << endl << endl;
	}  //end too many listings
    else {// continue on 
        
        while (inData && num <= MAX_LISTINGS) {
            // read rest of data about listing            
            inData >> housingList[num].price >> temp; // place status into temp value                                               
                   housingList[num].currentStatus = static_cast <status> (temp);                     
            inData >> housingList[num].zip;
            inData.ignore (1);
            getline(inData, housingList[num].realtyCompany);   //string with spaces
			   inData.ignore (0, '\n'); //ignore remainder of text 			
            num++;     // increment listing count            
            inData >> housingList[num].mlsNum;   // try to read next listing  
        }  // end while
		 
    } // end of else continue on array in bounds
  } // end of else
      
  inData.close();
  outData.close(); 
} // end of readListings

/*************************************************************************
  FUNCTION:	    displayListings
  DESCRIPTION:  Displays data for all listings, one record per line
  INPUT:		Parameters:	housingList - array of records that stores listings
							num - count of listings in the array of records
  OUTPUT: 	    Parameters:	housingList - array of records that stores listings
							num - count of listings in the array of records
*************************************************************************/
void displayListings (housingRec housingList[], int count)
{   
	 cout << endl;
	 cout << "count in is " << count << endl;
	 cout << setw(14) << "Asking" << setw(10) << "Listing" << endl;
	 cout << "MLS#    Price    Status     Zip Code    Realtor" << endl;
	 cout << "------  -------  ---------  ----------  -------------------" << endl;

	 for (int num = 0; num < count; num++) {
		  cout << right  << housingList[num].mlsNum
		   << setw(9) << housingList[num].price << "  "
		   << left << setw(11) << convertStatusToString (housingList[num].currentStatus)
		   << right << setw(10) << housingList[num].zip 
		   << left << "  " << housingList[num].realtyCompany << endl;	
	 } // end for
      
	 cout << endl;  
	 return;
} // end displayListings

// **************************************************************************
// FUNCTION:     convertStatusToString 
// DESCRIPTION:  turn enum data into string for output purposes
// INPUT:        Parameter:  currentStatus - enum data type for listing status
// OUTPUT:       Parameter:  statusString  - converted enum type to string
// **************************************************************************
string convertStatusToString (status currentStatus)
{
	string statusString;
	switch (currentStatus) {
		case AVAILABLE: statusString = "Available";
			break;
		case CONTRACT: statusString = "Contract";
			break;
		case SOLD: statusString = "Sold";
			break;	
	} //end switch
	return statusString;
} // end of convertStatusToString

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
	cout << "LOCAL REALTOR'S ASSOCIATION DATABASE" << endl;
	cout << "D - Display All Listings" << endl;
	cout << "A - Add a Listings" << endl;
	cout << "R - Remove a Listings" << endl;
	cout << "E - Exit the Program" << endl;
	cout << "---------------------------------------" << endl << endl;
	cout << "Choose an option from the menu: ";
	cin >> menuChoice;
	menuChoice = toupper (menuChoice); //uppercase user input
return menuChoice;
} // end of displayMenu

// **************************************************************************
// FUNCTION:     checkMenuChoice 
// DESCRIPTION:  Validates user input menu choice re-prompting until correct
// INPUT:        Parameter:  menuChoice -user input from menu choices
// OUTPUT:       Parameter:  menuChoice  - validated menu choice
// **************************************************************************
char checkMenuChoice (char& menuChoice)
{	
 //loop until valid input 	
	while( (menuChoice != 'D') && (menuChoice != 'A')
		&& (menuChoice != 'R') && (menuChoice != 'E') ) {
		if ( (menuChoice != 'D') && (menuChoice != 'A') && (menuChoice != 'R')
				&& (menuChoice != 'E') )
				cout << "Invalid Input- Please select D, A, R or E" << endl;
				menuChoice = displayMenu();
	}    // end while 

return menuChoice;
} // end of checkMenuChoice

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
void menuAction (char menuChoice, housingRec housingList[], int& listingCount, bool& quit)
{
 int mlsDelete; //user input to delete an MLS Listing
 bool deleted; // index to delete
 char commitChange; // user input to commit changes

switch (menuChoice) {
	case 'D':  //Display All		
		if (listingCount > 0)
			displayListings (housingList, listingCount);
		else 
			cout << "There are no records in the file." << endl;
		break;
	case 'A':  //Add a Listing		
		addListings (housingList, listingCount);
		break;
	case 'R': // Remove a Listing		
		displayMLS (housingList, listingCount);
		//cout << "Enter MLS Listing to delete: ";
		//cin >> mlsDelete;
		mlsDelete = MLSinput();
		deleteItem (mlsDelete, listingCount, housingList, deleted);
		if (deleted == false) //display error if not found
            cout << mlsDelete << " not found in list" << endl;
		else { // delete the listing            
            cout << "MLS Listing " << mlsDelete <<
                 " was deleted successfully" << endl;
			//displayListings (housingList, listingCount);
         } // end else
		break;
	case 'E': // Exit		
		cout << "Do you want to commit changes to file (Y/N)?: ";
		cin >> commitChange;
		commitChange = toupper (commitChange);
		if (commitChange == 'Y')
			writeListings(housingList, listingCount);
		quit = true;
		break;
} //end switch

} // end of menuAction

/*************************************************************************
  FUNCTION:	    loadExistingData
  DESCRIPTION:  attempts to open input file and if not able to creates it
  INPUT:		Parameters:	housingList - array of records that stores listings
							listingCount - count of listings in the array of records							
  OUTPUT: 	    Parameters:	listingCount - count of listings in the array of records					
*************************************************************************/

void loadExistingData (housingRec housingList[], int& listingCount) {
//local variables
ofstream outData; // output file stream
char loadExisting,
	 errorChoice;
	do {
        cout << "Load exisiting data from a file (Y/N)? ";
        cin >> loadExisting;
        loadExisting = toupper (loadExisting);
    } while ( (loadExisting != 'Y') && (loadExisting != 'N') ); 

	if (loadExisting == 'Y') { //append data to input file
		readListings (housingList, listingCount);		
		outData.open (LISTINGS_FILENAME.c_str() , ios::app );	
		
		// if file doesn't open present error and ask user if they
		// wish to exit or start with no data    
		if (!outData) { 
			do {
				cout << endl;
				cout << "Error opening data file" << endl ;
				cout << "Do you wish to exit or proceed with no data? (Q-QUIT or C-Continue)";
				cin >> errorChoice;
				errorChoice = toupper (errorChoice);
			} while ( (errorChoice == 'Q') && (errorChoice == 'C') );
		} // end if no file
	} // end of if load existing data
	else if (loadExisting == 'N') //create new  input file 
		outData.open (LISTINGS_FILENAME.c_str() );
	
	outData.close();
	return;
} // end of function loadExistingData

/*************************************************************************
  FUNCTION:	    addListings
  DESCRIPTION:  adds new listing to the array of records
  INPUT:		Parameters:	housingList - array of records that stores listings
							count - count of listings in the array of records
  OUTPUT: 	    Parameters:	housingList - updated array of records that stores listings
							count - updated count of listings in the array of records												
*************************************************************************/
void addListings (housingRec housingList[], int& count)
{  		
	char newListing = ' ';
	int mls;
	double price;
	int temp;	
	string zip, realtor;
	
	while ( (count <= MAX_LISTINGS) && ( newListing != 'N' ) ) {            		
		mls = MLSinput ();
		housingList[count].mlsNum = mls;		

		price = priceInput ();
		housingList[count].price = price;		

		temp = statusInput ();
		housingList[count].currentStatus = static_cast <status> (temp);				

		zip = zipInput ();
		housingList[count].zip = zip;
		cin.ignore();			
		
		realtor = realtorInput ();
		housingList[count].realtyCompany = realtor;				
		
		count++;  // increment listing count

		cout << endl;
		do { //prompt for adding a new listing with error check
			cout << "Would you like to enter another listing (Y/N)?: ";
			cin >> newListing;
			newListing = toupper (newListing);

		} while ( (newListing != 'Y') && (newListing != 'N') );

	}  // end while		
		     
return;
} // end addListings

/*************************************************************************
  FUNCTION:	    deleteItem
  DESCRIPTION:  Implementation of deleting an item from an ordered list
  INPUT:		Parameters:	itemToDel - listing to delete from the array of records
							listingCount - array of records that stores listings
							housingList - array of records that stores listings
							deleted - flags to whther record was deleted or not							
  OUTPUT: 	    Parameters:	housingList - updated array of records that stores listings
							listingCount - array of records that stores listings
							deleted - flags to whther record was deleted or not	
*************************************************************************/
void deleteItem (int itemToDel, int& listingCount, housingRec housingList[], bool& deleted)
{  
  int placeFound;               // index where found

  deleted = false;              // item not found and deleted yet
      
  // Find where itemToDel is in list
  placeFound = 0;                             // Start search at first index
  
  while ((placeFound < listingCount) &&           // While still values to search
         (housingList[placeFound].mlsNum != itemToDel))     // and value not found
            placeFound++;                        // increment index
 
  //If itemToDel was Found, delete it
  if (placeFound < listingCount) {                
     //cout << "place found is " << placeFound << endl;
	 //cout << "listingcount is " << listingCount << endl;
     //copy the record in the last occuppied index location within the array over the record
	 
     //housingList[placeFound] = housingList[listingCount - 1];	 	 
	 //cout << "MLS of placefound is now " << housingList[placeFound].mlsNum << endl;
     for (int num = placeFound + 1; num < listingCount; num++)
     housingList[num - 1] = housingList[num];     // Decrement list size
     listingCount--;

     // Zero out deleted last item
     //housingList[listingCount] = 0;	// Optional

     deleted = true;       
  }  // end if
  //cout << "listingcount out of loop is " << listingCount << endl;  
  return;
} //end of deleteItem

/*************************************************************************
  FUNCTION:	    displayMLS
  DESCRIPTION:  displays all MLS records from array of record
  INPUT:		Parameters:	housingList - array of records that stores listings
							count - array of records that stores listings							
*************************************************************************/
void displayMLS (housingRec housingList[], int count)
{
 int newLine = 0; //for add a new line
	 cout << endl; 
	 cout << "These are the MLS Listings stored on file:" << endl;
	 cout << "--------------------------------------------------" << endl;

	 for (int num = 0; num < count; num++) {
		  cout << right << housingList[num].mlsNum << setw(8);
		  newLine++;
		  if (newLine == 6) { // add a new line after displaying 6 listings
				cout << endl;
				newLine = 0;
		  }
	 }   // end for
	 cout << endl;  
	 return;
} // end displayMLS

/*************************************************************************
  FUNCTION:	    writeListings
  DESCRIPTION:  outputs array of records to output file
  INPUT:		Parameters:	housingList - array of records that stores listings
							count - array of records that stores listings
  OUTPUT:		File : Writes to output file
*************************************************************************/
void writeListings(housingRec housingList[], int count)
{
//local variables  
  ofstream outData;         // input file stream  
  outData.open (LISTINGS_FILENAME.c_str() );
  
  for (int num = 0; num < count; num++) {
      outData << right << housingList[num].mlsNum 
       << setw(7) << housingList[num].price  
       << setw(2) << housingList[num].currentStatus
       << setw(11) << housingList[num].zip
       << left  << " " << housingList[num].realtyCompany << endl;			
  }   // end for     
  outData.close();
  return;
} // end of writeListings

/*************************************************************************
  FUNCTION:	    MLSinput
  DESCRIPTION:  validates MLS input from user and re-prompts until correct
  INPUT:		None
  OUTPUT:		mlsIn - validated MLS input
*************************************************************************/
int MLSinput ()
{
	int mlsIn;
	do
	{
		cout << "Enter MLS Number: (6 digit number): ";
		cin >> mlsIn;

		if ( (mlsIn < 100000) || (mlsIn > MAX_MAX) ) 
			cout << "Invalid input.  Must be a six digit number." << endl;		

	} while ( (mlsIn < 100000) || (mlsIn > MAX_MAX) );
	
	return 	mlsIn;
} // end of mlsinput

/*************************************************************************
  FUNCTION:	    priceInput
  DESCRIPTION:  validates proce input from user and re-prompts until correct
  INPUT:		None
  OUTPUT:		priceIn - validated price input
*************************************************************************/
double priceInput () 
{
	double priceIn;
	do {
		cout << "Enter asking price: ";
		cin >> priceIn;
		
		if ( (priceIn < 1) || (priceIn > MAX_MAX) ) 
			cout << "Invalid input.  Must be a positive amount." << endl;		

	} while ( (priceIn < 1) || (priceIn > MAX_MAX) );
	
		return 	priceIn;
} // end of priceInput

/*************************************************************************
  FUNCTION:	    statusInput
  DESCRIPTION:  validates statis input from user and re-prompts until correct
  INPUT:		None
  OUTPUT:		temp - validated status input
*************************************************************************/
int statusInput () 
{
	int temp;
	do {
		cout << "Enter the status (0-Available, 1-Contract, or 2-Sold): ";
		cin >>  temp;
		
		if ( (temp < 0) || (temp > 2) ) 
			cout << "Invalid input.  Must be 0, 1, or 2." << endl;		

	} while ( (temp < 0) || (temp > 2) );
	
	return 	temp;
} // end of statusInput

/*************************************************************************
  FUNCTION:	    zipInput
  DESCRIPTION:  validates zip code input from user and re-prompts until correct
  INPUT:		None
  OUTPUT:		zip - validated zip code
*************************************************************************/
string zipInput () 
{
	int boolNum,
		boolZipLength;
	string  zip;
	int location,
		cell;
	do {
		boolNum = 0;
		boolZipLength = 0;
		cout << "Enter the ZIP code (5digits-4digit): ";
		cin >>  zip;	
		
		//cout << "zip length is " << zip.length() << endl;
		if (zip.length() > ZIP_MAX_LENGTH)
			boolZipLength = ERROR_1; //over length
        else
			if (zip.length() < ZIP_MAX_LENGTH)
				boolZipLength = ERROR_2;  //under length
               
        if (boolZipLength == 0) {
			//check first five numbers of zip code
			for (cell = 0; cell < MID_POINT; cell++)
			{				   
				if ( !isdigit(zip[cell]) )
				boolNum = ERROR_1;    //true         
			} // end else
            
			if (boolZipLength == 0 && boolNum == 0){ 
				// Check for dash				
				location = zip.find("-"); //
				//cout << "location is " << location << endl;
				if (location != 5)
					boolNum = ERROR_2;
			} // end of find dash

			if (boolZipLength == 0 && boolNum == 0){
			// check last 4 numbers
				for (cell = (MID_POINT+2); cell < ZIP_MAX_LENGTH; cell++){						   
				if ( !isdigit(zip[cell]) )
				boolNum = ERROR_3;    //true         
				} // end of for
			} // end of check last 4 numbers

		}  //end of check numbers and dash


			
      // Display Error messages (if applicable)      	 

		//display error
		if (boolZipLength == ERROR_1)
		{
			cout << "Zip code "<< zip << " is too long. " << endl;
			cout << "Must be 5 numbers then a dash (-) followed by 4 numbers.  Try again." << endl
				 << "i.e. 12345-7890" 
				 << endl << endl;
		}  //too long
		else
			if (boolZipLength == ERROR_2)
			{
				cout << "Zip code "<< zip << " is too short. " << endl;
				cout << "Must be 5 numbers then a dash (-) followed by 4 numbers.  Try again." << endl
				<< "i.e. 12345-7890"
				<< endl << endl;
			}  //too short
		else
			if (boolNum == ERROR_1)
			{
			   cout << "First 5 must be numbers." << endl;
			   cout << "Must be 5 numbers then a dash (-) followed by 4 numbers.  Try again." << endl
			   << "i.e. 12345-7890"
			   << endl << endl;
			}
		else
			if (boolNum == ERROR_2)
			{
			   cout << "There must be a dash seperating the numbers." << endl;
			   cout << "Must be 5 numbers then a dash (-) followed by 4 numbers.  Try again." << endl
			   << "i.e. 12345-7890"
			   << endl << endl;
			}
		else
			if (boolNum == ERROR_3)
			{
			   cout << "Last 4 must be numbers." << endl;
			   cout << "Must be 5 numbers then a dash (-) followed by 4 numbers.  Try again." << endl
			   << "i.e. 12345-7890"
			   << endl << endl;
			}
	} while ( (boolZipLength > 0) || (boolNum > 0 ));
	
	return 	zip;
} // end of zipInput

/*************************************************************************
  FUNCTION:	    realtorInput
  DESCRIPTION:  validates realtorinput from user and re-prompts until correct
  INPUT:		None
  OUTPUT:		realtor - validated realtor
*************************************************************************/
string realtorInput () 
{
int boolRealtorLength,
	boolAlpha,
	cell,
	realtorLength;
	string  realtor;
			
	do {
		boolRealtorLength = 0;
		boolAlpha = 0;

		cout << "Enter the Realtor:  ";		
		getline (cin, realtor);
		
		realtorLength = realtor.length();

		if (realtorLength > REALTOR_MAX_LENGTH)
			boolRealtorLength = ERROR_1; //over length				
               
        if (boolRealtorLength == 0) {
			//check for characters
			for (cell = 0; cell < realtorLength; cell++)
			{				   
				if ( !isalpha(realtor[cell]) )
				boolAlpha = ERROR_1;    //true         
			} // end else          									
		} //display error
		if (boolRealtorLength == ERROR_1)
		{
			cout << "Realtor Name is too long. " << endl;
			cout << "Must be under 20 characters.  Try again."		 
				 << endl << endl;
		}  //too long
		
		else
			if (boolAlpha == ERROR_1)
			{
			   cout << "Realtor Name must only contain alphabetic characters." << endl;
			   cout << "Try again."			   
			   << endl << endl;
			} // no non alphas
	
	} while ( (boolRealtorLength > 0) || (boolAlpha > 0 ));
	
	return 	realtor;
} // end of realtorInput
