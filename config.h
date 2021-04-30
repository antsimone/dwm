/* See LICENSE file for copyright and license details. */

/* Appearance */

static const int nmaster           = 1;    /* number of clients in master area */
static const int resizehints       = 0;    /* 1 means respect size hints in tiled resizals */
static const float mfact           = 0.55; /* factor of master area size [0.05..0.95] */
static const unsigned int borderpx = 1;    /* border pixel of windows */
static const unsigned int snap     = 32;   /* snap pixel */
static const unsigned int gappx    = 0;    /* gap pixel between windows */

static const int showbar           = 1; /* 0 means no bar */
static const int topbar            = 1; /* 0 means bottom bar */

static const char *fonts[]         = { "Terminus (TTF):pixelsize=12" };

static const char col_normfg[]     = "#969696"; /* "#bcbcbc" */
static const char col_normbg[]     = "#121212"; /* "#232323" */
static const char col_normborder[] = "#232323";
static const char col_selfg[]      = "#232323"; //"#d0913d"; /* "#dd8844" "#d89333" */
static const char col_selbg[]      = "#87875f"; //"#474747"; /* "#1b1d24" "#2a2e38" */
static const char col_selborder[]  = "#446fbd";



static const char *colors[][3] = {
    [SchemeNorm] = { col_normfg, col_normbg, col_normborder },
    [SchemeSel]  = { col_selfg, col_selbg,  col_selborder  },

    /* fg    background     null */
    [SchemeStatus]   = { col_normfg, col_normbg, "#000000" },
    [SchemeTagsSel]  = { col_selfg, col_selbg, "#000000" },
    [SchemeTagsNorm] = { "#696969", col_normbg, "#000000" },
    [SchemeInfoSel]  = { col_normfg, col_normbg, "#000000" },
    [SchemeInfoNorm] = { col_normfg, col_normbg, "#000000" },
};

/* Tags */

static const char *tags[] = { "1", "2", "3", "4", "5" };

/* Layouts */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },       /* first entry is default */
    { "[M]",      monocle },
    { "><>",      NULL },       /* no layout function means floating behavior */
};

/* Window rules */
static const Rule rules[] = {
    /* xprop(1):
     * WM_CLASS(STRING) = instance, class
     * WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask    iscentered    isfloating   monitor */
    { "Gimp"    , NULL,       NULL,       0,            0,          1,           -1 },
};

/* Key definitions */

#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY, view, {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY, toggleview, {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY, tag, {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY, toggletag, {.ui = 1 << TAG} },

#define XF86XK_MonBrightnessDown 0x1008ff03
#define XF86XK_MonBrightnessUp   0x1008ff02
#define XF86XK_AudioMute    	 0x1008ff12
#define XF86XK_AudioLowerVolume  0x1008ff11
#define XF86XK_AudioRaiseVolume  0x1008ff13

/* Shell commands */

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define VOL_MUTE "pamixer -t && pkill -RTMIN+3 dwmblocks"
#define VOL_LOWER "pamixer -u && pamixer --allow-boost -d 10 && pkill -RTMIN+3 dwmblocks"
#define VOL_RAISE "pamixer -u && pamixer --allow-boost -i 10 && pkill -RTMIN+3 dwmblocks"
#define SCREEN "maim -u ~/pics/$(date +%s).png"
#define SCREEN_CPY "maim -s --format png /dev/stdout | xclip -selection clipboard -t image/png -i"

/* Programs and commands */

static char dmenumon[2]       = "0";
static const char *dmenucmd[] = { "dmenu_run", NULL};
static const char *shut[]     = { "dmenu_shutdown", NULL };
static const char *term[]     = { "st", NULL };
static const char *browser[]  = { "firefox", NULL };
static const char *fm[]       = { "pcmanfm", NULL };
static const char *xbinc[]    = { "xbacklight", "-inc", "10", NULL };
static const char *xbdec[]    = { "xbacklight", "-dec", "10", NULL };
static const char *emacs[]    = { "emacsclient", "-ca", "", NULL };

/* Declare custom functions */

static void shiftview(const Arg *arg); /* shift tags view */
static void movestack(const Arg *arg); /* rotate windows in stack */

/* Key bindings */

static Key keys[] = {
  /* modifier         key        function        argument */
  TAGKEYS(            XK_1,                                 0 )
  TAGKEYS(            XK_2,                                 1 )
  TAGKEYS(            XK_3,                                 2 )
  TAGKEYS(            XK_4,                                 3 )
  TAGKEYS(            XK_5,                                 4 )
  { MODKEY|ShiftMask, XK_period,                spawn,      {.v = shut} },
  { 0,                XF86XK_MonBrightnessUp,   spawn,      {.v = xbinc} },
  { 0,                XF86XK_MonBrightnessDown, spawn,      {.v = xbdec} },
  { 0,                XF86XK_AudioMute,         spawn,      SHCMD(VOL_MUTE) },
  { 0,                XF86XK_AudioLowerVolume,  spawn,      SHCMD(VOL_LOWER) },
  { 0,                XF86XK_AudioRaiseVolume,  spawn,      SHCMD(VOL_RAISE) },
  { 0,                XK_Print,                 spawn,      SHCMD(SCREEN)},
  { MODKEY,           XK_Print,                 spawn,      SHCMD(SCREEN_CPY)},
  { MODKEY,           XK_period,                spawn,      {.v = dmenucmd} },
  { MODKEY,           XK_Return,                spawn,      {.v = term} },
  { MODKEY,           XK_e,                     spawn,      {.v = emacs} },
  { MODKEY,           XK_comma,                 spawn,      {.v = browser} },
  { MODKEY,           XK_f,                     spawn,      {.v = fm} },
  { MODKEY,           XK_space,                 setlayout,  {0 } },
  { MODKEY,           XK_n,                     shiftview,  {.i = +1 } },
  { MODKEY,           XK_p,                     shiftview,  {.i = -1 } },
  { MODKEY,           XK_j,                     focusstack, {.i = +1 } },
  { MODKEY,           XK_k,                     focusstack, {.i = -1 } },
  { MODKEY|ShiftMask, XK_j,                     movestack,  {.i = +1 } },
  { MODKEY|ShiftMask, XK_k,                     movestack,  {.i = -1 } },
  { MODKEY,           XK_h,                     setmfact,   {.f = -0.05 } },
  { MODKEY,           XK_l,                     setmfact,   {.f = +0.05 } },
  { MODKEY,           XK_i,                     incnmaster, {.i = +1 } },
  { MODKEY,           XK_d,                     incnmaster, {.i = -1 } },
  { MODKEY,           XK_b,                     togglebar,  {0 } },
  { MODKEY,           XK_c,                     killclient, {0 } },
  { MODKEY,           XK_0,                     view,       {.ui = ~0 } },
  { MODKEY,           XK_Tab,                   view,       {0 } },
  { MODKEY|ShiftMask, XK_Return,                zoom,       {0 } },
  { MODKEY|ShiftMask, XK_0,                     tag,        {.ui = ~0 } },
  { MODKEY|ShiftMask, XK_comma,                 tagmon,     {.i = -1 } },
  { MODKEY|ShiftMask, XK_q,                     quit,       {0 } },
};

/* Buttons */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button1,        zoom,           {0} },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        toggletag,      {0} },
    { ClkTagBar,            MODKEY,         Button3,        tag,            {0} },
};


void
movestack(const Arg *arg) {
    Client *c = NULL, *p = NULL, *pc = NULL, *i;

    if(arg->i > 0) {
        for(c = selmon->sel->next; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);
        if(!c)
            for(c = selmon->clients; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);
    }
    else {
        for(i = selmon->clients; i != selmon->sel; i = i->next)
            if(ISVISIBLE(i) && !i->isfloating)
                c = i;
        if(!c)
            for(; i; i = i->next)
                if(ISVISIBLE(i) && !i->isfloating)
                    c = i;
    }
    for(i = selmon->clients; i && (!p || !pc); i = i->next) {
        if(i->next == selmon->sel)
            p = i;
        if(i->next == c)
            pc = i;
    }

    /* Swap c and selmon->sel selmon->clients in the selmon->clients list */
    if(c && c != selmon->sel) {
        Client *temp = selmon->sel->next==c?selmon->sel:selmon->sel->next;
        selmon->sel->next = c->next==selmon->sel?c:c->next;
        c->next = temp;

        if(p && p != c)
            p->next = c;
        if(pc && pc != selmon->sel)
            pc->next = selmon->sel;

        if(selmon->sel == selmon->clients)
            selmon->clients = c;
        else if(c == selmon->clients)
            selmon->clients = selmon->sel;

        arrange(selmon);
    }
}

void
shiftview(const Arg *arg) {
    Arg shifted;

    /* Left circular shift */
    if(arg->i > 0)
        shifted.ui = (selmon->tagset[selmon->seltags] << arg->i)
            | (selmon->tagset[selmon->seltags] >> (LENGTH(tags) - arg->i));
    /* Right circular shift */
    else
        shifted.ui = selmon->tagset[selmon->seltags] >> (- arg->i)
            | selmon->tagset[selmon->seltags] << (LENGTH(tags) + arg->i);

    view(&shifted);
}
