Just a simple side scroller where you control a red square and can jump and move around


<img width="1681" height="857" alt="Screenshot_2026-06-20_20-10-51" src="https://github.com/user-attachments/assets/96961d13-f658-462e-85e1-13536a7dcc74" />


Bugs to fix

    1) Red Square can clip into platforms 


Ideas to implement next:

    Easy
    1) Variable jump height — if you release space early, cut vertical velocity so short taps give small jumps and held space gives full jumps
    - 2) Screen shake when landing from a high fall - DONE
    3) A collectible coin or star that disappears when touched and increments a counter displayed on screen

    Medium
    4) Expanding the map — straightforward now that the world/window separation is clean
    5) Moving platforms — just add x or y velocity to platform rects each frame and reverse when they hit a boundary
    6) A simple enemy that patrols back and forth on a platform and resets your position if touched
    7) Wall jumping — detect collision with the side of a platform and allow a jump while touching it
    8) Double jump — track jump count and allow a second jump before landing

    Hard
    9) A checkpoint system — save a respawn position when square touches certain spots
    10) A simple level system — when square reaches a goal, load a new platform layout
    11) A tile map loaded from a text file — to allow designing levels outside the code
    12) Parallax background — multiple background layers scrolling at different speeds for depth
