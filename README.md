# Asimov Physics Engine

A C++ physics and simulation engine designed for robotics applications, combining real-time rigid-body physics, OpenGL rendering, ECS-based architecture, and asset importing into a modular simulation framework.

The engine is built around:

* **C++** — Core engine and simulation logic
* **OpenGL** — Real-time rendering
* **GLM** — Mathematics and linear algebra
* **Assimp** — 3D model and asset importing
* **ReactPhysics3D** — Rigid-body physics and collision detection
* **EnTT** — Entity Component System (ECS)
* **ImGui** — User Interface

## Features

* Real-time rigid-body physics simulation
* Collision detection and response
* Entity Component System architecture
* OpenGL-based rendering
* Loading of common 3D model formats through Assimp
* Transform, camera, and rendering components
* Physics-driven robotic objects
* Modular architecture for extending simulation systems
* Support for articulated robotics workflows
* Separation between rendering, physics, and simulation logic
* Designed for experimentation with robotic mechanisms and environments

## Tech Stack

| Component     | Technology     |
| ------------- | -------------- |
| Language      | C++            |
| Rendering     | OpenGL         |
| Mathematics   | GLM            |
| Asset Loading | Assimp         |
| Physics       | ReactPhysics3D |
| ECS           | EnTT           |
| Build System  | CMake          |

## Architecture

The engine follows a modular ECS-oriented architecture.

```text
                    +----------------------+
                    |     Simulation       |
                    |       Manager        |
                    +----------+-----------+
                               |
             +-----------------+-----------------+
             |                 |                 |
             v                 v                 v
      +-------------+   +-------------+   +-------------+
      |    EnTT     |   |   Physics   |   |  Renderer   |
      |     ECS     |   | ReactPhys3D |   |   OpenGL    |
      +------+------+   +------+------+   +------+------+
             |                 |                 |
             +-----------------+-----------------+
                               |
                               v
                       +---------------+
                       |     GLM       |
                       |   Math /      |
                       | Transforms    |
                       +---------------+

                     Asset Pipeline
                           |
                           v
                       +--------+
                       | Assimp |
                       +--------+
```

### Entity Component System

Entities are represented using **EnTT**, allowing simulation objects to be composed from independent components.

Example components might include:

```text
Entity
├── TransformComponent
├── MeshComponent
├── RigidBodyComponent
├── ColliderComponent
├── RobotComponent
└── JointComponent
```

This allows robots, obstacles, sensors, and other simulation objects to be constructed without requiring deep inheritance hierarchies.

## Physics

Physics simulation is handled by **ReactPhysics3D**.

The physics layer is responsible for:

* Rigid bodies
* Collision shapes
* Collision detection
* Contact resolution
* Gravity
* Linear and angular velocity
* Forces and torques
* Physics constraints
* Simulation stepping


Physics objects can be synchronized with ECS entities so that the physics engine controls the physical state while the renderer displays the resulting transforms.

## Asset Loading

3D assets are imported using **Assimp**.

This allows the engine to work with common formats such as:

* `.obj`
* `.fbx`
* `.dae`
* `.gltf`
* `.glb`

## Robotics Simulation

The engine is intended to provide a foundation for simulating robotic systems in interactive environments.

Potential use cases include:

* Mobile robots
* Robotic arms
* Wheeled robots
* Autonomous navigation experiments
* Manipulation simulations
* Sensor simulation
* Collision testing
* Motion and control experiments
* Robot-environment interaction
* Physics-based prototyping


## Project Structure

```text
.
├── CMakeLists.txt
├── run_app.sh
├── assets/
│   ├── models/
│   ├── textures/
│   └── shaders/
│
├── include/
│ 
├── src/
│   └── engine/
│       ├── core/
│       ├── ecs/
│       ├── physics/
│       ├── rendering/
│       ├── robotics/
│       └── assets/
│
├── examples/
    ├── basic_scene/
    ├── rigid_body/
    └── robot_simulation/

```

## Requirements

Before building the engine, install:

* C++ compiler supporting **C++17** or newer
* CMake
* OpenGL
* GLM
* Assimp
* ReactPhysics3D
* EnTT

The exact OpenGL version and platform-specific requirements depend on the renderer implementation.

## Building

Clone the repository:

```bash
git clone https://github.com/KolKemboi/Asimov.git
cd <repository-directory>
```

Go to the Engine Folder and run

```bash
./run_app.sh
```


## Roadmap

* [ ] Basic ECS architecture
* [ ] OpenGL renderer
* [ ] Shader system
* [ ] Assimp asset pipeline
* [ ] ReactPhysics3D integration
* [ ] Physics debug rendering
* [ ] Robot entity abstraction
* [ ] Joint and constraint support
* [ ] Sensor framework
* [ ] Camera and visualization tools
* [ ] Robot control interfaces
* [ ] Scene serialization
* [ ] Simulation recording and playback
* [ ] GUI/editor
* [ ] Automated physics tests
* [ ] Robotics examples

## Contributing

Contributions are welcome.

When contributing:

1. Create a feature branch.
2. Keep systems modular.
3. Add tests for new simulation functionality where appropriate.
4. Keep rendering, physics, and ECS responsibilities separated.
5. Submit a pull request describing the changes.

## License

This project is licensed under the **MIT License**.

See [`LICENSE`](LICENSE) for details.
