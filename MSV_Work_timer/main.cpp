/*
  AUTHOR        Tomasz Jokiel
  DATE          12.2011
  APPVERSION    2.4.5               // Don't write empty fields with "0:00:00" to "Data_base.xml", static library linking
  APPVERSION    2.4.0               // "End Day" additionally create "Data_base.xml" file with stored day summary
  APPVERSION    2.3.3               // ENTER can confirm field "Add time" (Working like button "Add")
  APPVERSION    2.3.2               // Improve add time parsing value (avalible formats: 1,5; 1.5; 1:30; 1h; 1m; 1s; and minus values )
  APPVERSION    2.3.1               // Added help Question mark - About
  APPVERSION    2.3.0               // Added one edit field and colored editfield
  APPVERSION    2.2.0               // Changed Layout: add borders & status bar and fix background coloring
  APPVERSION    2.0.0               // Changed Layout
  APPVERSION    1.1.0               // Added field "Add time"
*/

#define   _WIN32_WINNT    0x0501
#include  <windows.h>
#include  <commctrl.h>               // for Common Controls library
#include  <stdio.h>
#include  <stdarg.h>
#include  "global.h"
#include  "functions.h"
#include  "res.h"                    // resources definitions


using namespace std;
// **************************************************************************************
// Local definition
#define ACTIVE_EDIT_BK_COLOR  RGB(230,255,230)
#define ADD_FIELD_BK_COLOR    RGB(255,255,210)
#define WINDOW_WIDTH          320
#define WINDOW_HEIGTH         215
#define GRID_TOP              20
#define GRID_LEFT             7
#define SEPARATE              5
#define BUTTON_WIDTH          60
#define BUTTON_HEIGTH         20
#define INPUT_GRID            15
#define INPUT_WIDTH           140
#define INPUT_ADD_WIDTH       70
#define INPUT_HEIGTH          20
#define TIMERS_V_GRIDE        160
#define TIMERS_SEPAR          2
#define TIMMERS_HEIGTH        16
#define RADIO_DIMMENSION      13
#define RADIO_WIDTH           13
#define RADIO_WIDTH_EX        60
#define RADIO_HEIGTH          13
#define ADD_TIME_V_GRID       237
#define STATUSBAR_H_GRID      195
#define DELTA_TIME_V_GRID     170
// **************************************************************************************
// **************************************************************************************
// Global Variables:
HINSTANCE g_hInstance;                      // current instance
BOOL      g_bTimersStart = FALSE;           // state of timers

INT       g_iBuffer;
UINT      g_uiCounterTmp;
UINT      g_uiLicznik1 = 0;
UINT      g_uiLicznik2 = 0;
UINT      g_uiLicznik3 = 0;
UINT      g_uiLicznik4 = 0;
UINT      g_uiLicznik5 = 0;

// brushes
HBRUSH    g_hBrush1 = CreateSolidBrush (ACTIVE_EDIT_BK_COLOR);
HBRUSH    g_hBrush2 = CreateSolidBrush (ADD_FIELD_BK_COLOR);

// borders
HWND      g_hBorder_1;
HWND      g_hBorder_2;

// Statusbar
HWND      g_hStatusBar;

// buttons
HWND      g_hButton_start;
HWND      g_hButton_pause;
HWND      g_hButton_stop;    // Release Hours
HWND      g_hButton_reset;
HWND      g_hButton_AddTime;

// radiobuttons
HWND      g_hButton_radio_1;
HWND      g_hButton_radio_2;
HWND      g_hButton_radio_3;
HWND      g_hButton_radio_4;
HWND      g_hButton_radio_5;
HWND      g_hButton_radio_6;   // Timer
HWND      g_hButton_radio_7;   // Day

// editboxes
HWND      g_hEditBox_1;
HWND      g_hEditBox_2;
HWND      g_hEditBox_3;
HWND      g_hEditBox_4;
HWND      g_hEditBox_5;
HWND      g_hEditBox_6;
HWND      g_hActiveEditBox;        // pointer to active edit control

string    g_sIniFileName                    = "Config.ini";
TCHAR     g_atcProgramPathBuffer[MAX_PATH]  = {0};
CHAR      g_acIniPath[MAX_PATH]             = {0};

CHAR      g_acTimerName_1[MAX_PATH]         = {0};
CHAR      g_acTimerName_2[MAX_PATH]         = {0};
CHAR      g_acTimerName_3[MAX_PATH]         = {0};
CHAR      g_acTimerName_4[MAX_PATH]         = {0};
CHAR      g_acTimerName_5[MAX_PATH]         = {0};
UCHAR     g_ucSelectedTimer                 = 1;
INT       g_iGlobalHoursDelta               = 0;
INT       g_iWeekHoursDelta                 = 0;
INT       g_iMonthHoursDelta                = 0;
INT       g_iAdditionalDayTime              = 0;            // Additional time for some user activities
HWND      g_hWnd;                                           /* This is the handle for our window */

// **************************************************************************************

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure      (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditProc             (HWND, UINT, WPARAM, LPARAM);         // For editbox "Add Time"
BOOL    CALLBACK DlgReleaseHoursProc  (HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDPROC wndProc;

/*  Make the class name into a global variable  */
char szClassName[ ]       = "WindowsApp";
char szMainWindowName[ ]  = "Work Timer";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    MSG        messages;     /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    HRSRC      hStyleXsl;    /* Handle for style file (*.xsl) */
    HGLOBAL    phStyleXslMem;
    DWORD      dwResourceSize;
    HACCEL     hAccelTable;                                                                         // Accelerators (i.e. Keyboard shortcut)
    
    GetCurrentDirectory(MAX_PATH, g_atcProgramPathBuffer);
    strcpy_s(g_acIniPath, g_atcProgramPathBuffer);
    strcat_s(g_acIniPath, ("\\"+g_sIniFileName).c_str());
    
  if(DEFAULT_ERROR == GetPrivateProfileInt("Settings", "enabled", DEFAULT_ERROR, g_acIniPath))          // Check if file exist - to do this try to read any value
  {
    // Write default configuration
    WritePrivateProfileString("Settings",   "enabled", "true",  g_acIniPath);

    WritePrivateProfileString("StateOfTimers",   "Timer_1", "0",  g_acIniPath);
    WritePrivateProfileString("StateOfTimers",   "Timer_2", "0",  g_acIniPath);
    WritePrivateProfileString("StateOfTimers",   "Timer_3", "0",  g_acIniPath);
    WritePrivateProfileString("StateOfTimers",   "Timer_4", "0",  g_acIniPath);
    WritePrivateProfileString("StateOfTimers",   "Timer_5", "0",  g_acIniPath);

    WritePrivateProfileString("NamesOfTimers",   "Timer_1", "",  g_acIniPath);
    WritePrivateProfileString("NamesOfTimers",   "Timer_2", "",  g_acIniPath);
    WritePrivateProfileString("NamesOfTimers",   "Timer_3", "",  g_acIniPath);
    WritePrivateProfileString("NamesOfTimers",   "Timer_4", "",  g_acIniPath);
    WritePrivateProfileString("NamesOfTimers",   "Timer_5", "",  g_acIniPath);

    WritePrivateProfileString("Settings",   "GlobalHoursDelta", "0",  g_acIniPath);
    WritePrivateProfileString("Settings",   "WeekHoursDelta"  , "0",  g_acIniPath);
  }
  else
  {
    // Read Configuration
    g_uiCounterTmp = GetPrivateProfileInt("StateOfTimers", "Timer_1", DEFAULT_ERROR, g_acIniPath);
    if(DEFAULT_ERROR != g_uiCounterTmp)
    {
      g_uiLicznik1 = g_uiCounterTmp;
    }

    g_uiCounterTmp = GetPrivateProfileInt("StateOfTimers", "Timer_2", DEFAULT_ERROR, g_acIniPath);
    if(DEFAULT_ERROR != g_uiCounterTmp)
    {
      g_uiLicznik2 = g_uiCounterTmp;
    }

    g_uiCounterTmp = GetPrivateProfileInt("StateOfTimers", "Timer_3", DEFAULT_ERROR, g_acIniPath);
    if(DEFAULT_ERROR != g_uiCounterTmp)
    {
      g_uiLicznik3 = g_uiCounterTmp;
    }

    g_uiCounterTmp = GetPrivateProfileInt("StateOfTimers", "Timer_4", DEFAULT_ERROR, g_acIniPath);
    if(DEFAULT_ERROR != g_uiCounterTmp)
    {
      g_uiLicznik4 = g_uiCounterTmp;
    }
    
    g_uiCounterTmp = GetPrivateProfileInt("StateOfTimers", "Timer_5", DEFAULT_ERROR, g_acIniPath);
    if(DEFAULT_ERROR != g_uiCounterTmp)
    {
      g_uiLicznik5 = g_uiCounterTmp;
    }

    // Global delta timer 
    g_iGlobalHoursDelta = GetPrivateProfileInt("Settings", "GlobalHoursDelta", DEFAULT_ERROR, g_acIniPath);
    if(DEFAULT_ERROR == g_iGlobalHoursDelta)
    {
      g_iGlobalHoursDelta = 0;
    }

    // Weekly delta timer
    g_iWeekHoursDelta = GetPrivateProfileInt("Settings", "WeekHoursDelta", DEFAULT_ERROR, g_acIniPath);
    if(DEFAULT_ERROR == g_iWeekHoursDelta)
    {
      g_iWeekHoursDelta = 0;
    }

    // Month delta timer
    g_iMonthHoursDelta = GetPrivateProfileInt("Settings", "MonthHoursDelta", DEFAULT_ERROR, g_acIniPath);
    if(DEFAULT_ERROR == g_iMonthHoursDelta)
    {
      g_iMonthHoursDelta = 0;
    }


    // Read names of timers
    GetPrivateProfileString("NamesOfTimers", "Timer_1",   DEFAULT_ERROR_STR, g_atcProgramPathBuffer , MAX_PATH,  g_acIniPath);
    if(strcmp(DEFAULT_ERROR_STR, g_atcProgramPathBuffer) != 0)
    {
      strcpy_s(g_acTimerName_1, g_atcProgramPathBuffer);
    }

    GetPrivateProfileString("NamesOfTimers", "Timer_2",   DEFAULT_ERROR_STR, g_atcProgramPathBuffer , MAX_PATH,  g_acIniPath);
    if(strcmp(DEFAULT_ERROR_STR, g_atcProgramPathBuffer) != 0)
    {
      strcpy_s(g_acTimerName_2, g_atcProgramPathBuffer);
    }

    GetPrivateProfileString("NamesOfTimers", "Timer_3",   DEFAULT_ERROR_STR, g_atcProgramPathBuffer , MAX_PATH,  g_acIniPath);
    if(strcmp(DEFAULT_ERROR_STR, g_atcProgramPathBuffer) != 0)
    {
      strcpy_s(g_acTimerName_3, g_atcProgramPathBuffer);
    }

    GetPrivateProfileString("NamesOfTimers", "Timer_4",   DEFAULT_ERROR_STR, g_atcProgramPathBuffer , MAX_PATH,  g_acIniPath);
    if(strcmp(DEFAULT_ERROR_STR, g_atcProgramPathBuffer) != 0)
    {
      strcpy_s(g_acTimerName_4, g_atcProgramPathBuffer);
    }
    
    GetPrivateProfileString("NamesOfTimers", "Timer_5",   DEFAULT_ERROR_STR, g_atcProgramPathBuffer , MAX_PATH,  g_acIniPath);
    if(strcmp(DEFAULT_ERROR_STR, g_atcProgramPathBuffer) != 0)
    {
      strcpy_s(g_acTimerName_5, g_atcProgramPathBuffer);
    }

    // Get selected timer
    g_uiCounterTmp = GetPrivateProfileInt("Settings", "SelectedTimer", DEFAULT_ERROR, g_acIniPath);
    if(DEFAULT_ERROR != g_uiCounterTmp)
    {
      g_ucSelectedTimer = g_uiCounterTmp;
    }


  }

  
  if(!bFileExist(STYLE_XSL_FILE_NAME))
  {
    hStyleXsl = FindResource(hThisInstance, MAKEINTRESOURCE(IDB_STYLE_XSL), RT_RCDATA);
    
    if (hStyleXsl != NULL)
    {
      phStyleXslMem   = LoadResource (hThisInstance, hStyleXsl);
      dwResourceSize  = SizeofResource(hThisInstance, hStyleXsl);

      HANDLE hFile    = CreateFile (STYLE_XSL_FILE_NAME, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN , NULL);

      DWORD dwWrittenBytes;
      
      if (!WriteFile (hFile, phStyleXslMem, dwResourceSize, &dwWrittenBytes, NULL))
      {
        MessageBox (NULL, "Can't write resource file.", "Error!", MB_ICONEXCLAMATION); 
      }
      
      if (dwWrittenBytes != dwResourceSize)
      {
        MessageBox (NULL, "Resource file created with error.", "Error!", MB_ICONEXCLAMATION); 
      }

      CloseHandle (hFile);
    }
  }
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default mouse-pointer */
    wincl.hIcon = LoadIcon(hThisInstance, "Icon");
    wincl.hIconSm = LoadIcon(hThisInstance, "Icon");
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    //wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    //wincl.hbrBackground = (HBRUSH) NULL_BRUSH;
    wincl.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;
        
    // Calculate desired dimmension
    RECT WindowRealDimension = {0, 0, WINDOW_WIDTH, WINDOW_HEIGTH};
    AdjustWindowRect(&WindowRealDimension, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, false);

    /* The class is registered, let's create the program*/
    g_hWnd = CreateWindowEx (
                              0,                   /* Extended possibilites for variation */
                              szClassName,         /* Classname */
                              szMainWindowName,       /* Title Text (Window name) */
                              WS_OVERLAPPEDWINDOW & (~(WS_SIZEBOX | WS_MAXIMIZEBOX)),   //default window but without posibility to change its size
                              CW_USEDEFAULT,       /* Windows decides the position */
                              CW_USEDEFAULT,       /* where the window ends up on the screen */
                              WindowRealDimension.right - WindowRealDimension.left,                 /* The programs width */
                              WindowRealDimension.bottom - WindowRealDimension.top,                 /* and height in pixels */
                              HWND_DESKTOP,        /* The window is a child-window to desktop */
                              NULL,                /* No menu */
                              hThisInstance,       /* Program Instance handler */
                              NULL                 /* No Window Creation data */
                              );

    /* Make the window visible on the screen */
    ShowWindow (g_hWnd, nFunsterStil);
    
    
//**************************************************************************************************
// Borders

  g_hBorder_1 = CreateWindowEx (
                                0,
                                "BUTTON",
                                "Timer control",
                                WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                                5,
                                0,
                                230,
                                180,
                                g_hWnd,
                                NULL,
                                hThisInstance,
                                NULL);
// -------------------------------------------------------------------------------------------------
  g_hBorder_2 = CreateWindowEx (
                                0,
                                "BUTTON",
                                "Add Time",
                                WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                                235,
                                0,
                                76,
                                110,
                                g_hWnd,
                                NULL,
                                hThisInstance,
                                NULL);
//**************************************************************************************************
// Status bar
InitCommonControls();     // must be here but have to add,  Project > "project name" Properties > Configuration properties > Linker > Input > Additional Dependiencies:    Add: comctl32.lib
g_hStatusBar = CreateWindowEx(
                              0,
                              STATUSCLASSNAME,
                              NULL,
                              WS_CHILD | WS_VISIBLE,
                              0, 0,
                              0, 0,
                              g_hWnd,
                              (HMENU)200,
                              g_hInstance,
                              NULL);
                              
int statusBarWidths[] = {50, 195, -1};
SendMessage (g_hStatusBar, SB_SETPARTS, 3, (LPARAM)statusBarWidths);
//SendMessage (g_hStatusBar, SB_SETTEXT, 1, (LPARAM)"czas 2");
//SendMessage (g_hStatusBar, SB_SETTEXT, 0, (LPARAM)"czas 1");
// -------------------------------------------------------------------------------------------------

  // Initialize buttons
  g_hButton_radio_1 = CreateWindowEx (
                                      0,
                                      "BUTTON",
                                      "radio1",
                                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                      GRID_LEFT,
                                      GRID_TOP+BUTTON_HEIGTH+2*SEPARATE,
                                      RADIO_DIMMENSION,
                                      RADIO_DIMMENSION,
                                      g_hWnd,
                                      (HMENU)ID_RADIOBUTTON_1,
                                      hThisInstance,
                                      NULL);

  g_hButton_radio_2 = CreateWindowEx (
                                      0,
                                      "BUTTON",
                                      "radio2",
                                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                      GRID_LEFT,
                                      GRID_TOP+BUTTON_HEIGTH+3*SEPARATE+INPUT_HEIGTH,
                                      RADIO_DIMMENSION,
                                      RADIO_DIMMENSION,
                                      g_hWnd,
                                      (HMENU)ID_RADIOBUTTON_2,
                                      hThisInstance,
                                      NULL);

  g_hButton_radio_3 = CreateWindowEx (
                                      0,
                                      "BUTTON",
                                      "radio3",
                                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                      GRID_LEFT,
                                      GRID_TOP+BUTTON_HEIGTH+4*SEPARATE+2*INPUT_HEIGTH,
                                      RADIO_DIMMENSION,
                                      RADIO_DIMMENSION,
                                      g_hWnd,
                                      (HMENU)ID_RADIOBUTTON_3,
                                      hThisInstance,
                                      NULL);

  g_hButton_radio_4 = CreateWindowEx (
                                      0,
                                      "BUTTON",
                                      "radio4",
                                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                      GRID_LEFT,
                                      GRID_TOP+BUTTON_HEIGTH+5*SEPARATE+3*INPUT_HEIGTH,
                                      RADIO_DIMMENSION,
                                      RADIO_DIMMENSION,
                                      g_hWnd,
                                      (HMENU)ID_RADIOBUTTON_4,
                                      hThisInstance,
                                      NULL);
                                      
  g_hButton_radio_5 = CreateWindowEx (
                                      0,
                                      "BUTTON",
                                      "radio5",
                                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                      GRID_LEFT,
                                      GRID_TOP+BUTTON_HEIGTH+6*SEPARATE+4*INPUT_HEIGTH,
                                      RADIO_DIMMENSION,
                                      RADIO_DIMMENSION,
                                      g_hWnd,
                                      (HMENU)ID_RADIOBUTTON_5,
                                      hThisInstance,
                                      NULL);
                                      
  g_hButton_radio_6 = CreateWindowEx (
                                      0,
                                      "BUTTON",
                                      "Day",
                                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                      ADD_TIME_V_GRID,
                                      GRID_TOP+BUTTON_HEIGTH+3*SEPARATE+INPUT_HEIGTH,
                                      RADIO_WIDTH_EX,
                                      RADIO_HEIGTH,
                                      g_hWnd,
                                      (HMENU)ID_RADIOBUTTON_6,
                                      hThisInstance,
                                      NULL);
                                      
  g_hButton_radio_7 = CreateWindowEx (
                                      0,
                                      "BUTTON",
                                      "Timer",
                                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                      ADD_TIME_V_GRID,
                                      //GRID_TOP+BUTTON_HEIGTH+3*SEPARATE+INPUT_HEIGTH
                                      GRID_TOP+BUTTON_HEIGTH+4*SEPARATE+INPUT_HEIGTH+RADIO_DIMMENSION,
                                      RADIO_WIDTH_EX,
                                      RADIO_HEIGTH,
                                      g_hWnd,
                                      (HMENU)ID_RADIOBUTTON_7,
                                      hThisInstance,
                                      NULL);
  
  // Initialize editbox
  g_hEditBox_1 = CreateWindowEx (
                                  0,
                                  "EDIT",
                                  NULL,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                  GRID_LEFT+INPUT_GRID,
                                  GRID_TOP+BUTTON_HEIGTH+SEPARATE,
                                  INPUT_WIDTH,
                                  INPUT_HEIGTH,
                                  g_hWnd,
                                  NULL,
                                  hThisInstance,
                                  NULL);

  g_hEditBox_2 = CreateWindowEx (
                                  0,
                                  "EDIT",
                                  NULL,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                  GRID_LEFT+INPUT_GRID,
                                  GRID_TOP+BUTTON_HEIGTH+2*SEPARATE+INPUT_HEIGTH,
                                  INPUT_WIDTH,
                                  INPUT_HEIGTH,
                                  g_hWnd,
                                  NULL,
                                  hThisInstance,
                                  NULL);

  g_hEditBox_3 = CreateWindowEx (
                                  0,
                                  "EDIT",
                                  NULL,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                  GRID_LEFT+INPUT_GRID,
                                  GRID_TOP+BUTTON_HEIGTH+3*SEPARATE+2*INPUT_HEIGTH,
                                  INPUT_WIDTH,
                                  INPUT_HEIGTH,
                                  g_hWnd,
                                  NULL,
                                  hThisInstance,
                                  NULL);

  g_hEditBox_4 = CreateWindowEx (
                                  0,
                                  "EDIT",
                                  NULL,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                  GRID_LEFT+INPUT_GRID,
                                  GRID_TOP+BUTTON_HEIGTH+4*SEPARATE+3*INPUT_HEIGTH,
                                  INPUT_WIDTH,
                                  INPUT_HEIGTH,
                                  g_hWnd,
                                  NULL,
                                  hThisInstance,
                                  NULL);
                                  
  g_hEditBox_5 = CreateWindowEx (
                                  0,
                                  "EDIT",
                                  NULL,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                  GRID_LEFT+INPUT_GRID,
                                  GRID_TOP+BUTTON_HEIGTH+5*SEPARATE+4*INPUT_HEIGTH,
                                  INPUT_WIDTH,
                                  INPUT_HEIGTH,
                                  g_hWnd,
                                  NULL,
                                  hThisInstance,
                                  NULL);
                                  
  g_hEditBox_6 = CreateWindowEx (
                                  0,
                                  "EDIT",
                                  NULL,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                  ADD_TIME_V_GRID,
                                  GRID_TOP+BUTTON_HEIGTH+SEPARATE,
                                  INPUT_ADD_WIDTH,
                                  INPUT_HEIGTH,
                                  g_hWnd,
                                  (HMENU)ID_EDITBOX_ADD,
                                  hThisInstance,
                                  NULL);
                                  
  wndProc = (WNDPROC) SetWindowLong (g_hEditBox_6, GWL_WNDPROC, (LPARAM) EditProc);                 // replace default procedure for g_hEditBox_6
                                                                                                    // wndProc will contain old (default Windows) procedure for g_hEditBox_6

  // Load values from INI file
  SetWindowText(g_hEditBox_1, g_acTimerName_1);
  SetWindowText(g_hEditBox_2, g_acTimerName_2);
  SetWindowText(g_hEditBox_3, g_acTimerName_3);
  SetWindowText(g_hEditBox_4, g_acTimerName_4);
  SetWindowText(g_hEditBox_5, g_acTimerName_5);
  
  // Check radio button by default
  CheckRadioButton (g_hWnd, ID_RADIOBUTTON_6, ID_RADIOBUTTON_7, ID_RADIOBUTTON_7);                    // Add time option set to "D" - "Day"
  
  switch(g_ucSelectedTimer)                                                               // Have to be placed after EDIT field creation
  {
  case 1:
    CheckRadioButton (g_hWnd, ID_RADIOBUTTON_1, ID_RADIOBUTTON_5, ID_RADIOBUTTON_1);
    g_hActiveEditBox = g_hEditBox_1;
    break;
  case 2:
    CheckRadioButton (g_hWnd, ID_RADIOBUTTON_1, ID_RADIOBUTTON_5, ID_RADIOBUTTON_2);
    g_hActiveEditBox = g_hEditBox_2;
    break;
  case 3:
    CheckRadioButton (g_hWnd, ID_RADIOBUTTON_1, ID_RADIOBUTTON_5, ID_RADIOBUTTON_3);
    g_hActiveEditBox = g_hEditBox_3;
    break;
  case 4:
    CheckRadioButton (g_hWnd, ID_RADIOBUTTON_1, ID_RADIOBUTTON_5, ID_RADIOBUTTON_4);
    g_hActiveEditBox = g_hEditBox_4;
    break;
  case 5:
    CheckRadioButton (g_hWnd, ID_RADIOBUTTON_1, ID_RADIOBUTTON_5, ID_RADIOBUTTON_5);
    g_hActiveEditBox = g_hEditBox_5;
    break;
  default:
    CheckRadioButton (g_hWnd, ID_RADIOBUTTON_1, ID_RADIOBUTTON_5, ID_RADIOBUTTON_1);
    g_hActiveEditBox = g_hEditBox_1;
    break;
  }

  // Initialize buttons
    g_hButton_start = CreateWindowEx (
                                   0,
                                   "BUTTON",
                                   "Start/Pause",
                                   WS_CHILD | WS_VISIBLE,
                                   GRID_LEFT,
                                   GRID_TOP,
                                   BUTTON_WIDTH+30,
                                   BUTTON_HEIGTH,
                                   g_hWnd,
                                   (HMENU)ID_BUTTON_1,
                                   hThisInstance,
                                   NULL);
/*
    g_hButton_pause = CreateWindowEx (
                                   0,
                                   "BUTTON",
                                   "Old Pause",
                                   WS_CHILD | WS_VISIBLE,
                                   310,
                                   30,
                                   60,
                                   20,
                                   g_hWnd,
                                   (HMENU)ID_BUTTON_2,
                                   hThisInstance,
                                   NULL);
*/
    g_hButton_stop = CreateWindowEx (
                                   0,
                                   "BUTTON",
                                   "End day",
                                   WS_CHILD | WS_VISIBLE,
                                   GRID_LEFT+BUTTON_WIDTH+SEPARATE+30,
                                   GRID_TOP,
                                   BUTTON_WIDTH,
                                   BUTTON_HEIGTH,
                                   g_hWnd,
                                   (HMENU)ID_BUTTON_3,
                                   hThisInstance,
                                   NULL);
                                   
    g_hButton_reset = CreateWindowEx (
                                   0,
                                   "BUTTON",
                                   "Reset",
                                   WS_CHILD | WS_VISIBLE,
                                   GRID_LEFT+2*(BUTTON_WIDTH+SEPARATE)+30,
                                   GRID_TOP,
                                   BUTTON_WIDTH,
                                   BUTTON_HEIGTH,
                                   g_hWnd,
                                   (HMENU)ID_BUTTON_5,
                                   hThisInstance,
                                   NULL);
                                   
    g_hButton_reset = CreateWindowEx (
                                   0,
                                   "BUTTON",
                                   "Add",
                                   WS_CHILD | WS_VISIBLE,
                                   GRID_LEFT+3*(BUTTON_WIDTH+SEPARATE)+30+12,
                                   GRID_TOP,
                                   BUTTON_WIDTH,
                                   BUTTON_HEIGTH,
                                   g_hWnd,
                                   (HMENU)ID_BUTTON_4,
                                   hThisInstance,
                                   NULL);
//**********************************************************************************
hAccelTable = LoadAccelerators(hThisInstance, MAKEINTRESOURCE(IDC_KEYBOARD_SHORTCUTS));             //load keyboard shortcut resource
//**********************************************************************************

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
		  if (!TranslateAccelerator(g_hWnd, hAccelTable, &messages))
		  {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
      }
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
  INT iLeftTime = 0;

  // Painting rectangle
  RECT rect = {5, 50, 300, 65};

  CHAR acBuffer[STRING_LENGTH] = {0};
  CHAR acBufferTmp[STRING_LENGTH] = {0};

	switch (message)
	{
  case WM_CREATE:
      InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
      break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
    case ID_RADIOBUTTON_1:
      //MessageBox (NULL, "1", "Informacja", MB_ICONINFORMATION | MB_OK);
      CheckRadioButton (hWnd, ID_RADIOBUTTON_1, ID_RADIOBUTTON_5, ID_RADIOBUTTON_1);
      vSwitchActive(hWnd, ID_TIMER_1);
      g_hActiveEditBox = g_hEditBox_1;
      InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
      break;

    case ID_RADIOBUTTON_2:
      CheckRadioButton (hWnd, ID_RADIOBUTTON_1, ID_RADIOBUTTON_5, ID_RADIOBUTTON_2);
      vSwitchActive(hWnd, ID_TIMER_2);
      g_hActiveEditBox = g_hEditBox_2;
      InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
      break;

    case ID_RADIOBUTTON_3:
      CheckRadioButton (hWnd, ID_RADIOBUTTON_1, ID_RADIOBUTTON_5, ID_RADIOBUTTON_3);
      vSwitchActive(hWnd, ID_TIMER_3);
      g_hActiveEditBox = g_hEditBox_3;
      InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
      break;

    case ID_RADIOBUTTON_4:
      CheckRadioButton (hWnd, ID_RADIOBUTTON_1, ID_RADIOBUTTON_5, ID_RADIOBUTTON_4);
      vSwitchActive(hWnd, ID_TIMER_4);
      g_hActiveEditBox = g_hEditBox_4;
      InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
      break;
      
    case ID_RADIOBUTTON_5:
      CheckRadioButton (hWnd, ID_RADIOBUTTON_1, ID_RADIOBUTTON_5, ID_RADIOBUTTON_5);
      vSwitchActive(hWnd, ID_TIMER_5);
      g_hActiveEditBox = g_hEditBox_5;
      InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
      break;
      
    case ID_RADIOBUTTON_6:
      CheckRadioButton (hWnd, ID_RADIOBUTTON_6, ID_RADIOBUTTON_7, ID_RADIOBUTTON_6);
      InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
      break;
      
    case ID_RADIOBUTTON_7:
      CheckRadioButton (hWnd, ID_RADIOBUTTON_6, ID_RADIOBUTTON_7, ID_RADIOBUTTON_7);
      InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
      break;

    case ID_BUTTON_1:                                     // button Start
      if(!g_bTimersStart)                                 // if timer paused perform START
      {
        g_bTimersStart = TRUE;
        // Start selected timer
        if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_1) == BST_CHECKED)             // Check if radio button is checked
        {
          vSwitchActive(hWnd, ID_TIMER_1);
        }
        else if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_2) == BST_CHECKED)        // Check if radio button is checked
        {
          vSwitchActive(hWnd, ID_TIMER_2);
        }
        else if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_3) == BST_CHECKED)        // Check if radio button is checked
        {
          vSwitchActive(hWnd, ID_TIMER_3);
        }
        else if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_4) == BST_CHECKED)        // Check if radio button is checked
        {
          vSwitchActive(hWnd, ID_TIMER_4);
        }
        else if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_5) == BST_CHECKED)        // Check if radio button is checked
        {
          vSwitchActive(hWnd, ID_TIMER_5);
        }
      }
      else                                              // if timer started perform PAUSE
      {
        g_bTimersStart = FALSE;
        vPauseAllTimers(hWnd);
      }
      break;
/*
    case ID_BUTTON_2:                                     // button Pause
      g_bTimersStart = FALSE;
      vPauseAllTimers(hWnd);
      break;
*/
    case ID_BUTTON_3:                                     // button "End Day"
      {
        LONG lTimeToRelease;
        CHAR cStringFormat[] = "%s?\n+%d:%02d:%02d";
        
        lTimeToRelease = (g_uiLicznik1 + g_uiLicznik2 + g_uiLicznik3 +g_uiLicznik4 + g_uiLicznik5) - (_8_HOURS + g_iAdditionalDayTime);
        
        if(lTimeToRelease<0)
        {
          lTimeToRelease *= (-1);
          strcpy_s(cStringFormat, "%s?\n-%d:%02d:%02d");
        }

        va_list args;
        va_start(args, cStringFormat);
        
        // Create string to dialog window
        //sprintf(acBuffer, cStringFormat, "Do you really want release hours", lTimeToRelease/3600, (lTimeToRelease/60)%60, lTimeToRelease%60); // unsafe
        uiSafeSprintf(acBuffer, sizeof(acBuffer), cStringFormat, "Do you really want release hours", lTimeToRelease/3600, (lTimeToRelease/60)%60, lTimeToRelease%60);
        
        DialogBox (GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_RELEASE), hWnd, DlgReleaseHoursProc);
/*
        if(IDYES == MessageBox(hWnd, acBuffer, "Release hours", MB_YESNO | MB_ICONQUESTION))
        {
          g_bTimersStart = FALSE;
          vPauseAllTimers(hWnd);
          vUpdateGlobalHours((_8_HOURS + g_iAdditionalDayTime) - (g_uiLicznik1 + g_uiLicznik2 + g_uiLicznik3 +g_uiLicznik4 + g_uiLicznik5));
          vSaveState(hWnd, TRUE, FALSE, FALSE);
          g_uiLicznik1 = 0;
          g_uiLicznik2 = 0;
          g_uiLicznik3 = 0;
          g_uiLicznik4 = 0;
          g_uiLicznik5 = 0;
          g_iAdditionalDayTime = 0;
          InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
        }
*/
        break;
      }
      
    case ID_BUTTON_4:                                                                           // Add time button
      vAddGlobalTime(hWnd);
/*
      GetWindowText(g_hEditBox_6,   g_atcProgramPathBuffer, MAX_PATH);
      g_iBuffer = iParseAddedTime(g_atcProgramPathBuffer);
      
      // Old way to add time -----------------------------------------------------------------------
      if( ('m' == g_atcProgramPathBuffer[strlen(g_atcProgramPathBuffer) -1])                        
          ||
          ('M' == g_atcProgramPathBuffer[strlen(g_atcProgramPathBuffer) -1]) )
      {
        g_iBuffer = atoi(g_atcProgramPathBuffer);
        g_iBuffer *= _1_MINUTE;                                                                       // multiple by minute
      }
      else
      if( ('h' == g_atcProgramPathBuffer[strlen(g_atcProgramPathBuffer) -1])
          ||
          ('H' == g_atcProgramPathBuffer[strlen(g_atcProgramPathBuffer) -1]) )
      {
        g_iBuffer = atoi(g_atcProgramPathBuffer);
        g_iBuffer *= _1_HOUR;                                                                         // multiple by hour
      }
      else
      if( ('s' == g_atcProgramPathBuffer[strlen(g_atcProgramPathBuffer) -1])
          ||
          ('S' == g_atcProgramPathBuffer[strlen(g_atcProgramPathBuffer) -1]) )
      {
        g_iBuffer = atoi(g_atcProgramPathBuffer);                                                     // seconds
      }

      if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_7) == BST_CHECKED)                                // Check if radio buttonfor "Timer" is checked
      {
        switch(ucReturnSelectedChb(hWnd))
        {
          case 1:
            g_iBuffer += (INT)g_uiLicznik1;
            g_uiLicznik1 = (g_iBuffer > 0) ?  g_iBuffer : 0;
            break;
          case 2:
            g_iBuffer += (INT)g_uiLicznik2;
            g_uiLicznik2 = (g_iBuffer > 0) ?  g_iBuffer : 0;
            break;
          case 3:
            g_iBuffer += (INT)g_uiLicznik3;
            g_uiLicznik3 = (g_iBuffer > 0) ?  g_iBuffer : 0;
            break;
          case 4:
            g_iBuffer += (INT)g_uiLicznik4;
            g_uiLicznik4 = (g_iBuffer > 0) ?  g_iBuffer : 0;
            break;
          case 5:
            g_iBuffer += (INT)g_uiLicznik5;
            g_uiLicznik5 = (g_iBuffer > 0) ?  g_iBuffer : 0;
            break;
          default:
            break;
        }
      }
      else if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_6) == BST_CHECKED)
      {
        g_iAdditionalDayTime -= g_iBuffer;
      }
      
      SetWindowText(g_hEditBox_6, "");
      InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
*/
      break;

    case ID_BUTTON_5:                                       // button Reset
      g_bTimersStart = FALSE;

      g_uiLicznik1 = 0;
      g_uiLicznik2 = 0;
      g_uiLicznik3 = 0;
      g_uiLicznik4 = 0;
      g_uiLicznik5 = 0;
      g_iAdditionalDayTime = 0;

      vPauseAllTimers(hWnd);
      InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
      break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);

    return 0;
		}
		break;

  case WM_TIMER:
    switch((TIMERS)wParam)
    {
      case ID_TIMER_1:
        g_uiLicznik1++;
        break;

      case ID_TIMER_2:
        g_uiLicznik2++;
        break;

      case ID_TIMER_3:
        g_uiLicznik3++;
        break;

      case ID_TIMER_4:
        g_uiLicznik4++;
        break;
        
      case ID_TIMER_5:
        g_uiLicznik5++;
        break;

      default:
        break;
    }
    //InvalidateRect(hWnd, 0, TRUE);                        // Invalidate area of window
    SetRect(&rect, 135, 30, 336, 253);
    InvalidateRect(hWnd, &rect, TRUE);                      // Invalidate rect with timers
    SetRect(&rect, GRID_LEFT, STATUSBAR_H_GRID, GRID_LEFT+60, STATUSBAR_H_GRID+16);
    InvalidateRect(hWnd, &rect, TRUE);                      // Invalidate rect with left time

    return 0;
    break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
    // *******************************************************************************************************
    uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", g_uiLicznik1/3600, (g_uiLicznik1/60)%60, g_uiLicznik1%60);
    TextOut(hdc,
            3*SEPARATE+RADIO_DIMMENSION+INPUT_WIDTH,
            GRID_TOP+BUTTON_HEIGTH+SEPARATE+TIMERS_SEPAR,
            acBuffer,
            (int)strlen(acBuffer));  // print time for given timer

    uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", g_uiLicznik2/3600, (g_uiLicznik2/60)%60, g_uiLicznik2%60);
    TextOut(hdc,
            3*SEPARATE+RADIO_DIMMENSION+INPUT_WIDTH,
            GRID_TOP+BUTTON_HEIGTH+2*SEPARATE+INPUT_HEIGTH+TIMERS_SEPAR,
            acBuffer,
            (int)strlen(acBuffer));  // print time for given timer

    uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", g_uiLicznik3/3600, (g_uiLicznik3/60)%60, g_uiLicznik3%60);
    TextOut(hdc,
            3*SEPARATE+RADIO_DIMMENSION+INPUT_WIDTH,
            GRID_TOP+BUTTON_HEIGTH+3*SEPARATE+2*INPUT_HEIGTH+TIMERS_SEPAR,
            acBuffer,
            (int)strlen(acBuffer));  // print time for given timer

    uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", g_uiLicznik4/3600, (g_uiLicznik4/60)%60, g_uiLicznik4%60);
    TextOut(hdc,
            3*SEPARATE+RADIO_DIMMENSION+INPUT_WIDTH,
            GRID_TOP+BUTTON_HEIGTH+4*SEPARATE+3*INPUT_HEIGTH+TIMERS_SEPAR,
            acBuffer,
            (int)strlen(acBuffer));  // print time for given timer
            
    uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", g_uiLicznik5/3600, (g_uiLicznik5/60)%60, g_uiLicznik5%60);
    TextOut(hdc,
            3*SEPARATE+RADIO_DIMMENSION+INPUT_WIDTH,
            GRID_TOP+BUTTON_HEIGTH+5*SEPARATE+4*INPUT_HEIGTH+TIMERS_SEPAR,
            acBuffer,
            (int)strlen(acBuffer));  // print time for given timer

    // *******************************************************************************************************
    // Print left time
    iLeftTime = _8_HOURS + g_iAdditionalDayTime - (g_uiLicznik1 + g_uiLicznik2 + g_uiLicznik3 + g_uiLicznik4 + g_uiLicznik5);
    if(iLeftTime >= 0)        //
    {
      uiSafeSprintf(acBufferTmp, sizeof(acBufferTmp), "%d:%02d:%02d", iLeftTime/3600, (iLeftTime/60)%60, iLeftTime%60);
    }
    if(iLeftTime > 0)
    {
      uiSafeSprintf(acBuffer, sizeof(acBuffer), "-%s", acBufferTmp);
    }
    else if(iLeftTime < 0)
    {
      iLeftTime *= -1;
      uiSafeSprintf(acBufferTmp, sizeof(acBufferTmp), "%d:%02d:%02d", iLeftTime/3600, (iLeftTime/60)%60, iLeftTime%60);
      uiSafeSprintf(acBuffer, sizeof(acBuffer), "+%s", acBufferTmp);
    }
    else  // in case of "0:00:00"
    {
      strcpy_s(acBuffer, acBufferTmp);
    }
/*
    TextOut(hdc,
            GRID_LEFT,
            STATUSBAR_H_GRID,
            acBuffer,
            (int)strlen(acBuffer));  // print left time
*/
SendMessage (g_hStatusBar, SB_SETTEXT, 0, (LPARAM)acBuffer);       // print left time in status bar
    // *******************************************************************************************************
    // Print Global  Delta


    if(g_iGlobalHoursDelta > 0)        //
    {
      uiSafeSprintf(acBufferTmp, sizeof(acBufferTmp), "%d:%02d:%02d", g_iGlobalHoursDelta/3600, (g_iGlobalHoursDelta/60)%60, g_iGlobalHoursDelta%60);
      uiSafeSprintf(acBuffer, sizeof(acBuffer), "Global delta:  +%s", acBufferTmp);
    }
    else if(g_iGlobalHoursDelta < 0)
    {
      uiSafeSprintf(acBufferTmp, sizeof(acBufferTmp), "%d:%02d:%02d", (g_iGlobalHoursDelta * -1)/3600, ((g_iGlobalHoursDelta * -1)/60)%60, (g_iGlobalHoursDelta * -1)%60);
      uiSafeSprintf(acBuffer, sizeof(acBuffer), "Global delta:  -%s", acBufferTmp);
    }
    else  // in case of "0:00:00"
    {
      uiSafeSprintf(acBufferTmp, sizeof(acBufferTmp), "%d:%02d:%02d", g_iGlobalHoursDelta/3600, (g_iGlobalHoursDelta/60)%60, g_iGlobalHoursDelta%60);
      uiSafeSprintf(acBuffer, sizeof(acBuffer), "Global delta:  %s", acBufferTmp);
    }
/*
    TextOut(hdc,
            DELTA_TIME_V_GRID,
            STATUSBAR_H_GRID,
            acBuffer,
            (int)strlen(acBuffer));  // print left time
*/
SendMessage (g_hStatusBar, SB_SETTEXT, 2, (LPARAM)acBuffer);  // print left time in status bar
    // *******************************************************************************************************
    // Print else texts
/*
    TextOut(hdc,
            ADD_TIME_V_GRID+RADIO_DIMMENSION+SEPARATE,
            GRID_TOP+BUTTON_HEIGTH+2*SEPARATE+INPUT_HEIGTH+TIMERS_SEPAR,
            "Day",
            (int)strlen("Day"));  // Print "Add time" option label for "D" ("Day")
    TextOut(hdc,
            ADD_TIME_V_GRID+RADIO_DIMMENSION+SEPARATE,
            GRID_TOP+BUTTON_HEIGTH+2*SEPARATE+INPUT_HEIGTH+RADIO_DIMMENSION+TIMMERS_HEIGTH+1*TIMERS_SEPAR,
            "Timer",
            (int)strlen("Timer"));  // Print "Add time" option label for "T" ("Timer")
*/
// *************************************************************************************************
    TextOut(hdc, 310, 180, "?", (int)strlen("?"));                                                  // Print Question mark
// *************************************************************************************************
		EndPaint(hWnd, &ps);
		break;
		
    case WM_LBUTTONDOWN:                                                                            // Print Help
    {
      if(
        (LOWORD (lParam) <= 321) && (LOWORD (lParam) >= 301)
         &&
        (HIWORD (lParam) <= 195) && (HIWORD (lParam) >= 175)
        )
       {
          MessageBox (NULL, "Work Timer v"
                            APPVERSION
                            "\n\nWork time counter.\n"
                            "Created by Tomasz Jokiel\n"
                            "tomasz.jokiel@gmail.com",
                            "About", MB_ICONINFORMATION | MB_OK);
       }
    }
    break;
		
    case WM_CTLCOLOREDIT:
        if(
            (g_hActiveEditBox == (HWND)lParam)
          )
        {
          hdc = (HDC) wParam;
          SetBkColor(hdc, ACTIVE_EDIT_BK_COLOR);
          return (LRESULT) g_hBrush1;
        }
        else if(g_hEditBox_6 == (HWND)lParam)
        {
          hdc = (HDC) wParam;
          SetBkColor(hdc, ADD_FIELD_BK_COLOR);
          return (LRESULT) g_hBrush2;
        }
        else
        {
          return DefWindowProc(hWnd, message, wParam, lParam);
        }
    break;

  case WM_CLOSE:
    vPauseAllTimers(hWnd);
    vSaveState(hWnd, FALSE, FALSE, FALSE);

    DestroyWindow (g_hButton_radio_1);
    DestroyWindow (g_hButton_radio_2);
    DestroyWindow (g_hButton_radio_3);
    DestroyWindow (g_hButton_radio_4);
    DestroyWindow (g_hButton_radio_5);
    DestroyWindow (g_hButton_radio_6);
    DestroyWindow (g_hButton_radio_7);
    DestroyWindow (g_hButton_start);
    DestroyWindow (g_hButton_pause);
    DestroyWindow (g_hButton_reset);
    DestroyWindow (g_hButton_stop);
    DestroyWindow (g_hButton_AddTime);
    DestroyWindow (g_hEditBox_1);
    DestroyWindow (g_hEditBox_2);
    DestroyWindow (g_hEditBox_3);
    DestroyWindow (g_hEditBox_4);
    DestroyWindow (g_hEditBox_5);
    DestroyWindow (g_hEditBox_6);

    return DefWindowProc(hWnd, message, wParam, lParam);
    break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


LRESULT CALLBACK EditProc (HWND hWndEdit, UINT msg,
                           WPARAM wParam, LPARAM lParam)
{
	static TCHAR szBuffer[100];
	switch (msg)
	{
    case WM_KEYDOWN:
      if(VK_RETURN == wParam)
      {
        //MessageBox (NULL, "OK", "Uchwyt", MB_ICONINFORMATION | MB_OK);
        vAddGlobalTime(g_hWnd);
      }
      break;
      
    default:
      return CallWindowProc (wndProc, hWndEdit, msg, wParam, lParam) ;
  }
  
  return CallWindowProc (wndProc, hWndEdit, msg, wParam, lParam) ;
}



/***************************************************************************************************
 * \brief Procedure for dialog window
 **************************************************************************************************/
BOOL CALLBACK DlgReleaseHoursProc (HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  HWND  hMainWindowHandle       = FindWindow(szClassName, szMainWindowName);
  HWND  hWndReleaseText         = GetDlgItem(hWndDlg, IDC_DIALOG_RELEASE_TIME_TEXT);              // Get dialog item - text window
  CHAR  acBuffer[STRING_LENGTH] = {0};

 switch (Msg)
 {
  // -------------------------------*
  case WM_INITDIALOG:
  // -------------------------------*
    {
      LONG lTimeToRelease;
      CHAR cStringFormat[] = "+%d:%02d:%02d";
      
      lTimeToRelease = (g_uiLicznik1 + g_uiLicznik2 + g_uiLicznik3 +g_uiLicznik4 + g_uiLicznik5) - (_8_HOURS + g_iAdditionalDayTime);
      
      if(lTimeToRelease<0)
      {
        lTimeToRelease *= (-1);
        strcpy_s(cStringFormat, "-%d:%02d:%02d");
      }
      
      // Create string to dialog window
      uiSafeSprintf(acBuffer, sizeof(acBuffer), cStringFormat, lTimeToRelease/3600, (lTimeToRelease/60)%60, lTimeToRelease%60);
      
      // Display prepared text with time to release
      SetWindowText( hWndReleaseText, acBuffer);
    }    
    break;
    
  // -------------------------------*
  case WM_COMMAND:
  // -------------------------------*
  {
   // reakcja na przyciski
   switch (LOWORD(wParam))
   {
    // -------------------------------
    case IDC_DIALOG_RELEASE_OK:
    // -------------------------------                                                              // Button OK was pressed
      {
        BOOL  bIsEndOfWeek;
        BOOL  bIsEndOfMonth;
        //******************************************************* CHeckcheckboxes
        // End of week?
        bIsEndOfWeek = (IsDlgButtonChecked (hWndDlg, IDC_DIALOG_RELEASE_CHBOX_1_END_WEEK) == BST_CHECKED);
        /*
        if(bIsEndOfWeek)
        {
          MessageBox (NULL, "End Week", "Info", MB_ICONEXCLAMATION);
        }
        else
        {
          MessageBox (NULL, "End Week NOT", "Info", MB_ICONEXCLAMATION);
        }
        */
        // End of month?
        bIsEndOfMonth = (IsDlgButtonChecked (hWndDlg, IDC_DIALOG_RELEASE_CHBOX_2_END_MONTH) == BST_CHECKED);
        /*
        if(bIsEndOfMonth)
        {
          MessageBox (NULL, "End Month", "Info", MB_ICONEXCLAMATION);
        }
        else
        {
          MessageBox (NULL, "End Month NOT", "Info", MB_ICONEXCLAMATION);
        }
        */
        //****************************************************************


        g_bTimersStart = FALSE;
        vPauseAllTimers(hMainWindowHandle);
        
        // ToDo following 3 functions should be enclose in one function, calling these three
        vUpdateGlobalHours((_8_HOURS + g_iAdditionalDayTime) - (g_uiLicznik1 + g_uiLicznik2 + g_uiLicznik3 +g_uiLicznik4 + g_uiLicznik5));
        vUpdateWeeklyHours((_8_HOURS + g_iAdditionalDayTime) - (g_uiLicznik1 + g_uiLicznik2 + g_uiLicznik3 +g_uiLicznik4 + g_uiLicznik5));
        vUpdatemonthHours ((_8_HOURS + g_iAdditionalDayTime) - (g_uiLicznik1 + g_uiLicznik2 + g_uiLicznik3 +g_uiLicznik4 + g_uiLicznik5));

        vSaveState(hMainWindowHandle, TRUE, bIsEndOfWeek, bIsEndOfMonth);
        g_uiLicznik1 = 0;
        g_uiLicznik2 = 0;
        g_uiLicznik3 = 0;
        g_uiLicznik4 = 0;
        g_uiLicznik5 = 0;
        g_iAdditionalDayTime = 0;
        InvalidateRect(hMainWindowHandle, 0, TRUE);                        // Invalidate area of window

        // Close release time dialog
        EndDialog (hWndDlg, IDC_DIALOG_RELEASE_OK);
      }
      break;

    // -------------------------------
    case IDC_DIALOG_RELEASE_CANCEL:
    // -------------------------------
      // Button Canacel was pressed
      EndDialog (hWndDlg, IDC_DIALOG_RELEASE_CANCEL);
      break;
   }
  }
  break;

  // -------------------------------*
  case WM_CLOSE:
  // -------------------------------*
    // Button Canacel was pressed
    EndDialog (hWndDlg, IDC_DIALOG_RELEASE_CANCEL);
    break;
    
  // -------------------------------*
  default:
  // -------------------------------*
    return FALSE;
 }

 return TRUE;
}

