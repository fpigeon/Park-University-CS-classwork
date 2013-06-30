/* ----------------------------------------------------------------------------
Author: Frank Pigeon
Class: CS362
Assignment: Week 8
Program helps keep track of the realtor's association homes for sale.
Processing: Uses linked lists to store housing listings.
Output: Outputs the records in the file to screen and at the end asks
user if they want to commit changes to LISTINGS.txt
FUNCTIONS:	 main - calls other functions and initializes linked list
			 showDescription - displays program description to screen
			 readListings - reads input file and stores them in the linked list             
			 displayListings - displays to screen contents of the linked list
			 convertStatusToString - works with the enumerated type for status
			 checkMenuChoice - validates menu choice
			 displayMenu - displays menu choices to the screen
			 menuAction - calls other functions based on menu choice
			 addListings - add new listings to the linked list
			 displayMLS - displays to screen all MLS fields for user to select one
			    to delete
			deleteItem - removes a record from the linked list
			writeListings - outputs linked list to output file
			MLSinput - promt, read, and validate MLS input
			priceInput - promt, read, and validate price input
			statusInput - promt, read, and validate status input
			zipInput - promt, read, and validate zip code input
			realtorInput - promt, read, and validate realtor input
			freeList - frees memory heap from linked list
			changeMLS - changes the price of the MLS listings
------------------------------------------------------------------------------*/

//Header File
#include <iostream> 
#include <iomanip> 
#include <cmath>  
#include <cctype> 
#include <fstream> 
#include <string>
#include <cstddef>
#include <cstdlib>

using namespace std;

//global constants
const string DESCRIPTION = "This program keeps track of homes for sale in the area.",
             DEVELOPER = "Frank Pigeon",
             CLASSNUM = "CS362",
             ASSIGNMENT = "Week 7";
const string CHANGES_FILENAME = "CHANGES.TXT";  //input file 
enum status {AVAILABLE, CONTRACT, SOLD}; // listing status options
const int ZIP_MAX_LENGTH = 10, // 10 chars in the zip code ie 12345-7890
	      MID_POINT = 5, // middle point in the zip code string
		  REALTOR_MAX_LENGTH = 20, // max legnth for realtor string
		  MAX_MAX = 999999, //max MLS and Price
		  MIN_MLS = 100000, //min MLS NUM
		  PRICE_MIN = 1, // min price 
		  MIN_STATUS = 0, //status min
		  MAX_STATUS = 2; //status max
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

struct houseNode { //pointers for the linked list
    housingRec house;	
    houseNode* next;
};  // end houseNode
          
//prototypes
void showDescription (string DESCRIPTION);
void displayListings (houseNode* housingList);
void readListings (houseNode* &first);
string convertStatusToString (status currentStatus);
char checkMenuChoice (char& menuChoice);
char displayMenu ();
void menuAction (char menuChoice, houseNode* &first, bool& quit);
void addListings (houseNode* &first);
void displayMLS (houseNode* first, bool& emptyRecord);
void deleteItem (houseNode* &first, int delName, bool& success);
void writeListings(houseNode* first);
int MLSinput ();
double priceInput ();
int statusInput () ;
string zipInput ();
string realtorInput ();
void freeList (houseNode* &first);	
void changeMLS (houseNode* &first, bool& emptyRecord);

/******************************************************************************
//  FUNCTION:	  main
//  DESCRIPTION:  Calls other functions and intializes linked list
//  INPUT: File:  Reads the user inputed text file
//  CALLS TO:	  showDescription, readListings, displayMenu, checkMenuChoice
				  menuAction
******************************************************************************/
int main()
{
    //local variables		
	houseNode* first = NULL; //node for storing the linked list
	
    //int listingCount;
	char menuChoice; // user option from menu
	bool quit = false; // flag to quit the program

    showDescription (DESCRIPTION);  // display program description
	readListings (first);		    // read from input file "LISTINGS.TXT"
	do { //loop until exit is selected
		menuChoice = displayMenu ();  //display the menu and accept input from the user
		checkMenuChoice (menuChoice); //error check user input
		menuAction (menuChoice, first, quit);  //action menu based on user decision
	} while (quit == false); //quit when user selects exit
	system ("PAUSE");
    return 0;
}  // end main

/*************************************************************************
  FUNCTION:	    menuAction
  DESCRIPTION:  calls functions based on user selected menu choice
  INPUT:		
     Parameters:	menuChoice -user input from menu choices							
					first - linked list contians MLS listing data
					quit - used to flag the end of the program
	 File: None
  OUTPUT:
	 Return Val: void
     Parameters:	first - linked list contians MLS listing data
					quit - used to flag the end of the program
	 File: None
  CALLS TO:	displayListings, addListings, displayMLS, deleteItem, changeMLS,
			writeListings, freeList
*************************************************************************/
void menuAction (char menuChoice, houseNode* &first, bool& quit)
{
 int mlsDelete; //user input to delete an MLS Listing
 bool deleted, // index to delete
      emptyRecord; //flag if there are no data in the list
 char commitChange; // user input to commit changes

switch (menuChoice) {
	case 'D':  //Display All
		if (first != NULL) // if there is data
			displayListings (first);  //show all listings to screen
		else  // no data
			cout << "There are no records in the file." << endl;
		break;
	case 'A':  //Add a Listing		
		addListings (first); //attempt to add a listing to the linked list
		break;
	case 'R': // Remove a Listing		
		displayMLS (first, emptyRecord); //display avail MLS num
		if (emptyRecord == true) //no data
			cout << "Error - There is no records in the file" << endl;
		else { //yes data continue on
			mlsDelete = MLSinput(); //error check input
			deleteItem (first, mlsDelete, deleted); //delete node from the list
			if (deleted == false) //display error if not found
				cout << mlsDelete << " not found in list" << endl;
			else { // delete the listing            
				cout << "MLS Listing " << mlsDelete <<
					 " was deleted successfully" << endl;			
			} // end else
		} //end else data in file
		break;
	case 'C':  //Add a Listing				
		changeMLS (first, emptyRecord);
		if (emptyRecord == true) //no data
			cout << "Error - There is no records in the file" << endl;
		break;
	case 'E': // Exit		
		cout << "Do you want to commit changes to file (Y/N)?: ";
		cin >> commitChange;
		commitChange = toupper (commitChange);
		if (commitChange == 'Y')
			writeListings(first);		
		quit = true;
		freeList (first); //free heap memory from linked list
		break;
} //end switch

} // end of menuAction


/*************************************************************************
  FUNCTION:	    readListings
  DESCRIPTION:  attempts to load existing data from input file
  INPUT:		
     Parameters:	first - linked list contians MLS listing data
					
	 File: inFile input stream from user generated input file
  OUTPUT:
	 Return Val: void
     Parameters:	first - linked list contians MLS listing data					
	 File: None
  CALLS TO:	none
*************************************************************************/
void readListings (houseNode* &first)
{
//local variables  
  bool userMenu = false; //flag for goint to the action menu
  ifstream inFile; // input file stream
  int temp; //temp for reading data from input file
  char loadExisting; //input reponse from user to load existing data
  string filename, //user generated file name
	  errorChoice;  //user choice if there was an error
  houseNode *newNode,				// used to allocate memory for list
			*lastNode = NULL;       // points to last node in list

  //promtpt to usae existing data
  do {
      cout << "Load exisiting data from a file (Y/N)? ";
      cin >> loadExisting;
      loadExisting = toupper (loadExisting);
  } while ( (loadExisting != 'Y') && (loadExisting != 'N') ); 
  
  if (loadExisting == 'Y') {    
	cout << endl << "Enter data filename: ";
	cin >> filename;
	filename = filename + ".TXT";
	inFile.open (filename.c_str() );
	if (!inFile) {  //file does not exist	 			 
		do { //until we have good file name or go to menu
			inFile.clear();  //clear file stream before trying another file			
			cout << "Error opening data file" << endl << endl;
			cout << "Enter another filename " << endl;
			cout << "or enter M for Menu:  ";
			cin >> errorChoice;
			if ( (errorChoice == "M") || (errorChoice == "m") )
				userMenu = true;
			else { 				
				errorChoice = errorChoice + ".TXT";
				inFile.open (errorChoice.c_str() );		
			} //end else
		} while ( (!inFile) || (userMenu == false) );
	} // end if no file 
	else{ //file opens and read data
		inFile >> temp; 
		while( inFile)
		{	 

        // create new node and store data read into it     
        newNode = new (nothrow) houseNode;
        
        if (newNode == NULL)
        {
           cout << "No more memory can be allocated!" << endl;
           system("pause");
        }
		else
		{
		  newNode->house.mlsNum = temp;
          inFile >> newNode->house.price;
		  inFile >> temp;
		  newNode->house.currentStatus = static_cast<status>(temp);
		  inFile >> newNode->house.zip;
		  getline(inFile, newNode->house.realtyCompany);          
          newNode->next = NULL;
        
          if (first == NULL)
		    first = newNode;

		  if (lastNode != NULL)	  
            lastNode->next = newNode;        
          
		  lastNode = newNode;
		}
        
			inFile >> temp;        		
		}     // end while
		inFile.close();   	
	} //end file opens and read data

  } //end of load existing
  else 
	cout <<  "return to menu" << endl;	//go to menu          
return;    
} // end of readListings

/*************************************************************************
  FUNCTION:	    displayListings
  DESCRIPTION:  Displays data for all listings, one record per line
  INPUT:		
     Parameters:	first - linked list contians MLS listing data
					
	 File: None
  OUTPUT:
	 Return Val: void
     Parameters:	first - linked list contians MLS listing data					
	 File: None
  CALLS TO:	none
*************************************************************************/
void displayListings (houseNode* first)
{  
  houseNode* tempPtr = first;

	 cout << endl;	 
	 cout << setw(14) << "Asking" << setw(10) << "Listing" << endl;
	 cout << "MLS#    Price    Status     Zip Code    Realtor" << endl;
	 cout << "------  -------  ---------  ----------  -------------------" << endl;

     while (tempPtr != NULL)
	{
		cout << right  << tempPtr->house.mlsNum
		     << setw(9) << tempPtr->house.price << "  "
		     << left << setw(11) << convertStatusToString (tempPtr->house.currentStatus)
		     << right << setw(10) << tempPtr->house.zip 
		     << left << " " << tempPtr->house.realtyCompany << endl;	     
      
        tempPtr = tempPtr->next;
	}   // end while
	 	 
	 return;
} // end displayListings

/*************************************************************************
  FUNCTION:	    convertStatusToString
  DESCRIPTION:  turn enum data into string for output purposes
  INPUT:		
     Parameters:	currentStatus - enum data type for listing status
	 File: None
  OUTPUT:
	 Return Val: string
     Parameters:	statusString  - converted enum type to string
	 File: None
  CALLS TO:	none
*************************************************************************/
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


/*************************************************************************
  FUNCTION:	    displayMenu
  DESCRIPTION:  Displays the menu choices
  INPUT:		
     Parameters:	None
	 File: None
  OUTPUT:
	 Return Val:    char
     Parameters:	menuChoice - user input from the menu choices
	 File: None
  CALLS TO:	none
*************************************************************************/
char displayMenu ()
{
//local variables
char  menuChoice;

	cout << "---------------------------------------" << endl;
	cout << "LOCAL REALTOR'S ASSOCIATION DATABASE" << endl;
	cout << "D - Display all Listings" << endl;
	cout << "A - Add a Listings" << endl;
	cout << "R - Remove a Listings" << endl;
	cout << "C - Change asking price" << endl;
	cout << "E - Exit the Program" << endl;
	cout << "---------------------------------------" << endl << endl;
	cout << "Choose an option from the menu: ";
	cin >> menuChoice;
	menuChoice = toupper (menuChoice); //uppercase user input
return menuChoice;
} // end of displayMenu

/*************************************************************************
  FUNCTION:	    checkMenuChoice
  DESCRIPTION:  Validates user input menu choice re-prompting until correct
  INPUT:		
     Parameters:	menuChoice -user input from menu choices
	 File: None
  OUTPUT:
	 Return Val:    char
     Parameters:	menuChoice - validated menu choice
	 File: None
  CALLS TO:	displayMenu
*************************************************************************/
char checkMenuChoice (char& menuChoice)
{	
 //loop until valid input 	
	while( (menuChoice != 'D') && (menuChoice != 'A')
		&& (menuChoice != 'R') && (menuChoice != 'E') && (menuChoice != 'C') ) {
		if ( (menuChoice != 'D') && (menuChoice != 'A') && (menuChoice != 'R')
				&& (menuChoice != 'E') && (menuChoice != 'C') )
				cout << "Invalid Input- Please select D, A, R, C, or E" << endl;
				menuChoice = displayMenu();
	}    // end while 

return menuChoice;
} // end of checkMenuChoice

/*************************************************************************
  FUNCTION:	    addListings
  DESCRIPTION:  adds new listing to the linked list
  INPUT:		
     Parameters:	first - linked list contians MLS listing data					
	 File: None
  OUTPUT:
	 Return Val: void
     Parameters:	first - linked list contians MLS listing data					
	 File: None
  CALLS TO:	displayListings, addListings, displayMLS, deleteItem, changeMLS,
			writeListings, freeList
*************************************************************************/
void addListings (houseNode* &first)
{  		
	char pauseChar;           // to pause after memory allocation failure
	char newListing = ' '; //blank space
	int mls; //MLS number
	double price; //price for listing
	int temp; //temp value for enum type
	string zip, //zip code
		realtor;	//realtor
	houseNode *newNode,	 // used to allocate memory for list
			  *lastNode = NULL,       // points to last node in list
			  *firstNode = NULL;
	cout << right << endl; 
	do  {
		// create new node and store data read into it     
		newNode = new (nothrow) houseNode;

		if (newNode == NULL)  {
           cout << "No more memory can be allocated!" << endl;
		   cin >> pauseChar;
           system("pause");
        }

		else {
			mls = MLSinput ();		
			newNode->house.mlsNum = mls;

			price = priceInput ();		
			newNode->house.price = price;

			temp = statusInput ();		
			newNode->house.currentStatus = static_cast<status>(temp);

			zip = zipInput ();		
			newNode->house.zip = zip;
			cin.ignore();			
		
			realtor = realtorInput ();
			newNode->house.realtyCompany = realtor;					
		    
			newNode->next = NULL;			
			
			newNode->next = first;			
		    first = newNode;

			cout << endl;
			do { //prompt for adding a new listing with error check
				cout << "Would you like to enter another listing (Y/N)?: ";
				cin >> newListing;
				newListing = toupper (newListing);

			} while ( (newListing != 'Y') && (newListing != 'N') );
		} // end else
	} while ( (newNode != NULL) && ( newListing != 'N' ) );
              		
return;
} // end addListings


/*************************************************************************
  FUNCTION:	    deleteItem
  DESCRIPTION:  Delete the node from the linked list whose name field matches the delName
	INPUT:		
     Parameters:	first - linked list contians MLS listing data
					delName - MLS listing to delete
					success - flag if MLS was deleted 
	 File: None
	OUTPUT:
	 Return Val: void
     Parameters:	first - linked list contians MLS listing data
					success - flag if MLS was deleted 
	 File: None
  CALLS TO:	none		
 
*************************************************************************/

void deleteItem (houseNode* &first, int delName, bool& success)
{        
  houseNode *here,                           // Node being checked
            *prev;                           // Node prior to node being checked

  success = false;                         // Name not found yet

  if ( first->house.mlsNum == delName ) {        // If delName found in first node,
         here = first;
         first = first->next;		      //  Delete top item
         delete here;
         success = true;
  }
  
  else {                                   // Not top item, so search...

         prev = first;                      // Initialize prev and here to 
         here = first->next;                // first 2 nodes in list

         // Look for delName (while not end of list and name doesn't match)
         while ( (here != NULL) && (here->house.mlsNum != delName) ) {   
             	prev = here;               // move prev down one
              	here = here->next;         // move here down one
       	 }

         if (here != NULL) {               // If found, delete item from list
                prev->next = here->next;
                delete here;
                success = true;
         }
         else                              // Otherwise, give error message
                cout << delName << " not found in list." << endl;

  }	// End else (not top Item)

  return; 
} //end of deleteItem

/*************************************************************************
  FUNCTION:	    displayMLS
  DESCRIPTION:  displays all MLS records from the linked list
  INPUT:		
     Parameters:	first - linked list contians MLS listing data
					emptyRecord - used to flag an empty record
	 File: None
  OUTPUT:
	 Return Val: void
     Parameters:	emptyRecord - used to flag an empty record
	 File: None
  CALLS TO:	none
*************************************************************************/
void displayMLS (houseNode* first, bool& emptyRecord)
{
 emptyRecord = false; //initialize flag to false
 houseNode* tempPtr = first;
 int newLine = 1; //for add a new line
	 cout << right << endl; 
	 if (tempPtr == NULL) {		 
		 emptyRecord = true;
	 } //emnd if no records
	 else {
		 cout << "These are the MLS Listings stored on file:" << endl;
		 cout << "--------------------------------------------------" << endl;

		 while (tempPtr != NULL) {
			  cout << setw(7) << tempPtr->house.mlsNum;
			  if (newLine == 6) { // add a new line after displaying 6 listings
					cout << endl;
					newLine = 1;
			  }
			  else				
			  //increment counters
				newLine++;
			  tempPtr = tempPtr->next;
		 }   // end while
		 cout << endl; 
	 } // end if data exists

	 return;
} // end displayMLS

/*************************************************************************
  FUNCTION:	    writeListings
  DESCRIPTION:  outputs linked list to output file
  INPUT:		
     Parameters:	first - linked list contians MLS listing data
	 File: None
  OUTPUT:
	 Return Val: void
     Parameters:	none
	 File: User-generated output text file
  CALLS TO:	none
*************************************************************************/
void writeListings(houseNode* first)
{
//local variables
  bool fileGood = false; //flag for good file read
  char overWrite; //user input whther to overwrite data
  string filename; //where the user wants to save th file
  ofstream outData;         // output file stream    
  houseNode* tempPtr = first; //node for linked list

  //promt user for file name
  cout << right << endl; 
  do {
	  cout << endl << "Enter data filename: ";
	  cin >> filename;
	  filename = filename + ".txt";
	  
	  outData.open (filename.c_str(), ios_base::out | ios_base::in);  // will not create file)
	  
	  if (outData.good()) { //if file exists prompt user to overwrite
		cout << "Warning, file already exists, proceed? ";
		cin >>overWrite;
		  overWrite = toupper (overWrite);
		  if (overWrite = 'N')
			  outData.close();
		  else {
			fileGood = true;
			outData.open (filename.c_str() );
			while (tempPtr != NULL) {
				   outData << right <<  tempPtr->house.mlsNum
				   << setw(7) << tempPtr->house.price 
				   << setw(2) <<  tempPtr->house.currentStatus
				   << setw(11) << tempPtr->house.zip 
				   << left  << " " << tempPtr->house.realtyCompany << endl;
		   
				   tempPtr = tempPtr->next;
			  } //end of overwire
			  outData.close();
		  } //end else
      } //end if file exists 
  } while (fileGood != false);

  if (fileGood == false){ // write output
	  outData.open (filename.c_str() );
	  while (tempPtr != NULL) {
		outData << right <<  tempPtr->house.mlsNum
		<< setw(7) << tempPtr->house.price 
		<< setw(2) <<  tempPtr->house.currentStatus
		<< setw(11) << tempPtr->house.zip 
		<< left  << " " << tempPtr->house.realtyCompany << endl;
		   
		tempPtr = tempPtr->next;
	  } //end of overwire
	  outData.close();
   }
    
  return;
} // end of writeListings

/*************************************************************************
  FUNCTION:	    MLSinput
  DESCRIPTION:   validates MLS input from user and re-prompts until correct
  INPUT:		
     Parameters:	none
	 File: None
  OUTPUT:
	 Return Val: int mlsIn - validated MLS number
     Parameters:	none
	 File: None
  CALLS TO:	none
*************************************************************************/
int MLSinput ()
{
	int mlsIn; //MLS number from user
	do
	{
		cout << "Enter MLS Number: (6 digit number): ";
		cin >> mlsIn;

		if ( (mlsIn < MIN_MLS) || (mlsIn > MAX_MAX) ) 
			cout << "Invalid input.  Must be a six digit number." << endl;		

	} while ( (mlsIn < MIN_MLS) || (mlsIn > MAX_MAX) );
	
	return 	mlsIn;
} // end of mlsinput

/*************************************************************************
  FUNCTION:	    priceInput
  DESCRIPTION:  validates price input from user and re-prompts until correct
  INPUT:		
     Parameters:	none
	 File: None
  OUTPUT:
	 Return Val: double priceIn - validated price
     Parameters:	none
	 File: None
  CALLS TO:	none
*************************************************************************/
double priceInput () 
{
	double priceIn; //price input from the user
	do {
		cout << "Enter asking price: ";
		cin >> priceIn;
		
		if ( (priceIn <PRICE_MIN) || (priceIn > MAX_MAX) ) 
			cout << "Invalid input.  Must be a positive amount." << endl;		

	} while ( (priceIn < PRICE_MIN) || (priceIn > MAX_MAX) );
	
		return 	priceIn;
} // end of priceInput

/*************************************************************************
  FUNCTION:	    statusInput
  DESCRIPTION:  validates status input from user and re-prompts until correct
  INPUT:		
     Parameters:	none
	 File: None
  OUTPUT:
	 Return Val: int temp - validated status to be enumerated
     Parameters:	none
	 File: None
  CALLS TO:	none
*************************************************************************/
int statusInput () 
{
	int temp; //input from the user
	do {
		cout << "Enter the status (0-Available, 1-Contract, or 2-Sold): ";
		cin >>  temp;
		
		if ( (temp < MIN_STATUS) || (temp > MAX_STATUS) ) 
			cout << "Invalid input.  Must be 0, 1, or 2." << endl;		

	} while ( (temp < MIN_STATUS) || (temp > MAX_STATUS) );
	
	return 	temp;
} // end of statusInput

/*************************************************************************
  FUNCTION:	    zipInput
  DESCRIPTION:  validates zip code input from user and re-prompts until correct
  INPUT:		
     Parameters:	none
	 File: None
  OUTPUT:
	 Return Val: string zip - validated ZIP COde
     Parameters:	none
	 File: None
  CALLS TO:	none
*************************************************************************/
string zipInput () 
{
	int boolNum,       //keeps error code
		boolZipLength; //keeps error code 
	string  zip; //zip code 
	int location, //location of string
		cell; //string array location

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
  INPUT:		
     Parameters:	none
	 File: None
  OUTPUT:
	 Return Val: string realtor - validated Realtor
     Parameters:	none
	 File: None
  CALLS TO:	none
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
	realtor = " " + realtor;
	return 	realtor;
} // end of realtorInput

/*************************************************************************
  FUNCTION:	    freeList
  DESCRIPTION:  deallocates memory 
  INPUT:		
     Parameters:	first - linked list contians MLS listing data					
	 File: None
  OUTPUT:
	 Return Val: void
     Parameters:	first - linked list contians MLS listing data					
	 File: None
  CALLS TO:	none
*************************************************************************/
void freeList (houseNode* &first)	
{
//local variables  
 houseNode *freeNode, *nextNode; //creates 2 temporary pointers
 freeNode = first;
 while (freeNode != NULL ) { //while not the end of the list
	nextNode = freeNode->next; // point nextode to the node
							   // following freeNode
	delete freeNode; // deallocate the node freeNode points to
	freeNode = nextNode;  // point freeNode to the next node in the list
 }//end while
  
 // afterall nodes have been de-allocated reinitailize to empty list
 first = NULL;
 return;
} // end of freeList

/*************************************************************************
  FUNCTION:	    showDescription
  DESCRIPTION:  displays program description to screen
  INPUT:		
     Parameters:	DESCRIPTION - description of what the program does
	 File: None
  OUTPUT:
	 Return Val: void
     Parameters:	none
	 File: None
  CALLS TO:	none
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
  FUNCTION:	    changeMLS
  DESCRIPTION:  reads MLS listings and updates the price
  INPUT:		
     Parameters:	first - linked list contians MLS listing data
					emptyRecord - used to flag that there is no data
	 File: CHANGES.TXT which contains the MLS and prices to be update
  OUTPUT:
	 Return Val: void
     Parameters:	first - linked list contians MLS listing data
					emptyRecord - used to flag that there is no data
	 File: None
  CALLS TO:	none
*************************************************************************/

void changeMLS (houseNode* &first, bool& emptyRecord)
//void deleteItem (houseNode* &first, int delName)
{
	bool found;     // Flag - have not found the targetName yet
	int  mls;       //MLS numer        
	double increase; //incrase amount from the text file
		   //price;
	emptyRecord = false; //initialize flag to false
	ifstream inData;	 
	inData.open (CHANGES_FILENAME.c_str() );
	
	houseNode* tempPtr;	
		cout << right << endl; 
		if (inData) { // if file opens   
			tempPtr = first; //initialize pointer to see if there is data
			if (tempPtr == NULL) // stop no data
				emptyRecord = true;
			else { //continue on  	 
				//Output Header 
				cout << "MLS Number" << setw(20) << "New Asking Price" << endl;
				cout << "----------" << setw(20)<< "------------------" << endl;
		
				inData >> mls; // priming read			
				//student *tryStu = listTop; // pointer, initialized to top node
				while (inData)
				{
					inData >> increase;
            
					tempPtr = first;
					found = false; 
			
					while ( (tempPtr != NULL) && (found != true) ) { // while haven’t found the student
                     														// we are looking for (targetName)
							                     							// AND haven't hit end of list...				
						if (tempPtr->house.mlsNum == mls)
						{ // Is targetName the name in this
							tempPtr->house.price = tempPtr->house.price + increase;// if targetName is never found in the list.	;
							found = true; // and set found flag to true
					
							cout << setw(6) << mls << setw(12) << tempPtr->house.price << endl;
						}
						else					
							tempPtr = tempPtr->next; // If not, move to next node in list
			
					}  //end while
			
					inData >> mls;			
					    
				} //end while data
		
			} //end there is data	 
		
		} //end if data opens
		else 
			 cout << "Input File does not exist"<< endl;
	inData.close(); 
	
	return;
} //end changeMLS
