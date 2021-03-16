#!/usr/bin/python3
from subprocess import PIPE, Popen, TimeoutExpired
import os
density = 0.01

while density <= 0.9:
    print('\n\t\t\033[1;31;48m *--> PID: ', os.getpid(), '\033[1;33;48m')

    cmd = './TModelCA++ {0:.2f}'.format(density)
    os.system(cmd)
    density = density + 0.01
