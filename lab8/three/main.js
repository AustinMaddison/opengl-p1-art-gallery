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

let material = null;

material = new THREE.MeshPhongMaterial({
    color: 0x00ff00,
    shininess: 60
});

const cube = new THREE.Mesh(new THREE.BoxGeometry(), material);
cube.rotation.x = 0.5;
cube.rotation.y = 0.5;
scene.add(cube);

material = new THREE.MeshPhongMaterial({
    color: 0xff0000,
    shininess: 100
});

const torus = new THREE.Mesh(new THREE.TorusGeometry( 0.5, 0.25, 100, 100 ), material);
torus.position.x = -2;
// torus.position.y = 0.5;
scene.add(torus);

material = new THREE.MeshPhongMaterial({
    color: 0x0000ff,
    shininess: 100
});

const ico = new THREE.Mesh(new THREE.IcosahedronGeometry(0.8, 0), material);
ico.position.x = 2;
// torus.position.y = 0.5;
scene.add(ico);



const ambientLight = new THREE.AmbientLight(0x606060);
scene.add(ambientLight);
const directionalLight = new THREE.DirectionalLight(0xffffff, 2);
directionalLight.position.set(1, 1, 1);
scene.add(directionalLight);

function animate() {
    requestAnimationFrame(animate);
    cube.rotation.x += 0.001;
    const time = Date.now() * 0.001;
    torus.rotation.x += Math.sin(time) * 0.01;
    torus.rotation.y += Math.cos(time) * 0.01;

    ico.rotation.y += Math.cos(time) * 0.02;
    ico.rotation.x -= Math.cos(time) * 0.02;

    ico.scale.x = (1 + Math.sin(time)) * 0.5;
    ico.scale.y = (1 + Math.cos(time)) * 0.5;

    // Try scaling the sphere uniformly with a cosine function with sphere.scale.set()
    controls.update();
    renderer.render(scene, camera);
}

animate();


// Handle window resize
window.addEventListener('resize', () => {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
});