# Physics & Collision System

This document outlines the architecture of the basic physics system used in BlankC Engine. This system was intentionally kept simple for efficiency and ease of modification, focusing on movement dynamics and Bounding Box collision detection.

## 1. Physics Architecture

BlankC Engine's physics system is not a full-fledged physics engine like PhysX or Bullet. It is a *custom* physics system handled in the `physics.c` module.

Main components:
1.  **PhysicsWorld:** The main manager that stores a list of all active physical entities in the world, as well as global parameters like the gravity vector.
2.  **RigidBody:** A component attached to an object that gives it physical properties. It stores position, velocity, spatial size (for collisions), and status (*static* or *dynamic*).

## 2. Physics Update Loop

Physics updates occur within the `physics_world_update()` function, which is called once per *frame* (controlled by delta time).

The update process works in three main stages for each dynamic *RigidBody* (`is_static == 0`):

1.  **Force Integration (Gravity):**
    The *velocity* of the *RigidBody* is increased by the gravity acceleration multiplied by the delta time.
    `Velocity = Velocity + (Gravity * DeltaTime)`
2.  **Position Integration:**
    The *RigidBody* position is updated based on the current velocity multiplied by delta time.
    `Position = Position + (Velocity * DeltaTime)`
3.  **Collision Resolution:**
    After the new position is calculated, the system checks if this new position causes the *RigidBody* to overlap with another *RigidBody*.

## 3. AABB Collision Detection

BlankC Engine uses **AABB (Axis-Aligned Bounding Box)** collisions. An AABB is a pure 3D box whose sides are always parallel to the X, Y, and Z axes (it cannot be tilted/rotated). This makes it extremely fast to compute.

### Collision Mechanism

If two AABBs overlap (`test_aabb_overlap`):
1. The system calculates the **MTV (Minimum Translation Vector)**. The MTV is the smallest vector required to separate the two colliding boxes.
2. If a moving object collides with a **static** object (wall, floor): 
   - The moving object is pushed out by the MTV amount.
   - Velocity on the collision axis is zeroed out to stop forced movement (preventing the object from continuing to try and push through).
3. If a moving object collides with another **dynamic** object:
   - Both objects are pushed away from each other, each taking half of the MTV value.

## 4. Limitations

Due to its simple nature, this system has several important limitations that programmers should be aware of:

- **Tunneling:** Objects moving at very high speeds can pass through thin walls in a single frame (because checking only occurs at the start and end positions of the frame, not *Continuous Collision Detection*). Solution: use thick walls or limit the maximum speed of objects.
- **No Physical Rotation:** AABB boxes do not respond to rotation. If you visually rotate a model in the scene, its collision box remains unchanged. This system does not yet support OBB (Oriented Bounding Box) or *mesh*-based collisions.
- **No Friction/Bounciness:** The system currently does not simulate surface friction or bounciness. All objects will slide or come to a complete stop based solely on manual velocity modifications.