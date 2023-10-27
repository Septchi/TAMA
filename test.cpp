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


struct Num{
  Digit lsb;
  Digit msb;
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
<<<<<<< HEAD
<<<<<<< HEAD
Digit* lShift(Digit *d){
  Digit* td = newDigit();
  d->right = newDigit();
  td = d->right;
  td->left = d;

	return td;
=======

Digit* lShift(Digit *d){
  Digit* td = newDigit();
  d->right = newDigit();
  d->right->left = d;
  cout << "post-shift: ";
  if(d->left) cout << d->left->val;
  cout << d->val << d->right->val << endl;
  cout << "post-shift: ";
  
  
	return d->right;
>>>>>>> noNum
}
=======
// Digit* lShift(Digit *d){
//   Digit* td = newDigit();
//   d->right = newDigit();
//   d->right->left = d;
//   cout << "post-shift: ";
//   if(d->left) cout << d->left->val;
//   cout << d->val << d->right->val << endl;
//   cout << "post-shift: ";
//   
//   
// 	return d->right;
// }
>>>>>>> parent of 90b692d (fixed multiplication)

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

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> parent of 90b692d (fixed multiplication)


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
<<<<<<< HEAD
  function<void(Digit*)> func = [&func](Digit* d){
=======
  function<void(Digit*)> func = [&](Digit* d){
>>>>>>> parent of 90b692d (fixed multiplication)
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

<<<<<<< HEAD
Num multNum(Num n1, Num n2){
=======
Digit* multNum(Digit* d1, Digit* d2){
>>>>>>> noNum
=======
Digit* lShift(num);
Num multNum(Num n1, Num n2){
>>>>>>> parent of 90b692d (fixed multiplication)
  function<Digit* (Digit*, Digit*)> func = 
	  [&func](Digit* d1, Digit* d2){
	  Digit* d = newDigit();
	  d = mult(d1, d2);
	  if(d1->left){
		// d = lShift(d);
      Digit* temp = newDigit();
      temp = func(d1->left, d2);
      temp = lShift(temp);
      cout << "temp: ";
      show(newNum(temp));
      d = add(d, temp);
	  }
	  return d;
  };
  function<Digit* (Digit*, Digit*)> map = 
	  [&map, &func](Digit* d1, Digit* d2){
	  Digit* d;
	  d = func(d1, d2);
	  cout << "d-m: ";
	  show(newNum(d));
	  if(d2->left){
		  // d = lShift(d);
      // cout << "here-m: ";
      // show(newNum(map(d1,d2->left)));
      // cout << "here-m-2: ";
      // show(newNum(lShift(map(d1,d2->left))));
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
  // show(newNum(mult(newDigit(1), newDigit(2))));
  return 0;
}
