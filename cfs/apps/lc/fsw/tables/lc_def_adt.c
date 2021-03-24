/*************************************************************************
** File:
**   $Id: lc_def_adt.c 1.2 2015/03/04 16:09:53EST sstrege Exp  $
**
**  Copyright � 2007-2014 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Goddard Space Flight Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
** Purpose: 
**   Limit Checker (LC) default actionpoint definition table (ADT)
**
** Notes:
**   This file provides a default ADT table that simply sets all
**   actionpoint entries to "not used". It has been formatted to make
**   it easy for mission developers to edit as needed (see the 
**   examples section below).
**
**   LC will append a trailer string to the end of the text 
**   specified in the "EventText" field with additional information.
**   See lc_action.h for the format.
**
**   Compiler Note
**   -------------
**   This file may generate the following warning when compiling 
**   with gcc if you are using the flags "-ansi -pedantic":
** 
**   "warning: ISO C90 forbids specifying subobject to initialize"
**
**   Removing "-pedantic" should eliminate the warning. An alternate
**   solution is to replace "-ansi" with "-std=c99" and leave the
**   "-pedantic" flag in place
** 
**   $Log: lc_def_adt.c  $
**   Revision 1.2 2015/03/04 16:09:53EST sstrege 
**   Added copyright information
**   Revision 1.1 2012/07/31 16:53:41EDT nschweis 
**   Initial revision
**   Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/lcx/fsw/tables/project.pj
**   Revision 1.7 2012/07/20 13:50:29EDT lwalling 
**   Remove compiler warning for unused data structure
**   Revision 1.6 2010/04/20 12:03:22PDT lwalling 
**   Fix ART vs ADT and WRT vs WDT typo in LC table files
**   Revision 1.5 2010/02/25 11:46:40EST lwalling 
**   Modified sample table entries to reference LC_BASE_AP_EID
**   Revision 1.4 2010/02/19 17:44:39EST lwalling 
**   Change state events limits to individual limits for Passive AP, FailToPass and PassToFail
**   Revision 1.3 2009/12/28 14:48:04EST lwalling 
**   Add initializer for event limiter value
**   Revision 1.2 2009/12/22 13:19:02EST lwalling 
**   Update list of included header files, modify contents of table file header structure
**   Revision 1.1 2009/12/18 14:10:49EST lwalling 
**   Initial revision
**   Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/lc/fsw/tables/project.pj
**   Revision 1.4 2009/01/15 15:36:19EST dahardis 
**   Unit test fixes
**   Revision 1.3 2008/12/10 16:00:26EST dahardis 
**   Changed the size of the template definition table from
**   256 to 176 entries
**   Revision 1.2 2008/12/03 13:59:45EST dahardis 
**   Corrections from peer code review
**   Revision 1.1 2008/10/29 14:19:16EDT dahardison 
**   Initial revision
**   Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/lc/fsw/src/project.pj
** 
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "lc_platform_cfg.h"
#include "lc_msgdefs.h"
#include "lc_tbl.h"
#include "lc_app.h"
#include "lc_events.h"

#include "../whe/fsw/src/whe_msg.h"
/*************************************************************************
** Examples
** (note that comment delimiters have been changed to '**')
**
** Actions that trigger off a single watchpoint:
** (see lc_def_wdt.c for companion watchpoint definitions)
**
**    ** #100 **  
**    {
**        .DefaultState        = LC_APSTATE_DISABLED,
**        .MaxPassiveEvents    = 2,
**        .MaxPassFailEvents   = 2,
**        .MaxFailPassEvents   = 2,
**        .RTSId               = RTS_ID_DIVINER_SAFE_MODE,
**        .MaxFailsBeforeRTS   = 60,
**        .EventType           = CFE_EVS_INFORMATION,
**        .EventID             = LC_BASE_AP_EID + 100,
**        .EventText           = { "Diviner: low input volt (1)" },
**        .RPNEquation         = { ** (WP_112) **
**                                 112,
**                                 LC_RPN_EQUAL
**                               }
**    },
** 
**    ** #101 **
**    {
**        .DefaultState        = LC_APSTATE_DISABLED,
**        .MaxPassiveEvents    = 2,
**        .MaxPassFailEvents   = 2,
**        .MaxFailPassEvents   = 2,
**        .RTSId               = RTS_ID_DIVINER_SAFE_MODE,
**        .MaxFailsBeforeRTS   = 3,
**        .EventType           = CFE_EVS_INFORMATION,
**        .EventID             = LC_BASE_AP_EID + 101,
**        .EventText           = { "Diviner: low input volt (2)" },
**        .RPNEquation         = { ** (WP_113) **
**                                 113,
**                                 LC_RPN_EQUAL
**                               }
**    },
** 
** Examples of more complex Reverse Polish Notation expressions:
**
**    ** #43 **
**    {
**        .DefaultState        = LC_APSTATE_ENABLED,
**        .MaxPassiveEvents    = 2,
**        .MaxPassFailEvents   = 2,
**        .MaxFailPassEvents   = 2,
**        .RTSId               = RTS_ID_ACS_EXIT_THRUSTER_MODE,
**        .MaxFailsBeforeRTS   = 10,
**        .EventType           = CFE_EVS_INFORMATION,
**        .EventID             = LC_BASE_AP_EID + 43,
**        .EventText           = { "GNC: delta-V sys attitude" },
**        .RPNEquation         = { ** (WP_26 && !WP_61 && !WP_64 && !WP_45 && WP_46 && WP_47) **
**                                 26, 61,
**                                 LC_RPN_NOT,
**                                 LC_RPN_AND,
**                                 64,
**                                 LC_RPN_NOT,
**                                 LC_RPN_AND,
**                                 45,
**                                 LC_RPN_NOT,
**                                 LC_RPN_AND,
**                                 46,
**                                 LC_RPN_AND,
**                                 47,
**                                 LC_RPN_AND,
**                                 LC_RPN_EQUAL
**                               }
**    },
**
**    ** #47 **
**    {
**        .DefaultState        = LC_APSTATE_ENABLED,
**        .MaxPassiveEvents    = 2,
**        .MaxPassFailEvents   = 2,
**        .MaxFailPassEvents   = 2,
**        .RTSId               = RTS_ID_ACS_POWER_OFF_ALL_RW,
**        .MaxFailsBeforeRTS   = 2,
**        .EventType           = CFE_EVS_INFORMATION,
**        .EventID             = LC_BASE_AP_EID + 47,
**        .EventText           = { "GNC: wheel on, attached" },
**        .RPNEquation         = { ** (!WP_80 && (WP_48 || WP_49 || WP_50 || WP_51))) **
**                                 80,
**                                 LC_RPN_NOT,
**                                 48, 49, 50, 51,
**                                 LC_RPN_OR,
**                                 LC_RPN_OR,
**                                 LC_RPN_OR,
**                                 LC_RPN_AND,
**                                 LC_RPN_EQUAL
**                               }
**    },
**
**    ** #142 **
**    {
**        .DefaultState        = LC_APSTATE_DISABLED,
**        .MaxPassiveEvents    = 2,
**        .MaxPassFailEvents   = 2,
**        .MaxFailPassEvents   = 2,
**        .RTSId               = RTS_ID_LEND_POWER_OFF,
**        .MaxFailsBeforeRTS   = 60,
**        .EventType           = CFE_EVS_INFORMATION,
**        .EventID             = LC_BASE_AP_EID + 142,
**        .EventText           = { "LEND: comp over temp #1" },
**        .RPNEquation         = { ** (WP_142 && WP_143) || (WP_144 && WP_145) || (WP_146 && WP_147) **
**                                 142, 143,
**                                 LC_RPN_AND,
**                                 144, 145,
**                                 LC_RPN_AND,
**                                 146, 147,
**                                 LC_RPN_AND,
**                                 LC_RPN_OR,
**                                 LC_RPN_OR,
**                                 LC_RPN_EQUAL
**                               }
**    },
**
*************************************************************************/

/*************************************************************************
** Exported Data
*************************************************************************/
/*
** Table file header
*/
static CFE_TBL_FileDef_t CFE_TBL_FileDef __attribute__((__used__)) =
{
    "LC_DefaultADT", LC_APP_NAME "." LC_ADT_TABLENAME,
    "LC actionpoint definition table", "lc_def_adt.tbl",
    (sizeof(LC_ADTEntry_t) * LC_MAX_ACTIONPOINTS)
};

/*
** Default actionpoint definition table (ADT) data
*/
LC_ADTEntry_t LC_DefaultADT[LC_MAX_ACTIONPOINTS] =
{
    /* Thermal Action Points */



    /* #0 Open Louver when temperature is greater than or equal to 13C, in order to keep the system at an ideal observation temperature w/ a 3C buffer */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_THERM_LOUVER_OPEN_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1001,
        .EventText           = { "The Louver has been opened!" },
        .RPNEquation         = { 
                                 13,
                                 LC_RPN_NOT,
                                 16,
                                 LC_RPN_NOT,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

    /* #1 Close the Louver when the temperature falls below 13C, in order to keep the system from cooling down below nominal observation temperatures */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_THERM_LOUVER_CLOSE_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1002,
        .EventText           = { "The Louver has been Closed!" },
        .RPNEquation         = { 
                                 13,
                                 16,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

    /* #2 Apply power to the heater if the temperature is below 13C. Keeps the system at an ideal observation temperature, w/ a 13 buffer */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_THERM_HTR_ON_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1003,
        .EventText           = { "The Heater has been turned ON!" },
        .RPNEquation         = { 
                                 14,
                                 LC_RPN_NOT,
                                 16,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

    /* #3 Ensure that the heater is powered off if the temperature is at 13 or greater */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_THERM_HTR_OFF_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1004,
        .EventText           = { "The heater is powered OFF!" },
        .RPNEquation         = {
                                 16,
                                 LC_RPN_NOT,
                                 14,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

    /* #4 If SBC is not OFF and the temperature falls below 0C, report Fatal Error */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_NOOP_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1005,
        .EventText           = { "The system temperature has fallen below 0C, while the equipment was powered ON. Fatal Hardware Damage is imminent" },
        .RPNEquation         = { 
                                 9,
                                 LC_RPN_NOT,
                                 18,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

    /* #5 If SBC is not OFF and the temperature rises above 30C, report Fatal Error */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_NOOP_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1006,
        .EventText           = { "The system temperature has risen above 30C, while the equipment was powered ON. Fatal Hardware Damage is imminent" },
        .RPNEquation         = { 
                                 9,
                                 LC_RPN_NOT,
                                 19,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

   /* #6 If Capacitor A is charging, not active, and has a charge exceed 82% while the SBC is observing, then discharge Capactor A */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_CAP_A_DISCHARGE_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1007,
        .EventText           = { "Capacitor A is now discharging!" },
        .RPNEquation         = { 
                                 8,
                                 LC_RPN_NOT,
                                 5,
                                 LC_RPN_AND,
                                 11,
                                 LC_RPN_AND,
                                 0,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

    /* #7 If Capacitor B is charging, not active, and has a charge exceed 82% while the SBC is observing, then discharge Capactor B */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_CAP_B_DISCHARGE_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1008,
        .EventText           = { "Capacitor B is now discharging!" },
        .RPNEquation         = { 
                                 7,
                                 8,
                                 LC_RPN_AND,
                                 11,
                                 LC_RPN_AND,
                                 1,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

    /* #8 If Capacitor A is Discharging, then set Capacitor B as Active */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_CAP_B_ACTIVE_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1009,
        .EventText           = { "Capacitor B has been set as Active!" },
        .RPNEquation         = { 
                                 4,
                                 LC_RPN_EQUAL
                               }
    },

    /* #9 If Capacitor B is Discharging, then set Capacitor A as Active */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_CAP_A_ACTIVE_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1010,
        .EventText           = { "Capacitor A has been set as Active!" },
        .RPNEquation         = { 
                                 6,
                                 LC_RPN_EQUAL
                               }
    },

    /* #10 If the charge of Capacitor A is over the observation threshold and the charge of Capacitor B is below the threshold, while A is charging and no observation is taking place, then set Capacitor A as Active. */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_CAP_A_ACTIVE_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1011,
        .EventText           = { "Capacitor A has been set as Active!" },
        .RPNEquation         = { 
                                 21,
                                 LC_RPN_NOT,
                                 20,
                                 LC_RPN_AND,
                                 5,
                                 LC_RPN_AND,
                                 10,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

    /* #11 If the charge of Capacitor B is over the observation threshold and the charge of Capacitor A is below the threshold, while A is charging and no observation is taking place, then set Capacitor B as Active. */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_CAP_B_ACTIVE_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1012,
        .EventText           = { "Capacitor B has been set as Active!" },
        .RPNEquation         = { 
                                 20,
                                 LC_RPN_NOT,
                                 21,
                                 LC_RPN_AND,
                                 7,
                                 LC_RPN_AND,
                                 10,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

    /* #12 If Capacitor A is active, in the charging state, and the capacitor is over the observation threshold charge, then start an observation if the SBC is powered */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_OBS_START_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1013,
        .EventText           = { "An observation has been started with Capacitor A!" },
        .RPNEquation         = { 
                                 5,
                                 8,
                                 LC_RPN_AND,
                                 20,
                                 LC_RPN_AND,
                                 10,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

    /* #13 If Capacitor B is active, in the charging state, and the capacitor is over the observation threshold charge, then start an observation if the SBC is powered */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_OBS_START_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1014,
        .EventText           = { "An observation has been started with Capcitor B!" },
        .RPNEquation         = { 
                                 8,
                                 LC_RPN_NOT,
                                 7,
                                 LC_RPN_AND,
                                 21,
                                 LC_RPN_AND,
                                 10,
                                 LC_RPN_AND,
                                 LC_RPN_EQUAL
                               }
    },

    /* #14 The SBC is in an unrecoverable Error state */
    {
        .DefaultState        = LC_APSTATE_ACTIVE,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = WHE_NOOP_CC,
        .MaxFailsBeforeRTS   = 1,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 1015,
        .EventText           = { "The SBC is in an unrecoverable Error state!" },
        .RPNEquation         = { 
                                 12,
                                 LC_RPN_EQUAL
                               }
    },

    /* #15 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #16 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #17 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #18 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #19 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #20 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #21 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #22 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #23 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #24 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #25 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #26 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #27 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #28 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #29 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #30 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #31 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #32 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #33 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #34 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #35 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #36 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #37 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #38 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #39 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #40 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #41 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #42 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #43 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #44 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #45 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #46 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #47 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #48 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #49 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #50 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #51 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #52 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #53 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #54 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #55 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #56 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #57 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #58 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #59 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #60 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #61 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #62 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #63 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #64 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #65 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #66 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #67 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #68 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #69 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #70 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #71 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #72 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #73 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #74 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #75 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #76 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #77 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #78 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #79 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #80 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #81 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #82 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #83 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #84 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #85 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #86 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #87 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #88 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #89 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #90 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #91 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #92 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #93 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #94 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #95 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #96 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #97 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #98 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #99 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #100 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #101 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #102 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #103 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #104 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #105 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #106 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #107 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #108 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #109 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #110 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #111 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #112 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #113 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #114 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #115 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #116 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #117 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #118 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #119 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #120 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #121 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #122 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #123 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #124 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #125 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #126 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #127 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #128 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #129 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #130 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #131 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #132 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #133 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #134 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #135 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #136 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #137 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #138 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #139 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #140 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #141 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #142 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #143 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #144 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #145 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #146 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #147 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #148 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #149 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #150 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #151 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #152 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #153 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #154 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #155 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #156 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #157 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #158 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #159 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #160 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #161 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #162 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #163 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #164 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #165 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #166 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #167 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #168 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #169 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #170 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #171 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #172 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #173 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #174 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    },

    /* #175 (unused) */
    {
        .DefaultState        = LC_ACTION_NOT_USED,
        .MaxPassiveEvents    = 0,
        .MaxPassFailEvents   = 0,
        .MaxFailPassEvents   = 0,
        .RTSId               = 0,
        .MaxFailsBeforeRTS   = 0,
        .EventType           = CFE_EVS_INFORMATION,
        .EventID             = 0,
        .EventText           = { " " },
        .RPNEquation         = { /* (WP_0) */
                                 0,
                                 LC_RPN_EQUAL
                               }
    }
}; /* end LC_DefaultADT */

/************************/
/*  End of File Comment */
/************************/
