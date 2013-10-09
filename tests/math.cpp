
#include <iostream>
#include <stdlib.h>

using namespace std;

#include "../math/vec2.h"

int main() {

	vec2 v1(1.0f , 2.0f);
	vec2 v2(3.0f , 5.2f);
	
	vec2 v3 = v1 + v2;
	float v4 = v1 * v2;
	
	cout << v3 << endl;
	cout << v4 << endl;

	return EXIT_SUCCESS;
}