# Input System

BlankC Engine uses the `Input` module (`input.c` and `input.h`) to handle all user interactions (keyboard and mouse). This module acts as an abstraction layer over GLFW's event system.

## 1. Polling Architecture

Unlike purely *event-driven* systems where you register *callback* functions, BlankC Engine uses a **Polling** system.

In every iteration of the *main loop*, the `engine_poll_input(Engine* engine)` function (inside `engine.c`) will run and synchronize the state from GLFW into our internal `Input` struct.

Workflow:
1.  **Read from OS:** GLFW reads hardware states (which keys are pressed, mouse position).
2.  **Store State:** These values are saved into boolean arrays (for keyboard) and floats (for mouse) inside the `Input` struct.
3.  **Use in Game:** Your game code (e.g., in `camera_controller.c` or `fps_game.c`) simply checks these state arrays, without needing to call GLFW functions directly.

## 2. Keyboard Input

Keyboard checking is done by querying the status of specific keys (Key Codes). BlankC Engine uses the exact same Key Code definitions as GLFW (e.g., `KEY_W`, `KEY_SPACE`, `KEY_ESCAPE`).

Main functions:
- `int input_key_down(Input* input, int key_code)`: Returns true (1) if the key is being held down this frame.
- `int input_key_pressed(Input* input, int key_code)`: Returns true (1) **only on the first frame** the key is pressed. Very useful for "single press" actions like shooting or jumping.

## 3. Mouse Input

The input system tracks two types of mouse data:
1.  **Mouse Buttons:** Controlled via `input_mouse_down()` and `input_mouse_pressed()` similarly to the keyboard. Button 0 is usually Left, button 1 is Right.
2.  **Movement (Delta):**
    - `input_mouse_dx()` and `input_mouse_dy()` return the cursor movement distance relative to the previous frame.
    - These values are crucial for implementing a *First Person Shooter* (FPS) or *free-look* camera.

## 4. Mouse Locking (Hiding the Cursor)

For 3D exploration games, you typically don't want the mouse cursor visible and moving out of the game window when rotating the camera.

You can control cursor visibility through the GL Context module:
- `gl_context_lock_mouse(engine->context, 1)`: Hides the cursor and locks it to the center of the screen. Mouse movement will continue to be calculated and returned by `input_mouse_dx/dy` without screen boundaries.
- `gl_context_lock_mouse(engine->context, 0)`: Shows the cursor again and lets it move freely (e.g., when opening the Main Menu).