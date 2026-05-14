# UI System (Nuklear)

BlankC Engine integrates **Nuklear** (an *immediate mode GUI library*) to handle user interfaces such as Main Menus, HUDs, and Debugging panels.

## 1. Immediate Mode GUI (IMGUI)

Unlike traditional UI systems (Retained Mode) where you create a "button object" initially and then update its text, **Immediate Mode GUI** means you define and draw the UI continuously every *frame* inside the *main loop*.

IMGUI Advantages:
- No UI state needs to be managed separately.
- Variable values are directly bound to UI elements (e.g., a slider directly changes the `float light_intensity` variable).
- Extremely fast for creating *debugging* tools or *in-game* interfaces.

## 2. Initialization

Nuklear is initialized automatically by `engine_create()`. The main Nuklear object (`struct nk_context*`) can be accessed via `engine->ui->ctx`.

## 3. UI Draw Flow

The UI creation cycle per frame in BlankC Engine:

1.  **New Frame:** `ui_new_frame()` is called at the beginning of `engine_update()`. This collects all mouse/keyboard input and prepares it for Nuklear.
2.  **Window Declaration:** You start a new window using `nk_begin()`.
3.  **Layout & Element Declaration:** You define the layout (how many columns per row) using `nk_layout_row_dynamic()`, then add elements (Texts, Buttons, Sliders).
4.  **Close Window:** You end the window declaration with `nk_end()`.
5.  **Render:** At the end of the frame, `engine_swap()` will call `ui_render()`, which takes all the declaration commands from steps 2-4 and sends them to the GPU (OpenGL) to be drawn on top of the 3D scene.

## 4. Usage Example

Here is an example of creating a simple Main Menu:

```c
struct nk_context* ctx = engine->ui->ctx;

// Start UI declaration
if (nk_begin(ctx, "Main Menu", nk_rect(50, 50, 200, 150),
    NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE)) 
{
    // Set 1 column per row, 40 pixels high
    nk_layout_row_dynamic(ctx, 40, 1);
    
    // Text Element
    nk_label(ctx, "Welcome to BlankC", NK_TEXT_CENTERED);
    
    // Button Element
    if (nk_button_label(ctx, "START GAME")) {
        // Logic when the Start button is pressed
        g_state = STATE_PLAYING;
    }
}
nk_end(ctx); // Must be called to close nk_begin
```

## 5. Handling Input Conflicts (Mouse Capture)

When you have a UI (like a Debug window) stacked on top of a 3D game, you don't want the action of clicking a UI button to also be considered "shooting a weapon" inside the game.

Use this function to check if the mouse is currently interacting with the UI:
```c
if (!ui_want_capture_mouse(engine->ui)) {
    // Mouse is not over a Nuklear UI, safe for 3D game input
    if (input_mouse_pressed(engine->input, 0)) shoot_gun();
}
```