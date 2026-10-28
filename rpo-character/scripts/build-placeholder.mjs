// Builds public/models/RPO_Placeholder.glb: a primitive blockout of the RPO
// character (black/electric-blue palette, ~1.8 m, Y-up, facing +Z, feet at Y=0).
// Stand-in only; not the final model, no rig. Run: npm run placeholder
import { writeFileSync } from 'node:fs';
import * as THREE from 'three';
import { GLTFExporter } from 'three/addons/exporters/GLTFExporter.js';

// GLTFExporter uses the browser FileReader; Node has Blob but not FileReader.
globalThis.FileReader ??= class {
  readAsArrayBuffer(blob) { blob.arrayBuffer().then((r) => { this.result = r; this.onloadend?.(); }); }
  readAsDataURL(blob) {
    blob.arrayBuffer().then((r) => {
      this.result = `data:${blob.type || 'application/octet-stream'};base64,${Buffer.from(r).toString('base64')}`;
      this.onloadend?.();
    });
  }
};

const mat = {
  skin: new THREE.MeshStandardMaterial({ name: 'Skin', color: '#4a2e20', roughness: 0.6 }),
  hair: new THREE.MeshStandardMaterial({ name: 'Hair', color: '#0d0907', roughness: 0.95 }),
  jacket: new THREE.MeshStandardMaterial({ name: 'Jacket', color: '#07080a', roughness: 0.5, metalness: 0.05 }),
  shirt: new THREE.MeshStandardMaterial({ name: 'Shirt', color: '#0a0b0e', roughness: 0.9 }),
  pants: new THREE.MeshStandardMaterial({ name: 'Pants', color: '#0b0c0f', roughness: 0.85 }),
  gear: new THREE.MeshStandardMaterial({ name: 'Gear', color: '#040405', roughness: 0.4, metalness: 0.2 }),
  lens: new THREE.MeshStandardMaterial({ name: 'Lens', color: '#05070c', roughness: 0.05, metalness: 0.9 }),
  metal: new THREE.MeshStandardMaterial({ name: 'Zip', color: '#8a9099', roughness: 0.3, metalness: 1 }),
  accent: new THREE.MeshStandardMaterial({
    name: 'AccentBlue', color: '#0a4dff', emissive: '#0a4dff', emissiveIntensity: 0.9, roughness: 0.3,
  }),
};

const root = new THREE.Group();
root.name = 'RPO_Placeholder';
root.userData = { placeholder: true, note: 'Primitive blockout. Replace with RPO_Character.glb.' };

function part(parent, name, geometry, material, [x, y, z], { rot = [0, 0, 0], scale = [1, 1, 1] } = {}) {
  const mesh = new THREE.Mesh(geometry, material);
  mesh.name = name;
  mesh.position.set(x, y, z);
  mesh.rotation.set(...rot);
  mesh.scale.set(...scale);
  parent.add(mesh);
  return mesh;
}

function group(name) {
  const g = new THREE.Group();
  g.name = name;
  root.add(g);
  return g;
}

const box = (w, h, d) => new THREE.BoxGeometry(w, h, d);
const cyl = (rt, rb, h, seg = 24) => new THREE.CylinderGeometry(rt, rb, h, seg);
const sphere = (r, ...rest) => new THREE.SphereGeometry(r, 32, 24, ...rest);

// Head, goggles and hood.
const head = group('Head');
part(head, 'Neck', cyl(0.055, 0.06, 0.1), mat.skin, [0, 1.5, 0]);
part(head, 'Face', sphere(0.105), mat.skin, [0, 1.64, 0.005], { scale: [0.95, 1.18, 1] });
part(head, 'Hair', sphere(0.112, 0, Math.PI * 2, 0, Math.PI * 0.45), mat.hair, [0, 1.655, -0.005], { scale: [1, 1.1, 1.02] });
part(head, 'GoggleStrap', new THREE.TorusGeometry(0.113, 0.014, 12, 48), mat.gear, [0, 1.735, 0], { rot: [Math.PI / 2, 0, 0] });
part(head, 'GoggleFrame', box(0.19, 0.06, 0.05), mat.gear, [0, 1.735, 0.095]);
part(head, 'GoggleLens', box(0.17, 0.045, 0.012), mat.lens, [0, 1.735, 0.122]);
part(head, 'GoggleLight', box(0.15, 0.007, 0.006), mat.accent, [0, 1.708, 0.124]);
part(head, 'Hood', new THREE.TorusGeometry(0.11, 0.045, 12, 32, Math.PI * 1.3), mat.jacket, [0, 1.47, -0.04],
  { rot: [Math.PI / 2 - 0.25, 0, Math.PI * 0.35 + Math.PI], scale: [1.15, 1, 1] });

// Torso: open jacket over a dark hoodie.
const torso = group('Torso');
part(torso, 'Jacket', cyl(0.215, 0.19, 0.52), mat.jacket, [0, 1.2, 0], { scale: [1, 1, 0.65] });
part(torso, 'Shirt', box(0.15, 0.48, 0.02), mat.shirt, [0, 1.2, 0.125]);
part(torso, 'ZipLeft', box(0.008, 0.48, 0.01), mat.metal, [-0.078, 1.2, 0.13]);
part(torso, 'ZipRight', box(0.008, 0.48, 0.01), mat.metal, [0.078, 1.2, 0.13]);
part(torso, 'Waistband', cyl(0.195, 0.195, 0.05), mat.gear, [0, 0.965, 0], { scale: [1, 1, 0.68] });
part(torso, 'ChevronL', box(0.06, 0.012, 0.008), mat.accent, [0.135, 1.33, 0.118], { rot: [0, 0, 0.5] });
part(torso, 'ChevronR', box(0.06, 0.012, 0.008), mat.accent, [0.185, 1.33, 0.112], { rot: [0, 0, -0.5] });
part(torso, 'ChestBadge', new THREE.ConeGeometry(0.022, 0.035, 3), mat.metal, [-0.14, 1.36, 0.12], { rot: [Math.PI / 2, 0, Math.PI] });

// Backpack and harness.
const pack = group('Backpack');
part(pack, 'Pack', box(0.3, 0.42, 0.13), mat.gear, [0, 1.22, -0.19]);
part(pack, 'PackAccentL', box(0.11, 0.015, 0.01), mat.accent, [-0.05, 1.3, -0.258], { rot: [0, 0, -0.55] });
part(pack, 'PackAccentR', box(0.11, 0.015, 0.01), mat.accent, [0.05, 1.3, -0.258], { rot: [0, 0, 0.55] });
for (const s of [-1, 1]) {
  const side = s < 0 ? 'R' : 'L'; // character's left is +X when facing +Z
  part(pack, `StrapFront${side}`, box(0.045, 0.42, 0.012), mat.gear, [s * 0.12, 1.22, 0.128]);
  part(pack, `StrapLight${side}`, box(0.01, 0.06, 0.006), mat.accent, [s * 0.12, 1.37, 0.136]);
  part(pack, `StrapTop${side}`, box(0.045, 0.012, 0.26), mat.gear, [s * 0.12, 1.445, -0.03]);
}

// Arms, cuffs and fingerless gloves.
const arms = group('Arms');
for (const s of [-1, 1]) {
  const side = s < 0 ? 'R' : 'L';
  part(arms, `Shoulder${side}`, sphere(0.078), mat.jacket, [s * 0.225, 1.41, 0]);
  part(arms, `UpperArm${side}`, cyl(0.07, 0.06, 0.3), mat.jacket, [s * 0.255, 1.26, 0], { rot: [0, 0, s * 0.12] });
  part(arms, `Forearm${side}`, cyl(0.06, 0.05, 0.26), mat.jacket, [s * 0.285, 0.99, 0.01], { rot: [0, 0, s * 0.06] });
  part(arms, `Cuff${side}`, new THREE.TorusGeometry(0.052, 0.008, 8, 24), mat.accent, [s * 0.292, 0.875, 0.01], { rot: [Math.PI / 2, 0, 0] });
  part(arms, `Wrist${side}`, cyl(0.04, 0.04, 0.03), mat.skin, [s * 0.295, 0.85, 0.01]);
  part(arms, `Glove${side}`, box(0.06, 0.1, 0.09), mat.gear, [s * 0.3, 0.785, 0.015]);
  part(arms, `Knuckles${side}`, box(0.065, 0.03, 0.06), mat.gear, [s * 0.302, 0.745, 0.035]);
  part(arms, `Fingers${side}`, box(0.05, 0.035, 0.07), mat.skin, [s * 0.302, 0.715, 0.03]);
  part(arms, `GloveLight${side}`, sphere(0.012), mat.accent, [s * 0.333, 0.8, 0.02]);
}

// Legs: cargo pants with thigh pockets and straps.
const legs = group('Legs');
part(legs, 'Hips', box(0.32, 0.16, 0.2), mat.pants, [0, 0.89, 0]);
for (const s of [-1, 1]) {
  const side = s < 0 ? 'R' : 'L';
  part(legs, `Thigh${side}`, cyl(0.095, 0.08, 0.42), mat.pants, [s * 0.1, 0.65, 0]);
  part(legs, `Shin${side}`, cyl(0.075, 0.068, 0.36), mat.pants, [s * 0.11, 0.27, 0]);
  part(legs, `Pocket${side}`, box(0.05, 0.15, 0.12), mat.pants, [s * 0.19, 0.6, 0]);
  part(legs, `ThighStrap${side}`, cyl(0.098, 0.098, 0.025), mat.gear, [s * 0.1, 0.52, 0]);
}

// Boots with blue-lit soles.
const boots = group('Boots');
for (const s of [-1, 1]) {
  const side = s < 0 ? 'R' : 'L';
  part(boots, `BootShaft${side}`, cyl(0.075, 0.08, 0.14), mat.gear, [s * 0.11, 0.16, 0]);
  part(boots, `Boot${side}`, box(0.12, 0.1, 0.28), mat.gear, [s * 0.11, 0.075, 0.04]);
  part(boots, `BootToe${side}`, cyl(0.06, 0.06, 0.12, 24), mat.gear, [s * 0.11, 0.065, 0.17], { rot: [0, 0, Math.PI / 2] });
  part(boots, `Sole${side}`, box(0.13, 0.025, 0.36), mat.accent, [s * 0.11, 0.0125, 0.05]);
  part(boots, `BootBadge${side}`, new THREE.ConeGeometry(0.018, 0.03, 3), mat.metal, [s * 0.11, 0.2, 0.078], { rot: [Math.PI / 2, 0, Math.PI] });
}

const exporter = new GLTFExporter();
const glb = await exporter.parseAsync(root, { binary: true });
const out = new URL('../public/models/RPO_Placeholder.glb', import.meta.url);
writeFileSync(out, Buffer.from(glb));
let meshes = 0;
root.traverse((n) => { if (n.isMesh) meshes++; });
const size = new THREE.Box3().setFromObject(root).getSize(new THREE.Vector3());
console.log(`Wrote ${out.pathname} (${glb.byteLength} bytes, ${meshes} meshes, ` +
  `${size.x.toFixed(2)} x ${size.y.toFixed(2)} x ${size.z.toFixed(2)} m)`);
