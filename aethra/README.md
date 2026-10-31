# Aethra prototype

A playable browser prototype of Aethra, a shared virtual universe with face-scanned avatars, street racing and elemental powers. This folder is separate from the Northpine marketing files in the rest of the repo.

The browser version tests gameplay. The realistic product (photoreal city, MetaHuman faces from a phone scan) is planned in Unreal Engine 5; see the design document and `../aethra-unreal/`.

## Play

Open `index.html` in a modern browser. It loads Three.js r128 and three add-ons (GLTFLoader, SkeletonUtils, Sky) from public CDNs, and the two characters from `models/`. It works with a keyboard and mouse, a touchscreen or a gamepad. Progress saves in the browser under the key `aethra-save-v1`.

## What's in it

- Two motion-captured characters (woman or man) that idle, walk, run and dance
- Neon Haven in late-afternoon daylight: sun shadows, sky reflections, glass, brick, stone and concrete buildings, asphalt roads with markings, street lights and trees
- Moving traffic you can stop and take, parked cars, pedestrians and a street party in the plaza
- Shop fronts with signs and awnings, bus shelters, bins, hydrants and plaza bollards
- Solid world: people, cars, drones and projectiles collide with buildings, tree trunks, street lights, street furniture, the fountain, other people and car bodies; the camera stays out of walls
- Street race against 3 rivals, with Shard stakes and Clean or Power mode
- Five elements (Fire, Air, Ice, Earth, Lightning) that unlock at levels 1–5 and rank up to 5
- Rogue drones (levels 1–10); beating ones above your level pays up to 3× XP
- Garage for power ranks, car paint and engine upgrades

Shards are earned only. Nothing in the prototype uses real money.

## Avatar Lab

`avatar-lab.html` shows the two characters built from atoms: 16,000 points per body (6,000 on phones), each pinned to a spot on the skin and coloured from the real texture. Atoms bond into molecules, one per bone, and molecules fuse into the solid body. Each frame, every atom follows its bones (linear blend skinning) through a spring, which keeps motion smooth. Buttons assemble the bodies from a cloud, break them apart, switch between the three stages and play idle, walk, run or dance.

## Character files

`models/soldier.js` and `models/michelle.js` are the Soldier and Michelle `.glb` models from the three.js examples, stored as base64 so they load as scripts. Both are Mixamo characters. Mixamo allows use in projects but not redistribution as standalone files; replace them with Aethra's own characters before any public release.

## World Weaver

`world-weaver.html` weaves five scenes from The Virtual Universe concept art (`world/`: the universe map, The Core, the Arrival Plaza, the World Gates and the Space Docks) using the same atoms → molecules → solid principle. Each picture becomes about 180,000 atoms (70,000 on phones) coloured from the art, which stream in from a cosmic cloud, starting at the centre and weaving outward. Atoms then bond into molecules of 12 × 12 atoms, and molecules fuse into a diorama whose depth is estimated from the picture. It's a 2.5D diorama: you can turn it to look in, but not walk behind it.

## Universe Flight

`universe-flight.html` turns the master map (`world/universe-map-2.jpg`) into a 3D space you fly through in an original ship. The Core and the ten places around it (Orbital Station, Nexus, Veladara, Kahem, Frostborn, Evermore, Astra, Velocity, Oblivion, Zephyr) sit where they are on the map, each an island of atoms coloured from its own part of the map art: a relief top, a rocky underside, orbit rings and a spire for The Core, and a ring for the station. Islands drift as loose clouds from afar and weave into solid worlds as you approach. Islands are solid, and glowing lanes link The Core to each world. Controls: keyboard and mouse, touch joystick and buttons, or a gamepad, plus a travel list that autopilots you to any world. About 220,000 atoms in all (90,000 on phones). When you reach a world, the card offers **Land on …**, which opens that world in `worlds.html`.

## Worlds

`worlds.html` lets you walk all eleven worlds as the woman or the man. Open it with a world id in the hash (`#nexus`, `#kahem`, and so on) or pick one from the list. Each world takes its colours from its part of the master map. When you arrive, about 70,000 atoms (26,000 on phones) stream in from the sky and weave every structure, starting near you and spreading outward; then the structures turn solid.

- **The Core:** the hub tower, a holographic globe and ten World Gates. Walk into a gate to travel.
- **Orbital Station:** a hangar deck, docked ships and a planet beyond the windows.
- **Nexus:** a lit neon metropolis.
- **Veladara:** a castle, forest and floating islands.
- **Kahem:** pyramids, obelisks and ruins in the dunes.
- **Frostborn:** ice spikes, pines, a frozen lake and an outpost, with snow falling.
- **Evermore:** a street of houses, a lake and a skyline.
- **Astra:** crystals, glass domes and craters, with low gravity.
- **Velocity:** an oval race track with boost pads.
- **Oblivion:** lava pools, spires and a citadel.
- **Zephyr:** a dense forest, a giant world tree and glowing mushrooms.

Everything is solid: walls, trees, crystals and pillars stop you, and the camera stays out of walls. Each world has a gate back to The Core.
