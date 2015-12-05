/* multilib hello world
 *
 * gcc $BUILD64 -D__BUILD=64 -o foo64 foo.c
 * gcc $BUILD32 -D__BUILD=32 -o foo32 foo.c
 */
#include <stdio.h>
int main(int argc, char **argv) {
	printf("hello world (%dbit)\n", __BUILD);
	return 0;
}
