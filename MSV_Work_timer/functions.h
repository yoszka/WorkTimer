#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "global.h"

//definition of Timers

enum TIMERS{
            ID_TIMER_1 = 1,
            ID_TIMER_2,
            ID_TIMER_3,
            ID_TIMER_4,
            ID_TIMER_5,

            ID_TIMERS_COUNT
           };


  #define TIMERS_COUNT    (ID_TIMERS_COUNT -1)


#include "string"
#include <map>
#include <vector>
using namespace std;
// ****************** Typedefs *********************************************************************

typedef struct
{
  UINT        uiTimer[TIMERS_COUNT];
  string      strActivities[TIMERS_COUNT];
  INT         iGlobalDelta;
  INT         iWeeklyDelta;
  INT         iMonthDelta;
  SYSTEMTIME  stDateTime;
  BOOL        bPrintWeekSummary;
  BOOL        bPrintMonthSummary;

}ST_SAVE_DATA;

// *************************************************************************************************

VOID  vSwitchActive(HWND hWnd, TIMERS ActiveTimer);
VOID  vUpdateGlobalHours(UINT uiDailyDifferencial);
VOID  vUpdateWeeklyHours(UINT uiDailyDifferencial);
VOID  vUpdatemonthHours(UINT uiDailyDifferencial);
VOID  vPauseAllTimers(HWND hWnd);
VOID  vAddGlobalTime(HWND hWnd);
VOID  vSaveState(HWND hWnd, BOOL bSaveToFile, BOOL bPrintWeekSummary, BOOL bPrintMonthSummary);
VOID  vPrintVersion();
VOID  vPrintUsage();
UCHAR ucReturnSelectedChb(HWND hWnd);
INT   iParseAddedTime(TCHAR *acTextBuffer);
BOOL  bSaveDayToFile(ST_SAVE_DATA stSaveData);
BOOL  bSaveDayToFileTop(ST_SAVE_DATA stSaveData);
BOOL  bFileExist(const char * filename);
UINT  uiSafeSprintf(CHAR* buffer, UINT uiMaxSize, CHAR* pcStringFormat, ...);



/***************************************************************************************************
* name       : Class cXMLTag
* brief      : Create XML object struct tag
***************************************************************************************************/
class cXMLTag
{
  string              strTagName;
  string              strTagValue;
  map<string, string> mapAttributes;
  vector<cXMLTag>     vectChildsTags;

  public:
    cXMLTag(string strTagName);                                           // constructor
    // -------------------------------------------------------------------
    void   vAddTagAttrybut(string strAttrybutName, string strAttrybutValue);
    void   vAddTagValue(string strTagVal);
    void   vAppendChildTag(cXMLTag cTag);
    BOOL   bIsTagEmpty();
    string strPrintTagContent();
};
#endif
