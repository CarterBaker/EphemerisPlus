## 🧩 Epherimis++: an astronomical tracking software

**Solo-developed systems architecture prototype** built in 5 days with the intent of creating a software capable of
tracking objects within our solar system. As it stands the project fell short of my current goals due to time constraints.
The idea was make a working model of the solar system for now and go from there. As of now each planet and the sun are
present but not orbiting the sun.

---

## 📹 Demo
  ▶️ [Watch the project demo on YouTube](https://youtu.be/hekUB8OGGjo)

---

## 🎮 Core Features

- 💾 **3d Mesh/ Shader pipeline**  
 - The software is capable of loading and generating mesh objects as well as using complex shaders to draw them to screen.

- 🌍 **Each planet veiwable in 3D**  
  - Each planet is loaded at runtime from an obj file and placed in the world at a scale position.

- 🧙 **2D Distant Icons**  
  - Since IRL planets are so small and so far apart there is also a 2d Icon that displays far away.

---

## 🔮 Future Plans
  - Add planetary orbit
  - Add every lanets moons
  - Find a way to calculate planetary rings
  - Find a way to calculate how much each planet is smushed aling it's axis
  - Add a window to input calculations for asteroids and meteorites and their trajectory

---

## 🧠 Developer Notes

  The planets are too far apart in real life so every planets radius and actual distance from the sun was multiplies by a small
  scale so that everything would fit within 100000 units from center. Since planets are so small and so far apart at a distance
  the software disables the planets 3d model and draws an icon onto a mesh that "billboards" towards the camera and scales up so
  that it is always visible. Later on using a shader on each planet to magnify them may be a more realistic approach.

---

## 🛠 Development Info

- **Language:** C++

---

## 🧰 Setup Instructions

1. Clone this repo:
   ```bash
   git clone https://github.com/CarterBaker/RPG_Prototype.git

2. Open with Visual Studio

3. Press f5 to test