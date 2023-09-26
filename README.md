# MyApex
Simple C++ cheats for Apex Legends (Linux Steam version).
### GameMode
Select correct gamemode to glow properly on the right teammates -> #0: BR/GUNRUN, 1: CONTROL/TDM

### Sense 
All enemies will be highlighted with Cyan color.
Based on their health value, they will glow differently.
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
git clone https://github.com/ximbot/myapex
```

2. Go inside the downloaded folder

```
cd myapex
```

3. Compile the code 

```
g++ Main.cpp -lX11
```

4. Go to your Steam and run Apex Legends

5. Run the cheat (requires sudo access for read & write memory)

```
sudo ./a.out
```

You should now see a lot of lines being printed saying "LOOP OK...." which means the cheat is running fine. Enjoy.













