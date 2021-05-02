#pragma once
enum SolidType_t {
	SOLID_NONE = 0,	// no solid model
	SOLID_BSP = 1,	// a BSP tree
	SOLID_BBOX = 2,	// an AABB
	SOLID_OBB = 3,	// an OBB (not implemented yet)
	SOLID_OBB_YAW = 4,	// an OBB, constrained so that it can only yaw
	SOLID_CUSTOM = 5,	// Always call into the entity for tests
	SOLID_VPHYSICS = 6,	// solid vphysics object, get vcollide from the model and collide with that
	SOLID_LAST,
};

enum SolidFlags_t {
	FSOLID_CUSTOMRAYTEST = 0x0001,	// Ignore solid type + always call into the entity for ray tests
	FSOLID_CUSTOMBOXTEST = 0x0002,	// Ignore solid type + always call into the entity for swept box tests
	FSOLID_NOT_SOLID = 0x0004,	// Are we currently not solid?
	FSOLID_TRIGGER = 0x0008,	// This is something may be collideable but fires touch functions
											// even when it's not collideable (when the FSOLID_NOT_SOLID flag is set)
											FSOLID_NOT_STANDABLE = 0x0010,	// You can't stand on this
											FSOLID_VOLUME_CONTENTS = 0x0020,	// Contains volumetric contents (like water)
											FSOLID_FORCE_WORLD_ALIGNED = 0x0040,	// Forces the collision rep to be world-aligned even if it's SOLID_BSP or SOLID_VPHYSICS
											FSOLID_USE_TRIGGER_BOUNDS = 0x0080,	// Uses a special trigger bounds separate from the normal OBB
											FSOLID_ROOT_PARENT_ALIGNED = 0x0100,	// Collisions are defined in root parent's local coordinate space
											FSOLID_TRIGGER_TOUCH_DEBRIS = 0x0200,	// This trigger will touch debris objects
											FSOLID_TRIGGER_TOUCH_PLAYER = 0x0400,	// This trigger will touch only players
											FSOLID_NOT_MOVEABLE = 0x0800,	// Assume this object will not move

											FSOLID_MAX_BITS = 12
};


class collideable_t {
public:
private:
	virtual void vtpad_0( void ) = 0;

public:
	virtual vec3_t& mins( void ) = 0;
	virtual vec3_t& maxs( void ) = 0;


	// Returns the bounds of a world-space box used when the collideable is being traced
	// against as a trigger. It's only valid to call these methods if the solid flags
	// have the FSOLID_USE_TRIGGER_BOUNDS flag set.
	virtual void			WorldSpaceTriggerBounds ( vec3_t * pVecWorldMins, vec3_t * pVecWorldMaxs ) const = 0;

	// custom collision test
	virtual bool			TestCollision ( const ray_t & ray, unsigned int fContentsMask, trace_t & tr ) = 0;

	// Perform hitbox test, returns true *if hitboxes were tested at all*!!
	virtual bool			TestHitboxes ( const ray_t & ray, unsigned int fContentsMask, trace_t & tr ) = 0;

	// Returns the BRUSH model index if this is a brush model. Otherwise, returns -1.
	virtual int				GetCollisionModelIndex ( ) = 0;

	// Return the model, if it's a studio model.
	virtual const model_t * GetCollisionModel ( ) = 0;

	// Get angles and origin.
	virtual const vec3_t & GetCollisionOrigin ( ) const = 0;
	virtual const vec3_t & GetCollisionAngles ( ) const = 0;
	virtual const matrix3x4_t & CollisionToWorldTransform ( ) const = 0;

	// Return a SOLID_ define.
	virtual SolidType_t		GetSolid ( ) const = 0;
	virtual int				GetSolidFlags ( ) const = 0;

	// Gets at the containing class...
	virtual void * GetIClientUnknown ( ) = 0;

	// We can filter out collisions based on collision group
	virtual int				GetCollisionGroup ( ) const = 0;

	// Returns a world-aligned box guaranteed to surround *everything* in the collision representation
	// Note that this will surround hitboxes, trigger bounds, physics.
	// It may or may not be a tight-fitting box and its volume may suddenly change
	virtual void			WorldSpaceSurroundingBounds ( vec3_t * pVecMins, vec3_t * pVecMaxs ) = 0;

	virtual UINT			GetRequiredTriggerFlags ( ) const = 0;

	// returns NULL unless this collideable has specified FSOLID_ROOT_PARENT_ALIGNED
	virtual const matrix3x4_t * GetRootParentToWorldTransform ( ) const = 0;
	virtual void * GetVPhysicsObject ( ) const = 0;
};