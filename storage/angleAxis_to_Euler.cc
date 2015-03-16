void toEuler(double x,double y,double z,double angle) {
    double s = sin(angle);
    double c = cos(angle);
    double t = 1 - c;
    //  if axis is not already normalised then uncomment this
    double magnitude = sqrt(x*x + y*y + z*z);
    if (magnitude==0) return;
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
    if ( (x*y*t +  z*s) > 0.998) { // north pole singularity detected
        heading = 2 * atan2( x * sin(angle / 2), cos(angle / 2));
        attitude = PI / 2;
        bank = 0.0;
        return;
    }
    if ((x*y*t + z*s) < -0.998) { // south pole singularity detected
        heading = -2 * atan2( x * sin(angle / 2), cos(angle / 2));
        attitude = -PI / 2;
        bank = 0;
        return;
    }
    heading = atan2(y * s- x * z * t , 1 - (y*y + z*z ) * t);
    attitude = asin(x * y * t + z * s) ;
    bank = atan2(x * s - y * z * t , 1 - (x*x + z*z) * t);
}