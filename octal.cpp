#include <iostream>
#include <cmath>

using namespace std;
int oct(int val){
  if (val>7) cout << "overloaded: " << val << endl;
  if (val<0){
    cout << "overloaded" << val << endl;
    val = 8+val;
  }
  return val%8;
}
struct Octal{
  int base=0;
  int exp =0;
};

Octal newOctal(int val){
  return Octal{.base=val%8, .exp=val/8};
}
Octal maxBase = newOctal(7);

Octal add(Octal o1, Octal o2){
  int sum = oct(o1.base + o2.base);
  return Octal{.base=sum, .exp=oct(o1.exp+o2.exp+(sum<o1.base || sum<o2.base ? 1: 0))};
}

//Assume o1 > o2
Octal sub(Octal o1, Octal o2){
  if(o2.base>=o1.base)
    return Octal{.base=oct(-(o2.base-o1.base)), .exp=oct(o1.exp-o2.exp-1)};
  return Octal{.base=oct(-(o2.base-o1.base)), .exp=oct(o1.exp-o2.exp)};
}

bool check(Octal o){
  return o.base%2==0;
}

int compare(Octal o1, Octal o2){
  if(o1.exp > o2.exp) return 1;
  if(o1.exp < o2.exp) return -1;
  if(o1.base > o2.base) return 1;
  if(o1.base < o2.base) return -1;
  else return 0;
}

Octal mult(Octal o1, Octal o2){
  Octal ans;
  Octal maxo = compare(o1, o2) == 1 ? o1 : o2;
  Octal mino = compare(o1, o2) == -1 ? o1 : o2;
  int a = 7/o2.base;
  int b = o1.base/a;
  ans.exp = b;
  if(b==0){
    ans.base = o2.base * o2.base;
    return ans;
  }
  int temp = (7%o2.base)+1;
  ans = sub(ans, newOctal(temp*b));
  temp = o1.base - (a*b);
  ans = add(ans, newOctal(o2.base*temp));
  return ans;
}

Octal square(Octal o){
  Octal ans;
  int a = 7/o.base;
  int b = o.base/a;
  ans.exp = b;
  if(b==0){
    ans.base = o.base * o.base;
    return ans;
  }
  int temp = ((7%o.base)+1);
  ans = sub(ans, newOctal(temp*b));
  temp = (o.base-(a*b));
  ans = add(ans, newOctal(o.base*temp));
  return ans;
}

Octal mod(Octal b, Octal n){
  Octal ans;
  ans.base = ((7 % n.base + 1) % n.base * b.exp % n.base + b.base)%n.base;
  return ans;
}

int main(){
  int x, y;
  cin >> x >> y;
  Octal o1 = newOctal(x);
  Octal o2 = newOctal(y);
  Octal res;
  res = mult(o1, o2);
  cout << "o1: " << o1.exp << o1.base << endl;
  cout << "o2: " << o2.exp << o2.base << endl;
  cout << res.exp << res.base << endl;
  return 0;
}
