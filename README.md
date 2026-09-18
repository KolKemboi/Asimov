# A.P.E. (Asimov Physics Engine)

**A.P.E. (Asimov Physics Engine)** is a C++-based real-time physics and simulation engine named after **Isaac Asimov**, the renowned science-fiction author best known for *The Foundation* saga.

The engine is being developed with a focus on **robotics simulation** and is intended to work alongside **ROS (Robot Operating System)**. The goal is to provide a flexible environment where physics simulation, rendering, and robotic systems can interact within the same framework.

A.P.E. combines an Entity Component System (ECS), real-time rendering, physics simulation, and an integrated editor to provide the foundation for building and experimenting with simulated robotic environments.

The project is designed with modularity in mind, allowing individual systems such as the renderer, physics engine, ECS, and ROS integration to evolve independently while communicating through a common engine architecture.

![Physics Engine](https://github.com/KolKemboi/Asimov/blob/0681f2b07b0ed5736f6a22318202f3fcfe09fc62/images/Engine.png)

## Motivation

The project aims to bridge the gap between **game-engine-style real-time simulation** and **robotics development**.

By combining physics simulation with ROS, A.P.E. can provide an environment for experimenting with robotic systems, sensors, rigid-body dynamics, collision detection, and simulated environments before deploying them to physical hardware.

The long-term vision is to provide a lightweight and extensible simulation platform that can be used for:

- Robotics simulation
- Physics experimentation
- ROS-based development
- Autonomous systems
- Sensor simulation
- Robot visualization
- Real-time environment prototyping
- Game and simulation development



## Structure of the Engine

![Engine Architecture](https://github.com/KolKemboi/Asimov/blob/0681f2b07b0ed5736f6a22318202f3fcfe09fc62/images/Asimov.drawio.png)

The engine is divided into several major subsystems, each responsible for a specific part of the runtime and editor.

### User Interface

The editor interface is built using **Dear ImGui**, specifically the docking branch. This provides a flexible interface for arranging panels and creating an editor-style workflow.

The UI includes tools for inspecting the scene hierarchy, editing entity components, creating and deleting objects, selecting physics collider types, and interacting with files.

### Entity Component System

Asimov uses **EnTT** as its Entity Component System (ECS).

The ECS architecture separates entities from the data associated with them. This provides a flexible foundation for managing objects in a scene while keeping engine systems decoupled and modular.

Components can represent different aspects of an entity, such as transforms, rendering information, or physics properties.

### Renderer

The current rendering backend is based on **OpenGL**, with **GLFW** providing window and input management and **GLAD** handling OpenGL function loading.

The renderer is designed with future backend expansion in mind, with a planned integration of **Vulkan through Rust**. This is intended to provide an additional, lower-level rendering backend while keeping the engine architecture extensible.

### Physics

Physics simulation is handled using **ReactPhysics3D**.

The physics system provides collision detection and rigid-body simulation, allowing entities in the scene to interact physically. The editor also provides a collider selection interface for configuring the collision geometry associated with objects.

## Features

### Scene Outliner

![SceneOutliner](https://github.com/KolKemboi/Asimov/blob/0681f2b07b0ed5736f6a22318202f3fcfe09fc62/images/outliner.png)

The **Scene Outliner** provides a hierarchical view of the objects currently present in a scene.

It allows users to quickly inspect the scene and select individual entities for further editing. This provides the central navigation point for managing objects within the editor.

### Properties Editor

![Properties](https://github.com/KolKemboi/Asimov/blob/0681f2b07b0ed5736f6a22318202f3fcfe09fc62/images/PropertiesEditor.png)

The **Properties Editor** allows users to inspect and modify the components associated with a selected entity.

Rather than requiring properties to be changed directly in code, the editor exposes relevant component data through the UI, making scene construction and experimentation more interactive.

### Add Object

![AddObject](https://github.com/KolKemboi/Asimov/blob/0681f2b07b0ed5736f6a22318202f3fcfe09fc62/images/AddObject.png)

The **Add Object** functionality allows new entities to be created directly from the editor.

This provides a straightforward workflow for constructing scenes and testing different combinations of rendering and physics components without manually creating every object in code.

### Delete Object

![DeleteObject](https://github.com/KolKemboi/Asimov/blob/0681f2b07b0ed5736f6a22318202f3fcfe09fc62/images/DeleteObject.png)

Objects can be removed directly from the scene through the **Delete Object** functionality.

This complements the scene-management workflow by allowing users to quickly create, test, and remove entities while working on a scene.

### File Dialog

![FileDialog](https://github.com/KolKemboi/Asimov/blob/0681f2b07b0ed5736f6a22318202f3fcfe09fc62/images/FileDialog.png)

The engine includes an integrated **file dialog** for interacting with files and engine resources.

This provides the foundation for managing external assets and scene-related files directly from the editor rather than relying entirely on external file-management tools.

### Collider Selector

![ColliderSelector](https://github.com/KolKemboi/Asimov/blob/0681f2b07b0ed5736f6a22318202f3fcfe09fc62/images/colliderTypeSelector.png)

The **Collider Selector** provides an interface for choosing the type of physics collider associated with an object.

This makes it possible to configure collision geometry directly within the editor and provides a more visual workflow for setting up physical interactions between objects.

## Technology Stack

| Subsystem | Technology |
|---|---|
| User Interface | Dear ImGui — Docking Branch |
| Entity Component System | EnTT |
| Rendering | OpenGL |
| Window/Input Management | GLFW |
| OpenGL Loader | GLAD |
| Physics | ReactPhysics3D |
| Planned Rendering Backend | Vulkan / Rust |

## Architecture

The overall architecture is designed around independent engine subsystems communicating through shared scene and entity data.

At the core is the **ECS**, which provides the data model for entities and components. Rendering systems consume the relevant rendering components, while the physics system operates on entities that contain physical properties.

The editor sits on top of these systems and provides an interactive interface for inspecting and modifying the current state of the engine.

This separation makes it possible to extend individual subsystems without requiring the entire engine to be redesigned.

## Project Goals

The long-term goal of Asimov is to develop a flexible real-time engine with a strong editor-driven workflow.

Current development focuses on building the core systems and establishing a modular architecture that can support additional rendering, physics, asset-management, and editor functionality in the future.

Planned development includes:

- Vulkan rendering backend integration
- Rust-based rendering components
- Improved scene and asset management
- Expanded physics functionality
- Additional editor tools
- Scene serialization
- More rendering features
- Improved debugging and profiling tools
