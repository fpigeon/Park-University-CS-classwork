/* ----------------------------------------------------------------------------
Author: Frank Pigeon
Class: CS362
Assignment: Week 4
Program reads a data file VEHICLES.txt and creates two seperate parrallel arrays.
Then it will store data in SORTED.TXT in descending order.  Next it will prompt
the user to enter a license plate and output the registration charge associated.
Input: Program will read the VEHICLES.txt
Processing: Create parrallel arrays and sort in descending order and then search
for corresponding charge for license plate entered.
Output: Outputs the license plates on file and the output the registration charge 

FUNCTIONS:	 main - Opens file, sends output, and closes file
            
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
const string DESCRIPTION = "This program does x, y, and z.",
             DEVELOPER = "Frank Pigeon",
             CLASSNUM = "CS362",
             ASSIGNMENT = "Week 4";
//const int NOT_FOUND = -1;  // non-index value indicates 
                           // value not found in list
const string LISTINGS_FILENAME = "LISTINGS.TXT";  //input file 
enum status {AVAILABLE, CONTRACT, SOLD}; // listing status options
const int MAX_LISTINGS = 750; // maximum number of patients
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

/******************************************************************************
//  FUNCTION:	  main
//  DESCRIPTION:  Processes each line in the vehicles file 
//  INPUT: File:  Reads the vehicles.txt and then outputs to parallel arrays 
//  CALLS TO:	  selectSort, binarySearch, alphaChecker, numChecker, displayError
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
//---------------------------------------------------------------------------
    
// showDescription Function    

//---------------------------------------------------------------------------- 

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

//---------------------------------------------------------------------------
    
// readListings Function    

//----------------------------------------------------------------------------    
void readListings(housingRec housingList[], int& num)
{
//local variables
  char errorChoice; // user input if error opening file
  ifstream inData;         // input file stream
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
  } // end if no file 
      

  else {  // file opened successfully     
     inData >> housingList[num].mlsNum;// priming read

     if (num == MAX_LISTINGS) //array bounds
         cout << "Error - too many records in the file" << endl;
     else {// continue on 
        
        while (inData) {
            // read rest of data about listing            
            inData >> housingList[num].price >> temp; // place status into temp value                                               
                   housingList[num].currentStatus = static_cast <status> (temp);                     
            inData >> housingList[num].zip;           
            getline(inData, housingList[num].realtyCompany);   //string with spaces
			inData.ignore (0, '\n'); //ignore remainder of text 			
            num++;     // increment listing count            
            inData >> housingList[num].mlsNum;   // try to read next listing  
        }  // end while
		 
     } // end of else continue on array in bounds
  } // end of else
      
  inData.close();   
} // end of readListings

// **************************************************************************
// FUNCTION:     displayListings 
// DESCRIPTION:  Displays data for all listings, one per line
// INPUT:        Parameter:  patientList - data for all patients
//                           num - count of patients stored in patientList
// **************************************************************************
void displayListings (housingRec housingList[], int count)
{   
 cout << endl;
 cout << "count in is " << count << endl;
 cout << setw(14) << "Asking" << setw(10) << "Listing" << endl;
 cout << "MLS#    Price    Status     Zip Code    Realtor" << endl;
 cout << "------  -------  ---------  ----------  -------------------" << endl;

 for (int num = 0; num < count; num++) {
      cout << right << setw(5) << housingList[num].mlsNum
       << setw(9) << housingList[num].price << "  "
       << left << setw(11) << convertStatusToString (housingList[num].currentStatus)
       << right << setw(10) << housingList[num].zip << " "
       << left << setw(21) << housingList[num].realtyCompany << endl;	
} // end for
      
  cout << endl;  
  return;
} // end displayListings

// **************************************************************************
// FUNCTION:     convertStatusToString 
// DESCRIPTION:  Displays data for all listings, one per line
// INPUT:        Parameter:  patientList - data for all patients
//                           num - count of patients stored in patientList
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
// DESCRIPTION:  Displays data for all listings, one per line
// INPUT:        Parameter:  patientList - data for all patients
//                           num - count of patients stored in patientList
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
	cout << "Please choose an option from the menu: ";
	cin >> menuChoice;
	menuChoice = toupper (menuChoice); //uppercase user input
return menuChoice;
} // end of displayMenu

// **************************************************************************
// FUNCTION:     checkMenuChoice 
// DESCRIPTION:  Displays data for all listings, one per line
// INPUT:        Parameter:  patientList - data for all patients
//                           num - count of patients stored in patientList
// **************************************************************************
char checkMenuChoice (char& menuChoice)
{	
while( (menuChoice != 'D') && (menuChoice != 'A') && (menuChoice != 'R') && (menuChoice != 'E') ) { //loop until valid input 	
	if ( (menuChoice != 'D') && (menuChoice != 'A') && (menuChoice != 'R') && (menuChoice != 'E') )
				cout << "Invalid Input- Please select D, A, R or E" << endl;
				menuChoice = displayMenu();
}    // end while 

return menuChoice;
} // end of checkMenuChoice

// **************************************************************************
// FUNCTION:     menuAction 
// DESCRIPTION:  Displays data for all listings, one per line
// INPUT:        Parameter:  patientList - data for all patients
//                           num - count of patients stored in patientList
// **************************************************************************
void menuAction (char menuChoice, housingRec housingList[], int& listingCount, bool& quit)
{
 int mlsDelete; //user input to delete an MLS Listing
 bool deleted; // index to delete
 char commitChange; // user input to commit changes

switch (menuChoice) {
	case 'D':  //Display All
		cout << "You choose D " << endl;
		if (listingCount > 0)
			displayListings (housingList, listingCount);
		else 
			cout << "There are no records in the file." << endl;
		break;
	case 'A':  //Add a Listing
		cout << "You choose A " << endl;
		addListings (housingList, listingCount);
		break;
	case 'R': // Remove a Listing
		cout << "You choose R " << endl;
		displayMLS (housingList, listingCount);
		cout << "Enter MLS Listing to delete: ";
		cin >> mlsDelete;
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
		cout << "You choose E " << endl;
		cout << "Do you want to commit changes to file (Y/N)?: ";
		cin >> commitChange;
		commitChange = toupper (commitChange);
		if (commitChange == 'Y')
			writeListings(housingList, listingCount);
		quit = true;
		break;
} //end switch

} // end of menuAction

// **************************************************************************
// FUNCTION:     loadExistingData 
// DESCRIPTION:  Displays data for all listings, one per line
// INPUT:        Parameter:  patientList - data for all patients
//                           num - count of patients stored in patientList
// **************************************************************************
// ask user to use existing data file
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

// **************************************************************************
// FUNCTION:     addListings 
// DESCRIPTION:  Displays data for all listings, one per line
// INPUT:        Parameter:  patientList - data for all patients
//                           num - count of patients stored in patientList
// **************************************************************************
void addListings (housingRec housingList[], int& count)
{  		
	char newListing;
	int mls;
	double price;
	int temp;	
	string zip, realtor;
	
	while ( (count >= MAX_LISTINGS) || ( newListing != 'N' ) ) {            		
		cout << "Enter MLS Number:   ";
		cin >> mls;
		housingList[count].mlsNum = mls;		

		cout << "Enter asking price: ";
		cin >> price;
		housingList[count].price = price;		

		cout << "Enter the status (0-Available, 1-Contract, or 2-Sold): ";
		cin >>  temp;
		housingList[count].currentStatus = static_cast <status> (temp);				

		cout << "Enter the ZIP code (5digits-4digit): ";
		cin >>  zip;
		housingList[count].zip = zip;
		cin.ignore();	
		
		cout << "Enter the Realtor:  ";		
		getline (cin, realtor);		
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

// **************************************************************************
// FUNCTION:     deleteItem 
// DESCRIPTION:  Implementation of deleting an item from an ordered list
// INPUT:        Parameter:  patientList - data for all patients
//                           num - count of patients stored in patientList
// **************************************************************************
// Implementation of deleting an item from an ordered list
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
     cout << "place found is " << placeFound << endl;
	 cout << "listingcount is " << listingCount << endl;
     //copy the record in the last occuppied index location within the array over the record
	 
     //housingList[placeFound] = housingList[listingCount - 1];	 	 
	 cout << "MLS of placefound is now " << housingList[placeFound].mlsNum << endl;
     for (int num = placeFound + 1; num < listingCount; num++)
     housingList[num - 1] = housingList[num];     // Decrement list size
     listingCount--;

     // Zero out deleted last item
     //housingList[listingCount] = 0;	// Optional

     deleted = true;       
  }  // end if
  cout << "listingcount out of loop is " << listingCount << endl;  
  return;
} //end of deleteItem

// **************************************************************************
// FUNCTION:     displayMLS 
// DESCRIPTION:  Displays data for all listings, one per line
// INPUT:        Parameter:  patientList - data for all patients
//                           num - count of patients stored in patientList
// **************************************************************************
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

// **************************************************************************
// FUNCTION:     writeListings 
// DESCRIPTION:  Displays data for all listings, one per line
// INPUT:        Parameter:  patientList - data for all patients
//                           num - count of patients stored in patientList
// **************************************************************************
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
