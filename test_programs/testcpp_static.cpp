struct T { T(); };
void f() {
	static T lsobj;
}


int main() {
	f();
	return 0;
}
