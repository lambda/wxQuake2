/////////////////////////////////////////////////////////////////////////////
// Name:        wxquake2.cpp
// Purpose:     implementation of wxQuake2Window class
// Author:      Vadim Zeitlin <vadim@tt-solutions.com>
// Modified by: Eric Kidd <eric.kidd@pobox.com>
// Created:     10.12.02
// RCS-ID:      $Id$
// Copyright:   Copyright 2002 Trustees of Dartmouth College
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include <wx/app.h>                 // for wxWakeUpIdle()
#include <wx/log.h>
#include <wx/toplevel.h>
#include <wx/msw/private.h>         // wxhInstance

#include "wxquake2.h"

extern "C"
{
    // Quake engine functions
    void Qcommon_Init(int argc, char **argv);
    void Qcommon_Frame(int msec);

    void Com_Quit(void);

    void Cbuf_AddText (char *text);
    void Cbuf_Execute (void);

    void Cvar_SetValue(char *var_name, float value);

    enum { qFalse, qTrue } VID_GetModeInfo(int *width, int *height, int mode);

    void AppActivate(BOOL fActive, BOOL minimize);

    void IN_Frame(void);

    int Sys_Milliseconds(void);
    LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Quake variables
    extern HINSTANCE global_hInstance;
    extern HWND cl_hwnd;
    extern int ActiveApp;

    // our variables used from Quake
    extern HWND g_hwndWxQuake;
    extern int g_isRunningWxQuake;
    extern int g_errorInWxQuake;
};

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------

// this is used by wxQuake2Window::Create() to pass HWND to VID_CreateWindow()
HWND g_hwndWxQuake = NULL;

// and this is set to 0 if/when Quake exits
int g_isRunningWxQuake = 1;

// set to 1 if an error occurs inside Quake
int g_errorInWxQuake = 0;

// ----------------------------------------------------------------------------
// Quake2Engine: this class is used to initalize/shutdown quake
// ----------------------------------------------------------------------------

class Quake2Engine
{
public:
    // ctors/dtor
    Quake2Engine(HWND hwnd, int videoMode);
    ~Quake2Engine();

    // accessors
    // ---------

    // has Quake been iitialized successfully?
    bool IsOk() const;

    // is Quake [still] running? (implies IsOk())
    bool IsRunning() const;

    // is Quake running in active state? (implies IsRunning())
    bool IsActive() const;

    // operations
    // ----------

    // run one iteration of the Quake main loop
    void ShowNextFrame();

    // even when Quake shouldn't run, we may want to update its input state to
    // release mouse &c
    void UpdateInput();

private:
    // the time (in ms since Windows start) when the last time was shown
    int m_timeLast;
};

// ============================================================================
// Quake2Engine implementation
// ============================================================================

// ----------------------------------------------------------------------------
// Quake2Engine ctor/dtor
// ----------------------------------------------------------------------------

Quake2Engine::Quake2Engine(HWND hwnd, int videoMode)
{
    // Quake likes adding elements to argv so make sure the array has enough
    // space
    int argc = 0;
    char *argv[256];

    // it also expects the first argument to be this for some reason
    argv[argc++] = "exe";

    // set the mode
    char modeStr[64]; // enough bytes for any integer
    sprintf(modeStr, "%d", videoMode);

    argv[argc++] = "+set";
    argv[argc++] = "sw_mode";
    argv[argc++] = modeStr;

    // don't show full screen initially
    argv[argc++] = "+set";
    argv[argc++] = "vid_fullscreen";
    argv[argc++] = "0";

    // FIXME: for testing only
#if 0
    argv[argc++] = "+map";
    argv[argc++] = "testy";
#endif

    argv[argc++] = NULL;

    // give Quake engine the instance and window to use
    global_hInstance = wxhInstance;
    g_hwndWxQuake = hwnd;

    Qcommon_Init(argc, argv);

    m_timeLast = Sys_Milliseconds();
}

Quake2Engine::~Quake2Engine()
{
    Com_Quit();
}

// ----------------------------------------------------------------------------
// Quake2Engine accessors
// ----------------------------------------------------------------------------

bool Quake2Engine::IsOk() const
{
    return g_errorInWxQuake == 0;
}

bool Quake2Engine::IsRunning() const
{
    return IsOk() && (g_isRunningWxQuake != 0);
}

bool Quake2Engine::IsActive() const
{
    return IsRunning() && ActiveApp;
}

// ----------------------------------------------------------------------------
// Quake2Engine operations
// ----------------------------------------------------------------------------

void Quake2Engine::ShowNextFrame()
{
    int timeCur = Sys_Milliseconds();

    Qcommon_Frame(timeCur - m_timeLast);

    m_timeLast = timeCur;
}

void Quake2Engine::UpdateInput()
{
    IN_Frame();
}

// ============================================================================
// wxQuake2Window implementation
// ============================================================================

BEGIN_EVENT_TABLE(wxQuake2Window, wxWindow)
    EVT_KILL_FOCUS(wxQuake2Window::OnKillFocus)

    EVT_IDLE(wxQuake2Window::OnIdle)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// wxQuake2Window ctors/dtor
// ----------------------------------------------------------------------------

void wxQuake2Window::Init()
{
    m_engine = NULL;
    m_nSuspend = 0;

    m_restoreSize = FALSE;
	m_isIconized = FALSE;
    m_isFullScreen = FALSE;

    SetBackgroundColour(*wxBLACK);
}

bool
wxQuake2Window::Create(wxWindow *parent,
                       int id,
                       const wxPoint& pos,
                       VideoMode mode)
{
    // create the window
    if ( !wxWindow::Create(parent,
                           id,
                           pos,
                           wxDefaultSize,
                           wxBORDER_NONE) )
    {
        return FALSE;
    }

    // initialize the engine
    if ( m_engine )
    {
        wxLogError(_T("Only one Quake II window can be opened currently."));

        return FALSE;
    }

    m_engine = new Quake2Engine(GetHwnd(), mode);
    if ( !m_engine->IsOk() )
    {
        delete m_engine;
        m_engine = NULL;
        return FALSE;
    }

    // set the size to be equal to the full size of the Quake window
    AdjustSize(mode);

	// FIXME - This doesn't seem to get called reliably, which means
	// there's probably something wrong with our processing of
	// WM_ACTIVATE events.
	AppActivate(TRUE, FALSE);

    return TRUE;
}

wxQuake2Window::~wxQuake2Window()
{
    delete m_engine;
}

// ----------------------------------------------------------------------------
// wxQuake2Window accessors
// ----------------------------------------------------------------------------

bool wxQuake2Window::IsOk() const
{
    return m_engine != NULL;
}

bool wxQuake2Window::IsSuspended() const
{
    return m_nSuspend != 0;
}

bool wxQuake2Window::IsFullScreen() const
{
    // TODO: this will get out of sync with the real state if Alt-Enter is
    //       pressed inside Quake; however it probably would be better to
    //       disallow toggling the mode from Quake rather than fixing it here
    //       as simply using Cvar_Get() isn't enough: we'd also have to adjust
    //       the parent frame size automatically...
    return m_isFullScreen;
}

wxTopLevelWindow *wxQuake2Window::GetTopLevelParent() const
{
    wxWindow *parent = GetParent();
    while ( parent && !parent->IsTopLevel() )
        parent = parent->GetParent();

    return wxDynamicCast(parent, wxTopLevelWindow);
}

// ----------------------------------------------------------------------------
// wxQuake2Window operations
// ----------------------------------------------------------------------------

void wxQuake2Window::ExecCommand(const wxString& cmd)
{
    wxCHECK_RET( m_engine->IsRunning(),
				 _T("can't exec commands without engine running") );

    Cbuf_AddText(const_cast<char *>(cmd.mb_str()));
    Cbuf_Execute();
}

void wxQuake2Window::Suspend()
{
    ++m_nSuspend;
}

void wxQuake2Window::Resume()
{
    wxCHECK_RET( m_nSuspend, _T("Resume() without matching Suspend()?") );

    if ( !--m_nSuspend )
    {
        // kick the event loop back into life to start getting idle events
        wxWakeUpIdle();
    }
}

void wxQuake2Window::ShowFullScreen(bool fullScreen)
{
    // remember our new state
    m_isFullScreen = fullScreen;

    // resize the parent frame
    wxTopLevelWindow *parentTop = GetTopLevelParent();
    wxCHECK_RET(parentTop, _T("wxQuake2Window should be a child of a wxTLW"));

    parentTop->ShowFullScreen(fullScreen);
    if ( !fullScreen )
    {
        // although ShowFullScreen() restores our size properly, unloaing DDRAW
        // does it once again and so we'll have to restore it a bit later
        m_restoreSize = TRUE;
        m_rectParentOld = parentTop->GetRect();
    }

    // finally tell the Quake about the mode change too (it will take effect
    // during the next frame display and we can't force it now because if we're
    // switching from full screen mode we're called right now from DDRAW code
    // and unloaing DDRAW DLL (done by Quake) would lead to the crash!)
    Cvar_SetValue("vid_fullscreen", fullScreen ? 1.0 : 0.0);
}

void wxQuake2Window::ToggleFullScreen()
{
    ShowFullScreen(!IsFullScreen());
}

void wxQuake2Window::AdjustSize(VideoMode mode)
{
    int w, h;
    if ( !VID_GetModeInfo(&w, &h, mode) )
    {
        // invalid size value?
        wxLogWarning(_T("Failed to retrieve the Quake window size for mode %d."),
                     mode);

        // use some arbitrary defaults...
        w = 640;
        h = 480;
    }

    SetClientSize(w, h);
}

void wxQuake2Window::SetVideoMode(VideoMode mode)
{
    ExecCommand(wxString::Format(_T("set sw_mode %d"), mode));

    if ( !IsFullScreen() )
    {
        AdjustSize(mode);
    }
}

bool wxQuake2Window::Show(bool show)
{
    if ( !wxWindow::Show(show) )
    {
        // nothing changed
        return FALSE;
    }

    // it doesn't make sense to leave Quake running when we're not shown so
    // suspend it when hiding the window -- and resume it back when showing it
    if ( show )
        Resume();
    else
        Suspend();

    if ( show )
        SetFocus();

    return TRUE;
}

void wxQuake2Window::SetFocus()
{
    // it is necessary to do this here and not in OnFocus() because we won't
    // get the latter (WM_SETFOCUS message goes directly to Quake...)
    AppActivate(TRUE, FALSE);

    wxWindow::SetFocus();
}

// ----------------------------------------------------------------------------
// wxQuake2Window event handlers
// ----------------------------------------------------------------------------

void wxQuake2Window::OnIdle(wxIdleEvent& event)
{
    if ( !IsOk() )
        return;

    // check if we weren't minimized/restored
    wxTopLevelWindow *parentTop = GetTopLevelParent();
    if ( parentTop && parentTop->IsIconized() != m_isIconized )
    {
        m_isIconized = !m_isIconized;

        AppActivate(TRUE, m_isIconized);
    }

    // let Quake run now
    if ( m_engine->IsActive() && !IsSuspended() )
    {
        m_engine->ShowNextFrame();

        // we *are* going to consume 100% of the CPU like this, of course, but
        // then Quake by itself does it anyhow...
        event.RequestMore();
    }

    // after (possibly) changing the video mode from inside ShowNextFrame(),
    // restore our window size
    if ( m_restoreSize )
    {
        m_restoreSize = FALSE;

        if ( parentTop )
        {
            parentTop->SetSize(m_rectParentOld);
        }
    }
}

void wxQuake2Window::OnKillFocus(wxFocusEvent& event)
{
    if ( !m_isIconized )
    {
        // we lost focus, deactivate Quake
        AppActivate(FALSE, FALSE);

        if ( m_engine )
        {
            // otherwise mouse would remain captured
            m_engine->UpdateInput();
        }
    }

    event.Skip();
}

long
wxQuake2Window::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    if ( m_engine && m_engine->IsRunning() )
    {
        // as Quake window proc doesn't get WM_CREATE, let it know about its
        // window which it sets there
        if ( !cl_hwnd )
        {
            cl_hwnd = GetHwnd();
        }

        // Note that we could intercept some events here and/or modify Quake
        // MainWndProc() to call us back instead of calling DefWindowProc().
		// For now, we only intercept WM_ERASEBKGND, which wxWindows can
		// handle in a far more intelligent fashion than Quake 2.
		if ( nMsg != WM_ERASEBKGND )
			return MainWndProc(GetHwnd(), nMsg,
						       (WPARAM)wParam, (LPARAM)lParam);
    }

    return wxWindow::MSWWindowProc(nMsg, wParam, lParam);
}
