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

void show(Digit* d){
  if(d->left) show(d->left);
  cout << d->val << " ";
}

int compare(Digit* d1, Digit* d2){
	if(d1->val > d2->val) return 1;
	else if(d1->val < d2->val) return -1;
	return 0;
}

Digit* lShift(Digit* d){
	Digit* td = newDigit();
	d->right = newDigit();
	td = d->right;
	td->left = d;
	return td;
}

Digit* rShift(Digit* d){
	return d->left;
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
  if(mind->val <= 1) return newDigit(d1->val*d2->val);
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
// assume d1 > d2;
Digit* div(Digit* d1, Digit* d2){
	Digit* d;
	if(d1->val < d2->val){
		int a = 7/d2->val;
		int b = d1->left->val/a;
		cout << "test: " << d1->left->val << endl;
		cout << "a: " << a << endl;
		cout << "b: " << b << endl;
		d = newDigit(b);
		Digit* temp = newDigit(7%d2->val+1);
		d = add(d,  div(add(newDigit(d1->val), mult(temp, d2)), d2));
	}
	else{
		d = newDigit(d1->val/d2->val);
	}
	return d;
}
Digit* multNum(Digit* d1, Digit* d2){
  function<Digit* (Digit*, Digit*)> func = 
	  [&func](Digit* d1, Digit* d2){
	  Digit* d = newDigit();
	  d = mult(d1, d2);
	  if(d1->left){
      d = add(d, lShift(func(d1->left, d2)));
	  }
	  return d;
  };
  function<Digit* (Digit*, Digit*)> map = 
	  [&map, &func](Digit* d1, Digit* d2){
	  Digit* d;
	  d = func(d1, d2);
	  if(d2->left){
		  d = add(d, lShift(map(d1, d2->left)));
	  }
	  return d;
  };
  return map(d1, d2);
}

int main(){
  Digit* n1 = newDigit(7);
  n1 = lShift(n1);
  n1 = add(n1, newDigit(7));
  Digit* n2 = newDigit(5);
  Digit* res;
  cout << "n1: ";
  show(n1);
  cout << endl;
  cout << "n2: ";
  show(n2);
  cout << endl;
  res = div(n1, n2);
  cout << "res: ";
  show(res);
  cout << endl;
  return 0;
}
