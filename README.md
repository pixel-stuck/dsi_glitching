# nand_payload

A small arm7 loader payload that allows easily swapping out code to run after a successful glitch.

# sd_payload

The payload used to simulate the flow of the arm7 bootrom in order to progress the state of the arm9 bootrom to the lockout phase. Arm7 controls the arm9 ROM lockout, and arm9 just waits. This allows a large, relatively imprecise window with multiple tries for skipping the check and progressing past the arm9 rom without actually locking it. It uses eMMC commands to indicate progress, both for debugging and scheduling the glitch with the FPGA.

# fake boot9

TODO