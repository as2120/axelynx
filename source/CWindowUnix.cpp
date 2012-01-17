#ifdef UNIX
#include "CWindowUnix.h"



#include <cassert>
#include "CCanvas.h"
#include "axelynx/Logger.h"
#include "axelynx/utils/axstring.h"
#include <stdlib.h>

CWindowUnix* CWindowUnix::instance_ =0;

static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, None};
static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

void fatalError(const char *message)
{
  fprintf(stderr, "main: %s\n", message);
  exit(1);
}

CWindowUnix::CWindowUnix()
	:dpy_(0),win_(0),cx_(0),doubleBuffer_(true),isActive_(true),isRunning_(true)
{
}


CWindowUnix::~CWindowUnix()
{
    Free();
}

bool CWindowUnix::Init(int width, int height, int bpp, axelynx::WindowMode wm, int samples)
{
XVisualInfo         *vi;
  Colormap             cmap;
  XSetWindowAttributes swa;
  int                  dummy;

    width_ = width;
    height_ = height;

  /*** (1) open a connection to the X server ***/

  dpy_ = XOpenDisplay(NULL);
  if (dpy_ == NULL)
    fatalError("could not open display");

int nelements;
	GLXFBConfig *fbc = glXChooseFBConfig(dpy_, DefaultScreen(dpy_), 0, &nelements);


  /*** (2) make sure OpenGL's GLX extension supported ***/

  if(!glXQueryExtension(dpy_, &dummy, &dummy))
    fatalError("X server has no OpenGL GLX extension");

  /*** (3) find an appropriate visual ***/

  /* find an OpenGL-capable RGB visual with depth buffer */
  vi = glXChooseVisual(dpy_, DefaultScreen(dpy_), dblBuf);
  if (vi == NULL)
  {
    vi = glXChooseVisual(dpy_, DefaultScreen(dpy_), snglBuf);
    if (vi == NULL) fatalError("no RGB visual with depth buffer");
    doubleBuffer_ = GL_FALSE;
  }
  //if(vi->class != TrueColor)
  //  fatalError("TrueColor visual required for this program");

  /*** (4) create an OpenGL rendering context  ***/

  /* create an OpenGL rendering context */
 // cx_ = glXCreateContext(dpy_, vi, /* no shared dlists */ None,                        /* direct rendering if possible */ GL_TRUE);


  /*** (5) create an X window with the selected visual ***/

  /* create an X colormap since probably not using default visual */
  cmap = XCreateColormap(dpy_, RootWindow(dpy_, vi->screen), vi->visual, AllocNone);
  swa.colormap = cmap;
  swa.border_pixel = 0;
  swa.event_mask = KeyPressMask    | ExposureMask
                 | ButtonPressMask | StructureNotifyMask | FocusChangeMask;

  win_ = XCreateWindow(dpy_, RootWindow(dpy_, vi->screen), 0, 0,
                      width_, height_, 0, vi->depth, InputOutput, vi->visual,
                      CWBorderPixel | CWColormap | CWEventMask, &swa);
  XSetStandardProperties(dpy_, win_, "main", "main", None,
                         0, 0, NULL);

PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

	int attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		0};

	cx_ = glXCreateContextAttribsARB(dpy_, *fbc, 0, true, attribs);

	//glXMakeCurrent (dpy, win, ctx);

  /*** (6) bind the rendering context to the window ***/

  glXMakeCurrent(dpy_, win_, cx_);

  /*** (7) request the X window to be displayed on the screen ***/

  XMapWindow(dpy_, win_);

  /*** (8) configure the OpenGL context for rendering ***/

  glEnable(GL_DEPTH_TEST); /* enable depth buffering */
  glDepthFunc(GL_LESS);    /* pedantic, GL_LESS is the default */
  glClearDepth(1.0);       /* pedantic, 1.0 is the default */

  /* frame buffer clears should be to black */
  glClearColor(0.0, 0.0, 0.0, 0.0);

  /* set up projection transform */

  /* establish initial viewport */
  /* pedantic, full window size is default viewport */

	isFullscreen_ = false;
	Resize(width,height,wm);

fprintf(stderr, "succers: %d\n", width);
	return true;
}

bool CWindowUnix::Resize(int width, int height, axelynx::WindowMode wm)
{

	assert(width > 0);
	assert(height > 0);


	// устанавливаем вьюпорт на все окно
	glViewport(0, 0, width, height);



	return true;
}

bool CWindowUnix::Free()
{


	return true;
}

bool CWindowUnix::SetText(const wchar_t *unicodetext)
{
    XTextProperty title;

    const char *buff = axelynx::utils::WideCharToMultiByte(unicodetext);

    XStringListToTextProperty((char**)&buff, sizeof(char), &title );
    XSetWMName(dpy_,win_,&title);

    delete[] buff;
	return 0;
}

bool CWindowUnix::ReInit(int width, int height, int bpp, axelynx::WindowMode wm, int samples)
{
	Free();
	return Init(width, height, bpp, wm, samples);
}

bool CWindowUnix::Sync()
{
//	Sleep(0);
  XEvent               event;
    if(XPending(dpy_))
    {
      XNextEvent(dpy_, &event);
      switch (event.type)
      {
        case KeyPress:
        {
          KeySym     keysym;
          XKeyEvent *kevent;
          char       buffer[1];
          /* It is necessary to convert the keycode to a
           * keysym before checking if it is an escape */
          kevent = (XKeyEvent *) &event;
          if (   (XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) == 1)
              && (keysym == (KeySym)XK_Escape) )
            exit(0);
          break;
        }
        case ButtonPress:
          break;
        case ConfigureNotify:
          glViewport(0, 0, event.xconfigure.width,
                     event.xconfigure.height);
          /* fall through... */
        case Expose:

          break;
        case FocusIn:
            CWindowUnix::Instance()->isActive_ = true;
        case FocusOut:
            CWindowUnix::Instance()->isActive_ = false;
      }
    }

	return true;
}

bool CWindowUnix::Flip()
{
  if (doubleBuffer_)
    glXSwapBuffers(dpy_, win_);/* buffer swap does implicit glFlush */
  else
    glFlush();  /* explicit flush for single buffered case */

	return true;
}

CWindowUnix * CWindowUnix::Instance()
{
	if(!instance_)
		instance_ = new CWindowUnix();

	return instance_;
}

#endif
