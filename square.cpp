#include <iostream>
#include <cmath>

using namespace std;
typedef long long unsigned int maxInt; 

const maxInt maxn = -1;

struct baseMax{
  maxInt base=0;
  maxInt exp=0;
};


baseMax newBase(int val){
  return baseMax{.base=unsigned(val)};
}

baseMax add(baseMax b1, baseMax b2){
  maxInt sum = b1.base + b2.base;
  return baseMax{.base=sum, .exp=b1.exp+b2.exp+(sum<b1.base || sum<b2.base ? 1 : 0)};
}
//assume b1>b2.
baseMax sub(baseMax b1, baseMax b2){
  maxInt diff;
  if(b2.base>=b1.base){
    diff = b2.base - b1.base;
    return baseMax{.base=-diff, .exp=b1.exp-b2.exp-1};
  }
  return baseMax{.base=b1.base-b2.base, .exp=b1.exp-b2.exp};
}

bool isEven(baseMax b){
  return b.base%2 == 0;
}

int compare(baseMax b1, baseMax b2){
  if(b1.exp > b2.exp) return 1;
  if(b1.exp < b2.exp) return -1;
  if(b1.base > b2.base) return 1;
  if(b1.base < b2.base) return -1;
  else return 0;
}

baseMax mod(baseMax b, maxInt n){
  baseMax ans;
  baseMax modbase = newBase(n);
  ans.base = ((maxn % modbase.base + 1) % modbase.base * b.exp + b.base)%modbase.base;
  return ans;
}

baseMax mult(baseMax b1, baseMax b2){
  baseMax ans;
  maxInt a = maxn/b2.base;
  maxInt b = b1.base/a;
  ans.exp = b;
    
  maxInt temp = maxn%b2.base+1;
  ans = sub(ans, mult(newBase(temp),newBase(b)));
  temp = (b1.base-a*b);
  ans = add(ans, newBase(b2.base*temp));
  return ans;
}
baseMax square(baseMax b){
  baseMax ans;
  maxInt a = maxn/b.base;
  ans.exp = b.base/a;
  if(ans.exp == 0){
    ans.base = b.base*b.base;
    return ans;
  }
  maxInt temp = ((maxn%b.base)+1)%b.base;
  ans = sub(ans, newBase(temp*b.base));

  return ans;
}
int main(){
  int n, m;
  cin >> n >> m;
  baseMax s=newBase(0);
  baseMax a, b, c;
  for(int i=0;i<n;i++){
    if(i==0)
      c = newBase(2);
    else if(i==1)
      c = newBase(1);
    else
     c = add(a, b);
    b = a;
    a = c;
    if(isEven(c)){
      s = add(s, square(c));
    }
  }
  cout << s.exp << " " << s.base << endl;
  cout << mod(s, m).base << endl;
  return 0;
}
