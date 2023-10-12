# MyApex
Simple C++ cheats for Apex Legends (Linux Steam version).
### GameMode
Select correct gamemode to glow properly on the right teammates -> #0: BR/GUNRUN, 1: CONTROL/TDM

### Sense 
All enemies will be highlighted with Cyan color.
Based on their health and shield value, they will glow differently.

### Item Glow
Rare to Legendary items in vicinity will glow.

### NoRecoil
Adjustable reduction in vertical and horizontal weapon recoil.
You can hold left shift to temporarily disable.

### Aimbot
An adjustable aimbot intended for CQC combat. 
Works of enemy posistion rather than bones. 
Only works horizontally.
You can hold left shift to temporarily disable.

## Requirements
1. Linux OS with X11
2. g++ compiler (Ubuntu 11.2.0-19ubuntu1) 11.2.0 or higher)
3. Git

### HOW TO SETUP & RUN

1. Open terminal and run the following to download the cheat source code

```
git clone https://github.com/Savage3e/Ximbot
```

2. Go inside the downloaded folder

```
cd Ximbot
```

3. Compile the code (requires sudo access)

```
g++ Main.cpp -lX11
```

4. Go to your Steam and run Apex Legends

5. Run the cheat (requires sudo access for read & write memory)

```
sudo ./a.out
```

If you see something simmilar to the text below it means the cheat is running fine. Enjoy.
```
======================== SETTINGS LOADED ========================
FEATURE_AIMBOT_ON               true
FEATURE_NORECOIL_ON             true
FEATURE_SENSE_ON                true
FEATURE_GAME_MODE               0       
SENSE_MAX_RANGE                 250 
SENSE_MAX_RANGE_OVER_WALL       250 
AIMBOT_TRIGGER                  0
AIMBOT_SMOOTHING                10
AIMBOT_ACTIVATION_FOV           9
AIMBOT_MAX_RANGE                69
NORECOIL_PITCH_STRENGTH         0.590000
NORECOIL_YAW_STRENGTH           0.500000
=================================================================

UPDATE[1000] OK. 
UPDATE[0] OK. 
UPDATE[1000] OK. 
UPDATE[0] OK. 

```











