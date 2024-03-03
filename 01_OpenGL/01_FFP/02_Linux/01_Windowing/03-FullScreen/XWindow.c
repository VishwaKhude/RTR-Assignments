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

Bool bFullScreen = False;

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
  memset((void*)&visualInfo, 0, sizeof(XVisualInfo));
  status = XMatchVisualInfo(display, defaultScreen, defaultDepth, TrueColor, &visualInfo);
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
  styleMask = CWBorderPixel | CWBackPixel | colormap | CWEventMask;

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

  //Center The Window
  ScreenWidth = XWidthOfScreen(XScreenOfDisplay(display, visualInfo.screen));
  ScreenHeight = XHeightOfScreen(XScreenOfDisplay(display, visualInfo.screen));
  XMoveWindow(display, window, (ScreenWidth - WIN_WIDTH)/2, (ScreenHeight - WIN_HEIGHT));

  //Event-Loop
  while(1)
  {
    XNextEvent(display, &event);

    switch(event. type)
    {
      case KeyPress:
        keySym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);

        switch(keySym)
        {
          case XK_Escape:
               keySym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
          break;
          
          default:
          break;

        }

        XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL); //last NULL parameter - XCompose(value not required in this code)
              switch(keys[0])
              {
                case 'F':
                case 'f':
                     if (bFullScreen == False)
                     {
                        toggleFullScreen();
                        bFullScreen = True;
                     }
                     else 
                     {
                        toggleFullScreen();
                        bFullScreen = False;
                     }
                break;
                
                default:
                break;

              }
        break;

       case 33:
          uninitialize();
          exit(0);
       break;

      default:
      break;
    }

  }
  
}

void toggleFullScreen(void)
{
  // local variable declarations
  Atom windowManagerStateNormal;
  Atom windowManagerStateFullScreen;

  XEvent event;

  // code
  windowManagerStateNormal = XInternAtom(display, "_NET_WM_STATE", False);

  windowManagerStateFullScreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

  memset((void*)&event, 0, sizeof(XEvent));
  event.type = ClientMessage;
  event.xclient.window = window;
  event.xclient.message_type = windowManagerStateNormal;
  event.xclient.format = 32;
  event.xclient.data.l[0] = bFullScreen?0:1;  // ? - if
  event.xclient.data.l[1] = windowManagerStateFullScreen;

  // Send the event
  XSendEvent(display, XRootWindow(display, visualInfo.screen), False, SubstructureNotifyMask, &event);
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

