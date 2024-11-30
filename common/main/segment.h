/*
 * Portions of this file are copyright Rebirth contributors and licensed as
 * described in COPYING.txt.
 * Portions of this file are copyright Parallax Software and licensed
 * according to the Parallax license below.
 * See COPYING.txt for license details.

THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/

/*
 *
 * Include file for functions which need to access segment data structure.
 *
 */

#pragma once

#include <compare>
#include <physfs.h>
#include <type_traits>
#include "pstypes.h"
#include "maths.h"
#include "vecmat.h"
#include "dxxsconf.h"
#include "dsx-ns.h"

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include "fwd-segment.h"
#include "countarray.h"
#include "valptridx.h"
#include "objnum.h"
#include "pack.h"

#ifdef dsx
namespace dsx {
// Returns true if segnum references a child, else returns false.
// Note that -1 means no connection, -2 means a connection to the outside world.
static inline bool IS_CHILD(segnum_t s)
{
	return s != segment_none && s != segment_exit;
}
}
#endif

namespace dcx {

static constexpr sidenum_t &operator++(sidenum_t &a)
{
	return a = static_cast<sidenum_t>(static_cast<unsigned>(a) + 1);
}

static constexpr side_relative_vertnum &operator++(side_relative_vertnum &a)
{
	return a = static_cast<side_relative_vertnum>(static_cast<uint8_t>(a) + 1);
}

//Structure for storing u,v,light values.
//NOTE: this structure should be the same as the one in 3d.h
struct uvl
{
	fix u, v, l;
};

enum class side_type : uint8_t
{
	quad = 1,	// render side as quadrilateral
	tri_02 = 2,	// render side as two triangles, triangulated along edge from 0 to 2
	tri_13 = 3,	// render side as two triangles, triangulated along edge from 1 to 3
};

enum class side_relative_vertnum : uint8_t
{
	_0,
	_1,
	_2,
	_3,
};

enum class segment_relative_vertnum : uint8_t
{
	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
};

enum class segment_special : uint8_t
{
	nothing,
	fuelcen,
	repaircen,
	controlcen,
	robotmaker,
	goal_blue,
	goal_red,
};

enum class materialization_center_number : uint8_t
{
	None = 0xff,
};

enum class station_number : uint8_t
{
	None = 0xff,
};

enum segnum_t : uint16_t
{
};

enum class vertnum_t : uint32_t
{
};

enum class wallnum_t : uint16_t
{
	None = 0xffff,
};

struct shared_side
{
	struct illegal_type;
	side_type m_type;           // replaces num_faces and tri_edge, 1 = quad, 2 = 0:2 triangulation, 3 = 1:3 triangulation
	const side_type &get_type() const { return m_type; }
	void set_type(side_type t) { m_type = t; }
	wallnum_t wall_num;
	std::array<vms_vector, 2> normals;  // 2 normals, if quadrilateral, both the same.
};

enum class sidenum_t : uint8_t
{
	WLEFT = 0,
	WTOP = 1,
	WRIGHT = 2,
	WBOTTOM = 3,
	WBACK = 4,
	WFRONT = 5
};

enum class sidemask_t : uint8_t
{
	left =	1u << static_cast<uint8_t>(sidenum_t::WLEFT),
	top =	1u << static_cast<uint8_t>(sidenum_t::WTOP),
	right =	1u << static_cast<uint8_t>(sidenum_t::WRIGHT),
	bottom =	1u << static_cast<uint8_t>(sidenum_t::WBOTTOM),
	back =	1u << static_cast<uint8_t>(sidenum_t::WBACK),
	front =	1u << static_cast<uint8_t>(sidenum_t::WFRONT),
};

static constexpr uint8_t operator&(const sidemask_t a, const sidemask_t b)
{
	return static_cast<uint8_t>(a) & static_cast<uint8_t>(b);
}

static constexpr sidemask_t operator~(const sidemask_t a)
{
	return static_cast<sidemask_t>(~static_cast<uint8_t>(a));
}

static constexpr sidemask_t &operator|=(sidemask_t &a, const sidemask_t b)
{
	return a = static_cast<sidemask_t>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

static constexpr sidemask_t build_sidemask(const sidenum_t s)
{
	return static_cast<sidemask_t>(1u << static_cast<uint8_t>(s));
}

enum class sound_ambient_flags : uint8_t
{
	None = 0,
	water = 1,
	lava = 2,
	water_and_lava = water | lava,
};

enum class texture1_value : uint16_t
{
	None,
};

/* texture2_value uses the low 14 bits for the array index and the high
 * 2 bits for texture2_rotation_high
 */
enum class texture2_value : uint16_t
{
	None,
};

enum class texture2_rotation_low : uint8_t
{
	Normal = 0,
	_1 = 1,
	_2 = 2,
	_3 = 3,
};

#define TEXTURE2_ROTATION_SHIFT	14u
#define TEXTURE2_ROTATION_INDEX_MASK	((1u << TEXTURE2_ROTATION_SHIFT) - 1u)

enum class texture2_rotation_high : uint16_t
{
	Normal = 0,
	_1 = static_cast<uint16_t>(texture2_rotation_low::_1) << TEXTURE2_ROTATION_SHIFT,
	_2 = static_cast<uint16_t>(texture2_rotation_low::_2) << TEXTURE2_ROTATION_SHIFT,
	_3 = static_cast<uint16_t>(texture2_rotation_low::_3) << TEXTURE2_ROTATION_SHIFT,
};

static constexpr texture2_rotation_high &operator++(texture2_rotation_high &t)
{
	/* Cast to uint32, step to the next value, cast back.  The cast back
	 * will truncate to the low 16 bits, causing
	 * texture2_rotation_high::_3 to roll over to
	 * texture2_rotation_high::Normal.
	 */
	return (t = static_cast<texture2_rotation_high>(static_cast<uint32_t>(t) + (1u << TEXTURE2_ROTATION_SHIFT)));
}

static constexpr texture_index get_texture_index(const texture1_value t)
{
	return static_cast<texture_index>(t);
}

static constexpr texture_index get_texture_index(const texture2_value t)
{
	return static_cast<texture_index>(static_cast<uint16_t>(t) & TEXTURE2_ROTATION_INDEX_MASK);
}

static constexpr texture2_rotation_high get_texture_rotation_high(const texture2_value t)
{
	return static_cast<texture2_rotation_high>(static_cast<uint16_t>(t) & ~TEXTURE2_ROTATION_INDEX_MASK);
}

static constexpr texture2_rotation_low get_texture_rotation_low(const texture2_rotation_high t)
{
	return static_cast<texture2_rotation_low>(static_cast<uint16_t>(t) >> TEXTURE2_ROTATION_SHIFT);
}

static constexpr texture2_rotation_low get_texture_rotation_low(const texture2_value t)
{
	return get_texture_rotation_low(get_texture_rotation_high(t));
}

static constexpr texture1_value build_texture1_value(const texture_index t)
{
	return static_cast<texture1_value>(t);
}

static constexpr texture2_value build_texture2_value(const texture_index t, const texture2_rotation_high rotation)
{
	return static_cast<texture2_value>(static_cast<uint16_t>(t) | static_cast<uint16_t>(rotation));
}

#undef TEXTURE2_ROTATION_SHIFT
#undef TEXTURE2_ROTATION_INDEX_MASK

[[nodiscard]]
static constexpr side_relative_vertnum next_side_vertex(const side_relative_vertnum a, unsigned add = 1)
{
	return static_cast<side_relative_vertnum>((static_cast<uint8_t>(a) + add) & 3);
}

struct unique_side
{
	texture1_value tmap_num;
	texture2_value tmap_num2;
	enumerated_array<uvl, 4, side_relative_vertnum>     uvls;
};

#ifdef dsx
struct shared_segment
{
#if DXX_USE_EDITOR
	segnum_t   segnum;     // segment number, not sure what it means
	short   group;      // group number to which the segment belongs.
#endif
	per_side_array<segnum_t> children;    // indices of 6 children segments, front, left, top, right, bottom, back
	enumerated_array<vertnum_t, MAX_VERTICES_PER_SEGMENT, segment_relative_vertnum> verts;    // vertex ids of 4 front and 4 back vertices
	segment_special special;    // what type of center this is
	materialization_center_number matcen_num; // which center segment is associated with.
	station_number station_idx;
	/* if DXX_BUILD_DESCENT_II */
	sound_ambient_flags s2_flags;
	/* endif */
	per_side_array<shared_side> sides;
};

struct unique_segment
{
	objnum_t objects;    // pointer to objects in this segment
	//      If bit n (1 << n) is set, then side #n in segment has had light subtracted from original (editor-computed) value.
	sidemask_t light_subtracted;
	/* if DXX_BUILD_DESCENT_II */
	sidemask_t slide_textures;
	/* endif */
	fix     static_light;
	per_side_array<unique_side> sides;
};

struct segment : unique_segment, shared_segment
{
};

template <typename S, typename U>
struct susegment
{
	S &s;
	U &u;
	constexpr susegment(const susegment &) = default;
	constexpr susegment(susegment &&) = default;
	template <typename T>
		requires(std::is_convertible<T &&, S &>::value && std::is_convertible<T &&, U &>::value)
		constexpr susegment(T &&t) :
			s(t), u(t)
	{
	}
	operator S &() const
	{
		return s;
	}
	operator U &() const
	{
		return u;
	}
};
#endif

struct count_segment_array_t : public count_array_t<segnum_t, MAX_SEGMENTS> {};

struct group
{
	struct segment_array_type_t : public count_segment_array_t {};
	struct vertex_array_type_t : public count_array_t<vertnum_t, MAX_VERTICES> {};
	segment_array_type_t segments;
	vertex_array_type_t vertices;
	void clear()
	{
		segments.clear();
		vertices.clear();
	}
};

#ifdef dsx
#define Highest_segment_index static_cast<segnum_t>(Segments.get_count() - 1)
DXX_VALPTRIDX_DEFINE_GLOBAL_FACTORIES(segment, seg, Segments);
#endif

// Globals from mglobal.c
struct vertex : vms_vector
{
	vertex() = default;
	vertex(const fix &a, const fix &b, const fix &c) :
		vms_vector{a, b, c}
	{
	}
	explicit vertex(const vms_vector &v) :
		vms_vector(v)
	{
	}
};

static constexpr vertnum_t operator++(vertnum_t &v)
{
	return (v = static_cast<vertnum_t>(static_cast<unsigned>(v) + 1));
}

}

#ifdef dsx
struct shared_side::illegal_type : std::runtime_error
{
	const shared_segment *const m_segment;
	const shared_side *const m_side;
	illegal_type(const shared_segment &seg, const shared_side &s) :
		runtime_error("illegal side type"),
		m_segment(&seg), m_side(&s)
	{
	}
	illegal_type(const shared_side &s) :
		runtime_error("illegal side type"),
		m_segment(nullptr), m_side(&s)
	{
	}
};

namespace dsx {

#if DXX_BUILD_DESCENT == 2
enum class delta_light_index : uint16_t
{
};

// New stuff, 10/14/95: For shooting out lights and monitors.
// Light cast upon vert_light vertices in segnum:sidenum by some light
struct delta_light : prohibit_void_ptr<delta_light>
{
	segnum_t   segnum;
	sidenum_t  sidenum;
	enumerated_array<uint8_t, 4, side_relative_vertnum> vert_light;
};

// Light at segnum:sidenum casts light on count sides beginning at index (in array Delta_lights)
struct dl_index {
	segnum_t   segnum;
	sidenum_t  sidenum;
	uint8_t count;
	delta_light_index index;
	constexpr std::strong_ordering operator<=>(const dl_index &rhs) const
	{
		if (const auto r = segnum <=> rhs.segnum; r != std::strong_ordering::equal)
			return r;
		return sidenum <=> rhs.sidenum;
	}
	constexpr bool operator==(const dl_index &) const = default;
};

struct d_level_shared_destructible_light_state
{
	valptridx<dl_index>::array_managed_type Dl_indices;
	d_delta_light_array Delta_lights;
};
#endif

}
#endif

namespace dcx {

#ifdef dsx
struct d_level_shared_vertex_state
{
#if DXX_USE_EDITOR
	/* Number of elements in Vertex_active which have a nonzero value.
	 * This can be less than the index of the highest defined vertex if
	 * there are unused vertices.
	 */
	unsigned Num_vertices;
#endif
private:
	vertex_array Vertices;
#if DXX_USE_EDITOR
	enumerated_array<uint8_t, MAX_VERTICES, vertnum_t> Vertex_active; // !0 means vertex is in use, 0 means not in use.
#endif
public:
	auto &get_vertices()
	{
		return Vertices;
	}
	const auto &get_vertices() const
	{
		return Vertices;
	}
#if DXX_USE_EDITOR
	auto &get_vertex_active()
	{
		return Vertex_active;
	}
	const auto &get_vertex_active() const
	{
		return Vertex_active;
	}
#endif
};

struct d_level_shared_segment_state
{
	unsigned Num_segments;
	d_level_shared_vertex_state LevelSharedVertexState;
	auto &get_segments()
	{
		return Segments;
	}
	const auto &get_segments() const
	{
		return Segments;
	}
	auto &get_vertex_state()
	{
		return LevelSharedVertexState;
	}
	const auto &get_vertex_state() const
	{
		return LevelSharedVertexState;
	}
};

struct d_level_unique_automap_state
{
	enumerated_array<uint8_t, MAX_SEGMENTS, segnum_t> Automap_visited;
};

struct d_level_unique_segment_state
{
	auto &get_segments()
	{
		return Segments;
	}
	const auto &get_segments() const
	{
		return Segments;
	}
};

extern d_level_unique_automap_state LevelUniqueAutomapState;
extern d_level_unique_segment_state LevelUniqueSegmentState;
#endif

template <unsigned bits>
requires(
	/* For every segment, require that the bits of the mask for that segment
	 * all reside in a single byte.  Since a byte is 8 bits wide, this
	 * constrains `bits` to be 1, 2, 4, or 8.  Any other value would mean that
	 * some segments have a mask that straddles 2 bytes.  Current logic would
	 * not handle a cross-byte value correctly, so disallow such attempts.
	 *
	 * A mask of 8 is not useful, since the caller could use
	 * `enumerated_array<uint8_t, MAX_SEGMENTS, segnum_t>` for that and avoid
	 * the shifting/masking logic on assignment.  Therefore, exclude
	 * `bits == 8`, even though the class should work correctly in that case.
	 */
	bits == 1 || bits == 2 || bits == 4
)
class visited_segment_mask_t
{
	/* For a segment with index `n`, the mask for that segment is stored in
	 * `a[n / divisor]`.
	 */
	static constexpr unsigned divisor{8u / bits};
	static constexpr unsigned bitmask_low_aligned{(1u << bits) - 1u};
	using array_t = std::array<uint8_t, (MAX_SEGMENTS + (divisor - 1)) / divisor>;
	/* Initialize `a` so that the storage is cleared. */
	array_t a{};
	/* The shift count depends on `bits`, but not on the type of the data being
	 * tracked, so store it in a base class, to avoid redundant instantiations.
	 */
	struct maskproxy_shift_count_type
	{
		const unsigned shift;
		typename array_t::value_type mask() const
		{
			return bitmask_low_aligned << shift;
		}
		constexpr maskproxy_shift_count_type(const unsigned s) :
			shift{s * bits}
		{
		}
	};
	template <typename byte_type>
	struct maskproxy_assignable_type : maskproxy_shift_count_type
	{
		using maskproxy_shift_count_type::shift;
		byte_type &byte;
		constexpr maskproxy_assignable_type(const unsigned s, byte_type &byte) :
			maskproxy_shift_count_type{s}, byte{byte}
		{
		}
		constexpr operator uint8_t() const
		{
			return (byte >> shift) & bitmask_low_aligned;
		}
		/* For the special case that the mask is 1 bit wide, accept only
		 * `bool`.  This version can avoid clearing the mask bits before
		 * assigning them, since `(old_value | true) == true`, and
		 * `(old_value & false) == false`.
		 */
		maskproxy_assignable_type &operator=(const bool value)
			requires(bits == 1)
		{
			const auto m{this->mask()};
			if (value)
				byte |= m;
			else
				byte &= ~m;
			return *this;
		}
		/* For the general case that the mask is not 1 bit wide, the bits to be
		 * written must be cleared before the new value is bitwise-or'd in,
		 * since `(old_value | 2) == 2` is not guaranteed.
		 */
		maskproxy_assignable_type &operator=(const uint8_t u)
			requires(bits != 1)
		{
			assert(!(u & ~bitmask_low_aligned));
			byte = (byte & ~this->mask()) | (u << shift);
			return *this;
		}
	};
	/* clang Class Template Argument Deduction fails to deduce from the
	 * available non-template constructor.  Define an explicit deduction guide
	 * to tell it what it should have implicitly deduced on its own.
	 *
	 * - [gcc-9, gcc-14]: succeed without this
	 * - [clang-14, clang-17]: fail without this
	 * - (clang-17, ...): untested
	 */
	template <typename byte_type>
		maskproxy_assignable_type(unsigned, byte_type &) -> maskproxy_assignable_type<byte_type>;
	static constexpr auto make_maskproxy(auto &a, const segnum_t s)
	{
		const auto segnum{static_cast<std::underlying_type_t<segnum_t>>(s)};
		return maskproxy_assignable_type{{segnum % divisor}, a.at({segnum / divisor})};
	}
public:
	auto operator[](const segnum_t segnum)
	{
		return make_maskproxy(a, segnum);
	}
	auto operator[](const segnum_t segnum) const
	{
		return make_maskproxy(a, segnum);
	}
};

using visited_segment_bitarray_t = visited_segment_mask_t<1>;

#ifdef dsx
imsegidx_t read_untrusted_segnum_le16(NamedPHYSFS_File fp);
imsegidx_t read_untrusted_segnum_le32(NamedPHYSFS_File fp);
imsegidx_t read_untrusted_segnum_xe16(NamedPHYSFS_File fp, physfsx_endian swap);
imsegidx_t read_untrusted_segnum_xe32(NamedPHYSFS_File fp, physfsx_endian swap);
#endif

}

#ifdef dsx
namespace dsx {

#if DXX_BUILD_DESCENT == 2
struct d_level_shared_segment_state : ::dcx::d_level_shared_segment_state
{
	d_level_shared_destructible_light_state DestructibleLights;
	segnum_t Secret_return_segment;
	vms_matrix Secret_return_orient;
};
#endif

extern d_level_shared_segment_state LevelSharedSegmentState;

}
#endif
