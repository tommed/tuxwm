#include <X11/keysym.h>

//
// Defaults go here!
//

#define PRODUCT_NAME_AND_VERSION    "Toms Unique X Window Manager - 0.1.0"

// launch commands
#define LAUNCHER_CMD                "`dmenu_path | dmenu -b -nb \"#000\" -nf \"#fff\" -sb \"#49a\" -sf \"#000\"` -fn \"-*-andale mono-*-*-*-*-17-*-*-*-*-*-*-*\" &"

// action keys
#define ACTIONKEY_QUIT              XK_q
#define ACTIONKEY_LAUNCHER          XK_p
#define ACTIONKEY_SNAPSHOT          XK_c
#define ACTIONKEY_KILL_WINDOW       XK_w
#define ACTIONKEY_NEXT_WINDOW       XK_Tab
#define ACTIONKEY_HIDE_ALL_WINDOWS  XK_F11

// dock panel
#define DOCKPANEL_HEIGHT            35
#define DOCKPANEL_BG_RGB_LIGHT      0.25, 0.30, 0.30
#define DOCKPANEL_BG_RGB_MEDIUM     0.17, 0.22, 0.22
#define DOCKPANEL_BG_RGB_DARK       0.05, 0.10, 0.10
#define DOCKPANEL_LOGO_LETTER       "W"
#define DOCKPANEL_LOGO_FONT_PATH    "/usr/share/fonts/TTF/DingsbumsBats.ttf"

// root window (aka desktop)
#define ROOT_WINDOW_FONT_PATH       "/usr/share/fonts/TTF/crop_types.ttf"
#define ROOT_WINDOW_BG_RGB          0.30, 0.63, 0.64
#define ROOT_WINDOW_FG_RGB          0.70, 0.89, 0.89
#define ROOT_WINDOW_FONT_SIZE       18
#define LAYOUT_MARGIN               8
