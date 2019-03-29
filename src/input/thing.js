const readline = require('readline');

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

let stage = 'test'
let bookings = [];
let testInfo = [];
let currTestCase = 1;
let totalTestCases = Infinity;

checkIntersect = (large, smol) => {
	if ((smol[1] <= large[1] && smol[1] >= large[0]) ||
		(smol[0] >= large[0] && smol[0] <= large[0]))
		return -1;
	return 0;
}

rl.on('line', (line) => {
	if (stage == 'test') {
		totalTestCases = parseInt(line);
		stage = 'newCase';
	} else if (stage == 'newCase') {
		testInfo = line.split(' ').map(x => parseInt(x));
		stage = 'info';
	} else if (stage == "info") {
		tmp = line.split(' ').map(x => parseInt(x));
		bookings.push([tmp[0], tmp[1], tmp[1] - tmp[0] + 1]);
		if (bookings.length == testInfo[1])
			stage = 'solve';
	} if (stage == 'solve') {
		let ret = 0;
		bookings.sort((a, b) => {
			return (a[2] < b[2]);
		});

		for (let i = 1; i  < bookings.length; i++) {
			ret += checkIntersect(bookings[0], bookings[i]);
		}

		console.log('Case #' + currTestCase + ': ' + ret);
		bookings.length = 0;
		++currTestCase;
		currTestCase > totalTestCases ? rl.close() : (stage = 'newCase');
	}
}).on('close', () => {
	process.exit(0);
});