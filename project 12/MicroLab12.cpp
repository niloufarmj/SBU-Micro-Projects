#include <conio.h>
#include <stdio.h>

#define VEC_SIZE 50 
#define STEP_SIZE 0.2

float a_vec[VEC_SIZE];
float b_vec[VEC_SIZE];
float y_vec[VEC_SIZE];

float zi1[VEC_SIZE];	//y^2 - a
float zi2[VEC_SIZE];	//y^2 + b
float zo1[VEC_SIZE];	//y^2 + (a+b)*y + ab
float zo2[4 * VEC_SIZE];//y^2 + (a+b)*y + ab

float cin_float() {
	int c = '0';
	int neg;

	c = _getche();

	neg = (c == '-') ? -1 : 1;
	c = (c == '+' || c == '-') ? '0' : c;

	float int_part = 0;
	while (c >= '0' && c <= '9') {
		int_part = int_part * 10 + (c - '0');
		c = _getche();
	}

	//user is done entering number
	if (c != '.') return neg * int_part;

	//user entered a '.' meaning they will enter the fractional part now
	c = '0';
	int pow = 1;
	float fraction_part = 0;
	while (c >= '0' && c <= '9') {
		fraction_part = fraction_part + ((c - '0') / (float)pow);
		pow *= 10;
		c = _getche();
	}

	return neg * (int_part + fraction_part);
}

//input a and b, give value to a_vec and b_vec and y_vec
void create_vectors(float a, float b) {
	float start = 0;
	for (int i = 0; i < VEC_SIZE; i++, start += STEP_SIZE) {
		a_vec[i] = a;
		b_vec[i] = b;
		y_vec[i] = start;
	}
}

//input y[] and a[] and b[], give value to zi1 and zi2
void calculate_zi_vectors(float y[], float a[], float b[]) {
	for (int i = 0; i < VEC_SIZE; i++) {
		zi1[i] = (y[i] * y[i]) - a[i];
		zi2[i] = (y[i] * y[i]) + b[i];
	}
}

//input y[], give value to zo1
void calculate_equation_vector(float y[]) {
	for (int i = 0; i < VEC_SIZE; i++) {
		zo1[i] = 1 / (2 * zi1[i] * zi2[i]);
	}
}

//input y[], give value to zo2
void calculate_equation_vectors_SSE(float y[]) {
	// align address to a 16-byte boundary
	__declspec(align(16)) float temp_zi1[VEC_SIZE];
	__declspec(align(16)) float temp_zi2[VEC_SIZE];
	__declspec(align(16)) float one_vec[VEC_SIZE];
	__declspec(align(16)) float two_vec[VEC_SIZE];
	__declspec(align(16)) float zo2_temp[4 * VEC_SIZE];

	for (int i = 0; i < VEC_SIZE; i++) {
		temp_zi1[i] = zi1[i];
		temp_zi2[i] = zi2[i];
		one_vec[i] = 1;
		two_vec[i] = 2;
	}

	__asm {

		mov ecx, 0

		LBL:
			movaps xmm0, oword ptr temp_zi1[ecx]	// load pointer to the "ecx"th value in temp_zi1 
			movaps xmm1, oword ptr temp_zi2[ecx]	// load pointer to the "ecx"th value in temp_zi2 
			movaps xmm2, oword ptr one_vec[ecx]		// load pointer to the "ecx"th value in one_vec
			movaps xmm3, oword ptr two_vec[ecx]		// load pointer to the "ecx"th value in two_vec
			movaps xmm4, xmm0						// load 4 float values from temp_zi1 
			movaps xmm5, xmm1						// load 4 float values from temp_zi2
			movaps xmm6, xmm2						// load 4 float values from one_vec 
			movaps xmm7, xmm3						// load 4 float values from two_vec 
			mulps xmm4, xmm5						// xmm4 = xmm4 * xmm5 --> zi1 = zi1 * zi2
			mulps xmm4, xmm7						// xmm4 = xmm4 * xmm7 --> zi1 = zi1 * 2 --> (zi1 = 2 * zi1 * zi2)
			divps xmm6, xmm4						// xmm6 = xmm6 / xmm4 --> one_vec = one_vec / zi1 --> one_vec = 1 / (2 * zi1 * zi2)
			movaps oword ptr zo2_temp[ecx], xmm6	// store in zo2_temp
			add ecx, 16								// ecx = ecx + 16 : to help get the pointer to the next value in the vectors
			cmp ecx, 400							// in each loop we multiply 4 values => we need 100/4 = 25 iterations => 25*16 = 400
			jnz LBL
	}

	for (int i = 0; i < VEC_SIZE; i++) {
		zo2[i] = zo2_temp[i];
	}
}

int main() {
	printf("a = ");
	float a = cin_float();
	printf("\nb = ");
	float b = cin_float();
	printf("\n");

	create_vectors(a, b);

	calculate_zi_vectors(y_vec, a_vec, b_vec); //y^2 - a and y^2 + b

	calculate_equation_vector(y_vec); // y^2 + (a+b)*y + ab

	calculate_equation_vectors_SSE(y_vec); // y^2 + (a+b)*y + ab using SSE

	for (int i = 0; i < VEC_SIZE; i++) {
		printf("--------------------- y = %f ---------------------\n", y_vec[i]);
		printf("zi1 = %f\n", zi1[i]);
		printf("zi2 = %f\n", zi2[i]);
		printf("zo1 = %f\n", zo1[i]);
		printf("zo2 = %f\n", zo2[i]);
	}

	return 0;
}
