
# Leander Flossie Engine

One of the key design choices I made with my engine was my inplementation of the Event System. 
Since I was interested in the signals from GODOT, I tried to make a cpp version of it that got more complicated the more time went into it.
it makes use of slots to encapsulate the use of menber functions allonf with the STD:functional libarary

the controls are:
game:
f1 = skip level
f2 = mute sounds
arrow keys / dpad = movement
space / right button = place bomb
z / down button = detonate all bombs (if you have the remote detonation powerup from level 3)

highscore / menu movement:
esc / start = back to main menu (if youre in the highscore screen)
up / down (also dpad) = swap between modes
space / enter / down button = select gamemode



GitHub repo for the Engine:
https://github.com/SebastianMaxG/Prog-4-Game-Engine

GitHub repo for the Signal (includes some basic examples)
https://github.com/SebastianMaxG/SignalAndSlots