/* See LICENSE file for copyright and license details. */

// needed for XF86 keys
#include <X11/XF86keysym.h>

// common definitions
#define TERMINAL "xfce4-terminal"
//#define TERMINAL "kitty"

/* appearance */
static const unsigned int borderpx       = 3;        /* border pixel of windows */
static const unsigned int snap           = 32;       /* snap pixel */
static const unsigned int gappih         = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv         = 10;       /* vert inner gap between windows */
static const unsigned int gappoh         = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov         = 10;       /* vert outer gap between windows and screen edge */
static const unsigned int systraypinning = 0;        /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;        /* systray spacing */
static const int systraypinningfailfirst = 1;        /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;        /* 0 means no systray */
static const int smartgaps               = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar                 = 1;        /* 0 means no bar */
static const int topbar                  = 1;        /* 0 means bottom bar */

static char *fonts[]          = { "monospace:size=10", "JoyPixels:pixelsize=10:antialias=true:autohint=true"  };
static const char dmenufont[]       = "Ubuntu Mono:style=Regular:size=12";
static char normbgcolor[]           = "#000000";
static char normbordercolor[]       = "#D8DEE9";
static char normfgcolor[]           = "#4C566A";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#770000";
static char selbgcolor[]            = "#000000";
static char *colors[][3] = {
	/*						fg					bg					border   */
	[SchemeNorm]      = { normfgcolor,			normbgcolor,		normbordercolor },
	[SchemeSel]       = { selfgcolor,  			selbgcolor,  		selbordercolor  },
	[SchemeStatus]    = { selfgcolor,   		selbgcolor,   		"#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]   = { "#000000",   		    "#ECEFF4",    		"#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm]  = { "#000000",   		    "#c7c0b1",   		"#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]   = { selfgcolor,   		"#A3BE8C",    		"#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]  = { normfgcolor,   		normbgcolor,   		"#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
 };

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class           instance    title       tags mask     isfloating   monitor */

	// these are all floating by default
	{ "Gimp",           NULL,       NULL,       0,            1,           -1 },
	{ "ROOT",			NULL,       NULL,       0,            1,           -1 },
	{ "XClock",			NULL,       NULL,       0,            1,           -1 },
	{ "Ristretto",		NULL,       NULL,       0,            1,           -1 },
	{ "Display",		NULL,       NULL,       0,            1,           -1 },
	{ "zoom",		    NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

/* layouts */
#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",     dwindle },
	// MUST be last
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 * I don't use this yet
 */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,						XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	//{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	//{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,           XK_f,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,		        XK_space,  cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	//{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask,             XK_q,      quit,           {0} },

	//  vanity gaps, doubt I need these but leave them here for reference
	// { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } },
	// { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } },
	// { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	// { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	// { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	// { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	// { MODKEY|Mod4Mask,              XK_0,      togglegaps,     {0} },
	// { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	// { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
	// { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
	// { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
	// { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
	// { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } },
	// { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } },
	// { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
	

	// volume keys
	{ 0,	XF86XK_AudioMute,			spawn,		SHCMD("pactl set-sink-mute 0 toggle && pkill -RTMIN+2 dwmblocks") },
	{ 0,	XF86XK_AudioRaiseVolume,	spawn,		SHCMD("pactl set-sink-volume 0 +5% && pkill -RTMIN+2 dwmblocks") },
	{ 0,	XF86XK_AudioLowerVolume,	spawn,		SHCMD("pactl set-sink-volume 0 -5% && pkill -RTMIN+2 dwmblocks") },

	// for keyboards without media buttons
	{ MODKEY,			XK_m,						spawn,		SHCMD("pactl set-sink-mute 0 toggle && pkill -RTMIN+2 dwmblocks") },
	{ MODKEY,			XK_equal,					spawn,		SHCMD("pactl set-sink-volume 0 +5% && pkill -RTMIN+2 dwmblocks") },
	{ MODKEY,			XK_minus,					spawn,		SHCMD("pactl set-sink-volume 0 -5% && pkill -RTMIN+2 dwmblocks") },

	//{ 0,	XF86XK_AudioRaiseVolume,	spawn,		SHCMD("pactl set-sink-volume 0 +1% && pkill -RTMIN+2 dwmblocks") },
	//{ 0,	XF86XK_AudioLowerVolume,	spawn,		SHCMD("pactl set-sink-volume 0 -1% && pkill -RTMIN+2 dwmblocks") },

	// caps and numlock
	{ 0,	LockMask,			spawn,		SHCMD("pkill -SIGRTMIN+11 dwmblocks") },
	{ 0,	Mod2Mask,			spawn,		SHCMD("pkill -SIGRTMIN+11 dwmblocks") },

	// cmus
	{ MODKEY,	XK_c,						spawn,		SHCMD(TERMINAL " -e cmus") },
	{ 0,		XF86XK_AudioPlay,			spawn,		SHCMD("cmus-remote -u && pkill -RTMIN+1 dwmblocks") },
	{ 0,		XF86XK_AudioPause,			spawn,		SHCMD("cmus-remote -u && pkill -RTMIN+1 dwmblocks") },
	{ MODKEY,	XK_p,						spawn,		SHCMD("cmus-remote -u && pkill -RTMIN+1 dwmblocks") },
	{ 0,		XF86XK_AudioNext,			spawn,		SHCMD("cmus-remote -n && pkill -RTMIN+1 dwmblocks") },
	{ 0,		XF86XK_AudioPrev,			spawn,		SHCMD("cmus-remote -r && pkill -RTMIN+1 dwmblocks") },

	// screenshots
	// credit: https://bbs.archlinux.org/viewtopic.php?id=86507
	{ 0,				XK_Print,			spawn,			SHCMD("sleep 0.2; scrot -s && notify-send -u low -t 3000 'Screen shot taken'") },
	//{ ShiftMask,		XK_Print,			spawn,			SHCMD("scrot -s && notify-send -u low -t 3000 'Screen shot taken'") },
	
	// launch programs
	{ MODKEY,				XK_w,		spawn,		SHCMD("google-chrome-stable") },
	{ MODKEY|ShiftMask,		XK_w,		spawn,		SHCMD("firefox") },
	{ MODKEY,				XK_s,		spawn,		SHCMD("skypeforlinux") },
	{ MODKEY,				XK_z,		spawn,		SHCMD("zoom") },
	{ MODKEY,				XK_u,		spawn,		SHCMD("~/.local/bin/bar_scripts/poweroptions") },

	//{ MODKEY|ControlMask,	XK_w,		spawn,		SHCMD(TERMINAL " -e nmtui") },

	// poweroptions
	{ 0,					XF86XK_PowerOff, spawn,		SHCMD("~/.local/bin/bar_scripts/poweroptions")},
	{ MODKEY|ShiftMask,		XK_u,			 spawn,		SHCMD("~/.local/bin/shutdown_script") },
	{ MODKEY|ShiftMask,		XK_r,			 spawn,		SHCMD("~/.local/bin/reboot") },

	// lock screen
	{ MODKEY,				XK_x,		spawn,		SHCMD("~/.local/bin/lock") },
	{ MODKEY|ShiftMask,		XK_x,		spawn,		SHCMD("slock") },

	// choose password
	{ MODKEY|ShiftMask,		XK_p,		spawn,		SHCMD("~/.local/bin/copy_password") },

	// choose Danish characters
	{ MODKEY,				XK_i,		spawn,		SHCMD("~/.local/bin/choose_danish_characters") },

	// newsboat
	{ MODKEY,				XK_n,		spawn,		SHCMD(TERMINAL " -e newsboat") },
	
	// vimwiki
	{ MODKEY|ShiftMask,		XK_n,		spawn,		SHCMD(TERMINAL " -e $EDITOR $VIMWIKI/index.wiki") },
	
	// taskwarrior
	//{ MODKEY|ShiftMask,	    XK_t,		spawn,		SHCMD(TERMINAL " -e task --hold") },

	// thunar (file browser)
	{ MODKEY|ShiftMask,	    XK_b,		spawn,		SHCMD("thunar") },

	// toggle sticky window
	// won't really use this
	//{ MODKEY|ShiftMask,		XK_s,		togglesticky,	{0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/* quitting */
static const int EMPTY_WINDOW_COUNT = 2;
