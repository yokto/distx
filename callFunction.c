int three() {
	return 3;
}

static int b() {
	three();
	return 4;
}

int a() {
	three();
	b();
	return b();
}
