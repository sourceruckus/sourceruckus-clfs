/* multilib hello world
 *
 * gcc $BUILD64 -o foo64 foo.c
 * gcc $BUILD32 -o foo32 foo.c
 */
#include <stdio.h>
int main(int argc, char **argv) {
	printf("hello world\n");
	return 0;
}
