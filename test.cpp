#include <functional>
#include <iostream>
#include <cmath>

using namespace std;

int oct(int val){
  int n = 16;
  val %= n;
  if(val<0){
    val = n+val;
  }
  return val;
}

struct Digit{
  int val = 0;
  Digit* right = nullptr;
  Digit* left = nullptr;
};

Digit* newDigit(int n){
  Digit* digit = new Digit;
  int temp = oct(n);
  digit->val = temp;
  if(temp<n){
    digit->right = newDigit(1);
    digit->right->left = digit;
  }
  return digit;
}
Digit* add(Digit* d1, Digit* d2){
  Digit* d = newDigit(0);
  if(!d1 && d2) return d2;
  if(!d2 && d1) return d1;
  int sum = oct(d1->val+d2->val);
  d->val = sum;
  if(sum<d1->val || sum < d2->val){
    if(d->right){
      d->right = add(d1->right, newDigit(1));
    }
    else
      d->right = newDigit(1);
  }
  if(d1->right || d2->right){
    if(d->right){
      d->right = add(d->right, add(d1->right, d2->right));
    }
    else {
      d->right = add(d1->right, d2->right);
    }
  }
  if(d->right)  
    d->right->left = d;
  return d;
}


struct Num{
  Digit lsb;
  Digit msb;
};


Num newNum(Digit* d){
  Num num;
  num.lsb = *d;
  function<Digit*(Digit*)> func = [&](Digit* td){
    if(td->right)
      return func(td->right);
    return td;
  };
  num.msb = *func(d);
  return num;
}

void show(Num num){
  function<void(Digit*)> func = [&](Digit* d){
    cout << d->val << " ";
    if(d->left) func(d->left);
  };
  func(&num.msb);
  cout << endl;
}

Num addNum(Num n1, Num n2){
  Digit* d = add(&n1.lsb, &n2.lsb);
  cout << "lsb: " << d->val << endl;
  return newNum(d);
}

int main(){
  Num n1 = newNum(newDigit(15));
  Num n2 = newNum(newDigit(15));
  Num res;
  res = addNum(n1, n2);
  cout << "n1: ";
  show(n1);
  cout << "n2: ";
  show(n2);
  cout << "res: ";
  show(res);
  cout << res.lsb.right->val << res.lsb.val << endl;
  return 0;
}
