# LedCube

## Tutorial - sequence.cube
### REPEAT
First argument: <b>int</b><br />
Following rows to repeat.

Second argument: <b>int</b><br />
The total duration of the repeat.

Repeats can consist of repeats.

<i>Example: repeat(1,4):</i>

### LED
#### With color
First three <i>(position 1-2-3)</i> is coordinated (<b>x, y, z</b>) where <b>1</b> is the first.<br />
Second three <i>(position 4-5-6)</i> is the color (<b>R, G, B</b>) where <b>1</b> is on, <b>0</b> is off.<br />
The last argument <i>(position 7)</i> is <b>delay</b> in <u>milliseconds</u>.<br />

<i>Example: 2,1,1,1,1,1,1</i>

#### Without color
First three <i>(position 1-2-3)</i> is coordinated (<b>x, y, z</b>) where <b>1</b> is the first.<br />
The last argument <i>(position 4)</i> is <b>delay</b> in <u>milliseconds</u>.<br />

<i>Example: 2,1,1,1</i>