# Year 4 Games Design & Development 
# Game Phyics Continuous Assesment
### Joe O'Regan
### K00203642
### 25/01/2018 Updated Screenshots & Readme


## Description:

Scene demonstrating the use of game physics using the Bullet physics engine, OpenGL (3D graphics), and SDL (audio only)

---

## Screenshots:

![Screenshot 1: Castle](https://raw.githubusercontent.com/joeaoregan/Yr4-GamePhysics-Bullet-OpenGL-SDL/master/Screenshots/Screenshot1.png "Castle")
Castle, walls are box objects, towers are cylinder and cone objects

---

![Screenshot 2: Arrow created using compound shape](https://raw.githubusercontent.com/joeaoregan/Yr4-GamePhysics-Bullet-OpenGL-SDL/master/Screenshots/Screenshot2.png "Arrow compound shape")
The arrow is created using compounds shapes, and fired right-clicking

---

![Screenshot 3: Arrows destroy individual castle blocks](https://raw.githubusercontent.com/joeaoregan/Yr4-GamePhysics-Bullet-OpenGL-SDL/master/Screenshots/Screenshot3.png "Arrows destroy individual castle blocks")
Arrows destroy individual castle blocks, with sound effect

---

![Screenshot 4: Exploding Ball](https://raw.githubusercontent.com/joeaoregan/Yr4-GamePhysics-Bullet-OpenGL-SDL/master/Screenshots/Screenshot4.png "Exploding ball")
The exploding balls are created using sphere objects. Fired using the middle mouse button, they explode on impact with castle triggering an explosion at the point of impact, with sound effect

---

![Screenshot 5: Castle walls destroyed by exploding ball](https://raw.githubusercontent.com/joeaoregan/Yr4-GamePhysics-Bullet-OpenGL-SDL/master/Screenshots/Screenshot5.png "Castle walls destroyed by exploding ball")
Castle walls destroyed by exploding ball

---

![Screenshot 6: Bonus Letter compound shapes](https://raw.githubusercontent.com/joeaoregan/Yr4-GamePhysics-Bullet-OpenGL-SDL/master/Screenshots/Screenshot6.png "Bonus Letter compound shapes")
The letters are created using compound shapes, and have a different points value and effect to the castle blocks

---

![Screenshot 7: Explosion](https://raw.githubusercontent.com/joeaoregan/Yr4-GamePhysics-Bullet-OpenGL-SDL/master/Screenshots/Screenshot7.png "Tower Explosion")
Tower exploding after being hit by exploding sphere object

---

### Book used in creating the game:


[Learning Game Physics with Bullet Physics and OpenGL](https://www.packtpub.com/game-development/learning-game-physics-bullet-physics-and-opengl)

---

## Running:

Requires Visual Studio. Created with Visual Studio 2017, and SDL dll files

---

## Files:
##### My Additions:
		
| No. | File        | Description |
| --- | ------------- |-------------|
| 1 | **Audio.h** | Game sound effects and music manager header |
| 2 | **Audio.cpp** | Game sound effects and music manager functions |
| 3 | **JoeScene.h** | Scene header file |
| 4 | **JoeScene.cpp** | Creates the actual scene used, and its game objects |
| 5 | **JoeSceneV2.h** | Testing version of scene header file |
| 6 | **JoeSceneV2.cpp** | Testing version of scene |
| 7 | **SDLInit.h** | Initialise SDL Audio |
| 8 | **Text.h** | Game text header file |
| 9 | **Text.cpp** | Functions to add text to the game, with singleton to access |

##### Files reused from example programs:

| No. |File        | Description           |
| --- | ------------- |-------------|
| 1 | **GLDebugFont.h** | Example Program text functions |
| 2 | **GLDebugFont.cpp** | Example Program text functions |

##### Original Tutorial Files (Some with slight modifications):

| No. |File        | Description           |
| --- | ------------- |-------------|
| 1 | **BasicDemo** | Example basic demo scene |
| 2 | **BulletOpenGLApplication** | Application layer code |
| 3 | **CollisionFilteringDemo** | Collision filtering demo scene |
| 4 | **DebugDrawer** | Interface to render debug data |
| 5 | **FreeGLUTCallbacks.h** | Header file |
| 6 | **GameObject** | Objects used in the scene |
| 7 | **Main.cpp** | Create a scene using JoeScene class |
| 8 | **OpenGLMotionState** | Motion state class |
| 9 | **SoftBodyDemo** | Tutorial example demonstrating the use of soft body objects |
