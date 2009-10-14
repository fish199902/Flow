#ifndef EFFECTS_H_
#define EFFECTS_H_

#warning CLEANUP: Check if everything here is implemented

#define BIT(x) ((int)1 << (x))

// Global effects - Modify the appearence of everything
/// Make the game look like 8-bit (except still in 3D).
#define FX_PIXELIZE BIT(0)

/// Use DOF to make the world feel tiny.
#define FX_MACRO BIT(1)

/// Apply distortions, giving the appearance of underwater.
#define FX_WATER BIT(2)

/// Use less lighting, with blue ambiance, giving a night effect.
#define FX_NIGHT BIT(3)


// Level piece effects -- Modify the appearance of a single piece

/// Make the object luminescent.
#define FX_GLOW BIT(0)

/// Make the object invisible.
#define FX_INVISIBLE BIT(1)

/// Make the object shiney.
#define FX_GLOSS BIT(2)

/// Make the object partially transparent.
#define FX_TRANSPARENT BIT(3)


// Piece modifiers -- Change how a level piece behaves

/// Player does not collide with object.
#define MOD_PHASED BIT(0)

/// Contact with this piece ends the level.
#define MOD_FINISH BIT(1)

/// Contact with this piece adds volume.
#define MOD_ADD_VOLUME BIT(2)

/// Contact with this piece subtracts volume.
#define MOD_SUB_VOLUME BIT(3)

/// This piece will merge with the player when contacted
#define MOD_MERGE_LIQUID BIT(4)

/// This piece is not fixed in place
#define MOD_DYNAMIC BIT(6)

/// This piece is a liquid, implies MOD_DYNAMIC
#define MOD_LIQUID BIT(5)|MOD_DYNAMIC

#endif // EFFECTS_H_
