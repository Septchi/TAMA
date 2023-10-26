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
  Digit* left = nullptr;
  Digit* right = nullptr;
};

Digit* newDigit(int n=0){
  Digit* digit = new Digit;
  int temp = oct(n);
  digit->val = temp;
  if(temp<n){
    digit->left = newDigit(1);
    digit->left->right = digit;
  }
  return digit;
}

int compare(Digit* d1, Digit* d2){
	if(d1->val > d2->val) return 1;
	else if(d1->val < d2->val) return -1;
	return 0;
}

Digit* lShift(Digit *d){
	Digit* td;
	td = newDigit();
	td->left = newDigit(d->val);
	td->left->right = td;
	return td;
}

Digit* add(Digit* d1, Digit* d2){
  Digit* d = newDigit(0);
  if(!d1 && d2) return d2;
  if(!d2 && d1) return d1;
  int sum = oct(d1->val+d2->val);
  d->val = sum;
  if(sum<d1->val || sum < d2->val){
    if(d->left){
      d->left = add(d1->left, newDigit(1));
    }
    else
      d->left = newDigit(1);
  }
  if(d1->left || d2->left){
    if(d->left){
      d->left = add(d->left, add(d1->left, d2->left));
    }
    else {
      d->left = add(d1->left, d2->left);
    }
  }
  if(d->left)  
    d->left->right = d;
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
	if(d1->left){
		d->left = sub(d1->left, d2->left);
		d->left->right = d;
    if(d2->val>d1->val)
      d->left->val -= 1;
	}
  if(d->left)
    if(d->left->val == 0)
      d->left = nullptr;
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
	d->left = b;
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
    if(td->left)
      return func(td->left);
    return td;
  };
  num.msb = *func(d);
  return num;
}

void show(Num num){
  function<void(Digit*)> func = [&](Digit* d){
    cout << d->val << " ";
    if(d->right) func(d->right);
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
  function<Digit* (Digit*, Digit*)> func = 
	  [&](Digit* d1, Digit* d2){
	  Digit* d;
	  d = mult(d1, d2);
	  cout << "d-f: ";
	  show(newNum(d));
	  if(d1->left){
		d = lShift(d);
		cout << "here-f: ";
		show(newNum(d));
		d = add(d, func(d1->left, d2));
	  }
	  return d;
	  cout << "d-f: ";
	  show(newNum(d));
  };
  function<Digit* (Digit*, Digit*)> map = 
	  [&](Digit* d1, Digit* d2){
	  Digit* d;
	  d = func(d1, d2);
	  cout << "d-m: ";
	  show(newNum(d));
	  if(d2->left){
		  d = lShift(d);
		  d = add(d, map(d1, d2->left));
	  }
	  return d;
  };
  return newNum(map(&n1.lsb, &n2.lsb));
}

int main(){
  Num n1 = newNum(newDigit(10));
  Num n2 = newNum(newDigit(10));
  Num res;
  cout << "n1: ";
  show(n1);
  cout << "n2: ";
  show(n2);
  res = multNum(n1, n2);
  cout << "res: ";
  show(res);
  cout << res.lsb.left->val << endl;
  return 0;
}
