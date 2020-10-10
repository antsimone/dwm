/* See LICENSE file for copyright and license details. */

/* -- window sizing -- */
static const unsigned int borderpx = 2;    /* border pixel of windows */
static const unsigned int snap     = 32;   /* snap pixel */
static const unsigned int gappx    = 0;    /* gap pixel between windows */
static const float mfact           = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster           = 1;    /* number of clients in master area */
static const int resizehints       = 0;    /* 1 means respect size hints in tiled resizals */

/* -- bar -- */
static const int showbar           = 1;    /* 0 means no bar */
static const int topbar            = 1;    /* 0 means bottom bar */
static const char *fonts[]         = { "monospace:pixelsize=12" };
static const char dmenufont[]      = "monospace:pixelsize=12";

/* -- colors -- */
static const char col_gray1[]      = "#222222";
static const char col_gray2[]      = "#444444";
static const char col_gray3[]      = "#bbbbbb";
static const char col_gray4[]      = "#eeeeee";
static const char col_cyan[]       = "#005577";

/* -- fg - bg - border -- */
static const char *colors[][3]     = {
    [SchemeNorm] = { col_gray3, "#000000", col_gray2 },
    [SchemeSel]  = { "#ffa64f", "#404040", "#0066ff" },
};

/* -- tags -- */
static const char *tags[] = { "1", "2", "3", "4" };

/* -- layouts -- */
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },       /* first entry is default */
    { "[M]",      monocle }, 
    { "><>",      NULL },       /* no layout function means floating behavior */
};

/* -- window rules -- */
static const Rule rules[] = {
    /* xprop(1):
     * WM_CLASS(STRING) = instance, class
     * WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask    iscentered    isfloating   monitor */
    { "Gimp"    , NULL,       NULL,       0,            0,          1,           -1 },
    { "Leafpad" , NULL,       NULL,       0,            1,          1,           -1 },

};

/* -- key definitions -- */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define XF86XK_MonBrightnessDown 0x1008ff03
#define XF86XK_MonBrightnessUp   0x1008ff02
#define XF86XK_AudioMute    	 0x1008ff12
#define XF86XK_AudioLowerVolume  0x1008ff11
#define XF86XK_AudioRaiseVolume  0x1008ff13

/* -- shell commands -- */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* -- commands -- */
static char dmenumon[2]       = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL};
static const char *termcmd[]  = { "st", NULL };
static const char *firefox[]  = { "firefox", NULL };

/* -- manual patches -- */
static void shiftview(const Arg *arg); /* shift tags view */
static void movestack(const Arg *arg); /* rotate windows in stack */

/* -- key bindings --*/
static Key keys[] = {
    TAGKEYS(XK_1       , 0 )
        TAGKEYS(XK_2       , 1 )
        TAGKEYS(XK_3       , 2 )
        TAGKEYS(XK_4       , 3 )
        /* modifier                     key        function        argument */
        { MODKEY|ShiftMask , XK_period                  ,spawn,          SHCMD("dmenu_shutdown"                                          ) },
        { 0                , XF86XK_MonBrightnessUp     ,spawn,          SHCMD("xbacklight -inc 10"                                      ) },
        { 0                , XF86XK_MonBrightnessDown   ,spawn,          SHCMD("xbacklight -dec 10"                                      ) },
        { 0                , XF86XK_AudioMute           ,spawn,          SHCMD("pamixer -t && pkill -RTMIN+3 dwmblocks"                  ) },
        { 0                , XF86XK_AudioLowerVolume    ,spawn,          SHCMD("pamixer -u && pamixer --allow-boost -d 10 && pkill -RTMIN+3 dwmblocks" ) },
        { 0                , XF86XK_AudioRaiseVolume    ,spawn,          SHCMD("pamixer -u && pamixer --allow-boost -i 10 && pkill -RTMIN+3 dwmblocks" ) },
        { MODKEY           , XK_period                  ,spawn,          {.v = dmenucmd    } },
        { MODKEY           , XK_Return                  ,spawn,          {.v = termcmd     } },
        { MODKEY           , XK_comma                   ,spawn,          {.v = firefox     } },
        { MODKEY           , XK_b                       ,togglebar,      {0                } },
        { MODKEY           , XK_j                       ,focusstack,     {.i = +1          } },
        { MODKEY           , XK_k                       ,focusstack,     {.i = -1          } },
        { MODKEY           , XK_i                       ,incnmaster,     {.i = +1          } },
        { MODKEY           , XK_d                       ,incnmaster,     {.i = -1          } },
        { MODKEY           , XK_h                       ,setmfact,       {.f = -0.05       } },
        { MODKEY           , XK_l                       ,setmfact,       {.f = +0.05       } },
        { MODKEY|ShiftMask , XK_Return                  ,zoom,           {0                } },
        { MODKEY           , XK_Tab                     ,view,           {0                } },
        { MODKEY           , XK_c                       ,killclient,     {0                } },
        { MODKEY           , XK_t                       ,setlayout,      {.v = &layouts[0] } },
        { MODKEY           , XK_f                       ,setlayout,      {.v = &layouts[2] } },
        { MODKEY           , XK_m                       ,setlayout,      {.v = &layouts[1] } },
        { MODKEY           , XK_n                       ,shiftview,      {.i = +1          } },
        { MODKEY           , XK_p                       ,shiftview,      {.i = -1          } },
        { MODKEY|ShiftMask , XK_j                       ,movestack,      {.i = +1          } },
        { MODKEY|ShiftMask , XK_k                       ,movestack,      {.i = -1          } },
        { MODKEY           , XK_space                   ,setlayout,      {0                } },
        { MODKEY|ShiftMask , XK_space                   ,togglefloating, {0                } },
        { MODKEY           , XK_0                       ,view,           {.ui = ~0         } },
        { MODKEY|ShiftMask , XK_0                       ,tag,            {.ui = ~0         } },
        { MODKEY|ShiftMask , XK_comma                   ,tagmon,         {.i = -1          } },
        { MODKEY|ShiftMask , XK_q                       ,quit,           {0                } },

};

/* -- buttons -- */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button1,        zoom,           {0} },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        toggleview,     {0} },
    { ClkTagBar,            0,              Button3,        view,           {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
