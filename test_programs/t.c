#include <sys/mman.h>
#include <stdio.h>

int main() {
	printf("PROT_READ %x", PROT_READ);
	printf("MAP_PRIVATE %x", MAP_PRIVATE);
	printf("MAP_DENYWRITE %x", MAP_DENYWRITE);

}
