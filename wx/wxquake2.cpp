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

// If we try to include the Quake 2 headers directly, MSVC++ gets
// confused.  We should probably patch the Quake 2 headers.
extern "C"
{
    // Quake engine functions
    void Qcommon_Init(int argc, char **argv);
    void Qcommon_Frame(int msec);

    void Com_Quit(void);

	char *CopyString (char *in);

    void Cbuf_AddText (char *text);
    void Cbuf_Execute (void);

	typedef void (*xcommand_t) (void);
	typedef enum { qFalse, qTrue } qboolean;

	qboolean Cmd_Exists (char *cmd_name);
	void Cmd_AddCommand (char *cmd_name, xcommand_t function);
	int Cmd_Argc (void);
	char *Cmd_Argv (int arg);

	void SCR_BeginLoadingInBackground();
	void SCR_EndLoadingInBackground();
	qboolean SCR_IsLoadingInBackground();

	void Com_Printf (char *fmt, ...);

    qboolean VID_GetModeInfo(int *width, int *height, int mode);

    void AppActivate(BOOL fActive, BOOL minimize);

    void IN_Frame(void);

    int Sys_Milliseconds(void);
    LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    typedef struct cvar_s
    {
		char		*name;
		char		*string;
		char		*latched_string;	// for CVAR_LATCH vars
		int			flags;
		qboolean	modified;	// set each time the cvar is changed
		float		value;
		struct cvar_s *next;
    } cvar_t;

    cvar_t *Cvar_Get (char *var_name, char *value, int flags);
    cvar_t *Cvar_Set (char *var_name, char *value);
    void	Cvar_SetValue (char *var_name, float value);
    float	Cvar_VariableValue (char *var_name);

    // Quake variables
    extern HINSTANCE global_hInstance;
    extern HWND cl_hwnd;
    extern int ActiveApp;

    // our variables used from Quake
    extern HWND g_hwndWxQuake;
    extern int g_isRunningWxQuake;
    extern int g_errorInWxQuake;

#ifdef IML_Q2_EXTENSIONS
    extern void CL_SetBinMsgHandler(void (*handler)(binmsg_byte *, size_t));
#endif // IML_Q2_EXTENSIONS
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
    Quake2Engine(HWND hwnd, int videoMode,
				 const wxString &basedir,
				 const wxString &game);
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

Quake2Engine::Quake2Engine(HWND hwnd, int videoMode,
						   const wxString &basedir,
						   const wxString &game)
{
    // Quake likes adding elements to argv so make sure the array has enough
    // space
    int argc = 0;
    char *argv[256];
	wxString quoted_basedir, quoted_game;

    // Quote the basedir and game, in case there are spaces.
	quoted_basedir = "\"" + basedir + "\"";
	quoted_game = "\"" + game + "\"";

    // it also expects the first argument to be this for some reason
    argv[argc++] = "exe";

	// if we've been supplied with a basedir, pass it to Quake.
	if (basedir != "")
	{
		argv[argc++] = "+set";
		argv[argc++] = "basedir";
		argv[argc++] = const_cast<char *>(quoted_basedir.mb_str());
	}

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

    // if a game mod was specified, load it
	if (game != "")
	{
		argv[argc++] = "+game";
		argv[argc++] = const_cast<char *>(quoted_game.mb_str());
	}
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

wxQuake2Window *wxQuake2Window::s_Instance = NULL;

void wxQuake2Window::Init()
{
	wxASSERT(s_Instance == NULL);
	s_Instance = this;

    m_engine = NULL;
    m_nSuspend = 0;

    m_restoreSize = FALSE;
	m_isIconized = FALSE;
    m_isFullScreen = FALSE;
    m_shouldRunGameInBackground = FALSE;

    SetBackgroundColour(*wxBLACK);
}

bool
wxQuake2Window::Create(wxWindow *parent,
                       int id,
                       const wxPoint& pos,
                       VideoMode mode,
					   const wxString &basedir,
					   const wxString &game)
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

    m_engine = new Quake2Engine(GetHwnd(), mode, basedir, game);
    if ( !m_engine->IsOk() )
    {
        delete m_engine;
        m_engine = NULL;
        return FALSE;
    }

    // set the size to be equal to the full size of the Quake window
    AdjustSize(mode);

	// register a test command, just to show we're here
	RegisterCommand("wxquake2");

    // Start listening to binmsgs.
#ifdef IML_Q2_EXTENSIONS
    CL_SetBinMsgHandler(&DoHandleBinMsg);
#endif // IML_Q2_EXTENSIONS

	// FIXME - This doesn't seem to get called reliably, which means
	// there's probably something wrong with our processing of
	// WM_ACTIVATE events.
	AppActivate(TRUE, FALSE);

    return TRUE;
}

wxQuake2Window::~wxQuake2Window()
{
	s_Instance = NULL;
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

bool wxQuake2Window::ShouldRunGameInBackground()
{
    return m_shouldRunGameInBackground;
}

bool wxQuake2Window::IsLoadingInBackground()
{
	return (SCR_IsLoadingInBackground() == qTrue) ? true : false;
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

    wxString terminated = cmd + "\n";
    Cbuf_AddText(const_cast<char *>(terminated.mb_str()));
    Cbuf_Execute();
}

void wxQuake2Window::ExecBackgroundLoadCommand(const wxString& cmd)
{
	// Handy for loading levels without displaying Quake 2.
	SCR_BeginLoadingInBackground();
	ExecCommand(cmd);
}

void wxQuake2Window::Print(const wxString& message)
{
	::Com_Printf("%s", const_cast<char *>(message.mb_str()));
}

void wxQuake2Window::SetVariable(const wxString& name, const wxString &value)
{
    Cvar_Set(const_cast<char *>(name.mb_str()),
			 const_cast<char *>(value.mb_str()));
}

void wxQuake2Window::GetVariable(const wxString& name, wxString &out_value)
{
    cvar_t *var = Cvar_Get(const_cast<char *>(name.mb_str()), "0", 0);
	wxASSERT(var != NULL && var->string != NULL);
	out_value = var->string;
}

void wxQuake2Window::SetVariable(const wxString& name, float value)
{
    Cvar_SetValue(const_cast<char *>(name.mb_str()), value);
}

void wxQuake2Window::GetVariable(const wxString& name, float &out_value)
{
    out_value = Cvar_VariableValue(const_cast<char *>(name.mb_str()));
}

void wxQuake2Window::Suspend()
{
    if (!m_nSuspend++)
    {
        // Experimental code to release mouse when suspending.
        DeactivateQuake();

#ifdef IML_Q2_EXTENSIONS
        // There are a couple of situations when we may continue running
        // Quake 2 even though it's theoretically suspended.  But even in
        // those situations, we won't need the GUI.
        q2_enable_gui(FALSE);
#endif // IML_Q2_EXTENSIONS
    }
}

void wxQuake2Window::Resume()
{
    wxCHECK_RET( m_nSuspend, _T("Resume() without matching Suspend()?") );

    if ( !--m_nSuspend )
    {
        // kick the event loop back into life to start getting idle events
        wxWakeUpIdle();

#ifdef IML_Q2_EXTENSIONS
        q2_enable_gui(TRUE);
#endif // IML_Q2_EXTENSIONS
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

void wxQuake2Window::RunGameInBackground(bool run)
{
    m_shouldRunGameInBackground = run;
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
// wxQuake2Window callback support
// ----------------------------------------------------------------------------

bool wxQuake2Window::RegisterCommand(const wxString &cmdName)
{
	// Return false if this command is already registered.
	// FIXME - This doesn't match the logic in Cmd_AddCommand,
	// which also considers variables.  This may cause us to leak
	// the specially-allocated string below.
	if (::Cmd_Exists(const_cast<char*>(cmdName.mb_str())))
		return false;

	// Ask Quake 2 to send cmdName to us in the future.  We need to
	// allocate a string on the heap, because Quake 2 requires this string
	// to be permanent.  Quake 2 will not attempt to free this string.
	char *str = CopyString(const_cast<char*>(cmdName.mb_str()));
	::Cmd_AddCommand(str, &DoQuake2Command);
	return true;
}

void wxQuake2Window::HandleCommand()
{
	// To implement a command, override this function, check CommandArgv(0)
	// for the command name, and pass any unknown commands to your
	// superclass.
	wxString command = CommandArgv(0);
	if (command == "wxquake2")
		::Com_Printf("Running under wxQuake2.\n");
	else
		::Com_Printf("Command '%s' registered by wxQuake2 but not "
					 "implemented.\n", CommandArgv(0).mb_str());
}

int wxQuake2Window::CommandArgc(void)
{
	return ::Cmd_Argc();
}

wxString wxQuake2Window::CommandArgv(int arg)
{
	return wxString(::Cmd_Argv(arg));
}

void wxQuake2Window::DoQuake2Command()
{
	// Receive a command from Quake, and dispatch it to our HandleCommand
	// method.
	wxASSERT(s_Instance);
	s_Instance->HandleCommand();
}

void wxQuake2Window::HandleBinMsg(unsigned char *buffer, size_t size)
{
    // Ignore all messages by default.
}

void wxQuake2Window::DoHandleBinMsg(unsigned char *buffer, size_t size)
{
    // Receive a binmsg from Quake and dispatch it.
    wxASSERT(s_Instance);
    s_Instance->HandleBinMsg(buffer, size);
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

    // Let Quake run now.  We used to check IsActive instead of IsRunning,
    // but that caused Quake to stop repainting the window when
    // our application wasn't the front application.
    if ( m_engine->IsRunning() && (!IsSuspended() || IsLoadingInBackground() ||
                                   ShouldRunGameInBackground()) )
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
	DeactivateQuake();
    event.Skip();
}

void wxQuake2Window::DeactivateQuake()
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
}

long
wxQuake2Window::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    if ( m_engine && m_engine->IsRunning() )
    {
        // As Quake window proc doesn't get WM_CREATE, let it know about its
        // window which it sets there.
        if ( !cl_hwnd )
        {
            cl_hwnd = GetHwnd();
        }

		// Experimental code to return focus to the Quake 2 window if it
		// has been lost to another window in the same frame.
		if ( nMsg == WM_LBUTTONDOWN )
			SetFocus();

#ifdef IML_Q2_EXTENSIONS
		// Interactive mouse support: Divert mouse events to wxWindows.
		if (q2_mouse_is_interactive() &&
			(nMsg == WM_LBUTTONDOWN ||
			 nMsg == WM_LBUTTONUP ||
			 nMsg == WM_RBUTTONDOWN ||
			 nMsg == WM_RBUTTONUP ||
			 nMsg == WM_MBUTTONDOWN ||
			 nMsg == WM_MBUTTONUP ||
			 nMsg == WM_MOUSEMOVE))
			return wxWindow::MSWWindowProc(nMsg, wParam, lParam);
#endif // IML_Q2_EXTENSIONS

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
