/* Testing Code */

#include <limits.h>
#include <math.h>

/* Routines used by floation point test code */

/* Convert from bit level representation to floating point number */
float u2f(unsigned u) {
  union {
    unsigned u;
    float f;
  } a;
  a.u = u;
  return a.f;
}

/* Convert from floating point number to bit-level representation */
unsigned f2u(float f) {
  union {
    unsigned u;
    float f;
  } a;
  a.f = f;
  return a.u;
}

int test_tconst(void){
  return 0xFFFFFFE0;
}
int test_bitNand(int x, int y){
  return ~(x&y);
}
int test_ezOverflow(int x,int y){
  long long res=(long long)x+(long long)y;
  return res>2147483647;
}
int test_fastModulo(int x,int y){
  long long yy=1;
  for(int i=1;i<=y;++i)yy*=2;
  return x%yy;
}
int test_findDifference(int x,int y){
  return x^y;
}
int test_absVal(int x){
  return x<0?-x:x;
}
int test_secondLowBit(int x){
  int y=x-(x&-x);
  return y&(-y);
}
int test_byteSwap(int x, int n, int m){
  if(n==m)return x;
  int dis_n = n<<3;
  int dis_m = m<<3;
  int a = (x>>dis_n)&0xff;
  int b = (x>>dis_m)&0xff;
  int xor = a^b;
  x^=(xor<<dis_n);
  x^=(xor<<dis_m);
  return x; 
}
int test_byteCheck(int x){
  union tests
  {
    int a;
    char b[4];
  }t;
  int ans=0;
  t.a=x;
  for(int i=0;i<4;++i)if(t.b[i]!=0)++ans;
  return ans;
}
int test_fractions(int x){
  return x*7/16;
}
int test_biggerOrEqual(int x, int y){
  return x >= y;
}
int test_hdOverflow(int x,int y){
  long long res=(long long)x+(long long)y;
  return res>2147483647||res<-2147483648;
}
int test_overflowCalc(int x, int y, int z){
  union tests
  {
    int a;
    unsigned b;
  }u,v,w;
  u.a=x;
  v.a=y;
  w.a=z;
  long long res=(long long)u.b+(long long)v.b+(long long)w.b;
  return res>>32;
}
int test_logicalShift(int x, int n) {
  unsigned u = (unsigned) x;
  unsigned shifted = u >> n;
  return (int) shifted;
}
int test_partialFill(int l, int h) {
  int ans=0;
  for(int i=l;i<=h;++i){
    if(i%2==0)ans+=1<<i;
  }
  return ans;
}

unsigned test_float_abs(unsigned uf) {
  float f = u2f(uf);
  if (isnan(f))
    return uf;
  if(f==0&&uf>>31)return 0;
  return f<0?f2u(-f):uf;
}
unsigned test_float_cmp(unsigned uf1, unsigned uf2) {
  float f1 = u2f(uf1), f2 = u2f(uf2);
  if (isnan(f1) || isnan(f2))
    return 0;
  return f1 > f2;
}
unsigned test_float_pow2(unsigned uf,int n) {
  float f = u2f(uf);
  if (isnan(f))
    return uf;
  for(int i=1;i<=n;++i)f*=2;
  for(int i=-1;i>=n;--i)f/=2;
  return f2u(f);
}
unsigned test_float_i2f(int x) {
  float f = (float) x;
  return f2u(f);
}

int test_oddParity(int x){
  int answer=0;
  for(int i=1;i<=32;++i){
    answer+=x%2;
    x>>=1;
  }
  return answer%2?0:1;
}
int test_bitReverse(int x){
  int ans=0;
  for(int i=0;i<32;i++){
    ans<<=1;
    ans|=(x&1);
    x>>=1;
  }
  return ans;
}
int test_mod7(int x){
  return x%7;
}