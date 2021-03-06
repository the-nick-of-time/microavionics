#include <p18cxxx.h>
#include <delays.h>
#include <string.h>
#include "lcd.h"
#include "bool.h"

/*
NOTE TO MATT: lcd.h only defines the public-facing functions, you will likely need to define additional helper functions like one to draw one pixel on the GLCD at a specified location
*/
/*------------------------------------------------------------------------------
 * Definitions for this GLCD interface for EasyPic Pro v7
 -----------------------------------------------------------------------------*/

/* CS Pin Assignments */
#define GLCD_CS0_TRIS   TRISBbits.TRISB0
#define GLCD_CS0_LAT    LATBbits.LATB0

#define GLCD_CS1_TRIS   TRISBbits.TRISB1
#define GLCD_CS1_LAT    LATBbits.LATB1

/* RS Pin Assignments */
#define GLCD_RS_TRIS     TRISBbits.TRISB2
#define GLCD_RS_LAT      LATBbits.LATB2

/* RW Pin Assignments */
#define GLCD_RW_TRIS TRISBbits.TRISB3
#define GLCD_RW_LAT LATBbits.LATB3

/* E Pin Assignments */
#define GLCD_E_TRIS	TRISBbits.TRISB4
#define GLCD_E_LAT	LATBbits.LATB4

/* RST Pin Assignments */
#define GLCD_RST_TRIS   TRISBbits.TRISB5
#define GLCD_RST_LAT    LATBbits.LATB5

/* Data Pin Assignments. The GLCD uses the full byte so this is fairly straightforward
*/
#define GLCD_DATA_TRIS TRISD
#define GLCD_DATA_LAT LATD
#define GLCD_DATA_PORT PORTD

/*------------------------------------------------------------------------------
 * Static variables for internal use (not visible outside of this file)
 -----------------------------------------------------------------------------*/

/* The following instruction delays assume 16MHz clock. These can be changed for
** other oscillator frequencies but note the valid range is only 0-255
*/
static const unsigned char EnableDelayCount_ = 2; // 5 us delay
static const unsigned char DataDelayCount_ = 4; // 1 us delay


void WriteData(unsigned char data);
void Enable( void );
void InitGLCD( void );
void SetColumn(unsigned char col);
void SetPage(unsigned char page);
void SetCursor(unsigned char col, unsigned char page);
void ClearLine(unsigned char page, unsigned char half);
void ClearGLCD( void );
void disp_char(char data[],int col,int page);


// Initialize the GLCD, return true when done successfully
bool startup_glcd(void){
    GLCD_CS0_TRIS = 0;
    GLCD_CS1_TRIS = 0;
    GLCD_RS_TRIS = 0;
    GLCD_RW_TRIS = 0;
    GLCD_E_TRIS = 0;
    GLCD_RST_TRIS = 0;
    GLCD_DATA_TRIS = 0;

    InitGLCD();
    return true;
}
// Print a string on the GLCD starting at x and y
bool write_string(char* string, int col, int page){
    unsigned char i;
    char data[15];
    for(i=0; string[i]!=0x00;i++){
        switch (string[i]) {
            case 'M':
                data[0]=0xFF;data[1]=0x06;data[2]=0x18;data[3]=0x06;data[4]=0xFF;
                break;
            case 'Y':
                data[0]=0x01;data[1]=0x06;data[2]=0xF8;data[3]=0x06;data[4]=0x01;
                break;
            case ' ':
                data[0]=0x00;data[1]=0x00;data[2]=0x00;data[3]=0x00;data[4]=0x00;
                break;
            case 'T':
                data[0]=0x01;data[1]=0x01;data[2]=0xFF;data[3]=0x01;data[4]=0x01;
                break;
            case 'U':
                data[0]=0x0F;data[1]=0x70;data[2]=0x80;data[3]=0x70;data[4]=0x0F;
                break;
            case 'R':
                data[0]=0xFF;data[1]=0x11;data[2]=0x31;data[3]=0x4A;data[4]=0x84;
                break;
            case 'N':
                data[0]=0xFF;data[1]=0x06;data[2]=0x18;data[3]=0x60;data[4]=0xFF;
                break;
            case 'H':
                data[0]=0xFF;data[1]=0x18;data[2]=0x18;data[3]=0x18;data[4]=0xFF;
                break;
            case 'E':
                data[0]=0xFF;data[1]=0xDB;data[2]=0xDB;data[3]=0xDB;data[4]=0xDB;
                break;
            case 'I':
                data[0]=0x81;data[1]=0x81;data[2]=0xFF;data[3]=0x81;data[4]=0x81;
                break;
            default:
                break;
        }
        Nop();
        disp_char(data,col+5*i,page);
    }
}

void disp_char(char data[],int col,int page){
    unsigned char j;
    for(j=0; j<5; j++){
        SetCursor(col+j,page);
        WriteData(data[j]);
    }
}
// Draws the whole board, probably will only be called at the beginning on the empty board
bool draw_board(Board* board){
    unsigned char x;
    unsigned char y;
    Cell* cell;
    for(x=0;x<8;x++){
        for(y=0;y<8;y++){
            cell = get_cell(board, y, x);
            draw_cell(*cell, x, y);
        }
    }
}

// Draws an individual cell, probably called every time a target is chosen
bool draw_cell(Cell cell, char x, char y){
    unsigned char j;
    unsigned char col;
    unsigned char mod;
    if (cell.targeted){
        if (cell.occupied){
            for(j=0;j<8;j++){
                col = 8*x+j;
                SetCursor(col,y);
                mod = col%8;
                switch (mod) {
                case 0:
                    WriteData(0x80);
                    break;
                case 1:
                    WriteData(0x22+0x80);
                    break;
                case 2:
                    WriteData(0x14+0x80);
                    break;
                case 3:
                    WriteData(0x08+0x80);
                    break;
                case 4:
                    WriteData(0x14+0x80);
                    break;
                case 5:
                    WriteData(0x22+0x80);
                    break;
                case 6:
                    WriteData(0x80);
                    break;
                case 7:
                    WriteData(0xFF);
                    break;
                default:
                    break;
                }
            }
        }
        else{
            for(j=0;j<8;j++){
                col = 8*x+j;
                SetCursor(col,y);
                mod = col%8;
                switch (mod) {
                case 0:
                    WriteData(0x80);
                    break;
                case 1:
                    WriteData(0x1C+0x80);
                    break;
                case 2:
                    WriteData(0x22+0x80);
                    break;
                case 3:
                    WriteData(0x22+0x80);
                    break;
                case 4:
                    WriteData(0x22+0x80);
                    break;
                case 5:
                    WriteData(0x1C+0x80);
                    break;
                case 6:
                    WriteData(0x80);
                    break;
                case 7:
                    WriteData(0xFF);
                    break;
                default:
                    break;
            }
            }
        }
    }
    else{
        for(j=0;j<8;j++){
            col = 8*x+j;
            SetCursor(col,y);
            mod = col%8;
            switch (mod) {
			case 0:
				WriteData(0x80);
				break;
			case 1:
				WriteData(0x80);
				break;
			case 2:
				WriteData(0x80);
				break;
			case 3:
				WriteData(0x80);
				break;
			case 4:
				WriteData(0x80);
				break;
			case 5:
				WriteData(0x80);
				break;
			case 6:
				WriteData(0x80);
				break;
			case 7:
				WriteData(0xFF);
				break;
			default:
				break;
            }
        }
    }
}

/******************************************************************************
 *     Function Name:	WriteData
 *     Parameters:      unsigned char data
 *     Description:		This function sends a byte to the GLCD by setting LATD
 *                      and cycling the enable pin. To write data to the GLCD,
 *                      RS must be logic HIGH and RW must be logic LOW.
 *
 ******************************************************************************/
void WriteData(unsigned char data){
    GLCD_RS_LAT = 1;      // RS must be logic HIGH with RW logic LOW to tell the
    GLCD_RW_LAT = 0;      // GLCD that data is being written to it.

    GLCD_DATA_LAT = data; // Set the Data LAT line to the data input.

    Delay1TCY();Delay1TCY();Delay1TCY();Delay1TCY(); // Delay 1 us

    Enable();
}

/******************************************************************************
 *     Function Name:	Enable
 *     Parameters:      None
 *     Description:		This function switches the EN pin of the GLCD to enable
 *                      data or an instruction
 *
 ******************************************************************************/
void Enable( void ){
    GLCD_E_LAT = 1;  // The GLCD E pin must first be driven logic HIGH...
    Delay10TCYx( EnableDelayCount_ ); // Delay 5 us
    GLCD_E_LAT = 0;  // and then driven logic LOW to submit the command.
    Delay10TCYx( EnableDelayCount_ );

}

/******************************************************************************
 *     Function Name:	InitGLCD
 *     Parameters:      None
 *     Description:		This function initializes the 123x64 WDG0151 graphic LCD
 *                      by sending the display on command to both controllers
 *
 ******************************************************************************/
void InitGLCD( void ){
    GLCD_CS0_LAT = 1;   // Turn both GLCD controllers off.
    GLCD_CS1_LAT = 1;
    GLCD_RST_LAT = 1;   // Enable GLCD RESET.
    GLCD_CS0_LAT = 0;
    GLCD_CS1_LAT = 0;   // Turn both controllers on again.
    GLCD_RS_LAT = 0;    // RS and RW instruction write configuration.
    GLCD_RW_LAT = 0;
    GLCD_DATA_LAT = 0x3F;   // This command turns the GLCD on.
    Enable();
    ClearGLCD();
}

/******************************************************************************
 *     Function Name:	SetColumn
 *     Parameters:      unsigned char col
 *     Description:		This function sets the cursor to the column on the GLCD
 *                      specified by col, which is a number from 0 to 127
 *
 ******************************************************************************/
void SetColumn(unsigned char col){
    GLCD_RS_LAT = 0;    // RS and RW low sets the GLCD to take instructions.
    GLCD_RW_LAT = 0;
    if(col < 64){       // If col is within the first 64 columns then it is
        GLCD_CS0_LAT = 0; // being controlled by the first controller.
        GLCD_CS1_LAT = 1;
    }else{              // Otherwise it is being controlled by the second one.
        GLCD_CS0_LAT = 1;
        GLCD_CS1_LAT = 0;
        col = col - 64; // No matter what, both controllers need a value 0 - 63.
    }
    col = (col | 0x40) & 0x7F; // To set Y address (column), DB7 must be LOW and
    GLCD_DATA_LAT = col;        // DB6 must be HIGH - hence the OR. Then the AND
    Enable();                   // sets the col values and retains the previous
}                               // settings.

/******************************************************************************
 *     Function Name:	SetPage
 *     Parameters:      unsigned char page
 *     Description:		This function sets the page of the GLCD. Can be from 0
 *                      to 7.
 *
 ******************************************************************************/
void SetPage(unsigned char page){
    if(page > 7){       // Checks that the input is within the allowable range.
        return;
    }
    GLCD_RS_LAT = 0;    // Set RS and RW for an instruction.
    GLCD_RW_LAT = 0;
    page = (page | 0xB8) & 0xBF; // Similar to setting the column, setting the X
    GLCD_DATA_LAT = page;         // location (page) requires setting DB7 and
    Enable();                     // DB5:3 HIGH, DB6 LOW, and setting DB2:0 to
}                                 // the desired page value.

/******************************************************************************
 *     Function Name:	SetCursor
 *     Parameters:      unsigned char x, unsigned char y
 *     Description:		This function sets the page and column of the GLCD.
 *
 ******************************************************************************/
void SetCursor(unsigned char col, unsigned char page){
    SetColumn(col);     // Sets the desired column (X) and page (Y) together.
    SetPage(page);      // SetPage does not need to change CS since it is already
}                         // set using SetColumn.

/******************************************************************************
 *     Function Name:	ClearLine
 *     Parameters:      unsigned char page, unsigned char half
 *     Description:		This function completely clears the specified page (from
 *                      0 to 7) and half (0 being left and 1 being right) of the
 *                      GLCD.
 *
 ******************************************************************************/
void ClearLine(unsigned char page, unsigned char half){
    unsigned char i;
    if(half == 0){          // Half 0 is the left side of the GLCD.
        GLCD_CS0_LAT = 0;
        GLCD_CS1_LAT = 1;
        for(i=0;i<65;i++){      // Loop through every column of the chosen page.
            SetCursor(i,page);  // Set page and column.
            WriteData(0x00);    // Writing 0x00 essentially "clears" that column.
        }
    }else if(half == 1){    // Half 1 is the right side of the GLCD.
        GLCD_CS0_LAT = 1;
        GLCD_CS1_LAT = 0;
        for(i=65;i<128;i++){
            SetCursor(i,page);
            WriteData(0x00);
        }
    }else{
        return;
    }
}

/******************************************************************************
 *     Function Name:	ClearGLCD
 *     Parameters:      None
 *     Description:		This function completely clears the entire GLCD screen.
 *
 ******************************************************************************/
void ClearGLCD( void ){
    unsigned char j;
    for(j=0;j<8;j++){       // Loops through every page on both halves of the
        ClearLine(j,0);     // GLCD to clear the entire thing.
        ClearLine(j,1);
    }
}
