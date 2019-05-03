import goertzel
import numpy as np
import time


n = 5000
d = np.sin(np.linspace(0,10*np.pi,n)+np.pi)
t = time.time()
m,ang = goertzel.goertzelFilter(d,1000,n*200)
print(m,ang)