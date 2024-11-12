import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';

// Scene setup
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x87ceeb); // Sky blue

// Camera setup
const camera = new THREE.PerspectiveCamera(
    75, // FOV
    window.innerWidth / window.innerHeight, // Aspect ratio
    0.1, // Near plane
    1000 // Far plane
);
camera.position.z = 5;

// Renderer setup
const renderer = new THREE.WebGLRenderer({ antialias: true });
renderer.setSize(window.innerWidth, window.innerHeight);
renderer.setPixelRatio(window.devicePixelRatio);
document.body.appendChild(renderer.domElement);

// Controls
const controls = new OrbitControls(camera, renderer.domElement);
controls.enableDamping = true;

// Handle window resize
window.addEventListener('resize', () => {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
});

// Add objects
// Cube
const geometry = new THREE.BoxGeometry();
const material = new THREE.MeshPhongMaterial({
    color: 0x00ff00,
    shininess: 60
});
const cube = new THREE.Mesh(geometry, material);
cube.rotation.x = 0.5;
cube.rotation.y = 0.5;
scene.add(cube);

// Sphere
const sphereGeometry = new THREE.SphereGeometry(0.65, 35, 35);
const sphereMaterial = new THREE.MeshStandardMaterial({
    color: 0xff33b7,
    roughness: 0.5
});
const sphere = new THREE.Mesh(sphereGeometry, sphereMaterial);
sphere.position.x = -2;
scene.add(sphere);

// Torus
const torusGeometry = new THREE.TorusGeometry(0.5, 0.2, 16, 100);
const torusMaterial = new THREE.MeshStandardMaterial({
    color: 0xb033ff,
    metalness: 0.5
});
const torus = new THREE.Mesh(torusGeometry, torusMaterial);
torus.position.x = 2;
scene.add(torus);

// Add lighting
const ambientLight = new THREE.AmbientLight(0x404040);
scene.add(ambientLight);
const directionalLight = new THREE.DirectionalLight(0xffffff, 1);
directionalLight.position.set(1, 1, 1);
scene.add(directionalLight);


// Animation loop
function animate() {
    requestAnimationFrame(animate);

    // Cube animation
    cube.rotation.x += 0.01;
    cube.rotation.y += 0.01;

    // Sphere animation
    sphere.position.y = Math.sin(Date.now() * 0.001) * 0.5;

    // Torus animation
    torus.rotation.z += 0.01;
    torus.scale.set(
        Math.abs(Math.sin(Date.now() * 0.001)) + 0.5,
        Math.abs(Math.sin(Date.now() * 0.001)) + 0.5,
        Math.abs(Math.sin(Date.now() * 0.001)) + 0.5
    );

    controls.update();
    renderer.render(scene, camera);
}
animate();