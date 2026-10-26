# Aethra prototype

A playable browser prototype of Aethra, a shared virtual universe with face-scanned avatars, street racing and elemental powers. This folder is separate from the Northpine marketing files in the rest of the repo.

The browser version tests gameplay. The realistic product (photoreal city, MetaHuman faces from a phone scan) is planned in Unreal Engine 5; see the design document and `../aethra-unreal/`.

## Play

Open `index.html` in a modern browser. It loads Three.js r128 and three add-ons (GLTFLoader, SkeletonUtils, Sky) from public CDNs, and the two characters from `models/`. It works with a keyboard and mouse, a touchscreen or a gamepad. Progress saves in the browser under the key `aethra-save-v1`.

## What's in it

- Two motion-captured characters (woman or man) that idle, walk, run and dance
- Neon Haven in late-afternoon daylight: sun shadows, sky reflections, glass, brick, stone and concrete buildings, asphalt roads with markings, street lights and trees
- Moving traffic you can stop and take, parked cars, pedestrians and a street party in the plaza
- Street race against 3 rivals, with Shard stakes and Clean or Power mode
- Five elements (Fire, Air, Ice, Earth, Lightning) that unlock at levels 1–5 and rank up to 5
- Rogue drones (levels 1–10); beating ones above your level pays up to 3× XP
- Garage for power ranks, car paint and engine upgrades

Shards are earned only. Nothing in the prototype uses real money.

## Character files

`models/soldier.js` and `models/michelle.js` are the Soldier and Michelle `.glb` models from the three.js examples, stored as base64 so they load as scripts. Both are Mixamo characters. Mixamo allows use in projects but not redistribution as standalone files; replace them with Aethra's own characters before any public release.
