wxQuake2 - Support for embedding Quake2 in a wxWindows application
Copyright 2002 Trustees of Dartmouth College

Author:      Vadim Zeitlin <vadim@tt-solutions.com>
Maintainer:  Eric Kidd <eric.kidd@pobox.com>
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
