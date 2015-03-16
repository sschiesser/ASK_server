
outlets = 1;

var packet = new Array(8);
var out = new Array(4);

function list()
{
	var i;
	packet = arrayfromargs(arguments);

	for(i = 0; i < 4; i++) {		
		out[i] = ( (packet[(i*2)] << 8) | packet[(i*2)+1] ) / 16384.0;
		if (out[i] >= 2.0) {
			out[i] = out[i] - 4.0f;
		}
	}
	outlet(0, out);
}