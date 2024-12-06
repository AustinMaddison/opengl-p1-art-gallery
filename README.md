# The Art Gallery

### Screenshots


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
Compile and run project `game.vcxproj` which will compile and run  `/game/builds/Release/The Art Gallery.exe`.

### How to run built binary?
Run the **windows-64bit** binary in `/game/builds/Release/The Art Gallery.exe`.

### Features

#### Procedurally Transformed Paintings From Image File

#### Flickering Lights

#### Academy Color Encoding System (ACES) Tonemapping

#### World Space TexCoords For Enviromental Textures

#### Spotlight Lights

#### Pixelated Lighting

#### NPR Diffuse-Specular Lambert Model

---

### Future Features

#### Deffered Rendering
So we can calculate lighting once. Also it would also force me to create framebuffer and render targets. Which would make it easier to facilitate post-processing and ui in a neater way.
For example, tonemapping is being done at a object level when it should be done over a single buffer.

#### UI

##### Concept
![ui](https://github.com/user-attachments/assets/87a4e46c-0f7b-4c34-8da5-3e5f75712e63)

##### Font rendering using SDF Font Atlas. Thought of making tooltips for what painting you are looking at.

This technique allows for anti aliasing that is resolution agnostic, since over or under sampling cannout occur since we are evaluating the distance from the closest surface. Also allows for font styles such as producing soft shadows and outlines easily.
![font](https://github.com/user-attachments/assets/30cfe107-ad87-49d8-a129-b54b03296cbc)


##### Font Rendering Using SDF Text Atlas




