# Compiling
You must have devkitARM installed. 

A copy of the unprotected half of the arm9 bootrom named `boot9.bin` is required in the `data` directory. Note that it must be trimmed to 32K. Additionally, you must provide the stage 2 key. Place it in the array provided in `main.c`.
