// Julian Reyes jar160830@utdallas.edu CS 3377.501
// CDK and Binary File I/O

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <iomanip>
#include <sstream>
#include <string>

#include "cdk.h"   // Includes header file for cdk function


// Definitions for CDK Matrix
#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Binary File Contents"   // Title of matrix
 


const int maxRecordStringLength = 25;       // Constant used for max length of string 
 


using namespace std;


// Binary file format has 3 header items and is immediately followed by records
//
// Class BinaryFileHeader consists of values that will be read in from binary file
class BinaryFileHeader
{
public:

  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;

};


// Class BinaryFileRecord consists of a string length and string that wil be read from file -> will use an array
//  of this class to have mutliple objects holding these values from the file
class BinaryFileRecord
{
public:
  
  uint8_t strLength;                             // 8-bit integer -> need to change to 
  char stringBuffer[maxRecordStringLength];      // char array (string) to hold string read from binary file

};




// Start of main
int main(int argc, char* argv[]) {



  // Creates an instance of the object from class BinaryFileHeader, uses a pointer to point to address of obj
  BinaryFileHeader *myHeader = new BinaryFileHeader();


  // Opening ifstream to read form binary file, opens file for input and uses binary flag 
  ifstream binInfile ("cs3377.bin", ios::in | ios::binary);



  // If file cannot be opened, output error and exit
  if (binInfile.fail()) {
    cerr << "Error: Cannot open input file" << endl; 
    return 1;
  }

  // Read binary file using BinaryFileHeader for size (how much to read from file) 
  binInfile.read((char *) myHeader, sizeof(BinaryFileHeader));
  




  // Create an array of BinaryRecord objects
  BinaryFileRecord *myRecords = new BinaryFileRecord[myHeader->numRecords]();


  // Initializes the myRecord array by incrementing the address of pointer for each new set of entries
  for (uint32_t i = 0; i < myHeader->numRecords; i++) {         // Use uint32_t for i bc numRecords is uint64_t
    // Read binary file using BinaryFileRecords for how much to read, index through each obj by adding i
    binInfile.read((char *)(myRecords + i), sizeof(BinaryFileRecord));
  }



  // Closes binaryfle -> no leakage
  binInfile.close();



  // Creates a string stream to build string from read uints in obj
  stringstream ss;



  // Collect BinaryFileRecords data
  //
  // Creates array to hold data values for stringlengths and stringrecords (more than 1)
  string strlengths[myHeader->numRecords],
         strRecords[myHeader->numRecords];

  

  // Uses for loop to step through each record and build a string with data from file
  for (uint32_t i = 0; i < myHeader->numRecords; i++) {
    
    ss.clear();           // Clears stream before use to not have conflicting data


    // Write strLength to stream
    ss << unsigned((myRecords + i)->strLength);     // Cast to unsigned to read (or uint16_t etc)
    
    // Write what's in stream (separated by spaces) to string array
    ss >> strlengths[i];
    
    // Writw the given string (from stringBuffer) to the string array
    strRecords[i] = (myRecords + i)->stringBuffer;
  }




  // Declare string to hold values after they are built
  string hexVal,
         vNum,
         nRecords;
 
  // Collect BinaryFileHeader data
  //
 
  ss.clear();        // Clear stream before use 

  // Build string with supplied substrings and converted magicNumber
  ss << "0x" << hex << uppercase << myHeader->magicNumber << endl;   // All number values from here on are in hex
  ss >> hexVal;
 

  ss.clear();
  
  // Build string for versionNumber, write to string
  ss << dec << myHeader->versionNumber;                 // Must change back to decimal
  ss >> vNum;
 
 

  
  ss.clear();

  // Build string for numRecords, write to string
  ss << myHeader->numRecords;
  ss >> nRecords;
  





  // Begin CDK implementation
  //
  
  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"", "a", "b", "c"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  
  // Initialize the Cdk screen. You must czech terminal is large enough to prevent coredump
  window = initscr();
  cdkscreen = initCDKScreen(window);

  // Start CDK Colors 
  initCDKColor();

  
  // Create the matrix.  Need to manually cast (const char**) to (char **)
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);
  // If the matrix is null error out
  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }


  

  // Display the Matrix 
  drawCDKMatrix(myMatrix, true);

  

  // Build strings to be displayed in matrix, data value to beginning of message 
  string magic = "Magic: ";
  magic.append(hexVal);

  string version = "Version: ";
  version.append(vNum);

  string numRecords = "NumRecords: ";
  numRecords.append(nRecords);
  

  ss.clear();

  
  string strlen;



  // Dipslay a message
  //
  // Needs c-style string in 4th argument
  setCDKMatrixCell(myMatrix, 1, 1,magic.c_str());   
  setCDKMatrixCell(myMatrix, 1, 2, version.c_str());
  setCDKMatrixCell(myMatrix, 1, 3, numRecords.c_str());


  // After displaying BinaryFileHeader info, go into loop to display strlen and stringVal in appropriate location
  for (uint32_t i = 0; i < myHeader->numRecords; i++) {

    // Begin building strlen
    strlen = "strlen: ";
    strlen.append(strlengths[i]);    // Append the length to the "strlen: "

    // Set cell in appropriate locations 
    setCDKMatrixCell(myMatrix, i+2, 1, strlen.c_str());        // strlen will be in 1st col and multiple rows
    setCDKMatrixCell(myMatrix, i+2, 2, strRecords[i].c_str()); // records will be in 2nd col and multiple rows
  
  }

  
  // Set matrix after filling
  drawCDKMatrix(myMatrix, true);    /* required  */

  // So we can see results, pause until a key is pressed
  unsigned char x;
  cin >> x;            // Exits CDK window upon keystroke



  // Cleanup screen
  endCDK();



} // End of main
