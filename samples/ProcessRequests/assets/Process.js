var request = new Request()

function Process(input) {
	input.path = "Hello";
	log(input.path);
	log(input.referrer);
	log(input.host);
	log(input.userAgent);
}