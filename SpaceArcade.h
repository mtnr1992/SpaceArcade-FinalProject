/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  ID_PANEL                         1       /* callback function: ExitId */
#define  ID_PANEL_ID                      2       /* control type: picture, callback function: (none) */

#define  MainPanel                        2       /* callback function: ExitFunc2 */
#define  MainPanel_ENTER_NAME             2       /* control type: string, callback function: (none) */
#define  MainPanel_TOPCHARTSBUTTON        3       /* control type: pictButton, callback function: TOP_CHARTS_BUTTON */
#define  MainPanel_COMMANDBUTTON          4       /* control type: pictButton, callback function: START */
#define  MainPanel_PICTURE_3              5       /* control type: picture, callback function: (none) */
#define  MainPanel_PICTURE_2              6       /* control type: picture, callback function: (none) */
#define  MainPanel_PICTURE                7       /* control type: picture, callback function: (none) */

#define  PANEL                            3       /* callback function: ExitFunc */
#define  PANEL_CANVAS                     2       /* control type: canvas, callback function: (none) */
#define  PANEL_AMMO                       3       /* control type: scale, callback function: (none) */
#define  PANEL_LEVEL                      4       /* control type: numeric, callback function: (none) */
#define  PANEL_POINTS_BAR                 5       /* control type: numeric, callback function: (none) */
#define  PANEL_LIFE_BAR                   6       /* control type: numeric, callback function: (none) */
#define  PANEL_DECORATION                 7       /* control type: deco, callback function: (none) */
#define  PANEL_PAUSE_CONTINUE             8       /* control type: textButton, callback function: Pause_Continue */
#define  PANEL_TIMER                      9       /* control type: timer, callback function: Tick */
#define  PANEL_PICTURE                    10      /* control type: picture, callback function: (none) */
#define  PANEL_PICTURE_2                  11      /* control type: picture, callback function: (none) */
#define  PANEL_PICTURE_3                  12      /* control type: picture, callback function: (none) */
#define  PANEL_PICTURE_4                  13      /* control type: picture, callback function: (none) */

#define  TOP_CHART                        4       /* callback function: exitTOP_CHART */
#define  TOP_CHART_TABLE                  2       /* control type: table, callback function: (none) */
#define  TOP_CHART_PICTURE                3       /* control type: picture, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_About                    2       /* callback function: ABOUT */
#define  MENUBAR_Help                     3       /* callback function: HELP */
#define  MENUBAR_Demo                     4       /* callback function: DEMO */


     /* Callback Prototypes: */

void CVICALLBACK ABOUT(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK DEMO(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK ExitFunc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitFunc2(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitId(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitTOP_CHART(int panel, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK HELP(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK Pause_Continue(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK START(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Tick(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TOP_CHARTS_BUTTON(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
