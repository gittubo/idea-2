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

1. Install **Unreal Engine 5.6** from the Epic Games Launcher (5.4 or later should also work). On Windows, also install **Visual Studio 2022** with the "Game development with C++" workload; on a Mac, install **Xcode**.
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

## Realistic people and city

- **People:** enable the **MetaHuman** plugin and create MetaHumans for the player and the pedestrians. MetaHumans can also be made from a 3D face scan, which is the route to "scan your face into the game". Check Epic's MetaHuman documentation for the current steps, as the tools change between engine versions.
- **City:** build streets from photo-scanned assets on **Fab** (Quixel Megascans), and lay out blocks with the **PCG** framework. `Config/DefaultEngine.ini` already turns on Lumen lighting and reflections and virtual shadow maps.
- **Motion:** replace the template animations with motion-captured walk, run, driving and combat moves.
- **Solid objects:** every prop must block the player and cars. Fab and Megascans meshes ship with collision; check each one in the Static Mesh editor (Show → Simple Collision) and add a capsule or box where it is missing. For trees painted with the Foliage tool, set the foliage type's Collision Presets to **BlockAll** so trunks are solid. Keep **Use Complex Collision as Simple** off for props, so the physics stays fast.

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
