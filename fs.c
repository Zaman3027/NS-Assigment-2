#include<stdio.h>
#include "../miracl.h"

big bigPow(big a, big b);
int main() {
    miracl *mir = mirsys(50000,10);
    big prime = mirvar(0);
    big generator = mirvar(0);
    irand(2324);
    bigbits(32,prime);
    bigbits(3,generator);
    //makes prime
    nxprime(prime,prime);
    nxprime(generator,generator);
    printf("Prime Number : ");
    otnum(prime,stdout);
    printf("Generator : ");
    otnum(generator,stdout);

    big n = mirvar(0);
    multiply(prime,generator,n);
    printf("Product(g,prime) ->n : ");
    otnum(n,stdout);

    // Co-prime of n

    big s = mirvar(0);
    big t = mirvar(0);
    big z = mirvar(0);
    big o = mirvar(0);
    convert(0,z);
    convert(1,o);
    do{
        bigrand(n,s);
        egcd(s,n,t);
    }while (mr_compare(s,z)==0|| 
    mr_compare(s,o)==0||
    mr_compare(t,o)!=0);
    
    printf("Co-Prime to n -> s: ");
    otnum(s,stdout);

    //compute V
    big v = mirvar(0);
    powmod(s,mirvar(2),n,v);
    printf("s^2mod(n) -> v:");
    otnum(v,stdout);

    //Step 3 round

    big r = mirvar(0);
    irand(2526);
    bigrand(n,r);
    big x = mirvar(0);
    powmod(r,mirvar(2),n,x);
    
    otnum(x,stdout);
    big e = mirvar(1);
    big y = mirvar(0);

    big k = mirvar(0); //s^e
    k = bigPow(s,e);
    multiply(r,k,k);
    powmod(k,mirvar(1),n,y);
    printf("(r * s^e) mod n y-> :");
    otnum(y,stdout);
    
    big y_mod_n = mirvar(0);
    powmod(y,mirvar(2),n,y_mod_n);

    big l = bigPow(v,e);
    multiply(l,x,l);
    powmod(l,mirvar(1),n,l);

    if(mr_compare(l,y_mod_n)==0) {
        printf("Autorized \n");
    }else printf("Unauthorized \n");


    return 0;
}

big bigPow(big a, big b) {
    big c = mirvar(0);
    if(mr_compare(b,mirvar(0))==0) return mirvar(1);
    subdiv(b,2,c);
    big k = bigPow(a,c);
    big d = mirvar(1);
    multiply(k,k,d);
    if(!subdivisible(b,2)) {
        multiply(d,a,d);
    }
    return d;
}