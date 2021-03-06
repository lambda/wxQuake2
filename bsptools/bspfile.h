// -*- Mode: C++; tab-width: 4; c-basic-offset: 4; -*-
// @BEGIN_LICENSE
//
// Tamale - Multimedia authoring and playback system
// Copyright 1993-2004 Trustees of Dartmouth College
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// @END_LICENSE

//! \file bspfile.h
/// Interfaces for accessing the contents of *.BSP files.

/// Support for accessing the contents of *.BSP files.
namespace bspfile {
    using std::string;
    using std::vector;

    /// A three-element vector.  This is used to represent vertices, min
    /// and max bounds, origins, and a variety of other values found
    /// throughout a BSP file.
    ///
    /// \see vec3_t, dvertex_t, LUMP_VERTEXES
    class vec3 {
        float m_components[3];
        
    public:
        /// Initialize a new vector to undefined values.
        vec3() {}

        /// Create a new vector.
        vec3(float x, float y, float z) {
            m_components[0] = x;
            m_components[1] = y;
            m_components[2] = z;
        }

        /// Create a new vector.
        ///
        /// \param components Pointer to an array of three floats.
        vec3(float *components) {
            m_components[0] = components[0];
            m_components[1] = components[1];
            m_components[2] = components[2];
        }

        /// Access a vector's components.
        float operator[](size_t i) const {
            assert(0 <= i && i < 3);
            return m_components[i];
        }

        /// Access a vector's components.
        float &operator[](size_t i) {
            assert(0 <= i && i < 3);
            return m_components[i];
        }
    };

    /// A set of clusters.  A cluster is a medium-sized region of a map,
    /// represented by an integer.  cluster_set objects are used to
    /// represent sets of clusters visibile and/or audible from a given
    /// cluster.  For more information, see leaf::get_cluster.  (For
    /// information on <i>dynamic</i> connectivity, see area.)
    ///
    /// \see leaf::get_cluster, leaf::get_pvs, leaf::get_phs, area
    class cluster_set {
    public:
        /// Returns true if the set contains <tt>cluster</tt>.
        bool contains(size_t cluster) const;
    };

    /// Areas are medium-sized regions of a map, used to control dynamic
    /// visibility and audibility.  Each area is completely sealed off by
    /// walls and portals.  Portals connect areas.  Each portal can be
    /// either open or closed.  Whenever a portal changes state, all areas
    /// are immediately flood-filled by the engine to determine
    /// connectivity.  If two areas are not connected, then sound and light
    /// cannot pass between them.
    ///
    /// Portals are actually invisible.  Each portal has an associated
    /// door, which controls whether the portal is on or off.
    ///
    /// Static visibility and audibility are handled using the PVS and
    /// PHS tables.  See leaf::get_cluster for more information.
    ///
    /// \see leaf::get_area, leaf::get_pvs, leaf::get_phs, darea_t,
    /// LUMP_AREAS, dareaportal_t, LUMP_AREAPORTALS
    class area {
    public:
        /// A link between two areas.
        class portal {
        public:
            /// The ID of the portal.  There should be two area::portal
            /// objects for each actual portal; these represent the two
            /// different sides of the portal.  These two area::portal
            /// objects will share the same ID.
            size_t portal_number;

            /// The other area to which this portal connects.
            area *connecting_to;
        };

    private:
        portal *m_portals;

    public:
        /// The number of portals associated with this area.
        size_t get_portal_count() const;

        /// Get the specified portal.
        portal *get_portal(size_t id) const;
    };

    /// A geometric plane.  Planes are represented in the standard fashion:
    /// a normal vector and a distance from the origin.
    ///
    /// \see dplane_t, LUMP_PLANES
    /// \todo get_type
    class plane {
        vec3 m_normal;
        float m_dist;

    public:
        /// Create a new plane.
        plane(const vec3 &normal, float distance)
            : m_normal(normal), m_dist(distance) {}
        
        /// The plane's normal vector.
        vec3 get_normal() const { return m_normal; }
        /// The plane's distance from the origin.
        float get_dist() const { return m_dist; }
    };
    
    /// Texture information for a surface.  The actual textures themselves
    /// (and lots of related data) are stored in external *.wal files.
    /// This class is responsible for binding the *.wal file to a surface.
    ///
    /// \see texinfo_t, LUMP_TEXINFO
    ///
    /// \todo Figure out what to do with vecs & flags fields.
    class texinfo {
        int m_value;
        string m_name;
        texinfo *m_next_texinfo;

    public:
        /// Create a new texinfo.
        texinfo(int value, const string &name, texinfo *next_texinfo = NULL)
            : m_value(value), m_name(name), m_next_texinfo(next_texinfo) {}

        /// The value associated with this texture.  The meaning of this
        /// field varies depending on which texture flags are set; typically,
        /// it represents the light value of the texture.
        int get_value() const;

        /// Get the name of this texture.  According to the Quake 2
        /// comments, there should be an matching textures/*.wal file.
        string get_texture_name() const;

        /// Returns the next texture info in an animation chain, or NULL if
        /// the texture is not animated.
        texinfo *get_next_texinfo() const;
    };

    /// A ploygon edge between two vertex objects.  Direction is
    /// unspecified, and depends on the polygon winding.  May be shared.
    ///
    /// \see dedge_t, LUMP_EDGES
    class edge {
        vec3 m_endpoints[2];

    public:
        /// Create a new edge.
        edge(const vec3 &endpoint_0, const vec3 &endpoint_1)
            { m_endpoints[0] = endpoint_0; m_endpoints[1] = endpoint_1; }
        
        /// Get endpoint 0 or 1 of the edge.
        vec3 get_endpoint(size_t id) const;
    };

    /// A polygon face.  In general, faces are only used in the drawing
    /// code; all collision detection relies on brushes instead.
    ///
    /// \see node, leaf, brush, dface_t, LUMP_FACES, LUMP_SURFEDGES (lookup
    /// table mapping faces to edges)
    ///
    /// \todo get_side
    /// \todo get_light_styles
    /// \todo get_light_offset
    class face {
	public:
        /// Enumeration indicating which side of a plane the face is on.
        enum side {
            PLANE_FRONT, ///< The side of a plane towards the normal.
            PLANE_BACK   ///< The side of a plane opposite the normal.
        };

    private:
        plane *m_plane;
        side m_side;
        size_t m_edge_count;
        edge *m_edges;
        texinfo *m_texinfo;
        
    public:
        /// Create a new polygon face.
        face(plane *plane_, side side_, size_t edge_count, edge *edges,
             texinfo *texinfo_)
            : m_plane(plane_), m_side(side_), m_edge_count(edge_count),
              m_edges(edges), m_texinfo(texinfo_) {}

        /// Get the plane in which the polygon lies.
        plane *get_plane() const;

        /// Get the side of the plane towards which the polygon faces.
        side get_side() const;

        /// The number of edges associated with the polygon.
        size_t get_edge_count() const;
        /// Get the specified edge.
        edge *get_edge(size_t id) const;

        /// Get the texture info for this face.
        texinfo *get_texinfo() const;
    };

    /// A brushside is a single side of a brush.  Because a brush is a
    /// convex polyhedron, a brushside can be represented as a plane.
    ///
    /// \see brush, dbrushside_t, LUMP_BRUSHSIDES
    class brushside {
        plane *m_plane;
        texinfo *m_texinfo;

    public:
        /// Create a new brush side.
        brushside(plane *p, texinfo *t)
            : m_plane(p), m_texinfo(t) {}

        /// The plane in which this brushside lies.
        ///
        /// \todo I'm not sure how the engine determines which side is
        /// inside and which is outside.
        plane *get_plane() const;

        /// Get the texture info for this face.
        texinfo *get_texinfo() const;
    };

    /// A brush is one of the convex polyhedrons from which the map is
    /// constructed.  Brushes are not used for drawing; that is handled by
    /// faces instead.  Brushes contain no edge or vertex data.  Instead,
    /// they contain a set of sides specified as planes.  This works
    /// because any convex polyhedron can be uniquely described by a set of
    /// planes.
    ///
    /// Brushes appear to be used for collision detection and other spatial
    /// operations in the client and server.
    ///
    /// I'm not sure whether all the original brush data is actually stored
    /// in the BSP, or whether invisible backplanes (and other unnecessary
    /// stuff) are compiled out by the BSP-building tools.
    ///
    /// \see leaf, face, dbrush_t, LUMP_BRUSHES
    /// \todo get_contents
    class brush {
        size_t m_brushside_count;
        brushside *m_brushsides;

    public:
        /// Create a new brush.
        brush(size_t brushside_count, brushside *brushsides)
            : m_brushside_count(brushside_count), m_brushsides(brushsides) {}

        /// The number of brushsides associated with this brush.
        size_t get_brushside_count() const;
        /// Get the specified brushside.
        brushside *get_brushside(size_t id) const;
    };

    /// An object with a bounding box.
    class bounded {
        vec3 m_mins;
        vec3 m_maxs;

    public:
        /// Get the minimum X, Y and Z co-ordinates of this object.
        vec3 get_mins() const;
        /// Get the maximum X, Y and Z co-ordinates of this object.
        vec3 get_maxs() const;
    };

    /// Either a BSP node or a BSP leaf.
    class node_child : public bounded {};

    /// A leaf of the BSP tree.
    ///
    /// \see model, node, dleaf_t, LUMP_LEAFS, LUMP_LEAFFACES (lookup table
    /// mapping leafs to faces), LUMP_LEAFBRUSHES (lookup table mapping
    /// leafs to brushes)
    ///
    /// \todo get_contents
    class leaf : public node_child {
        size_t m_cluster;
        cluster_set *m_pvs;
        cluster_set *m_phs;
        area *m_area;
        size_t m_brush_count;
        brush *m_brushes;
        size_t m_face_count;
        face *m_faces;

    public:
        /// Create a new leaf.
        leaf(size_t cluster, area *a, size_t brush_count, brush *brushes,
             size_t face_count, face *faces)
            : m_cluster(cluster), m_area(a), m_brush_count(brush_count),
              m_brushes(brushes), m_face_count(face_count), m_faces(faces) {}

        /// Get the cluster number associated with this leaf.  A cluster is
        /// a medium sized region of a level which can be checked against
        /// another leaf's pvs or phs to determine potential visibility and
        /// audibility.  The number returned by this function is only
        /// meaningful as an argument to cluster_set::contains.
        ///
        /// To get a cluster_set object, call either leaf::get_pvs (for
        /// visibility data) or leaf::get_phs (for audibility data).
        ///
        /// \see leaf::get_pvs, leaf::get_phs, cluster_set::contains
        size_t get_cluster() const;
        /// Get the potentially-visible set of clusters for this leaf.
        /// This is used to limit which portions of the map get drawn and
        /// which clients receive data about multicasted visual events.
        /// The PVS only takes static map state into account; you can
        /// further prune what needs to be drawn using area portals (which
        /// manage dynamic barriers such as doors).
        ///
        /// \see leaf::get_phs, area, dvis_t, LUMP_VISIBILITY
        cluster_set get_pvs() const;
        /// Get the potentially-hearable set of clusters for this leaf.
        /// This is used by the AI to limit what a monster can hear, and to
        /// limit which clients receive information about multicasted
        /// audible events.  The PHS only takes static map state into
        /// account; you can further prune what needs to be drawn using
        /// area portals (which manage dynamic barriers such as doors).
        ///
        /// \see leaf::get_pvs, area, dvis_t, LUMP_VISIBILITY
        cluster_set get_phs() const;
        /// Get the area containing this leaf.  Areas are used to contol
        /// dynamic visibility and audibility.
        area *get_area() const;

        /// The number of brushes associated with this leaf.
        size_t get_brush_count() const;
        /// Get the specified brush.
        brush *get_brush(size_t id) const;

        /// The number of faces associated with this leaf.  Only used by
        /// the drawing code.
        ///
        /// \todo Pull into a superclass?
        size_t get_face_count() const;
        /// Get the specified face.
        face *get_face(size_t id) const;
    };

    /// An interior node of the BSP tree.
    ///
    /// \see model, leaf, dnode_t, LUMP_NODES
    class node : public node_child {
        plane *m_split_plane;
        node_child *m_children[2];
        size_t m_face_count;
        face *m_faces;

    public:
		 /// \todo Remove.
		node() {}
        node(plane *split_plane, node_child *child_0, node_child *child_1,
             size_t face_count, face *faces);

        /// Get the split plane for this node.  All objects one side of the
        /// plane will be placed into child 0, and all objects on the other
        /// side will be placed into child 1.  Side 0 is in the direction
        /// of the normal; side 1 is opposite.
        plane get_split_plane() const;
        /// Get either child 0 or child 1.
        node_child *get_child(size_t id) const;

        /// The number of faces associated with this node.  This appears to
        /// be used only in the dynamic lighting code and in the software
        /// BSP rendering code.
        size_t get_face_count() const;
        /// Get the specified face.
        face *get_face(size_t id) const;
    };

    /// A BSP ("binary space partition") tree.  There is one model for the
    /// level itself, and a model for each entity with attached BSP data
    /// (e.g. doors, platforms, triggers).  Generally speaking, only
    /// "architectural" data is stored as a BSP; smaller objects such as
    /// players, weapons and barrels are stored in separate md2 files.
    ///
    /// <b>Basic Binary Space Partition Concepts</b>
    ///
    /// A BSP tree consists of interior nodes and leafs.  Each interior
    /// node contains a plane and two children, which may be other interior
    /// nodes or leafs.  The plane is used to partition space: everything
    /// on one side of the plane goes into one child, and everything on the
    /// goes into the other child.  Objects which straddle the plane may be
    /// placed in both children.  As you go down the tree, each successive
    /// node splits to remaining space in two.
    ///
    /// BSP trees allow fast searching of space, quick intersection tests
    /// for line segments, and efficient boolean operations on complicated
    /// shapes.
    ///
    /// You can learn more about BSP trees in any graphics or game
    /// programming textbook.
    ///
    /// <b>Related Quake 2 Algorithms</b>
    ///
    ///   - CM_PointLeafnum() finds the leaf corresponding to a point.
    ///   - CM_BoxLeafnums() finds the leafs intersecting a box.
    ///   - CM_BoxTrace() sweeps a box along a line segment and determines
    ///     what it hits.
    ///   - CM_HeadnodeForBox() constructs a temporary BSP tree from an
    ///     axis-aligned bounding box.
    ///
    /// \see leaf, node, dmodel_t, LUMP_MODELS
    class model : public bounded {
        vec3 m_origin;
        node *m_headnode;
        size_t m_face_count;
        face *m_faces;

    public:
        /// Commented "for sounds or lights" in the Quake 2 source, but
        /// apparently never used.
        vec3 get_origin() const;

        /// Get the headnode of this model's BSP tree.
        node *get_headnode() const;

        /// The number of faces associated with this model.  According to
        /// the Quake 2 source, "submodels just draw faces without walking
        /// the bsp tree".  In general, the ref_* drivers use this any time
        /// they want to get all faces without rummaging through the BSP
        /// tree.
        size_t get_face_count() const;
        /// Get the specified face.
        face *get_face(size_t id) const;
    };

    /// An entity in the map (including monsters, objects, doors, etc.).
    /// An entity consists of a set of key/value pairs.  All entities have
    /// a class (represented by the key "classname").  Some entities have
    /// an associated model (represented by the key "model", with a value
    /// such as "*1", "*2", etc.).  The model is a small BSP tree
    /// describing a door, platform, or other bit of dynamic architecture
    /// represented by an entity.
    ///
    /// The entity with the classname "worldspawn" represents the map
    /// itself, and may contain a number of global properties.
    ///
    /// \see LUMP_ENTITIES
    class entity {
    public:
        /// Get the class of this entity.
        string get_classname();

        /// Does this entity have the specified property?
        bool has_property(const string &name) const;
        /// Get the specified property's value.
        string get_property(const string &name) const;

        /// Does this entity have an associated model?
        bool has_model() const;
        /// Get the model associated with this entity.
        model *get_model() const;
    };

    /// A Quake 2 BSP file containing data for a single map.
    ///
    /// \see dheader_t
    class level {
        vector<unsigned char> m_raw_data;

        vector<entity> m_entities;
        vector<model> m_models;
        vector<node> m_nodes;
        vector<leaf> m_leafs;
        vector<brush> m_brushes;
        vector<brushside> m_brushsides;
        vector<face> m_faces;
        vector<texinfo> m_texinfos;
        vector<plane> m_planes;
        vector<edge> m_edges;
        vector<vec3> m_vertices;

        vector<edge*> m_surfedges;
        vector<face*> m_leaffaces;
        vector<brush*> m_leafbrushes;

        template <typename T>
        void get_lump(size_t lump_id, T *&out_base, size_t &out_size);
        template <typename RecordType, typename ObjectType>
        void load_records(size_t lump_id, vector<ObjectType> &out_objs);

        void load_pvs_phs();
        void load_areas_and_portals();
        void load_entities();

    public:
        level(const string &filename);

        /// \name Entity Functions
        ///
        /// These functions allow you to access the level's entities.

        //@{

        /// The number of entities in this level.
		size_t get_entity_count() const { return m_entities.size(); }
        /// Get the specified entity.  Entity number 0 <i>should</i> be the
        /// worldspawn.
		const entity *get_entity(size_t id) const { return &m_entities[id]; }

        //@}

        /// \name BSP Model Functions
        ///
        /// The functions allow you to access the level's BSP models.
        /// There is one model for the level itself, and "submodels" for
        /// various entities such as doors, platforms, triggers, etc.

        //@{

        /// The number of models in this level.
		size_t get_model_count() const { return m_models.size(); }
        /// Get the specified model.  Model number 0 is the level itself.
		model *get_model(size_t id) { return &m_models[id]; }

        /// The number of BSP nodes in this level. 
		size_t get_node_count(size_t id) const { return m_nodes.size(); }
        /// Get the specified BSP node.  Node 0 <i>should</i> be the root
        /// node for the level's BSP tree.
		node *get_node(size_t id) { return &m_nodes[id]; }

        /// The number of BSP leafs in this level.
		size_t get_leaf_count() const { return m_leafs.size(); }
        /// Get the specified BSP leaf.
		leaf *get_leaf(size_t id) { return &m_leafs[id]; }

        /// The number of brushes in the level.
		size_t get_brush_count() const { return m_brushes.size(); }
        /// Get the specified brush.
		brush *get_brush(size_t id) { return &m_brushes[id]; }

        /// The number of brushsides in the level.
		size_t get_brushside_count() const { return m_brushsides.size(); }
        /// Get the specified brushside.
		brushside *get_brushside(size_t id) { return &m_brushsides[id]; }

        /// The number of polygon faces in the level.
		size_t get_face_count() const { return m_faces.size(); }
        /// Get the specified face.
		face *get_face(size_t id) { return &m_faces[id]; }

        /// The number of planes in this level.
		size_t get_plane_count() const { return m_planes.size(); }
        /// Get the specified plane.
		plane *get_plane(size_t id) { return &m_planes[id]; }

        /// The number of polygon edges in the level.
		size_t get_edge_count() const { return m_edges.size(); }
        /// Get the specified edge.
		edge *get_edge(size_t id) { return &m_edges[id]; }

        /// The number of vertices in the level.
		size_t get_vertex_count() const { return m_vertices.size(); }
        /// Get the specified vertex.
		vec3 get_vertex(size_t id) { return m_vertices[id]; }

        //@}

        /// \name Texture Functions
        ///
        /// Face and brushside objects are covered with textures.

        //@{

        /// The number of texinfos in the level.
		size_t get_texinfo_count() const { return m_texinfos.size(); }
        /// Get the specified texinfo.
		texinfo *get_texinfo(size_t id) { return &m_texinfos[id]; }

        //@}


        /// \name Cluster And Area Functions
        ///
        /// Levels are divided into clusters, which track static
        /// relationships between different parts of the level; and areas,
        /// which track dynamic relationships.

        //@{

        /// The number of clusters in the level.  A cluster is a
        /// medium-sized area of the level used for determining what can be
        /// seen and heard from a given location.
        size_t get_cluster_count() const;
        /// Get the "potentially visible set" for the specified cluster.
        cluster_set get_cluster_pvs(size_t id) const;
        /// Get the "potentially hearable set" for the specified cluster.
        cluster_set get_cluster_phs(size_t id) const;

        /// The number of areas in this level.
		/// \bug Implement.
		size_t get_area_count() const { return 0; }
        /// Get the specified area.
		/// \bug Implement.
		area *get_area(size_t id) { return NULL; }

        //@}

		/// \bug Implement.
		edge *get_surfedge_list(size_t count, size_t index) { return NULL; }
		/// \bug Implement.
		brushside *get_brushside_list(size_t count, size_t index) { return NULL; }
		/// \bug Implement.
		brush *get_leafbrush_list(size_t count, size_t index) { return NULL; }
		/// \bug Implement.
		face *get_leafface_list(size_t count, size_t index) { return NULL; }
    };

    //=====================================================================
    //  Data Types We Know We Don't Care About
    //=====================================================================

    // LUMP_LIGHTING
    // class lighting {};

    // Does not appear to be used anywhere by engine.
    //class pop {};
};
