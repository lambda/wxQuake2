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

//! \file bspfile.cpp
/// Implementation for accessing the contents of *.BSP files.

#include "precompiled.h"
#include <fstream>
#include "bspfile.h"

using namespace bspfile;


//=========================================================================
//  Internal Quake 2 Headers
//=========================================================================

extern "C" {
    /// Temporary remapping of 'false' for parsing Quake 2 C code.
#	define false QFALSE
    /// Temporary remapping of 'true' for parsing Quake 2 C code.
#	define true QTRUE
#	include "../game/q_shared.h"
#	include "../qcommon/qfiles.h"
#	undef false
#	undef true
};


//=========================================================================
//  level Methods
//=========================================================================

namespace {

    template <typename ObjectType, typename RecordType>
    ObjectType conv_record(level *l, RecordType &r);
    
    template <>
    inline vec3 conv_record<vec3,dvertex_t>(level *l, dvertex_t &r) {
        return vec3(r.point);
    }
    
    template <>
    inline plane conv_record<plane,dplane_t>(level *l, dplane_t &r) {
        return plane(vec3(r.normal), r.dist);
    }
    
    template <>
    inline texinfo conv_record<texinfo,texinfo_t>(level *l, texinfo_t &r) {
        /// \todo Fill in more fields.
        return texinfo(r.value, r.texture); 
    }
    
    template <>
    inline edge conv_record<edge,dedge_t>(level *l, dedge_t &r) {
        return edge(l->get_vertex(r.v[0]), l->get_vertex(r.v[1]));
    }
    
    // LUMP_SURFEDGES
    template <>
    inline edge *conv_record<edge*,int>(level *l, int &r) {
        return l->get_edge(r);
    }
    
    template <>
    inline face conv_record<face,dface_t>(level *l, dface_t &r) {
        return face(l->get_plane(r.planenum),
                    r.side ? face::PLANE_BACK : face::PLANE_FRONT,
                    r.numedges,
                    l->get_surfedge_list(r.numedges, r.firstedge),
					l->get_texinfo(r.texinfo));
    }
    
    // LUMP_LEAFFACES
    template <> inline face *
    conv_record<face*,unsigned short>(level *l, unsigned short &r) {
        return l->get_face(r);
    }
    
    template <> inline brushside
    conv_record<brushside,dbrushside_t>(level *l, dbrushside_t &r) {
        return brushside(l->get_plane(r.planenum), l->get_texinfo(r.texinfo));
    }
    
    template <>
    inline brush conv_record<brush,dbrush_t>(level *l, dbrush_t &r) {
        return brush(r.numsides,
                     l->get_brushside_list(r.numsides, r.firstside));
    }
    
    // LUMP_LEAFBRUSHES
    template <> inline brush *
    conv_record<brush*,unsigned short>(level *l, unsigned short &r) {
        return l->get_brush(r);
    }
    
    template <>
    inline leaf conv_record<leaf,dleaf_t>(level *l, dleaf_t &r) {
        return leaf(r.cluster, l->get_area(r.area),
                    r.numleafbrushes,
                    l->get_leafbrush_list(r.numleafbrushes, r.firstleafbrush),
                    r.numleaffaces,
                    l->get_leafface_list(r.numleaffaces, r.firstleafface));
    }
    
    template <>
    inline node conv_record<node,dnode_t>(level *l, dnode_t &r) {
        return node(); /// \todo Implement.
    }
    
    template <>
    inline model conv_record<model,dmodel_t>(level *l, dmodel_t &r) {
        return model(); /// \todo Implement.
    }
};


/// Get the base and size of the specified BSP lump.
///
/// \param lump_id The LUMP_* value for the lump to get.
/// \param out_base A pointer to the lump.
/// \param out_size The number of T objects in this lump.
template <typename T>
void level::get_lump(size_t lump_id, T *&out_base, size_t &out_size) {
    unsigned char *data = &m_raw_data[0];
    dheader_t *header = reinterpret_cast<dheader_t *>(data);
    lump_t lump = header->lumps[lump_id];
    out_base = reinterpret_cast<T*>(data + lump.fileofs);
    if (lump.filelen % sizeof(T) != 0)
        throw std::runtime_error("BSP lump has invalid size");
    out_size = lump.filelen / sizeof(T);
}

/// Load all records of the specified type into our level object.  In the
/// process, we convert the raw records into high-level objects.
///
/// \param lump_id The lump from which to read the records.
/// \param out_objs The vector in which to store the 
template <typename RecordType, typename ObjectType>
void level::load_records(size_t lump_id, vector<ObjectType> &out_objs) {
    RecordType *records;
    size_t count;
    get_lump(lump_id, records, count);
    out_objs.reserve(count);
    for (size_t i = 0; i < count; i++)
        out_objs.push_back(conv_record<ObjectType,RecordType>(this, records[i]));
}

void level::load_pvs_phs() {

}

void level::load_areas_and_portals() {

}

void level::load_entities() {
    char *entity_str;
    size_t length;
    get_lump(LUMP_ENTITIES, entity_str, length);
}

/// Read a level from disk.
///
/// \param filename The relative path of the file to read,
/// including the *.BSP extension.
/// \callgraph
level::level(const string &filename) {
    // Open the *.BSP file.
    std::ifstream file(filename.c_str(), std::ios::binary);

    // Load the actual level data.
    load_records<dvertex_t>(LUMP_VERTEXES, m_vertices);
    load_pvs_phs();
    load_areas_and_portals();
    load_records<dplane_t>(LUMP_PLANES, m_planes);
    load_records<texinfo_t>(LUMP_TEXINFO, m_texinfos);
    load_records<dedge_t>(LUMP_EDGES, m_edges);
    load_records<int>(LUMP_SURFEDGES, m_surfedges);
    load_records<dface_t>(LUMP_FACES, m_faces);
    load_records<unsigned short>(LUMP_LEAFFACES, m_leaffaces);
    load_records<dbrushside_t>(LUMP_BRUSHSIDES, m_brushsides);
    load_records<dbrush_t>(LUMP_BRUSHES, m_brushes);
    load_records<unsigned short>(LUMP_LEAFBRUSHES, m_leafbrushes);
    load_records<dleaf_t>(LUMP_LEAFS, m_leafs);
    load_records<dnode_t>(LUMP_NODES, m_nodes);
    load_records<dmodel_t>(LUMP_MODELS, m_models);
    load_entities();
}

/*
#define	LUMP_ENTITIES		0
#define	LUMP_PLANES			1
#define	LUMP_VERTEXES		2
#define	LUMP_VISIBILITY		3
#define	LUMP_NODES			4
#define	LUMP_TEXINFO		5
#define	LUMP_FACES			6
#define	LUMP_LIGHTING		7
#define	LUMP_LEAFS			8
#define	LUMP_LEAFFACES		9
#define	LUMP_LEAFBRUSHES	10
#define	LUMP_EDGES			11
#define	LUMP_SURFEDGES		12
#define	LUMP_MODELS			13
#define	LUMP_BRUSHES		14
#define	LUMP_BRUSHSIDES		15
#define	LUMP_POP			16
#define	LUMP_AREAS			17
#define	LUMP_AREAPORTALS	18
#define	HEADER_LUMPS		19
*/
