# RPO Character — Three.js Starter

A small browser-based 3D viewer prepared for the realistic READY PLAYER ONE (RPO) character asset.

## Requirements
- Node.js 20.19+ or 22.12+
- A real exported model at `public/models/RPO_Character.glb`

## Run locally
```bash
npm install
npm run dev
```
Open the local URL Vite prints in your terminal. For a production build:
```bash
npm run build
npm run preview
```

## Add the character
Copy the final `.glb` to:
`public/models/RPO_Character.glb`

The viewer uses Three.js `GLTFLoader` and `OrbitControls`, physically based material rendering, an environment map, and blue rim lighting. It will display embedded animation clip 1 if the file contains animation clips.

## Placeholder model
Until the real model exists, the viewer falls back to `public/models/RPO_Placeholder.glb`:
a primitive blockout (boxes, cylinders, spheres) in the reference palette, about 1.78 m tall,
Y-up, facing +Z, feet at Y=0, parts grouped as Head, Torso, Backpack, Arms, Legs, Boots.
It has no rig, no textures and no animation, and is not a likeness. Regenerate it with:
```bash
npm run placeholder
```
Once `public/models/RPO_Character.glb` exists, the viewer loads that instead.

## Important asset requirements
A reference sheet is not itself a 3D model. The GLB must be created by an image-to-3D service or in Blender. For a usable real-time character, check:
- Full 360-degree shape and silhouette (not a flat/card-like mesh)
- Textures and UVs embedded or correctly linked
- Correct scale and forward direction
- Reasonable polygon count and draw calls
- Humanoid skeleton and skin weights if animated
- Accessories attached to the correct bones or mesh parts
- Test in the target browser and on a mid-range device

## Suggested production pipeline
1. Generate a base mesh from the reference using a multiview-capable image-to-3D model if possible. The sheet contains multiple panels, so a clean crop of the main full-body front view is often better for single-image generation.
2. Inspect the result in Blender from front, back and side views.
3. Correct anatomy, hands, boots, goggles, backpack, and any mesh intersections.
4. Retopologize/decimate as appropriate, UV unwrap, bake PBR textures, and verify material maps.
5. Rig and weight-paint the body; test idle, walk, run, jump and combat poses.
6. Export as glTF Binary (`.glb`) and load it here.
7. Optimize textures, geometry, shadows and animation for browser performance.

## Folder map
- `src/main.js` — scene, lighting, controls and GLB loader
- `src/style.css` — viewer UI
- `public/models/` — place the actual model here (holds the placeholder for now)
- `scripts/build-placeholder.mjs` — generates the placeholder GLB
- `CLAUDE_HANDOFF.md` — modeling brief for Claude
