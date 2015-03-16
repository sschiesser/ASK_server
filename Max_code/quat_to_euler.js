outlets = 3;

var q = new Array(8);
var out = new Array(4);

var heading;
var attitude;
var bank;

function list()
{
	q = arrayfromargs(arguments);
	var w = q[0];
	var x = q[1];
	var y = q[2];
	var z = q[3];

	var sqw = w*w;
	var sqx = x*x;
	var sqy = y*y;
	var sqz = z*z;
	var unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	var test = x*y + z*w;

	if (test > 0.499 * unit) { // singularity at north pole
    	heading = 2 * Math.atan2(x,w);
    	attitude = Math.PI/2;
    	bank = 0;
	}else if (test < -0.499 * unit) { // singularity at south pole
    	heading = -2 * Math.atan2(x,w);
    	attitude = -Math.PI/2;
    	bank = 0;
	} else {
    	heading = Math.atan2(2*y*w - 2*x*z , sqx - sqy - sqz + sqw);
    	attitude = Math.asin( 2 * test / unit);
    	bank = Math.atan2(2*x*w - 2*y*z , -sqx + sqy - sqz + sqw)
	}
	outlet(0, heading);
	outlet(1, attitude);
	outlet(2, bank);
}
