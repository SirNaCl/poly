#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "error.h"
#include "poly.h"

typedef struct term_t term_t;

/**
 * Steg:
 * Poly är en länkad lista av termer
 * Multiplicera term för term (egen struct, (koefficient, exponent, next))
 *      ny term = koef*koef*^(exp+exp)
 * Ger flera polynom som ska adderas
 * för varje term i poly 1: addera koefficienter med termer av samma grad i övriga poly
 *
 */

struct term_t
{
    int coef;
    int exp;
    term_t *next;
};

struct poly_t
{
    term_t *next;
};

int str_nextint(const char *s, int *step)
{
    int l = 0;
    while (isdigit(*(s + l)))
        l++;

    char r[l];
    strncpy(r, s, l);
    *step = l;
    return atoi(r);
}

void free_term(term_t *t)
{
    if (t->next)
        free_term(t->next);

    free(t);
}

/**
 * @brief Insert term into polynomial in correct position (increase coefficient if term with correct exponent exists)
 *
 * @param p target polynomial
 * @param t term to be inserted
 */
void insert_term(poly_t *p, term_t *t)
{
    term_t *n = p->next;
    term_t *prev;

    // Loop until we find correct location in polynomial
    while (n && t->exp <= n->exp)
    {
        prev = n;
        n = n->next;
    }

    if (prev)
    {
        // Found term with same exponent
        if (prev->exp == t->exp)
        {
            prev->coef += t->coef;
            free_term(t);
        }
        else
        {
            prev->next = t;
            t->next = n;
        }
    }
    else // First insertion or new largest
    {
        p->next = t;
        t->next = NULL;
    }
}

poly_t *new_poly_from_string(const char *s)
{
    poly_t *p = (poly_t *)malloc(sizeof(poly_t));
    p->next = NULL;

    const char *cpy = s;
    int negative = 0;
    int step = 0;
    while (strlen(cpy) > 0)
    {
        negative = 0; // used to check if next coefficient should be negative
        term_t *t = (term_t *)malloc(sizeof(term_t));
        t->next = NULL;
        t->coef = 0;
        t->exp = 0;

        // Space between terms
        if (*cpy == ' ')
        {
            if (*(cpy + 1) == '-')
                negative = 1;

            cpy += 3;
        }

        // Term has coefficient
        if (isdigit(*cpy))
        {
            t->coef = str_nextint(cpy, &step);
            cpy += step;
            if (negative)
                t->coef = -t->coef;
        }
        else
            t->coef = 1;

        // Term exponent
        if (*cpy == 'x')
        {
            cpy++;
            if (*cpy == '^')
            {
                cpy++;
                t->exp = str_nextint(cpy, &step);
                cpy += step;
            }
            else
                t->exp = 1;
        }
        insert_term(p, t);
        negative = 0;
    }

    return p;
}

void free_poly(poly_t *p)
{
    if (p->next)
        free_term(p->next);
    free(p);
}

term_t *mul_terms(term_t *t1, term_t *t2)
{
    term_t *r = (term_t *)malloc(sizeof(term_t));
    r->next = NULL;
    r->coef = t1->coef * t2->coef;
    r->exp = t1->exp + t2->exp;
    return r;
}

poly_t *mul(poly_t *p1, poly_t *p2)
{
    term_t *t1, *t2;
    poly_t *r = (poly_t *)malloc(sizeof(poly_t));

    r->next = NULL;
    t1 = p1->next;

    while (t1)
    {
        t2 = p2->next;
        while (t2)
        {
            insert_term(r, mul_terms(t1, t2));
            t2 = t2->next;
        }
        t1 = t1->next;
    }

    return r;
}

void print_poly(poly_t *p)
{
    term_t *n = p->next;

    while (n)
    {
        if (abs(n->coef) > 1 || (abs(n->coef) > 0 && !n->exp))
            printf("%d", abs(n->coef));

        if (n->exp > 1)
            printf("x^%d", n->exp);
        else if (n->exp > 0)
            printf("x");

        if (n->next)
        {
            if (n->next->coef < 0)
                printf(" - ");

            else
                printf(" + ");
        }

        n = n->next;
    }
    printf("\n");
}