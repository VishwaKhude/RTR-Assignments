// Standard Header Files
#include <stdio.h>  // for printf()
#include <stdlib.h>  // for exit()
#include <memory.h>  // for memset

// X11 Header Files
#include <X11/Xlib.h>
#include <X11/Xutil.h>  //for XvisualInfo and related APIs
#include <X11/XKBlib.h>

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
 
// Global Variable Declarations
Display *display = NULL;
Colormap colormap;
Window window;
XVisualInfo visualInfo;

BOOL bFullScreen = FALSE;

int main(void)
{

  //Local Function Declartions
  void uninitialize(void);
  void toggleFullScreen(void);

  //Local Variable Declarations
  int defaultScreen;
  int defaultDepth;
  Status status;
  XSetWindowAttributes windowAttributes;
  int styleMask;
  Atom windowManagerDelete;
  XEvent event;
  KeySym keySym;
  char keys[26];
  int ScreenWidth, ScreenHeight;

  static XFontStruct *pFontStruct = NULL;
  static int winWidth, winHeight;
  static GC gc;
  XColor greenColor;
  XGCValues gcValues;
  int stringLength;
  int fontHeight;
  char str[] = "Hello World !!!"; 

  //Code

  //step-1 Open connection with X-server And Get The Display Interface
  display = XOpenDisplay(NULL);

  if (display == NULL)
  {
    printf("XOpenDisplay failed \n");
    uninitialize();
    exit(1);
  }

  //step-2 Get default screen from above display
  defaultScreen = XDefaultScreen(display);

  //step-3 Get default depth from above two
  defaultDepth = XDefaultDepth(display, defaultScreen);

  //step-4 Get visual info from above three
  memset((void*)&VisualInfo, 0, sizeof(XVisualInfo));
  status = XMatchVisualInfo(display, defaultScreen, defaultDepth, TrueColor, &VisualInfo);
  if (status ==0)
  {
    printf("XMatchVisualInfo failed() \n");
    uninitialize();
    exit(1);
  }

  //step-5 Set Window attributes/properties
  memset((void*)&windowAttributes, 0, sizeof(XSetWindowAttributes));
  windowAttributes.border_pixel = 0;
  windowAttributes.background_pixel = XBlackPixel(display, visualInfo.screen);
  windowAttributes.background_pixmap = 0;
  windowAttributes.colormap = XCreateColormap(display, 
                                              XRootWindow(display, visualInfo.screen), 
                                              visualInfo.visual,
                                              AllocNone);

  //step-6 Assign this colormap to Global Colormap
  colormap = windowAttributes.colormap;

  //step-7 Set the style of window
  stylemask = CWBorderPixel | CWBackPixel | Colormap | CWEventMask;

  //step-8 Create the window
  window = XCreateWindow(display,
                        XRootWindow(display, visualInfo.screen), 
                        0,            //topleft x-Coordinate
                        0,            //topleft y-Coordinate
                        WIN_WIDTH,    //Window's Width
                        WIN_HEIGHT,   //Window Height
                        0,            //Border Width(default)
                        visualInfo.depth,
                        InputOutput,
                        visualInfo.visual,
                        styleMask,
                        &windowAttributes);
  
  if (!window)
  {
    printf("XCreateWindow() failed \n");
    uninitialize();
    exit(1);
  }

  //Step-9 Specify To Which Events This Window Should Response
  XSelectInput(display,
               window,
               ExposureMask | VisibilityChangeMask  | StructureNotifyMask | 
               KeyPressMask | ButtonPressMask | PointerMotionMask);

  // Step-10 Specify Window Manager Delete Atom
  windowManagerDelete = XInternAtom(display, "WM_DELETE_WINDOW", True);

  // Step-11 Add/Set Above Atom As Protocol For Window Manager
  XSetWMProtocols(display, window, &windowManagerDelete, 1);

  //Step-12 Give Caption-Name To Window
  XStoreName(display, window, "Vishwa Mahesh Khude's : XWindow");

  //Step-13 Show/Map the Window
  XMapWindow(display, window);

  //Event-Loop
  while(1)
  {
    XNextEvent(display, &event);

    switch(event, type)
    {
      case MapNotify:
        pFontStruct = XLoadQueryFont(display, "fixed");
        break;

      case FocusIn:
        break;
      
      case FocusOut:
        break;

      case ConfigureNotify:
        winWidth = event.xconfigure.width;
        winHeight = event.xconfigure.height;
        break;

      case Expose:
        gc = XCreateGC(display, window, 0, &cValues);
        XSetFont(display, colormap, pFontStruct->fid);
        XAllocNamedColor(display, colormap, "green", &greenColor, &greenColor);
        XSetForeground(display, gc, greenColor.pixel);

        stringLength = strlen(str);
        stringWidth = XTextWidth(pFontStruct, str, stringLength);
        stringLength = pFontStruct->ascent+pFontStruct->descent;
        XDrawString(display, window, gc, (winWidth - stringWidth) /2, (winHeight - fontHeight) /2, str, stringLength);
        break;

      case ButtonPress:
        switch (event.xbutton.button)
        {
          case 1:
            break;
          
          case 2:
            break;

          case 3:
            break;

          default:
            break;
        }

        break;
      
      case KeyPress:
        keySym = XkbkeycodeToKeysym(display, event.xkey.keycode, 0, 0);

        switch(keySym)
        {
          case XK_Escape:
                XUnloadFont(display, pFontStruct->fid);
                XFreeGC(display, gc);
                uninitialize();
                exit(0);
          break;

          default:
          break;

        }
        break;

      case 33:
          XUnloadFont(display, pFontStruct->fid);
          XFreeGC(display, gc);
          uninitialize();
          exit(0);
      break;

      default:
      break;
    }

  }
  
}

void uninitialize(void)
{
    //Code
    if(window)
    {
        XDestroyWindow(display, window);
    }

    if(colormap)
    {
        XFreeColormap(display, colormap);
    }

    if(display)
    {
        XCloseDisplay(display);
        display = NULL;
    }
}

