
![d](https://github.com/user-attachments/assets/0017470a-49a8-4ddc-8074-5cdd5477095b)

---

### How to compile and run?
Open the solution `final-project.sln` file using **Visual Studio 2022**.
```
├──  final-project
│  ├──  final-project.sln
│  ├──  final-project.vcxproj
│  └──  final-project.vcxproj.filters
├──  game
│  ├──  builds
│  ├──  game.vcxproj
│  ├──  game.vcxproj.filters
│  ├──  game.vcxproj.user
│  ├──  include
│  ├──  intermediates
│  ├──  lib
│  ├──  resources
│  └──  src
```
Compile and run project `game.vcxproj`, which will produce and execute `/game/builds/Release/The Art Gallery.exe`.

---
### Features

#### Academy Color Encoding System (ACES) Tonemapping
- Perceptual light values falloff in a natural looking and pleasing way.

#### World Space TexCoords For Enviromental Textures
- This allowed for easy texturing. Additionally we could use sample 3D noise to break up repetition in the textuers by mixing different tones without producing seams.

#### Spotlight Lights
![a](https://github.com/user-attachments/assets/e4bfa11e-4ffe-4505-a5c3-28477a0a8668)

#### Flickering Lights
https://github.com/user-attachments/assets/b7898958-2cd3-4d9b-9a16-14eeee1a29b0

#### Pixelated Lighting
![c](https://github.com/user-attachments/assets/8648eff2-28ed-4d8b-9f46-383296ed6a74)

#### NPR Diffuse-Specular Phong Model
![b](https://github.com/user-attachments/assets/fd494b52-69d1-4766-8117-cd6c97b6fba1)




---

### Future Plans

#### Deffered Rendering
So we can calculate lighting once. Also it would also force me to create framebuffer and render targets. Which would make it easier to facilitate post-processing and ui in a neater way.
For example, tonemapping is being done at a object level when it should be done over a single buffer.

#### UI

##### Concept
![ui](https://github.com/user-attachments/assets/87a4e46c-0f7b-4c34-8da5-3e5f75712e63)

##### Font rendering using SDF Font Atlas. Thought of making tooltips for what painting you are looking at.

![font](https://github.com/user-attachments/assets/30cfe107-ad87-49d8-a129-b54b03296cbc)
This technique allows for  alias free type, since over or under sampling cannot occur due the propperties of decoding distance fields into shape. Also allows for font styles such as producing soft shadows and outlines easily.
