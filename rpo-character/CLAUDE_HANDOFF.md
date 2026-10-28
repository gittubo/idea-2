# Claude Handoff — RPO Realistic Game Character

Use the attached READY PLAYER ONE (RPO) character reference sheet as the authoritative design reference. The target is a real, exportable 3D asset for a Three.js browser game, not a 2D illustration.

## Visual identity to preserve
- Young adult male, dark brown skin, short textured curls, defined jaw and subtle facial hair.
- Black futuristic visor/goggles with electric-blue illuminated elements.
- Black tactical jacket with hood, backpack/straps, blue accents and small insignia.
- Black tactical cargo trousers with thigh pockets and straps.
- Fingerless black armored gloves.
- Heavy black sci-fi boots with electric-blue details.
- Consistent realistic materials and the black/electric-blue palette.

## Required deliverables
1. `RPO_Character.glb` — actual textured 3D character; no image-plane substitute.
2. `RPO_Character.blend` if Blender is used.
3. Texture maps and a brief asset report.
4. If supported, a humanoid rig with skin weights and at least an idle animation; clearly state if rigging is not complete.

## Technical targets for browser gameplay
- Export as glTF 2.0 binary (`.glb`) with PBR materials.
- Aim for a sensible real-time polygon budget; keep high detail in the face, visor, hands, boots and silhouette.
- Keep UVs clean and textures optimized. Prefer 2K maps for the main character unless close-up gameplay needs more.
- Avoid baked studio backgrounds, lighting, or shadows in the textures.
- Set a consistent forward axis and scale; feet should rest at ground level.
- Use a skeleton with clean deformation at shoulders, elbows, hips and knees if the character is animated.
- Keep goggles, backpack, gloves and boots attached to appropriate bones or mesh groups.
- Validate the exported GLB by re-importing it and checking front, back, left, right and three-quarter views.

## Workflow
1. Inspect the image and identify the main full-body view as the primary silhouette reference. Treat detail panels as supplemental material references, not separate characters.
2. If image-to-3D is available, create a base mesh. A multiview workflow may be more reliable if clean front/side/back crops are available.
3. Inspect and repair the mesh in Blender; image-to-3D output may require retopology, anatomy cleanup, accessory separation, UV and material fixes.
4. Rig and test the character. Do not claim a rig or animation exists unless it is included and verified.
5. Export `RPO_Character.glb` into this project at `public/models/RPO_Character.glb`.
6. Run `npm install`, `npm run dev`, and test loading and orientation in the included Three.js viewer.

## Honesty checks
Do not deliver a render in place of a 3D model. Do not claim exact likeness, production-ready topology, rigging, or successful export unless verified. If tools or credits are unavailable, provide the best concrete next steps and identify which deliverables remain missing.
