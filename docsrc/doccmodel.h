#include "../game/q_shared.h"

//! \fn cmodel_t *CM_LoadMap (char *name, qboolean clientload,
///                           unsigned *checksum)
///
/// Load a map from disk, including all *.BSP data.  Sets the current map
/// to the loaded map.
///
/// \param name The file name of the map.
/// \param clientload True if the map is being loaded by the client.
/// \param checksum [out] The checksum of the loaded map.
/// \return The primary cmodel_t of the level itself.

//! \fn cmodel_t *CM_InlineModel (char *name)
///
/// Returns a submodel of the current map.
///
/// \param name "*1" for the first submodel, "*2" for the next, etc.  These
/// are the same strings which used by an entity's "model" property.

//! \fn int CM_NumClusters (void);
///
/// Returns the number of clusters in the map.  Clusters are used to implement
/// static visibility of objects and audibility of sounds.

//! \fn int CM_NumInlineModels (void);
///
/// Returns the number of inline models in a map.  Inline models describe
/// doors, platforms, and other architectual entities.

//! \fn char *CM_EntityString (void)
///
/// Returns the entity string for the level.  This is an ASCII formatted
/// string describing the keys and values for all entities on the level.
/// You can find an example by scolling to the end of any BSP file.

//! \fn int CM_HeadnodeForBox (vec3_t mins, vec3_t maxs)
///
/// Construct a simple, temporary BSP for the bounding box of an MD2 model.
/// For uniformity, both MD2 and BSP models are treated as BSP models for
/// certain operations.  This BSP is stored in global storage, so each call
/// to this function wipes out the previous result.
///
/// \param mins Minimum X, Y and Z co-ordinates.
/// \param maxs Maximum X, Y and Z co-ordinates.
/// \return The node ID of the temporary BSP model.

//! \fn int CM_PointContents (vec3_t p, int headnode)
///
/// Get the content-type flags for the specified point.  This can be used to
/// tell if a given point is underwater, in lava, and so on.
///
/// \callgraph
///
/// \param p The point.
/// \param headnode The headnode of the BSP to search.
/// \return Bitwise OR of zero or more CONTENT_* flags.

//! \fn int CM_TransformedPointContents (vec3_t p, int headnode, vec3_t origin,
///                                      vec3_t angles)
///
/// Get the content-type flags of point, assuming a transformation has been
/// applied to the BSP model.  Presumably used for submodels.
///
/// \param p The point.
/// \param headnode The headnode of the BSP to search.
/// \param origin The current origin of the BSP model.
/// \param angles The current Euler rotation of the BSP model.
/// \return Bitwise OR of zero or more CONTENT_* flags.
///
/// \see CM_PointContents

//! \fn void CM_TestInLeaf (int leafnum)
///
/// Determine whether the specified box overlaps any brush in the specified
/// leaf, and if so, update the trace data.  This is done by walking
/// through all leafs attached to this brush, and calling
/// CM_TestBoxInBrush().  (Since brushes may appear in multiple leafs, this
/// function is careful to avoid checking brushes which were already
/// checked by another leaf during the current trace.)

//! \fn void CM_TestBoxInBrush (vec3_t mins, vec3_t maxs, vec3_t p1,
///                             trace_t *trace, cbrush_t *brush)
///
/// Determine whether the specified box overlaps the specified brush, and if
/// so, updates the trace data.
///
/// \param mins Minimum X, Y and Z co-ordinates of the box.
/// \param maxs Maximum X, Y and Z co-ordinates of the box.
/// \param p1   The origin of the box.
/// \param trace [out] The trace record to fill out if the box overlaps
///             the brush.
/// \param brush The brush to test against.
///
/// \see CM_ClipBoxToBrush (a similar function)
///
/// <h3>Algorithm</h3>
///
///   - For each brush side:
///     - If all of object is on front side (normal side) of the side's plane:
///       - Return immediately, since there's no intersection.
///   - Update the trace for an impact with this brush.

//! \fn void CM_ClipBoxToBrush (vec3_t mins, vec3_t maxs, vec3_t p1, vec3_t p2,
///                             trace_t *trace, cbrush_t *brush)
///
/// Perform the trace on an individual brush.
///
/// \param mins Minimum X, Y and Z co-ordinates of the box, relative to p1/p2.
/// \param maxs Maximum X, Y and Z co-ordinates of the box, relative to p1/p2.
/// \param p1 The start point for this section of the trace.
/// \param p2 The end point for this section of the trace.
/// \param trace [out] The trace record to fill out if the box encounters
///                    this brush.
/// \param brush The brush to test against.
///
/// \see CM_TestBoxInBrush (a similar function)
///
/// <h3>Algorithm</h3>
///
/// This is a bit of an oversimplification, but it gives you the general
/// idea:
///
///   - For each brush side:
///     - If the trace is completely in front of the side:
///       - Return immediately because we have no intersection.
///     - If the trace is completely behind the side:
///       - Skip to the next side.
///     - Determine if we're entering or leaving.  
///     - If this intersection is better than what we have, record it.
///   - Update trace result data structures appropriately.

//! \fn void CM_TraceToLeaf (int leafnum)
///
/// Perform the trace on an individual leaf.
///
/// \callgraph
///
/// \param leafnum The number of the current leaf.
///
/// <h3>Algorithm</h3>
///
///   - If the leaf contents don't intersect our trace contents, return.
///   - For each brush we haven't already seen during this trace:
///     - If the leaf contents don't intersect our trace contents, continue.
///     - Call CM_ClipBoxToBrush() on the leaf.

//! \fn void CM_RecursiveHullCheck (int num, float p1f, float p2f, vec3_t p1,
///                                 vec3_t p2)
///
/// Perform a trace on a node and as many of its children as necessary.
///
/// \callgraph
///
/// \param num The number of the current node (negative for leafs).
/// \param p1f The fraction of the overall trace complete by p1 (0.0-1.0).
/// \param p2f The fraction of the overall trace complete by p2 (0.0-1.0).
/// \param p1 The starting point for this section of the trace.
/// \param p2 The stopping point for this section of the trace.
///
/// <h3>Algorithm</h3>
///
///   - If we've already found a better intersection, give up now.
///   - If we're a leaf, call CM_TraceToLeaf() instead.
///   - Otherwise, determine which side(s) of the split plane our box
///     falls on.
///     - If the box's trace falls on only one side of the plane, call
///       CM_RecursiveHullCheck on the appropriate child node.
///     - If the box's trace crosses the plane, split our trace line in half
///       and call CM_RecursiveHullCheck on both child nodes. 

//! \fn trace_t CM_BoxTrace (vec3_t start, vec3_t end, vec3_t mins,
///                          vec3_t maxs, int headnode, int brushmask)
///
/// Trace a moving box along a line through space and see what it hits
/// first.  This function is used for collision detection for moving
/// objects, for checking visibility (using a zero-size box), for figuring
/// out what a weapon hit, and so on.  See trace_t for more detailed
/// documentation of this function's result, which is fairly complicated.
///
/// \callgraph
///
/// \param start The starting position of the trace.
/// \param end The ending position of the trace.
/// \param mins The minimum X, Y and Z co-ordinates of the box relative
/// to the 'start' parameter.
/// \param maxs The maximum X, Y and Z co-ordinates of the box relative
/// to the 'start' parameter.
/// \param headnode The headnode of the BSP model in which to trace.
/// \param brushmask The content types which will stop the box's trace.
/// \return A description of the trace's results.
///
/// \see WANT_TRACE_EXIT, CM_TestBoxInBrush, CM_TransformedBoxTrace
///
/// <h3>Algorithm</h3>
///
/// This function does a lot of setup and teardown of trace data.  But
/// the actual algorithm works pretty much as follows:
///
///   - If the start and end points are the same, do a quick position
///     test:
///     - Call CM_BoxLeafnums_headnode() to get a list of all the leafs our
///       box intersects.
///     - For each leafnum:
///       - Call CM_TestInLeaf() to update the trace data.
///   - Otherwise, call CM_RecursiveHullCheck().

//! \fn trace_t CM_TransformedBoxTrace (vec3_t start, vec3_t end, vec3_t mins,
///                                     vec3_t maxs, int headnode,
///                                     int brushmask, vec3_t origin,
///                                     vec3_t angles)
///
/// Trace a box along a line through space and see what it hits, assuming
/// a transformed model.
///
/// \callgraph
///
/// \param start The starting position of the trace.
/// \param end The ending position of the trace.
/// \param mins The minimum X, Y and Z co-ordinates of the box relative
/// to the 'start' parameter.
/// \param maxs The maximum X, Y and Z co-ordinates of the box relative
/// to the 'start' parameter.
/// \param headnode The headnode of the BSP model in which to trace.
/// \param brushmask The content types which will stop the box's trace.
/// \param origin The current origin of the BSP model.
/// \param angles The current Euler rotation of the BSP model.
/// \return A description of the trace's results.
///
/// \see CM_BoxTrace

//! \fn byte *CM_ClusterPVS (int cluster)
///
/// The decompressed "potentially visible set" of the specified cluster.

//! \fn byte *CM_ClusterPHS (int cluster)
///
/// The decompressed "potentially hearable set" of the specified cluster.

//! \fn int CM_PointLeafnum (vec3_t p);
///
/// Get the leaf number of the specified point.
///
/// \callgraph
///
/// <h3>Algorithm</h3>
///
/// Recursively walk down the nodes of a BSP tree, starting with the
/// headnode.  At each node, consult the splitting plane.  If the point
/// lies on the side of the plane to which the normal points (or on the
/// splitting plane itself), descend along child 0 of the node.  Otherwise,
/// descend along child 1.  Stop when a leaf is reached.

//! \fn void CM_BoxLeafnums_r (int nodenum)
///
/// Helper function for CM_BoxLeafnums().

//! \fn int	CM_BoxLeafnums_headnode (vec3_t mins, vec3_t maxs, int *list,
///                                  int listsize, int headnode, int *topnode)
///
/// Helper function for CM_BoxLeafnums().

//! \fn int CM_BoxLeafnums (vec3_t mins, vec3_t maxs, int *list, int listsize,
///                         int *topnode)
///
/// Get a list of all BSP leafs intersecting a box.
///
/// \callgraph
///
/// \param mins Minimum X, Y and Z co-ordinates of the box.
/// \param maxs Maximum X, Y and Z co-ordinates of the box.
/// \param list [out] Storage for the leaf numbers.
/// \param listsize Maximum number of leafs to return.
/// \param topnode The tighest-fit node which contains the entire box.
/// \return The number of leafs intersected by the box.
///
/// \see CM_PointLeafnum (a simpler relative of this function)

//! \fn int CM_LeafContents (int leafnum)
///
/// Get the content-type flags for the specified leaf.
///
/// \return Bitwise OR of zero or more CONTENT_* flags.

//! \fn int CM_LeafCluster (int leafnum)
///
/// Get the number of the cluster containing the specified leaf.
///
/// \see CM_NumClusters

//! \fn int CM_LeafArea (int leafnum)
///
/// Get the number of the area containing the specified leaf.  Areas are
/// used to describe dynamic connections between different parts of the
/// map, and are used in conjuction with clusters to cull visibility and
/// audibility.
///
/// \see CM_NumClusters

//! \fn void CM_SetAreaPortalState (int portalnum, qboolean open)
///
/// Set a portal to open or closed.  This function is called by doors with
/// attached portals.
///
/// \param portalnum The portal which we want to open or close.
/// \param open The new state for this portal.

//! \fn qboolean CM_AreasConnected (int area1, int area2)
///
/// Given the current sets of open and closed portals, are these two areas
/// currently connected?

//! \fn int CM_WriteAreaBits (byte *buffer, int area)
///
/// Writes a length byte followed by a bit vector of all the areas
/// connected to the specified area.  This is used by the client refreshes
/// to cull visibility.
///
/// \param buffer [out] The buffer in which to store the data.
/// \param area The current area.

//! \fn qboolean CM_HeadnodeVisible (int nodenum, byte *visbits)
///
/// Scan the specified BSP node to see if it contains any
/// potentially-visible leafs.  Checks the cluster numbers of all leaves
/// against the specified PVS data until it finds a match.
///
/// This function is used to help determine whether the server should send
/// information about an entity to the player.  Generally speaking, the
/// server does not send information about distant entities.
///
/// \callgraph
///
/// \param nodenum The BSP node at which to start the search.
/// \param visbits The set of potentially visible clusters.
///
/// \see CM_WriteAreaBits

//! \fn void CM_WritePortalState (FILE *f)
///
/// Write the current area portal state to disk when saving a game.

//! \fn void CM_ReadPortalState (FILE *f)
///
/// Read the current area portal state from disk when loading a game.



/*! \def WANT_TRACE_EXIT
**
**  \brief [IML] Special content flag for use with CM_BoxTrace.
**
**  When included in the 'brushmask' parameter of CM_BoxTrace, this flag
**  activates a special tracing mode which traces both into and out of
**  objects.  This can be used iteratively (and with care) to trace a line
**  through multiple BSP walls, recording the entrance and exit surfaces as
**  we go.
** 
**  \see trace_t, CM_BoxTrace, CM_TransformedBoxTrace
*/

/*! \struct trace_t
**  \brief A trace is returned when a box is swept through the world.
**
**  [IML] We add support for calculating the exit surface of a trace, for
**  use in radiation shielding code.  These fields are only used if if the
**  content mask includes WANT_TRACE_EXIT.  If WANT_TRACE_EXIT is true,
**  then 'allsolid' and 'startsolid' are undefined--use 'entered' and
**  'exited' instead.
**
**  \see WANT_TRACE_EXIT, CM_BoxTrace, CM_TransformedBoxTrace
*/

/*! \var trace_t::allsolid
**  \brief if true, plane is not valid
*/
/*! \var trace_t::startsolid
**  \brief if true, the initial point was in a solid area
*/
/*! \var trace_t::fraction
**  \brief time completed, 1.0 = didn't hit anything
*/
/*! \var trace_t::endpos
**  \brief final position
*/
/*! \var trace_t::plane
**  \brief surface normal at impact
*/
/*! \var trace_t::surface
**  \brief surface hit
*/
/*! \var trace_t::contents
**  \brief contents on other side of surface hit
*/
/*! \var trace_t::entered
**
**  \brief [IML] The fields fraction, endpos, plane, surface are valid if
**  'entered' is true.  The field 'contents' is set, but it may not mean
**  much.
*/
/*! \var trace_t::exited
**
**  \brief [IML] Fields exit_fraction, nextpos, exit_plane and exit_surface
**  are valid if 'exited' is true.
*/
/*! \var trace_t::exit_fraction
**  \brief [IML] time we exited the face
*/
/*! \var trace_t::nextpos
**  \brief [IML] spot at which to begin next trace
*/
/*! \var trace_t::exit_plane
**  \brief [IML] surface normal at exit
*/
/*! \var trace_t::exit_surface
**  \brief [IML] surface exited
*/
/*! \var trace_t::ent
**  \brief not set by CM_*() functions
*/
