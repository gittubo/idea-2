import './style.css';
import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';
import { RoomEnvironment } from 'three/addons/environments/RoomEnvironment.js';

const app = document.querySelector('#app');
const status = document.querySelector('#status');
const loading = document.querySelector('#loading');

const scene = new THREE.Scene();
scene.background = new THREE.Color('#0b1017');
scene.fog = new THREE.Fog('#0b1017', 10, 28);

const camera = new THREE.PerspectiveCamera(35, innerWidth / innerHeight, 0.1, 100);
camera.position.set(3.2, 2.1, 6.2);

const renderer = new THREE.WebGLRenderer({ antialias: true, alpha: false });
renderer.setPixelRatio(Math.min(devicePixelRatio, 2));
renderer.setSize(innerWidth, innerHeight);
renderer.outputColorSpace = THREE.SRGBColorSpace;
renderer.toneMapping = THREE.ACESFilmicToneMapping;
renderer.toneMappingExposure = 1.05;
app.appendChild(renderer.domElement);

const pmrem = new THREE.PMREMGenerator(renderer);
scene.environment = pmrem.fromScene(new RoomEnvironment(), 0.04).texture;
pmrem.dispose();

scene.add(new THREE.HemisphereLight(0xb8d5ff, 0x171923, 1.4));
const key = new THREE.DirectionalLight(0xe8f1ff, 3.0);
key.position.set(4, 7, 5);
scene.add(key);
const rim = new THREE.DirectionalLight(0x167bff, 2.6);
rim.position.set(-4, 3, -3);
scene.add(rim);
const warmFill = new THREE.PointLight(0xffa46b, 25, 12);
warmFill.position.set(0, 1.3, 3.5);
scene.add(warmFill);

const floor = new THREE.Mesh(
  new THREE.CircleGeometry(8, 96),
  new THREE.MeshStandardMaterial({ color: '#111820', roughness: 0.78, metalness: 0.16 })
);
floor.rotation.x = -Math.PI / 2;
floor.position.y = -0.02;
scene.add(floor);

const grid = new THREE.GridHelper(16, 32, 0x183451, 0x101d2b);
grid.position.y = 0.002;
scene.add(grid);

const controls = new OrbitControls(camera, renderer.domElement);
controls.target.set(0, 1.05, 0);
controls.enableDamping = true;
controls.dampingFactor = 0.06;
controls.minDistance = 2.2;
controls.maxDistance = 12;
controls.maxPolarAngle = Math.PI * 0.49;

let character = null;
let mixer = null;
const clock = new THREE.Clock();

function frameObject(object) {
  const box = new THREE.Box3().setFromObject(object);
  const size = box.getSize(new THREE.Vector3());
  const center = box.getCenter(new THREE.Vector3());
  // Center the model at X/Z and place its feet at Y=0.
  object.position.x -= center.x;
  object.position.z -= center.z;
  object.position.y -= box.min.y;
  const maxDim = Math.max(size.x, size.y, size.z);
  controls.target.set(0, Math.max(size.y * 0.52, 0.8), 0);
  camera.position.set(maxDim * 0.72, size.y * 0.42, maxDim * 1.8);
  controls.update();
}

const loader = new GLTFLoader();

function showModel(gltf, label) {
  character = gltf.scene;
  character.traverse((node) => {
    if (node.isMesh) {
      node.castShadow = true;
      node.receiveShadow = true;
      if (node.material) {
        node.material.needsUpdate = true;
      }
    }
  });
  scene.add(character);
  frameObject(character);
  if (gltf.animations?.length) {
    mixer = new THREE.AnimationMixer(character);
    mixer.clipAction(gltf.animations[0]).play();
    status.textContent = `Loaded ${label} · ${gltf.animations.length} animation clip(s)`;
  } else {
    status.textContent = `Loaded ${label} · no animation clips embedded`;
  }
  loading.classList.add('hidden');
}

loader.load(
  '/models/RPO_Character.glb',
  (gltf) => showModel(gltf, 'RPO character'),
  (event) => {
    if (event.total) status.textContent = `Loading model… ${Math.round(event.loaded / event.total * 100)}%`;
  },
  (error) => {
    console.warn('RPO model not loaded yet:', error);
    // Fall back to the primitive blockout until the real model exists.
    loader.load(
      '/models/RPO_Placeholder.glb',
      (gltf) => showModel(gltf, 'PLACEHOLDER blockout (not the final model)'),
      undefined,
      () => { status.textContent = 'Model missing — add RPO_Character.glb to public/models/'; }
    );
  }
);

addEventListener('resize', () => {
  camera.aspect = innerWidth / innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(innerWidth, innerHeight);
});

addEventListener('keydown', (event) => {
  if (event.key.toLowerCase() === 'r') {
    if (character) frameObject(character);
    else {
      camera.position.set(3.2, 2.1, 6.2);
      controls.target.set(0, 1.05, 0);
      controls.update();
    }
  }
});

function animate() {
  requestAnimationFrame(animate);
  const dt = clock.getDelta();
  if (mixer) mixer.update(dt);
  controls.update();
  renderer.render(scene, camera);
}
animate();
