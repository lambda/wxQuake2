wxQuake2 - Support for embedding Quake2 in a wxWindows application
Copyright 2002-2008 Trustees of Dartmouth College

Author:      id Software, Vadim Zeitlin <vadim@tt-solutions.com>
Maintainer:  Eric Kidd <eric.m.kidd@dartmouth.edu>
License:     wxWindows License[*]

[*] The wxWindows license only applies to the new wxQuake2 code, not to the
original Quake 2 code included in this distribution.  Quake 2 itself is
available under a variety of licenses, including the GNU General Public
License and a commercial use license.  If you haven't signed a contract
with Id Software, your application will fall under the GPL.  Talk to your
lawyers for a description of what this means.


Building wxQuake 2
------------------

Set the environment variable $WXWIN to point to your local copy of
wxWindows.  Consult your Windows documentation for instructions on setting
environment variables.

To build Quake2, you'll need to install Microsoft Visual C++ version
6.0 from a properly-licensed MSDN CD.  Once that is done, you'll need
to install MASM (the Microsoft macro assembler) and copy two files
into your VC98/Bin directory:

  ML.EXE
  ML.ERR


IML Quake 2 Extensions
----------------------

In addition to the wxWindows support, this Quake 2 build contains a small
number of extra, optional extensions which may be helpful for writing Quake
2 games.  These extensions are enabled when IML_Q2_EXTENSIONS is defined.

The IML extensions currently include:

  - A "trigger" for running console commands from within a level.

If the IML extensions are activated, the game_imports_t and game_exports_t
APIs will use non-standard interfaces and WILL NOT WORK with regular
Quake 2 DLLs.

The IML_Q2_EXTENSIONS are fully implemented for client/, server/, and
game/, but not necessarily for ctf/.


Hacking the Master Source
-------------------------

All wxWindows-specific code should be wrapped with:

  #ifdef __WXWINDOWS__
  ... code goes here ...
  #endif // __WXWINDOWS__

All other extensions should be wrapped with:

  #ifdef IML_Q2_EXTENSIONS
  ... code goes here ...
  #endif // IML_Q2_EXTENSIONS

All IML_Q2_EXTENSIONS should be properly supported by the game/
subdirectory.


Making Your Own Game
--------------------

Make a copy of the game/ subdirectory, import it into a separate CVS
repository, and hack on it there.  If you need to add a new
IML_Q2_EXTENSION, then add it on the master tree (as specified above),
export a diff, and merge it into your private game repository.  Yes, this
is a pain, but it's the only way to keep wxQuake2 independent of the games
you hack.
