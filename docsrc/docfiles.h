/// \defgroup client_grp client: Client Files
///
/// The client is responsible for interacting with the user, loading video
/// drivers, and communicating with the server.  The client relies on
/// qcommon and a specific platform implementation.

//@{

/*! \file client/asm_i386.h
**  \brief Member variable offsets so x86 assembly can access structs.
*/

/*! \file client/cdaudio.h
**  \brief Platform-independent interface to platform-specific CD audio code.
*/

/*! \file client/cl_cin.c
**  \brief Support for cinematics (movies) and PCX graphics.
*/

/*! \file client/cl_ents.c
**  \brief Client-side support for entities, projectiles and sex-specific
**  models.
*/

/*! \file client/cl_fx.c
**  \brief Entity effects including dynamic lights, particles, beams, etc.
*/

/*! \file client/cl_input.c
**
**  \brief Mapping of keyboard input to console commands, particularly
**  togglable keys.
*/

/*! \file client/cl_inv.c
**  \brief Client inventory screen.
*/

/*! \file client/cl_main.c
**  \brief Core client code, including setup, networking, command dispatch,
**  etc.
*/

/*! \file client/cl_newfx.c
**  \brief More trails, beams, explosions and particles.
*/

/*! \file client/cl_parse.c
**  \brief More code for communicating with the server, including the
**  packet-type dispatcher.
*/

/*! \file client/cl_pred.c
**
**  \brief Client-side movement prediction.
**
**  Appears to assume that all objects will remain in motion until they hit
**  something.  Used to make the client feel responsive even when the
**  network isn't.
*/

/*! \file client/cl_scrn.c
**  \brief Builds the actual screen image for each frame.
*/

/*! \file client/cl_tent.c
**
**  \brief More support for particles, beams and explosions, including
**  network packet parsing.
*/

/*! \file client/cl_view.c
**
**  \brief Miscellaneous support code for drawing the 3D portion of the
**  screen.
*/

/*! \file client/client.h
**  \brief Common header for many client source files.
*/

/*! \file client/console.c
**
**  \brief Implementation of interactive console.  Type "`" while playing
**  to see it.
*/

/*! \file client/console.h
**  \brief Interface to interactive console.  Type "`" while playing
**  to see it.
*/

/*! \file client/input.h
**  \brief Platform-independent interface to platform-specific input code.
*/

/*! \file client/keys.c
**  \brief Key names, bindings, messages, etc.
*/

/*! \file client/keys.h
**  \brief Key names, bindings, messages, etc.
*/

/*! \file client/menu.c
**  \brief The main game menu (mostly game-specific).
*/

/*! \file client/qmenu.c
**  \brief General menu code.
*/

/*! \file client/qmenu.h
**  \brief General menu code.
*/

/*! \file client/qoverlay.c
**
**  \brief [IML] Support for compositing overlays over an embedded Quake 2
**  widget.
*/

/*! \file client/qoverlay.h
**
**  \brief [IML] Support for compositing overlays over an embedded Quake 2
**  widget.
*/

/*! \file client/ref.h
**  \brief Interface between display driver and client.
*/

/*! \file client/screen.h
**  \brief Interface to various screen-related functions, including cinematics.
*/

/*! \file client/snd_dma.c
**  \brief Main sound-system code.
*/

/*! \file client/snd_loc.h
**  \brief Internal header for use only by sound subsystem.
*/

/*! \file client/snd_mem.c
**  \brief Appears to be WAV-loading or WAV-parsing code.
*/

/*! \file client/snd_mix.c
**  \brief Mixes sounds for snd_dma.c.
*/

/*! \file client/sound.h
**  \brief Public interface to sound subsystem.
*/

/*! \file client/vid.h
**  \brief Portable interface to non-portable portion of sound system.
*/

/*! \file client/x86.c
**  \brief x86 assembly for high-precision timers.
*/

//@}


/// \defgroup game_grp game: Game DLL Files
///
/// The game DLL implements the AI and game logic for a specific Quake
/// 2-based game.

//@{

/*! \file game/binmsg.c
**
**  \brief Binary RPC messages.
**
**  This is a custom add-on which allows the game DLL to send messages to
**  client-side scripting languages.
*/

/*! \file game/binmsg.h
**
**  \brief Binary RPC messages.
**
**  This is a custom add-on which allows the game DLL to send messages to
**  client-side scripting languages.
*/

/*! \file game/g_ai.c
**
**  \brief Core AI code shared by all monsters.
*/

/*! \file game/g_chase.c
**  \brief Chase camera support.
*/

/*! \file game/g_cmds.c
**  \brief Console commands implemented by the game.
*/

/*! \file game/g_combat.c
**  \brief Calculation of weapon damage, and some other combat support.
*/

/*! \file game/g_func.c
**  \brief <tt>func_*</tt> entities, including platforms, doors, etc.
*/

/*! \file game/g_items.c
**
**  \brief Weapons and powerup support.
**
**  Includes table defining weapons and powerups.
*/

/*! \file game/g_local.h
**
**  \brief Internal game DLL APIs.
*/

/*! \file game/g_main.c
**  \brief Main game code, including initialization, frame processing, etc.
*/

/*! \file game/g_misc.c
**
**  \brief A real grab-bag of cruft.
**
**  Includes area portals, gibs (fragments of monsters and players),
**  decapitation, debris, explosions, paths (simple AI patrol routes),
**  lights, explosive walls, barrels, teleporters, easter eggs, one-shot
**  special entities, and even less explicable stuff.
*/

/*! \file game/g_monster.c
**  \brief Non-AI code shared by all monsters.
*/

/*! \file game/g_phys.c
**
**  \brief Simple physics model.
**
**  Includes clipped movement, gravity, pushing, various physics models for
**  different types of objects.
*/

/*! \file game/g_save.c
**
**  \brief Support for parsing entity strings, saving games, and loading
**  games.
**
**  If you add anything to edict_s, you'll need to make changes here, too.
*/

/*! \file game/g_spawn.c
**
**  \brief Support for spawning new entities, parsing edict_s (in
**  conjuction with g_save.c), assigning players to teams, and specifying
**  the layout of client status displays.
*/

/*! \file game/g_svcmds.c
**
**  \brief IP address filtering for banning players.
**
**  I have no idea why this is in the game DLL and not the server.
*/

/*! \file game/g_target.c
**
**  \brief "Targetted" entities can be controlled by a trigger.
*/

/*! \file game/g_trigger.c
**  \brief Triggers control targetted entities.
*/

/*! \file game/g_turret.c
**  \brief Some kind of gun turret support.
*/

/*! \file game/g_utils.c
**
**  \brief Miscellaneous game functions.
**
**  Includes geometric functions, searching of entity fields, finding all
**  objects within a specified radius, trigger/target support functions,
**  entity spawning and destruction, touching of triggers and objects, and
**  KillBox (which probably implements telefragging).
*/

/*! \file game/g_weapon.c
**  \brief Weapon firing code.
*/

/*! \file game/game.h
**  \brief APIs used to communicate between the server and the game.
*/

/*! \file game/m_actor.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_berserk.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_boss2.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_boss3.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_boss31.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_boss32.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_brain.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_chick.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_flash.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_flipper.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_float.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_flyer.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_gladiator.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_gunner.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_hover.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_infantry.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_insane.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_medic.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_move.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_mutant.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_parasite.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_soldier.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_supertank.c
**  \brief Specific monster implementation.
*/

/*! \file game/m_tank.c
**  \brief Specific monster implementation.
*/

/*! \file game/p_client.c
**
**  \brief Core player support code.
**
**  Includes player spawn points, obituary messages, player death,
**  initialization of player data, userinfo strings, the client think
**  function, ClientBeginServerFrame (which is called once for each player
**  before processing any entities), etc.
**
**  \see ClientEndServerFrame
*/

/*! \file game/p_hud.c
**  \brief HUD and scoring displays.
*/

/*! \file game/p_trail.c
**
**  \brief Player "breadcrumb" trail.
**
**  This is used to allow monsters to follow players in the absence of a
**  real pathing system.
*/

/*! \file game/p_view.c
**
**  \brief Player view effects, including incredibly tangential stuff.
**
**  Includes the bobbing of the display during movement, view offset from
**  the player origin, falling damage, drowning, footsteps, weapon hums,
**  player animation, and ClientEndServerFrame (which is called once for
**  each player after processing all entities).
**
**  \see ClientBeginServerFrame
*/

/*! \file game/p_weapon.c
**  \brief Main code for player weapons.
*/

/*! \file game/q_shared.c
**
**  \brief Common Quake 2 code used everywhere, specifically including the
**  game, client, server and ref_modules.
**
**  Includes geometric operations, pathname manipulation,
**  serialization/deserialization, varargs, tokenization, library
**  replacement functions, info string support (whatever that is), and a
**  few other miscellaneous things.
*/

/*! \file game/q_shared.h
**
**  \brief Common Quake 2 definitions used by all modules, specifically
**  including the game, client, server and ref_modules.
*/

//@}


/// \defgroup null_grp null: Null Platform Support
///
/// The null module contains stub implementations of all platform-specific
/// functionality.

//@{

/*! \file null/cd_null.c
**  \brief Stub implementation of CD functions.
*/

/*! \file null/cl_null.c
**
**  \brief Stub implementation of platform-specific initialization,
**  shutdown, per-frame processing, etc.
*/

/*! \file null/glimp_null.c
**
**  \brief Stub implementation of platform-specific GL rendering code.
*/

/*! \file null/in_null.c
**  \brief Stub implementation of platform-specific input.
*/

/*! \file null/snddma_null.c
**  \brief Stub implementation of platform-specific audio.
*/

/*! \file null/swimp_null.c
**  \brief Stub implementation of platform-specific software rendering code.
*/

/*! \file null/sys_null.c
**  \brief Stub implementation of platform-specific system functions.
*/

/*! \file null/vid_null.c
**
**  \brief Stub implementation of platform-specific video code.
**
**  Includes window creation, available video modes, video menu support,
**  etc.
*/

//@}


/// \defgroup qcommon_grp qcommon: Common Files
///
/// The qcommon module is used by both the client and the server, but not
/// the video drivers or the game DLL.  It includes support for networking,
/// reading data files, performing movement prediction, etc.

//@{

/*! \file qcommon/cmd.c
**  \brief Support for processing of console commands.
*/

/*! \file qcommon/cmodel.c
**
**  \brief BSP model support.
**
**  Includes level loading, collision detection, mapping of points to BSP
**  leaves, and static and dynamic connectivity between areas of the map
**  (for culling and sound propagation).
*/

/*! \file qcommon/common.c
**
**  \brief Miscellaneous code.
**
**  Includes printf-style functions, error reporting, packet building,
**  memory-block management, argc/argv parsing, random numbers, application
**  startup, and Qcommon_Frame, which does all per-frame processing.
*/

/*! \file qcommon/crc.c
**  \brief Cyclic Redundancy Check for verifying packet integrity.
*/

/*! \file qcommon/crc.h
**  \brief Cyclic Redundancy Check for verifying packet integrity.
*/

/*! \file qcommon/cvar.c
**  \brief Console variable support.
*/

/*! \file qcommon/files.c
**
**  \brief Virtual filesystem support.
**
**  Includes support for *.PAK files and directory search paths.
*/

/*! \file qcommon/md4.c
**  \brief MD4 cryptographic hash function, used for checksums.
*/

/*! \file qcommon/net_chan.c
**
**  \brief Management of network channels, including transmission and
**  reception of packets.
*/

/*! \file qcommon/pmove.c
**
**  \brief Player movement code.
**
**  Shared between client and server to support movement prediction.
*/

/*! \file qcommon/qcommon.h
**  \brief Interface to qcommon library.
*/

/*! \file qcommon/qfiles.h
**  \brief Definitions for parsing PAK, PCX, MD2, SP2, WAL and BSP formats.
*/

//@}


/// \defgroup gl_grp ref_gl: Open GL Video Driver
///
/// Platform-independent support for OpenGL graphics.

//@{

/*! \file ref_gl/gl_draw.c
**  \brief Drawing 2D graphics, text, fades, etc.
*/

/*! \file ref_gl/gl_image.c
**  \brief Image loading and texture management.
*/

/*! \file ref_gl/gl_light.c
**  \brief Lightmaps and dynamic lighting.
*/

/*! \file ref_gl/gl_local.h
**  \brief Internal interfaces for OpenGL renderer.
*/

/*! \file ref_gl/gl_mesh.c
**  \brief Appears to be support for drawing MD2 models.
*/

/*! \file ref_gl/gl_model.c
**
**  \brief Support for loading all types of models, and a few utility
**  functions.
*/

/*! \file ref_gl/gl_model.h
**  \brief Interfaces to the GL renderer's model representations and APIs.
*/

/*! \file ref_gl/gl_rmain.c
**  \brief Core OpenGL renderer code.
**
**  Includes tons of different stuff, mostly implementing the functions
**  exported by the renderer.
*/

/*! \file ref_gl/gl_rmisc.c
**  \brief More miscellaneous renderer code, including screenshots.
*/

/*! \file ref_gl/gl_rsurf.c
**  \brief The actual surface drawing code.
**
**  Includes drawing support support for alpha channels, light maps,
**  outlines, regular surfaces, etc.
*/

/*! \file ref_gl/gl_warp.c
**  \brief Water and sky drawing support.
*/

/*! \file ref_gl/qgl.h
**  \brief Interface to OpenGL wrapper functions.
*/

//@}


/// \defgroup soft_grp ref_soft: Software Video Driver
///
/// Platform-independent support for unaccelerated graphics.

//@{

/*! \file ref_soft/asm_draw.h
**  \brief Data structure offsets for assembly code in software renderer.
*/

/*! \file ref_soft/d_ifacea.h
**  \brief Data structure offsets for assembly code in software renderer.
*/

/*! \file ref_soft/r_aclip.c
**  \brief Clip routines for drawing MD2 models directly to the screen.
*/

/*! \file ref_soft/r_alias.c
**  \brief MD2 model drawing code.
*/

/*! \file ref_soft/r_bsp.c
**  \brief BSP model drawing code.
*/

/*! \file ref_soft/r_draw.c
**  \brief Drawing of pictures, characters, fades, etc.
*/

/*! \file ref_soft/r_edge.c
**  \brief Surface drawing code, and some edge/span stuff.
**
**  From the comments and a cursory analysis, this looks like a some kind
**  of fancy zero-overdraw (or perhaps low-overdraw) BSP drawing code.
*/

/*! \file ref_soft/r_image.c
**  \brief Image list management, and PCX and TGA loading.
*/

/*! \file ref_soft/r_light.c
**  \brief Lightmap and dynamic lighting support.
*/

/*! \file ref_soft/r_local.h
**  \brief Internal APIs and data structures for the software renderer.
*/

/*! \file ref_soft/r_main.c
**  \brief Core software renderer code.
**
**  Includes tons of different stuff, mostly implementing the functions
**  exported by the renderer.
*/

/*! \file ref_soft/r_misc.c
**  \brief More miscellaneous renderer code, including screenshots.
*/

/*! \file ref_soft/r_model.c
**
**  \brief Support for loading all types of models, and a few utility
**  functions.
*/

/*! \file ref_soft/r_model.h
**
**  \brief Interfaces to the software renderer's model representations and
**  APIs.
*/

/*! \file ref_soft/r_over.c
**  \brief [IML] Overlay implementation.
**
**  This is a custom add-on for floating 2D overlays over the Quake 2
**  display.
*/

/*! \file ref_soft/r_part.c
**  \brief Particle drawing code.
*/

/*! \file ref_soft/r_poly.c
**  \brief Low-level polygon drawing code.
*/

/*! \file ref_soft/r_polyse.c
**
**  \brief Routines for drawing sets of polygons sharing the same texture
**  (used for MD2 models).
*/

/*! \file ref_soft/r_rast.c
**  \brief Low-level rasterization code.
*/

/*! \file ref_soft/r_scan.c
**  \brief Portable C scan-level rasterization code, all pixel depths.
*/

/*! \file ref_soft/r_sprite.c
**  \brief Sprite-drawing code.
*/

/*! \file ref_soft/r_surf.c
**  \brief Durface-drawing code.
*/

//@}


/// \defgroup server_grp server: Server Files
///
/// The server is in charge of managing the game DLL and communicating with
/// the client.

//@{

/*! \file server/server.h
**  \brief Interface to server files.
*/

/*! \file server/sv_ccmds.c
**  \brief Console commands implemented by the server.
*/

/*! \file server/sv_ents.c
**
**  \brief Support for sending entity and player state to clients.
**
**  Includes edict_t diff support, potentially visible entity
**  identification (entities far from the player should not be sent, to
**  prevent cheating and excess network use), and demo recording.
*/

/*! \file server/sv_game.c
**
**  \brief AI module startup and shutdown, plus miscellaneous functions.
**
**  Includes bbox calculation for entities with BSP models, PVS/PHS checks,
**  config strings, printf functions, etc.
*/

/*! \file server/sv_init.c
**
**  \brief Map and game initialization.
*/

/*! \file server/sv_main.c
**
**  \brief Main server code.
**
**  Includes client management, basic protocol messages, SV_Frame (the
**  master per-frame function), timeouts, heartbeats, etc.
*/

/*! \file server/sv_null.c
**
**  \brief Can be used to stub-out server system for client-only builds.
*/

/*! \file server/sv_send.c
**
**  \brief Support for sending various messages to the client.
*/

/*! \file server/sv_user.c
**
**  \brief Appears to contain various per-user functions.
**
**  Includes level changes, file downloads, and client movement message
**  processing.
*/

/*! \file server/sv_world.c
**
**  \brief Support for spatial processing.
**
**  Includes point-to-content-type mapping, calculation of BSP hulls for
**  all entities (including ones without BSP models), movement clipping,
**  and tracing of lines through 3D to see what they hit.  Also includes a
**  system of "areas"--NOT TO BE CONFUSED WITH THE "AREAS" IN THE BSP
**  FILE--which are used to keep track of which entities need to be tested
**  for collisions in a given part of the level.
*/

//@}


/// \defgroup win32_grp win32: Win32 Platform Support
///
/// These files implement all platform-specific features for 32-bit Windows
/// machines.

//@{

/*! \file win32/cd_win.c
**  \brief Win32 CD drive support, including CD audio playback.
*/

/*! \file win32/conproc.c
**  \brief Appears to be something related to qhost.
*/

/*! \file win32/conproc.h
**  \brief Appears to be something related to qhost.
*/

/*! \file win32/glw_imp.c
**  \brief Win32 portion of OpenGL renderer.
*/

/*! \file win32/glw_win.h
**  \brief Definition of OpenGL state data structure.
*/

/*! \file win32/in_win.c
**  \brief Win32 input code.
*/

/*! \file win32/net_wins.c
**  \brief Win32 network interface based on WinSock.
*/

/*! \file win32/q_shwin.c
**  \brief Win32 system interface functions.
*/

/*! \file win32/qgl_win.c
**  \brief Win32 versions of OpenGL wrapper functions.
** 
**  These do some logging and smooth out various issues about where OpenGL
**  function pointers actually come from.
*/

/*! \file win32/resource.h
**  \brief Automatically generated Win32 resource IDs.
*/

/*! \file win32/rw_ddraw.c
**  \brief DirectDraw back end for Win32 software renderer.
*/

/*! \file win32/rw_dib.c
**  \brief Fallback GDI/DIB back end for Win32 software renderer.
*/

/*! \file win32/rw_imp.c
**  \brief Win32 portion of software renderer.
*/

/*! \file win32/rw_win.h
**  \brief Definition of Win32 software renderer state data structure.
*/

/*! \file win32/snd_win.c
**  \brief Win32 sound implementation.
*/

/*! \file win32/sys_win.c
**  \brief Miscellaneous Win32 system interface functions.
*/

/*! \file win32/vid_dll.c
**
**  \brief Win32 window callback procedures, event dispatching, and ref_*
**  module management.
*/

/*! \file win32/vid_menu.c
**  \brief Win32 video mode menu.
*/

/*! \file win32/winquake.h
**  \brief Various Win32-specific interfaces and global variables.
*/

//@}


/// \defgroup wx_grp wx: wxWindows Intergration
///
/// Support for embedding Quake 2 as a wxWindows widget.

//@{

/*! \file wx/main.cpp
**  \brief [IML] Demo program.
*/

/*! \file wx/wxquake2.cpp
**  \brief [IML] Implementation of wxWindows-embedded Quake 2 widget.
*/

/*! \file wx/wxquake2.h
**  \brief [IML] Interface to wxWindows-embedded Quake 2 widget.
*/

//@}
