/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     main file of the wxWindows/QuakeII sample
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

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// wxQuake2Window declaration
#include "wxquake2.h"

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

class Quake2DemoApp : public wxApp
{
public:
    virtual bool OnInit();
};

class Quake2DemoFrame : public wxFrame
{
public:
    // ctor(s)
    Quake2DemoFrame();

    // event handlers
    void OnToggleShow(wxCommandEvent& event);
    void OnToggleShowFullScreen(wxCommandEvent& event);
    void OnSetMode(wxCommandEvent& event);
    void OnQuakeCommand(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    wxQuake2Window *m_quake;

    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Quake2Demo_EnterCommand = 200,
    Quake2Demo_Show,
    Quake2Demo_ShowFullScreen,
    Quake2Demo_Quit,

    Quake2Demo_Mode0 = 300,
    Quake2Demo_Mode1,
    Quake2Demo_Mode2,
    Quake2Demo_Mode3,
    Quake2Demo_Mode4,
    Quake2Demo_ModeMax,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Quake2Demo_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(Quake2DemoFrame, wxFrame)
    EVT_MENU(Quake2Demo_EnterCommand,  Quake2DemoFrame::OnQuakeCommand)
    EVT_MENU(Quake2Demo_Show,  Quake2DemoFrame::OnToggleShow)
    EVT_MENU(Quake2Demo_ShowFullScreen, Quake2DemoFrame::OnToggleShowFullScreen)
    EVT_MENU(Quake2Demo_Quit,  Quake2DemoFrame::OnQuit)
    EVT_MENU(Quake2Demo_About, Quake2DemoFrame::OnAbout)

    EVT_MENU_RANGE(Quake2Demo_Mode0, Quake2Demo_ModeMax - 1,
                   Quake2DemoFrame::OnSetMode)
END_EVENT_TABLE()

IMPLEMENT_APP(Quake2DemoApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool Quake2DemoApp::OnInit()
{
    // create the main application window
    Quake2DemoFrame *frame = new Quake2DemoFrame;

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(TRUE);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
Quake2DemoFrame::Quake2DemoFrame()
               : wxFrame(NULL, -1, _T("wxWindows/Quake II Demo"))
{
    m_quake = NULL;

    // set the frame icon
    SetIcon(wxICON(wxquake));

    // create a menu bar
    wxMenu *menuFile = new wxMenu;
    menuFile->AppendCheckItem(Quake2Demo_Show, _T("&Show/hide Quake\tCtrl-S"),
                              _T("Toggle Quake window on/off"));
    menuFile->AppendSeparator();
    menuFile->Append(Quake2Demo_EnterCommand, _T("&Quake command...\tCtrl-Q"),
                     _T("Enter a Quake II console command"));
    menuFile->AppendSeparator();
    menuFile->Append(Quake2Demo_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    // modes menu
    wxMenu *menuModes = new wxMenu;
    menuModes->AppendRadioItem(Quake2Demo_Mode0, _T("Mode &0 (320x240)"));
    menuModes->AppendRadioItem(Quake2Demo_Mode1, _T("Mode &1 (400x300)"));
    menuModes->AppendRadioItem(Quake2Demo_Mode2, _T("Mode &2 (512x384)"));
    menuModes->AppendRadioItem(Quake2Demo_Mode3, _T("Mode &3 (640x480)"));
    menuModes->AppendRadioItem(Quake2Demo_Mode4, _T("Mode &4 (800x600)"));
    menuModes->AppendSeparator();
    menuModes->AppendCheckItem(Quake2Demo_ShowFullScreen,
                               _T("&Full screen\tCtrl-F"),
                               _T("Toggle Quake between window and screen mode"));

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Quake2Demo_About, _T("&About...\tF1"), _T("Show about dialog"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("&File"));
    menuBar->Append(menuModes, _T("&Mode"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
    menuBar->Check(Quake2Demo_Show, TRUE);
    menuBar->Check(Quake2Demo_Mode3, TRUE);

    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to wxWindows Quake II Demo!"));

    m_quake = new wxQuake2Window(this, -1, wxDefaultPosition,
                                 wxQuake2Window::MODE_640_480);
    if ( !m_quake->IsOk() )
    {
        wxLogError(_T("Failed to initialize Quake II engine."));
        delete m_quake;
        m_quake = NULL;
    }
    else
    {
        SetClientSize(m_quake->GetBestSize());
    }
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

void Quake2DemoFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void Quake2DemoFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_T("Written by Vadim Zeitlin"),
                 _T("wxWindows/Quake II Demo"),
                 wxOK | wxICON_INFORMATION,
                 this);
}

void Quake2DemoFrame::OnToggleShow(wxCommandEvent& event)
{
    if ( !m_quake )
        return;

    m_quake->Show(event.IsChecked());
}

void Quake2DemoFrame::OnToggleShowFullScreen(wxCommandEvent& WXUNUSED(event))
{
    m_quake->ToggleFullScreen();
}

void Quake2DemoFrame::OnSetMode(wxCommandEvent& event)
{
    if ( !m_quake )
    {
        wxLogError(_T("Can't change Quake video mode: engine not running."));
        return;
    }

    m_quake->SetVideoMode((wxQuake2Window::VideoMode)
                            (event.GetId() - Quake2Demo_Mode0));
    SetClientSize(m_quake->GetBestSize());
}

void Quake2DemoFrame::OnQuakeCommand(wxCommandEvent& WXUNUSED(event))
{
    if ( !m_quake )
    {
        wxLogError(_T("Can't execute Quake commands: engine not running."));
        return;
    }

    static wxString s_cmd;

    s_cmd = wxGetTextFromUser(_T("Enter Quake II console command: "),
                              _T("wxWindows/Quake II Demo"),
                              s_cmd,
                              this);
    if ( s_cmd.empty() )
        return;

    m_quake->ExecCommand(s_cmd);

    wxLogStatus(this, _T("Sent \"%s\" to Quake console."), s_cmd.c_str());
}

