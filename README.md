# LedCube

## Tutorial - sequence.cube
### REPEAT
First argument: int
Number of following rows to repeat
Second argument: int
The total duration of the repeat

Example: repeat(1,4):

### LED
#### With color
First three (pos 1-2-3) is coordinated (x, y, z) where 1 is the first.
Second three (pos 4-5-6) is the color (R, G, B) where 1 is on, 0 is off.
The last argument (pos 7) is delay in milliseconds.

Example: 2,1,1,1,1,1,1

#### Without color
First three (pos 1-2-3) is coordinated (x, y, z) where 1 is the first.
The last argument (pos 4) is delay in milliseconds.

Example: 2,1,1,1
