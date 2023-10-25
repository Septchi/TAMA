#include <functional>
#include <iostream>
#include <cmath>

using namespace std;

int oct(int val){
  int n = 8;
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

int compare(Digit* d1, Digit* d2){
	if(d1->val > d2->val) return 1;
	else if(d1->val < d2->val) return -1;
	return 0;
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
//assume d1 > d2
Digit* sub(Digit* d1, Digit* d2){
	Digit* d = newDigit(0);
  if(!d2){
    d = d1;
    return d;
  }
	int diff = oct(d1->val-d2->val);
	d = newDigit(diff);
	if(d1->right){
		d->right = sub(d1->right, d2->right);
		d->right->left = d;
    if(d2->val>d1->val)
      d->right->val -= 1;
	}
  if(d->right)
    if(d->right->val == 0)
      d->right = nullptr;
	return d;
}
Digit* mult(Digit* d1, Digit* d2){
	Digit* d = newDigit(0);
	Digit* mind = compare(d1, d2) == -1 ? d1 : d2; 
	Digit* maxd = compare(d1, d2) != -1 ? d1 : d2; 
  if(mind->val == 0) return d;
  if(mind->val == 1) return maxd;
	Digit* a = newDigit(7/mind->val); 
	Digit* b = newDigit(maxd->val/a->val); 
	if(b->val == 0){
		d->val = mind->val*maxd->val;
		return d;
	}
	d->right = b;
	Digit* temp = newDigit(7%mind->val+1);
	d = sub(d, mult(temp, b));
	a = newDigit(7/mind->val); 
	b = newDigit(maxd->val/a->val); 
	temp = sub(maxd,  mult(a, b));
  d = add(d, mult(temp, mind));
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
  return newNum(d);
}

Num subNum(Num n1, Num n2){
	Digit* d = sub(&n1.lsb, &n2.lsb);
	return newNum(d);
}

Num multNum(Num n1, Num n2){
  function<Digit* (Digit*, Digit*)> func = [&](Digit* d1, Digit* d2){
    return mult(d1, d2);
  };
  Digit* d = mult(&n1.lsb, &n2.lsb);
  return newNum(d);
}

int main(){
  Num n1 = newNum(newDigit(8));
  Num n2 = newNum(newDigit(7));
  Num res;
  res = multNum(n1, n2);
  cout << "n1: ";
  show(n1);
  cout << "n2: ";
  show(n2);
  cout << "res: ";
  show(res);
  cout << res.lsb.val << endl;
  return 0;
}
