CoordinateSystem-Android.md

Definition of the coordinate system used below.


The X axis refers to the screen's horizontal axis (the small edge in portrait mode, the long edge in landscape mode) and points to the right.

The Y axis refers to the screen's vertical axis and points towards the top of the screen (the origin is in the lower-left corner).

The Z axis points toward the sky when the device is lying on its back on a table.

IMPORTANT NOTE: The axis are swapped when the device's screen orientation changes. To access the unswapped values, use indices 3, 4 and 5 in values[].

SENSOR_ORIENTATION, SENSOR_ORIENTATION_RAW:

All values are angles in degrees.

values[0]: Azimuth, rotation around the Z axis (0<=azimuth<360). 0 = North, 90 = East, 180 = South, 270 = West

values[1]: Pitch, rotation around X axis (-180<=pitch<=180), with positive values when the z-axis moves toward the y-axis.

values[2]: Roll, rotation around Y axis (-90<=roll<=90), with positive values when the z-axis moves toward the x-axis.

Note that this definition of yaw, pitch and roll is different from the traditional definition used in aviation where the X axis is along the long side of the plane (tail to nose).

