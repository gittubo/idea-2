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

`universe-flight.html` is a cockpit-view flight through a real-looking star system. The Core sits at the centre with the ten worlds where they are on the master map, each a planet with a real surface photo, a day and night side, clouds or city lights, and a glowing atmosphere. Velocity has rings, Oblivion glows with lava, and the Orbital Station is a rotating habitat ring. Planets are not walls: fly into a planet's atmosphere and the sky turns from black to that world's colour, the ship slows, and a **Walk on {world}** button opens that world in `worlds.html`. **Take off** points you back to space. Controls: W/S throttle, Shift boost, mouse or arrow keys to steer, A/D roll; a touch joystick with THRUST, BRAKE and BOOST buttons; or a gamepad. The Autopilot list flies you to any world.

## Worlds

`worlds.html` lets you walk all eleven worlds as the woman or the man. **Veladara** is the first world rebuilt for realism: a photographed sky lights it, and it is made of scanned trees, boulders, grass, ferns, cliffs and stone fort ruins with real ground textures. The other ten are still the earlier simple-shape versions and will be converted the same way. Open it with a world id in the hash (`#nexus`, `#kahem`, and so on) or pick one from the list. Each world takes its colours from its part of the master map. Worlds appear complete when you arrive. The player's planted foot is kept on the ground surface each frame, so feet don't sink into slopes or floors.

- **The Core:** the hub tower, a holographic globe and ten World Gates. Walk into a gate to travel.
- **Orbital Station:** a hangar deck, docked ships and a planet beyond the windows.
- **Nexus:** a lit neon metropolis.
- **Veladara:** a valley of scanned trees and boulders, ringed by cliffs, with the ruins of a stone fort.
- **Kahem:** pyramids, obelisks and ruins in the dunes.
- **Frostborn:** ice spikes, pines, a frozen lake and an outpost, with snow falling.
- **Evermore:** a street of houses, a lake and a skyline.
- **Astra:** crystals, glass domes and craters, with low gravity.
- **Velocity:** an oval race track with boost pads.
- **Oblivion:** lava pools, spires and a citadel.
- **Zephyr:** a dense forest, a giant world tree and glowing mushrooms.

Everything is solid: walls, trees, crystals and pillars stop you, and the camera stays out of walls. Each world has a gate back to The Core.

## Asset credits

- Veladara's models, ground textures and sky (`assets/veladara/`) are from Poly Haven (CC0): island_tree_02, boulder_01, grass_medium_01, fern_02, modular_fort_01, mountainside, aerial_grass_rock and kloofendal_48d_partly_cloudy_puresky. They were simplified and their textures resized for the web.
- Planet, sun and Milky Way textures (`planets/`) are from Solar System Scope, licensed CC BY 4.0, which requires this credit.
