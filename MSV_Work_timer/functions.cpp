#include <windows.h>
#include  <stdio.h>
#include  <stdarg.h>
#include "global.h"
#include "functions.h"
#include <map>
#include <vector>
#include <fstream>



using namespace std;

// ****************** Local definitions *******************************************

#define NO_STRING                         -1

// ****************** Declaration of external global variables ********************

// External global variables
extern BOOL     g_bTimersStart;            // state of timers
extern UINT     g_uiLicznik1;
extern UINT     g_uiLicznik2;
extern UINT     g_uiLicznik3;
extern UINT     g_uiLicznik4;
extern UINT     g_uiLicznik5;

// editboxes
extern HWND     g_hEditBox_1;
extern HWND     g_hEditBox_2;
extern HWND     g_hEditBox_3;
extern HWND     g_hEditBox_4;
extern HWND     g_hEditBox_5;
extern HWND     g_hEditBox_6;

// global strings
extern TCHAR    g_atcProgramPathBuffer[MAX_PATH];
extern CHAR     g_acIniPath[MAX_PATH];

// others 
extern INT      g_iGlobalHoursDelta;
extern INT      g_iWeekHoursDelta;
extern INT      g_iMonthHoursDelta;
extern INT      g_iAdditionalDayTime;
extern INT      g_iBuffer;





// ****************** Typedefs *********************************************************************



// ****************** Definition of function *******************************************************






/***************************************************************************************************
* brief      : Switch active Timer
* side effect: 
***************************************************************************************************/
VOID vSwitchActive(HWND hWnd, TIMERS ActiveTimer)
{
  if(!g_bTimersStart)     // If Timers were disabled exit function
  {
    return;
  }

  SetTimer(hWnd, ActiveTimer, 1000, 0);     // Activate timer

  for(UCHAR i = ID_TIMER_1; i < ID_TIMERS_COUNT; i++)
  {
    if((TIMERS)i == ActiveTimer)          // skip timer to activate
    {
      continue;
    }

    KillTimer(hWnd, i);                   // dezactivate other timers
  }
}





/***************************************************************************************************
* brief      : Pause all timers
* side effect: 
***************************************************************************************************/
VOID vPauseAllTimers(HWND hWnd)
{
  for(UCHAR i = ID_TIMER_1; i < ID_TIMERS_COUNT; i++)
  {
    KillTimer(hWnd, i);                   // dezactivate other timers
  }
}






/***************************************************************************************************
* brief      : Save state of controls
* side effect: 
***************************************************************************************************/
VOID vSaveState(HWND hWnd, BOOL bSaveToFile, BOOL bPrintWeekSummary, BOOL bPrintMonthSummary)
{
  CHAR acBuffer[STRING_LENGTH] = {0};
  ST_SAVE_DATA stSaveData;

  // Save state of timers
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d", g_uiLicznik1);
  stSaveData.uiTimer[0] = g_uiLicznik1;
  WritePrivateProfileString("StateOfTimers",   "Timer_1", acBuffer,  g_acIniPath);
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d", g_uiLicznik2);
  stSaveData.uiTimer[1] = g_uiLicznik2;
  WritePrivateProfileString("StateOfTimers",   "Timer_2", acBuffer,  g_acIniPath);
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d", g_uiLicznik3);
  stSaveData.uiTimer[2] = g_uiLicznik3;
  WritePrivateProfileString("StateOfTimers",   "Timer_3", acBuffer,  g_acIniPath);
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d", g_uiLicznik4);
  stSaveData.uiTimer[3] = g_uiLicznik4;
  WritePrivateProfileString("StateOfTimers",   "Timer_4", acBuffer,  g_acIniPath);
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d", g_uiLicznik5);
  stSaveData.uiTimer[4] = g_uiLicznik5;
  WritePrivateProfileString("StateOfTimers",   "Timer_5", acBuffer,  g_acIniPath);

  // Save selected Timer
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d", ucReturnSelectedChb(hWnd));
  WritePrivateProfileString("Settings",   "SelectedTimer", acBuffer,  g_acIniPath);

  // Save Names of Timers
  GetWindowText(g_hEditBox_1,   g_atcProgramPathBuffer, MAX_PATH);
  stSaveData.strActivities[0] = g_atcProgramPathBuffer;
  WritePrivateProfileString("NamesOfTimers",   "Timer_1", g_atcProgramPathBuffer,  g_acIniPath);
  GetWindowText(g_hEditBox_2,   g_atcProgramPathBuffer, MAX_PATH);
  stSaveData.strActivities[1] = g_atcProgramPathBuffer;
  WritePrivateProfileString("NamesOfTimers",   "Timer_2", g_atcProgramPathBuffer,  g_acIniPath);
  GetWindowText(g_hEditBox_3,   g_atcProgramPathBuffer, MAX_PATH);
  stSaveData.strActivities[2] = g_atcProgramPathBuffer;
  WritePrivateProfileString("NamesOfTimers",   "Timer_3", g_atcProgramPathBuffer,  g_acIniPath);
  GetWindowText(g_hEditBox_4,   g_atcProgramPathBuffer, MAX_PATH);
  stSaveData.strActivities[3] = g_atcProgramPathBuffer;
  WritePrivateProfileString("NamesOfTimers",   "Timer_4", g_atcProgramPathBuffer,  g_acIniPath);
  GetWindowText(g_hEditBox_5,   g_atcProgramPathBuffer, MAX_PATH);
  stSaveData.strActivities[4] = g_atcProgramPathBuffer;
  WritePrivateProfileString("NamesOfTimers",   "Timer_5", g_atcProgramPathBuffer,  g_acIniPath);

  // Save Global Hours Delta
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d", g_iGlobalHoursDelta);
  WritePrivateProfileString("Settings",   "GlobalHoursDelta", acBuffer,  g_acIniPath);

  // Save Weekly Hours Delta
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d", g_iWeekHoursDelta);
  WritePrivateProfileString("Settings",   "WeekHoursDelta", acBuffer,  g_acIniPath);

  // Save Month Hours Delta
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d", g_iMonthHoursDelta);
  WritePrivateProfileString("Settings",   "MonthHoursDelta", acBuffer,  g_acIniPath);

  
  //Save to file
  if(bSaveToFile)
  {
    stSaveData.iGlobalDelta       = g_iGlobalHoursDelta;      // Global Hours Delta
    stSaveData.iWeeklyDelta       = g_iWeekHoursDelta;        // Weekly Hours Delta
    stSaveData.iMonthDelta        = g_iMonthHoursDelta;       // Month  Hours Delta
    stSaveData.bPrintWeekSummary  = bPrintWeekSummary;        // Print Weekly Hours Delta in summary
    stSaveData.bPrintMonthSummary = bPrintMonthSummary;       // Print Month  Hours Delta in summary
    GetSystemTime (&stSaveData.stDateTime);                   // Todays date
    //bSaveDayToFile(stSaveData);
    bSaveDayToFileTop(stSaveData);

    if(TRUE == bPrintWeekSummary)
    { 
      // Clear weekly delta hours when are released to file
      g_iWeekHoursDelta = 0;
      WritePrivateProfileString("Settings",   "WeekHoursDelta", "0",  g_acIniPath);
    }

    if(TRUE == bPrintMonthSummary)
    { 
      // Clear weekly delta hours when are released to file
      g_iMonthHoursDelta = 0;
      WritePrivateProfileString("Settings",   "MonthHoursDelta", "0",  g_acIniPath);
    }
  }
}






/***************************************************************************************************
* brief      : Return selected radio button
* side effect: 
***************************************************************************************************/
UCHAR ucReturnSelectedChb(HWND hWnd)
{
  if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_1) == BST_CHECKED)             // Check if radio button is checked
  {
    return 1;
  }
  else if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_2) == BST_CHECKED)        // Check if radio button is checked
  {
    return 2;
  }
  else if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_3) == BST_CHECKED)        // Check if radio button is checked
  {
    return 3;
  }
  else if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_4) == BST_CHECKED)        // Check if radio button is checked
  {
    return 4;
  }
  else if(IsDlgButtonChecked (hWnd, ID_RADIOBUTTON_5) == BST_CHECKED)        // Check if radio button is checked
  {
    return 5;
  }

  return 0;   // If none of checkbox is selected
}




/***************************************************************************************************
* brief      : Update global hours
* side effect: 
***************************************************************************************************/
VOID vUpdateGlobalHours(UINT uiDailyDifferencial)
{
  CHAR acBuffer[STRING_LENGTH] = {0};
  CHAR acBufferTmp[STRING_LENGTH] = {0};

  g_iGlobalHoursDelta -= uiDailyDifferencial;

  if(g_iGlobalHoursDelta < 0)
  {

    uiSafeSprintf(acBufferTmp, sizeof(acBufferTmp), "%d:%02d:%02d", (g_iGlobalHoursDelta * -1)/3600, ((g_iGlobalHoursDelta * -1)/60)%60, (g_iGlobalHoursDelta * -1)%60);
    uiSafeSprintf(acBuffer, sizeof(acBuffer), "-%s", acBufferTmp);
  }
  else if(g_iGlobalHoursDelta > 0)
  {
    uiSafeSprintf(acBufferTmp, sizeof(acBufferTmp), "%d:%02d:%02d", g_iGlobalHoursDelta/3600, (g_iGlobalHoursDelta/60)%60, g_iGlobalHoursDelta%60);
    uiSafeSprintf(acBuffer, sizeof(acBuffer), "+%s", acBufferTmp);
  }
  else  // in case of "0:00:00"
  {
    strcpy_s(acBuffer, acBufferTmp);
  }

  //MessageBox (NULL, acBuffer, "Global delta", MB_ICONERROR | MB_OK);
}


/***************************************************************************************************
* brief      : Update week hours
* side effect: 
***************************************************************************************************/
VOID vUpdateWeeklyHours(UINT uiDailyDifferencial)
{
  g_iWeekHoursDelta -= uiDailyDifferencial;
}



/***************************************************************************************************
* brief      : Update month hours
* side effect: 
***************************************************************************************************/
VOID vUpdatemonthHours(UINT uiDailyDifferencial)
{
  g_iMonthHoursDelta -= uiDailyDifferencial;
}



/***************************************************************************************************
* brief      : Retrieve and parse string of time to add
* side effect: 
***************************************************************************************************/
INT iParseAddedTime(TCHAR *acTextBuffer)
{
  string  sInputString = acTextBuffer;
  string  sBuffer;
  CHAR    cPosOfFind = 0;
  INT     iReturnValue = 0;
  INT     iTempValue = 0;
  
  CHAR cBuffer[100] = {0};
  double n, w;

  enum SIGN{
            PLUS = 0,
            MINUS
           };
           
  SIGN  bSign = PLUS;
  
  if(NO_STRING != (sInputString.find('-')))
  {
    bSign = MINUS;
  }
  
  if(NO_STRING != (cPosOfFind = sInputString.find(':')))
  {
    
    sBuffer = sInputString.substr((INT)bSign, cPosOfFind - ((INT)bSign));
    iReturnValue = atoi(sBuffer.c_str()) * _1_HOUR;
    
    //MessageBox (NULL, sBuffer.c_str(), "?", MB_ICONINFORMATION | MB_OK);

    sBuffer = sInputString.substr(cPosOfFind+1, sInputString.length() - (cPosOfFind+1));
    iReturnValue += atoi(sBuffer.c_str()) * _1_MINUTE;
  }
  else if(
          (NO_STRING != (cPosOfFind = sInputString.find(',')))
          ||
          (NO_STRING != (cPosOfFind = sInputString.find('.')))
        )
  {
    acTextBuffer[cPosOfFind] = '.';
    iReturnValue = (INT)atof(acTextBuffer) * _1_HOUR;
    return iReturnValue;
  }
  else
  {
    iReturnValue = atoi(sInputString.c_str()) * _1_HOUR;
    return iReturnValue;
    //return ((bSign == MINUS) ? -iReturnValue : iReturnValue);
  }


//iReturnValue = atoi(sInputString.c_str());
iReturnValue = (bSign == MINUS) ? -iReturnValue : iReturnValue;
return iReturnValue;
//MessageBox (NULL, "1", sInputString.c_str(), MB_ICONINFORMATION | MB_OK);
}


/***************************************************************************************************
* brief      : Add time from editob "Add Time" to global delta hours counter
* side effect: use global varialbes: g_hEditBox_6, g_atcProgramPathBuffer, g_iBuffer
*              g_uiLicznik1, g_uiLicznik2, g_uiLicznik3, g_uiLicznik4, g_uiLicznik5
***************************************************************************************************/
VOID vAddGlobalTime(HWND hWnd)
{

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
}


/***************************************************************************************************
* brief      : Save state of program at "End day"
* side effect: Use global variable "g_iAdditionalDayTime"
***************************************************************************************************/
BOOL bSaveDayToFile(ST_SAVE_DATA stSaveData)
{
  string   strBuffer;
  DWORD    dwWrittenBytes, dwFileSize;
  HANDLE   hFile;
  CHAR     acBuffer[BIG_STRING_LENGTH] = {0};
  CHAR     acTimeFormatString[] = "-%d:%02d:%02d";
  LONG     lPosition;
  BOOL     bAppendData = TRUE;                  // if TRUE data will be added at the end of file, structure is already created
  
  fstream fFileStr(DATA_BASE_FILE_NAME, ios::ate | ios::out | ios::in); //ate
  
  if(!fFileStr. good())
  {
    fFileStr.clear();
    fFileStr.open(DATA_BASE_FILE_NAME, ios::out);
    bAppendData = FALSE;                        // data won't be added, this is new, empty file (create all structure)
    
    if(!fFileStr. good())
    {
      MessageBox (NULL, "Can't open file.", "Error!", MB_ICONEXCLAMATION);
      return FALSE;
    }    
    
    fFileStr << "<?xml version=\"1.0\" ?>\n"; 
    fFileStr << "<?xml-stylesheet type=\"text/xsl\" href=\""<< STYLE_XSL_FILE_NAME <<"\" ?>\n\n";
    fFileStr << "<WT_SUMMARY>\n\n"; 
    fFileStr << "  <WT_LISTING></WT_LISTING>\n\n";                 // header structure ... ToDO
  }  
  
// --------------------------------------------------------------------------------------------------
  cXMLTag cTagWtDaySummary  ("WT_DAY_SUMMARY");
  cXMLTag cTagWtHeader      ("WT_HEADER");
  cXMLTag cTagWtRecord      ("WT_RECORD");
  cXMLTag cTagTimer_1       ("TIMER_1");
  cXMLTag cTagTimer_2       ("TIMER_2");
  cXMLTag cTagTimer_3       ("TIMER_3");
  cXMLTag cTagTimer_4       ("TIMER_4");
  cXMLTag cTagTimer_5       ("TIMER_5");
  cXMLTag cTagTimerHeader_1 ("H_TIMER_1");
  cXMLTag cTagTimerHeader_2 ("H_TIMER_2");
  cXMLTag cTagTimerHeader_3 ("H_TIMER_3");
  cXMLTag cTagTimerHeader_4 ("H_TIMER_4");
  cXMLTag cTagTimerHeader_5 ("H_TIMER_5");  
  cXMLTag cTagDate          ("DATE");
  cXMLTag cTagDailyDelta    ("DAILY_DELTA");
  cXMLTag cTagGlobalDelta   ("GLOBAL_DELTA");
// --------------------------------------------------------------------------------------------------
  // Create string for <DATE>
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%i-%i-%i", stSaveData.stDateTime.wDay, stSaveData.stDateTime.wMonth, stSaveData.stDateTime.wYear);
  strBuffer = acBuffer;
  cTagDate.vAddTagValue(strBuffer);
  
// --------------------------------------------------------------------------------------------------
  // Create string for <DAILY_DELTA>
  INT iThisDayDelta = ((stSaveData.uiTimer[0] + stSaveData.uiTimer[1] + stSaveData.uiTimer[2] + stSaveData.uiTimer[3] + stSaveData.uiTimer[4]) - (_8_HOURS + g_iAdditionalDayTime));
   // Create string for global delta hours
  if(iThisDayDelta < 0)
  {
    iThisDayDelta *= (-1);
    strcpy_s(acTimeFormatString, "-%d:%02d:%02d");
  }
  else
  {
    strcpy_s(acTimeFormatString, "+%d:%02d:%02d");
  }
  
  uiSafeSprintf(acBuffer, sizeof(acBuffer), acTimeFormatString, (iThisDayDelta)/3600, ((iThisDayDelta)/60)%60, (iThisDayDelta)%60);
  strBuffer = acBuffer;
  cTagDailyDelta.vAddTagValue(strBuffer);

  // append XML tag with global delta hours
  //cTagDay.vAppendChildTag(cTagThisDayDelta);
  
// --------------------------------------------------------------------------------------------------
  // Create string for <GLOBAL_DELTA>
  if(stSaveData.iGlobalDelta < 0)
  {
    stSaveData.iGlobalDelta *= (-1);
    strcpy_s(acTimeFormatString, "-%d:%02d:%02d");
  }
  else
  {
    strcpy_s(acTimeFormatString, "+%d:%02d:%02d");
  }
  
  uiSafeSprintf(acBuffer, sizeof(acBuffer), acTimeFormatString, (stSaveData.iGlobalDelta)/3600, ((stSaveData.iGlobalDelta)/60)%60, (stSaveData.iGlobalDelta)%60);
  strBuffer = acBuffer;
  cTagGlobalDelta.vAddTagValue(strBuffer);
  
// --------------------------------------------------------------------------------------------------
  // Create strings for timers values <TIMER_1>...<TIMER_5>
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", (stSaveData.uiTimer[0])/3600, ((stSaveData.uiTimer[0])/60)%60, (stSaveData.uiTimer[0])%60);
  strBuffer = acBuffer;
  cTagTimer_1.vAddTagValue(strBuffer);
  
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", (stSaveData.uiTimer[1])/3600, ((stSaveData.uiTimer[1])/60)%60, (stSaveData.uiTimer[1])%60);
  strBuffer = acBuffer;
  cTagTimer_2.vAddTagValue(strBuffer);

  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", (stSaveData.uiTimer[2])/3600, ((stSaveData.uiTimer[2])/60)%60, (stSaveData.uiTimer[2])%60);
  strBuffer = acBuffer;
  cTagTimer_3.vAddTagValue(strBuffer);

  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", (stSaveData.uiTimer[3])/3600, ((stSaveData.uiTimer[3])/60)%60, (stSaveData.uiTimer[3])%60);
  strBuffer = acBuffer;
  cTagTimer_4.vAddTagValue(strBuffer);

  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", (stSaveData.uiTimer[4])/3600, ((stSaveData.uiTimer[4])/60)%60, (stSaveData.uiTimer[4])%60);
  strBuffer = acBuffer;
  cTagTimer_5.vAddTagValue(strBuffer); 

// --------------------------------------------------------------------------------------------------
  // Create strings for timers values <H_TIMER_1>...<H_TIMER_5>
  cTagTimerHeader_1.vAddTagValue(stSaveData.strActivities[0]);
  cTagTimerHeader_2.vAddTagValue(stSaveData.strActivities[1]);
  cTagTimerHeader_3.vAddTagValue(stSaveData.strActivities[2]);
  cTagTimerHeader_4.vAddTagValue(stSaveData.strActivities[3]);
  cTagTimerHeader_5.vAddTagValue(stSaveData.strActivities[4]);

// --------------------------------------------------------------------------------------------------
  // Create structure
  cTagWtHeader.vAppendChildTag(cTagTimerHeader_1);
  cTagWtHeader.vAppendChildTag(cTagTimerHeader_2);
  cTagWtHeader.vAppendChildTag(cTagTimerHeader_3);
  cTagWtHeader.vAppendChildTag(cTagTimerHeader_4);
  cTagWtHeader.vAppendChildTag(cTagTimerHeader_5);
  
  cTagWtRecord.vAppendChildTag(cTagDate);
  cTagWtRecord.vAppendChildTag(cTagDailyDelta);
  cTagWtRecord.vAppendChildTag(cTagGlobalDelta);
  cTagWtRecord.vAppendChildTag(cTagTimer_1);
  cTagWtRecord.vAppendChildTag(cTagTimer_2);
  cTagWtRecord.vAppendChildTag(cTagTimer_3);
  cTagWtRecord.vAppendChildTag(cTagTimer_4);
  cTagWtRecord.vAppendChildTag(cTagTimer_5);
  
  cTagWtDaySummary.vAppendChildTag(cTagWtHeader);
  cTagWtDaySummary.vAppendChildTag(cTagWtRecord);
  
// --------------------------------------------------------------------------------------------------
  strBuffer = cTagWtDaySummary.strPrintTagContent();
  strBuffer += "\n\n";

// ********************************************************************************************* END Create string structure
  char *pcBuffer = new char[strBuffer.length() + 10];
  strcpy(pcBuffer, strBuffer.c_str());

  if(bAppendData)
  {
    // find position of <WT_FOOTER> tag
    /*
    fstream fFileStrTmp(DATA_BASE_FILE_NAME, ios::in);
    char    *pcGetBuffer = new char[BIG_STRING_LENGTH];
    string  strTemp_str;
    LONG    lTagPos;
    
    fFileStrTmp.seekg (-BIG_STRING_LENGTH, ios::end);
    fFileStrTmp.read ( pcGetBuffer, BIG_STRING_LENGTH );
    
    MessageBox (NULL, pcGetBuffer, "Error!", MB_ICONEXCLAMATION);
    
    strTemp_str = pcGetBuffer;
    lTagPos     = strTemp_str.find("<WT_FOOTER>");
    
    fFileStrTmp.close();
    delete pcGetBuffer;
    */
    fFileStr.seekp (-97, ios::end);            // set pointer before <WT_FOOTER> tag
  }
  

  fFileStr.write (pcBuffer, strlen(pcBuffer));
  
  fFileStr << "\n\n<WT_FOOTER> File Generated By Work Timer  v" APPVERSION " - "<<stSaveData.stDateTime.wHour<<":"<<stSaveData.stDateTime.wMinute<<" "<<stSaveData.stDateTime.wDay<<"."<<stSaveData.stDateTime.wMonth<<"."<<stSaveData.stDateTime.wYear<<"</WT_FOOTER>\n";  // write end structure
  fFileStr << "</WT_SUMMARY>\n";
    
  fFileStr.close(); 
  delete pcBuffer  ;
  
  return TRUE;
}



/***************************************************************************************************
* brief      : Save state of program (newest entry at the Top) at "End day"
* side effect: Use global variable "g_iAdditionalDayTime"
***************************************************************************************************/
BOOL bSaveDayToFileTop(ST_SAVE_DATA stSaveData)
{
  char     *pcBuffer;
  string   strBufferOld;
  string   strBuffer;
  DWORD    dwFileSize;
  CHAR     acBuffer[BIG_STRING_LENGTH] = {0};
  CHAR     acTimeFormatString[] = "-%d:%02d:%02d";
  LONG     lPosition;
  LONG     lPositionTmp;
  BOOL     bAppendData = TRUE;                  // if TRUE data will be added at the end of file, structure is already created
  
  //fstream fFileStr(DATA_BASE_FILE_NAME, ios::in | ios::binary);
  fstream fFileStr(DATA_BASE_FILE_NAME, ios::in | ios::binary);
  
  
// ********************************************************************************************  
// ********************************************************************************************
// ********************************************************************************************
// ********************************************************************************************
// ********************************************************************************************
  if(fFileStr.good())
  {//MessageBox (NULL, "GOOD", "Error!", MB_ICONEXCLAMATION);
    // get length of file:
    fFileStr.seekg (0, ios::end);
    dwFileSize = fFileStr.tellg();
    fFileStr.seekg (0, ios::beg);  
    
    // allocate memory:
    pcBuffer = new char [dwFileSize]; 
    
    // read data as a block:
    fFileStr.read (pcBuffer, dwFileSize);
    
    fFileStr.close();
    
    //strBufferOld = pcBuffer;
    strBufferOld.clear();
    strBufferOld.assign(pcBuffer);
    //MessageBox (NULL, pcBuffer, "Error!", MB_ICONEXCLAMATION);
    
    //delete[] pcBuffer;
    //return FALSE;

    // Find position of beginning of "Day" data
    lPosition = strBufferOld.find("<WT_DAY_SUMMARY>");

    // Find position of beginning of "Week" data
    lPositionTmp = strBufferOld.find("<WT_WEEK_SUMMARY>");

    // elimine hiher position
    if((lPositionTmp < lPosition) && ((-1) != lPositionTmp ))
    {
      lPosition = lPositionTmp;
    }

    // Find position of beginning of "Month" data
    lPositionTmp = strBufferOld.find("<WT_MONTH_SUMMARY>");

     // elimine hiher position
    if((lPositionTmp < lPosition) && ((-1) != lPositionTmp ))
    {
      lPosition = lPositionTmp;
    }
    
    delete[] pcBuffer;
  }
  else
  {//MessageBox (NULL, "FUCK", "Error!", MB_ICONEXCLAMATION);
    fFileStr.clear();
    fFileStr.close();
    //return FALSE;
    //fFileStr.open(DATA_BASE_FILE_NAME, ios::in);
  
    bAppendData = FALSE;                        // data won't be added, this is new, empty file (create all structure)
  /*  
    if(!fFileStr. good())
    {
      MessageBox (NULL, "Can't open file.", "Error!", MB_ICONEXCLAMATION);
      return FALSE;
    }    
    
    fFileStr << "<?xml version=\"1.0\" ?>\n"; 
    fFileStr << "<?xml-stylesheet type=\"text/xsl\" href=\""<< STYLE_XSL_FILE_NAME <<"\" ?>\n\n";
    fFileStr << "<WT_SUMMARY>\n\n"; 
    fFileStr << "  <WT_LISTING></WT_LISTING>\n\n";                 // header structure ... ToDO
  */
  }
/*  
  strBufferOld  = "<?xml version=\"1.0\" ?>\n"; 
  strBufferOld += "<?xml-stylesheet type=\"text/xsl\" href=\""<< STYLE_XSL_FILE_NAME <<"\" ?>\n\n";
  strBufferOld += "<WT_SUMMARY>\n\n"; 
  strBufferOld += "  <WT_LISTING></WT_LISTING>\n\n";
*/  
// --------------------------------------------------------------------------------------------------
  cXMLTag cTagWtDaySummary  ("WT_DAY_SUMMARY");
  cXMLTag cTagWtHeader      ("WT_HEADER");
  cXMLTag cTagWtRecord      ("WT_RECORD");
  cXMLTag cTagTimer_1       ("TIMER_1");
  cXMLTag cTagTimer_2       ("TIMER_2");
  cXMLTag cTagTimer_3       ("TIMER_3");
  cXMLTag cTagTimer_4       ("TIMER_4");
  cXMLTag cTagTimer_5       ("TIMER_5");
  cXMLTag cTagTimerHeader_1 ("H_TIMER_1");
  cXMLTag cTagTimerHeader_2 ("H_TIMER_2");
  cXMLTag cTagTimerHeader_3 ("H_TIMER_3");
  cXMLTag cTagTimerHeader_4 ("H_TIMER_4");
  cXMLTag cTagTimerHeader_5 ("H_TIMER_5");  
  cXMLTag cTagDate          ("DATE");
  cXMLTag cTagDailyDelta    ("DAILY_DELTA");
  cXMLTag cTagGlobalDelta   ("GLOBAL_DELTA");
  // Week summary
  cXMLTag cTagWtWeekSummary             ("WT_WEEK_SUMMARY");
  cXMLTag cTagWtWeekSummaryRecord       ("WT_WEEK_SUMMARY_RECORD");
  cXMLTag cTagWtWeekSummaryGlobalDelta  ("WT_WEEK_SUMMARY_GLOBAL_DELTA");
  cXMLTag cTagWtWeekSummaryWeeklyDelta  ("WT_WEEK_SUMMARY_WEEKLY_DELTA");

  // Month summary
  cXMLTag cTagWtMonthSummary             ("WT_MONTH_SUMMARY");
  cXMLTag cTagWtMonthSummaryRecord       ("WT_MONTH_SUMMARY_RECORD");
  cXMLTag cTagWtMonthSummaryGlobalDelta  ("WT_MONTH_SUMMARY_DELTA_HOURS");
// --------------------------------------------------------------------------------------------------
  // Create string for <DATE>
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%02d-%02d-%i", stSaveData.stDateTime.wDay, stSaveData.stDateTime.wMonth, stSaveData.stDateTime.wYear);
  strBuffer = acBuffer;
  cTagDate.vAddTagValue(strBuffer);
  
// --------------------------------------------------------------------------------------------------
  // Create string for <DAILY_DELTA>
  INT iThisDayDelta = ((stSaveData.uiTimer[0] + stSaveData.uiTimer[1] + stSaveData.uiTimer[2] + stSaveData.uiTimer[3] + stSaveData.uiTimer[4]) - (_8_HOURS + g_iAdditionalDayTime));
   // Create string for global delta hours
  if(iThisDayDelta < 0)
  {
    iThisDayDelta *= (-1);
    strcpy_s(acTimeFormatString, "-%d:%02d:%02d");
  }
  else
  {
    strcpy_s(acTimeFormatString, "+%d:%02d:%02d");
  }
  
  uiSafeSprintf(acBuffer, sizeof(acBuffer), acTimeFormatString, (iThisDayDelta)/3600, ((iThisDayDelta)/60)%60, (iThisDayDelta)%60);
  strBuffer = acBuffer;
  cTagDailyDelta.vAddTagValue(strBuffer);

  // append XML tag with global delta hours
  //cTagDay.vAppendChildTag(cTagThisDayDelta);
  
// --------------------------------------------------------------------------------------------------
  // Create string for <GLOBAL_DELTA>
  if(stSaveData.iGlobalDelta < 0)
  {
    stSaveData.iGlobalDelta *= (-1);
    strcpy_s(acTimeFormatString, "-%d:%02d:%02d");
  }
  else
  {
    strcpy_s(acTimeFormatString, "+%d:%02d:%02d");
  }
  
  uiSafeSprintf(acBuffer, sizeof(acBuffer), acTimeFormatString, (stSaveData.iGlobalDelta)/3600, ((stSaveData.iGlobalDelta)/60)%60, (stSaveData.iGlobalDelta)%60);
  strBuffer = acBuffer;
  cTagGlobalDelta.vAddTagValue(strBuffer);

// --------------------------------------------------------------------------------------------------
  // STore global delta to weekly summary <WT_WEEK_SUMMARY_GLOBAL_DELTA>
  if(stSaveData.bPrintWeekSummary)
  {
    cTagWtWeekSummaryGlobalDelta.vAddTagValue(strBuffer);
  }
// --------------------------------------------------------------------------------------------------
  // Create string for <WT_WEEK_SUMMARY_WEEKLY_DELTA>
  if(stSaveData.bPrintWeekSummary)
  {
    if(stSaveData.iWeeklyDelta < 0)
    {
      stSaveData.iWeeklyDelta *= (-1);
      strcpy_s(acTimeFormatString, "-%d:%02d:%02d");
    }
    else
    {
      strcpy_s(acTimeFormatString, "+%d:%02d:%02d");
    }
  
    uiSafeSprintf(acBuffer, sizeof(acBuffer), acTimeFormatString, (stSaveData.iWeeklyDelta)/3600, ((stSaveData.iWeeklyDelta)/60)%60, (stSaveData.iWeeklyDelta)%60);
    strBuffer = acBuffer;
    cTagWtWeekSummaryWeeklyDelta.vAddTagValue(strBuffer);
  }

// --------------------------------------------------------------------------------------------------
  // Create string for <WT_MONTH_SUMMARY_DELTA_HOURS>
  if(stSaveData.bPrintMonthSummary)
  {
    if(stSaveData.iMonthDelta < 0)
    {
      stSaveData.iMonthDelta *= (-1);
      strcpy_s(acTimeFormatString, "-%d:%02d:%02d");
    }
    else
    {
      strcpy_s(acTimeFormatString, "+%d:%02d:%02d");
    }
  
    uiSafeSprintf(acBuffer, sizeof(acBuffer), acTimeFormatString, (stSaveData.iMonthDelta)/3600, ((stSaveData.iMonthDelta)/60)%60, (stSaveData.iMonthDelta)%60);
    strBuffer = acBuffer;
    cTagWtMonthSummaryGlobalDelta.vAddTagValue(strBuffer);
  }
// --------------------------------------------------------------------------------------------------
  // Create strings for timers values <TIMER_1>...<TIMER_5>
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", (stSaveData.uiTimer[0])/3600, ((stSaveData.uiTimer[0])/60)%60, (stSaveData.uiTimer[0])%60);
  strBuffer = (stSaveData.uiTimer[0] > 0) ? acBuffer : "";
  cTagTimer_1.vAddTagValue(strBuffer);
  
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", (stSaveData.uiTimer[1])/3600, ((stSaveData.uiTimer[1])/60)%60, (stSaveData.uiTimer[1])%60);
  strBuffer = (stSaveData.uiTimer[1] > 0) ? acBuffer : "";
  cTagTimer_2.vAddTagValue(strBuffer);

  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", (stSaveData.uiTimer[2])/3600, ((stSaveData.uiTimer[2])/60)%60, (stSaveData.uiTimer[2])%60);
  strBuffer = (stSaveData.uiTimer[2] > 0) ? acBuffer : "";
  cTagTimer_3.vAddTagValue(strBuffer);

  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", (stSaveData.uiTimer[3])/3600, ((stSaveData.uiTimer[3])/60)%60, (stSaveData.uiTimer[3])%60);
  strBuffer = (stSaveData.uiTimer[3] > 0) ? acBuffer : "";
  cTagTimer_4.vAddTagValue(strBuffer);

  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d:%02d", (stSaveData.uiTimer[4])/3600, ((stSaveData.uiTimer[4])/60)%60, (stSaveData.uiTimer[4])%60);
  strBuffer = (stSaveData.uiTimer[4] > 0) ? acBuffer : "";
  cTagTimer_5.vAddTagValue(strBuffer); 

// --------------------------------------------------------------------------------------------------
  // Create strings for timers values <H_TIMER_1>...<H_TIMER_5>
  cTagTimerHeader_1.vAddTagValue((stSaveData.uiTimer[0] > 0)? stSaveData.strActivities[0] : "");
  cTagTimerHeader_2.vAddTagValue((stSaveData.uiTimer[1] > 0)? stSaveData.strActivities[1] : "");
  cTagTimerHeader_3.vAddTagValue((stSaveData.uiTimer[2] > 0)? stSaveData.strActivities[2] : "");
  cTagTimerHeader_4.vAddTagValue((stSaveData.uiTimer[3] > 0)? stSaveData.strActivities[3] : "");
  cTagTimerHeader_5.vAddTagValue((stSaveData.uiTimer[4] > 0)? stSaveData.strActivities[4] : "");

// --------------------------------------------------------------------------------------------------
  // Create structure

  // Day summary
  cTagWtHeader.vAppendChildTag(cTagTimerHeader_1);
  cTagWtHeader.vAppendChildTag(cTagTimerHeader_2);
  cTagWtHeader.vAppendChildTag(cTagTimerHeader_3);
  cTagWtHeader.vAppendChildTag(cTagTimerHeader_4);
  cTagWtHeader.vAppendChildTag(cTagTimerHeader_5);
  
  cTagWtRecord.vAppendChildTag(cTagDate);
  cTagWtRecord.vAppendChildTag(cTagDailyDelta);
  cTagWtRecord.vAppendChildTag(cTagGlobalDelta);
  cTagWtRecord.vAppendChildTag(cTagTimer_1);
  cTagWtRecord.vAppendChildTag(cTagTimer_2);
  cTagWtRecord.vAppendChildTag(cTagTimer_3);
  cTagWtRecord.vAppendChildTag(cTagTimer_4);
  cTagWtRecord.vAppendChildTag(cTagTimer_5);
  
  cTagWtDaySummary.vAppendChildTag(cTagWtHeader);
  cTagWtDaySummary.vAppendChildTag(cTagWtRecord);

  // Weekly summary
  if(stSaveData.bPrintWeekSummary)
  {
    cTagWtWeekSummaryRecord.vAppendChildTag(cTagWtWeekSummaryGlobalDelta);
    cTagWtWeekSummaryRecord.vAppendChildTag(cTagWtWeekSummaryWeeklyDelta);

    cTagWtWeekSummary.vAppendChildTag(cTagWtWeekSummaryRecord);
  }

  // Month summary
  if(stSaveData.bPrintMonthSummary)
  {
    cTagWtMonthSummaryRecord.vAppendChildTag(cTagWtMonthSummaryGlobalDelta);

    cTagWtMonthSummary.vAppendChildTag(cTagWtMonthSummaryRecord);
  }
  
// --------------------------------------------------------------------------------------------------

  strBuffer  = "<?xml version=\"1.0\" ?>\n"; 
  strBuffer += "<?xml-stylesheet type=\"text/xsl\" href=\"";
  strBuffer += STYLE_XSL_FILE_NAME;
  strBuffer += "\" ?>\n\n";
  strBuffer += "<WT_SUMMARY>\n\n"; 
  strBuffer += "  <WT_LISTING></WT_LISTING>\n\n";

  if(stSaveData.bPrintMonthSummary)                                                                                  // Add month summary if choosen
  {
    strBuffer += cTagWtMonthSummary.strPrintTagContent();
    strBuffer += "\n\n";
  }

  if(stSaveData.bPrintWeekSummary)                                                                                    // Add week summary if choosen
  {
    strBuffer += cTagWtWeekSummary.strPrintTagContent();
    strBuffer += "\n\n";
  }
  
  strBuffer += cTagWtDaySummary.strPrintTagContent();
  strBuffer += "\n\n";
  
  //MessageBox (NULL, strBuffer.c_str(), "Kupa", MB_ICONEXCLAMATION);
  
  if(bAppendData)
  {
    strBuffer += strBufferOld.substr(lPosition);
    lPosition  = strBuffer.find("<WT_FOOTER>");
    
    strBuffer  = strBuffer.substr(0, lPosition);
  }
  //else
  //{
  strBuffer += "<WT_FOOTER> File Generated By Work Timer  v";
  strBuffer += APPVERSION;
  strBuffer += " - ";
 
  uiSafeSprintf(acBuffer, sizeof(acBuffer), "%d:%02d  %d.%02d.%d", (stSaveData.stDateTime.wHour+1), stSaveData.stDateTime.wMinute, stSaveData.stDateTime.wDay, stSaveData.stDateTime.wMonth, stSaveData.stDateTime.wYear);
  strBuffer += acBuffer;  
  
  strBuffer += "</WT_FOOTER>\n";  // write end structure
  strBuffer += "</WT_SUMMARY>\n";
  //}
// ********************************************************************************************* END Create string structure
  pcBuffer = new char[strBuffer.length() + 10];
  strcpy(pcBuffer, strBuffer.c_str());
  
  
  // Work on Existing file, try to overwrite it
  //fFileStr.open(DATA_BASE_FILE_NAME, ios::out | ios::binary);
  fFileStr.clear();
  fFileStr.open(DATA_BASE_FILE_NAME, ios::out | ios::binary);
  
  if(!fFileStr.good())
  {
    MessageBox (NULL, "Can't open file.", "Error!", MB_ICONEXCLAMATION);
    fFileStr.close(); 
    delete pcBuffer  ;    
    return FALSE;  
  }

  fFileStr.write (pcBuffer, strlen(pcBuffer));
    
  fFileStr.close(); 
 
  delete[] pcBuffer;
  
  return TRUE;
}




/***************************************************************************************************
* name       : Class cXMLTag
* brief      : Create XML object struct tag
***************************************************************************************************/
// -------------------------------------------------------------------
cXMLTag::cXMLTag(string strName)
{
  strTagName = strName;
}

// -------------------------------------------------------------------
void cXMLTag::vAddTagAttrybut(string strAttrybutName, string strAttrybutValue)
{
  mapAttributes.insert(make_pair(strAttrybutName, strAttrybutValue));
}

// -------------------------------------------------------------------
void cXMLTag::vAppendChildTag(cXMLTag cTag)
{
  vectChildsTags.push_back(cTag);
}

// -------------------------------------------------------------------
string cXMLTag::strPrintTagContent()
{
  string strTagContent;
  
  // Tag name
  strTagContent = "<" + strTagName;
  
  // Tag attributes
  for( map<string, string>::iterator ii = mapAttributes.begin(); ii != mapAttributes.end(); ++ii)
  {
    strTagContent += " ";
    strTagContent += (*ii).first;
    strTagContent += "=\"";
    strTagContent += (*ii).second;
    strTagContent += "\"";
  }  
  // end tag name
  strTagContent += ">";
  
  if(strTagValue.length())                          // if value exist, add value
  {
    strTagContent += strTagValue;
  }
  else                                              // else add child tags
  {
    for(int ii=0; ii < vectChildsTags.size(); ii++)
    {
      strTagContent += "\n\t";
      strTagContent += vectChildsTags[ii].strPrintTagContent();
    }
    
    if(vectChildsTags.size())
    {
      strTagContent += "\n";
    }
  }
//  MessageBox (NULL, strTagContent.c_str(), "Informacja", MB_ICONINFORMATION | MB_OK);
  //end tag
  strTagContent += "</" + strTagName;
  strTagContent += ">";
  
  return strTagContent;
}

// -------------------------------------------------------------------
void cXMLTag::vAddTagValue(string strTagVal)
{
  strTagValue = strTagVal;
}


// -------------------------------------------------------------------
BOOL cXMLTag::bIsTagEmpty()
{
  if((0 == strTagValue.length()) && (0 == vectChildsTags.size()))
  {
    return TRUE;
  }
  
  return FALSE;
}


/***************************************************************************************************
* name       : file_exists
* brief      : Check if given file (file path) exist
***************************************************************************************************/
BOOL bFileExist(const char * filename)
{
    if (FILE * file = fopen(filename, "r"))
    {
      fclose(file);
      return TRUE;
    }
    return FALSE;
}

/***************************************************************************************************
* name       : uiSafeSprintf
* brief      : Make safe sprintf operation
***************************************************************************************************/
UINT uiSafeSprintf(CHAR* pcBuffer, UINT uiMaxSize, CHAR* pcStringFormat, ...)
{
  va_list args;
  va_start(args, pcStringFormat);
  return vsnprintf(pcBuffer, uiMaxSize-1, pcStringFormat, args);    
}
