// OctProj.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "OctProj.h"
#include "LoadTexture.h"

//#include <strsafe.h>
#include <atlstr.h>
#include <cstringt.h>
#include <math.h>
#include <assert.h>
#include <atlstr.h>

#define MAX_LOADSTRING 100

//static HGLRC hRC;
//static HDC hDC;
//BOOL   keys [256];
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name


///////////////////////////////////////////////////////////////////
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glaux.h"

HWND    hWnd;
HDC     hDC;
HGLRC   hRC;
const int OBJECTSCOUNT = 12;
CLoadTexture texture (OBJECTSCOUNT);
unsigned int uGlCompileList[OBJECTSCOUNT];
unsigned int uGlRollList[3];
GLuint gluFontList;

GLfloat glfyCoord[3] = {0, 0, 0};
GLfloat glfSpeed[3] = {1.0f, 1.0f, 1.0f};
GLfloat glfSpeed2 (1.0f);
GLfloat glfSpeed3 (1.0f);
int nRandVal[3]; 

void LoadGeometry ();
GLvoid BuildFont (GLvoid);
GLvoid DeleteFont(GLvoid);
GLvoid glPrint (GLuint font, const wchar_t *fmt, ...);
GLvoid glPrint(GLuint font, const char *fmt, ...);

// Set up pixel format for graphics initialization
void SetupPixelFormat()
{
    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    int pixelformat;

    ppfd = &pfd;
    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    ppfd->dwLayerMask = PFD_MAIN_PLANE;
    ppfd->iPixelType = PFD_TYPE_COLORINDEX;
    ppfd->cColorBits = 32;
    ppfd->cDepthBits = 32;
    ppfd->cAccumBits = 0;
    ppfd->cStencilBits = 0;

    int nPixFormat = GetPixelFormat (hDC);
    pixelformat = ChoosePixelFormat(hDC, ppfd);
    SetPixelFormat(hDC, pixelformat, ppfd);
}

// Initialize OpenGL graphics
void InitGraphics()
  {
  hDC = GetDC(hWnd);

  SetupPixelFormat();

  hRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hRC);

  glClearColor(0, 0, 0, 0.5);
  glShadeModel (GL_SMOOTH);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);

  glEnable (GL_TEXTURE_2D);
  glDisable (GL_COLOR);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  BuildFont();            // Построить шрифт
  }

// Resize graphics to fit window
void ResizeGraphics()
{
    // Get new window size
    RECT rect;
	int width, height;
	GLfloat aspect;

    GetClientRect(hWnd, &rect);
    width = rect.right;
    height = rect.bottom;
    aspect = (GLfloat)width / height;

    // Adjust graphics to window size
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// Draw frame
void DrawGraphics()
  {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  // Set location in front of camera
  glLoadIdentity();
  glTranslated(0.0, 0.0, -20);

  GLfloat glfMaxSpeed (0.5);

  glPushMatrix ();
    {
    glTranslated (-3.0, 0.0, 0.0);
    glPushMatrix ();
      {
      glTranslated(-4.0, glfyCoord[0] - (nRandVal[0]+1)*4, 0.0f);

      glfyCoord[0] += glfSpeed[0];
      if (glfyCoord[0] > 20)
        {
        glfyCoord[0] = 0;
        glfSpeed[0] /=2.0f;
        if (glfSpeed[0] < glfMaxSpeed)
          glfSpeed[0] = 0.0f;
        }

      glCallList (uGlRollList[0]);
      }
    glPopMatrix ();

    glPushMatrix ();
      {
      glTranslated(0.0, glfyCoord[1] - (nRandVal[1]+1)*4, 0.0f);

      glfyCoord[1] += glfSpeed[1];
      if (glfyCoord[1] > 20)
        {
        glfyCoord[1] = 0;
        if (glfSpeed[0] == 0.0f)
          glfSpeed[1] /=2.0f;
        if (glfSpeed[1] < glfMaxSpeed)
          glfSpeed[1] = 0.0f;
        }

      glCallList (uGlRollList[1]);
      }
    glPopMatrix ();

    glPushMatrix ();
      {
      glTranslated(4.0, glfyCoord[2] - (nRandVal[2]+1)*4, 0.0f);

      glfyCoord[2] += glfSpeed[2];
      if (glfyCoord[2] > 20)
        {
        glfyCoord[2] = 0;
        if (glfSpeed[1] == 0.0f)
          glfSpeed[2] /=2.0f;
        if (glfSpeed[2] < glfMaxSpeed)
          glfSpeed[2] = 0.0f;
        }
      glCallList (uGlRollList[2]);
      }
    glPopMatrix ();
    }
  glPopMatrix ();

  glColor3f(1.0,1.0f,1.0);

glRasterPos2f (4.0, 0.0);//(-1.7,1.15);
//String st = "КООРДИНАТЫ нажатия мыши >>:: 3D(X,Y,Z) |";
//CAtlString atlStr("ТЕСТ");
//atlStr.OemToCharA ();
CStringW sText ("Text example: Пример текста");
CHAR str[255];
CharToOem (sText, str); 
glPrint(gluFontList, str);//sText);
//glEnable(GL_COLOR_MATERIAL);

  SwapBuffers(hDC);
  }

// Handle window events and messages
LONG WINAPI MainWndProc (HWND hWnd, UINT uMsg, WPARAM  wParam, LPARAM  lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
      {
      }
      break;
    case WM_SIZE:
        ResizeGraphics();
        break;

    case WM_CLOSE:
        DeleteFont() ;
        DestroyWindow(hWnd);
        break;
 
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
      switch (wParam)
        {
        case 37:
        case 38:
        case 39:
        case 40:
          break;
        default:
          break;
        }
      break;
    case WM_LBUTTONDOWN:
      for (int n(0); n < 3; n++)
        {
        glfSpeed[n] = 1.0f;
        nRandVal[n] = rand () % 6;
        }
      break;

    // Default event handler
    default: 
        return DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return 1; 
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_OCTPROJ, szWindowClass, MAX_LOADSTRING);

    WNDCLASS wndclass;
    MSG      msg;
 
    // Define the window class
    wndclass.style         = 0;
    wndclass.lpfnWndProc   = (WNDPROC)MainWndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon (hInstance, szTitle);
    wndclass.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wndclass.lpszMenuName  = szTitle;
    wndclass.lpszClassName = szWindowClass;
 
    // Register the window class
    if (!RegisterClass(&wndclass)) return FALSE;
 
    // Create the window
    hWnd = CreateWindow (
            szWindowClass,
            szTitle,
            WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            800,
            600,
            NULL,
            NULL,
            hInstance,
            NULL);
 
    if (!hWnd) return FALSE;

    // Initialize OpenGL
    InitGraphics();

    BuildFont ();
    LoadGeometry ();
    
    // Display the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Event loop
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
        {
            if (!GetMessage(&msg, NULL, 0, 0)) return TRUE;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        DrawGraphics();
    }

    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}

void LoadGeometry ()
  {
  GLfloat fglSeekX = 0;

  std::vector<CString> vsTexName;
  vsTexName.push_back (".\\img\\email.bmp");
  vsTexName.push_back (".\\img\\Favourites.bmp");
  vsTexName.push_back (".\\img\\Floppy Drive.bmp");
  vsTexName.push_back (".\\img\\Floppy.bmp");
  vsTexName.push_back (".\\img\\Hard Drive blue.bmp");
  vsTexName.push_back (".\\img\\Help.bmp");

  for (int n(0); n < vsTexName.size (); n++)
    {
    uGlCompileList[n] = glGenLists (1);

    glNewList (uGlCompileList[n], GL_COMPILE);
      {
      GLuint gluTex;
      texture.LoadTexture (vsTexName[n], gluTex);
      glBindTexture (GL_TEXTURE_2D, gluTex);
      // Draw a square
      glBegin(GL_QUADS);
        {
        //glColor3d(1, 0, 0);
        glTexCoord2f (0.0f, 0.0f); glVertex3d(-2, -2, 0);
        glTexCoord2f (1.0f, 0.0f); glVertex3d(2, -2, 0);
        glTexCoord2f (1.0f, 1.0f); glVertex3d(2, 2, 0);
        glTexCoord2f (0.0f, 1.0f); glVertex3d(-2, 2, 0);
        }
      glEnd();
      }
    glEndList ();
    }

  for (int n(0); n < 3; n++)
    {
    uGlRollList[n] = glGenLists (1);
    glNewList (uGlRollList[n], GL_COMPILE);
      {
      for (int nIndex(0), n(-40); 
           n < 40; 
           n+=4.0f, nIndex++)
        {
        glPushMatrix ();
          {
          glTranslatef (fglSeekX, n, 0.0f);
          glCallList (uGlCompileList[nIndex]);
          }
        glPopMatrix ();

        if (nIndex == 5)
          nIndex = 0;
        }
      }
    glEndList ();
    }
  }

GLvoid BuildFont (GLvoid)  
  {
  HFONT  hFont;            

  gluFontList = glGenLists(224);  

  hFont = CreateFontW(  24,        
        0,        
        0,        
        0,        
        FW_BOLD,      
        FALSE,        
        FALSE,        
        FALSE,        
        UNICODE_NOCHAR,// cDEFAULT_CHARSET, //ANSI_CHARSET,
        OUT_TT_PRECIS,     
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        FF_DONTCARE|DEFAULT_PITCH,
        _T("Times New Roman"));   

  SelectObject (hDC , hFont);
  wglUseFontBitmaps(hDC , 32, 224, gluFontList);
  }

GLvoid DeleteFont(GLvoid)     
{
   //glDeleteLists(base, 223);  
   glDeleteLists(gluFontList, 223);
}

GLvoid glPrint(GLuint font, const wchar_t *fmt, ...)
{
  wchar_t    text[256];    
  va_list    ap;        
  if (fmt == NULL)    
    return;           

  va_start(ap, fmt);  
  vswprintf(text, fmt, ap);
  va_end(ap);                 
  glPushAttrib(GL_LIST_BIT);  
  glListBase(font - 32);      
  glCallLists(wcslen(text), GL_UNSIGNED_BYTE , text);
  glPopAttrib();
}

GLvoid glPrint(GLuint font, const char *fmt, ...)
{
  char    text[256];    
  va_list    ap;        
  if (fmt == NULL)    
    return;           

  va_start(ap, fmt);  
  vsprintf(text, fmt, ap);
  va_end(ap);                 
  glPushAttrib(GL_LIST_BIT);  
  glListBase(font - 32);      
  glCallLists(strlen(text), GL_UNSIGNED_BYTE , text);
  glPopAttrib();
}