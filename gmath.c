#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  i = addc(calculate_ambient(alight, areflect),calculate_diffuse(light, dreflect, normal),calculate_specular(light, sreflect, view, normal));
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[0];
  a.green = alight.green * areflect[1];
  a.blue = alight.blue * areflect[2];
  return a;
}

color addc(color a, color b, color c){
  color * d = malloc(sizeof(color));
  d->red = a.red + b.red + c.red;
  d->green = a.green + b.green + c.green;
  d->blue = a.blue + b.blue + c.blue;
  limit_color(d);
  return *d;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double *lcolor = light[1];
  double *lvector = light[0];
  normalize(lvector);
  normalize(normal);
  d.red = lcolor[0] * dreflect[0] * dot_product(lvector, normal);
  d.green = lcolor[1] * dreflect[1] * dot_product(lvector, normal);
  d.blue = lcolor[2] * dreflect[2] * dot_product(lvector, normal);
  if (dot_product(lvector, normal) <= 0) {
    d.red = 0;
    d.blue = 0;
    d.green = 0;
  }
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  double *lcolor = light[1];
  double *lvector = light[0];
  normalize(lvector);
  normalize(normal);
  normalize(view);
  
  s.red = (int)(lcolor[0] * sreflect[0] * exponent((dot_product(addv(scalev(2.0 * dot_product(normal, lvector), normal), scalev( -1.0, lvector)), view)), 16));
  s.green = (int)(lcolor[1] * sreflect[1] * exponent((dot_product(addv(scalev(2.0 * dot_product(normal, lvector), normal), scalev( -1.0, lvector)), view)), 16));
  s.blue = (int)(lcolor[2] * sreflect[2] * exponent((dot_product(addv(scalev(2.0 * dot_product(normal, lvector), normal), scalev( -1.0, lvector)), view)), 16));
  

  if (dot_product(normal, lvector) <= 0) {
    s.red = 0;
    s.blue = 0;
    s.green = 0;
  }
  return s;
}

double exponent(double a, int b){
  int i;
  int aa = 1;
  for(i = 0; i < b; i++){
    aa *= a;
  }
  return aa;
}

double* addv(double a[3], double b[3]){
  double * c = (double *)malloc(3 * sizeof(double));
  c[0] = a[0] + b[0];
  c[1] = a[1] + b[1];
  c[2] = a[2] + b[2];
  return c;
}

double* scalev(double a, double b[3]){
  double * c = (double *)malloc(3 * sizeof(double));
  c[0] = a * b[0];
  c[1] = a * b[1];
  c[2] = a * b[2];
  return c;
}

//limit each component of c to a max of 255
void limit_color( color * c ) {
  if(c->red > 255){
    c->red = 255;
  }
  if(c->green > 255){
    c->green = 255;
  }

  if(c->blue > 255){
    c->blue = 255;
  }
  if(c->red < 0){
    c->red = 0;
  }
  if(c->green < 0){
    c->green = 0;
  }

  if(c->blue < 0){
    c->blue = 0;
  }
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double length = sqrt(dot_product(vector, vector));
  vector[0] /= length;
  vector[1] /= length;
  vector[2] /= length;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
