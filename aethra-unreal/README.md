# Aethra in Unreal Engine 5: starter code

This is the start of the realistic version of Aethra: a photoreal city and lifelike people. The playable browser prototype in `../aethra/` is the gameplay reference. This project carries the same rules (levels, the five elements, the stronger-opponent bonus, race payouts) into Unreal Engine 5 C++.

**Status:** the shared rules in `Source/Aethra/Core/AethraRules.h` compile and pass their tests (see *Rules tests* below). The Unreal classes have **not been compiled yet**: Unreal can't run where this was written. Expect a few small fixes on the first build.

## What's here

| Class | What it does |
| --- | --- |
| `AethraRules.h` | Plain C++ rules shared by everything: XP per level, element stats per rank, opponent XP bonus (up to 3×), race prizes and pot |
| `AAethraPlayerState` | Level, XP, Shards, Power Points and element ranks; replicated for multiplayer |
| `AAethraCharacter` | Third-person player: jog, sprint, jump, camera, Enhanced Input, gets into the nearest car |
| `UElementPowerComponent` | Energy, cooldowns and the five elements: Fire projectile, Air dash, Ice freeze, Earth shockwave, Lightning chain. In a car: Air nitro, Ice freezes rival cars |
| `AFireballProjectile` | The Fire projectile |
| `AAethraVehicle` | Chaos vehicle you can enter and leave, with boost, freeze and self-driving for rivals and traffic |
| `ARaceCheckpoint`, `ARaceManager` | Street race: takes the stake, lines up the grid, drives rivals, tracks gates in order, pays out |
| `AEnemyDrone` | Rogue drone that hovers, chases, gets frozen or knocked back, and pays XP to whoever defeats it |
| `AAethraGameMode` | Uses the Aethra player state |

## Setup

1. Use **Unreal Engine 5.8** (the project is set to 5.8; 5.4 or later should also work). On Windows, also install **Visual Studio 2022** with the "Game development with C++" workload; on a Mac, install **Xcode**.
2. Right-click `Aethra.uproject` and choose **Generate Visual Studio project files** (Windows), then double-click `Aethra.uproject` and let it build the module.
3. In the editor, add starter content with **Add → Add Feature or Content Pack**: **Third Person** (mannequin and animations) and **Vehicle** (a ready car with wheels).
4. Create the Blueprints:
   - `BP_AethraCharacter` from `AAethraCharacter`: set the mesh and animation Blueprint from the Third Person pack.
   - `BP_Car` from `AAethraVehicle`: copy the mesh and wheel setup from the Vehicle pack's car.
   - `BP_Fireball` from `AFireballProjectile`: add a Niagara flame and a light, then set it as **Fireball Class** on the character's Powers component.
   - `BP_Drone` from `AEnemyDrone`: give it a mesh and implement **On Attack** (spawn an enemy shot) and **On Defeated** (explosion).
   - `BP_AethraGameMode` from `AAethraGameMode`: set **Default Pawn Class** to `BP_AethraCharacter`, and make it the default game mode.
5. Create the input assets and assign them on the Blueprints, following the controls table in the design document: Input Actions `IA_Move` (Axis2D), `IA_Look` (Axis2D), `IA_Jump`, `IA_Sprint`, `IA_Cast`, `IA_NextElement`, `IA_PreviousElement`, `IA_Interact`, plus `IA_Throttle`, `IA_Brake`, `IA_Steer`, `IA_Handbrake`, `IA_Exit` and `IA_Power` for cars. Then make an Input Mapping Context for walking and one for driving.
6. Build a test race: place `RaceCheckpoint` actors along a road and set each one's **Manager** to a `RaceManager`. In that manager, list the checkpoints in order, add four **Grid Slots** and set **Rival Class** to `BP_Car`. Call **Start Race** from a trigger.

## Photoreal people and worlds (Unreal 5.8)

This is where the GTA-level look comes from. The browser prototype can't reach it; Unreal with Lumen, Nanite, scanned assets and MetaHumans can. Menu names below may differ slightly in 5.8; Epic's MetaHuman and Fab documentation has the exact current steps.

**1. Your player as a MetaHuman (photoreal, from a face scan)**
1. Enable the **MetaHuman** plugins (Edit → Plugins, search "MetaHuman"), then restart the editor.
2. Create a MetaHuman in the editor's **MetaHuman Creator**. To use a real face, start from a **MetaHuman Identity**: either a short video of your face filmed on a phone (MetaHuman Animator), or a 3D scan of your head (Mesh to MetaHuman). Then pick hair, skin, body type and clothing.
3. Make `BP_AethraPlayer` from `AAethraCharacter`. Set its **Mesh** to the MetaHuman's body mesh and its Anim Class to a locomotion Animation Blueprint retargeted to the MetaHuman skeleton (UE5's IK Retargeter does this from the Third Person mannequin). Add the MetaHuman's face, hair and clothing parts as child components of the mesh, the same way the MetaHuman's own Blueprint has them, so the face follows the body.
4. In `BP_AethraGameMode`, set **Default Pawn Class** to `BP_AethraPlayer`.
5. Make a second MetaHuman for the female player and as many as you like for pedestrians.

**2. Veladara as a real place (first world to build)**
1. New level from the **Open World** template (World Partition, landscape, sky atmosphere, volumetric clouds, Lumen).
2. Sculpt and paint the landscape; use free **Fab** (Quixel Megascans) surfaces for grass, soil and rock. Megascans are free with Unreal.
3. Scatter Megascans trees, boulders, ferns and grass with the **PCG** plugin (Procedural Content Generation) or the Foliage tool. Keep Nanite on for rocks and trunks. Trunks and boulders must have collision (see *Solid objects*).
4. Add castle ruins from Fab's medieval kits, and a World Gate back to The Core.
5. Light it with **Sky Atmosphere**, **Volumetric Clouds**, a **Directional Light** as the sun and **Exponential Height Fog**. `Config/DefaultEngine.ini` already turns on Lumen and virtual shadow maps.

**3. Space and planets**
Use **Sky Atmosphere** on large planet meshes for real atmospheric scattering, so a planet's sky turns blue as you descend and you can fly down to land. The browser's Universe Flight shows the intended layout and the land-on-entry behaviour.

**Solid objects:** every prop must block the player and cars. Fab and Megascans meshes ship with collision; check each one in the Static Mesh editor (Show → Simple Collision) and add a capsule or box where it is missing. For trees painted with the Foliage tool or PCG, set Collision Presets to **BlockAll** so trunks are solid. Keep **Use Complex Collision as Simple** off for props, so the physics stays fast.

## Rules tests

The rules compile without Unreal:

```
cd Tests
g++ -std=c++20 -I../Source/Aethra/Core RulesTest.cpp -o rules_test && ./rules_test
```

It prints `All rules checks passed`.

## Not done yet

- Multiplayer: casting powers, entering cars and starting races still need Server RPCs. Progress already replicates.
- Saving progress to an account backend.
- HUD widgets (health, energy, element bar, minimap, race position).
- Traffic and pedestrian spawners; the vehicle's self-driving is the building block.
- Anything involving real money. See the design document's Economy and Legal sections.
