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
  char stringBuffer[maxRecordStringLength];

};




int main(int argc, char* argv[]) {

  
  BinaryFileHeader *myHeader = new BinaryFileHeader();

  ifstream binInfile ("cs3377.bin", ios::in | ios::binary);








  // If file cannot be opened, output error and exit
  if (binInfile.fail()) {
    cerr << "Error: Cannot open input file" << endl; 
    return 1;
  }


  binInfile.read((char *) myHeader, sizeof(BinaryFileHeader));
  



  //cout << hex << myHeader->magicNumber << endl;
   // cout << myHeader->versionNumber << endl;
  //  cout << myHeader->numRecords << endl;
 





  // Create an array of BinaryRecord objects
  BinaryFileRecord *myRecords = new BinaryFileRecord[myHeader->numRecords]();


  // Initializes the myRecord array by incrementing the address of pointer for each new set of entries
  for (uint32_t i = 0; i < myHeader->numRecords; i++) {
    binInfile.read((char *)(myRecords + i), sizeof(BinaryFileRecord));
  }


  binInfile.close();



  stringstream ss;
  string strlengths[myHeader->numRecords],
         strRecords[myHeader->numRecords];

  //******* strLen not appearing
  for (uint32_t i = 0; i < myHeader->numRecords; i++) {
    
    ss.clear();
    ss << unsigned((myRecords + i)->strLength);          // Cast to unsigned to read (or uint16_t etc)
    ss >> strlengths[i];
    
    strRecords[i] = (myRecords + i)->stringBuffer;
  }



  
  //cout << hex << myHeader->magicNumber << endl;
  //cout << myHeader->versionNumber << endl;
  // cout << myHeader->numRecords << endl;
  






  //stringstream ss;
  ss.clear();

  ss << "0x" << hex << uppercase << myHeader->magicNumber << endl;   // All number values from here on are in hex
  string hexVal;
  ss >> hexVal;
  //cout << hexVal << endl;

  ss.clear();
  
  //c_str() for c-style string
  string vNum;
  ss << dec << myHeader->versionNumber;                 // Must change back to decimal
  ss >> vNum;
  // cout << vNum << endl;
  

  // Clears stream to free up space
  ss.clear();

  string nRecords;
  // cout << myHeader->numRecords << endl;
  ss << myHeader->numRecords;
  ss >> nRecords;
  //  cout << nRecords << endl;





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

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }


  



  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Dipslay a message
   */


  string magic = "Magic: ";
  magic.append(hexVal);

  string version = "Version: ";
  version.append(vNum);

  string numRecords = "NumRecords: ";
  numRecords.append(nRecords);
  


  ss.clear();


  
  
  string strlen;


  // Needs c-style string
  setCDKMatrixCell(myMatrix, 1, 1,magic.c_str());
  setCDKMatrixCell(myMatrix, 1, 2, version.c_str());
  setCDKMatrixCell(myMatrix, 1, 3, numRecords.c_str());


  
  for (uint32_t i = 0; i < myHeader->numRecords; i++) {
    strlen = "strlen: ";
    strlen.append(strlengths[i]);

    
    //strRec.append(strRecords[i]);
    setCDKMatrixCell(myMatrix, i+2, 1, strlen.c_str());
    setCDKMatrixCell(myMatrix, i+2, 2, strRecords[i].c_str());
    
  }

  /*
  setCDKMatrixCell(myMatrix, 2, 1, "strlen: " + (myRecords)->strLength);
  setCDKMatrixCell(myMatrix, 3, 1, "strlen: " + (myRecords + 1)->strLength);
  setCDKMatrixCell(myMatrix, 4, 1, "strlen: " + (myRecords + 2)->strLength);
  setCDKMatrixCell(myMatrix, 5, 1, "strlen: " + (myRecords + 3)->strLength);
  */
  //setCDKMatrixCell(myMatrix, 2, 1, stringBuff.c_str());
  //setCDKMatrixCell(myMatrix, 3, 1, (myRecords + 1)->stringBuffer);
  //setCDKMatrixCell(myMatrix, 4, 1, (myRecords + 2)->stringBuffer);
  // setCDKMatrixCell(myMatrix, 5, 1, (myRecords + 3)->stringBuffer);
  

  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
