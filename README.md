# goertzel-filter-optimized
Goertzel Filter extension for python (Optimized in C)

### How to install:
- clone the repo<br>
```git clone https://github.com/buckerman/goertzel-filter-optimized.git```
- build and install the module<br>
```sudo python3 setup.py install```


### How to use it:
```python
from goertzel import goertzelFilter
#mag, phase = goertzelFilter(signal_np_array,freq,samplerate[,numOfElements])

```