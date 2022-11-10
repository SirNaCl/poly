#include <stdio.h>
#include "poly.h"

static void poly_test(const char *a, const char *b)
{
	poly_t *p;
	poly_t *q;
	poly_t *r;

	printf("Begin polynomial test of (%s) * (%s)\n", a, b);

	p = new_poly_from_string(a);
	q = new_poly_from_string(b);

	print_poly(p);
	print_poly(q);
	r = mul(p, q);

	print_poly(r);

	free_poly(p);
	free_poly(q);
	free_poly(r);

	printf("End polynomial test of (%s) * (%s)\n", a, b);
}

int main(void)
{
	poly_test("x^2 - 7x + 1", "3x + 2");
	putchar('\n');
	poly_test("x^10000000 + 2", "2x^2 + 3x + 4");
	poly_test("403x^43 + 702", "389x^979 + 515x^940 - 735x^868 - 22x^831 - 63x^741 + 487x^734 - 117x^710 + 594x^668 + 349x^582 + 403x^572 - 383x^523 - 373x^454 + 444x^431 - 880x^403 + 722x^351 - 728x^324 + 303x^235 + 896x^211 - 533x^191 - 943x^129 + 643x^83 - 411x^63 - 786x^2 - 113");

	return 0;
}
