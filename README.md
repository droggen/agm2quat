# agm2quat
Conversion of acceleration-gyro-magnetic field to quaternion orientation

Companion tool to BlueSense

This is a quick hack - no memory release, etc. 

Example: sample.dat is a recording with AGM from col 2-9 and quaternions in 10-13. The following command will recompute the quaternions from the raw data
agm2quat -if sample.dat -of sample.cnv -c 2 -b 0.35 -f 500 -mds

The parameter -mds emulates the behaviour of Bluesense which, due to computational complexity, is not processing the magnetic field data at each sample.
This leads to some "jerkiness" in the quaternions. Converting the raw data without the -mds parameter addresses this.


