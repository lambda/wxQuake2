/////////////////////////////////////////////////////////////////////////////
// Name:        wxquake2.h
// Purpose:     declaration of wxQuake2Window class
// Author:      Vadim Zeitlin <vadim@tt-solutions.com>
// Modified by: Eric Kidd <eric.kidd@pobox.com>
// Created:     10.12.02
// RCS-ID:      $Id$
// Copyright:   Copyright 2002 Trustees of Dartmouth College
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXQUAKE2_H_
#define _WXQUAKE2_H_

#include <wx/window.h>

#ifdef IML_Q2_EXTENSIONS
extern "C" {
#include "../client/qoverlay.h"
#include "../game/binmsg.h"
};
#endif // IML_Q2_EXTENSIONS

// setup the DLL declaration macro: WXQ2_EXPORTS must be defined when compiling
// this code as DLL, WXQ2_IMPORTS -- when using it from DLL
#if defined(WXQ2_EXPORTS)
    #define WXQ2_EXPORT WXEXPORT
#elif defined(WXQ2_IMPORTS)
    #define WXQ2_EXPORT WXIMPORT
#else
    #define WXQ2_EXPORT
#endif

class WXDLLEXPORT wxTopLevelWindow;

// ----------------------------------------------------------------------------
// wxQuake2Window: shows Quake screen inside a wxWindow
// ----------------------------------------------------------------------------

class WXQ2_EXPORT wxQuake2Window : public wxWindow
{
public:
    // the valid video modes for the Quake window
    //
    // NB: these constants must be in sync with vid_modes[] in win32/vid_dll.c!
    enum VideoMode
    {
        MODE_320_240,
        MODE_400_300,
        MODE_512_384,
        MODE_640_480,
        MODE_800_600,
        // others omitted for now
    };

    // ctors and dtor
    // --------------

    // default ctor, use Create() later
    wxQuake2Window() { Init(); }

    // use IsOk() to check if the creation succeeded!
    wxQuake2Window(wxWindow *parent,
                   int id = -1,
                   const wxPoint& pos = wxDefaultPosition,
                   VideoMode mode = MODE_640_480,
				   const wxString &basedir = "",
				   const wxString &game = "",
                   const wxString &ref = "soft")
    {
        Init();

        (void)Create(parent, id, pos, mode, basedir, game, ref);
    }

    // creates the widget and initializes the quake engine if necessary
    //
    // do check the return code!
    bool Create(wxWindow *parent,
                int id = -1,
                const wxPoint& pos = wxDefaultPosition,
                VideoMode mode = MODE_640_480,
				const wxString &basedir = "",
				const wxString &game = "",
                const wxString &ref = "soft");

    // dtor will shut down the quake engine if this is the last wxQuake2Window
    // in existence
    virtual ~wxQuake2Window();


    // accessors
    // ---------

    // have we been created successfully?
    bool IsOk() const;

    // are we currently suspended?
    bool IsSuspended() const;

    // are we currently shown in full screen mode?
    bool IsFullScreen() const;

    // are we running the game's audio, networking and physics in the
    // background?
    bool ShouldRunGameInBackground();

    // are we loading a level in the background?
    bool IsLoadingInBackground();

    // operations
    // ----------

    // execute Quake console command non interactively
    void ExecCommand(const wxString& cmd);

    // execute a loading-related command while Quake 2 is in the
    // background--examples include 'map', 'newgame', etc.
    void ExecBackgroundLoadCommand(const wxString& cmd);

    // print a message to the Quake 2 player
    void Print(const wxString& message);

    // accessing Quake 2 variables (These functions may fail.  If
    // they do, they will print an error on the Quake 2 console.
    // Yes, this is completely useless behavior.)
    void SetVariable(const wxString& name, const wxString &value);
    void GetVariable(const wxString& name, wxString &out_value);
    void SetVariable(const wxString& name, float value);
    void GetVariable(const wxString& name, float &out_value);

    // suspend Quake engine -- note that this happens automatically when window
    // is hidden
    void Suspend();

    // resume Quake (called automatically when window is shown)
    void Resume();

    // show the window in full screen mode
    void ShowFullScreen(bool fullScreen = TRUE);

    // toggle between the window and full screen mode
    void ToggleFullScreen();

    // Tell the engine to continute running the game in the background.
    void RunGameInBackground(bool run = TRUE);

    // change the video mode
    void SetVideoMode(VideoMode mode);

    // overloaded base class methods
    virtual void SetFocus();
    virtual bool Show(bool show = TRUE);

protected:
    // Quake 2 commands
    virtual bool RegisterCommand(const wxString &cmdName);
    virtual void HandleCommand();
    int CommandArgc(void);
    wxString CommandArgv(int arg);

    // binmsg dispatching
    virtual void HandleBinMsg(unsigned char *buffer, size_t size);

    // event handlers
    void OnIdle(wxIdleEvent& event);
    void OnKillFocus(wxFocusEvent& event);

    // If Quake isn't iconized, deactivate it and release the mouse.
    void DeactivateQuake();

    // override Windows messages handling
    virtual long MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

private:
    // this is only needed by CommandFunction
    static wxQuake2Window *s_Instance;

    // we pass this callback to Quake 2 when we want to register a
    // console command
    static void DoQuake2Command();

    // we pass this callback to Quake 2 to receive binmsgs
    static void DoHandleBinMsg(unsigned char *buffer, size_t size);

    // common part of all ctors
    void Init();

    // adjust our size to the size of the Quake window in this mode
    void AdjustSize(VideoMode mode);

    // get our parent frame
    wxTopLevelWindow *GetTopLevelParent() const;

    // Pass the values of is_active and is_iconized to Quake 2.
    void SendAppActivate(bool is_active, bool is_iconized);

    // Send the current frame's values for active/iconized to Quake 2.
    void SendAppActivateFrameValues();

    // Pass the values of is_active and is_iconized to Quake 2 if they
    // have changed since the last time we sent them.
    void SendAppActivateFrameValuesIfChanged();

    // Returns true if the current frame is active.
    bool IsFrameActive();

    // Returns true if the current frame is iconized.
    bool IsFrameIconized();

    // the engine stuff
    class Quake2Engine *m_engine;

    // suspend count
    size_t m_nSuspend;

    // true if we're in full screen
    bool m_isFullScreen;

    // true if we want to run the game in the background
    bool m_shouldRunGameInBackground;

    // true if we have to restore our old size after switching from full screen
    bool m_restoreSize;

    // true if m_isActive and m_isIconized have been initialized.
    bool m_isActiveAndIconizedStateInitialized;

    // true if the frame containing us is currently active
    bool m_isActive;

    // true if the frame containing us is currently iconized
    bool m_isIconized;

    // the size to restore (only valid if m_restoreSize == true)
    wxRect m_rectParentOld;


    DECLARE_EVENT_TABLE()
};

#ifdef IML_Q2_EXTENSIONS

class wxQuake2Overlay
{
    DECLARE_NO_COPY_CLASS(wxQuake2Overlay)

    overlay_t *m_Overlay;

public:
    // See qoverlay.h for documentation of the overlay functions.
    wxQuake2Overlay(int format, unsigned char *data,
                    const wxRect &r, int stride)
        : m_Overlay(q2_overlay_new(format, data, r.x, r.y, r.width, r.height,
                                   stride))
    {
    }

    virtual ~wxQuake2Overlay()
    {
        q2_overlay_delete(m_Overlay);
    }

    virtual void Show(bool show)
    {
        q2_overlay_show(m_Overlay, show);
    }

    virtual void MoveTo(const wxPoint &p)
    {
        q2_overlay_move(m_Overlay, p.x, p.y);
    }

    virtual void DirtyRect(const wxRect &r)
    {
        q2_overlay_dirty_rect(m_Overlay, r.x, r.y, r.width, r.height);
    }
};

#endif // IML_Q2_EXTENSIONS

#endif // _WXQUAKE2_H_
