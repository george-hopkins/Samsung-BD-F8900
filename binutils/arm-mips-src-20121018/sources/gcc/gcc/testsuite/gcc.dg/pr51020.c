/* PR 51020 */
/* { dg-do compile } */
/* { dg-options "---" } */
int main() {return 0;}
/* { dg-error "unrecognized.*'---'" ""  { target *-*-* } 0 } */
