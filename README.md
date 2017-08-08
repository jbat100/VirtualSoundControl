# NOTE:

This project has transitioned to sonosthesia, which changed the approach to connecting existing software using plugins, scripts and extensions (see www.sonosthesia.com/home) 

# Background

VirtualSoundControl is a project in early stages of development aiming to provide a set of tools for the control of sound synthesis through the manipulation of virtual objects, in a 3D virtual reality / game-like setting. Currently in pre-alpha (prototyping) stage, in heavy development.

Flexibility and configurability are primary objectives. The main interaction paradigm currently in development is the triggering of MIDI/OSC signals by virtual object collisions. Currently only rigid-body dynamics are used to detect collisions but there are future plans to include soft-body dynamics also. 

Collisions are described by a number of parameters, for example:
- relative/absolute speed of the objects in collision.
- distance of the collision point to another, fixed or moving point.
- color/texture attributes of the colliding objects at the collision point.
- many others …

Each object in the virtual world has a collision action chain which describes how it reacts to collisions, for example it can send note/control signals and trigger envelopes (currently using MIDI/OSC but the engine could be directly incorporated in software synthesisers to control sound synthesis directly). Different collision parameters described above can be mapped to parameters in the sound domain (note channel, pitch and velocity, control value, envelope amplitude and time stretch etc…).

# Dependencies

The project is written in portable C++, at least as portable as the libraries it uses, which are listed in this section. All the libraries have been chosen for their recognised performance and stability and, most importantly, their permissive licences (BSD/MIT style) which would allow this project to be used in commercial projects without the usual problems associated with GPL/LGPL style licenses. 

- Boost Libraries: used for smart pointers, threads, asserts, foreach and probably later for the bind, signals and other cool features. Currently no C++ 11 features are used, their Boost equivalents are used instead, when needed.   
- OGRE: used for graphics and 3D maths when needed.
- Bullet: used for physics simulations, particularly collision detections.
- STK: used RtMidi for portable MIDI code (and possibly later audio also). 

An OSX interface layer is currently being written on top of the C++ core, using the Cocoa frameworks, in Objective-C++. This interface layer is being kept as separate from the core as possible so that other interface layers can be developed for different environments (GTK, .NET, iOS, Android etc…).

# File Hierarchy

### Apple

Apple specific files, UI and MVC support classes, Objective-C++ mostly.

### Application

Top level application-managing classes, virtual sound control environments, tests, and related classes.

### Envelope

Abstractions for a envelopes, can be used for MIDI/OSC or audio. 

### Interface

Cross-platform UI abstractions, key bindings, template responder-chains.

### Mapping

IM Stands for Interface-Mapping. Classes which handle the translation from the virtual object domain to the sound synthesis control domain live here.

### MIDI

Layer on top of RtMidi, to make things like sending note on/off, control, pitch bend (etc…) messages easier. Also provides higher-level abstractions for midi input/output management. 

### OB

Stands for Ogre-Bullet. Layer on top of the Ogre and Bullet C++ libraries (and the OgreBullet wrapper) which gives high level tools for creating scenes, elements (objects), running dynamics and collision detection. 

### Sound

Attempt at an audio sample rate sound synthesiser based on STK. Currently stopped dev on this in favour of sending MIDI/OSC messages to existing synthesisers.

### Task

A set of tools used for performing threaded, queued, timed, tasks (with stepped execution) in the background.  

