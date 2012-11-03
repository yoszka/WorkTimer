/*
Global definition
*/

#ifndef _GLOBAL_H
  #define _GLOBAL_H
  
  #define APPVERSION                "2.4.5"         // Actual version of application
  #define STYLE_XSL_FILE_NAME       "WT_style.xsl"  // File name with style for output XML file
  #define DATA_BASE_FILE_NAME       "Data_base.xml" // Data base output XML file name

  #define MAX_LOADSTRING               100
  #define STRING_LENGTH                 50
  #define MID_STRING_LENGTH            170
  #define BIG_STRING_LENGTH            300
  #define DEFAULT_ERROR         0xFFFFFFFF
  #define DEFAULT_ERROR_STR             ""
  #define _8_HOURS               (60*60*8)
  #define _1_HOUR                  (60*60)
  #define _1_MINUTE                   (60)

  // definition for BUTTONS
  #define ID_RADIOBUTTON_1            501
  #define ID_RADIOBUTTON_2            502
  #define ID_RADIOBUTTON_3            503
  #define ID_RADIOBUTTON_4            504
  #define ID_RADIOBUTTON_5            505
  #define ID_RADIOBUTTON_6            506
  #define ID_RADIOBUTTON_7            507

  #define ID_BUTTON_1                 511
  #define ID_BUTTON_2                 512
  #define ID_BUTTON_3                 513
  #define ID_BUTTON_4                 514
  #define ID_BUTTON_5                 515

  #define ID_EDITBOX_ADD              520
  
  // Release time Dialog
  #define IDD_DIALOG_RELEASE                      1000
  #define IDC_DIALOG_RELEASE_OK                   1001
  #define IDC_DIALOG_RELEASE_CANCEL               1002
  #define IDC_DIALOG_RELEASE_CHBOX_1_END_WEEK     1003
  #define IDC_DIALOG_RELEASE_CHBOX_2_END_MONTH    1004
  #define IDC_DIALOG_RELEASE_TITLE                1005
  #define IDC_DIALOG_RELEASE_TIME_TEXT            1006

#endif // _GLOBAL_H
