Just a simple side scroller where you control a red square and can jump and move around


<img width="1681" height="857" alt="Screenshot_2026-06-20_20-10-51" src="https://github.com/user-attachments/assets/96961d13-f658-462e-85e1-13536a7dcc74" />





Ideas to implement next:

    Easy
    1) Variable jump height — if you release space early, cut vertical velocity so short taps give small jumps and held space gives full jumps
    2) Screen shake when landing from a high fall
    3) A collectible coin or star that disappears when touched and increments a counter displayed on screen

    Medium
    4) Expanding the map — straightforward now that the world/window separation is clean
    5) Moving platforms — just add x or y velocity to platform rects each frame and reverse when they hit a boundary
    6) A simple enemy that patrols back and forth on a platform and resets your position if touched
