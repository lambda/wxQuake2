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
                   VideoMode mode = MODE_640_480)
    {
        Init();

        (void)Create(parent, id, pos, mode);
    }

    // creates the widget and initializes the quake engine if necessary
    //
    // do check the return code!
    bool Create(wxWindow *parent,
                int id = -1,
                const wxPoint& pos = wxDefaultPosition,
                VideoMode mode = MODE_640_480);

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


    // operations
    // ----------

    // execute Quake console command non interactively
    void ExecCommand(const wxString& cmd);

    // suspend Quake engine -- note that this happens automatically when window
    // is hidden
    void Suspend();

    // resume Quake (called automatically when window is shown)
    void Resume();

    // show the window in full screen mode
    void ShowFullScreen(bool fullScreen = TRUE);

    // toggle between the window and full screen mode
    void ToggleFullScreen();

    // change the video mode
    void SetVideoMode(VideoMode mode);


    // overloaded base class methods
    virtual void SetFocus();
    virtual bool Show(bool show = TRUE);

protected:
    // event handlers
    void OnIdle(wxIdleEvent& event);
    void OnKillFocus(wxFocusEvent& event);

    // override Windows messages handling
    virtual long MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

private:
    // common part of all ctors
    void Init();

    // adjust our size to the size of the Quake window in this mode
    void AdjustSize(VideoMode mode);

    // get our parent frame
    wxTopLevelWindow *GetTopLevelParent() const;


    // the engine stuff
    class Quake2Engine *m_engine;

    // suspend count
    size_t m_nSuspend;

    // true if we're in full screen
    bool m_isFullScreen;

    // true if we have to restore our old size after switching from full screen
    bool m_restoreSize;

    // true if the frame containing us is currently iconized
    bool m_isIconized;

    // the size to restore (only valid if m_restoreSize == true)
    wxRect m_rectParentOld;


    DECLARE_EVENT_TABLE()
};

#endif // _WXQUAKE2_H_
